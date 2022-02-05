/***
 * @Author                          : Seasky.Liu
 * @LastEditors                     : Seasky.Liu
 * @FilePath                        : \keyboard_v7\applications\flash_task.h
 * @Description                     : https://github.com/SEASKY-Master
 * @symbol_custom_string_obkoro1    :
 * @版权所有: @Seasky.Liu
 * @联系方式: liuwei_seasky@163.com
 * @开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途,违权必究
 * @************************************************************************
 * @				If you want a thing done well, do it yourself.
 * @************************************************************************
 */

#ifndef _FLASH_TASK_H
#define _FLASH_TASK_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "bsp_flash.h"
#include "bsp_password.h"
#include "lvgl_app.h"

#define USER_FALSH_PAGE ADDR_FLASH_SECTOR_11

#define FLASH_DELAY_TIME 500
#define FLASH_MAX_TRY_COUNT 5
#define NORMAL_LCD_SHOW_TIME 40
#define MIN_LCD_SHOW_TIME 20
//从flash读数据
#define cali_flash_read(address, buf, len) flash_read((address), (buf), (len))
//往一页flash写数据
#define cali_flash_write(address, buf, len) flash_write_single_address((address), (buf), (len))
//擦除flash
#define cali_flash_erase(address, page_num) flash_erase_address((address), (page_num))

	typedef enum
	{
		MCU_NORMAL_MODE = 0,
		MCU_STOP_MODE,
		MCU_SLEEP_MODE,
		MCU_STANDBY_MAX,
	} mcu_standby;
	typedef struct
	{
		uint8_t mcu_sleep_ctr; // mcu_standby
		uint8_t left_dir;
		uint8_t right_dir;
		uint8_t lcd_touch_power;
		uint8_t rgb_power;
		uint8_t sleep_time;
	} mcu_ctr;
	//该部分将会被存储到flash中,llq_store，不超过128k
	typedef struct
	{
		password_ctr bsp_password; //用户信息
		llq_lvgl_style_code lvgl_style;
		mcu_ctr llq_mcu_ctr;
		uint16_t crc_check;
	} llq_store;

	typedef enum
	{
		FLASH_NO_INIT = 0, // flash还未进行初始化
		FLASH_READY = 1,   // flash准备就绪
		FLASH_READ = 2,	   // flash正在进行读操作
		FLASH_WRITE = 3,   // flash正在进行写操作
		FLASH_ERR = 4,
	} llq_flash_status;
	typedef enum
	{
		FLASH_CMD_NO = 0,
		FLASH_CMD_READ,
		FLASH_CMD_READ_OK,
		FLASH_CMD_READ_ERR,
		FLASH_CMD_WRITE,
		FLASH_CMD_WRITE_OK,
		FLASH_CMD_WRITE_ERR,
	} llq_flash_cmd;
	//提供flash操作相关的接口
	typedef struct
	{
		llq_store llq_store_p;		  //需要存储的内容
		pass_word_info password_edit; //当前正在编辑的名称和密码，准备存入FLASH
		uint16_t password_index;	  //准备写入到那一条名称和密码
		uint8_t read_count;
		uint8_t write_count;

		uint8_t lcd_timer_count;
		uint8_t lcd_timer_enable;
		uint8_t lcd_power;
		llq_flash_status flash_status; // flash状态
		llq_flash_cmd flash_cmd;
	} llq_store_ctr;

	/**
	 * @Author          : Seasky.Liu
	 * @Date            : 2022-01-18 14:18:28
	 * @Description     : 初始化flash
	 * @param            {*}
	 * @return           {*}
	 */
	void flash_info_init(void);

	void flash_task_creat(void);
	/***
	 * @Author: Seasky.Liu
	 * @Description: 更新密码
	 * @param {char} *name
	 * @param {char} *word
	 * @param {uint8_t} num
	 * @return {*}
	 */

	void updata_password_edit(char *name, char *word, uint8_t num);

	/***
	 * @Author: Seasky.Liu
	 * @Description: 获取密码信息
	 * @param {char} *name
	 * @param {char} *word
	 * @param {uint8_t} num
	 * @return {*}
	 */
	void get_password_info(char *name, char *word, uint8_t num);

	void set_user_info(char *username, char *password, uint8_t num);

	void get_user_info(char *username, char *password, uint8_t num);

	void get_user_name(char *username, uint8_t num);

	void get_pass_word(char *password, uint8_t num);

	void flash_info_lvgl_style(llq_lvgl_style_code *pstyle_code);

	void flash_save_setting(void);

	uint8_t check_standby_normal(void);

	void set_standy_mode(mcu_standby standby_p);

	void flash_config_exit_standy(void);

	void flash_config_enter_standy(void);

	void flash_config_save_standy(void);

	void key_setting_show_info(uint8_t level, uint8_t enc_t, uint8_t enable);

	uint8_t get_flash_ble_enable(void);

	uint8_t get_flash_lcd_touch_enable(void);

	uint8_t get_flash_rgb_enable(void);

	void lcd_wake_up(void);

	void get_current_name(char *username);
	void get_current_word(char *password);
	void get_default_name(char *username);
	void get_default_word(char *password);
	void set_tx_current(uint8_t curr_num);
	void set_tx_default(uint8_t def_num);
	password_ctr *password_show(void);
#ifdef __cplusplus
}
#endif

#endif
