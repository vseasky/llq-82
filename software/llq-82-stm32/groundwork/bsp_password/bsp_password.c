/*
 * @author                          : Seasky.Liu
 * @Date                            : 2021-11-22 14:56:38
 * @LastEditTime: 2022-02-05 14:48:10
 * @LastEditors: Please set LastEditors
 * @Description                     : https://github.com/SEASKY-Master
 * @FilePath                        : \keyboard_v7\groundwork\bsp_password\bsp_password.c
 * @symbol_custom_string_obkoro1    :
 * 版权所有:@Seasky.liu
 * 联系方式:liuwei_seasky@163.com
 * 开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途
 * ************************************************************************
 *            If you want a thing done well, do it yourself.
 * ************************************************************************
 */
#include "stdio.h"
#include "bsp_password.h"

void set_pass_word_def(password_ctr *password_ctr_t)
{
    uint8_t i = 0;
    for (i = 0; i < MAX_PASS_NUM; i++)
    {
        snprintf(password_ctr_t->pass_word_t[i].user_name, MAX_USER_NAME_LEN, "password%d", i);
        snprintf(password_ctr_t->pass_word_t[i].pass_word, MAX_PASS_WORD_LEN, "12345678%d", i);
    }
    password_ctr_t->pass_word_num = MAX_PASS_NUM;
}

void set_username(password_ctr *password_ctr_t, char *pass_t, uint8_t num)
{
    if (num < MAX_PASS_NUM)
    {
        memset(password_ctr_t->pass_word_t[num].user_name, 0, sizeof(password_ctr_t->pass_word_t[num].user_name));
        strcat(password_ctr_t->pass_word_t[num].user_name, pass_t);
    }
}

void set_password(password_ctr *password_ctr_t, char *pass_t, uint8_t num)
{
    if (num < MAX_PASS_NUM)
    {
        memset(password_ctr_t->pass_word_t[num].pass_word, 0, sizeof(password_ctr_t->pass_word_t[num].pass_word));
        strcat(password_ctr_t->pass_word_t[num].pass_word, pass_t);
    }
}

void write_user_info(password_ctr *password_ctr_t, char *username, char *password, uint8_t num)
{
    if (num < MAX_PASS_NUM)
    {
        memset(password_ctr_t->pass_word_t[num].user_name, 0, sizeof(password_ctr_t->pass_word_t[num].user_name));
        memset(password_ctr_t->pass_word_t[num].pass_word, 0, sizeof(password_ctr_t->pass_word_t[num].pass_word));
        snprintf(password_ctr_t->pass_word_t[num].user_name, MAX_USER_NAME_LEN, username);
        snprintf(password_ctr_t->pass_word_t[num].pass_word, MAX_PASS_WORD_LEN, password);
    }
}

void read_user_info(password_ctr *password_ctr_t, char *username, char *password, uint8_t num)
{
    if (num < MAX_PASS_NUM)
    {
        snprintf(username, MAX_USER_NAME_LEN, password_ctr_t->pass_word_t[num].user_name);
        snprintf(password, MAX_PASS_WORD_LEN, password_ctr_t->pass_word_t[num].pass_word);
    }
}
void read_user_name(password_ctr *password_ctr_t, char *username, uint8_t num)
{
    if (num < MAX_PASS_NUM)
    {
        snprintf(username, MAX_USER_NAME_LEN, password_ctr_t->pass_word_t[num].user_name);
    }
}

void read_pass_word(password_ctr *password_ctr_t, char *password, uint8_t num)
{
    if (num < MAX_PASS_NUM)
    {
        snprintf(password, MAX_PASS_WORD_LEN, password_ctr_t->pass_word_t[num].pass_word);
    }
}

void read_current_name(password_ctr *password_ctr_t, char *username)
{
    if (password_ctr_t->pass_word_point < MAX_PASS_NUM)
    {
        snprintf(username, MAX_USER_NAME_LEN, password_ctr_t->pass_word_t[password_ctr_t->pass_word_point].user_name);
    }
}

void read_current_word(password_ctr *password_ctr_t, char *password)
{
    if (password_ctr_t->pass_word_point < MAX_PASS_NUM)
    {
        snprintf(password, MAX_PASS_WORD_LEN, password_ctr_t->pass_word_t[password_ctr_t->pass_word_point].pass_word);
    }
}

void read_default_name(password_ctr *password_ctr_t, char *username)
{
    if (password_ctr_t->pass_word_default_point < MAX_PASS_NUM)
    {
        snprintf(username, MAX_USER_NAME_LEN, password_ctr_t->pass_word_t[password_ctr_t->pass_word_default_point].user_name);
    }
}
void read_default_word(password_ctr *password_ctr_t, char *password)
{
    if (password_ctr_t->pass_word_default_point < MAX_PASS_NUM)
    {
        snprintf(password, MAX_PASS_WORD_LEN, password_ctr_t->pass_word_t[password_ctr_t->pass_word_default_point].pass_word);
    }
}