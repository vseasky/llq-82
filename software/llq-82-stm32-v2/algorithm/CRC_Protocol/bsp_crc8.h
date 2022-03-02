/*** 
 * @author                          : Seasky.Liu
 * @Date                            : 2021-11-22 14:56:36
 * @LastEditTime                    : 2021-11-22 16:57:37
 * @LastEditors                     : your name
 * @Description                     : https://github.com/SEASKY-Master
 * @FilePath                        : \keyboard_v4\algorithm\CRC_Protocol\bsp_crc8.h
 * @symbol_custom_string_obkoro1    : 
 * @版权所有:@Seasky.liu
 * @联系方式:liuwei_seasky@163.com
 * @开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途
 * @************************************************************************
 * @           If you want a thing done well, do it yourself.
 * @************************************************************************
 */
#ifndef _BSP_CRC8_H
#define _BSP_CRC8_H

#include "main.h"


#define	CRC_START_8	0x00

uint8_t crc_8( const uint8_t *input_str,uint16_t num_bytes);
uint8_t update_crc_8(uint8_t crc, uint8_t val );

#endif
