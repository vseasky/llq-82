

//  A5 0C 02 00 00 00 04 00 00 00 00 FE

#include "bsp_uart.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "soc/uart_struct.h"

#include "esp_hidd_prf_api.h"
#include "hidd_le_prf_int.h"
#include "hid_dev.h"
#include "esp_log.h"

#define UART_TAG "UART_TAG"

#define EX_UART_NUM (UART_NUM_1)
#define ECHO_TEST_TXD (GPIO_NUM_17)
#define ECHO_TEST_RXD (GPIO_NUM_18)
// #define ECHO_TEST_RTS (GPIO_NUM_19)
// #define ECHO_TEST_CTS (GPIO_NUM_20)
#define ECHO_TEST_RTS (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS (UART_PIN_NO_CHANGE)

#define BUF_RX_SIZE (1024)
#define BUF_TX_SIZE (1024)

uart_isr_handle_t handle;
llq_struct_t llq_info =
    {
        .llq_status = LLQ_RESET,
        .battery_cap = 0,
};
static void IRAM_ATTR uart_irq_handler(void *param);

uint8_t uartRxBuf[200];
uint8_t uartTxBuf[24];
uint8_t buff_check = 0;
uint8_t uartRxCount = 0;
uint16_t recSizeLen = 0;

// HID keyboard input report length
#define UART_HID_KEYBOARD_IN_RPT_LEN 8

// HID LED output report length
#define UART_HID_LED_OUT_RPT_LEN 1

// HID mouse input report length
#define UART_HID_MOUSE_IN_RPT_LEN 5

// HID consumer control input report length
#define UART_HID_CC_IN_RPT_LEN 2

void mx_uart_init()
{
    esp_err_t r;
    //串口配置结构体
    uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,           //波特率
        .data_bits = UART_DATA_8_BITS,         //数据位
        .parity = UART_PARITY_DISABLE,         //校验位
        .stop_bits = UART_STOP_BITS_1,         //停止位
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, //流控位
        .source_clk = UART_SCLK_APB,
    };
    //设置串口
    uart_param_config(EX_UART_NUM, &uart_config);
    uart_set_pin(EX_UART_NUM, ECHO_TEST_TXD, ECHO_TEST_RXD, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(EX_UART_NUM, BUF_RX_SIZE , BUF_TX_SIZE, 0, NULL, 0);
    uart_isr_free(EX_UART_NUM);
    //创建自定义串口中断
    r = uart_isr_register(EX_UART_NUM, uart_irq_handler, NULL, ESP_INTR_FLAG_LOWMED | ESP_INTR_FLAG_IRAM, &handle);
    if (r != ESP_OK)
    {
        // printf("uart_isr_register error!\n");
    }
    else
    {
        // printf("uart_isr_register ok!\n");
    }
    uart_enable_rx_intr(EX_UART_NUM); //使能中断接收
    // uart_set_rx_timeout(EX_UART_NUM, 40);
}
static void llq_keyboard_updata(uint8_t report_id_t, uint8_t *buff)
{
    //如果处于未连接状态
    if (llq_info.llq_status != LLQ_CONNECT)
    {
        return;
    }
    switch (report_id_t)
    {
    case UART_REPORT_ID_MOUSE:
    {
        //鼠标
        hid_dev_send_report(hidd_le_env.gatt_if, 0,
                            HID_RPT_ID_MOUSE_IN, HID_REPORT_TYPE_INPUT, UART_HID_MOUSE_IN_RPT_LEN, buff);
    }
    break;
    case UART_REPORT_ID_KEY:
    {
        //键盘
        hid_dev_send_report(hidd_le_env.gatt_if, 0,
                            HID_RPT_ID_KEY_IN, HID_REPORT_TYPE_INPUT, UART_HID_KEYBOARD_IN_RPT_LEN, buff);
    }
    break;
    case UART_REPORT_ID_MEDIA:
    {
        //媒体
        esp_hidd_send_consumer_value(0, buff[0], buff[1]);
    }
    break;
    default:
        break;
    }
    return;
}
void llq_status_config(LLQ_STATE set_llq_status)
{
    llq_info.llq_status = set_llq_status;
}
void llq_status_updata(void)
{
    uint8_t ret = 0;
    uartTxBuf[0] = FRAME_HEAD;
    uartTxBuf[1] = UART_REPORT_ID_INFO; //消息ID
    uartTxBuf[2] = llq_info.battery_cap;//电池电量
    //连接状态
    uartTxBuf[3] = llq_info.llq_status; //
    // LED状态灯指示，目前ESP32无法获取，如果获取成功后，直接更新 （llq_info.keyboard_lock）即可，同时方便的话，可以给我也提供一份程序@滑稽
    uartTxBuf[4] = (((uint8_t)(llq_info.keyboard_lock.num_lock) << ENUM_NUM_LOCK) |
                    ((uint8_t)(llq_info.keyboard_lock.caps_lock) << ENUM_CAPS_LOCK) |
                    ((uint8_t)(llq_info.keyboard_lock.scroll_lock) << ENUM_SCROLL_LOCK) |
                    ((uint8_t)(llq_info.keyboard_lock.compose) << ENUM_COMPOSE) |
                    ((uint8_t)(llq_info.keyboard_lock.kana) << ENUM_KANA));
    // byte2~7保留
    uartTxBuf[5] = 0;  // 2
    uartTxBuf[6] = 0;  // 3
    uartTxBuf[7] = 0;  // 4
    uartTxBuf[8] = 0;  // 5
    uartTxBuf[9] = 0;  // 6
    uartTxBuf[10] = 0; // 7
    uartTxBuf[FRAME_LEN - 1] = FRAME_END;
    // uart_tx_chars(EX_UART_NUM,&uartTxBuf[0],12);
    // uart_write_bytes(EX_UART_NUM, &uartTxBuf[0], FRAME_END);
    ret = uart_tx_chars(EX_UART_NUM,((char*)uartTxBuf), FRAME_LEN);
}
//中断过长会直接复位
//为了提高效率，使用定长接收,同时不使用较复杂的协议
static void IRAM_ATTR uart_irq_handler(void *param)
{
    volatile uart_dev_t *uart = &UART1;
    uint8_t uartRxBuf1[200];
    static uint8_t uartRxCount = 0;
    static uint16_t recSizeLen = 0;
    uint8_t res;
    uart->int_clr.rxfifo_full_int_clr = 1;
    uart->int_clr.frm_err_int_clr = 1;
    //检查是否产生超时中断
    if (uart->int_st.rxfifo_tout_int_st)
    {
        uart->int_clr.rxfifo_tout_int_clr = 1;
        //获取本次接收长度
        recSizeLen = uart->status.rxfifo_cnt;
        //定长接收
        if (recSizeLen >= FRAME_LEN)
        {
            //将缓冲更新到buff
            while (uart->status.rxfifo_cnt)
            {
                uartRxBuf[uartRxCount] = (uart->fifo.val) & 0XFF;
                uartRxCount++;
            }
            //检验帧头
            if ((uartRxBuf[0] == FRAME_HEAD) && (uartRxBuf[FRAME_LEN - 1] == FRAME_END))
            {
                /*通知处理BUFF1*/
                llq_info.battery_cap = uartRxBuf[2];
                llq_keyboard_updata(uartRxBuf[1], &uartRxBuf[3]);
            }
            uartRxCount = 0; //恢复，然后重新计数
        }
        else if (recSizeLen > 0)
        {
            //不计入buff，但是清空FIFO
            while (uart->status.rxfifo_cnt)
            {
                res = (uart->fifo.val) & 0XFF;
            }
            uartRxCount = 0;
        }
    }
    return;
}