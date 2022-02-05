/*
 * @FilePath                        : \llq-82\groundwork\bsp_adc\bsp_adc.c
 * @author                          : Seasky.Liu
 * @Date                            : 2021-11-24 09:25
 * @LastEditors                     : Seasky.Liu
 * @Description                     : https://github.com/SEASKY-Master
 * @symbol_custom_string_obkoro1    :
 * 版权所有:@Seasky.liu
 * 联系方式:liuwei_seasky@163.com
 * 开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途
 * ************************************************************************
 *            If you want a thing done well, do it yourself.
 * ************************************************************************
 */

#include "bsp_adc.h"
#include "bsp_delay.h"
#include "adc.h"

extern ADC_HandleTypeDef hadc1;
volatile fp32 voltage_vrefint_proportion = 8.0586080586080586080586080586081e-4f;
static uint16_t adcx_get_chx_value(ADC_HandleTypeDef *ADCx, uint32_t ch)
{
    static ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ch;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES; // ADC_SAMPLETIME_3CYCLES;

    if (HAL_ADC_ConfigChannel(ADCx, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    HAL_ADC_Start(ADCx);

    HAL_ADC_PollForConversion(ADCx, 10);
    return (uint16_t)HAL_ADC_GetValue(ADCx);
}
//获取基准电压，初始化之后使用
void init_vrefint_reciprocal(void)
{
    uint8_t i = 0;
    uint32_t total_adc = 0;
    for (i = 0; i < 200; i++)
    {
        total_adc += adcx_get_chx_value(&hadc1, ADC_CHANNEL_VREFINT);
    }
    voltage_vrefint_proportion = 200 * 1.2f / total_adc;
}
//获取CPU的温度
fp32 get_temprate(void)
{
    uint16_t adcx = 0;
    fp32 temperate;

    adcx = adcx_get_chx_value(&hadc1, ADC_CHANNEL_TEMPSENSOR);
    temperate = (fp32)adcx * voltage_vrefint_proportion;
    temperate = (temperate - 0.76f) * 400.0f + 25.0f;

    return temperate;
}
//获取电压
fp32 get_voltage(void)
{
    fp32 voltage;
    uint16_t adcx = 0;
    adcx = adcx_get_chx_value(&hadc1, ADC_CHANNEL_3);
    voltage = (fp32)adcx;
    return voltage;
}
u16 get_adc_average(u8 ch, u8 times)
{
    u32 temp_val = 0;
    u8 t;
    for (t = 0; t < times; t++)
    {
        temp_val += adcx_get_chx_value(&hadc1, ADC_CHANNEL_3);
        delay_ms(5);
    }
    return temp_val / times;
}
float get_oled_voltage()
{
    u8 t;
    u32 total_val;
    u16 adcx;
    total_val = 0;
    float temp;

    for (t = 0; t < 5; t++)
    {
        total_val += adcx_get_chx_value(&hadc1, ADC_CHANNEL_3);
    }
    adcx = (fp32)total_val / 5;        //获取通道ch转换值
    temp = (float)adcx * (3.3 / 4096); //获取计算后的带小数的实际电压值，比如3.1111
    adcx = temp;                       //赋值整数部分给adcx变量，因为adcx为u16整形
    temp *= 1000;                      //小数部分乘以1000，例如：0.1111就转换为111.1，相当于保留三位小数。
    return temp;
}
