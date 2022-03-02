/***
 * @Author                          : Seasky.Liu
 * @LastEditors: Please set LastEditors
 * @FilePath                        : \keyboard_v7\applications\touch_tack.h
 * @Description                     : https://github.com/SEASKY-Master
 * @symbol_custom_string_obkoro1    :
 * @版权所有: @Seasky.Liu
 * @联系方式: liuwei_seasky@163.com
 * @开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途,违权必究
 * @************************************************************************
 * @				If you want a thing done well, do it yourself.
 * @************************************************************************
 */

#ifndef _TOUCH_TASK_H
#define _TOUCH_TASK_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "bsp_bm2166.h"

    typedef enum
    {
        TOUCH_IDLE = 0,
        TOUCH_GET_NUM,
        TOUCH_AUTO_DELETE,
        TOUCH_AUTO_ADD,
        TOUCH_AUTO_DELECT,
    } touch_task_enum;

    typedef enum
    {
        TOUCH_SEND_NULL = 0,
        TOUCH_SEND_DEF_WORD = 1,
        TOUCH_SEND_DEF_NAME = 2,
        TOUCH_SEND_CUR_WORD = 3,
        TOUCH_SEND_CUR_NAME = 4,
        TOUCH_SEND_MAX,
    } touch_send_ctr;

    typedef struct
    {
        touch_task_enum touch_status;
        uint8_t touch_num_get_ok;
        uint16_t touch_num;
        uint8_t touch_add_num;
        uint8_t touch_add_count;
        touch_send_ctr touch_send_type;
        touch_send_ctr touch_send_ready;
        struct
        {
            uint8_t delect_status;
            uint16_t page_id;   //指纹的ID
            uint16_t mathscore; //通过率
        } touch_delect;
    } touch_task_info;

    void touch_task_creat(void);
    void touch_task_irq(void);
    void touch_show_info(uint8_t level, uint8_t enc_t, uint8_t enable);
    void touch_num_callback(uint16_t num_t);
    void touch_delect_callback(uint8_t ok_t, uint16_t page_id_t, uint16_t mathscore_t);
    void touch_tx_ready(touch_send_ctr touch_send_p);
    uint8_t get_touch_page(void);

#ifdef __cplusplus
}
#endif
#endif
