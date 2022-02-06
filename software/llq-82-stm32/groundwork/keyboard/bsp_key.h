/***
 * @author                          : Seasky.Liu
 * @Date                            : 2021-11-22 14:56:38
 * @LastEditTime                    : 2022-01-18 15:25:01
 * @LastEditors                     : Seasky.Liu
 * @Description                     : https://github.com/SEASKY-Master
 * @FilePath                        : \keyboard_v7\groundwork\keyboard\bsp_key.h
 * @symbol_custom_string_obkoro1    :
 * @版权所有:@Seasky.liu
 * @联系方式:liuwei_seasky@163.com
 * @开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途
 * @************************************************************************
 * @           If you want a thing done well, do it yourself.
 * @************************************************************************
 */

#ifndef _BSP_KEY_H
#define _BSP_KEY_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "struct_typedef.h"
#include "keyboard_cfg.h"

#define MAX_SEG 6
#define MAX_POS 14
#define MAX_KEY 80

#define PRESS_LEVEL 1 //定义按键按下时的电平

#define TIME_ALL_CYCLE 1.2f //扫描一次所有按键消耗时间  单位ms

// 6ms	按键复位3ms
#define TIME_RESTER_COUNT 	5 //定义按键复位时间，一般来说按键弹起最好比按键按下快
// 12ms	短按触发12ms
#define TIME_PRESS_COUNT 	10 //实际时间 TIME_ALL_CYCLE*TIME_PRESS_COUNT 		(单位ms)确认为短按时间
// 0.75s
#define TIME_PRESS_L_COUNT  625 	 //实际时间 TIME_ALL_CYCLE*TIME_PRESS_L_COUNT 	(单位ms)确认为长按时间
#define TIME_MAX_COUNT 		2500     //定义按键最长计数时间

//定义HID报文周期，蓝牙HID需要大于等于10，USB可以设为2，此处考虑了兼容，设置为10
#define TIME_HID_DELAY 10

//长按状态HID更新时间,最小为2
#define TIME_PRESS_L_HID_COUNT 2 // HID报文周期为10ms,长按状态20ms发送一个字符


#define KEY_NO_USER 255
#define KEY_USER 1

/*宏定义特殊按键*/
#define KEY_LEFT_SHIFT 55
#define KEY_RIGHT_SHIFT 66
#define KEY_LEFT_CONTROL 68
#define KEY_LEFT_WIN 69
#define KEY_LEFT_ALT 70
#define KEY_RIGHT_ALT 72
#define KEY_RIGHT_WIN 73
#define KEY_RIGHT_CONTROL 75

    typedef enum
    {
        KEY_UPSPRING,     //按键弹起
        KEY_PRESS,        //按键按下
        KEY_PRESS_L_TIME, //按键长按
    } key_states;
    typedef struct
    {
        key_states states;          //按键状态
        uint16_t time_count;        //按键按下时间
        uint16_t rester_time_count; //按键复位时间
    } key_time_info;
    typedef struct
    {
        GPIO_TypeDef *gpio;
        uint16_t gpio_pin;
    } key_gpio;
    typedef struct
    {
        keypad_page_t key_char;
        key_time_info press_it;
    } key_press;
    typedef struct
    {
        uint8_t num_lock : 1;
        uint8_t caps_lock : 1;
        uint8_t scroll_lock : 1;
        uint8_t compose : 1;
        uint8_t kana : 1;
    } llq_82_lock;
    typedef enum
    {
        LLQ_NULL_HID,
        LLQ_USB_HID,
        LLQ_BLE_HID,
    } llq_82_mode_enum;
    typedef enum
    {
        LLQ_WORK_KEYBOARD = 0, //键盘控制
        LLQ_WORK_MOUSE,        //鼠标控制
        LLQ_WORK_KEYBOARD_STR, //自身键盘输入(lvgl)
        LLQ_WORK_MEDIA,        //媒体控制
        LLQ_WORK_STRING,       //字符串转HID输出
    } llq_82_work_mode;
    typedef enum
    {
        LLQ_INPUT_BUFFER1 = 0,
        LLQ_INPUT_BUFFER2,
    } llq_82_input_str;
    typedef struct
    {
        llq_82_mode_enum keyboard_hid_mode; //键盘模式,有线或者无线
        llq_82_work_mode keyboard_work; //键盘工作状态
        uint8_t usb_connect_status;
        uint8_t ble_connect_status;
    } key_hid_mode;
    typedef struct
    {
        key_gpio key_seg[MAX_SEG];
        key_gpio key_pos[MAX_POS];
        uint8_t key_is_press_seg[MAX_SEG][MAX_POS]; //在所在行，判断是否按下
        key_press key_is_press[MAX_KEY];            //更新到ID
        llq_82_lock keyboard_lock;                  //
        keyboard_cfg keyboard;
        llq_82_input_str keyboard_user;
        key_hid_mode keyboard_mode;
    } key_info_t;
    /***
     * @Author: Seasky.Liu
     * @Description: 键盘默认配置
     * @param {key_info_t} *key_cfg
     * @return {*}
     */
    void key_set_default(key_info_t *key_cfg);

    /***
     * @Author: Seasky.Liu
     * @Description: 设置矩阵按键行电平
     * @param {key_info_t} *key_cfg
     * @param {uint8_t} seg
     * @return {*}
     */
    void key_scan_gpio_set(key_info_t *key_cfg, uint8_t seg);

    /***
     * @Author: Seasky.Liu
     * @Description: 读取矩阵按键列电平
     * @param {key_info_t} *key_cfg
     * @param {uint8_t} seg
     * @return {*}
     */
    void key_scan_gpio_read(key_info_t *key_cfg, uint8_t seg);

    /***
     * @Author: Seasky.Liu
     * @Description: 更新键盘状态
     * @param {key_info_t} *key_cfg
     * @param {uint8_t} *rx_buff
     * @return {*}
     */
    void keyboard_led_get(key_info_t *key_cfg, uint8_t *rx_buff);

    /***
     * @Author: Seasky.Liu
     * @Description: 更新按键扫描结果
     * @param {key_info_t} *key_cfg
     * @return {*}
     */
    void key_scan_updata(key_info_t *key_cfg);

    /***
     * @Author: Seasky.Liu
     * @Description: 更新输出上报缓冲
     * @param {uint8_t} *pTxbuf
     * @param {key_info_t} *key_cfg
     * @return {*}
     */
    void key_update(uint8_t *pTxbuf, key_info_t *key_cfg);

    /**
     * @Author          : Seasky.Liu
     * @Description     : 无论是否需要自动设置模式，都需要引用该函数
     * @param            {key_info_t} *key_cfg
     * @param            {uint8_t} usb_connect
     * @param            {uint8_t} ble_connect
     * @return           {*}
     */
    void keyboard_mode_check(key_info_t *key_cfg, uint8_t usb_connect, uint8_t ble_connect);

    /**
     * @Author          : Seasky.Liu
     * @Description     : 手动设置模式，使用前需要将自动设置清除
     * @param            {key_info_t} *key_cfg
     * @param            {llq_82_mode_enum} mode_t
     * @return           {*}
     */
    void keyboard_mode_set(key_info_t *key_cfg, llq_82_mode_enum mode_t);

    /**
     * @Author          : Seasky.Liu
     * @Description     : 获取键盘模式
     * @param            {key_info_t} *key_cfg
     * @return           {*}
     */
    llq_82_mode_enum keyboard_mode_get(key_info_t *key_cfg);

    /**
     * @Author          : Seasky.Liu
     * @Description     : 获取键盘模式
     * @param            {key_info_t} *key_cfg
     * @return           {*}
     */
    llq_82_work_mode keyboard_work_get(key_info_t *key_cfg);

    /**
     * @Author          : Seasky.Liu
     * @Description     : 设置键盘工作内容，完成工作后需要复位到键盘模式，仅在键盘模式可以切换其他模式
     * @param            {key_info_t} *key_cfg
     * @param            {llq_82_work_mode} work_mode_t
     * @return           {*}
     */
    void keyboard_work_set(key_info_t *key_cfg, llq_82_work_mode work_mode_t);

    /**
     * @Author          : Seasky.Liu
     * @Description     : 默认键盘，短暂切换为其他模式，需要复位
     * @param            {key_info_t} *key_cfg
     * @return           {*}
     */
    void keyboard_work_reset(key_info_t *key_cfg);

    void keyboard_work_input_exit(key_info_t *key_cfg);
    
    uint8_t media_updata(key_info_t *key_cfg);

#ifdef __cplusplus
}
#endif

#endif
