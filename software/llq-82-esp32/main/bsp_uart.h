/*** 
 * @Author                         : Seasky.Liu
 * @Date                           : Do not edit
 * @LastEditTime: 2022-01-17 21:35:49
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
#ifndef _BSP_UART_H
#define _BSP_UART_H

#include "hal/uart_hal.h"

/*定一个合适的波特率*/
#define UART_BAUD_RATE 256000
//延迟=固有延迟(波特率*数据长度)+中断响应延迟+空中延迟，
//ESP32必须使用中断方式，同时就算使用中断，串口中断响应也比STM32略差

#define FRAME_HEAD (0XA5)
#define FRAME_LEN  (8+4)
#define FRAME_END  (0XFE)

#define UART_REPORT_ID_MOUSE 0X01
#define UART_REPORT_ID_KEY   0X02
#define UART_REPORT_ID_MEDIA 0X03
#define UART_REPORT_ID_INFO  0X04

typedef enum
{
    LLQ_RESET = 0 ,
    LLQ_DISCONNECT,
    LLQ_CONNECT,
    LLQ_ERR,
}LLQ_STATE;

typedef struct
{
	uint8_t num_lock:1;
	uint8_t caps_lock:1;
	uint8_t scroll_lock:1;
	uint8_t compose:1;
	uint8_t kana:1;
}lock_key;

typedef enum
{
    ENUM_NUM_LOCK = 0,
    ENUM_CAPS_LOCK,
    ENUM_SCROLL_LOCK,
    ENUM_COMPOSE,
    ENUM_KANA,
}enum_lock;

typedef struct 
{
    LLQ_STATE llq_status;   //链接状态
    uint8_t   battery_cap;  //电池容量
    lock_key  keyboard_lock;//键盘lock指示灯
}llq_struct_t;


void mx_uart_init(void);
void llq_status_config(LLQ_STATE set_llq_status);
void llq_status_updata(void);
#endif