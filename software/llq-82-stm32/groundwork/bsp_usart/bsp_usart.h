/*** 
 * @author                          : Seasky.Liu
 * @Date                            : 2021-11-22 14:56:38
 * @LastEditTime                    : 2022-01-04 14:54
 * @LastEditors                     : your name
 * @Description                     : https://github.com/SEASKY-Master
 * @FilePath                        : \keyboard_v6\groundwork\bsp_usart\bsp_usart.h
 * @symbol_custom_string_obkoro1    : 
 * @版权所有:@Seasky.liu
 * @联系方式:liuwei_seasky@163.com
 * @开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途
 * @************************************************************************
 * @           If you want a thing done well, do it yourself.
 * @************************************************************************
 */
#ifndef _BSP_USART_H
#define _BSP_USART_H
#ifdef __cplusplus
extern "C" {
#endif

#include "struct_typedef.h"

#define USART_RX_LEN 200

/*** 
 * @Author: Seasky.Liu
 * @Description: 初始化串口DMA配置
 * @param {*}
 * @return {*}
 */
void bsp_usart1_init(void);
void bsp_usart3_init(void);
void USER_UART_DMA_RxCpltCallback(UART_HandleTypeDef *huart);


#ifdef __cplusplus
}
#endif
#endif
