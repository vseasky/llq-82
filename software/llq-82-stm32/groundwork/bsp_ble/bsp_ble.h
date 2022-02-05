/*** 
 * @Author                         : Seasky.Liu
 * @Date                           : Do not edit
 * @LastEditTime: 2022-02-04 18:21:50
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
#ifndef _BSP_BLE_H_
#define _BSP_BLE_H_

#include "struct_typedef.h"

#define FRAME_HEAD (0XA5)
#define FRAME_LEN  (8+4)
#define FRAME_END  (0XFE)


#define UART_REPORT_ID_MOUSE 0X01
#define UART_REPORT_ID_KEY   0X02
#define UART_REPORT_ID_MEDIA 0X03
#define UART_REPORT_ID_INFO  0X04//自定义消息


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
}ble_lock_key;

typedef enum
{
    ENUM_NUM_LOCK = 0,
    ENUM_CAPS_LOCK,
    ENUM_SCROLL_LOCK,
    ENUM_COMPOSE,
    ENUM_KANA,
}ble_enum_lock;

typedef struct 
{
    uint8_t         tx_battery_cap;
    uint8_t         rx_battery_cap;
    LLQ_STATE       ble_connect;
    ble_lock_key    ble_lock;
}bsp_ble_info;


void ble_tx_buff_updata(uint8_t *tx_p,uint8_t len);
void ble_rx_buff_updata(uint8_t *rx_p, uint8_t len);
void ble_set_battery(uint8_t cap_t);
void ble_clear_connect(void);
uint8_t ble_check_connect(void);

#endif