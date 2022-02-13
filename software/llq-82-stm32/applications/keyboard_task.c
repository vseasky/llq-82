/*
 * @author                          : Seasky.Liu
 * @LastEditTime: 2022-02-06 15:36:16
 * @LastEditors: Please set LastEditors
 * @Description                     : https://github.com/SEASKY-Master
 * @FilePath                        : \llq-82\applications\keyboard_task.c
 * @symbol_custom_string_obkoro1    :
 * 版权所有:@Seasky.liu
 * 联系方式:liuwei_seasky@163.com
 * 开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途
 * ************************************************************************
 *            If you want a thing done well, do it yourself.
 * ************************************************************************
 */
#include "keyboard_task.h"
#include "usb_device.h"
#include "usbd_hid.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "keyboard_cfg.h"
#include "key_task.h"
#include "bsp_key.h"
#include "lvgl_task.h"
#include "bsp_ble.h"
#include "flash_task.h"

#define CapsLock_Num 41

extern USBD_HandleTypeDef hUsbDeviceFS;
extern uint8_t HID_Rx_Buffer[16];

key_info_t key_info;
static void keyboard_updata_char(uint8_t *data_buff, uint8_t iscaps);
static void keyboard_update_str(uint8_t *pTxbuf, uint8_t *p_tx_str);
static void keyboard_task(void const *pvParameters);
static void input_char_updata(char *char_t);
static void delete_one_char(void);
static void updata_lvgl(void);

/*字符串输出控制*/
static uint8_t str_or_key_ctr = 0;
static uint8_t key_str_msg[64] = {0};
static uint8_t key_str_shift[64] = {0};
static uint8_t key_str_len = 0;
static osThreadId keyboard_TaskHandle;

/*输入双缓冲,正在写入时，配置另一个预显示*/
char input_buffer1[MAX_USER_NAME_LEN] = "";
char input_buffer2[MAX_PASS_WORD_LEN] = "";

void keyboard_task_creat(void)
{
    osThreadDef(keyboard_task_t, keyboard_task, osPriorityRealtime, 0, 512);
    keyboard_TaskHandle = osThreadCreate(osThread(keyboard_task_t), NULL);
}

void keyboard_input_mode(void)
{
    if (LLQ_WORK_STRING != keyboard_work_get(&key_info))
    {
        keyboard_work_set(&key_info, LLQ_WORK_KEYBOARD_STR);
    }
}

void keyboard_input_exit(void)
{
    keyboard_work_input_exit(&key_info);
}

void keyboard_mode_reset(void)
{
    keyboard_work_reset(&key_info);
}

void keyboard_user_info_get(uint8_t num)
{
    get_user_info(input_buffer1, input_buffer2, num);
}

void keyboard_user_info_set(uint8_t num)
{
    set_user_info(input_buffer1, input_buffer2, num);
}

void keyboard_media_ctr(void)
{
    keyboard_work_set(&key_info, LLQ_WORK_MEDIA);
}
void keyboard_buffer_show(uint8_t options, uint8_t check)
{
    static char password_len[MAX_PASS_WORD_LEN];
    memset(password_len, 0, MAX_PASS_WORD_LEN);
    memset(password_len, '*', strlen(input_buffer2));
    lv_set_gui_input(2, input_buffer1, options, !check);
    lv_set_gui_input(4, password_len, options, check);
    key_info.keyboard_user = check;
}
static void HID_SendMouse(uint8_t *report, uint16_t len)
{
    report[0] = 0x01;
    switch (keyboard_mode_get(&key_info))
    {
    case LLQ_USB_HID:
    {
        USBD_HID_SendReport(&hUsbDeviceFS, report, len);
    }
    break;
    case LLQ_BLE_HID:
    {
        // report[0] = UART_REPORT_ID_MOUSE;
        if (0 != get_ble_power_status())
        {
            ble_tx_buff_updata(report, len);
        }
    }
    break;
    default:
        break;
    }
}

static void HID_SendKeyBoard(uint8_t *report, uint16_t len)
{
    static uint8_t i = 0;
    report[0] = 0x02;
    switch (keyboard_mode_get(&key_info))
    {
    case LLQ_USB_HID:
    {
        USBD_HID_SendReport(&hUsbDeviceFS, report, len);
    }
    break;
    case LLQ_BLE_HID:
    {
        // report[0] = UART_REPORT_ID_KEY;
        if (0 != get_ble_power_status())
        {
            ble_tx_buff_updata(report, len);
        }
    }
    break;
    default:
        break;
    }
}

static uint8_t USB_Convert_BLE_Media(void)
{
    uint8_t msg_media = 0;
    if (key_info.keyboard.media_off.key_play == 1)
    {
        msg_media = HID_CONSUMER_PLAY;
    }
    else if (key_info.keyboard.media_off.key_next_track == 1)
    {
        msg_media = HID_CONSUMER_SCAN_NEXT_TRK;
    }
    else if (key_info.keyboard.media_off.key_previous_track == 1)
    {
        msg_media = HID_CONSUMER_SCAN_PREV_TRK;
    }
    else if (key_info.keyboard.media_off.key_volume_increment == 1)
    {
        msg_media = HID_CONSUMER_VOLUME_UP;
    }
    else if (key_info.keyboard.media_off.key_volume_decrement == 1)
    {
        msg_media = HID_CONSUMER_VOLUME_DOWN;
    }
    else if (key_info.keyboard.media_off.key_mute == 1)
    {
        msg_media = HID_CONSUMER_MUTE;
    }
    else if (key_info.keyboard.media_off.key_pause == 1)
    {
        msg_media = HID_CONSUMER_PAUSE;
    }
    else if (key_info.keyboard.media_off.key_stop == 1)
    {
        msg_media = HID_CONSUMER_STOP;
    }
    else
    {
        msg_media = 0;
    }
    return msg_media;
}
static void HID_SendMedia(uint8_t *report, uint16_t len)
{
    report[0] = 0x03;
    switch (keyboard_mode_get(&key_info))
    {
    case LLQ_USB_HID:
    {
        USBD_HID_SendReport(&hUsbDeviceFS, report, len);
    }
    break;
    case LLQ_BLE_HID:
    {
        //可能需要转换
        if (0 != get_ble_power_status())
        {
            report[1] = USB_Convert_BLE_Media();
            if (report[1] != 0)
            {
                report[2] = 1;
            }
            else
            {
                report[2] = 0;
            }
            ble_tx_buff_updata(report, len);
        }
    }
    break;
    default:
        break;
    }
}
static void usb_connect_check(void)
{
    uint8_t usb_err_hid_status = 0;
	static  uint8_t ble_reset_count = 0;
    usb_err_hid_status = USBD_HID_Check_Connect(&hUsbDeviceFS);
    if (usb_err_hid_status)
    {
        //有线键盘模式
        keyboard_mode_check(&key_info, 1, 0);
        power_ble_close_fun();
        ble_clear_connect();
		ble_reset_count = 0;
    }
    else
    {
        //无线键盘模式
        keyboard_mode_check(&key_info, 0, ble_check_connect());
		if(ble_reset_count<5)
		{
			if(ble_reset_count%2)
			{
				power_ble_open_fun();
			}
			else
			{
				power_ble_close_fun();
			}
			ble_reset_count++;
		}
		else if (get_ble_power_status() == 0)
        {
            power_ble_open_fun();
        }
    }
}

static void keyboard_hid_info_updata(void)
{
    static uint8_t key_tx_buffer[9] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    static uint8_t key_tx_str = 0;
    usb_connect_check();
    switch (keyboard_work_get(&key_info))
    {
    case LLQ_WORK_KEYBOARD:
    {
        key_update(&key_tx_buffer[1], &key_info);
        //发送HID报文
        HID_SendKeyBoard(key_tx_buffer, 9); //发送报文
    };
    case LLQ_WORK_MOUSE:
    {
        keyboard_work_reset(&key_info);
    };
    break;
    case LLQ_WORK_KEYBOARD_STR:
    {
        key_update(&key_tx_buffer[1], &key_info);
        //输入转化为字符串,默认小写状态，按住shift为大写
        keyboard_updata_char(key_tx_buffer, 0);
    }
    break;
    case LLQ_WORK_MEDIA:
    {
        key_tx_buffer[1] = media_updata(&key_info);
        HID_SendMedia(key_tx_buffer, 4); //发送报文
        if (key_info.keyboard.media_off.media_msg == 0)
        {
            keyboard_work_reset(&key_info);
        }
        if (key_info.keyboard.media_off.media_msg != 0)
        {
            key_info.keyboard.media_off.key_play = 0;
            key_info.keyboard.media_off.key_next_track = 0;
            key_info.keyboard.media_off.key_previous_track = 0;
            key_info.keyboard.media_off.key_volume_increment = 0;
            key_info.keyboard.media_off.key_volume_decrement = 0;
            key_info.keyboard.media_off.key_mute = 0;
            key_info.keyboard.media_off.key_pause = 0;
            key_info.keyboard.media_off.key_stop = 0;
        }
    };
    break;
    case LLQ_WORK_STRING:
    {
        keyboard_update_str(&key_tx_buffer[1], &key_tx_str);
        //发送HID报文
        HID_SendKeyBoard(key_tx_buffer, 9); //发送报文
    };
    break;
    default:
        break;
    }
}

static void updata_lvgl(void)
{
    if ((keyboard_mode_get(&key_info) == LLQ_USB_HID))
    {
        lv_set_bel_wifi(0, 0, 1);
    }
    else if ((keyboard_mode_get(&key_info) == LLQ_BLE_HID))
    {
        lv_set_bel_wifi(1, 0, 0);
    }
    else
    {
        lv_set_bel_wifi(0, 0, 0);
    }
}
/**
 * @Author                         : Seasky.Liu
 * @Date                           : Do not edit
 * @description                    : HID消息上报，必须为一帧有效数据，一帧全部弹起,不然无法准确控制按键响应频率 3ms上报一次
 * @param                           {*}
 * @return                          {*}
 */
static void keyboard_task(void const *pvParameters)
{
    TickType_t peroid = osKernelSysTick();
    //设置键盘按键默认值
    key_set_default(&key_info);
    while (1)
    {
        /*获取键状态锁*/
        keyboard_led_get(&key_info, &HID_Rx_Buffer[0]);
        //键盘事件更新
        keyboard_hid_info_updata();
        updata_lvgl();
        osDelayUntil(&peroid, TIME_HID_DELAY);
    }
}

/**********************************************************************************************************************************************************************************/
/**********************************************************************************************************************************************************************************/

/**
 * @Author                         : Seasky.Liu
 * @description                    : 150us 进入一次 1200us完成一次全键盘扫描，按键扫描
 * @param                           {*}
 * @return                          {*}
 */
void key_scanf_irq(void)
{
    static uint8_t key_scan_seg = 1;
    if (key_scan_seg <= MAX_SEG)
    {
        key_scan_gpio_read(&key_info, key_scan_seg); //键盘扫描GPIO读取
    }
    key_scan_seg++;
    if (key_scan_seg >= 8)
    {
        // 8*75us = 600us
        //根据扫描得到的值更新对应按键（0-87）标志，长短按区分
        key_scan_updata(&key_info);
        key_scan_seg = 1;
    }
    if (key_scan_seg <= MAX_SEG)
    {
        key_scan_gpio_set(&key_info, key_scan_seg); //键盘扫描GPIO输出设置
    }
}

/**
 * @Author          : Seasky.Liu
 * @Description     : 键盘按键转化为字符信号
 * @param            {uint8_t} *data_buff
 * @param            {uint8_t} iscaps
 * @return           {*}
 */
static void keyboard_updata_char(uint8_t *data_buff, uint8_t iscaps)
{
    static char char_input[7] = "";
    /*键盘报文和字符串转换使用*/
    static const uint8_t num_shift[10] = "!@#$%^&*()";
    static const uint8_t symbol_no_shift[12] = "-=[]\\;;\'`,./";
    static const uint8_t symbol_shift[12] = "_+{}|::\"~<>\?";
    uint8_t i = 0, k = 0;
    for (i = 0; i < 6; i++)
    {
        char_input[i] = 0;
    }
    for (i = 3; i < 8; i++)
    {
        /******************shift按下**********************/
        if ((data_buff[1] >> Right_Shift != 0) | (data_buff[1] >> Left_Shift != 0))
        {
            if (data_buff[i] == Keyboard_Backspace)
            {
                delete_one_char();
            }
            else if (data_buff[i] == Keyboard_ENTER)
            {
                char_input[k] = '\n';
                k++;
            }
            else if ((data_buff[i] >= Keyboard_a) && (data_buff[i] <= Keyboard_z))
            {
                if (iscaps == 1)
                {
                    char_input[k] = 'a' + data_buff[i] - Keyboard_a;
                    k++;
                }
                else
                {
                    char_input[k] = 'A' + data_buff[i] - Keyboard_a;
                    k++;
                }
            }
            else if ((data_buff[i] >= Keyboard_1) && (data_buff[i] <= Keyboard_0))
            {
                char_input[k] = num_shift[data_buff[i] - Keyboard_1];
                k++;
            }
            else if ((data_buff[i] >= Keyboard_JianHao) && (data_buff[i] <= Keyboard_XieGang_WenHao))
            {
                char_input[k] = symbol_shift[data_buff[i] - Keyboard_JianHao];
                k++;
            }
        }
        else
        /******************shift弹起**********************/
        {
            if (data_buff[i] == Keyboard_Backspace)
            {
                delete_one_char();
            }
            else if (data_buff[i] == Keyboard_ENTER)
            {
                char_input[k] = '\n';
                k++;
            }
            else if ((data_buff[i] >= Keyboard_a) && (data_buff[i] <= Keyboard_z))
            {
                if (iscaps == 0)
                {
                    char_input[k] = 'a' + data_buff[i] - Keyboard_a;
                    k++;
                }
                else
                {
                    char_input[k] = 'A' + data_buff[i] - Keyboard_a;
                    k++;
                }
            }
            else if ((data_buff[i] >= Keyboard_1) && (data_buff[i] <= Keyboard_9))
            {
                char_input[k] = '1' + data_buff[i] - Keyboard_1;
                k++;
            }
            else if ((data_buff[i] == Keyboard_0))
            {
                char_input[k] = '0';
                k++;
            }
            else if ((data_buff[i] >= Keyboard_JianHao) && (data_buff[i] <= Keyboard_XieGang_WenHao))
            {
                char_input[k] = symbol_no_shift[data_buff[i] - Keyboard_JianHao];
                k++;
            }
        }
    }
    input_char_updata(char_input);
}

/**
 * @Author: Seasky.Liu
 * @Description: 字符串方式输出键值,配合key_update_str完成发送
 * @param {char} *str
 * @return {*}
 */
void str_to_key_msg(char *str)
{
    uint16_t i = 0;
    uint8_t msg_caps_lock = 0;
    // iscaps标识当前是大写还是小写状态，无线功能暂时无法区分,
    //感兴趣可以列为数组查询方式，比较直接
    key_str_len = strlen(str);
    //设置为字符串输出给PC
    key_info.keyboard_mode.keyboard_work = LLQ_WORK_STRING;
    msg_caps_lock = key_info.keyboard_lock.caps_lock;
    for (i = 0; i < key_str_len; i++)
    {
        if ((str[i] >= 'A') & (str[i] <= 'Z'))
        {
            key_str_msg[i] = Keyboard_a + str[i] - 'A';
            key_str_shift[i] = !msg_caps_lock;
        }
        else if ((str[i] >= 'a') & (str[i] <= 'z'))
        {
            key_str_msg[i] = Keyboard_a + str[i] - 'a';
            key_str_shift[i] = msg_caps_lock;
        }
        else if (str[i] == '0')
        {
            key_str_msg[i] = Keyboard_0;
            key_str_shift[i] = 0;
        }
        else if ((str[i] >= '1') & (str[i] <= '9'))
        {
            key_str_msg[i] = Keyboard_1 + str[i] - '1';
            key_str_shift[i] = 0;
        }
        switch (str[i])
        {
        case '`':
        {
            key_str_msg[i] = Keyboard_BoLangXian;
            key_str_shift[i] = 0;
        };
        break;
        case '-':
        {
            key_str_msg[i] = Keyboard_JianHao;
            key_str_shift[i] = 0;
        };
        break;
        case '=':
        {
            key_str_msg[i] = Keyboard_DengHao;
            key_str_shift[i] = 0;
        };
        break;
        case '[':
        {
            key_str_msg[i] = Keyboard_ZuoZhongKuoHao;
            key_str_shift[i] = 0;
        };
        break;
        case ']':
        {
            key_str_msg[i] = Keyboard_YouZhongKuoHao;
            key_str_shift[i] = 0;
        };
        break;
        case '\\':
        {
            key_str_msg[i] = Keyboard_FanXieGang;
            key_str_shift[i] = 0;
        };
        break;
        case ';':
        {
            key_str_msg[i] = Keyboard_FenHao;
            key_str_shift[i] = 0;
        };
        break;
        case '\'':
        {
            key_str_msg[i] = Keyboard_DanYinHao;
            key_str_shift[i] = 0;
        };
        break;
        case ',':
        {
            key_str_msg[i] = Keyboard_Douhao;
            key_str_shift[i] = 0;
        };
        break;
        case '.':
        {
            key_str_msg[i] = Keyboard_JuHao;
            key_str_shift[i] = 0;
        };
        break;
        case '/':
        {
            key_str_msg[i] = Keyboard_XieGang_WenHao;
            key_str_shift[i] = 0;
        };
        break;
        case '\n':
        {
            key_str_msg[i] = Keyboard_ENTER;
            key_str_shift[i] = 0;
        };
        break;
        case '~':
        {
            key_str_msg[i] = Keyboard_BoLangXian;
            key_str_shift[i] = 1;
        };
        break;
        case '!':
        {
            key_str_msg[i] = Keyboard_1;
            key_str_shift[i] = 1;
        };
        break;
        case '@':
        {
            key_str_msg[i] = Keyboard_2;
            key_str_shift[i] = 1;
        };
        break;
        case '#':
        {
            key_str_msg[i] = Keyboard_3;
            key_str_shift[i] = 1;
        };
        break;
        case '$':
        {
            key_str_msg[i] = Keyboard_4;
            key_str_shift[i] = 1;
        };
        break;
        case '%':
        {
            key_str_msg[i] = Keyboard_5;
            key_str_shift[i] = 1;
        };
        break;
        case '^':
        {
            key_str_msg[i] = Keyboard_6;
            key_str_shift[i] = 1;
        };
        break;
        case '&':
        {
            key_str_msg[i] = Keyboard_7;
            key_str_shift[i] = 1;
        };
        break;
        case '*':
        {
            key_str_msg[i] = Keyboard_8;
            key_str_shift[i] = 1;
        };
        break;
        case '(':
        {
            key_str_msg[i] = Keyboard_9;
            key_str_shift[i] = 1;
        };
        break;
        case ')':
        {
            key_str_msg[i] = Keyboard_0;
            key_str_shift[i] = 1;
        };
        break;
        case '_':
        {
            key_str_msg[i] = Keyboard_JianHao;
            key_str_shift[i] = 1;
        };
        break;
        case '+':
        {
            key_str_msg[i] = Keyboard_DengHao;
            key_str_shift[i] = 1;
        };
        break;
        case '{':
        {
            key_str_msg[i] = Keyboard_ZuoZhongKuoHao;
            key_str_shift[i] = 1;
        };
        break;
        case '}':
        {
            key_str_msg[i] = Keyboard_YouZhongKuoHao;
            key_str_shift[i] = 1;
        };
        break;
        case '|':
        {
            key_str_msg[i] = Keyboard_FanXieGang;
            key_str_shift[i] = 1;
        };
        break;
        case ':':
        {
            key_str_msg[i] = Keyboard_FenHao;
            key_str_shift[i] = 1;
        };
        break;
        case '\"':
        {
            key_str_msg[i] = Keyboard_DanYinHao;
            key_str_shift[i] = 1;
        };
        break;
        case '<':
        {
            key_str_msg[i] = Keyboard_Douhao;
            key_str_shift[i] = 1;
        };
        break;
        case '>':
        {
            key_str_msg[i] = Keyboard_JuHao;
            key_str_shift[i] = 1;
        };
        break;
        case '?':
        {
            key_str_msg[i] = Keyboard_XieGang_WenHao;
            key_str_shift[i] = 1;
        };
        break;
        }
    }
}

/**
 * @Author          : Seasky.Liu
 * @Description     : 键盘输入转str
 * @param            {uint8_t} *pTxbuf
 * @param            {uint8_t} *p_tx_str
 * @return           {*}
 */
static void keyboard_update_str(uint8_t *pTxbuf, uint8_t *p_tx_str)
{
    uint8_t i;
    static key_unpress = 0;
    //带自动发送控制
    for (i = 0; i < 8; i++)
        pTxbuf[i] = 0;
    //发送按下
    if (key_unpress == 0)
    {
        pTxbuf[0] = key_str_shift[*p_tx_str] << Left_Shift;
        //因为要区分大小写等，不使用六个
        pTxbuf[2] = key_str_msg[*p_tx_str];
        (*p_tx_str)++;
        key_unpress = 1;
    }
    else
    {
        //判断发送完成
        if ((*p_tx_str) >= key_str_len)
        {
            //字符串键值上报完成
            memset(key_str_shift, 0, sizeof(key_str_shift));
            memset(key_str_msg, 0, sizeof(key_str_msg));
            key_str_len = 0;
            *p_tx_str = 0;
            //恢复键盘输入
            keyboard_work_reset(&key_info);
        }
        key_unpress = 0;
    }
}

/**
 * @Author          : Seasky.Liu
 * @Date            : 2022-01-19 10:39:58
 * @Description     : 本地字符串输入
 * @param            {char} *char_t
 * @return           {*}
 */
static void input_char_updata(char *char_t)
{
    switch (key_info.keyboard_user)
    {
    case LLQ_INPUT_BUFFER1:
    {
        //密码名称输入长度限制
        if (strlen(input_buffer1) <= MAX_USER_NAME_LEN)
        {
            strcat(input_buffer1, char_t);
        }
    }
    break;
    case LLQ_INPUT_BUFFER2:
    {
        //密码输入长度限制
        if (strlen(input_buffer2) <= MAX_PASS_WORD_LEN)
        {
            strcat(input_buffer2, char_t);
        }
    }
    break;
    }
    return;
}
static void delete_one_char(void)
{
    uint8_t length = 0;
    switch (key_info.keyboard_user)
    {
    case LLQ_INPUT_BUFFER1:
    {
        length = strlen(input_buffer1);
        if (length > 0)
        {
            input_buffer1[length - 1] = '\0';
        }
    };
    break;
    case LLQ_INPUT_BUFFER2:
    {
        length = strlen(input_buffer2);
        if (length > 0)
        {
            input_buffer2[length - 1] = '\0';
        }
    };
    break;
    }
    return;
}

void media_play(void)
{
    key_info.keyboard.media_off.key_play = 1;
    keyboard_work_set(&key_info, LLQ_WORK_MEDIA);
}
void media_next_track(void)
{
    key_info.keyboard.media_off.key_next_track = 1;
    keyboard_work_set(&key_info, LLQ_WORK_MEDIA);
}
void media_previous_track(void)
{
    key_info.keyboard.media_off.key_previous_track = 1;
    keyboard_work_set(&key_info, LLQ_WORK_MEDIA);
}

void media_volume_increment(void)
{
    key_info.keyboard.media_off.key_volume_increment = 1;
    keyboard_work_set(&key_info, LLQ_WORK_MEDIA);
}
void media_volume_decrement(void)
{
    key_info.keyboard.media_off.key_volume_decrement = 1;
    keyboard_work_set(&key_info, LLQ_WORK_MEDIA);
}
void media_mute(void)
{
    key_info.keyboard.media_off.key_mute = 1;
    keyboard_work_set(&key_info, LLQ_WORK_MEDIA);
}
void media_paus(void)
{
    key_info.keyboard.media_off.key_pause = 1;
    keyboard_work_set(&key_info, LLQ_WORK_MEDIA);
}
void media_stop(void)
{
    key_info.keyboard.media_off.key_stop = 1;
    keyboard_work_set(&key_info, LLQ_WORK_MEDIA);
}
