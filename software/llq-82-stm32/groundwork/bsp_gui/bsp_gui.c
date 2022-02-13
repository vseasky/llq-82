/*
 * @Author                          : Seasky.Liu
 * @LastEditors: Please set LastEditors
 * @FilePath                        : \llq-82\groundwork\bsp_gui\bsp_gui.c
 * @Description                     : https://github.com/SEASKY-Master
 * @symbol_custom_string_obkoro1    :
 * 版权所有: @Seasky.Liu
 * 联系方式: liuwei_seasky@163.com
 * 开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途,违权必究
 * ************************************************************************
 * 				If you want a thing done well, do it yourself.
 * ************************************************************************
 */
#include "bsp_gui.h"
#include "flash_task.h"
#include "keyboard_task.h"
#include "bsp_key.h"
#include "touch_task.h"
#include "bsp_sw6208.h"

oled_gui mx_oled_info_s = {0};
/****************************************************************/
/*						GUI字符列表 							*/
//根据需要添加相关显示函数
uint8_t mx_oled_gui_str[MAX_GUI_LEVEL1][MAX_SIZE_LENGTH] =
	{".password  ", ".touch  ", ".power     ", ".lvgl_style    ", ".rgb_style    ", ".setting    ", ".keyboard type"};

// uint8_t mx_oled_gui_task1[MAX_TASK1_NUM][MAX_SIZE_LENGTH];

uint8_t mx_oled_gui_task2[MAX_TASK2_NUM][MAX_SIZE_LENGTH] =
	{".get_touch_num", ".set_add_num", ".add_touch", ".delete_touch", ".delect_touch", ".close_touch", ".open_touch"};

uint8_t mx_oled_gui_task3[MAX_TASK3_NUM][MAX_SIZE_LENGTH];

uint8_t mx_oled_gui_task4[MAX_TASK4_NUM][MAX_SIZE_LENGTH] =
	{".background_color", ".font_select_color", ".font_setting_color", ".font_normal_color", ".font_input_color"};

uint8_t mx_oled_gui_task5[MAX_TASK5_NUM][MAX_SIZE_LENGTH] =
	{".rgb_style1  ", ".rgb_style2  ", ".rgb_style3  ", ".rgb_style4  ", ".rgb_style5  ", ".rgb_style6  ", ".rgb_style7  "};

uint8_t mx_oled_gui_task6[MAX_TASK6_NUM][MAX_SIZE_LENGTH] =
	{".left_enc_dir  ", ".right_enc_dir  ", ".default password", ".lcd_touch_power  ", ".rgb_power  ", ".lcd_sleep_time", ".task6_p7  "};

uint8_t mx_oled_gui_task7[MAX_TASK7_NUM][MAX_SIZE_LENGTH] =
	{".type1  ", ".type2  ", ".type3  ", ".type4  ", ".type5  ", ".type6  ", ".type7  "};

const uint8_t oled_level2_max_num[MAX_GUI_LEVEL1] = {MAX_TASK1_NUM, MAX_TASK2_NUM, MAX_TASK3_NUM, MAX_TASK4_NUM, MAX_TASK5_NUM, MAX_TASK6_NUM, MAX_TASK7_NUM};

static void interface_input_config(void);
static void interface_key_scanf(void);
static void interface_finger(uint8_t i, uint8_t fin_mx_task[][MAX_SIZE_LENGTH]);
static void interface_gui_main(void);
static void interface_level1(void);
static void interface_level2(void);
static void interface_show_only(void);
static void interface_task1(void);
static void interface_task2(void);
static void interface_task3(void);
static void interface_task4(void);
static void interface_task5(void);
static void interface_task6(void);
static void interface_task7(void);

/***************自定义功能函数******************/
static void dbus_show(void);
static void ina226_task3(void);
static void task2_imu(void);
static void task2_temp(void);
/***************自定义功能函数******************/

const oled_gui *get_oled_info_data(void)
{
	bsp_gui_init();
	return &mx_oled_info_s;
}

void bsp_gui_init(void)
{
	uint8_t i = 0;
	//	for (i = 0; i < MAX_TASK1_NUM; i++)
	//	{
	//		snprintf(mx_oled_gui_task1[i], MAX_SIZE_LENGTH, ".hello-%d", i);
	//	}
}

void interface_main(void)
{
	interface_input_config();
	interface_key_scanf();
	//目录深度
	switch (mx_oled_info_s.oled_directory)
	{
	case 0:
	{
		interface_gui_main();
		touch_tx_ready(TOUCH_SEND_DEF_WORD);
	};
	break;
	case 1:
	{
		interface_level1();
	};
	break;
	case 2:
	{
		interface_level2();
	}
	break;
	case 3:
	{
		interface_level2();
	}
	break;
	}
	if (((mx_oled_info_s.oled_key_button_s == mx_key_right) || (mx_oled_info_s.oled_key_button_s == mx_key_on)) && (mx_oled_info_s.oled_directory == 3))
	{
		flash_save_setting();
	}
	mx_oled_info_s.oled_key_button_s = mx_key_null;
}

void gui_lock(void)
{
	mx_oled_info_s.oled_gui_lock = 1;
}

void gui_unlock_s(void)
{
	if (mx_oled_info_s.oled_gui_lock != 0)
	{
		mx_oled_info_s.oled_gui_lock = 0;
		mx_oled_info_s.oled_directory--;
	}
}

void set_interface_button(oled_key_button button_p)
{
	mx_oled_info_s.oled_key_button_s = button_p;
}

void set_interface_enc(uint8_t enc_t)
{
	static uint8_t last_oled_level1_options = 0;
	static uint8_t last_oled_level2_options[MAX_GUI_LEVEL1];
	mx_oled_info_s.oled_options_all = enc_t; //存储一次原始的值
	if ((mx_oled_info_s.oled_directory == 0) || (mx_oled_info_s.oled_directory == 3))
	{
		//一级目录选项
		last_oled_level1_options = (enc_t % MAX_GUI_LEVEL1 + MAX_GUI_LEVEL1 - mx_oled_info_s.oled_level1_options) % MAX_GUI_LEVEL1;
		//二级目录选项
		last_oled_level2_options[mx_oled_info_s.oled_level1_options] =
			(enc_t % (oled_level2_max_num[mx_oled_info_s.oled_level1_options]) +
			 (oled_level2_max_num[mx_oled_info_s.oled_level1_options]) -
			 mx_oled_info_s.oled_level2_options[mx_oled_info_s.oled_level1_options]) %
			(oled_level2_max_num[mx_oled_info_s.oled_level1_options]);
		return;
	}
	else if (mx_oled_info_s.oled_directory == 1)
	{
		//当前处于一级目录
		//一级目录选项
		mx_oled_info_s.oled_level1_options = (enc_t % MAX_GUI_LEVEL1 + MAX_GUI_LEVEL1 - last_oled_level1_options) % MAX_GUI_LEVEL1;
		//二级目录选项
		last_oled_level2_options[mx_oled_info_s.oled_level1_options] =
			(enc_t % (oled_level2_max_num[mx_oled_info_s.oled_level1_options]) +
			 (oled_level2_max_num[mx_oled_info_s.oled_level1_options]) -
			 mx_oled_info_s.oled_level2_options[mx_oled_info_s.oled_level1_options]) %
			(oled_level2_max_num[mx_oled_info_s.oled_level1_options]);
	}
	else if (mx_oled_info_s.oled_directory == 2)
	{
		//当前处于二级目录
		//一级目录选项
		last_oled_level1_options = (enc_t % MAX_GUI_LEVEL1 + MAX_GUI_LEVEL1 - mx_oled_info_s.oled_level1_options) % MAX_GUI_LEVEL1;
		//二级目录选项
		mx_oled_info_s.oled_level2_options[mx_oled_info_s.oled_level1_options] =
			(enc_t % (oled_level2_max_num[mx_oled_info_s.oled_level1_options]) +
			 (oled_level2_max_num[mx_oled_info_s.oled_level1_options]) -
			 last_oled_level2_options[mx_oled_info_s.oled_level1_options]) %
			(oled_level2_max_num[mx_oled_info_s.oled_level1_options]);
	}
	return;
}

static void interface_input_config(void)
{
	//设置键盘输入开启条件
	if ((mx_oled_info_s.oled_directory == 3) && (mx_oled_info_s.oled_level1_options == 0))
	{
		keyboard_input_mode();
	}
	else
	{
		keyboard_input_exit();
	}
}
static void interface_key_scanf(void)
{
	oled_key_button key_button;
	key_button = mx_oled_info_s.oled_key_button_s;
	if (mx_oled_info_s.oled_directory <= 2)
	{
		mx_oled_info_s.oled_key_button_s = mx_key_null;
	}
	switch (key_button)
	{
	case mx_key_null:
		break;
	case mx_key_on:
		break;
	case mx_key_left:
	{
		if (mx_oled_info_s.oled_gui_lock == 0)
		{
			if (mx_oled_info_s.oled_directory > 0)
			{
				mx_oled_info_s.oled_directory--;
				break;
			}
			else
			{
				mx_oled_info_s.oled_directory = 0;
				break;
			}
		}
	}
	break;
	case mx_key_right:
	{
		if (mx_oled_info_s.oled_gui_lock == 0)
		{
			if (mx_oled_info_s.oled_directory < MAX_DIRECTORY)
			{
				mx_oled_info_s.oled_directory++;
				break;
			}
			else
			{
				mx_oled_info_s.oled_directory = MAX_DIRECTORY;
				break;
			}
		}
	}
	break;
	case mx_key_top:
	{
		//目录
		if (mx_oled_info_s.oled_directory == 0)
		{
			break; //当前在0级界面跳出
		}
		else if (mx_oled_info_s.oled_directory == 1)
		{
			if (mx_oled_info_s.oled_level1_options > 0)
			{
				mx_oled_info_s.oled_level1_options--;
				break;
			}
			else
			{
				mx_oled_info_s.oled_level1_options = 0;
				break;
			}
		}
		else if (mx_oled_info_s.oled_directory == 2)
		{
			if (mx_oled_info_s.oled_level2_options[mx_oled_info_s.oled_level1_options] > 0)
			{
				mx_oled_info_s.oled_level2_options[mx_oled_info_s.oled_level1_options]--;
				break;
			}
			else
			{
				mx_oled_info_s.oled_level2_options[mx_oled_info_s.oled_level1_options] = 0;
				break;
			}
		}
		else if (mx_oled_info_s.oled_directory == 3)
		{
			break; //当前在3级界面跳出
		}
		else
		{
			break;
		}
	}
	break;
	case mx_key_bottom:
	{
		if (mx_oled_info_s.oled_directory == 0)
		{
			break; //当前在0级界面跳出
		}
		else if (mx_oled_info_s.oled_directory == 1)
		{
			if (mx_oled_info_s.oled_level1_options < (MAX_GUI_LEVEL1 - 1))
			{
				mx_oled_info_s.oled_level1_options++;
				break;
			}
			else
			{
				mx_oled_info_s.oled_level1_options = (MAX_GUI_LEVEL1 - 1);
				break;
			}
		}
		else if (mx_oled_info_s.oled_directory == 2)
		{
			if (mx_oled_info_s.oled_level2_options[mx_oled_info_s.oled_level1_options] < (oled_level2_max_num[mx_oled_info_s.oled_level1_options] - 1))
			{
				mx_oled_info_s.oled_level2_options[mx_oled_info_s.oled_level1_options]++;
				break;
			}
			else
			{
				mx_oled_info_s.oled_level2_options[mx_oled_info_s.oled_level1_options] = (oled_level2_max_num[mx_oled_info_s.oled_level1_options] - 1);
				break;
			}
		}
		else if (mx_oled_info_s.oled_directory == 3)
		{
			break; //当前在3级界面跳出
		}
		else
		{
			break;
		}
	}
	break;
	}
}
static void interface_gui_main(void)
{
	/***************interface_gui_main:开始0****************/
	//设置你的主界面以显示logo
	lv_app_select(LVGL_GUI_MAIN);
	/***************interface_gui_main:结束0****************/
	mx_oled_info_s.oled_level1_options = 0;
}
static void interface_finger(uint8_t get_finger, uint8_t fin_mx_task[][MAX_SIZE_LENGTH])
{
	//选项滑块
	uint8_t k;
	static const uint8_t slider_width = MAX_GUI_SHOW_NUM - 1;
	static uint8_t min = 0, max = MAX_GUI_SHOW_NUM - 1;
	//如果移动到最大选项
	if (get_finger > max)
	{
		min = get_finger - slider_width;
		max = get_finger;
	}
	else if (get_finger < min)
	{
		min = get_finger;
		max = get_finger + slider_width;
	}
	for (k = min; k <= max; k++)
	{
		lv_set_gui_lable(k - min, &fin_mx_task[k][0], k + 1, (k == get_finger));
	}
}
static void interface_finger_select(uint8_t get_finger, uint8_t select, uint8_t fin_mx_task[][MAX_SIZE_LENGTH])
{
	//选项滑块
	uint8_t k;
	static const uint8_t slider_width = MAX_GUI_SHOW_NUM - 1;
	static uint8_t min = 0, max = MAX_GUI_SHOW_NUM - 1;
	//如果移动到最大选项
	if (get_finger > max)
	{
		min = get_finger - slider_width;
		max = get_finger;
	}
	else if (get_finger < min)
	{
		min = get_finger;
		max = get_finger + slider_width;
	}
	for (k = min; k <= max; k++)
	{
		lv_set_gui_lable_select(k - min, &fin_mx_task[k][0], k + 1, (k == get_finger), (k == select));
	}
}
static void interface_show_only(void)
{
	if (mx_oled_info_s.oled_directory >= 3)
	{
		mx_oled_info_s.oled_directory = 2;
	}
}
void gui_interface_reset(void)
{
	mx_oled_info_s.oled_directory = 0;
	mx_oled_info_s.oled_level1_options = 0;
}
/*gui项目一级界面显示函数*/
static void interface_level1(void)
{
	uint8_t level1_i = 0;

	/***************interface_level1:开始0****************/
	//设置菜单界面依赖
	lv_app_select(LVGL_GUI_MENU);
	/***************interface_level1:结束0****************/

	interface_finger(mx_oled_info_s.oled_level1_options,
					 mx_oled_gui_str);
	for (level1_i = 0; level1_i < MAX_GUI_LEVEL1; level1_i++)
	{
		//配置重新进入默认选择第一项
		mx_oled_info_s.oled_level2_options[level1_i] = 0;
	}
	/***************interface_level1:开始1****************/
	touch_tx_ready(TOUCH_SEND_DEF_WORD);
	/***************interface_level1:结束1****************/
}
/*gui项目二级界面显示函数*/
static void interface_level2(void)
{
	/***************interface_level2:开始0****************/
	//设置菜单界面依赖
	lv_app_select(LVGL_GUI_MENU);
	/***************interface_level2:结束0****************/
	if (mx_oled_info_s.oled_level1_options != 0)
	{
		touch_tx_ready(TOUCH_SEND_DEF_WORD);
	}
	switch (mx_oled_info_s.oled_level1_options)
	{
	case 0:
		interface_task1();
		break;
	case 1:
		interface_task2();
		break;
	case 2:
		interface_task3();
		break;
	case 3:
		interface_task4();
		break;
	case 4:
		interface_task5();
		break;
	case 5:
		interface_task6();
		break;
	case 6:
		interface_task7();
		break;
	}
}

/*gui界面函数1*/
static void interface_task1(void)
{
	/***************interface_task1:开始0****************/
	static uint8_t first_into = 0;
	static uint8_t last_oled_task1_option;

	//选项滑块
	uint8_t k;
	static const uint8_t slider_width = MAX_GUI_SHOW_NUM - 1;
	static uint8_t min = 0, max = MAX_GUI_SHOW_NUM - 1;
	/***************interface_task1:结束0****************/

	if (mx_oled_info_s.oled_directory == 2)
	{
		// interface_finger(mx_oled_info_s.oled_level2_options[0],
		// 				 mx_oled_gui_task1);

		//如果移动到最大选项
		if (mx_oled_info_s.oled_level2_options[0] > max)
		{
			min = mx_oled_info_s.oled_level2_options[0] - slider_width;
			max = mx_oled_info_s.oled_level2_options[0];
		}
		else if (mx_oled_info_s.oled_level2_options[0] < min)
		{
			min = mx_oled_info_s.oled_level2_options[0];
			max = mx_oled_info_s.oled_level2_options[0] + slider_width;
		}
		for (k = min; k <= max; k++)
		{
			lv_set_gui_lable(k - min, &password_show()->pass_word_t[k].user_name[0], k + 1, (k == mx_oled_info_s.oled_level2_options[0]));
		}

		set_tx_current(mx_oled_info_s.oled_level2_options[0]);
		touch_tx_ready(TOUCH_SEND_CUR_WORD);
	}
	/***************interface_task1:开始1****************/
	// mx_oled_info_s.oled_directory = 3 自定义为功能界面
	if (mx_oled_info_s.oled_directory == 3)
	{
		keyboard_input_mode();
		if (first_into <= 10)
		{
			first_into++;
		}
		if (first_into < 2)
		{
			keyboard_user_info_get(mx_oled_info_s.oled_level2_options[0]);
			lv_set_gui_setting(0, "user info set", mx_oled_info_s.oled_level2_options[0], 0);
		}
		else
		{
			if (((mx_oled_info_s.oled_options_all + last_oled_task1_option) % 2) == 0)
			{
				// get_user_name(task_username, mx_oled_info_s.oled_task1_options);
				lv_set_gui_setting(1, "username", mx_oled_info_s.oled_level2_options[0], 1);
				lv_set_gui_setting(3, "password", mx_oled_info_s.oled_level2_options[0], 0);
				keyboard_buffer_show(1, 0);
				touch_tx_ready(TOUCH_SEND_CUR_NAME);
			}
			else
			{
				// get_pass_word(task_password, mx_oled_info_s.oled_task1_options);
				lv_set_gui_setting(1, "username", mx_oled_info_s.oled_level2_options[0], 0);
				lv_set_gui_setting(3, "password", mx_oled_info_s.oled_level2_options[0], 1);
				keyboard_buffer_show(1, 1);
				touch_tx_ready(TOUCH_SEND_CUR_WORD);
			}
			if (((mx_oled_info_s.oled_key_button_s == mx_key_right) || (mx_oled_info_s.oled_key_button_s == mx_key_on)) && (mx_oled_info_s.oled_directory == 3))
			{
				keyboard_user_info_set(mx_oled_info_s.oled_level2_options[0]);
			}
		}
	}
	else
	{
		last_oled_task1_option = (mx_oled_info_s.oled_options_all) % 2;
		first_into = 0;
	}
	/***************interface_task1:结束1****************/
}

/*指纹模块相关操作*/
static void interface_task2(void)
{

	/***************interface_task2:开始0****************/
	static uint8_t last_options_task2 = 0;
	static uint8_t curr_options_task2 = 0;
	/***************interface_task2:结束0****************/

	if (mx_oled_info_s.oled_directory == 2)
		interface_finger(mx_oled_info_s.oled_level2_options[1],
						 mx_oled_gui_task2);

	/***************interface_task2:开始1****************/
	// mx_oled_info_s.oled_directory = 3 自定义为功能界面
	if (mx_oled_info_s.oled_directory == 3)
	{
		curr_options_task2 = mx_oled_info_s.oled_options_all - last_options_task2;
		if (curr_options_task2 > 99)
		{
			curr_options_task2 = 99; //编码器设置计数到99
		}
		touch_show_info(mx_oled_info_s.oled_level2_options[1], curr_options_task2, (mx_oled_info_s.oled_key_button_s == mx_key_right) || (mx_oled_info_s.oled_key_button_s == mx_key_on));
	}
	else
	{
		last_options_task2 = mx_oled_info_s.oled_options_all - curr_options_task2;
	}
	/***************interface_task2:结束1****************/
}

/*gui界面函数3*/
static void interface_task3(void)
{
	/***************interface_task3:开始0****************/
	uint8_t pd_type, pd_source, pd_sink;
	uint16_t buffer[7];
	bsp_sw6208_for_gui(buffer);
	pd_type = (buffer[0] >> 7) & 0X01;
	pd_sink = (buffer[0] >> 4) & 0X07;
	pd_source = (buffer[0]) & 0X0F;
	if (pd_sink > 0)
	{
		if ((pd_sink <= 7) && (pd_type < 2))
		{
			snprintf(mx_oled_gui_task3[0], MAX_SIZE_LENGTH, ".%s>>[%s]", pd_type_str[pd_type], pd_sink_str[pd_sink]);
		}
	}
	else if (pd_source > 0)
	{
		if ((pd_source <= 10) && (pd_type < 2))
		{
			snprintf(mx_oled_gui_task3[0], MAX_SIZE_LENGTH, ".%s>>[%s]", pd_type_str[pd_type], pd_source_str[pd_source]);
		}
	}
	else
	{
		snprintf(mx_oled_gui_task3[0], MAX_SIZE_LENGTH, ".%s>>[null]", pd_type_str[pd_type]);
	}
	snprintf(mx_oled_gui_task3[1], MAX_SIZE_LENGTH, ".bat_volt>>[%d](0.1V)", buffer[1]);
	snprintf(mx_oled_gui_task3[2], MAX_SIZE_LENGTH, ".bus_volt>>[%d](0.1V)", buffer[2]);
	snprintf(mx_oled_gui_task3[3], MAX_SIZE_LENGTH, ".chip_temp>>[%d](10mV)", buffer[3]);
	snprintf(mx_oled_gui_task3[4], MAX_SIZE_LENGTH, ".bat_elec>>[%d](%%)", buffer[4]);
	snprintf(mx_oled_gui_task3[5], MAX_SIZE_LENGTH, ".in_cur>>[%d](0.1A)", buffer[5]);
	snprintf(mx_oled_gui_task3[6], MAX_SIZE_LENGTH, ".out_cur>>[%d](0.1A)", buffer[6]);
	/***************interface_task3:结束0****************/

	if (mx_oled_info_s.oled_directory == 2)
	{
		interface_finger(mx_oled_info_s.oled_level2_options[2],
						 mx_oled_gui_task3);
	}

	/***************interface_task3:开始1****************/
	// mx_oled_info_s.oled_directory = 3 自定义为功能界面

	//该目录仅用作显示
	interface_show_only();

	/***************interface_task3:结束1****************/
}
/*gui界面函数4*/
// {".background_color", ".font_select_color", ".font_setting_color", ".font_normal_color", ".font_input_color"};
static void interface_task4(void)
{
	/***************interface_task4:开始0****************/
	static uint8_t last_options_task4[5] = {0};
	static uint8_t curr_options_task4[5] = {0};
	/***************interface_task4:结束0****************/

	interface_finger(mx_oled_info_s.oled_level2_options[3],
					 mx_oled_gui_task4);

	/***************interface_task4:开始1****************/
	// mx_oled_info_s.oled_directory = 3 自定义为功能界面

	//该目录仅用作显示

	if (mx_oled_info_s.oled_directory == 3)
	{
		curr_options_task4[mx_oled_info_s.oled_level2_options[3]] = (mx_oled_info_s.oled_options_all % LLQ_MAX_COLOR_NUM + LLQ_MAX_COLOR_NUM - last_options_task4[mx_oled_info_s.oled_level2_options[3]]) % LLQ_MAX_COLOR_NUM;
		lv_color_code_config(mx_oled_info_s.oled_level2_options[3], curr_options_task4[mx_oled_info_s.oled_level2_options[3]], (mx_oled_info_s.oled_key_button_s == mx_key_right) || (mx_oled_info_s.oled_key_button_s == mx_key_on));
	}
	else
	{
		curr_options_task4[0] = (mx_oled_info_s.oled_options_all % LLQ_MAX_COLOR_NUM + LLQ_MAX_COLOR_NUM - curr_options_task4[0]) % LLQ_MAX_COLOR_NUM;
		curr_options_task4[1] = (mx_oled_info_s.oled_options_all % LLQ_MAX_COLOR_NUM + LLQ_MAX_COLOR_NUM - curr_options_task4[1]) % LLQ_MAX_COLOR_NUM;
		curr_options_task4[2] = (mx_oled_info_s.oled_options_all % LLQ_MAX_COLOR_NUM + LLQ_MAX_COLOR_NUM - curr_options_task4[2]) % LLQ_MAX_COLOR_NUM;
		curr_options_task4[3] = (mx_oled_info_s.oled_options_all % LLQ_MAX_COLOR_NUM + LLQ_MAX_COLOR_NUM - curr_options_task4[3]) % LLQ_MAX_COLOR_NUM;
		curr_options_task4[4] = (mx_oled_info_s.oled_options_all % LLQ_MAX_COLOR_NUM + LLQ_MAX_COLOR_NUM - curr_options_task4[4]) % LLQ_MAX_COLOR_NUM;
	}
	/***************interface_task4:结束1****************/
}
/*gui界面函数5*/

// {".rgb_style1  ", ".rgb_style2  ", ".rgb_style3  ", ".rgb_style4  ", ".rgb_style5  ", ".rgb_style6  ", ".rgb_style7  "};
static void interface_task5(void)
{

	/***************interface_task2:开始0****************/

	/***************interface_task2:结束0****************/

	interface_finger_select(mx_oled_info_s.oled_level2_options[4], get_flash_rgb_style(),
							mx_oled_gui_task5);

	/***************interface_task3:开始1****************/
	// mx_oled_info_s.oled_directory = 3 自定义为功能界面
	if (mx_oled_info_s.oled_directory >= 3)
	{
		set_flash_rgb_style(mx_oled_info_s.oled_level2_options[4]);
		flash_save_setting();
		mx_oled_info_s.oled_directory = 2;
	}
	//该目录仅用作显示
//	interface_show_only();

	/***************interface_task3:结束1****************/
}
/*gui界面函数6*/
//{".left_enc_dir  ", ".right_enc_dir  ", ".task6_p3  ", ".task6_p4  ", ".task6_p5  ", ".task6_p6  ", ".task6_p7  "};
static void interface_task6(void)
{
	/***************interface_task6:开始0****************/
	static uint8_t curr_options_task6 = 0;
	static uint8_t last_options_task6 = 0;
	/***************interface_task6:结束0****************/

	interface_finger(mx_oled_info_s.oled_level2_options[5],
					 mx_oled_gui_task6);

	/***************interface_task6:开始1****************/
	// mx_oled_info_s.oled_directory = 3 自定义为功能界面

	//该目录仅用作显示
	// interface_show_only();

	if (mx_oled_info_s.oled_directory == 3)
	{
		curr_options_task6 = (mx_oled_info_s.oled_options_all % 100 - last_options_task6 + 100) % 100;
		key_setting_show_info(mx_oled_info_s.oled_level2_options[5], curr_options_task6, (mx_oled_info_s.oled_key_button_s == mx_key_right) || (mx_oled_info_s.oled_key_button_s == mx_key_on));
		if ((mx_oled_info_s.oled_level2_options[5] == 3) && ((mx_oled_info_s.oled_key_button_s == mx_key_right) || (mx_oled_info_s.oled_key_button_s == mx_key_on)))
		{
			flash_save_setting();
			gui_interface_reset();
		}
	}
	else
	{
		last_options_task6 = mx_oled_info_s.oled_options_all - curr_options_task6;
	}
	/***************interface_task6:结束1****************/
}
/*gui界面函数7*/
static void interface_task7(void)
{
	/***************interface_task2:开始0****************/

	/***************interface_task2:结束0****************/

	interface_finger(mx_oled_info_s.oled_level2_options[6],
					 mx_oled_gui_task7);

	/***************interface_task3:开始1****************/
	// mx_oled_info_s.oled_directory = 3 自定义为功能界面

	//该目录仅用作显示
	interface_show_only();

	/***************interface_task3:结束1****************/
}

/***************自定义功能函数******************/
static void dbus_show(void)
{
}
/*imu测试显示函数*/
static void task2_imu(void)
{
}
static void task2_temp(void)
{
}

static void ina226_task3(void)
{
}
/***************自定义功能函数******************/
