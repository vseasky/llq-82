/*
 * @Author                         : Seasky.Liu
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-02-27 17:15:36
 * @FilePath                        : \llq-82\applications\flash_task.c
 * @Description                    : https://github.com/SEASKY-Master
 * @版权所有: @Seasky.Liu
 * @联系方式: liuwei_seasky@163.com
 * @开源协议: 请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途,违权必究
 * ************************************************************************
 * 				If you want a thing done well, do it yourself.
 * ************************************************************************
 */

#include "flash_task.h"
#include "bsp_crc16.h"
#include "bsp_flash.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "bsp_ble.h"

extern lvgl_app_info lvgl_task_app_t;
extern bsp_ble_info bsp_ble_addr;

llq_store_ctr falsh_store_ctr = {0};

static void flash_task(void const *pvParameters);
static int user_read_config(void);
static int user_save_config(void);
static void mx_sys_init(void);
static osThreadId FlashTaskHandle;

/**
 * @Author          : Seasky.Liu
 * @Description     : 初始化flash,该函数每次上电只允许执行一次
 * @param            {*}
 * @return           {*}
 */
void flash_info_init(void)
{
    static uint8_t read_flash_count = 0;
    static uint8_t read_flash_it = 0;
    uint8_t flash_i;
    memset(&falsh_store_ctr, 0, sizeof(llq_store_ctr));
    init_crc16_tab();
    falsh_store_ctr.flash_status = FLASH_NO_INIT;
    falsh_store_ctr.flash_cmd = FLASH_CMD_NO;
    falsh_store_ctr.flash_cmd = FLASH_CMD_READ;

    read_flash_it = 1;
    read_flash_count = 0;
    //最多尝试5次读取，五次读取失败，认为flash异常，需要重新写入
    while (read_flash_count < 5)
    {
        read_flash_count++;
        if (0 == user_read_config())
        {
            //读取FLASH成功
            falsh_store_ctr.flash_cmd = FLASH_CMD_READ_OK;
            read_flash_it = 0;
            break;
        }
        else
        {
            read_flash_it = 1;
        }
    }
    if (0 == read_flash_it)
    {
        //读取FLASH成功
        falsh_store_ctr.flash_cmd = FLASH_CMD_READ_OK;
    }
    else
    {
        falsh_store_ctr.flash_cmd = FLASH_CMD_READ_ERR;
        falsh_store_ctr.flash_cmd = FLASH_CMD_WRITE;
        //读取失败,写入默认参数
        set_pass_word_def(&falsh_store_ctr.llq_store_p.bsp_password);
        set_lvgl_def(&falsh_store_ctr.llq_store_p.lvgl_style);
        falsh_store_ctr.llq_store_p.llq_mcu_ctr.mcu_sleep_ctr = MCU_NORMAL_MODE;
        falsh_store_ctr.llq_store_p.llq_mcu_ctr.sleep_time = NORMAL_LCD_SHOW_TIME;
        falsh_store_ctr.llq_store_p.llq_mcu_ctr.left_dir = 0;
        falsh_store_ctr.llq_store_p.llq_mcu_ctr.right_dir = 0;
        falsh_store_ctr.llq_store_p.bsp_password.pass_word_point = 0;
        falsh_store_ctr.llq_store_p.bsp_password.pass_word_default_point = 0;
        falsh_store_ctr.llq_store_p.llq_mcu_ctr.lcd_touch_power = 0;
        falsh_store_ctr.llq_store_p.llq_mcu_ctr.rgb_power = 0;
        falsh_store_ctr.llq_store_p.llq_mcu_ctr.rgb_style = 0;
        for (flash_i = 0; flash_i < 6; flash_i++)
        {
            falsh_store_ctr.llq_store_p.esp_now_mac[flash_i] = 0X82;
        }
        for (flash_i = 0; flash_i < MAX_TOUCH_NUM; flash_i++)
        {
            falsh_store_ctr.llq_store_p.touch_point[flash_i] = 0;
        }
        if (0 == user_save_config())
        {
            falsh_store_ctr.flash_cmd = FLASH_CMD_WRITE_OK;
        }
        else
        {
            falsh_store_ctr.flash_cmd = FLASH_CMD_WRITE_ERR;
            NVIC_SystemReset();
        };
    }
    // 检查是否初始化成功
    if ((falsh_store_ctr.flash_cmd == FLASH_CMD_READ_OK) || (falsh_store_ctr.flash_cmd == FLASH_CMD_WRITE_OK))
    {
        set_lvgl_style(&falsh_store_ctr.llq_store_p.lvgl_style);
        falsh_store_ctr.flash_status = FLASH_READY;
    }
}

void flash_task_creat(void)
{
    osThreadDef(flash_task_t, flash_task, osPriorityNormal, 0, 256);
    FlashTaskHandle = osThreadCreate(osThread(flash_task_t), NULL);
}
static void power_config(void)
{
    //短暂打开lcd
    if (falsh_store_ctr.lcd_timer_enable > 0)
    {
        falsh_store_ctr.lcd_timer_count++;
        if ((falsh_store_ctr.lcd_timer_count >= MIN_LCD_SHOW_TIME) && (falsh_store_ctr.lcd_timer_count >= falsh_store_ctr.llq_store_p.llq_mcu_ctr.sleep_time))
        {
            falsh_store_ctr.lcd_timer_count = 0;
            falsh_store_ctr.lcd_timer_enable = 0;
            gui_interface_reset();
        }
        falsh_store_ctr.lcd_power = 0;
    }
    else
    {
        falsh_store_ctr.lcd_power = falsh_store_ctr.llq_store_p.llq_mcu_ctr.lcd_touch_power;
    }
    config_power(falsh_store_ctr.lcd_power,
                 falsh_store_ctr.llq_store_p.llq_mcu_ctr.rgb_power);
}
/**
 * @Author          : Seasky.Liu
 * @Description     : flash任务
 * @param            {*}
 * @return           {*}
 */
static void flash_task(void const *pvParameters)
{
    static uint8_t enter_standby_time = 0;
    //初始化flash，建议提前
    while (1)
    {
        //如果flash已经初始化过了
        if ((falsh_store_ctr.flash_status != FLASH_NO_INIT) && (falsh_store_ctr.flash_status != FLASH_ERR))
        {

            switch (falsh_store_ctr.flash_cmd)
            {
            case FLASH_CMD_NO:
            {
                // flash空闲
                falsh_store_ctr.read_count = 0;
                falsh_store_ctr.write_count = 0;
                // flash写入成功后才改变状态
                if (!check_standby_normal())
                {
                    flash_save_setting();
                    enter_standby_time++;
                    if (enter_standby_time >= 2)
                    {
                        sys_enter_stop();
                    }
                }
                else
                {
                    enter_standby_time = 0;
                }
                power_config();
            };
            break;
            case FLASH_CMD_READ:
            {
                if (0 == user_read_config())
                {
                    falsh_store_ctr.flash_cmd = FLASH_CMD_READ_OK;
                }
                else
                {
                    falsh_store_ctr.flash_cmd = FLASH_CMD_READ_ERR;
                };
            };
            break;
            case FLASH_CMD_READ_OK:
            {
                falsh_store_ctr.flash_cmd = FLASH_CMD_NO;
                falsh_store_ctr.flash_status = FLASH_READY;
            };
            break;
            case FLASH_CMD_READ_ERR:
            {
                //读取失败处理
                falsh_store_ctr.read_count++;
                //如果读取失败，尝试继续读取
                if (falsh_store_ctr.read_count <= FLASH_MAX_TRY_COUNT)
                {
                    falsh_store_ctr.flash_status = FLASH_READ;
                    falsh_store_ctr.flash_cmd = FLASH_CMD_READ;
                }
                else
                {
                    //读取失败，并且超时
                    falsh_store_ctr.read_count = 0;
                    //异常处理
                    falsh_store_ctr.flash_status = FLASH_ERR;
                }
            };
            break;
            case FLASH_CMD_WRITE:
            {
                if (0 == user_save_config())
                {
                    falsh_store_ctr.flash_cmd = FLASH_CMD_WRITE_OK;
                }
                else
                {
                    falsh_store_ctr.flash_cmd = FLASH_CMD_WRITE_ERR;
                };
            };
            break;
            case FLASH_CMD_WRITE_OK:
            {
                falsh_store_ctr.flash_cmd = FLASH_CMD_NO;
                falsh_store_ctr.flash_status = FLASH_READY;
            };
            break;
            case FLASH_CMD_WRITE_ERR:
            {
                //写入失败处理
                falsh_store_ctr.write_count++;
                //如果写入失败，尝试继续写入
                if (falsh_store_ctr.write_count <= FLASH_MAX_TRY_COUNT)
                {
                    falsh_store_ctr.flash_status = FLASH_WRITE;
                    falsh_store_ctr.flash_cmd = FLASH_CMD_WRITE;
                }
                else
                {
                    //写入失败，并且超时
                    falsh_store_ctr.write_count = 0;
                    falsh_store_ctr.flash_status = FLASH_ERR;
                }
            };
            break;
            }
        }
        osDelay(FLASH_DELAY_TIME);
    }
}

/**
 * @Author          : Seasky.Liu
 * @Description     : 读取flash信息，并进行crc校验
 * @param            {*}
 * @return           {*}
 */
static int user_read_config(void)
{
    uint16_t crc_check_t = 0;
    memcpy(&falsh_store_ctr.llq_store_p, (void *)USER_FALSH_PAGE, sizeof(llq_store));
    crc_check_t = crc_16(&falsh_store_ctr.llq_store_p, sizeof(llq_store) - 2);
    if (crc_check_t == falsh_store_ctr.llq_store_p.crc_check)
    {
        return 0;
    }
    //读取失败
    return -1;
}

/**
 * @Author          : Seasky.Liu
 * @Description     : 根据密码信息生成crc校验，并存入flash
 * @param            {*}
 * @return           {*}
 */
static int user_save_config(void)
{
    uint16_t crc_check_t = 0;
    crc_check_t = crc_16(&falsh_store_ctr.llq_store_p, sizeof(llq_store) - 2);
    falsh_store_ctr.llq_store_p.crc_check = crc_check_t;
    cali_flash_erase(USER_FALSH_PAGE, 1);
    return cali_flash_write(USER_FALSH_PAGE, &falsh_store_ctr.llq_store_p, (sizeof(llq_store) + 3) / 4);
}

/**
 * @Author          : Seasky.Liu
 * @Date            : 2022-01-18 14:08:47
 * @Description     : 更新指定密码
 * @param            {char} *name
 * @param            {char} *word
 * @param            {uint8_t} num
 * @return           {*}
 */
void updata_password_edit(char *name, char *word, uint8_t num)
{
    //更新名称
    memset(falsh_store_ctr.password_edit.user_name, 0, sizeof(falsh_store_ctr.password_edit.user_name));
    snprintf(falsh_store_ctr.password_edit.user_name, MAX_USER_NAME_LEN - STR_PROCESS, "%s", name);
    //更新密码
    memset(falsh_store_ctr.password_edit.pass_word, 0, sizeof(falsh_store_ctr.password_edit.pass_word));
    snprintf(falsh_store_ctr.password_edit.pass_word, MAX_PASS_WORD_LEN - STR_PROCESS, "%s", word);
    //更新设置的密码项
    falsh_store_ctr.password_index = num;
    //设置
    set_username(&falsh_store_ctr.llq_store_p, falsh_store_ctr.password_edit.user_name, falsh_store_ctr.password_index);
    set_password(&falsh_store_ctr.llq_store_p, falsh_store_ctr.password_edit.pass_word, falsh_store_ctr.password_index);
    //保存配置
    // user_save_config();
    //更新lvgl显示
    //如果flash状态空闲
    if ((falsh_store_ctr.flash_cmd == FLASH_CMD_NO) && (falsh_store_ctr.flash_status == FLASH_READY))
    {
        falsh_store_ctr.flash_status = FLASH_WRITE;
        falsh_store_ctr.flash_cmd = FLASH_CMD_WRITE;
    }
    return;
}

uint8_t get_flash_lcd_touch_enable(void)
{
    return falsh_store_ctr.lcd_power;
}

uint8_t get_flash_rgb_enable(void)
{
    return falsh_store_ctr.llq_store_p.llq_mcu_ctr.rgb_power;
}

void get_password_info(char *name, char *word, uint8_t num)
{
    if (num < MAX_PASS_NUM)
    {
        snprintf(name, MAX_USER_NAME_LEN - STR_PROCESS, "%s", falsh_store_ctr.llq_store_p.bsp_password.pass_word_t[num].user_name);
        snprintf(word, MAX_USER_NAME_LEN - STR_PROCESS, "%s", falsh_store_ctr.llq_store_p.bsp_password.pass_word_t[num].pass_word);
    }
}
void lcd_wake_up(void)
{
    falsh_store_ctr.lcd_timer_enable = 1;
    falsh_store_ctr.lcd_timer_count = 0;
}

void flash_set_mac(uint8_t *mac_t)
{
    falsh_store_ctr.llq_store_p.esp_now_mac[0] = mac_t[0];
    falsh_store_ctr.llq_store_p.esp_now_mac[1] = mac_t[1];
    falsh_store_ctr.llq_store_p.esp_now_mac[2] = mac_t[2];
    falsh_store_ctr.llq_store_p.esp_now_mac[3] = mac_t[3];
    falsh_store_ctr.llq_store_p.esp_now_mac[4] = mac_t[4];
    falsh_store_ctr.llq_store_p.esp_now_mac[5] = mac_t[5];
}
void flash_get_mac(uint8_t *mac_t)
{
    mac_t[0] = falsh_store_ctr.llq_store_p.esp_now_mac[0];
    mac_t[1] = falsh_store_ctr.llq_store_p.esp_now_mac[1];
    mac_t[2] = falsh_store_ctr.llq_store_p.esp_now_mac[2];
    mac_t[3] = falsh_store_ctr.llq_store_p.esp_now_mac[3];
    mac_t[4] = falsh_store_ctr.llq_store_p.esp_now_mac[4];
    mac_t[5] = falsh_store_ctr.llq_store_p.esp_now_mac[5];
}

uint8_t flash_mac_check(uint8_t *mac_t)
{
    if (falsh_store_ctr.llq_store_p.esp_now_mac[0] != mac_t[0])
    {
        return 1;
    }
    if (falsh_store_ctr.llq_store_p.esp_now_mac[1] != mac_t[1])
    {
        return 1;
    }
    if (falsh_store_ctr.llq_store_p.esp_now_mac[2] != mac_t[2])
    {
        return 1;
    }
    if (falsh_store_ctr.llq_store_p.esp_now_mac[3] != mac_t[3])
    {
        return 1;
    }
    if (falsh_store_ctr.llq_store_p.esp_now_mac[4] != mac_t[4])
    {
        return 1;
    }
    if (falsh_store_ctr.llq_store_p.esp_now_mac[5] != mac_t[5])
    {
        return 1;
    }
    return 0;
}
void get_user_info(char *username, char *password, uint8_t num)
{
    read_user_info(&falsh_store_ctr.llq_store_p.bsp_password, username, password, num);
}

void set_user_info(char *username, char *password, uint8_t num)
{
    write_user_info(&falsh_store_ctr.llq_store_p.bsp_password, username, password, num);
}

void get_user_name(char *username, uint8_t num)
{
    read_user_name(&falsh_store_ctr.llq_store_p.bsp_password, username, num);
}

void get_pass_word(char *password, uint8_t num)
{
    read_pass_word(&falsh_store_ctr.llq_store_p.bsp_password, password, num);
}

void get_current_name(char *username)
{
    read_current_name(&falsh_store_ctr.llq_store_p.bsp_password, username);
}

void get_current_word(char *password)
{
    read_current_word(&falsh_store_ctr.llq_store_p.bsp_password, password);
}

void get_default_name(char *username)
{
    read_default_name(&falsh_store_ctr.llq_store_p.bsp_password, username);
}

void get_default_word(char *password)
{
    read_default_word(&falsh_store_ctr.llq_store_p.bsp_password, password);
}

void set_tx_current(uint8_t curr_num)
{
    if (curr_num < MAX_PASS_NUM)
    {
        falsh_store_ctr.llq_store_p.bsp_password.pass_word_point = curr_num;
    }
}
void set_tx_default(uint8_t def_num)
{
    if (def_num < MAX_PASS_NUM)
    {
        falsh_store_ctr.llq_store_p.bsp_password.pass_word_default_point = def_num;
    }
}

void flash_info_lvgl_style(llq_lvgl_style_code *pstyle_code)
{
    memset(&falsh_store_ctr.llq_store_p.lvgl_style, 0, sizeof(llq_lvgl_style_code));
    memcpy(&falsh_store_ctr.llq_store_p.lvgl_style, pstyle_code, sizeof(llq_lvgl_style_code));
}

void flash_save_setting(void)
{
    falsh_store_ctr.flash_cmd = FLASH_CMD_WRITE;
}
uint8_t check_flash_save(void)
{
    if (falsh_store_ctr.flash_cmd == FLASH_CMD_WRITE)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
uint8_t check_standby_normal(void)
{
    return (falsh_store_ctr.llq_store_p.llq_mcu_ctr.mcu_sleep_ctr == MCU_NORMAL_MODE);
}

void set_standy_mode(mcu_standby standby_p)
{
    falsh_store_ctr.llq_store_p.llq_mcu_ctr.mcu_sleep_ctr = standby_p;
}
void flash_config_enter_standy(void)
{
    falsh_store_ctr.llq_store_p.llq_mcu_ctr.mcu_sleep_ctr = MCU_STOP_MODE;
    // flash_save_setting();
}
void flash_config_exit_standy(void)
{
    falsh_store_ctr.llq_store_p.llq_mcu_ctr.mcu_sleep_ctr = MCU_NORMAL_MODE;
    flash_save_setting();
}
void flash_config_save_standy(void)
{
    if (0 == user_save_config())
    {
        if (!check_standby_normal())
        {
            sys_enter_stop();
        }
    }
    else
    {
    };
}

password_ctr *password_show(void)
{
    return &falsh_store_ctr.llq_store_p.bsp_password;
}
void key_setting_show_info(uint8_t level, uint8_t enc_t, uint8_t enable)
{

    switch (level)
    {
    case 0:
    {
        // Clockwise 顺时针 - > counterclockwise
        if (enable)
        {
            falsh_store_ctr.llq_store_p.llq_mcu_ctr.left_dir = enc_t % 2;
        }
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[0],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[1],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_setting_color, LV_SYMBOL_SETTINGS, "Encoder direction[L]");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[2],
                              "%s %4s  \tCurrent:	[%d]",
                              lvgl_task_app_t.llq_style.font_select_color, " ", falsh_store_ctr.llq_store_p.llq_mcu_ctr.left_dir);
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[3],
                              "%s %4s  \tSet	:	[%d]",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", enc_t % 2);
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[4],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
    }
    break;
    case 1:
    {
        if (enable)
        {
            falsh_store_ctr.llq_store_p.llq_mcu_ctr.right_dir = enc_t % 2;
        }
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[0],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[1],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_setting_color, LV_SYMBOL_SETTINGS, "Encoder direction[R]");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[2],
                              "%s %4s  \tCurrent:	[%d]",
                              lvgl_task_app_t.llq_style.font_select_color, " ", falsh_store_ctr.llq_store_p.llq_mcu_ctr.right_dir);
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[3],
                              "%s %4s  \tSet	:	[%d]",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", enc_t % 2);
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[4],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
    }
    break;
    case 2:
    {
        if (enable)
        {
            set_tx_default(enc_t % MAX_PASS_NUM);
        }
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[0],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[1],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_setting_color, LV_SYMBOL_SETTINGS, "default_password");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[2],
                              "%s %4s  \tCurrent:	[%d]",
                              lvgl_task_app_t.llq_style.font_select_color, " ", falsh_store_ctr.llq_store_p.bsp_password.pass_word_default_point);
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[3],
                              "%s %4s  \tSet	:	[%d]",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", enc_t % MAX_PASS_NUM);
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[4],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
    }
    break;
    case 3:
    {
        if (enable)
        {
            falsh_store_ctr.llq_store_p.llq_mcu_ctr.lcd_touch_power = enc_t % 2;
        }
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[0],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[1],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_setting_color, LV_SYMBOL_SETTINGS, "lcd_touch_power");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[2],
                              "%s %4s  \tCurrent:	[%d]",
                              lvgl_task_app_t.llq_style.font_select_color, " ", falsh_store_ctr.llq_store_p.llq_mcu_ctr.lcd_touch_power);
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[3],
                              "%s %4s  \tSet	:	[%d]",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", enc_t % 2);
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[4],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
    }
    break;
    case 4:
    {
        if (enable)
        {
            falsh_store_ctr.llq_store_p.llq_mcu_ctr.rgb_power = enc_t % 2;
        }
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[0],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[1],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_setting_color, LV_SYMBOL_SETTINGS, "rgb_power");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[2],
                              "%s %4s  \tCurrent:	[%d]",
                              lvgl_task_app_t.llq_style.font_select_color, " ", falsh_store_ctr.llq_store_p.llq_mcu_ctr.rgb_power);
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[3],
                              "%s %4s  \tSet	:	[%d]",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", enc_t % 2);
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[4],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
    }
    break;
    case 5:
    {
        if (enable)
        {
            falsh_store_ctr.llq_store_p.llq_mcu_ctr.sleep_time = enc_t + 20;
            if (falsh_store_ctr.llq_store_p.llq_mcu_ctr.sleep_time < MIN_LCD_SHOW_TIME)
            {
                falsh_store_ctr.llq_store_p.llq_mcu_ctr.sleep_time = MIN_LCD_SHOW_TIME;
            }
        }
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[0],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[1],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_setting_color, LV_SYMBOL_SETTINGS, "set lcd wake time");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[2],
                              "%s %4s  \tCurrent(>20):[%d](*0.5s)",
                              lvgl_task_app_t.llq_style.font_select_color, " ", falsh_store_ctr.llq_store_p.llq_mcu_ctr.sleep_time);
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[3],
                              "%s %4s  \tSet	:	[%d]",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", enc_t + 20);
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[4],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
    }
    break;
    case 6:
    {
        if (enable)
        {
            falsh_store_ctr.llq_store_p.llq_mcu_ctr.sleep_time = enc_t;
            if (falsh_store_ctr.llq_store_p.llq_mcu_ctr.sleep_time < MIN_LCD_SHOW_TIME)
            {
                falsh_store_ctr.llq_store_p.llq_mcu_ctr.sleep_time = MIN_LCD_SHOW_TIME;
            }
        }
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[0],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "llq-82-mac");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[1],
                              "%s %4s  %02x:%02x:%02x:%02x:%02x:%02x",
                              lvgl_task_app_t.llq_style.font_normal_color, " ",
                              bsp_ble_addr.llq_mac[0],
                              bsp_ble_addr.llq_mac[1],
                              bsp_ble_addr.llq_mac[2],
                              bsp_ble_addr.llq_mac[3],
                              bsp_ble_addr.llq_mac[4],
                              bsp_ble_addr.llq_mac[5]);
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[2],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "llq-82-now");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[3],
                              "%s %4s  %02x:%02x:%02x:%02x:%02x:%02x",
                              lvgl_task_app_t.llq_style.font_normal_color, " ",
                              bsp_ble_addr.now_mac[0],
                              bsp_ble_addr.now_mac[1],
                              bsp_ble_addr.now_mac[2],
                              bsp_ble_addr.now_mac[3],
                              bsp_ble_addr.now_mac[4],
                              bsp_ble_addr.now_mac[5]);
        keyboard_buffer3_show(4);
    }
    break;
    }
}

uint8_t get_flash_rgb_style(void)
{
    static uint8_t last_style = 0;
    if (falsh_store_ctr.flash_cmd == FLASH_CMD_NO)
    {
        last_style = falsh_store_ctr.llq_store_p.llq_mcu_ctr.rgb_style;
    }
    return last_style;
}

void set_flash_rgb_style(uint8_t rgt_style)
{
    falsh_store_ctr.llq_store_p.llq_mcu_ctr.rgb_style = rgt_style;
}