/*** 
 * @author                          : Seasky.Liu
 * @Date                            : 2021-11-22 14:56:38
 * @LastEditTime: 2022-02-05 14:50:51
 * @LastEditors: Please set LastEditors
 * @Description                     : https://github.com/SEASKY-Master
 * @FilePath                        : \keyboard_v6\groundwork\bsp_password\bsp_password.h
 * @symbol_custom_string_obkoro1    : 
 * @版权所有:@Seasky.liu
 * @联系方式:liuwei_seasky@163.com
 * @开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途
 * @************************************************************************
 * @           If you want a thing done well, do it yourself.
 * @************************************************************************
 */
#ifndef _BSP_PASSWORD_H
#define	_BSP_PASSWORD_H

 #ifdef __cplusplus
extern "C" {
#endif


#include "struct_typedef.h"

#define STR_PROCESS			4				//预留大小
#define MAX_USER_NAME_LEN 32+STR_PROCESS	//密码名称长度
#define MAX_PASS_WORD_LEN 32+STR_PROCESS	//密码长度
#define MAX_PASS_NUM	   24				//最大密码个数

typedef struct
{
	char user_name[MAX_USER_NAME_LEN];
	char pass_word[MAX_PASS_WORD_LEN];
}pass_word_info;
typedef  enum
{
	KEY_BOARD_NO_CAP,
	KEY_BOARD_IS_CAP
}iscapital;
typedef struct
{
	pass_word_info   pass_word_t[MAX_PASS_NUM];
	uint8_t 		 pass_word_num;
	uint8_t 		 pass_word_default_point;
	uint8_t			 pass_word_point;
	iscapital 		 iscapital_t;		//大小写需要适配,无线状态无法获取大小写
}password_ctr;


void set_pass_word_def(password_ctr *password_ctr_t);


void set_username(password_ctr *password_ctr_t,char *pass_t,uint8_t num);


void set_password(password_ctr *password_ctr_t,char *pass_t,uint8_t num);

void write_user_info(password_ctr *password_ctr_t, char *username, char *password, uint8_t num);

void read_user_info(password_ctr *password_ctr_t,char *username,char *password,uint8_t num);

void read_user_name(password_ctr *password_ctr_t,char *username,uint8_t num);


void read_pass_word(password_ctr *password_ctr_t,char *password,uint8_t num);


void read_current_name(password_ctr *password_ctr_t, char *username);
void read_current_word(password_ctr *password_ctr_t, char *password);
void read_default_name(password_ctr *password_ctr_t, char *username);
void read_default_word(password_ctr *password_ctr_t, char *password);


#ifdef __cplusplus
}
#endif

#endif

