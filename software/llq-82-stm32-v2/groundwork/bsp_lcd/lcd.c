/*
 * @FilePath                        : \keyboard_v6\groundwork\bsp_lcd\lcd.c
 * @author                          : Seasky.Liu
 * @Date                            : 2021-11-24 09:25
 * @LastEditors                     : your name
 * @Description                     : https://github.com/SEASKY-Master
 * @symbol_custom_string_obkoro1    : 
 * 版权所有:@Seasky.liu
 * 联系方式:liuwei_seasky@163.com
 * 开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途
 * ************************************************************************
 *            If you want a thing done well, do it yourself.
 * ************************************************************************
 */
#include "lcd.h"
#include "lcd_init.h"
#include "stm32f4xx_hal.h"
#include "spi.h"

/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：xsta,ysta   起始坐标
                xend,yend   终止坐标
								color       要填充的颜色
      返回值：  无
******************************************************************************/
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{
    u16 i,j;
    LCD_Address_Set(xsta,ysta,xend-1,yend-1);//设置显示范围
    for(i=ysta; i<yend; i++)
        {
            for(j=xsta; j<xend; j++)
                {
                    LCD_WR_DATA(color);
                }
        }
}

/******************************************************************************
      函数说明：在指定位置画点
      入口数据：x,y 画点坐标
                color 点的颜色
      返回值：  无
******************************************************************************/

void LCD_DrawPoint(u16 x,u16 y,u16 color)
{
    LCD_Address_Set(x,y,x,y);//设置光标位置
    LCD_WR_DATA(color);
}


void LCD_DrawPoint_DMA(u16 x,u16 y,u16 color)
{
    LCD_WR_REG(0x2a);//列地址设置
    LCD_WR_DATA(x+40);
    LCD_WR_DATA(x+40);
    LCD_WR_REG(0x2b);//行地址设置
    LCD_WR_DATA(y+53);
    LCD_WR_DATA(y+53);
    LCD_WR_REG(0x2c);//储存器写
    LCD_WR_DATA(color);
}

void LCD_Fill_DMA(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,const uint8_t *pic)
{
    uint16_t length;
    uint16_t width;
    length = x2 - x1 + 1 ;
    width  = y2 - y1 + 1;
	//非DMA SPI设置刷屏区域
	LCD_Address_Set(x1,y1,x2,y2);
	//DMA SPI方式刷屏
	HAL_SPI_Transmit_DMA(&hspi1,&pic[0],length*width*2);
	while(hspi1.State == HAL_SPI_STATE_BUSY_TX);
}

///******************************************************************************
//      函数说明：显示图片
//      入口数据：x,y起点坐标
//                length 图片长度
//                width  图片宽度
//                pic[]  图片数组
//      返回值：  无
//******************************************************************************/
void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width,const u8 pic[])
{
    u16 i,j;
    u32 k=0;
    LCD_Address_Set(x,y,x+length-1,y+width-1);
    for(i=0; i<length; i++)
        {
            for(j=0; j<width; j++)
                {
                    LCD_WR_DATA8(pic[k*2]);
                    LCD_WR_DATA8(pic[k*2+1]);
                    k++;
                }
        }
}


