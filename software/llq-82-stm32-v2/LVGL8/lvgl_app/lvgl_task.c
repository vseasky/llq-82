/*
 * @author                          : Seasky.Liu
 * @Date                            : 2021-11-22 14:56:40
 * @LastEditTime: 2022-01-29 15:22:29
 * @LastEditors: Please set LastEditors
 * @Description                     : https://github.com/SEASKY-Master
 * @FilePath                        : \keyboard_v4\LVGL8\lvgl_app\lvgl_task.c
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
#include "main.h"
#include "cmsis_os.h"

#include "lvgl_task.h"
#include "lvgl.h"
#include "lv_port_disp.h"

#include "key_task.h"
#include "flash_task.h"

#define LVGL_TICK 25
#define LVGL_TASK_TIME 100

extern enc_key key_enc_info;
extern lvgl_app_info lvgl_task_app_t;

void lvgl_main_win(void);
// static void lv_ex_label(void);
static void lvgl_task(void const *pvParameters);
static void lvgl_tick_task(void const *pvParameters);
static void lvgl_user_task(void const *pvParameters);
static uint8_t lcd_init_ok = 0;
static osThreadId lvgl_task_handle;
static osThreadId lvgl_tick_task_handle;
static osThreadId lvgl_tick_user_handle;

void lvgl_task_creat(void)
{
    /*创建线程*/
    osThreadDef(lvgl_task_t, lvgl_task, osPriorityNormal, 0, 1024);
    lvgl_task_handle = osThreadCreate(osThread(lvgl_task_t), NULL);
    osThreadDef(lvgl_tick_task_t, lvgl_tick_task, osPriorityNormal, 0, 512);
    lvgl_tick_task_handle = osThreadCreate(osThread(lvgl_tick_task_t), NULL);
    //用户任务
    osThreadDef(lvgl_user_task_t, lvgl_user_task, osPriorityRealtime, 0, 512);
    lvgl_tick_user_handle = osThreadCreate(osThread(lvgl_user_task_t), NULL);
}

static void lvgl_user_task(void const *pvParameters)
{
    TickType_t peroid = osKernelSysTick();
    bsp_lcd_init(); // LCD初始化
    lv_init();
    lv_port_disp_init();
    lvgl_app_init(&lvgl_task_app_t);
    lcd_init_ok = 1;
    while (1)
    {
        if (0 != get_lcd_power_status())
        {
            if (lcd_init_ok == 1)
            {
                bsp_lcd_init(); //LCD初始化
                lcd_init_ok = 0;
            }
            else
            {
                interface_main();
                lv_main_lable_updata();
            }
        }
        else
        {
            lcd_init_ok = 1;
        }
        osDelayUntil(&peroid, LVGL_TASK_TIME);
    }
}

/***************************lvgl***************************/
static void lvgl_tick_task(void const *pvParameters)
{
    while (1)
    {
        if (0 != get_lcd_power_status())
        {
            if (lcd_init_ok == 0)
            {
                lv_tick_inc(LVGL_TICK);
            }
        }
        osDelay(LVGL_TICK);
    }
}
static void lvgl_task(void const *pvParameters)
{
    while (1)
    {
        if (0 != get_lcd_power_status())
        {
            if (lcd_init_ok == 0)
            {
                lv_timer_handler();
            }
        }
        osDelay(LVGL_TICK);
    }
}
