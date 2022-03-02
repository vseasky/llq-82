/*** 
 * @FilePath                        : \keyboard_v6\groundwork\bsp_sw6208\bsp_sw6208.h
 * @author                          : Seasky.Liu
 * @Date                            : 2021-11-24 10:03
 * @LastEditors: Please set LastEditors
 * @Description                     : https://github.com/SEASKY-Master
 * @symbol_custom_string_obkoro1    : 
 * @版权所有:@Seasky.liu
 * @联系方式:liuwei_seasky@163.com
 * @开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途
 * @************************************************************************
 * @           If you want a thing done well, do it yourself.
 * @************************************************************************
 */
#include "struct_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define SW6208_ADDR 0X78

#define SW6208_BUTTON_CFG1_REG 0X03  //按键配置
#define SW6208_BUTTON_CFG2_REG 0X04  //短按按键配置
#define SW6208_LED_REG 0X06          //灯显示状态
#define SW6208_BUTTON_EVT_REG 0X07   //按键事件指示
#define SW6208_PLUG_EVT1_REG 0X08    //端口拔出事件指示
#define SW6208_PLUG_EVT2_REG 0X09    //端口插入事件指示
#define SW6208_ERR_EVT_REG 0X0A      //电池异常事件
#define SW6208_SYS_ERR0_REG 0X0B     //系统异常事件指示 0
#define SW6208_SYS_STA_REG 0X0C      //系统状态指示
#define SW6208_QUICK_CHARGE_REG 0X0F //快充协议指示
#define SW6208_ADC_CFG_REG 0X12      //ADC 配置
#define SW6208_ADC_HIGH_REG 0X13     //ADC 数据高 8bits
#define SW6208_ADC_LOW_REG 0X14      //ADC 数据低 4bits
#define SW6208_EN_CHARGE_REG 0X18    //输入输出控制使能
#define SW6208_PORT_EVT_REG 0X19     //端口事件触发
#define SW6208_FAST_CFG0_REG 0X1A    //快充配置 0
#define SW6208_FAST_CFG1_REG 0X1B    //快充配置 1
#define SW6208_FAST_CFG2_REG 0X1C    //快充配置 2
#define SW6208_FAST_CFG3_REG 0X1D    //快充配置 3
#define SW6208_FAST_CFG4_REG 0X1E    //快充配置 4
#define SW6208_FAST_LED_REG 0X1F     //快充指示灯状态
#define SW6208_WLED_CFG_REG 0X20     //WLED配置
#define SW6208_SYS_ERR1_REG 0X21     //系统异常事件指示 1
#define SW6208_PD_TYPE_REG 0X22      //PD命令
#define SW6208_TYPEC_CFG_REG 0X28    //TYPE-C配置
#define SW6208_TYPEC_STA_REG 0X29    //TYPE-C指示
#define SW6208_PD_CFG0_REG 0X2A      //PD配置0
#define SW6208_PD_CFG1_REG 0X2B      //PD配置1
#define SW6208_PD_CFG2_REG 0X2C      //PD配置2
#define SW6208_PD_CFG_REG 0X2D       //多场景PD控制
#define SW6208_CUR_CTR_REG 0X2E      //小电流充电控制
#define SW6208_PLUG_CFG0_REG 0X30    //插拔检测配置0
#define SW6208_PLUG_CFG1_REG 0X31    //插拔检测配置1
#define SW6208_WIRE_CHARGE_REG 0X32  //无线充配置
#define S26208_CUR_CFG_REG 0X33      //小电流充电配置
#define SW6208_BOOST_CFG0_REG 0X40   //BOOST配置0
#define SW6208_BOOST_CFG1_REG 0X41   //BOOST配置1
#define SW6208_CHARGER_CFG0_REG 0X42 //Charger 配置 0
#define SW6208_CHARGER_CFG1_REG 0X43 //Charger 配置 1
#define SW6208_CHARGER_CFG2_REG 0X44 //Charger 配置 2
#define SW6208_CHARGER_CFG3_REG 0X45 //Charger 配置 3
#define SW6208_CHARGER_CFG4_REG 0X46 //Charger 配置 4
#define SW6208_NTC_CFG0_REG 0X47     //NTC配置0
#define SW6208_NTC_CFG1_REG 0X48     //NTC配置1
#define SW6208_TEMP_CFG_REG 0X49     //温度设置
#define SW6208_CHIP_VER_REG 0X57     //芯片版本
#define SW6208_BATTERY_REG 0X72      //最终电量
#define SW6208_BAT_CAP0_REG 0X73     //电池容量低8位
#define SW6208_BAT_CAP1_REG 0X74     //电池容量高4位
#define SW6208_CHARGE_CTR_REG 0X7A   //充电控制
#define SW6208_PROCE_POWER_REG 0X7E  //最终处理电量

#define BATTERY_VOLT_ADC_CALC 1.2f
#define IN_OUT_VOLT_ADC_CALC 4.0f
#define CHIP_TEMP_ADC_CALC (1.0f) / (6.82f)
#define NTC_VALUE_ADC_CALC 1.1f
#define IN_CURR_ADC_CALC (25.0f) / (11.0f)
#define OUT_CURR_ADC_CALC (25.0f) / (11.0f)

    typedef enum
    {
        SW6208_ADC_BATTERY_VOLT = 0,
        SW6208_ADC_IN_OUT_VOLT = 1,
        SW6208_ADC_CHIP_TEMP = 2,
        SW6208_ADC_NTC_VALUE = 3,
        SW6208_ADC_IN_CURR = 4,
        SW6208_ADC_OUT_CURR = 5
    } SW6208_ADC_ENUM;
    typedef struct
    {
        uint8_t chip_ver;
        uint8_t ischarger;
        uint16_t battery_cap; //电池容量
        uint8_t battery_elec; //电池电量
        uint8_t ntc_current;  //ntc电流
        uint8_t sys_state;    //
        uint8_t charge_port;  //快充协议
        __packed struct
        {
            float adc_battery_volt; //电池电压 		mV
            float adc_in_out_volt;  //输入输出电压	mV
            float adc_chip_temp;    //芯片温度		adc
            float adc_ntc_volt;     //NTC电压		adc
            float adc_ntc_res;      //NTC阻值		k欧
            float adc_in_current;   //输入电流		mA
            float adc_out_current;  //输出电流		mA
        } adc_value;
        uint16_t current;
        uint16_t temp;
        uint16_t vel;
    } bsp_sw6208_info;
bsp_sw6208_info* get_sw6208_point(void);
    void bsp_sw6208_init(void);
    void bsp_sw6208_get_info(void);
    void bsp_sw6208_for_gui(uint16_t buffer[7]);

    extern const char pd_type_str[][20];
    extern const char pd_sink_str[][20];
    extern const char pd_source_str[][20];

#ifdef __cplusplus
}
#endif
