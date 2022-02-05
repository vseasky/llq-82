/*** 
 * @Author                         : Seasky.Liu
 * @Date                           : Do not edit
 * @LastEditTime: 2022-02-05 16:33:20
 * @LastEditors: Please set LastEditors
 * @Description                    : https://github.com/SEASKY-Master
 * @FilePath                       : Do not edit
 * @symbol_custom_string_obkoro1          : 版权所有:@Seasky.liu
 * @联系方式:liuwei_seasky@163.com
 * @开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途
 * @************************************************************************
 * @           If you want a thing done well, do it yourself.
 * @************************************************************************
 */
/***
 * @Author                          : Seasky.Liu
 * @LastEditors                     : Seasky.Liu
 * @FilePath                        : \keyboard_v7\applications\key_task.h
 * @Description                     : https://github.com/SEASKY-Master
 * @symbol_custom_string_obkoro1    :
 * @版权所有: @Seasky.Liu
 * @联系方式: liuwei_seasky@163.com
 * @开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途,违权必究
 * @************************************************************************
 * @				If you want a thing done well, do it yourself.
 * @************************************************************************
 */

#ifndef _KEY_TSAK_H
#define _KEY_TSAK_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "struct_typedef.h"

#define MAX_KEY_NUM 2 //按键个数
#define KEY_TASK_TIME_CYCLE 2     //单位ms
#define KEY_TASK_TIME_P_COUNT 10  // TIME_P_TIME/TIME_CYCLE	20ms
#define KEY_TASK_TIME_L_COUNT 750 // TIME_L_TIME/TIME_CYCLE	1500ms

#define KEY_PRESS_MAX_COUNT 2000 //按下最长计时	4s
#define KEY_RES_MAX_COUNT 2000   //复位最长计时	4s
#define KEY_RES_COUNT 6          //按键复位弹起计数时间		12ms
#define KEY_RES_NUM_COUNT 250    //按键计数停止计数时间		500ms
#define KEY_MAX_NUM 100

#define KEY_TASK_PRESS_S 1    //按键按下返回信号
#define KEY_TASK_UPSPRING_S 0 //按键弹起返回信号

#define KEY_TASK_PRESS_LEVEL 0 //定义按键按下时的电平

    typedef enum
    {
        KEY_TASK_UPSPRING = 0,     //按键弹起
        KEY_TASK_PRESS,        //按键按下
        KEY_TASK_PRESS_L_TIME, //按键长按
    } key_task_states;
    typedef struct
    {
        key_task_states states;  //按键状态
        uint8_t click_state_num; //连续短按次数
        uint16_t time_count;     //按键按下时间计数
        uint16_t res_count;      //按键弹起时间计数
        uint8_t click_num;       //按键状态为短按计数
    } key_task_time_info;
    typedef struct
    {
        GPIO_TypeDef *key_gpio;      //定义的按键GPIO
        uint16_t key_gpio_pin;       //定义的按键GPIO
        key_task_time_info key_info; //按键算法信息
    } mx_key_task_info;
    typedef struct
    {
        mx_key_task_info key_t[2];
        struct
        {
            uint16_t enc_t;
            uint8_t  dir;
        } encoder_t[2];
        uint8_t  click_switch;
        uint8_t (*get_click_state_num)(mx_key_task_info *click_state_t);
    } enc_key;

    void key_task_creat(void);
    void key_scanf_for_standby(void);
    void key_enc_init(void);
    void key_enc_scanf(void);
    uint8_t key_get_press_count(uint8_t enc_t);


#ifdef __cplusplus
}
#endif

#endif
