/***
 * @Author                         : Seasky.Liu
 * @Date                           : Do not edit
 * @LastEditTime: 2022-02-05 11:18:07
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

#ifndef _BSP_INTERFACE_H
#define _BSP_INTERFACE_H

#include "struct_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        void (*power_ble_config)(uint8_t state);
        void (*power_lcd_config)(uint8_t state);
        void (*power_led_config)(uint8_t state);
        void (*bsp_init)(void);
        void (*power_ble_open)(void);
        void (*power_ble_close)(void);
        void (*power_lcd_open)(void);
        void (*power_lcd_close)(void);
        void (*power_led_open)(void);
        void (*power_led_close)(void);
        void (*power_all_open)(void);
        void (*power_all_close)(void);
        void (*power_enter_stop)(void);
    } bsp_interface_struct;

    void bsp_interface_init(void);
    void power_all_open_fun(void);
    void power_all_close_fun(void);
    void sys_enter_stop(void);
    void config_power(uint8_t lcd_p, uint8_t rgb_p);

    void power_ble_open_fun(void);
    void power_ble_close_fun(void);
    uint8_t get_ble_power_status(void);
    uint8_t get_lcd_power_status(void);
    uint8_t get_touch_power_status(void);
    uint8_t get_rgb_power_status(void);

#ifdef __cplusplus
}
#endif

#endif
