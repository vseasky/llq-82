/***
 * @author                          : Seasky.Liu
 * @LastEditors                     : your name
 * @Description                     : https://github.com/SEASKY-Master
 * @FilePath                        : \keyboard_v5\applications\keyboard_task.h
 * @symbol_custom_string_obkoro1    :
 * @版权所有:@Seasky.liu
 * @联系方式:liuwei_seasky@163.com
 * @开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途
 * @************************************************************************
 * @           If you want a thing done well, do it yourself.
 * @************************************************************************
 */

#ifndef _KEY_BOARD_TASK_H
#define _KEY_BOARD_TASK_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "struct_typedef.h"
#include "main.h"

	void keyboard_task_creat(void);

	/***
	 * @Author: Seasky.Liu
	 * @Description: 矩阵按键中断服务函数
	 * @param {*}
	 * @return {*}
	 */
	void key_scanf_irq(void);

	/***
	 * @Author: Seasky.Liu
	 * @Description: 字符串方式输出
	 * @param {char} *str
	 * @return {*}
	 */
	void str_to_key_msg(char *str);
	void keyboard_user_info_get(uint8_t num);
	void keyboard_user_info_set(uint8_t num);
	void keyboard_input_mode(void);
	void keyboard_input_exit(void);
	void keyboard_mode_reset(void);
	void keyboard_buffer_show(uint8_t options, uint8_t check);
	void keyboard_buffer3_show(uint8_t options);
	void usb_connect_disable(void);
    void get_now_mac(void);
	void set_now_mac(void);
	void media_play(void);
	void media_next_track(void);
	void media_previous_track(void);
	void media_volume_increment(void);
	void media_volume_decrement(void);
	void media_mute(void);
	void media_paus(void);
	void media_stop(void);
#ifdef __cplusplus
}
#endif
#endif
