/***
 * @author                          : Seasky.Liu
 * @Date                            : 2021-11-22 14:56:40
 * @LastEditTime                    : 2022-01-24 13:23:47
 * @LastEditors                     : Seasky.Liu
 * @Description                     : https://github.com/SEASKY-Master
 * @FilePath                        : \llq-82\LVGL8\lvgl_app\lvgl_app.h
 * @symbol_custom_string_obkoro1    :
 * @版权所有:@Seasky.liu
 * @联系方式:liuwei_seasky@163.com
 * @开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途
 * @************************************************************************
 * @           If you want a thing done well, do it yourself.
 * @************************************************************************
 */
#ifndef _LVGL_APP_H
#define _LVGL_APP_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"
#include "lv_port_disp.h"

#define INFO_WIN_BAR_Y 0
#define INFO_BATTERY_X 190
#define INFO_BLUETOOTH_X 160
#define INFO_WIFI_X 140
#define LVGL_MAX_LABLE 5

#define LLQ_82_BLUE_TYPE1 "#359bff"

	typedef enum
	{
		LLQ_RED = 0,
		LLQ_GREEN,
		LLQ_BLUE,
		LLQ_YELLOW,
		LLQ_MAGENTA,
		LLQ_CYAN,
		LLQ_WHITE,
		LLQ_BLACK,
		LLQ_LIGHTGRAY,
		LLQ_DIMGRAY,
		LLQ_MAX_COLOR_NUM,
	} llq_82_color_enum;

	typedef struct
	{
		char *color_name;
		char *color_type;
	} llq_82_style_color;

	typedef enum
	{
		LVGL_GUI_MAIN,
		LVGL_GUI_MENU,
		LVGL_GUI_MAX
	} GUI_MENU;
	typedef enum
	{
		NAME_INPUT = 0,
		WORD_INPUT,
	} PASSWORD_INPUT_CTR;
	typedef struct
	{
		uint8_t last_enc;
		uint8_t cur_enc;
		GUI_MENU level1;		//界面选择
		uint8_t password_level; //密码名称界面
		uint8_t level3;
	} lvgl_state_ctr;
	typedef enum
	{
		LLQ_BACK_COLOR = 0,
		LLQ_SELECT_COLOR,
		LLQ_SETTING_COLOR,
		LLQ_NORMAL_COLOR,
		LLQ_INPUT_COLOR,
		LLQ_MAX_STYLE,
	} llq_style_enum;
	typedef struct
	{
		char background_color[10];
		char font_select_color[10];
		char font_setting_color[10];
		char font_normal_color[10];
		char font_input_color[10];
	} llq_lvgl_style;
	typedef struct
	{
		//llq_82_color_enum
		llq_82_color_enum code[5];
	} llq_lvgl_style_code;
	typedef struct
	{
		uint8_t ble_status : 1;
		uint8_t wifi_status : 1;
		uint8_t usb_status : 1;
		uint8_t charger_status : 1;
		uint8_t battery_elec;
	} widget_main_lable;
	typedef struct
	{
		struct
		{
			lv_obj_t *app_obj_main;
			lv_obj_t *icon;
			lv_obj_t *label_info;
			widget_main_lable lable_status;
		} main_win;
		struct
		{
			lv_obj_t *app_obj_widget;
			lv_obj_t *roller_t;

		} widget_main;
		struct
		{
			lv_color_t lvgl_back_ground;
			lv_obj_t *app_obj_widget;
			lv_obj_t *roller_t;
			lv_obj_t *lable[LVGL_MAX_LABLE];
		} widget1;
		llq_lvgl_style llq_style;
		llq_lvgl_style_code llq_style_code;
		lvgl_state_ctr gui_key_ctr;
	} lvgl_app_info;

	void set_lvgl_def(llq_lvgl_style_code *plvgl_style);
	void set_lvgl_style(llq_lvgl_style_code *plvgl_style);
	void llq_lvgl_set_style_code(llq_style_enum pStyle, llq_82_color_enum pColor);

	void lvgl_app_init(lvgl_app_info *lvgl_app_t);
	void lv_app_run(lvgl_app_info *lvgl_app_t);
	void lv_set_gui_lable(uint8_t num, char *str, uint8_t id, uint8_t enable);
	void lv_set_gui_lable_select(uint8_t num, char *str, uint8_t id, uint8_t enable,uint8_t select);
	void lv_set_gui_setting(uint8_t num, char *str, uint8_t id, uint8_t enable);
	void lv_set_gui_input(uint8_t num, char *str, uint8_t id, uint8_t enable);
	void lv_set_bel_wifi(uint8_t ble_t, uint8_t wifi_t, uint8_t usb_t);
	void lv_bat_set_value(uint8_t value, uint8_t ischarger);
	void lv_app_select(GUI_MENU level_t);
	void lv_main_lable_updata(void);
	void lv_color_code_config(uint8_t color_id, uint8_t get_finger, uint8_t enable);

#ifdef __cplusplus
}
#endif

#endif