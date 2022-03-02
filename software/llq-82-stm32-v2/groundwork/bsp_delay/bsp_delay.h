/*** 
 * @author                          : Seasky.Liu
 * @Date                            : 2021-11-22 14:56:38
 * @LastEditTime                    : 2022-01-04 15:00
 * @LastEditors                     : your name
 * @Description                     : https://github.com/SEASKY-Master
 * @FilePath                        : \keyboard_v6\groundwork\bsp_delay\bsp_delay.h
 * @symbol_custom_string_obkoro1    : 
 * @版权所有:@Seasky.liu
 * @联系方式:liuwei_seasky@163.com
 * @开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途
 * @************************************************************************
 * @           If you want a thing done well, do it yourself.
 * @************************************************************************
 */

#ifndef _BSP_DELAY_H
#define _BSP_DELAY_H


#ifdef __cplusplus
extern "C" {
#endif

#include "struct_typedef.h"

void delay_init(void);
void delay_us(uint16_t nus);
void delay_ms(uint16_t nms);


#ifdef __cplusplus
}
#endif


#endif
