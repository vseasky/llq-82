/*
 * @Author                         : Seasky.Liu
 * @Date                           : Do not edit
 * @LastEditTime: 2022-02-04 18:22:59
 * @LastEditors: Please set LastEditors
 * @Description                    : https://github.com/SEASKY-Master
 * @FilePath                       : Do not edit
 * @symbol_custom_string_obkoro1          : 版权所有:@Seasky.liu
 * 联系方式:liuwei_seasky@163.com
 * 开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途
 * ************************************************************************
 *            If you want a thing done well, do it yourself.
 * ************************************************************************
 */
#include "bsp_ble.h"
#include "usart.h"

bsp_ble_info bsp_ble_addr;
uint8_t ble_tx_buff[12];
void ble_tx_buff_updata(uint8_t *tx_p, uint8_t len)
{
	uint8_t i = 0;
	ble_tx_buff[0] = FRAME_HEAD;
	ble_tx_buff[1] = tx_p[0];
	ble_tx_buff[2] = bsp_ble_addr.tx_battery_cap;
	if (len <= 9)
	{
		for (i = 1; i < len; i++)
		{
			ble_tx_buff[2 + i] = tx_p[i];
		}
	}
	ble_tx_buff[11] = FRAME_END;
	HAL_UART_Transmit_DMA(&huart1, ble_tx_buff, 12);
}

void ble_rx_buff_updata(uint8_t *rx_p, uint8_t len)
{
	if (len == FRAME_LEN)
	{
		if ((rx_p[0] == FRAME_HEAD) && (rx_p[11] == FRAME_END))
		{
			if (rx_p[1] == UART_REPORT_ID_INFO)
			{
				bsp_ble_addr.rx_battery_cap = rx_p[2];
				bsp_ble_addr.ble_connect = rx_p[3];
				bsp_ble_addr.ble_lock.num_lock = (rx_p[4]>>ENUM_NUM_LOCK)&0x01;
				bsp_ble_addr.ble_lock.caps_lock = (rx_p[4]>>ENUM_CAPS_LOCK)&0x01;
				bsp_ble_addr.ble_lock.scroll_lock = (rx_p[4]>>ENUM_SCROLL_LOCK)&0x01;
				bsp_ble_addr.ble_lock.compose =(rx_p[4]>>ENUM_COMPOSE)&0x01;
				bsp_ble_addr.ble_lock.kana =(rx_p[4]>>ENUM_KANA)&0x01;
			}
		}
	}
}

uint8_t ble_check_connect(void)
{
	if(bsp_ble_addr.ble_connect == LLQ_CONNECT)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
void ble_clear_connect(void)
{
	bsp_ble_addr.ble_connect = LLQ_RESET;
}
void ble_set_battery(uint8_t cap_t)
{
	bsp_ble_addr.tx_battery_cap = cap_t;
}