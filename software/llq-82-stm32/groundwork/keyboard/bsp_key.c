/*
 * @author                          : Seasky.Liu
 * @Date                            : 2021-11-22 14:56:38
 * @LastEditTime: 2022-02-05 16:13:58
 * @LastEditors: Please set LastEditors
 * @Description                     : https://github.com/SEASKY-Master
 * @FilePath                        : \llq-82\groundwork\keyboard\bsp_key.c
 * @symbol_custom_string_obkoro1    :
 * 版权所有:@Seasky.liu
 * 联系方式:liuwei_seasky@163.com
 * 开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途
 * ************************************************************************
 *            If you want a thing done well, do it yourself.
 * ************************************************************************
 */

#include "bsp_key.h"

void key_info_get(key_info_t *key_cfg);

void key_set_default(key_info_t *key_cfg)
{
    /* key seg gpio 配置*/
    key_cfg->key_seg[0].gpio = KEY_SEG1_GPIO_Port;
    key_cfg->key_seg[0].gpio_pin = KEY_SEG1_Pin;
    key_cfg->key_seg[1].gpio = KEY_SEG2_GPIO_Port;
    key_cfg->key_seg[1].gpio_pin = KEY_SEG2_Pin;
    key_cfg->key_seg[2].gpio = KEY_SEG3_GPIO_Port;
    key_cfg->key_seg[2].gpio_pin = KEY_SEG3_Pin;
    key_cfg->key_seg[3].gpio = KEY_SEG4_GPIO_Port;
    key_cfg->key_seg[3].gpio_pin = KEY_SEG4_Pin;
    key_cfg->key_seg[4].gpio = KEY_SEG5_GPIO_Port;
    key_cfg->key_seg[4].gpio_pin = KEY_SEG5_Pin;
    key_cfg->key_seg[5].gpio = KEY_SEG6_GPIO_Port;
    key_cfg->key_seg[5].gpio_pin = KEY_SEG6_Pin;

    /* key pos gpio 配置*/
    key_cfg->key_pos[0].gpio = KEY_POS1_GPIO_Port;
    key_cfg->key_pos[0].gpio_pin = KEY_POS1_Pin;
    key_cfg->key_pos[1].gpio = KEY_POS2_GPIO_Port;
    key_cfg->key_pos[1].gpio_pin = KEY_POS2_Pin;
    key_cfg->key_pos[2].gpio = KEY_POS3_GPIO_Port;
    key_cfg->key_pos[2].gpio_pin = KEY_POS3_Pin;
    key_cfg->key_pos[3].gpio = KEY_POS4_GPIO_Port;
    key_cfg->key_pos[3].gpio_pin = KEY_POS4_Pin;
    key_cfg->key_pos[4].gpio = KEY_POS5_GPIO_Port;
    key_cfg->key_pos[4].gpio_pin = KEY_POS5_Pin;
    key_cfg->key_pos[5].gpio = KEY_POS6_GPIO_Port;
    key_cfg->key_pos[5].gpio_pin = KEY_POS6_Pin;
    key_cfg->key_pos[6].gpio = KEY_POS7_GPIO_Port;
    key_cfg->key_pos[6].gpio_pin = KEY_POS7_Pin;
    key_cfg->key_pos[7].gpio = KEY_POS8_GPIO_Port;
    key_cfg->key_pos[7].gpio_pin = KEY_POS8_Pin;
    key_cfg->key_pos[8].gpio = KEY_POS9_GPIO_Port;
    key_cfg->key_pos[8].gpio_pin = KEY_POS9_Pin;
    key_cfg->key_pos[9].gpio = KEY_POS10_GPIO_Port;
    key_cfg->key_pos[9].gpio_pin = KEY_POS10_Pin;
    key_cfg->key_pos[10].gpio = KEY_POS11_GPIO_Port;
    key_cfg->key_pos[10].gpio_pin = KEY_POS11_Pin;
    key_cfg->key_pos[11].gpio = KEY_POS12_GPIO_Port;
    key_cfg->key_pos[11].gpio_pin = KEY_POS12_Pin;
    key_cfg->key_pos[12].gpio = KEY_POS13_GPIO_Port;
    key_cfg->key_pos[12].gpio_pin = KEY_POS13_Pin;
    key_cfg->key_pos[13].gpio = KEY_POS14_GPIO_Port;
    key_cfg->key_pos[13].gpio_pin = KEY_POS14_Pin;
    /*
        对应原理图
        SEG1 0-12   ->13
        SEG2 13-26  ->14
        SEG3 27-40	->14
        SEG4 41-54	->14
        SEG5 55-67	->13
        SEG6 68-79	->12
    */

    /************************键盘ID映射************************/

    /*初始化键盘按键->向量表*/
    //	SEG1 0-12   ->13
    key_cfg->key_is_press[0].key_char = Keyboard_ESCAPE;
    key_cfg->key_is_press[1].key_char = Keyboard_F1;
    key_cfg->key_is_press[2].key_char = Keyboard_F2;
    key_cfg->key_is_press[3].key_char = Keyboard_F3;
    key_cfg->key_is_press[4].key_char = Keyboard_F4;
    key_cfg->key_is_press[5].key_char = Keyboard_F5;
    key_cfg->key_is_press[6].key_char = Keyboard_F6;
    key_cfg->key_is_press[7].key_char = Keyboard_F7;
    key_cfg->key_is_press[8].key_char = Keyboard_F8;
    key_cfg->key_is_press[9].key_char = Keyboard_F9;
    key_cfg->key_is_press[10].key_char = Keyboard_F10;
    key_cfg->key_is_press[11].key_char = Keyboard_F11;
    key_cfg->key_is_press[12].key_char = Keyboard_F12;
    //	SEG2 13-26  ->14
    key_cfg->key_is_press[13].key_char = Keyboard_BoLangXian;
    key_cfg->key_is_press[14].key_char = Keyboard_1;
    key_cfg->key_is_press[15].key_char = Keyboard_2;
    key_cfg->key_is_press[16].key_char = Keyboard_3;
    key_cfg->key_is_press[17].key_char = Keyboard_4;
    key_cfg->key_is_press[18].key_char = Keyboard_5;
    key_cfg->key_is_press[19].key_char = Keyboard_6;
    key_cfg->key_is_press[20].key_char = Keyboard_7;
    key_cfg->key_is_press[21].key_char = Keyboard_8;
    key_cfg->key_is_press[22].key_char = Keyboard_9;
    key_cfg->key_is_press[23].key_char = Keyboard_0;
    key_cfg->key_is_press[24].key_char = Keyboard_JianHao;
    key_cfg->key_is_press[25].key_char = Keyboard_DengHao;
    key_cfg->key_is_press[26].key_char = Keyboard_Backspace;
    //	SEG3 27-40	->14
    key_cfg->key_is_press[27].key_char = Keyboard_Tab;
    key_cfg->key_is_press[28].key_char = Keyboard_q;
    key_cfg->key_is_press[29].key_char = Keyboard_w;
    key_cfg->key_is_press[30].key_char = Keyboard_e;
    key_cfg->key_is_press[31].key_char = Keyboard_r;
    key_cfg->key_is_press[32].key_char = Keyboard_t;
    key_cfg->key_is_press[33].key_char = Keyboard_y;
    key_cfg->key_is_press[34].key_char = Keyboard_u;
    key_cfg->key_is_press[35].key_char = Keyboard_i;
    key_cfg->key_is_press[36].key_char = Keyboard_o;
    key_cfg->key_is_press[37].key_char = Keyboard_p;
    key_cfg->key_is_press[38].key_char = Keyboard_ZuoZhongKuoHao;
    key_cfg->key_is_press[39].key_char = Keyboard_YouZhongKuoHao;
    key_cfg->key_is_press[40].key_char = Keyboard_FanXieGang;
    //  SEG4 41-54	->14
    key_cfg->key_is_press[41].key_char = Keyboard_CapsLock;
    key_cfg->key_is_press[42].key_char = Keyboard_a;
    key_cfg->key_is_press[43].key_char = Keyboard_s;
    key_cfg->key_is_press[44].key_char = Keyboard_d;
    key_cfg->key_is_press[45].key_char = Keyboard_f;
    key_cfg->key_is_press[46].key_char = Keyboard_g;
    key_cfg->key_is_press[47].key_char = Keyboard_h;
    key_cfg->key_is_press[48].key_char = Keyboard_j;
    key_cfg->key_is_press[49].key_char = Keyboard_k;
    key_cfg->key_is_press[50].key_char = Keyboard_l;
    key_cfg->key_is_press[51].key_char = Keyboard_FenHao;
    key_cfg->key_is_press[52].key_char = Keyboard_DanYinHao;
    key_cfg->key_is_press[53].key_char = Keyboard_ENTER;
    key_cfg->key_is_press[54].key_char = Keyboard_Tab; //记得改回去
    //	SEG5 55-67	->13
    key_cfg->key_is_press[55].key_char = Keyboard_LeftShift; //特殊按键 55
    key_cfg->key_is_press[56].key_char = Keyboard_z;
    key_cfg->key_is_press[57].key_char = Keyboard_x;
    key_cfg->key_is_press[58].key_char = Keyboard_c;
    key_cfg->key_is_press[59].key_char = Keyboard_v;
    key_cfg->key_is_press[60].key_char = Keyboard_b;
    key_cfg->key_is_press[61].key_char = Keyboard_n;
    key_cfg->key_is_press[62].key_char = Keyboard_m;
    key_cfg->key_is_press[63].key_char = Keyboard_Douhao;
    key_cfg->key_is_press[64].key_char = Keyboard_JuHao;
    key_cfg->key_is_press[65].key_char = Keyboard_XieGang_WenHao;
    key_cfg->key_is_press[66].key_char = Keyboard_RightShift; //特殊按键 66
    key_cfg->key_is_press[67].key_char = Keyboard_Delete;

    key_cfg->key_is_press[68].key_char = Keyboard_LeftControl; //特殊按键 68
    key_cfg->key_is_press[69].key_char = Keyboard_LeftWindows; //特殊按键 69
    key_cfg->key_is_press[70].key_char = Keyboard_LeftAlt;     //特殊按键 70
    key_cfg->key_is_press[71].key_char = Keyboard_KongGe;
    key_cfg->key_is_press[72].key_char = Keyboard_RightAlt;     //特殊按键 72
    key_cfg->key_is_press[73].key_char = Keyboard_RightWindows; //特殊按键 73
    key_cfg->key_is_press[74].key_char = Keyboard_Application;
    key_cfg->key_is_press[75].key_char = Keyboard_RightControl; //特殊按键 75
    key_cfg->key_is_press[76].key_char = Keyboard_UpArrow;
    key_cfg->key_is_press[77].key_char = Keyboard_LeftArrow;
    key_cfg->key_is_press[78].key_char = Keyboard_DownArrow;
    key_cfg->key_is_press[79].key_char = Keyboard_RightArrow;
}

/*拉高需要使用的seg gpio电平，将其余seg gpio置底*/
void key_scan_gpio_set(key_info_t *key_cfg, uint8_t seg)
{
    uint8_t i;
    for (i = 0; i < MAX_SEG; i++)
    {
        if (i == (seg - 1))
        {
            HAL_GPIO_WritePin(key_cfg->key_seg[i].gpio, key_cfg->key_seg[i].gpio_pin, GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(key_cfg->key_seg[i].gpio, key_cfg->key_seg[i].gpio_pin, GPIO_PIN_RESET);
        }
    }
}
/*读取GPIO 电平*/
void key_scan_gpio_read(key_info_t *key_cfg, uint8_t seg)
{
    uint8_t i;
    for (i = 0; i < MAX_POS; i++)
    {
        key_cfg->key_is_press_seg[seg - 1][i] =
            HAL_GPIO_ReadPin(key_cfg->key_pos[i].gpio, key_cfg->key_pos[i].gpio_pin);
    }
}
/**
 * @Author                         : Seasky.Liu
 * @Date                           : Do not edit
 * @description                    : 获取键盘状态指示
 * @param                           {*}
 * @return                          {*}
 * @param {key_info_t} *key_cfg
 * @param {uint8_t} *rx_buff
 */
void keyboard_led_get(key_info_t *key_cfg, uint8_t *rx_buff)
{
    if (rx_buff[0] != 0)
    {
        key_cfg->keyboard_lock.num_lock = (rx_buff[1]) & 0X01;
        key_cfg->keyboard_lock.caps_lock = (rx_buff[1] >> 1) & 0X01;
        key_cfg->keyboard_lock.scroll_lock = (rx_buff[1] >> 2) & 0X01;
        rx_buff[0] = 0;
    }
}
/*将扫描得到的键值更新到对应序号*/
void key_scan_updata(key_info_t *key_cfg)
{
    uint8_t i;
    /*
        对应原理图
        SEG1 0-12   ->13
        SEG2 13-26  ->14
        SEG3 27-40	->14
        SEG4 41-54	->14
        SEG5 55-67	->13
        SEG6 68-79	->12
    */
    /**********按键扫描计数*********/
    /*SEG1 0-12   ->13*/
    for (i = 0; i < 13; i++)
    {
        if (key_cfg->key_is_press_seg[0][i] == PRESS_LEVEL)
        {
            /*按键按下计数*/
            if (key_cfg->key_is_press[i].press_it.time_count <= TIME_MAX_COUNT)
            {
                key_cfg->key_is_press[i].press_it.time_count++;
            }
        }
        else
        {
            /*复位计数*/
            if (key_cfg->key_is_press[i].press_it.rester_time_count <= TIME_RESTER_COUNT)
            {
                key_cfg->key_is_press[i].press_it.rester_time_count++;
            }
        }
    }
    /*SEG2 13-26  ->14*/
    for (i = 0; i < 14; i++)
    {
        if (key_cfg->key_is_press_seg[1][i] == PRESS_LEVEL)
        {
            /*按键按下计数*/
            if (key_cfg->key_is_press[i + 13].press_it.time_count <= TIME_MAX_COUNT)
            {
                key_cfg->key_is_press[i + 13].press_it.time_count++;
            }
        }
        else
        {
            /*复位计数*/
            if (key_cfg->key_is_press[i + 13].press_it.rester_time_count <= TIME_RESTER_COUNT)
            {
                key_cfg->key_is_press[i + 13].press_it.rester_time_count++;
            }
        }
    }
    /*SEG3 27-40	->14*/
    for (i = 0; i < 14; i++)
    {
        if (key_cfg->key_is_press_seg[2][i] == PRESS_LEVEL)
        {
            /*按键按下计数*/
            if (key_cfg->key_is_press[i + 27].press_it.time_count <= TIME_MAX_COUNT)
            {
                key_cfg->key_is_press[i + 27].press_it.time_count++;
            }
        }
        else
        {
            /*复位计数*/
            if (key_cfg->key_is_press[i + 27].press_it.rester_time_count <= TIME_RESTER_COUNT)
            {
                key_cfg->key_is_press[i + 27].press_it.rester_time_count++;
            }
        }
    }
    /*SEG4 41-54	->14*/
    for (i = 0; i < 14; i++)
    {
        if (key_cfg->key_is_press_seg[3][i] == PRESS_LEVEL)
        {
            /*按键按下计数*/
            if (key_cfg->key_is_press[i + 41].press_it.time_count <= TIME_MAX_COUNT)
            {
                key_cfg->key_is_press[i + 41].press_it.time_count++;
            }
        }
        else
        {
            /*复位计数*/
            if (key_cfg->key_is_press[i + 41].press_it.rester_time_count <= TIME_RESTER_COUNT)
            {
                key_cfg->key_is_press[i + 41].press_it.rester_time_count++;
            }
        }
    }
    /*SEG5 55-67	->13*/
    for (i = 0; i < 13; i++)
    {
        if (key_cfg->key_is_press_seg[4][i] == PRESS_LEVEL)
        {
            /*按键按下计数*/
            if (key_cfg->key_is_press[i + 55].press_it.time_count <= TIME_MAX_COUNT)
            {
                key_cfg->key_is_press[i + 55].press_it.time_count++;
            }
        }
        else
        {
            /*复位计数*/
            if (key_cfg->key_is_press[i + 55].press_it.rester_time_count <= TIME_RESTER_COUNT)
            {
                key_cfg->key_is_press[i + 55].press_it.rester_time_count++;
            }
        }
    }
    /*SEG6 68-79	->12*/
    for (i = 0; i < 12; i++)
    {
        if (key_cfg->key_is_press_seg[5][i] == PRESS_LEVEL)
        {
            /*按键按下计数*/
            if (key_cfg->key_is_press[i + 68].press_it.time_count <= TIME_MAX_COUNT)
            {
                key_cfg->key_is_press[i + 68].press_it.time_count++;
            }
        }
        else
        {
            /*复位计数*/
            if (key_cfg->key_is_press[i + 68].press_it.rester_time_count <= TIME_RESTER_COUNT)
            {
                key_cfg->key_is_press[i + 68].press_it.rester_time_count++;
            }
        }
    }
    /**********根据按键扫描计数，判断按键事件*********/
    //检测触发按键事件
    for (i = 0; i < MAX_KEY; i++)
    {
        /*按键长按*/
        if (key_cfg->key_is_press[i].press_it.time_count >= TIME_PRESS_L_COUNT)
        {
            if (key_cfg->key_is_press[i].press_it.states != KEY_PRESS_L_TIME)
            {
                key_cfg->key_is_press[i].press_it.rester_time_count = 0;
                key_cfg->key_is_press[i].press_it.states = KEY_PRESS_L_TIME;
            }
        }
        /*按键短按*/
        else if (key_cfg->key_is_press[i].press_it.time_count >= TIME_PRESS_COUNT)
        {
            if (key_cfg->key_is_press[i].press_it.states != KEY_PRESS)
            {
                key_cfg->key_is_press[i].press_it.rester_time_count = 0;
                key_cfg->key_is_press[i].press_it.states = KEY_PRESS;
            }
        }
        /*按键复位*/
        if (key_cfg->key_is_press[i].press_it.rester_time_count >= TIME_RESTER_COUNT)
        {
            if (key_cfg->key_is_press[i].press_it.states != KEY_UPSPRING)
            {
                key_cfg->key_is_press[i].press_it.time_count = 0;
                key_cfg->key_is_press[i].press_it.states = KEY_UPSPRING;
            }
        }
    }
}

void key_info_get(key_info_t *key_cfg)
{
    uint8_t i, point = 0;
    /*记录正在使用的按键*/
    static uint8_t used_it[6] = {KEY_NO_USER, KEY_NO_USER, KEY_NO_USER, KEY_NO_USER, KEY_NO_USER, KEY_NO_USER};
    static uint8_t used_key[6] = {KEY_NO_USER, KEY_NO_USER, KEY_NO_USER, KEY_NO_USER, KEY_NO_USER, KEY_NO_USER};
    /*按键松手处理*/
    for (i = 0; i < 6; i++)
    {
        /*如果此缓冲区还在使用*/
        if (used_it[i] == KEY_USER)
        {
            /*检查按键是否松手*/
            if (key_cfg->key_is_press[used_key[i]].press_it.states == KEY_UPSPRING)
            {
                /*如果已经松手，清空缓冲区，六键无冲释放*/
                used_it[i] = KEY_NO_USER;
                used_key[i] = KEY_NO_USER;
                key_cfg->keyboard.keypad[i] = 0;
                key_cfg->keyboard.keypad_status[i] = 0;
                key_cfg->keyboard.used_it[i] = 0;
            }
        }
    }

    /*特殊按键*/
    if (key_cfg->key_is_press[KEY_LEFT_ALT].press_it.states != KEY_UPSPRING)
    {
        key_cfg->keyboard.byte0_off.Left_Alt = 1;
    }
    else
    {
        key_cfg->keyboard.byte0_off.Left_Alt = 0;
        key_cfg->keyboard.byte0_off.Left_Alt_used_it = 0;
    }
    key_cfg->keyboard.byte0_off.Left_Alt_Status = key_cfg->key_is_press[KEY_LEFT_ALT].press_it.states;

    if (key_cfg->key_is_press[KEY_LEFT_CONTROL].press_it.states != KEY_UPSPRING)
    {
        key_cfg->keyboard.byte0_off.Left_Control = 1;
    }
    else
    {
        key_cfg->keyboard.byte0_off.Left_Control = 0;
        key_cfg->keyboard.byte0_off.Left_Control_used_it = 0;
    }
    key_cfg->keyboard.byte0_off.Left_Control_Status = key_cfg->key_is_press[KEY_LEFT_CONTROL].press_it.states;

    if (key_cfg->key_is_press[KEY_LEFT_WIN].press_it.states != KEY_UPSPRING)
    {
        key_cfg->keyboard.byte0_off.Left_GUI = 1;
    }
    else
    {
        key_cfg->keyboard.byte0_off.Left_GUI = 0;
        key_cfg->keyboard.byte0_off.Left_GUI_used_it = 0;
    }
    key_cfg->keyboard.byte0_off.Left_GUI_Status = key_cfg->key_is_press[KEY_LEFT_WIN].press_it.states;

    if (key_cfg->key_is_press[KEY_LEFT_SHIFT].press_it.states != KEY_UPSPRING)
    {
        key_cfg->keyboard.byte0_off.Left_Shift = 1;
    }
    else
    {
        key_cfg->keyboard.byte0_off.Left_Shift = 0;
        key_cfg->keyboard.byte0_off.Left_Shift_used_it = 0;
    }
    key_cfg->keyboard.byte0_off.Left_Shift_Status = key_cfg->key_is_press[KEY_LEFT_SHIFT].press_it.states;

    if (key_cfg->key_is_press[KEY_RIGHT_ALT].press_it.states != KEY_UPSPRING)
    {
        key_cfg->keyboard.byte0_off.Right_Alt = 1;
    }
    else
    {
        key_cfg->keyboard.byte0_off.Right_Alt = 0;
        key_cfg->keyboard.byte0_off.Right_Alt_used_it = 0;
    }
    key_cfg->keyboard.byte0_off.Right_Alt_Status = key_cfg->key_is_press[KEY_RIGHT_ALT].press_it.states;

    if (key_cfg->key_is_press[KEY_RIGHT_CONTROL].press_it.states != KEY_UPSPRING)
    {
        key_cfg->keyboard.byte0_off.Right_Control = 1;
    }
    else
    {
        key_cfg->keyboard.byte0_off.Right_Control = 0;
        key_cfg->keyboard.byte0_off.Right_Control_used_it = 0;
    }
    key_cfg->keyboard.byte0_off.Right_Control_Status = key_cfg->key_is_press[KEY_RIGHT_CONTROL].press_it.states;
    if (key_cfg->key_is_press[KEY_RIGHT_WIN].press_it.states != KEY_UPSPRING)
    {
        key_cfg->keyboard.byte0_off.Right_GUI = 1;
    }
    else
    {
        key_cfg->keyboard.byte0_off.Right_GUI = 0;
        key_cfg->keyboard.byte0_off.Right_GUI_used_it = 0;
    }
    key_cfg->keyboard.byte0_off.Right_GUI_Status = key_cfg->key_is_press[KEY_RIGHT_WIN].press_it.states;
    if (key_cfg->key_is_press[KEY_RIGHT_SHIFT].press_it.states != KEY_UPSPRING)
    {
        key_cfg->keyboard.byte0_off.Right_Shift = 1;
    }
    else
    {
        key_cfg->keyboard.byte0_off.Right_Shift = 0;
        key_cfg->keyboard.byte0_off.Right_Shift_used_it = 0;
    }
    key_cfg->keyboard.byte0_off.Right_Shift_Status = key_cfg->key_is_press[KEY_RIGHT_SHIFT].press_it.states;
    /*六键无冲按键处理*/
    for (i = 0; i < MAX_KEY; i++)
    {
        if ((i != KEY_LEFT_ALT) && (i != KEY_LEFT_CONTROL) && (i != KEY_LEFT_WIN) && (i != KEY_LEFT_SHIFT) && (i != KEY_RIGHT_ALT) && (i != KEY_RIGHT_CONTROL) && (i != KEY_RIGHT_WIN) && (i != KEY_RIGHT_SHIFT))
        {
            if (key_cfg->key_is_press[i].press_it.states != KEY_UPSPRING)
            {
                //如果该按键按下，遍历6个缓冲区，如果未使用，则使用此缓冲区
                for (point = 0; point < 6; point++)
                {
                    /*如果六个缓冲区都未包含此按键*/
                    if ((used_key[0] != i) && (used_key[1] != i) && (used_key[2] != i) && (used_key[3] != i) && (used_key[4] != i) && (used_key[5] != i))
                    {
                        if (used_it[point] == KEY_NO_USER)
                        {
                            used_it[point] = KEY_USER; //标记此按键已经使用
                            used_key[point] = i;       //记录使用此缓冲区的按键
                            key_cfg->keyboard.keypad[point] = key_cfg->key_is_press[i].key_char;
                            key_cfg->keyboard.keypad_status[point] = key_cfg->key_is_press[i].press_it.states;
                            point = 6; //如果找到未使用的缓冲区，直接退出循环
                        }
                    }
                    /*以下部分加入长短按识别*/
                    else if (used_key[0] == i)
                    {
                        key_cfg->keyboard.keypad_status[0] = key_cfg->key_is_press[i].press_it.states;
                    }
                    else if (used_key[1] == i)
                    {
                        key_cfg->keyboard.keypad_status[1] = key_cfg->key_is_press[i].press_it.states;
                    }
                    else if (used_key[2] == i)
                    {
                        key_cfg->keyboard.keypad_status[2] = key_cfg->key_is_press[i].press_it.states;
                    }
                    else if (used_key[3] == i)
                    {
                        key_cfg->keyboard.keypad_status[3] = key_cfg->key_is_press[i].press_it.states;
                    }
                    else if (used_key[4] == i)
                    {
                        key_cfg->keyboard.keypad_status[4] = key_cfg->key_is_press[i].press_it.states;
                    }
                    else if (used_key[5] == i)
                    {
                        key_cfg->keyboard.keypad_status[5] = key_cfg->key_is_press[i].press_it.states;
                    }
                }
            }
        }
    }
}


uint8_t media_updata(key_info_t *key_cfg)
{
    key_cfg->keyboard.media_off.media_msg = 
    (uint8_t)(key_cfg->keyboard.media_off.key_play<<MEDIA_PLAY)|
    (uint8_t)(key_cfg->keyboard.media_off.key_next_track<<MEDIA_NEXT_TRACK)|
    (uint8_t)(key_cfg->keyboard.media_off.key_previous_track<<MEDIA_PREVIOUS_TRACK)|
    (uint8_t)(key_cfg->keyboard.media_off.key_volume_increment<<MEDIA_VOLUME_INCREMENT)|
    (uint8_t)(key_cfg->keyboard.media_off.key_volume_decrement<<MEDIA_VOLUME_DECREMENT)|
    (uint8_t)(key_cfg->keyboard.media_off.key_mute<<MEDIA_MUTE)|
    (uint8_t)(key_cfg->keyboard.media_off.key_pause<<MEDIA_PAUSE)|
    (uint8_t)(key_cfg->keyboard.media_off.key_stop<<MEDIA_STOP);
    return key_cfg->keyboard.media_off.media_msg;
}
void key_update(uint8_t *pTxbuf, key_info_t *key_cfg)
{
    static uint8_t res_hid = 0;
    static uint16_t res_hid_l_press[6] = {0};
    uint8_t i;
    for (i = 0; i < 8; i++)
        pTxbuf[i] = 0;
    //重复，一帧有效，一帧弹起，如果不发送弹起，高频率发送无法准确控制频率
    /*特殊按键特殊处理,特殊按键不需要长短按区分，否则电脑可能有滞带键提示*/
    key_info_get(key_cfg);
    if (key_cfg->keyboard.byte0_off.Left_Control == 1)
    {
        pTxbuf[0] |= 1 << Left_Control;
        key_cfg->keyboard.byte0_off.Left_Control_used_it = 1;
    }
    if (key_cfg->keyboard.byte0_off.Left_Shift == 1)
    {
        pTxbuf[0] |= 1 << Left_Shift;
        key_cfg->keyboard.byte0_off.Left_Shift_used_it = 1;
    }
    if (key_cfg->keyboard.byte0_off.Left_Alt == 1)
    {
        pTxbuf[0] |= 1 << Left_Alt;
        key_cfg->keyboard.byte0_off.Left_Alt_used_it = 1;
    }
    if (key_cfg->keyboard.byte0_off.Left_GUI == 1)
    {
        pTxbuf[0] |= 1 << Left_GUI;
        key_cfg->keyboard.byte0_off.Left_Alt_used_it = 1;
    }
    if (key_cfg->keyboard.byte0_off.Right_Control == 1)
    {
        pTxbuf[0] |= 1 << Right_Control;
        key_cfg->keyboard.byte0_off.Left_Alt_used_it = 1;
    }
    if (key_cfg->keyboard.byte0_off.Right_Shift == 1)
    {
        pTxbuf[0] |= 1 << Right_Shift;
        key_cfg->keyboard.byte0_off.Left_Alt_used_it = 1;
    }
    if (key_cfg->keyboard.byte0_off.Right_Alt == 1)
    {
        pTxbuf[0] |= 1 << Right_Alt;
        key_cfg->keyboard.byte0_off.Left_Alt_used_it = 1;
    }
    if (key_cfg->keyboard.byte0_off.Right_GUI == 1)
    {
        pTxbuf[0] |= 1 << Right_GUI;
        key_cfg->keyboard.byte0_off.Left_Alt_used_it = 1;
    }
    pTxbuf[1] = 0;
    if (res_hid == 0)
    {
        /*BYTE2--BYTE7 普通按键 需要区分长按短按*/
        for (i = 0; i < 6; i++)
        {
            if ((key_cfg->keyboard.used_it[i] == 0) && (key_cfg->keyboard.keypad_status[i] == KEYBOARD_PRESS))
            {
                pTxbuf[i + 2] = key_cfg->keyboard.keypad[i];
                key_cfg->keyboard.used_it[i] = 1;
                res_hid_l_press[i] = 0;
            }
            if (key_cfg->keyboard.keypad_status[i] == KEYBOARD_PRESS_L_TIME)
            {
                if (res_hid_l_press[i] >= TIME_PRESS_L_HID_COUNT)
                {
                    pTxbuf[i + 2] = key_cfg->keyboard.keypad[i];
                    res_hid_l_press[i] = 0;
                }
                res_hid_l_press[i]++;
            }
        }
    }
    res_hid++;
    if (res_hid > 1)
    {
        res_hid = 0;
    }
}

/**
 * @Author          : Seasky.Liu
 * @Date            : 2022-01-19 11:14:16
 * @Description     : 无论是否需要自动设置模式，都需要引用该函数
 * @param            {key_info_t} *key_cfg
 * @param            {uint8_t} usb_connect
 * @param            {uint8_t} ble_connect
 * @return           {*}
 */
void keyboard_mode_check(key_info_t *key_cfg, uint8_t usb_connect, uint8_t ble_connect)
{
    key_hid_mode *p_key_mode;
    p_key_mode = &key_cfg->keyboard_mode;
    p_key_mode->usb_connect_status = usb_connect;
    p_key_mode->ble_connect_status = ble_connect;
    if (p_key_mode->usb_connect_status != 0)
    {
         p_key_mode->keyboard_hid_mode = LLQ_USB_HID;
    }
    else if(p_key_mode->ble_connect_status != 0)
    {
         p_key_mode->keyboard_hid_mode = LLQ_BLE_HID;
    }
    else
    {
        p_key_mode->keyboard_hid_mode = LLQ_NULL_HID;
    }
}


/**
 * @Author          : Seasky.Liu
 * @Date            : 2022-01-19 11:14:34
 * @Description     : 获取键盘模式
 * @param            {key_info_t} *key_cfg
 * @return           {*}
 */
llq_82_mode_enum keyboard_mode_get(key_info_t *key_cfg)
{
    key_hid_mode *p_key_mode;
    p_key_mode = &key_cfg->keyboard_mode;

    return p_key_mode->keyboard_hid_mode;
}

/**
 * @Author          : Seasky.Liu
 * @Date            : 2022-01-19 11:15:36
 * @Description     : 获取键盘模式
 * @param            {key_info_t} *key_cfg
 * @return           {*}
 */
llq_82_work_mode keyboard_work_get(key_info_t *key_cfg)
{
    key_hid_mode *p_key_mode;
    p_key_mode = &key_cfg->keyboard_mode;

    return p_key_mode->keyboard_work;
}

/**
 * @Author          : Seasky.Liu
 * @Date            : 2022-01-19 11:15:32
 * @Description     : 设置键盘工作内容，完成工作后需要复位到键盘模式，仅在键盘模式可以切换其他模式
 * @param            {key_info_t} *key_cfg
 * @param            {llq_82_work_mode} work_mode_t
 * @return           {*}
 */
void keyboard_work_set(key_info_t *key_cfg, llq_82_work_mode work_mode_t)
{
    key_hid_mode *p_key_mode;
    p_key_mode = &key_cfg->keyboard_mode;

    if (p_key_mode->keyboard_work == LLQ_WORK_KEYBOARD)
    {
        p_key_mode->keyboard_work = work_mode_t;
    }
}

/**
 * @Author          : Seasky.Liu
 * @Date            : 2022-01-19 11:15:25
 * @Description     : 默认键盘，短暂切换为其他模式，需要复位
 * @param            {key_info_t} *key_cfg
 * @return           {*}
 */
void keyboard_work_reset(key_info_t *key_cfg)
{
    key_hid_mode *p_key_mode;
    p_key_mode = &key_cfg->keyboard_mode;

    p_key_mode->keyboard_work = LLQ_WORK_KEYBOARD;
}

void keyboard_work_input_exit(key_info_t *key_cfg)
{
    key_hid_mode *p_key_mode;
    p_key_mode = &key_cfg->keyboard_mode;

    if (p_key_mode->keyboard_work == LLQ_WORK_KEYBOARD_STR)
    {
        p_key_mode->keyboard_work = LLQ_WORK_KEYBOARD;
    }
}
