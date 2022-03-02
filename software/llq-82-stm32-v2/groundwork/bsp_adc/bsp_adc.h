/*** 
 * @author                          : Seasky.Liu
 * @LastEditors                     : Seasky.Liu
 * @Description                     : https://github.com/SEASKY-Master
 * @FilePath                        : \keyboard_v7\groundwork\bsp_adc\bsp_adc.h
 * @symbol_custom_string_obkoro1    : 
 * @版权所有:@Seasky.liu
 * @联系方式:liuwei_seasky@163.com
 * @开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途
 * @************************************************************************
 * @           If you want a thing done well, do it yourself.
 * @************************************************************************
 */

#ifndef BSP_ADC_H
#define BSP_ADC_H


#ifdef __cplusplus
extern "C" {
#endif


#include "struct_typedef.h"

void init_vrefint_reciprocal(void);
fp32 get_temprate(void);
fp32 get_voltage(void);
fp32 get_oled_voltage(void);


#ifdef __cplusplus
}
#endif

#endif
