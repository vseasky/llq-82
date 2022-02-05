/*
 * @Author                         : Seasky.Liu
 * @Date                           : Do not edit
 * @LastEditTime: 2022-02-05 14:15:54
 * @LastEditors: Please set LastEditors
 * @Description                    : https://github.com/SEASKY-Master
 * @FilePath                       : Do not edit
 * @symbol_custom_string_obkoro1          : 版权所有:@Seasky.liu
 * 联系方式:liuwei_seasky@163.com
 * 开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途
 * ************************************************************************
 *            If you want a thing done well, do it yourself.
 * ************************************************************************
 */

#include "touch_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "bsp_bm2166.h"

#include "bsp_password.h"
#include "keyboard_task.h"
#include "stm32f4xx_hal.h"
#include "usart.h"
#include "bsp_ble.h"
#include "keyboard_cfg.h"
#include "lvgl_app.h"

#define TOUCH_TASK_TIME 10

touch_task_info touch_task_ctr = {0};

extern bm2166_touch bm2166_touch_info;
extern lvgl_app_info lvgl_task_app_t;

static void touch_auto_send(void);
static void touch_task(void const *pvParameters);
static void touch_get_num_fun(void);
static void touch_set_num_fun(uint8_t add_num);
static uint8_t touch_add_fun(void);
static uint8_t touck_delete_fun(void);
static uint8_t touck_delect_fun(void);

static osThreadId Touch_TaskHandle;

void touch_task_creat(void)
{
    osThreadDef(touch_task_t, touch_task, osPriorityRealtime, 0, 128);
    Touch_TaskHandle = osThreadCreate(osThread(touch_task_t), NULL);
}
void touch_task_irq(void)
{
    if (touch_task_ctr.touch_status == TOUCH_IDLE)
    {
        touck_delect_fun();
    }
}


void touch_tx_ready(touch_send_ctr touch_send_p)
{
    touch_task_ctr.touch_send_ready = touch_send_p;
}
/**
 * @Author                         : Seasky.Liu
 * @Date                           : Do not edit
 * @description                    : 指纹检测成功回调，此处可以添加不同指纹对应不同的账号密码
 * @param                           {*}
 * @return                          {*}
 * @param {uint8_t} ok_t
 * @param {uint16_t} page_id_t
 * @param {uint16_t} mathscore_t
 */
void touch_delect_callback(uint8_t ok_t, uint16_t page_id_t, uint16_t mathscore_t)
{
    if (touch_task_ctr.touch_status == TOUCH_AUTO_DELECT)
    {
        touch_task_ctr.touch_delect.delect_status = ok_t;
        touch_task_ctr.touch_delect.page_id = page_id_t;
        touch_task_ctr.touch_delect.mathscore = mathscore_t;
        touch_task_ctr.touch_send_type = touch_task_ctr.touch_send_ready;
        //根据UI界面确定字符串类型
    }
}

void touch_num_callback(uint16_t num_t)
{
    touch_task_ctr.touch_num_get_ok = 0;
    touch_task_ctr.touch_num = num_t;
}
static void touch_task(void const *pvParameters)
{
    static uint16_t idle_led_count;
    TickType_t peroid_touch = osKernelSysTick();
    bm2166_init();
    bm2166_touch_info.hand_shake_s = 1;
    bm2166_touch_info.get_image_t = 0xff;
    bm2166_touch_info.genchar_t = 0xff;
    bm2166_touch_info.high_search_t.high_search_s = 0xff;
    bm2166_touch_info.press_t = 0;
    bm2166_touch_info.frame_calc_t.ack_ctr_cmd = 0;
    bm2166_touch_info.frame_calc_t.ack_rec_ok = BM_ACK_NO;
    if (0 != get_touch_power_status())
    {
        hand_shake_ask();
        osDelay(100);
        config_led_ask(0xF6, 0X0F, 0x03, 0); //打开灯效,自动灯效
                                             //	config_led_ask(0xF5, 0XFF, 0x03, 0); //关闭灯效
    }
    while (1)
    {
        //串口，只需要不发送就ok
        if (0 != get_touch_power_status())
        {
            if (bm2166_touch_info.hand_shake_s == 0)
            {
                switch (touch_task_ctr.touch_status)
                {
                case TOUCH_IDLE:
                {
                    if (idle_led_count <= 300)
                    {
                        idle_led_count++;
                    }
                    if (idle_led_count >= 250)
                    {
                        get_iamge_ask();
                    }
                    bm2166_check_err_time();
                };
                break;
                case TOUCH_GET_NUM:
                {
                    if (touch_task_ctr.touch_num_get_ok != 0)
                    {
                        validtemp_num_ask();
                    }
                    else
                    {
                        touch_task_ctr.touch_status = TOUCH_IDLE;
                        touch_task_ctr.touch_num_get_ok = 1;
                        break;
                    }
                };
                break;
                case TOUCH_AUTO_DELETE:
                {
                    if (bm2166_touch_info.empty_is_ok != 0)
                    {
                        empty_ask();
                    }
                    else
                    {
                        idle_led_count = 0;
                    }
                };
                break;
                case TOUCH_AUTO_ADD:
                {

                    if (touch_task_ctr.touch_add_count >= touch_task_ctr.touch_add_num)
                    {
                        idle_led_count = 0;
                    }
                    else
                    {
                        //自动添加指纹
                        if (add_finger(touch_task_ctr.touch_add_count) == TOUCH_ADD_OK)
                        {
                            touch_task_ctr.touch_add_count++;
                        }
                    }
                };
                break;
                case TOUCH_AUTO_DELECT:
                {
                    if ((touch_task_ctr.touch_delect.delect_status == 0) && (touch_task_ctr.touch_delect.mathscore != 0))
                    {
                        //设置显示
                        touch_task_ctr.touch_delect.delect_status = 1;
                        touch_task_ctr.touch_status = TOUCH_IDLE;
                        idle_led_count = 0;
                    }
                    else
                    {
                        auto_finger_detect();
                    }
                };
                break;
                }
            }
            else
            {
                bm2166_check_err_time();
                hand_shake_ask();
            }
        }
        else
        {
            bm2166_touch_info.hand_shake_s = 1;
            bm2166_touch_info.get_image_t = 0xff;
            bm2166_touch_info.genchar_t = 0xff;
            bm2166_touch_info.high_search_t.high_search_s = 0xff;
            bm2166_touch_info.press_t = 0;
            bm2166_touch_info.frame_calc_t.ack_rec_ok = BM_ACK_NO;
            bm2166_touch_info.frame_calc_t.ack_ctr_cmd = 0;
            bm2166_check_err_time();
        }
        touch_auto_send();
        osDelayUntil(&peroid_touch, TOUCH_TASK_TIME); //
    }
}
static void touch_auto_send(void)
{
    char touch_tx_name[MAX_PASS_WORD_LEN];
    char touch_tx_word[MAX_PASS_WORD_LEN];
    switch (touch_task_ctr.touch_send_type)
    {
    case TOUCH_SEND_NULL:
    {
    }
    break;
    case TOUCH_SEND_DEF_WORD:
    {
        memset(touch_tx_word, 0, sizeof(touch_tx_word));
        get_default_word(touch_tx_word);
        str_to_key_msg(touch_tx_word);
        touch_task_ctr.touch_send_type = TOUCH_SEND_NULL;
    }
    break;
    case TOUCH_SEND_DEF_NAME:
    {
        memset(touch_tx_name, 0, sizeof(touch_tx_name));
        get_default_name(touch_tx_name);
        str_to_key_msg(touch_tx_name);
        touch_task_ctr.touch_send_type = TOUCH_SEND_NULL;
    }
    break;
    case TOUCH_SEND_CUR_WORD:
    {
        memset(touch_tx_word, 0, sizeof(touch_tx_word));
        get_current_word(touch_tx_word);
        str_to_key_msg(touch_tx_word);
        touch_task_ctr.touch_send_type = TOUCH_SEND_NULL;
    }
    break;
    case TOUCH_SEND_CUR_NAME:
    {
        memset(touch_tx_name, 0, sizeof(touch_tx_name));
        get_current_name(touch_tx_name);
        str_to_key_msg(touch_tx_name);
        touch_task_ctr.touch_send_type = TOUCH_SEND_NULL;
    }
    break;
    default:
    {
        touch_task_ctr.touch_send_type = TOUCH_SEND_NULL;
    }
    break;
    }
}

static void touch_get_num_fun(void)
{
    if (touch_task_ctr.touch_status != TOUCH_GET_NUM)
    {
        touch_task_ctr.touch_num_get_ok = 1;
    }
    touch_task_ctr.touch_status = TOUCH_GET_NUM;
}

static void touch_set_num_fun(uint8_t add_num)
{
    if (add_num < MAX_TOUCH_NUM)
    {
        touch_task_ctr.touch_add_num = add_num;
    }
}

static uint8_t touch_add_fun(void)
{
    if (touch_task_ctr.touch_status != TOUCH_AUTO_ADD)
    {
        touch_task_ctr.touch_add_count = 0;
    }
    touch_task_ctr.touch_status = TOUCH_AUTO_ADD;
    return 0;
}

static uint8_t touck_delete_fun(void)
{

    if (touch_task_ctr.touch_status != TOUCH_AUTO_DELETE)
    {
        bm2166_touch_info.empty_is_ok = 1;
    }
    touch_task_ctr.touch_status = TOUCH_AUTO_DELETE;
    return 0;
}
static uint8_t touck_delect_fun(void)
{
    if (touch_task_ctr.touch_status != TOUCH_AUTO_DELECT)
    {
        touch_task_ctr.touch_delect.delect_status = 1;
    }
    touch_task_ctr.touch_status = TOUCH_AUTO_DELECT;
    return 0;
}
static uint8_t touch_close_fun(void)
{
}
static uint8_t touch_open_fun(void)
{
}

void touch_show_info(uint8_t level, uint8_t enc_t, uint8_t enable)
{
    switch (level)
    {
    case 0:
    {
        touch_get_num_fun();
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[0],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[1],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_setting_color, LV_SYMBOL_EYE_OPEN, "Number of fingerprints");

        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[2],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[3],
                              "%s %4s  \tNumber	:	[%d]",
                              lvgl_task_app_t.llq_style.font_select_color, " ", touch_task_ctr.touch_num);
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[4],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
    }
    break;
    case 1:
    {
        if (enable)
        {
            touch_task_ctr.touch_add_num = enc_t;
        }
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[0],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[1],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_setting_color, LV_SYMBOL_SETTINGS, "Set add number");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[2],
                              "%s %4s  \tNumber	:	[%d]",
                              lvgl_task_app_t.llq_style.font_select_color, " ", touch_task_ctr.touch_add_num);
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[3],
                              "%s %4s  \tSet	:	[%d]",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", enc_t);
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[4],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
    }
    break;
    case 2:
    {
        if (touch_task_ctr.touch_add_num > 0)
        {
            touch_add_fun();
            lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[3],
                                  "%s %4s  \tTarget     :[%d]",
                                  lvgl_task_app_t.llq_style.font_normal_color, " ", touch_task_ctr.touch_add_num);
            if (touch_task_ctr.touch_add_count >= touch_task_ctr.touch_add_num)
            {
                lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[2],
                                      "%s %4s  %s",
                                      lvgl_task_app_t.llq_style.font_normal_color, " ", "Successful entry.");
                lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[4],
                                      "%s %4s  \tAdd [%d] fingerprints.",
                                      lvgl_task_app_t.llq_style.font_select_color, " ", touch_task_ctr.touch_add_num);
            }
            else
            {
                lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[2],
                                      "%s %4s  %s",
                                      lvgl_task_app_t.llq_style.font_normal_color, " ", "Press your fingerprint!");
                lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[4],
                                      "%s %4s  \tCurrent    :[%d]!",
                                      lvgl_task_app_t.llq_style.font_select_color, " ", touch_task_ctr.touch_add_count);
            }
        }
        else
        {
            lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[3],
                                  "%s %4s  \tTarget     :[%d]",
                                  lvgl_task_app_t.llq_style.font_normal_color, " ", touch_task_ctr.touch_add_num);
            lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[2],
                                  "%s %4s  %s",
                                  lvgl_task_app_t.llq_style.font_normal_color, " ", "Number must be greater than 0.");
            lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[4],
                                  "%s %4s  \tCurrent    :[%d]!",
                                  lvgl_task_app_t.llq_style.font_select_color, " ", touch_task_ctr.touch_add_count);
        }
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[0],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[1],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_setting_color, LV_SYMBOL_SETTINGS, "Add the fingerprints");
    }
    break;
    case 3:
    {
        touck_delete_fun();
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[0],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[1],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_setting_color, LV_SYMBOL_SETTINGS, "Empty the fingerprints");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[2],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
        if (bm2166_touch_info.empty_is_ok == 0)
        {
            lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[3],
                                  "%s %4s  \tEmpty succeed.",
                                  lvgl_task_app_t.llq_style.font_select_color, " ");
        }
        else
        {
            lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[3],
                                  "%s %4s  \tEmpty failed!",
                                  lvgl_task_app_t.llq_style.font_select_color, " ");
        }
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[4],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
    }
    break;
    case 4:
    {
        touck_delect_fun();
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[0],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[1],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_setting_color, LV_SYMBOL_SETTINGS, "Delete test fun");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[2],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[3],
                              "%s %4s  \tpage_id  :\t[%d]",
                              lvgl_task_app_t.llq_style.font_select_color, " ", touch_task_ctr.touch_delect.page_id);
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[4],
                              "%s %4s  \tFraction :\t[%d]",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", touch_task_ctr.touch_delect.mathscore);
    }
    break;
    case 5:
    {
        touch_close_fun();
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[0],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[1],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_setting_color, LV_SYMBOL_SETTINGS, "Close power");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[2],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[3],
                              "%s %4s  \tNumber	:	[%d]",
                              lvgl_task_app_t.llq_style.font_select_color, " ", touch_task_ctr.touch_add_num);
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[4],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
    }
    break;
    case 6:
    {
        touch_open_fun();
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[0],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[1],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_setting_color, LV_SYMBOL_EYE_OPEN, "open power");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[2],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[3],
                              "%s %4s  Number	:	[%d]",
                              lvgl_task_app_t.llq_style.font_select_color, " ", touch_task_ctr.touch_add_num);
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[4],
                              "%s %4s  %s",
                              lvgl_task_app_t.llq_style.font_normal_color, " ", "");
    }
    break;
    }
}
