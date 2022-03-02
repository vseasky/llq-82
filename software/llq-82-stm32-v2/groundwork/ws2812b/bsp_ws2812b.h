/***
 * @author                          : Seasky.Liu
 * @Date                            : 2021-11-22 14:56:39
 * @LastEditTime                    : 2021-11-27 11:11:10
 * @LastEditors                     : your name
 * @Description                     : https://github.com/SEASKY-Master
 * @FilePath                        : \keyboard_v5\groundwork\ws2812b\bsp_ws2812b.h
 * @symbol_custom_string_obkoro1    :
 * @版权所有:@Seasky.liu
 * @联系方式:liuwei_seasky@163.com
 * @开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途
 * @************************************************************************
 * @           If you want a thing done well, do it yourself.
 * @************************************************************************
 */
#ifndef _BSP_WS2812B_H
#define _BSP_WS2812B_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "struct_typedef.h"

#define MAX_WS2812B_NUM 35
#define WS2812B_R_OFFSET 1
#define WS2812B_G_OFFSET 2
#define WS2812B_B_OFFSET 0

    typedef enum
    {
        L_DIN_1 = 1,
        L_DIN_2,
        L_DIN_3,
    } din_ch;
    typedef union
    {
        uint32_t WS2812B_RGB;
        uint8_t RGB_OFFSET[4];
    } ws2812b_led;
    typedef struct
    {
        uint8_t id;
        uint8_t name;
        din_ch id_ch;
        uint8_t id_ch_id;  // ch中所在id
        uint8_t id_column; //所在列
        uint8_t id_line;   //所在行
        ws2812b_led rgb_set;
    } keyboard_rgb;

    /***
     * @Author: Seasky.Liu
     * @Description: 初始化
     * @param {keyboard_rgb} *board_rgb_led
     * @return {*}
     */
    void mx_ws2812b_init(keyboard_rgb *board_rgb_led);

    /***
     * @Author: Seasky.Liu
     * @Description:    按ID更新LED
     * @param {keyboard_rgb} *board_rgb_led
     * @return {*}
     */
    void ws2812b_id_cfg(keyboard_rgb *board_rgb_led);

    /***
     * @Author: Seasky.Liu
     * @Description: 放入中断服务函数之中，保证中断函数代码足够简洁
     * @param {*}
     * @return {*}
     */
    void ws2812b_IRQ(void);

#ifdef __cplusplus
}
#endif
#endif
