/*
 * @author                          : Seasky.Liu
 * @Date                            : 2021-11-22 14:56:39
 * @LastEditTime: 2022-02-06 19:15:36
 * @LastEditors: Please set LastEditors
 * @Description                     : https://github.com/SEASKY-Master
 * @FilePath                        : \llq-82\groundwork\ws2812b\bsp_ws2812b.c
 * @symbol_custom_string_obkoro1    :
 * 版权所有:@Seasky.liu
 * 联系方式:liuwei_seasky@163.com
 * 开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途
 * ************************************************************************
 *            If you want a thing done well, do it yourself.
 * ************************************************************************
 */
#include "bsp_ws2812b.h"
#include "tim.h"
/*
    PB6     ------> TIM4_CH1  1-27--------27
    PB7     ------> TIM4_CH2  1-27--------27
    PB8     ------> TIM4_CH3  28-54-------27
    PB9     ------> TIM4_CH4  55-87-------33
*/
static void mx_ws2812b_low(uint8_t ch);
static void mx_ws2812b_high(uint8_t ch);
static void mx_ws2812b_reset(void);
/*IRQ中使用暂存，IRQ中不能包含过多代码，故封装显示*/
ws2812b_led ws2812b_irq_cfg[3][MAX_WS2812B_NUM];

/**
 * @Author: Seasky.Liu
 * @Description: 初始化ws2812
 * @param {keyboard_rgb} *board_rgb_led
 * @return {*}
 */
void mx_ws2812b_init(keyboard_rgb *board_rgb_led)
{
    uint8_t i;
    for (i = 0; i < 80; i++)
    {
        /*
            PB7     ------> TIM4_CH2 ------> L_DIN_1 ------> 1-27--------28
            PB8     ------> TIM4_CH3 ------> L_DIN_2 ------> 28-55-------27
            PB9     ------> TIM4_CH4 ------> L_DIN_3 ------> 56-80-------33
        */
        board_rgb_led[i].id = i + 1;
        /*通道配置*/
        if (board_rgb_led[i].id <= 27)
        {
            board_rgb_led[i].id_ch = L_DIN_1;
            board_rgb_led[i].id_ch_id = i + 1;
        }
        else if (board_rgb_led[i].id <= 55)
        {
            board_rgb_led[i].id_ch = L_DIN_2;
            board_rgb_led[i].id_ch_id = i + 1 - 27;
        }
        else if (board_rgb_led[i].id <= 80)
        {
            board_rgb_led[i].id_ch = L_DIN_3;
            board_rgb_led[i].id_ch_id = i + 1 - 55;
        }

        /*  行列配置
                1->13
                2->14
                3->14
                4->13
                5->12
                6->8
                7->4
                8->2
        */
        if (board_rgb_led[i].id <= 13)          //13
        {
            board_rgb_led[i].id_line = 1;       //所在行
            board_rgb_led[i].id_column = i + 1; //所在列
        }
        else if (board_rgb_led[i].id <= 27)     //14
        {
            board_rgb_led[i].id_line = 2;            //所在行
            board_rgb_led[i].id_column = i + 1 - 13; //所在列
        }
        else if (board_rgb_led[i].id <= 41)     //14
        {
            board_rgb_led[i].id_line = 3;        //所在行
            board_rgb_led[i].id_column = i - 26; //所在列
        }
        else if (board_rgb_led[i].id <= 54)     //13
        {
            board_rgb_led[i].id_line = 4;        //所在行
            board_rgb_led[i].id_column = i - 40; //所在列
        }
        else if (board_rgb_led[i].id == 55)     //2
        {
            //特殊
            board_rgb_led[i].id_line = 8; //所在行
            board_rgb_led[i].id_column = 1;
        }
        else if (board_rgb_led[i].id <= 67)     //12
        {
            board_rgb_led[i].id_line = 5;        //所在行
            board_rgb_led[i].id_column = i - 54; //所在列
        }
        else if (board_rgb_led[i].id == 68)     //2
        {
            //特殊
            board_rgb_led[i].id_line = 8; //所在行
            board_rgb_led[i].id_column = 2;
        }
        else if (board_rgb_led[i].id <= 76)     //8
        {
            board_rgb_led[i].id_line = 6;        //所在行
            board_rgb_led[i].id_column = i - 67; //所在列
        }
        else if (board_rgb_led[i].id <= 80)     //4
        {
            board_rgb_led[i].id_line = 7;        //所在行
            board_rgb_led[i].id_column = i - 75; //所在列
        }
    }
}

/*根据id分配到3个显示通道*/
void mx_ws2812b_config(uint8_t ch, uint8_t set)
{
    TIM4->ARR = 95;
    switch (ch)
    {
    case 1:
        TIM4->CCR1 = set;
        break;
    case 2:
        TIM4->CCR2 = set;
        break;
    case 3:
        TIM4->CCR3 = set;
        break;
    case 4:
        break;
    }
}
/*按ID更新*/
void ws2812b_id_cfg(keyboard_rgb *board_rgb_led)
{
    uint8_t i;
    for (i = 0; i < 80; i++)
    {
        /*
            PB7     ------> TIM4_CH2 ------> L_DIN_1 ------> 1-27--------28
            PB8     ------> TIM4_CH3 ------> L_DIN_2 ------> 28-55-------27
            PB9     ------> TIM4_CH4 ------> L_DIN_3 ------> 56-80-------33
        */
        switch (board_rgb_led[i].id_ch)
        {
        case L_DIN_1:
            ws2812b_irq_cfg[0][i].WS2812B_RGB = board_rgb_led[i].rgb_set.WS2812B_RGB;
            break;
        case L_DIN_2:
            ws2812b_irq_cfg[1][i - 27].WS2812B_RGB = board_rgb_led[i].rgb_set.WS2812B_RGB;
            break;
        case L_DIN_3:
            ws2812b_irq_cfg[2][i - 55].WS2812B_RGB = board_rgb_led[i].rgb_set.WS2812B_RGB;
            break;
        }
    }
}
/*将此函数放入定时器中断中，定时器中断中须有必要的判断以及中断清除操作，
不建议直接使用中断回调函数*/
void ws2812b_IRQ(void)
{
    static uint16_t ws2812b_num = 0;
    static uint8_t ws2812b_size = 0;
    if (ws2812b_num == MAX_WS2812B_NUM)
    {
        ws2812b_num = 0;
        mx_ws2812b_reset();
    }
    else
    {
        if ((ws2812b_irq_cfg[0][ws2812b_num].WS2812B_RGB >> (23 - ws2812b_size) & 0x01) == 1)
        {
            mx_ws2812b_high(1);
        }
        else
        {
            mx_ws2812b_low(1);
        }
        if ((ws2812b_irq_cfg[1][ws2812b_num].WS2812B_RGB >> (23 - ws2812b_size) & 0x01) == 1)
        {
            mx_ws2812b_high(2);
        }
        else
        {
            mx_ws2812b_low(2);
        }
        if ((ws2812b_irq_cfg[2][ws2812b_num].WS2812B_RGB >> (23 - ws2812b_size) & 0x01) == 1)
        {
            mx_ws2812b_high(3);
        }
        else
        {
            mx_ws2812b_low(3);
        }
        ws2812b_size++;
        if (ws2812b_size == 24)
        {
            ws2812b_num++;
            ws2812b_size = 0;
        }
    }
}

/*pwm输出 0码*/
static void mx_ws2812b_low(uint8_t ch)
{
    mx_ws2812b_config(ch, 4);
}
/*pwm输出 1码*/
static void mx_ws2812b_high(uint8_t ch)
{
    mx_ws2812b_config(ch, 9);
}
/*发送完所有字节后输出PWM复位码*/
static void mx_ws2812b_reset(void)
{
    TIM4->ARR = 1199;
    TIM4->CCR1 = 0;
    TIM4->CCR2 = 0;
    TIM4->CCR3 = 0;
    TIM4->CCR4 = 0;
}