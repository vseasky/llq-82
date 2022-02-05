/***
 * @author                          : Seasky.Liu
 * @LastEditors                     : Seasky.Liu
 * @Description                     : https://github.com/SEASKY-Master
 * @FilePath                        : \llq-82\groundwork\struct_typedef.h
 * @symbol_custom_string_obkoro1    :
 * @版权所有:@Seasky.liu
 * @联系方式:liuwei_seasky@163.com
 * @开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途
 * @************************************************************************
 * @           If you want a thing done well, do it yourself.
 * @************************************************************************
 */
#ifndef STRUCT_TYPEDEF_H
#define STRUCT_TYPEDEF_H

#include "main.h"
#ifdef __cplusplus
extern "C"
{
#endif

    typedef signed char int8_t;
    typedef signed short int int16_t;
    typedef signed int int32_t;
    typedef signed long long int64_t;

    /* exact-width unsigned integer types */
    typedef unsigned char uint8_t;
    typedef unsigned short int uint16_t;
    typedef unsigned int uint32_t;
    typedef unsigned long long uint64_t;
    typedef unsigned char bool_t;
    typedef float fp32;
    typedef double fp64;

    typedef unsigned char u8;
    typedef unsigned short int u16;
    typedef unsigned int u32;

#ifdef __cplusplus
}
#endif

#endif
