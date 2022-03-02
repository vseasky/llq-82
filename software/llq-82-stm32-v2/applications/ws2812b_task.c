/*
 * @author                          : Seasky.Liu
 * @Date                            : 2021-11-22 14:56:36
 * @LastEditTime: 2022-02-06 21:36:37
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

static void rgb_show_type1(uint8_t reset_p);
static void rgb_show_type2(uint8_t reset_p);
static void rgb_show_type3(uint8_t reset_p);
static void rgb_show_type4(uint8_t reset_p);
static void rgb_show_type5(uint8_t reset_p);

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
keyboard_rgb board_rgb[80];

static osThreadId Ws2812b_TaskHandle;
void ws2812b_task_creat(void)
{
    osThreadDef(rgbled_task, ws2812b_task, osPriorityNormal, 0, 128);
    Ws2812b_TaskHandle = osThreadCreate(osThread(rgbled_task), NULL);
}
void ws2812b_task(void const *pvParameters)
{
    static uint8_t last_style = 0;
	static uint8_t curr_style = 0;
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
    last_style = 0xff;
    while (1)
    {
        if (0 != get_rgb_power_status())
        {
#if 0            
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
#else
			curr_style = get_flash_rgb_style();
            switch (curr_style)
            {
            case 0:
                rgb_show_type1((uint8_t)(last_style != curr_style));
                break;
            case 1:
                rgb_show_type2((uint8_t)(last_style != curr_style));
                break;
            case 2:
                rgb_show_type3((uint8_t)(last_style != curr_style));
                break;
            case 3:
                rgb_show_type4((uint8_t)(last_style != curr_style));
                break;
            case 4:
                rgb_show_type5((uint8_t)(last_style != curr_style));
                break;
            case 5:
                rgb_show_type2((uint8_t)(last_style != curr_style));
                break;
            case 6:
                rgb_show_type2((uint8_t)(last_style != curr_style));
                break;
            case 7:
                rgb_show_type2((uint8_t)(last_style != curr_style));
                break;
            }
            last_style = curr_style;
#endif
        }
        else
        {
            ws2812b_data.WS2812B_RGB = 0;
            osDelay(100);
        }
    }
}

static void rgb_show_type1(uint8_t reset_p)
{
    uint8_t k;
    static uint8_t rgb_id[8] = {0};
    if (reset_p!=0)
    {
        for (k = 0; k < 8; k++)
        {
            rgb_id[k] = 0;
        }
        for (k = 0; k < 80; k++)
        {
            board_rgb[k].rgb_set.WS2812B_RGB = 0;
        }
    }
    else
    {
        for (k = 0; k < 8; k++)
        {
            if (rgb_id[k] <= 20)
            {
                rgb_id[k]++;
            }
        }
    }
    for (k = 0; k < 80; k++)
    {
        switch (board_rgb[k].id_line)
        {
        case 1:
        {
            if (board_rgb[k].id_column <= rgb_id[0] + 1)
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
                board_rgb[k].rgb_set.RGB_OFFSET[WS2812B_R_OFFSET] = 75;
            }
            else
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
            }
        }
        break;
        case 2:
        {
            if (board_rgb[k].id_column <= rgb_id[1] + 1)
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
                board_rgb[k].rgb_set.RGB_OFFSET[WS2812B_G_OFFSET] = 75;
            }
            else
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
            }
        };
        break;
        case 3:
        {
            if (board_rgb[k].id_column <= rgb_id[2] + 1)
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
                board_rgb[k].rgb_set.RGB_OFFSET[WS2812B_B_OFFSET] = 75;
            }
            else
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
            }
        };
        break;
        case 4:
        {
            if (board_rgb[k].id_column <= rgb_id[3] + 1)
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
                board_rgb[k].rgb_set.RGB_OFFSET[WS2812B_R_OFFSET] = 75;
            }
            else
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
            }
        };
        break;
        case 5:
        {
            if (board_rgb[k].id_column <= rgb_id[4] + 1)
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
                board_rgb[k].rgb_set.RGB_OFFSET[WS2812B_G_OFFSET] = 75;
            }
            else
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
            }
        };
        break;
        case 6:
        {
            if (board_rgb[k].id_column <= rgb_id[5] + 1)
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
                board_rgb[k].rgb_set.RGB_OFFSET[WS2812B_B_OFFSET] = 75;
            }
            else
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
            }
        };
        break;
        case 7:
        {

            if (board_rgb[k].id_column <= rgb_id[6] + 1)
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
                board_rgb[k].rgb_set.RGB_OFFSET[WS2812B_R_OFFSET] = 75;
                board_rgb[k].rgb_set.RGB_OFFSET[WS2812B_G_OFFSET] = 75;
            }
            else
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
            }
        };
        break;
        case 8:
        {
            if (board_rgb[k].id_column <= rgb_id[7] + 1)
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
                board_rgb[k].rgb_set.RGB_OFFSET[WS2812B_G_OFFSET] = 75;
                board_rgb[k].rgb_set.RGB_OFFSET[WS2812B_B_OFFSET] = 75;
            }
            else
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
            }
        };
        break;

        default:
            board_rgb[k].rgb_set.WS2812B_RGB = 0;
            break;
        }
    }
    ws2812b_id_cfg(board_rgb);
    osDelay(50);
}

static void rgb_show_type2(uint8_t reset_p)
{
    uint8_t k;
    static uint8_t rgb_id[8] = {0};
    static uint8_t rgb_breathe_cou = 75;
    static uint8_t rgb_breathe_dir = 0;
    if (reset_p!=0)
    {
        rgb_breathe_cou = 75;
        rgb_breathe_dir = 0;
        for (k = 0; k < 8; k++)
        {
            rgb_id[k] = 0;
        }
        for (k = 0; k < 80; k++)
        {
            board_rgb[k].rgb_set.WS2812B_RGB = 0;
        }
    }
    else
    {
        for (k = 0; k < 8; k++)
        {
            if (rgb_id[k] <= 20)
            {
                rgb_id[k]++;
            }
        }
    }
    if (rgb_breathe_dir == 0)
    {
        rgb_breathe_cou++;
        if (rgb_breathe_cou > 75)
        {
            rgb_breathe_dir = 1;
        }
    }
    else
    {
        rgb_breathe_cou--;
        if (rgb_breathe_cou <= 5)
        {
            rgb_breathe_dir = 0;
        }
    }
    for (k = 0; k < 80; k++)
    {
        switch (board_rgb[k].id_line)
        {
        case 1:
        {
            if (board_rgb[k].id_column <= rgb_id[0] + 1)
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
                board_rgb[k].rgb_set.RGB_OFFSET[WS2812B_R_OFFSET] = rgb_breathe_cou;
            }
            else
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
            }
        }
        break;
        case 2:
        {
            if (board_rgb[k].id_column <= rgb_id[1] + 1)
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
                board_rgb[k].rgb_set.RGB_OFFSET[WS2812B_G_OFFSET] = rgb_breathe_cou;
            }
            else
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
            }
        };
        break;
        case 3:
        {
            if (board_rgb[k].id_column <= rgb_id[2] + 1)
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
                board_rgb[k].rgb_set.RGB_OFFSET[WS2812B_B_OFFSET] = rgb_breathe_cou;
            }
            else
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
            }
        };
        break;
        case 4:
        {
            if (board_rgb[k].id_column <= rgb_id[3] + 1)
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
                board_rgb[k].rgb_set.RGB_OFFSET[WS2812B_R_OFFSET] = rgb_breathe_cou;
            }
            else
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
            }
        };
        break;
        case 5:
        {
            if (board_rgb[k].id_column <= rgb_id[4] + 1)
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
                board_rgb[k].rgb_set.RGB_OFFSET[WS2812B_G_OFFSET] = rgb_breathe_cou;
            }
            else
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
            }
        };
        break;
        case 6:
        {
            if (board_rgb[k].id_column <= rgb_id[5] + 1)
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
                board_rgb[k].rgb_set.RGB_OFFSET[WS2812B_B_OFFSET] = rgb_breathe_cou;
            }
            else
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
            }
        };
        break;
        case 7:
        {

            if (board_rgb[k].id_column <= rgb_id[6] + 1)
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
                board_rgb[k].rgb_set.RGB_OFFSET[WS2812B_R_OFFSET] = rgb_breathe_cou;
                board_rgb[k].rgb_set.RGB_OFFSET[WS2812B_G_OFFSET] = rgb_breathe_cou;
            }
            else
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
            }
        };
        break;
        case 8:
        {
            if (board_rgb[k].id_column <= rgb_id[7] + 1)
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
                board_rgb[k].rgb_set.RGB_OFFSET[WS2812B_G_OFFSET] = rgb_breathe_cou;
                board_rgb[k].rgb_set.RGB_OFFSET[WS2812B_B_OFFSET] = rgb_breathe_cou;
            }
            else
            {
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
            }
        };
        break;

        default:
            board_rgb[k].rgb_set.WS2812B_RGB = 0;
            break;
        }
    }
    ws2812b_id_cfg(board_rgb);
    osDelay(25);
}

static void rgb_show_color(uint8_t reset_p, uint8_t *rgb_color, uint8_t breathe_en)
{
    /*  行列配置
         1->13
         2->14
         3->14
         4->13
         5->12
         6->8
         7->4
         8->2
    */
    static uint8_t rgb_point = 0;
    static uint8_t rgb_id[8] = {13, 1, 14, 1, 12, 1, 1, 4};
    static uint8_t rgb_breathe_cou[3] = {5};
    static uint8_t rgb_breathe_dir[3] = {0};
    static ws2812b_led color_t;
    static uint8_t rgb_breathe_time = 0;
    uint8_t k = 0;
    if (reset_p!=0)
    {
        rgb_point = 0;
        rgb_id[0] = 13; //递减
        rgb_id[1] = 1;  //递增
        rgb_id[2] = 14; //递减
        rgb_id[3] = 1;  //递增
        rgb_id[4] = 12; //递减
        rgb_id[5] = 1;  //递增

        rgb_id[6] = 1; //递增
        rgb_id[7] = 2; //递减
        for (k = 0; k < 80; k++)
        {
            board_rgb[k].rgb_set.WS2812B_RGB = 0;
        }
    }
    else
    {
        switch (rgb_point)
        {
        case 0:
        {
            rgb_point = 1;
        }
        break;
        case 1:
        {
            // reg_id[0] = 13; //递减
            if (rgb_id[0] > 0)
            {
                rgb_id[0]--;
            }
            else
            {
                rgb_point = 2;
            }
        }
        break;
        case 2:
        {
            // reg_id[1] = 1;  //递增
            if (rgb_id[1] < 14)
            {
                rgb_id[1]++;
            }
            else
            {
                rgb_point = 3;
            }
        }
        break;
        case 3:
        {
            // reg_id[2] = 14; //递减
            if (rgb_id[2] > 0)
            {
                rgb_id[2]--;
            }
            else
            {
                rgb_point = 4;
            }
        }
        break;
        case 4:
        {
            // reg_id[3] = 1;  //递增
            if (rgb_id[3] < 13)
            {
                rgb_id[3]++;
            }
            else
            {
                rgb_point = 5;
            }
        }
        break;
        case 5:
        {
            //  reg_id[4] = 12; //递减
            if (rgb_id[4] > 0)
            {
                rgb_id[4]--;
            }
            else
            {
                rgb_point = 6;
            }
        }
        break;
        case 6:
        {
            // reg_id[5] = 1;  //递增
            if (rgb_id[5] < 8)
            {
                rgb_id[5]++;
            }
            else
            {
                rgb_point = 7;
            }
        }
        break;
        case 7:
        {
            // reg_id[6] = 1; //递增
            if (rgb_id[6] < 4)
            {
                rgb_id[6]++;
            }
            else
            {
                rgb_point = 8;
            }
        }
        break;
        case 8:
        {
            // reg_id[7] = 2; //递减
            if (rgb_id[7] > 0)
            {
                rgb_id[7]--;
            }
            else
            {
                rgb_point = 9;
            }
        }
        break;
        }
    }
    if (rgb_point < 9)
    {
        color_t.RGB_OFFSET[WS2812B_R_OFFSET] = rgb_color[0];
        color_t.RGB_OFFSET[WS2812B_G_OFFSET] = rgb_color[1];
        color_t.RGB_OFFSET[WS2812B_B_OFFSET] = rgb_color[2];
        for (k = 0; k < 80; k++)
        {
            switch (board_rgb[k].id_line)
            {
            case 1:
            {
                if ((board_rgb[k].id_column == rgb_id[0]) && (rgb_point == board_rgb[k].id_line))
                {
                    board_rgb[k].rgb_set.WS2812B_RGB = color_t.WS2812B_RGB;
                }
            }
            break;
            case 2:
            {
                if ((board_rgb[k].id_column == rgb_id[1]) && (rgb_point == board_rgb[k].id_line))
                {
                    board_rgb[k].rgb_set.WS2812B_RGB = color_t.WS2812B_RGB;
                }
            };
            break;
            case 3:
            {
                if ((board_rgb[k].id_column == rgb_id[2]) && (rgb_point == board_rgb[k].id_line))
                {
                    board_rgb[k].rgb_set.WS2812B_RGB = color_t.WS2812B_RGB;
                }
            };
            break;
            case 4:
            {
                if ((board_rgb[k].id_column == rgb_id[3]) && (rgb_point == board_rgb[k].id_line))
                {
                    board_rgb[k].rgb_set.WS2812B_RGB = color_t.WS2812B_RGB;
                }
            };
            break;
            case 5:
            {
                if ((board_rgb[k].id_column == rgb_id[4]) && (rgb_point == board_rgb[k].id_line))
                {
                    board_rgb[k].rgb_set.WS2812B_RGB = color_t.WS2812B_RGB;
                }
            };
            break;
            case 6:
            {
                if ((board_rgb[k].id_column == rgb_id[5]) && (rgb_point == board_rgb[k].id_line))
                {
                    board_rgb[k].rgb_set.WS2812B_RGB = color_t.WS2812B_RGB;
                }
            };
            break;
            case 7:
            {
                if ((board_rgb[k].id_column == rgb_id[6]) && (rgb_point == board_rgb[k].id_line))
                {
                    board_rgb[k].rgb_set.WS2812B_RGB = color_t.WS2812B_RGB;
                }
            };
            break;
            case 8:
            {
                if ((board_rgb[k].id_column == rgb_id[7]) && (rgb_point == board_rgb[k].id_line))
                {
                    board_rgb[k].rgb_set.WS2812B_RGB = color_t.WS2812B_RGB;
                }
            };
            break;

            default:
                board_rgb[k].rgb_set.WS2812B_RGB = 0;
                break;
            }
        }
    }
    else
    {
        if (breathe_en)
        {
            for (k = 0; k < 3; k++)
            {
                if (rgb_breathe_dir[k] == 0)
                {
                    rgb_breathe_cou[k]++;
                    if (rgb_breathe_cou[k] > rgb_color[k])
                    {
                        rgb_breathe_dir[k] = 1;
                    }
                }
                else
                {
                    rgb_breathe_cou[k]--;
                    if (rgb_breathe_cou[k] <= 0)
                    {
                        rgb_breathe_dir[k] = 0;
                    }
                }
                color_t.RGB_OFFSET[WS2812B_R_OFFSET] = rgb_breathe_cou[0];
                color_t.RGB_OFFSET[WS2812B_G_OFFSET] = rgb_breathe_cou[1];
                color_t.RGB_OFFSET[WS2812B_B_OFFSET] = rgb_breathe_cou[2];
            }
            for (k = 0; k < 80; k++)
            {
                board_rgb[k].rgb_set.WS2812B_RGB = color_t.WS2812B_RGB;
            }
        }
        else
        {
            color_t.RGB_OFFSET[WS2812B_R_OFFSET] = rgb_color[0];
            color_t.RGB_OFFSET[WS2812B_G_OFFSET] = rgb_color[1];
            color_t.RGB_OFFSET[WS2812B_B_OFFSET] = rgb_color[2];
            board_rgb[k].rgb_set.WS2812B_RGB = color_t.WS2812B_RGB;
        }
    }
    ws2812b_id_cfg(board_rgb);
    osDelay(25);
}
static void rgb_show_type3(uint8_t reset_p)
{
    static ws2812b_led color;
    uint8_t color_arr[3];
    color.WS2812B_RGB = 0;
    color.RGB_OFFSET[WS2812B_R_OFFSET] = 75;
    color_arr[0] = color.RGB_OFFSET[WS2812B_R_OFFSET];
    color_arr[1] = color.RGB_OFFSET[WS2812B_G_OFFSET];
    color_arr[2] = color.RGB_OFFSET[WS2812B_B_OFFSET];
    rgb_show_color(reset_p, color_arr, 1);
}

static void rgb_show_type4(uint8_t reset_p)
{
    static ws2812b_led color;
    uint8_t color_arr[3];
    color.WS2812B_RGB = 0;
    color.RGB_OFFSET[WS2812B_G_OFFSET] = 75;
    color_arr[0] = color.RGB_OFFSET[WS2812B_R_OFFSET];
    color_arr[1] = color.RGB_OFFSET[WS2812B_G_OFFSET];
    color_arr[2] = color.RGB_OFFSET[WS2812B_B_OFFSET];
    rgb_show_color(reset_p, color_arr, 1);
}

static void rgb_show_type5(uint8_t reset_p)
{
    static ws2812b_led color;
    uint8_t color_arr[3];
    color.WS2812B_RGB = 0;
    color.RGB_OFFSET[WS2812B_B_OFFSET] = 75;
    color_arr[0] = color.RGB_OFFSET[WS2812B_R_OFFSET];
    color_arr[1] = color.RGB_OFFSET[WS2812B_G_OFFSET];
    color_arr[2] = color.RGB_OFFSET[WS2812B_B_OFFSET];
    rgb_show_color(reset_p, color_arr, 1);
}
