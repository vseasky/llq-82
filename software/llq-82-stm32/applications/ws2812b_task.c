/*
 * @author                          : Seasky.Liu
 * @Date                            : 2021-11-22 14:56:36
 * @LastEditTime: 2022-02-05 15:27:58
 * @LastEditors: Please set LastEditors
 * @Description                     : https://github.com/SEASKY-Master
 * @FilePath                        : \llq-82\applications\ws2812b_task.c
 * @symbol_custom_string_obkoro1    :
 * 版权所有:@Seasky.liu
 * 联系方式:liuwei_seasky@163.com
 * 开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途
 * ************************************************************************
 *            If you want a thing done well, do it yourself.
 * ************************************************************************
 */

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "ws2812b_task.h"

#define RGB_FLOW_COLOR_CHANGE_TIME 1000
#define RGB_FLOW_COLOR_LENGHT 6

static void ws2812b_task(void const *pvParameters);

// blue-> green(dark)-> red -> blue(dark) -> green(dark) -> red(dark) -> blue
//蓝 -> 绿(灭) -> 红 -> 蓝(灭) -> 绿 -> 红(灭) -> 蓝
uint32_t RGB_flow_color[RGB_FLOW_COLOR_LENGHT + 1] = {0xFF0000FF, 0x0000FF00, 0xFFFF0000, 0x000000FF, 0xFF00FF00, 0x00FF0000, 0xFF0000FF};

ws2812b_led ws2812b_data;
/*
1.2us 			-> 	0.833MHz
需要配置小于 	->	0.83MHz
TIM4-APB1 		->	42MHz->84MHz
PSC（7-1） 		->  12MHz
ARR (96-1)   	-> 	0.25MHz ->8us
ARR (1200-1)  	-> 	0.01MHz ->100us
*/
fp32 delta_alpha, delta_red, delta_green, delta_blue;
fp32 alpha, red, green, blue;
//键盘RGB灯信息
keyboard_rgb board_rgb[87];
static osThreadId Ws2812b_TaskHandle;
void ws2812b_task_creat(void)
{
    osThreadDef(rgbled_task, ws2812b_task, osPriorityNormal, 0, 128);
    Ws2812b_TaskHandle = osThreadCreate(osThread(rgbled_task), NULL);
}
void ws2812b_task(void const *pvParameters)
{
    __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_UPDATE);
    HAL_TIM_Base_Start(&htim4);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    ws2812b_data.RGB_OFFSET[WS2812B_R_OFFSET] = 0;
    ws2812b_data.RGB_OFFSET[WS2812B_G_OFFSET] = 0;
    ws2812b_data.RGB_OFFSET[WS2812B_B_OFFSET] = 0;
    uint8_t k;
    uint16_t i, j;
    uint32_t aRGB;
    mx_ws2812b_init(board_rgb);
    while (1)
    {
        if (0 != get_rgb_power_status())
        {
            for (i = 0; i < RGB_FLOW_COLOR_LENGHT; i++)
            {

                red = ((RGB_flow_color[i] & 0x00FF0000) >> 16);
                green = ((RGB_flow_color[i] & 0x0000FF00) >> 8);
                blue = ((RGB_flow_color[i] & 0x000000FF) >> 0);

                delta_red = (fp32)((RGB_flow_color[i + 1] & 0x00FF0000) >> 16) - (fp32)((RGB_flow_color[i] & 0x00FF0000) >> 16);
                delta_green = (fp32)((RGB_flow_color[i + 1] & 0x0000FF00) >> 8) - (fp32)((RGB_flow_color[i] & 0x0000FF00) >> 8);
                delta_blue = (fp32)((RGB_flow_color[i + 1] & 0x000000FF) >> 0) - (fp32)((RGB_flow_color[i] & 0x000000FF) >> 0);

                delta_red /= RGB_FLOW_COLOR_CHANGE_TIME;
                delta_green /= RGB_FLOW_COLOR_CHANGE_TIME;
                delta_blue /= RGB_FLOW_COLOR_CHANGE_TIME;
                for (j = 0; j < RGB_FLOW_COLOR_CHANGE_TIME; j++)
                {
                    red += delta_red;
                    green += delta_green;
                    blue += delta_blue;
                    aRGB = ((uint32_t)(red)) << 16 | ((uint32_t)(green)) << 8 | ((uint32_t)(blue)) << 0;
                    ws2812b_data.RGB_OFFSET[WS2812B_R_OFFSET] = red / 5;
                    ws2812b_data.RGB_OFFSET[WS2812B_G_OFFSET] = green / 5;
                    ws2812b_data.RGB_OFFSET[WS2812B_B_OFFSET] = blue / 5;
                    /*实际效果可以精确控制指定LED*/
                    for (k = 0; k < 87; k++)
                    {
                        board_rgb[k].rgb_set.WS2812B_RGB = ws2812b_data.WS2812B_RGB;
                    }
                    ws2812b_id_cfg(board_rgb);
                    osDelay(2);
                }
            }
        }
        else
        {
            ws2812b_data.WS2812B_RGB = 0;
            osDelay(100);
        }
    }
}


