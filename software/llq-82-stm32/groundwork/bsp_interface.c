/*
 * @Author                         : Seasky.Liu
 * @Date                           : Do not edit
 * @LastEditTime: 2022-02-05 11:23:25
 * @LastEditors: Please set LastEditors
 * @Description                    : https://github.com/SEASKY-Master
 * @FilePath                        : \llq-82\groundwork\bsp_interface.c
 * @symbol_custom_string_obkoro1          : 版权所有:@Seasky.liu
 * 联系方式:liuwei_seasky@163.com
 * 开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途
 * ************************************************************************
 *            If you want a thing done well, do it yourself.
 * ************************************************************************
 */

#include "bsp_interface.h"
#include "bsp_delay.h"

#include "FreeRTOS.h"
#include "task.h"
#include "key_task.h"

#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "spi.h"
#include "i2c.h"

uint8_t wake_up_stop_mode = 0;
static uint8_t ESP32_EN_GPIO_Port_Status = 0;
static uint8_t LCD_EN_GPIO_Port_Status = 0;
static uint8_t LED_EN_GPIO_Port_Status = 0;

bsp_interface_struct bsp_interface_addr;



uint8_t get_ble_power_status(void)
{
    return ESP32_EN_GPIO_Port_Status;
}
uint8_t get_lcd_power_status(void)
{
    return LCD_EN_GPIO_Port_Status;
}
uint8_t get_touch_power_status(void)
{
    return LCD_EN_GPIO_Port_Status;
}

uint8_t get_rgb_power_status(void)
{
    return LED_EN_GPIO_Port_Status;
}
static void esp32_power_set(uint8_t state)
{
    ESP32_EN_GPIO_Port_Status = state;
    HAL_GPIO_WritePin(ESP32_EN_GPIO_Port, ESP32_EN_Pin, state);
}
static void touch_lcd_power_set(uint8_t state)
{
    LCD_EN_GPIO_Port_Status = state;
    HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, state);
}
static void led_power_set(uint8_t state)
{
    LED_EN_GPIO_Port_Status = state;
    HAL_GPIO_WritePin(LED_EN_GPIO_Port, LED_EN_Pin, state);
}
void power_ble_open_fun(void)
{
    esp32_power_set(1);
}
static void power_lcd_open_fun(void)
{
    touch_lcd_power_set(1);
}
static void power_led_open_fun(void)
{
    led_power_set(1);
}
void power_ble_close_fun(void)
{
    esp32_power_set(0);
}
static void power_lcd_close_fun(void)
{
    touch_lcd_power_set(0);
}
static void power_led_close_fun(void)
{
    led_power_set(0);
}
void power_all_open_fun(void)
{
    power_ble_open_fun();
    power_lcd_open_fun();
    power_led_open_fun();
}
void power_all_close_fun(void)
{
    power_ble_close_fun();
    power_lcd_close_fun();
    power_led_close_fun();
}


/**
 * @Author                         : Seasky.Liu
 * @Date                           : Do not edit
 * @description                    : 0为使能
 * @param                           {*}
 * @return                          {*}
 * @param {uint8_t} ble_p
 * @param {uint8_t} lcd_p
 * @param {uint8_t} rgb_p
 */
void config_power(uint8_t lcd_p,uint8_t rgb_p)
{
    if (lcd_p != 0)
    {
        power_lcd_close_fun();
    }
    else
    {
        power_lcd_open_fun();
    }

    if (rgb_p != 0)
    {
        power_led_close_fun();
    }
    else
    {
        power_led_open_fun();
    }
}
/**
 * @Author          : Seasky.Liu
 * @Description     : 初始化默认关闭所有电源
 * @param            {*}
 * @return           {*}
 */
static void power_init_fun(void)
{
    power_all_close_fun();
}
/**
 * @Author                         : Seasky.Liu
 * @description                    : 复位所有GPIO
 * @param                           {*}
 * @return                          {*}
 */
static void gpio_reset_all(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_DISABLE();
    __HAL_RCC_GPIOH_CLK_DISABLE();
    __HAL_RCC_GPIOA_CLK_DISABLE();
    __HAL_RCC_GPIOB_CLK_DISABLE();
    __HAL_RCC_GPIOD_CLK_DISABLE();

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_All, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_All, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_All, GPIO_PIN_RESET);
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_All);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_All);
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_All);

    /* EXTI interrupt init*/
    HAL_NVIC_DisableIRQ(EXTI0_IRQn);
    HAL_NVIC_DisableIRQ(EXTI2_IRQn);
    HAL_NVIC_DisableIRQ(EXTI3_IRQn);
    HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
}

/**
 * @Author                         : Seasky.Liu
 * @Date                           : Do not edit
 * @description                    : 初始化外部中断，用于低功耗唤醒
 * @param                           {*}
 * @return                          {*}
 */
static void wake_up_exti_config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = SW_KEY_NET2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(SW_KEY_NET2_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LEFT_KEY_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(LEFT_KEY_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = SW_IRQ_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(EXTI0_IRQn, 8, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    HAL_NVIC_SetPriority(EXTI2_IRQn, 8, 0);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 8, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}
/**
 * @Author                         : Seasky.Liu
 * @Date                           : Do not edit
 * @description                    : STM32 反初始化，复位所有状态
 * @param                           {*}
 * @return                          {*}
 */
static void bsp_deinit(void)
{
    __HAL_RCC_PWR_CLK_DISABLE(); //失能PWR时钟
    __HAL_RCC_APB1_FORCE_RESET();
    __HAL_RCC_APB1_RELEASE_RESET();
    __HAL_RCC_APB2_FORCE_RESET();
    __HAL_RCC_APB2_RELEASE_RESET();
    __HAL_RCC_AHB1_FORCE_RESET();
    __HAL_RCC_AHB1_RELEASE_RESET();
    __HAL_RCC_AHB2_FORCE_RESET();
    __HAL_RCC_AHB2_RELEASE_RESET();
    __HAL_RCC_AHB3_FORCE_RESET();
    __HAL_RCC_AHB3_RELEASE_RESET();
    HAL_ADC_DeInit(&hadc1);
    HAL_TIM_Encoder_MspDeInit(&htim1);
    HAL_TIM_Encoder_MspDeInit(&htim2);
    HAL_TIM_Encoder_MspDeInit(&htim3);
    HAL_TIM_Encoder_MspDeInit(&htim4);
    HAL_UART_MspDeInit(&huart1);
    HAL_UART_MspDeInit(&huart3);
    HAL_SPI_MspDeInit(&hspi1);
    HAL_I2C_MspDeInit(&hi2c3);
    gpio_reset_all();
}

/**
 * @Author                         : Seasky.Liu
 * @Date                           : Do not edit
 * @description                    : 延时，1ms
 * @param                           {*}
 * @return                          {*}
 * @param {uint32_t} ms_t
 */
void sys_delay_ms(uint32_t ms_t)
{
    uint32_t i = 0;
    while (ms_t)
    {
        for (i = 0; i < 1680; i++) // 1680*100/168Mhz = 1000us
        {
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP(); // 10/168
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP(); // 10/168
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP(); // 10/168
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP(); // 10/168
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP(); // 10/168

            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP(); // 10/168
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP(); // 10/168
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP(); // 10/168
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP(); // 10/168
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP(); // 10/168
        }
        ms_t--;
    }
}

/**
 * @Author                         : Seasky.Liu
 * @Date                           : Do not edit
 * @description                    : 编码器按键同时按下，或SW6208中断来临，按键需要消抖
 * @param                           {*}
 * @return                          {*}
 */
static uint8_t get_sys_wake_state(void)
{
    return (((HAL_GPIO_ReadPin(SW_KEY_NET2_GPIO_Port, SW_KEY_NET2_Pin) == 0) &&
             (HAL_GPIO_ReadPin(LEFT_KEY_GPIO_Port, LEFT_KEY_Pin) == 0)) ||
            (wake_up_stop_mode));
}

/**
 * @Author                         : Seasky.Liu
 * @Date                           : Do not edit
 * @description                    : STM32  进入停止模式，待机，中断唤醒
 * @param                           {*}
 * @return                          {*}
 */
void sys_enter_stop(void)
{
    power_all_close_fun();                                              //关闭所有电源
    bsp_deinit();                                                       //反初始化所有外设，防止异常唤醒H
    HAL_SuspendTick();                                                  //关闭滴答定时器
    wake_up_exti_config();                                              //配置唤醒中断源，需要保证无其他中断产生
    __HAL_RCC_PWR_CLK_ENABLE();                                         //使能电源管理
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI); //进入停止模式
    __set_FAULTMASK(1);                                                 //进入临界区
    NVIC_SystemReset();
    __set_FAULTMASK(0); //退出临界区
}

/**
 * @Author                         : Seasky.Liu
 * @Date                           : Do not edit
 * @description                    : 外部中断
 * @param                           {*}
 * @return                          {*}
 */
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}
void EXTI2_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}

void bsp_interface_init(void)
{
    bsp_interface_addr.power_ble_config = esp32_power_set;
    bsp_interface_addr.power_lcd_config = touch_lcd_power_set;
    bsp_interface_addr.power_led_config = led_power_set;
    bsp_interface_addr.power_enter_stop = sys_enter_stop;
    bsp_interface_addr.bsp_init = power_init_fun;
    bsp_interface_addr.power_ble_open = power_ble_open_fun;
    bsp_interface_addr.power_ble_close = power_ble_close_fun;
    bsp_interface_addr.power_lcd_open = power_lcd_open_fun;
    bsp_interface_addr.power_lcd_close = power_lcd_close_fun;
    bsp_interface_addr.power_led_open = power_led_open_fun;
    bsp_interface_addr.power_led_close = power_led_close_fun;
    bsp_interface_addr.power_all_open = power_all_open_fun;
    bsp_interface_addr.power_all_close = power_all_close_fun;
}