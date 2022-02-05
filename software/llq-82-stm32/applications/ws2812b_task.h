/*** 
 * @author                          : Seasky.Liu
 * @LastEditors                     : Seasky.Liu
 * @Description                     : https://github.com/SEASKY-Master
 * @FilePath                        : \keyboard_v7\applications\ws2812b_task.h
 * @symbol_custom_string_obkoro1    : 
 * @版权所有:@Seasky.liu
 * @联系方式:liuwei_seasky@163.com
 * @开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途
 * @************************************************************************
 * @           If you want a thing done well, do it yourself.
 * @************************************************************************
 */

#ifndef _WS2812B_TASK_H
#define _WS2812B_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tim.h"
#include "struct_typedef.h"
#include "bsp_ws2812b.h"

typedef struct
{
	ws2812b_led mx_ws2812b_one[27];
}keyboard_led;
void ws2812b_task_creat(void);


#ifdef __cplusplus
}
#endif

#endif
