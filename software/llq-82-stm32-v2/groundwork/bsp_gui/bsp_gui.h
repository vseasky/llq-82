/*** 
 * @Author                          : Seasky.Liu
 * @LastEditors: Please set LastEditors
 * @FilePath                        : \llq-82\groundwork\bsp_gui\bsp_gui.h
 * @Description                     : https://github.com/SEASKY-Master
 * @symbol_custom_string_obkoro1    : 
 * @版权所有: @Seasky.Liu
 * @联系方式: liuwei_seasky@163.com
 * @开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途,违权必究
 * @************************************************************************
 * @				If you want a thing done well, do it yourself.
 * @************************************************************************
 */
#ifndef _BSP_GUI_H
#define _BSP_GUI_H


#include "struct_typedef.h"

#define MAX_GUI 		7

#define MAX_DIRECTORY   3 	//最大目录三层
#define MAX_SIZE_LENGTH 24

//gui 可以显示的字符串条数
#define MAX_GUI_SHOW_NUM 5

//一级目录数量
#define MAX_GUI_LEVEL1  7

//二级目录数量
#define MAX_TASK1_NUM	24
#define MAX_TASK2_NUM	7
#define MAX_TASK3_NUM	7
#define MAX_TASK4_NUM	5
#define MAX_TASK5_NUM	7
#define MAX_TASK6_NUM	7
#define MAX_TASK7_NUM	7

typedef enum
{
	oled_interface_task1 = 0,
	oled_interface_task2  = 1,
	oled_interface_task3 = 2,
	oled_interface_task4 = 3,
	oled_interface_task5 =4,
	oled_interface_task6 = 5,
	oled_interface_task7 = 6,
}oled_gui_task;
typedef enum
{
	mx_key_null = 0,
	mx_key_on  = 1,
	mx_key_left = 2,
	mx_key_right = 3,
	mx_key_top =4,
	mx_key_bottom = 5,
}oled_key_button;

typedef struct
{
	uint8_t oled_gui_lock:2;	 				  //该值为0,gui可更改，该值为1，gui锁定
	uint8_t oled_level1_options;				  //一级目录大小
	uint8_t oled_level2_options[MAX_GUI_LEVEL1];  //二级目录大小
	uint8_t oled_options_all;					  //
	uint8_t oled_directory;      				  //定义目录标签 默认0级目录
	oled_gui_task   oled_gui_tsak_s;
	oled_key_button oled_key_button_s;
}oled_gui;

const oled_gui* get_oled_info_data(void);
void gui_interface_reset(void);;
void bsp_gui_init(void);
void interface_main(void);
void gui_lock(void);	 //gui锁定函数
void gui_unlock_s(void); //gui解锁函数，根据实际情况修改触发条件
void set_interface_button(oled_key_button button_p);
void set_interface_enc(uint8_t enc_t);

#endif
