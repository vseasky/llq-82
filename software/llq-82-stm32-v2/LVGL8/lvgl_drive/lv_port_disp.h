/*** 
 * @author                          : Seasky.Liu
 * @Date                            : 2021-11-22 14:56:40
 * @LastEditTime                    : 2021-11-22 16:58:21
 * @LastEditors                     : your name
 * @Description                     : https://github.com/SEASKY-Master
 * @FilePath                        : \keyboard_v4\LVGL8\lvgl_drive\lv_port_disp.h
 * @symbol_custom_string_obkoro1    : 
 * @????:@Seasky.liu
 * @????:liuwei_seasky@163.com
 * @????:?????????????????????????????????
 * @************************************************************************
 * @           If you want a thing done well, do it yourself.
 * @************************************************************************
 */
/**
 * @file lv_port_disp_templ.h
 *
 */

 /*Copy this file as "lv_port_disp.h" and set this value to "1" to enable content*/
#if 1

#ifndef LV_PORT_DISP_TEMPL_H
#define LV_PORT_DISP_TEMPL_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/
#define SCREEN_HEIGHT 135  // ÆÁÄ»ÏñËØ¸ß¶È
#define SCREEN_WIDTH  240  // ÆÁÄ»ÏñËØ¿í¶È

void lv_port_disp_init(void);


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_PORT_DISP_TEMPL_H*/

#endif /*Disable/Enable content*/
