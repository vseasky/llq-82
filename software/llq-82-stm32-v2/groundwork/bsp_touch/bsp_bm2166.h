/*** 
 * @author                          : Seasky.Liu
 * @Date                            : 2021-11-22 14:56:38
 * @LastEditTime: 2022-02-06 14:43:57
 * @LastEditors: Please set LastEditors
 * @Description                     : https://github.com/SEASKY-Master
 * @FilePath                        : \keyboard_v6\groundwork\bsp_touch\bsp_bm2166.h
 * @symbol_custom_string_obkoro1    : 
 * @版权所有:@Seasky.liu
 * @联系方式:liuwei_seasky@163.com
 * @开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途
 * @************************************************************************
 * @           If you want a thing done well, do it yourself.
 * @************************************************************************
 */
#ifndef __AS608_H
#define __AS608_H


#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "struct_typedef.h"

#define PS_Sta   PAin(6)//读指纹模块状态引脚
#define CharBuffer1 	0x01
#define CharBuffer2 	0x02

#define BMC_ADDR 		0XFFFFFFFF

#define FRAME_HEAD   	0XEF01

#define BM_GET_IMAGE 		0X01
#define BM_GENCHAE			0X02
#define BM_MATCH			0X03	//精确对比指纹
#define BM_REG_MODE			0X05	//合并指纹特征
#define BM_STORECHAR		0X06 	//保存特征
#define BM_EMPTY			0X0D	
#define BM_HIGH_SEARCH		0X1B
#define ValidTempleteNum	0X1D
#define BM_GETENROLLIMAGE	0X29
#define BM_HAND_SHAKE		0X35	//握手指令
#define BM_LED_CFG          0X3C

#define BM2166_PACK     	0X01
//应答
#define BM2166_ACK			0X07

#define BM_ACK_NO  			 0
#define BM_ACK_WAIT 	     1
#define BM_ACK_OK   		 2
#define BM_ACK_MAX_WAIT_TIME 300


#define TOUCH_MAX_RUN_COUNT 	10 
#define TOUCH_MAX_FINGER_COUNT 	300
#define TOUCH_INPUT_COUNT		2	//指纹录入次数
#define TOUCH_ADD_OK      		0
#define TOUCH_ADD_ERR			1
typedef enum
{
	ACK_DONE_OK 		= 	0X00,//表示指令执行完毕或OK；
	ACK_RX_ERROR 		=	0X01,//表示数据包接收错误；
	ACK_NO_TOUCH		=	0X02,//表示传感器上没有手指；
	ACK_FINGER_ENTER_ERR =  0X03,//表示录入指纹图像失败；
	ACK_IMAGE_DRY_LIGHT	=	0X04,//表示指纹图像太干、太淡而生不成特征；
	ACK_IMAGE_WET_MUDDY	=	0X05,//表示指纹图像太湿、太糊而生不成特征；
	ACK_IMAGE_MESSY		=	0X06,//表示指纹图像太乱而生不成特征；
	ACK_IMAGE_SMALL		=	0X07,//表示指纹图像正常，但特征点太少（或面积太小）而生不成特征；
	ACK_IMAGE_NOT_MATCH	=	0X08,//表示指纹不匹配；
	ACK_NO_FINGER_F		=	0X09,//表示没搜索到指纹；
	ACK_MERGE_FAILED	=	0X0A,//表示特征合并失败；
	ACK_OUT_RANGE		=	0X0B,//表示访问指纹库时地址序号超出指纹库范围；
	ACK_WRONG_INVALID	=	0X0C,//表示从指纹库读模板出错或无效；
	ACK_UPLOAD_FAILED	=	0X0D,//表示上传特征失败；
	ACK_RECEIVE_NOT		=	0X0e,//表示模块不能接收后续数据包；
	ACK_IMG_UP_FAILED	=	0X0f,//表示上传图像失败；
	ACK_FAILED_DEL_TEMP	=	0X10,//表示删除模板失败；
	ACK_CLEAR_FAILED	=	0X11,//表示清空指纹库失败；
	ACK_LOW_POWER_NO_E	=	0X12,//表示不能进入低功耗状态；
	ACK_PASS_INCORRECT	=	0X13,//表示口令不正确；
	ACK_RES_FAILED		=	0X14,//表示系统复位失败；
	ACK_NO_VALID		=	0X15,//表示缓冲区内没有有效原始图而生不成图像；
	ACK_UPGRADE_FAILED	=	0X16,//表示在线升级失败；
	ACK_NOT_MOVE		=	0X17,//表示残留指纹或两次采集之间手指没有移动过；
	ACK_FLASH_WR_ERR	=	0X18,//表示读写 FLASH 出错；
	ACK_AFTER_REC		=	0Xf0,//有后续数据包的指令，正确接收后用 0xf0 应答； f1,//有后续数据包的指令，命令包用 0xf1 应答；
	ACK_FLASH_SUM_ERR	=	0Xf2,//表示烧写内部 FLASH 时，校验和错误；
	ACK_FLASH_PACK_ERR	=	0Xf3,//表示烧写内部 FLASH 时，包标识错误；
	ACK_FLASH_LEN_ERR	=	0Xf4,//表示烧写内部 FLASH 时，包长度错误；
	ACK_FLASH_CODE_ERR	=	0Xf5,//表示烧写内部 FLASH 时，代码长度太长；
	ACK_FLASH_PRO_ERR	=	0Xf6,//表示烧写内部 FLASH 时，烧写 FLASH 失败；
	ACK_RANDOW_FAILED	=	0X19,//随机数生成失败；
	ACK_INVALID_REG		=	0X1a,//无效寄存器号；
	ACK_ERR_NUMBER		=	0X1b,//寄存器设定内容错误号；
	ACK_NOTE_ERR		=	0X1c,//记事本页码指定错误；
	ACK_PORT_FAILED		=	0X1d,//端口操作失败；
	ACK_ENROLL_FAILED	=	0X1e,//自动注册（enroll）失败；
	ACK_LIB_FULL		=	0X1f,//指纹库满；
	ACK_DEV_ADDR_ERR	=	0X20,//设备地址错误；
	ACK_PASS_WRONG		=	0X21,//密码有误；
	ACK_TEMP_NOT_ENPTY	=	0X22 ,//指纹模板非空；
	ACK_TEMP_EMPTY		=	0X23 ,//指纹模板为空；
	ACK_LIB_EMPTY	=	0X24 ,//指纹库为空；
	ACK_ENTRIES_ERR	=	0X25 ,//录入次数设置错误；
	ACK_TIMEOUT		=	0X26 ,//超时；
	ACK_ALREADY_EXISTS	=	0X27 ,//指纹已存在；
	ACK_TEMP_RELATED	=	0X28 ,//指纹模板有关联；
	ACK_INIT_ERR	=	0X29 ,//传感器初始化失败；

	//用于配置默认值，检测是否发生回调
	ACK_NORMAL      =   0XFF,
	//Reserved
	//0X2AH—ef,
}ack_enum;

//共用状态机
typedef enum
{
	STATE_PRESS_NO = 0, //默认指纹未按下
	STATE_PRESS_READY,	//指纹按下，准备开始处理
	STATE_PRESS_RUN,	//已经开始处理
	STATE_SEARCH,
	STATE_MATCH,		//精确对比指纹
	STATE_REG_MODEL,	//合并特征
	STATE_STORECHAR,	//保存特征文件
	STATE_PRESS_END,	//指纹处理结束
}state_machine;
typedef struct
{
	uint16_t frame_header;
	uint32_t frame_addr;
	uint16_t frame_logo;
	uint16_t frame_len;
	uint16_t frame_ins;
	uint16_t checksum;
	uint16_t all_len;
	uint8_t  rec_is_ok;
	/*根据指令处理返回消息*/
	uint8_t  ack_ctr_cmd;
	uint8_t  ack_rec_ok;//如果上一帧未收到，将暂时不允许发送指令
}bm2166_calc;

typedef struct
{
	state_machine press_t;
	uint8_t       run_count;
	bm2166_calc   frame_calc_t;
	uint8_t 	  hand_shake_s;	//握手状态,1：握手失败，0：握手成功，使用前应该标记为1
	ack_enum 	  get_image_t; 	//验证指纹时指纹状态
	ack_enum	  genchar_t;	//生成指纹特征状态
	ack_enum	  regmode_t;	//合并特征状态
	ack_enum	  storechar_t;
	struct
	{
		ack_enum high_search_s;
		uint16_t page_id;	//指纹的ID
		uint16_t mathscore; //通过率
	}high_search_t;
	ack_enum	get_enroll_t;//注册指纹时指纹状态
	struct
	{
		ack_enum match_ack_t;
		uint16_t score;
	}match_t;
	uint16_t valid_num;//注册的有效模板个数
	uint8_t  empty_is_ok;
	void (*bm_transmit)(uint8_t *pData,uint16_t len);
}bm2166_touch;

void bm2166_init(void);


/*** 
 * @Author: Seasky.Liu
 * @Description: 发送握手信号
 * @param {*}
 * @return {*}
 */
void hand_shake_ask(void);

/*** 
 * @Author: Seasky.Liu
 * @Description: 发送读取指纹状态指令
 * @param {*}
 * @return {*}
 */
void get_iamge_ask(void);

/*** 
 * @Author: Seasky.Liu
 * @Description: 发送获取指纹图像指令
 * @param {*}
 * @return {*}
 */
void get_enrollImage_ask(void);

/*** 
 * @Author: Seasky.Liu
 * @Description: 发送获取有效指纹数量指令
 * @param {*}
 * @return {*}
 */
void validtemp_num_ask(void);

/*** 
 * @Author: Seasky.Liu
 * @Description: 发送指纹检测指令
 * @param {uint8_t} buffer_id
 * @return {*}
 */
void genchar_ask(uint8_t buffer_id);

/*** 
 * @Author: Seasky.Liu
 * @Description: 发送指纹快速搜索指令
 * @param {uint8_t} buffer_id
 * @param {uint16_t} start_page
 * @param {uint16_t} pagenum
 * @return {*}
 */
void high_speed_search_ask(uint8_t buffer_id,uint16_t start_page,uint16_t pagenum);


/*** 
 * @Author: Seasky.Liu
 * @Description: 接收数据处理回调函数
 * @param {uint8_t} *pData
 * @param {uint16_t} rec_len
 * @return {*}
 */
void bsp_bm2166_receive(uint8_t *pData,uint16_t rec_len);



/*** 
 * @Author: Seasky.Liu
 * @Description: 自动识别指纹
 * @param {*}
 * @return {*}
 */
void auto_finger_detect(void);


/**
 * @Author: Seasky.Liu
 * @Description: 添加指纹，重新设置指纹
 * @param {uint16_t} page_id
 * @return {*}
 */
uint8_t add_finger(uint16_t page_id);


/**
 * @Author: Seasky.Liu
 * @Description: 清空指纹
 * @param {*}
 * @return {*}
 */
void finger_empty(void);


/**
 * @Author: Seasky.Liu
 * @Description: 设置灯效
 * @param {uint8_t} ctr_t
 * @param {uint8_t} speed_t
 * @param {uint8_t} color_t
 * @param {uint8_t} count_t
 * @return {*}
 */
void config_led_ask(uint8_t ctr_t,uint8_t speed_t,uint8_t color_t,uint8_t count_t);


void bm2166_check_err_time(void);

#ifdef __cplusplus
}
#endif

#endif

