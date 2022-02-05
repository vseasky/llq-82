/*
 * @author                          : Seasky.Liu
 * @Date                            : 2021-11-22 14:56:38
 * @LastEditTime: 2022-02-04 18:16:07
 * @LastEditors: Please set LastEditors
 * @Description                     : https://github.com/SEASKY-Master
 * @FilePath                        : \keyboard_v7\groundwork\bsp_usart\bsp_usart.c
 * @symbol_custom_string_obkoro1    : 
 * 版权所有:@Seasky.liu
 * 联系方式:liuwei_seasky@163.com
 * 开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途
 * ************************************************************************
 *            If you want a thing done well, do it yourself.
 * ************************************************************************
 */
#include "bsp_usart.h"
#include "bsp_bm2166.h"

uint8_t 	usart3_rx_buf[2][USART_RX_LEN];
uint16_t 	usart3_rx_pos;
uint8_t 	usart1_rx_buf[2][USART_RX_LEN];
uint16_t 	usart1_rx_pos;
extern  UART_HandleTypeDef huart1;
extern  UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;


void bsp_usart1_init(void)
{
   //使能DMA串口接收
   SET_BIT(huart1.Instance->CR3, USART_CR3_DMAR);
   __HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);     //空闲中断使能
//	__HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE); 	//接收中断使能
   __HAL_UART_CLEAR_IDLEFLAG(&huart1);       		//清除串口空闲中断标志位
   //失效DMA
   __HAL_DMA_DISABLE(&hdma_usart1_rx);

   while(hdma_usart1_rx.Instance->CR & DMA_SxCR_EN)
       {
           __HAL_DMA_DISABLE(&hdma_usart1_rx);
       }
   hdma_usart1_rx.Instance->PAR = (uint32_t) & (USART1->DR);
   //内存缓冲区1
   hdma_usart1_rx.Instance->M0AR = (uint32_t)(usart1_rx_buf[0]);
   //内存缓冲区2
   hdma_usart1_rx.Instance->M1AR = (uint32_t)(usart1_rx_buf[1]);
   //数据长度
   hdma_usart1_rx.Instance->NDTR = USART_RX_LEN;
   //使能双缓冲区
   SET_BIT(hdma_usart1_rx.Instance->CR,DMA_SxCR_DBM);
   //使能DMA
   __HAL_DMA_ENABLE(&hdma_usart1_rx);
}

void bsp_usart3_init(void)
{
    //使能DMA串口接收
    SET_BIT(huart3.Instance->CR3, USART_CR3_DMAR);
    __HAL_UART_ENABLE_IT(&huart3,UART_IT_IDLE);     //空闲中断使能
//	__HAL_UART_ENABLE_IT(&huart3,UART_IT_RXNE); 	//接收中断使能
    __HAL_UART_CLEAR_IDLEFLAG(&huart3);       		//清除串口空闲中断标志位
    //失效DMA
    __HAL_DMA_DISABLE(&hdma_usart3_rx);

    while(hdma_usart3_rx.Instance->CR & DMA_SxCR_EN)
        {
            __HAL_DMA_DISABLE(&hdma_usart3_rx);
        }
    hdma_usart3_rx.Instance->PAR = (uint32_t) & (USART3->DR);
    //内存缓冲区1
    hdma_usart3_rx.Instance->M0AR = (uint32_t)(usart3_rx_buf[0]);
    //内存缓冲区2
    hdma_usart3_rx.Instance->M1AR = (uint32_t)(usart3_rx_buf[1]);
    //数据长度
    hdma_usart3_rx.Instance->NDTR = USART_RX_LEN;
    //使能双缓冲区
    SET_BIT(hdma_usart3_rx.Instance->CR,DMA_SxCR_DBM);
    //使能DMA
    __HAL_DMA_ENABLE(&hdma_usart3_rx);
}
void USER_UART_DMA_RxCpltCallback(UART_HandleTypeDef *huart)
{
    uint16_t rx_flag;
    static uint16_t this_time_rx_len = 0;
    if(huart->Instance == huart3.Instance)
        {
            //判断是否是空闲中断
            if(huart3.Instance->SR & UART_FLAG_RXNE)//接收到数据
                {
                    __HAL_UART_CLEAR_PEFLAG(&huart3);
                }
            else if(RESET != __HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE))
                {
                    __HAL_UART_CLEAR_IDLEFLAG(&huart3);//清楚空闲中断标志（否则会一直不断进入中断）
                    if ((hdma_usart3_rx.Instance->CR & DMA_SxCR_CT) == RESET)
                        {
                            //失效DMA
                            __HAL_DMA_DISABLE(&hdma_usart3_rx);
                            //获取接收数据长度,长度 = 设定长度 - 剩余长度
                            this_time_rx_len = USART_RX_LEN - hdma_usart3_rx.Instance->NDTR;
                            //重新设定数据长度
                            hdma_usart3_rx.Instance->NDTR = USART_RX_LEN;
                            //设定缓冲区1
                            hdma_usart3_rx.Instance->CR |= (uint32_t)(DMA_SxCR_CT);
                            //使能DMA
                            __HAL_DMA_ENABLE(&hdma_usart3_rx);
                            /*计算缓冲区0*/
                            bsp_bm2166_receive(usart3_rx_buf[0],this_time_rx_len);
                        }
                    else
                        {
                            //失效DMA
                            __HAL_DMA_DISABLE(&hdma_usart3_rx);
                            //获取接收数据长度,长度 = 设定长度 - 剩余长度
                            this_time_rx_len = USART_RX_LEN - hdma_usart3_rx.Instance->NDTR;
                            //重新设定数据长度
                            hdma_usart3_rx.Instance->NDTR = USART_RX_LEN;
                            //设定缓冲区0
                            hdma_usart3_rx.Instance->CR &= ~(uint32_t)(DMA_SxCR_CT);
                            //使能DMA
                            __HAL_DMA_ENABLE(&hdma_usart3_rx);
                            /*计算缓冲区1*/
                            bsp_bm2166_receive(usart3_rx_buf[1],this_time_rx_len);
                        }
                }
        }
		else if(huart->Instance == huart1.Instance)
		{
		            //判断是否是空闲中断
           if(huart1.Instance->SR & UART_FLAG_RXNE)//接收到数据
               {
                   __HAL_UART_CLEAR_PEFLAG(&huart1);
               }
           else if(RESET != __HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE))
               {
                   __HAL_UART_CLEAR_IDLEFLAG(&huart1);//清楚空闲中断标志（否则会一直不断进入中断）
                   if ((hdma_usart3_rx.Instance->CR & DMA_SxCR_CT) == RESET)
                       {
                           //失效DMA
                           __HAL_DMA_DISABLE(&hdma_usart1_rx);
                           //获取接收数据长度,长度 = 设定长度 - 剩余长度
                           this_time_rx_len = USART_RX_LEN - hdma_usart1_rx.Instance->NDTR;
                           //重新设定数据长度
                           hdma_usart1_rx.Instance->NDTR = USART_RX_LEN;
                           //设定缓冲区1
                           hdma_usart1_rx.Instance->CR |= (uint32_t)(DMA_SxCR_CT);
                           
                           //使能DMA
                           __HAL_DMA_ENABLE(&hdma_usart1_rx);
                           /*计算缓冲区0*/
                           ble_rx_buff_updata(usart1_rx_buf[0],this_time_rx_len);
                           
                       }
                   else
                       {
                           //失效DMA
                           __HAL_DMA_DISABLE(&hdma_usart1_rx);
                           //获取接收数据长度,长度 = 设定长度 - 剩余长度
                           this_time_rx_len = USART_RX_LEN - hdma_usart1_rx.Instance->NDTR;
                           //重新设定数据长度
                           hdma_usart1_rx.Instance->NDTR = USART_RX_LEN;
                           //设定缓冲区0
                           hdma_usart1_rx.Instance->CR &= ~(uint32_t)(DMA_SxCR_CT);
                           
                           //使能DMA
                           __HAL_DMA_ENABLE(&hdma_usart1_rx);
                           /*计算缓冲区1*/
                           ble_rx_buff_updata(usart1_rx_buf[1],this_time_rx_len);
                       }
               }
		}
}