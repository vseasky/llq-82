/*
 * @author                          : Seasky.Liu
 * @Date                            : 2021-11-24 10:02:48
 * @LastEditTime: 2022-01-23 22:31:58
 * @LastEditors: Please set LastEditors
 * @Description                     : https://github.com/SEASKY-Master
 * @FilePath                        : \keyboard_v6\groundwork\bsp_sw6208\bsp_sw6208.c
 * @symbol_custom_string_obkoro1    :
 * 版权所有:@Seasky.liu
 * 联系方式:liuwei_seasky@163.com
 * 开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途
 * ************************************************************************
 *            If you want a thing done well, do it yourself.
 * ************************************************************************
 */
#include "bsp_sw6208.h"
#include "i2c.h"
#include "bsp_delay.h"

// 7
// PD 版本指示
// 0: PD2.0
// 1: PD2.0
// 注意此指示只在 PD 沟通后有效
const char pd_type_str[][20] = {"PD2.0",
                                "PD3.0"};


const char pd_sink_str[][20] = {"null",
                                "PD sink",
                                " ",
                                "HV sink",
                                "FC sink",
                                "FCP sink",
                                "SCP sink",
                                "PE1.1 sink"};
                                
const char pd_source_str[][20] = {"null",
                                  "PD source",
                                  "PPS source",
                                  "QC2.0 source",
                                  "QC3.0 source",
                                  "FCP source",
                                  "PE2.0 /1.1 source",
                                  "SFCP source",
                                  "AFC source",
                                  "SCP source",
                                  "reserved"};

bsp_sw6208_info bsp_sw6208_addr;
bsp_sw6208_info* get_sw6208_point(void)
{
    return &bsp_sw6208_addr;
}
//发送,写入数据
void SW6208_SendData(uint8_t addr, uint8_t reg, uint8_t data)
{
    uint8_t temp[2] = {0};
    temp[0] = data;
    HAL_I2C_Mem_Write(&hi2c3,
                      addr,
                      reg,
                      I2C_MEMADD_SIZE_8BIT,
                      temp, 1, 0xff);
}

//设置寄存器指针
void SW6208_SetRegPointer(uint8_t addr, uint8_t reg)
{
    HAL_I2C_Master_Transmit(&hi2c3,
                            addr,
                            &reg,
                            1,
                            0xff);
}

//读取数据
uint8_t SW6208_ReadData(uint8_t addr)
{
    uint8_t temp[2] = {0};
    HAL_I2C_Master_Receive(&hi2c3, addr + 1, temp, 1, 0xff);
    return temp[0];
}

//获取id
void SW6208_Get_ID(uint8_t addr)
{
    uint32_t temp = 0;
    SW6208_SetRegPointer(addr, 0XB0);
    temp = SW6208_ReadData(addr);
}

/**
 * @Author: Seasky.Liu
 * @Description: 初始化PD充电配置
 * @param {*}
 * @return {*}
 */
void bsp_sw6208_init(void)
{
    //不使用按键
    SW6208_SendData(SW6208_ADDR, SW6208_BUTTON_CFG1_REG, 0X00);
    SW6208_SendData(SW6208_ADDR, SW6208_BUTTON_CFG2_REG, 0X00);
    //清除事件指示
    SW6208_SendData(SW6208_ADDR, SW6208_LED_REG, 0X08);
    SW6208_SendData(SW6208_ADDR, SW6208_BUTTON_EVT_REG, 0XFF);
    SW6208_SendData(SW6208_ADDR, SW6208_PLUG_EVT1_REG, 0XFF);
    SW6208_SendData(SW6208_ADDR, SW6208_PLUG_EVT2_REG, 0XFF);
    SW6208_SendData(SW6208_ADDR, SW6208_ERR_EVT_REG, 0XFF);
    SW6208_SendData(SW6208_ADDR, SW6208_SYS_ERR0_REG, 0XFF);
    //ADC配置,首次选择为电压
    SW6208_SendData(SW6208_ADDR, SW6208_ADC_CFG_REG, 0X00); //选择ADC电压为电池电压
    //输入输出控制使能
    SW6208_SendData(SW6208_ADDR, SW6208_EN_CHARGE_REG, 0x00); //关闭所有输出口
    /*快充配置*/
    //快充配置
    SW6208_SendData(SW6208_ADDR, SW6208_FAST_CFG0_REG, 0X86); //快充配置0 -> 1000 0110
    SW6208_SendData(SW6208_ADDR, SW6208_FAST_CFG1_REG, 0XD7); //快充配置1 -> 1101 0111
    SW6208_SendData(SW6208_ADDR, SW6208_FAST_CFG2_REG, 0X8F); //快充配置2 -> 1000 1111
    SW6208_SendData(SW6208_ADDR, SW6208_FAST_CFG3_REG, 0XFF); //快充配置3 -> 1111 1111
    SW6208_SendData(SW6208_ADDR, SW6208_FAST_CFG4_REG, 0X02); //快充配置3 -> 0001 0010
    //PD配置
    SW6208_SendData(SW6208_ADDR, SW6208_PD_CFG0_REG, 0X20); //PD配置0 -> 0010 0000
    SW6208_SendData(SW6208_ADDR, SW6208_PD_CFG1_REG, 0X80); //PD配置1 -> 1000 0000
    SW6208_SendData(SW6208_ADDR, SW6208_PD_CFG2_REG, 0X00); //PD配置2 -> 0000 0000
    SW6208_SendData(SW6208_ADDR, SW6208_PD_CFG_REG, 0X20);  //PD配置2 -> 0010 0000
    //BOOST 配置
    SW6208_SendData(SW6208_ADDR, SW6208_BOOST_CFG0_REG, 0X06);
    SW6208_SendData(SW6208_ADDR, SW6208_BOOST_CFG1_REG, 0X01);

    //Charger 配置
    SW6208_SendData(SW6208_ADDR, SW6208_CHARGER_CFG0_REG, 0X00);
    SW6208_SendData(SW6208_ADDR, SW6208_CHARGER_CFG1_REG, 0XF0);
    SW6208_SendData(SW6208_ADDR, SW6208_CHARGER_CFG2_REG, 0X1A);
    SW6208_SendData(SW6208_ADDR, SW6208_CHARGER_CFG3_REG, 0X3F);
    SW6208_SendData(SW6208_ADDR, SW6208_CHARGER_CFG4_REG, 0X00); //600K
    //电池容量
    //SW6208_SendData(SW6208_ADDR, SW6208_BAT_CAP0_REG, 0X04);//
    //SW6208_SendData(SW6208_ADDR, SW6208_BAT_CAP1_REG, 0X60);//

    SW6208_SetRegPointer(SW6208_ADDR, SW6208_CHIP_VER_REG);
    bsp_sw6208_addr.chip_ver = SW6208_ReadData(SW6208_ADDR);

    //不使用按键
    // SW6208_SendData(SW6208_ADDR, SW6208_BUTTON_CFG1_REG, 0X00);
    // SW6208_SendData(SW6208_ADDR, SW6208_BUTTON_CFG2_REG, 0X00);
    // //ADC配置
    // SW6208_SendData(SW6208_ADDR, SW6208_ADC_CFG_REG, (1 << 0)); //选择ADC电压为电池电压
    // //输入输出使能控制
    // SW6208_SendData(SW6208_ADDR, SW6208_EN_CHARGE_REG, 0); //关闭所有输出口
    // //快充配置
    // SW6208_SendData(SW6208_ADDR, SW6208_FAST_CFG0_REG, 0); //不检测DM、不支持12V、最高9V
    // SW6208_SendData(SW6208_ADDR, SW6208_FAST_CFG1_REG, 0); //C口输入使能、其余禁用
    // SW6208_SendData(SW6208_ADDR, SW6208_FAST_CFG2_REG, 0); //禁用source，保留sink
    // SW6208_SendData(SW6208_ADDR, SW6208_FAST_CFG4_REG, 0); //支持边充边放，禁用QC SOURCE

    // //PD配置
    // SW6208_SendData(SW6208_ADDR, SW6208_PD_CFG0_REG, 0);    //PD3.0 最高9V
    // SW6208_SendData(SW6208_ADDR, SW6208_PD_CFG2_REG, 0); //5V3A,9V/2A,12V/1.5A

    // //BOOST 配置
    // SW6208_SendData(SW6208_ADDR, SW6208_BOOST_CFG0_REG, 0);
    // SW6208_SendData(SW6208_ADDR, SW6208_BOOST_CFG1_REG, 0);

    // //Charger 配置
    // SW6208_SendData(SW6208_ADDR, SW6208_CHARGER_CFG0_REG, 0); //500k 18W
    // SW6208_SendData(SW6208_ADDR, SW6208_CHARGER_CFG4_REG, 0); //500K

    // //电池容量
    // //SW6208_SendData(SW6208_ADDR, SW6208_BAT_CAP0_REG, 0X04);//
    // //SW6208_SendData(SW6208_ADDR, SW6208_BAT_CAP1_REG, 0X60);//

    // SW6208_SetRegPointer(SW6208_ADDR, SW6208_CHIP_VER_REG);
    // bsp_sw6208_addr.chip_ver = SW6208_ReadData(SW6208_ADDR);
}
static void bsp_sw6208_get_adc_info(bsp_sw6208_info *bsp_sw6208_info_p)
{
    static uint8_t adc_check_num = 0;
    uint8_t temp_test[2] = {0};
    uint8_t ntc_reg = 0;
    //读取ADC电压
    SW6208_SetRegPointer(SW6208_ADDR, SW6208_ADC_HIGH_REG);
    temp_test[0] = SW6208_ReadData(SW6208_ADDR);
    SW6208_SetRegPointer(SW6208_ADDR, SW6208_ADC_LOW_REG);
    temp_test[1] = SW6208_ReadData(SW6208_ADDR);
    switch (adc_check_num)
    {
    case SW6208_ADC_BATTERY_VOLT:
    {
        bsp_sw6208_info_p->adc_value.adc_battery_volt = (float)(BATTERY_VOLT_ADC_CALC * ((temp_test[0] << 4) | ((temp_test[1]) & 0x0F)));
    }
    break;
    case SW6208_ADC_IN_OUT_VOLT:
    {
        bsp_sw6208_info_p->adc_value.adc_in_out_volt = (float)(IN_OUT_VOLT_ADC_CALC * ((temp_test[0] << 4) | ((temp_test[1]) & 0x0F)));
    }
    break;
    case SW6208_ADC_CHIP_TEMP:
    {
        bsp_sw6208_info_p->adc_value.adc_chip_temp = (float)(CHIP_TEMP_ADC_CALC * ((temp_test[0] << 4) | ((temp_test[1]) & 0x0F)));
    }
    break;
    case SW6208_ADC_NTC_VALUE:
    {
        SW6208_SetRegPointer(SW6208_ADDR, SW6208_NTC_CFG1_REG);
        bsp_sw6208_info_p->ntc_current = (SW6208_ReadData(SW6208_ADDR)) & 0X01;
        if ((bsp_sw6208_info_p->ntc_current) == 0) //80uA
        {
            bsp_sw6208_info_p->adc_value.adc_ntc_volt = (float)(2.2 * ((temp_test[0] << 4) | ((temp_test[1]) & 0x0F)));
            bsp_sw6208_info_p->adc_value.adc_ntc_res = (bsp_sw6208_info_p->adc_value.adc_ntc_volt) / (80); //mv/uA=K欧
            //查表法获取温度,非线性计算较为复杂，此处省略
        }
        else //40uA
        {
            bsp_sw6208_info_p->adc_value.adc_ntc_volt = (float)(1.1 * ((temp_test[0] << 4) | ((temp_test[1]) & 0x0F)));
            bsp_sw6208_info_p->adc_value.adc_ntc_res = (bsp_sw6208_info_p->adc_value.adc_ntc_volt) / (40);
            //查表法获取温度,非线性计算较为复杂，此处省略
        }
    }
    break;
    case SW6208_ADC_IN_CURR:
    {
        bsp_sw6208_info_p->adc_value.adc_in_current = (float)(IN_CURR_ADC_CALC * ((temp_test[0] << 4) | ((temp_test[1]) & 0x0F)));
    }
    break;
    case SW6208_ADC_OUT_CURR:
    {
        bsp_sw6208_info_p->adc_value.adc_out_current = (float)(OUT_CURR_ADC_CALC * ((temp_test[0] << 4) | ((temp_test[1]) & 0x0F)));
    }
    break;
    default:
        break;
    }
    //ADC配置
    if ((adc_check_num >= 0) && (adc_check_num <= SW6208_ADC_OUT_CURR))
    {
        adc_check_num++;
    }
    else if (adc_check_num > SW6208_ADC_OUT_CURR)
    {
        adc_check_num = 0;
    }
    //切换adc
    SW6208_SendData(SW6208_ADDR, SW6208_ADC_CFG_REG, (adc_check_num)); //选择ADC电压为电池电压
}
void bsp_sw6208_get_info(void)
{
    uint8_t temp_test[2];
    SW6208_SetRegPointer(SW6208_ADDR, SW6208_LED_REG);
    bsp_sw6208_addr.ischarger = (SW6208_ReadData(SW6208_ADDR) >> 2) & 0X01;
    //系统状态指示
    SW6208_SetRegPointer(SW6208_ADDR, SW6208_SYS_STA_REG);
    bsp_sw6208_addr.sys_state = SW6208_ReadData(SW6208_ADDR);
    //快充协议指示
    SW6208_SetRegPointer(SW6208_ADDR, SW6208_QUICK_CHARGE_REG);
    bsp_sw6208_addr.charge_port = SW6208_ReadData(SW6208_ADDR);
    //读取电池容量
    SW6208_SetRegPointer(SW6208_ADDR, SW6208_BAT_CAP0_REG); //电池容量低8位
    temp_test[0] = SW6208_ReadData(SW6208_ADDR);
    SW6208_SetRegPointer(SW6208_ADDR, SW6208_BAT_CAP1_REG);
    temp_test[1] = SW6208_ReadData(SW6208_ADDR);
    bsp_sw6208_addr.battery_cap = ((temp_test[0]) | (((temp_test[1]) & 0x0F) << 4));
    //读取电池电量
    SW6208_SetRegPointer(SW6208_ADDR, SW6208_PROCE_POWER_REG);
    bsp_sw6208_addr.battery_elec = SW6208_ReadData(SW6208_ADDR);

    bsp_sw6208_get_adc_info(&bsp_sw6208_addr);
}

// snprintf(mx_oled_gui_task3[0], MAX_SIZE_LENGTH, ".charging type--%04d", buffer[0]);
// snprintf(mx_oled_gui_task3[1], MAX_SIZE_LENGTH, ".battery_volt---%04d", buffer[1]);
// snprintf(mx_oled_gui_task3[2], MAX_SIZE_LENGTH, ".in_out_volt----%04d", buffer[2]);
// snprintf(mx_oled_gui_task3[3], MAX_SIZE_LENGTH, ".chip_temp------%04d", buffer[3]);
// snprintf(mx_oled_gui_task3[4], MAX_SIZE_LENGTH, ".ntc_res--------%04d", buffer[4]);
// snprintf(mx_oled_gui_task3[5], MAX_SIZE_LENGTH, ".in_current-----%04d", buffer[5]);
// snprintf(mx_oled_gui_task3[6], MAX_SIZE_LENGTH, ".out_current----%04d", buffer[6]);
void bsp_sw6208_for_gui(uint16_t buffer[7])
{
    // buffer[0] = //充电类型
    buffer[0] = (uint16_t)bsp_sw6208_addr.charge_port;
    buffer[1] = (uint16_t)((bsp_sw6208_addr.adc_value.adc_battery_volt + 0.5) / 100.0);
    buffer[2] = (uint16_t)((bsp_sw6208_addr.adc_value.adc_in_out_volt + 0.5) / 100.0);
    buffer[3] = (uint16_t)((bsp_sw6208_addr.adc_value.adc_chip_temp + 0.5) / 100.0);
    buffer[4] = (uint16_t)((bsp_sw6208_addr.battery_elec));
    buffer[5] = (uint16_t)((bsp_sw6208_addr.adc_value.adc_in_current + 0.5) / 100.0);
    buffer[6] = (uint16_t)((bsp_sw6208_addr.adc_value.adc_out_current + 0.5) / 100.0);
    // uint16_t  battery_cap; //电池容量
    // uint8_t   battery_elec;//电池电量
    // uint8_t   ntc_current; //ntc电流
    // uint8_t   sys_state;
    // uint8_t   charge_port; //快充协议
    // __packed struct
    // {
    // 	float     adc_battery_volt;//电池电压
    // 	float     adc_in_out_volt; //输入输出电压
    // 	float     adc_chip_temp;   //芯片温度
    // 	float     adc_ntc_volt;    //NTC电压
    //     float     adc_ntc_res;     //NTC阻值
    // 	float     adc_in_current;  //输入电流
    // 	float     adc_out_current; //输出电流
    // }adc_value;
}