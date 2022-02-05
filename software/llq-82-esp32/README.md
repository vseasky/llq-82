# LLQ-82-ESP32说明书

## 前言

> 该程序是直接修改于`ble_hid_device_demo`，本人在此添加了串口和协议

## 使用说明

| `llq_status`    | 连接状态     | LLQ_STATE |
| --------------- | ------------ | --------- |
| `battery_cap`   | 电池电量     | uint8_t   |
| `keyboard_lock` | 键盘Lock指示 | enum_lock |

> 注意本人并没有完成`ESP32`的电池电量更新和键盘`Lock`指示功能，但是在协议中预留了接口，你可以尝试修改修改程序以获取`PC`端的 `lock`指示灯状态、和尝试添加电池服务，将电池电量更新到`PC`蓝牙指示图标。

### 消息类型

```
#define UART_REPORT_ID_MOUSE 0X01//鼠标消息
#define UART_REPORT_ID_KEY   0X02//键盘消息
#define UART_REPORT_ID_MEDIA 0X03//媒体消息
#define UART_REPORT_ID_INFO  0X04//自定义消息
```

### `ESP32-UART` 配置

| 波特率 | 256000                   |
| ------ | ------------------------ |
| 数据位 | UART_DATA_8_BITS         |
| 校验位 | UART_PARITY_DISABLE      |
| 停止位 | UART_STOP_BITS_1         |
| 流控位 | UART_HW_FLOWCTRL_DISABLE |

### `ESP32-TX`协议说明

| byte0    | `0XA5`                | 固定帧头           |
| -------- | --------------------- | ------------------ |
| byte1    | `UART_REPORT_ID_INFO` | 自定义数据         |
| byte2    | `battery_cap`         | 电池电量           |
| byte3    | `llq_status`          | 蓝牙链接状态       |
| byte4    | `keyboard_lock`       | PC`lock`指示灯状态 |
| byte5~10 | 保留                  | 保留               |
| byte11   | `0XFE`                | 固定帧尾           |

### `ESP32-RX`协议说明

| byte0    | `0XA5`        | 固定帧头     |
| -------- | ------------- | ------------ |
| byte1    | `hid report`  | HID `report` |
| byte2    | `battery_cap` | 电池电量     |
| byte3~10 | `llq_status`  | HID消息      |
| byte11   | `0XFE`        | 固定帧尾     |

###  `report id` ->>  hid_buff[8]

##### `UART_REPORT_ID_MOUSE`

    鼠标每次发送4个字节
        BYTE1 –
            bit7: 1 表示 Y 坐标的变化量超出－256 ~ 255的范围,0表示没有溢出
            bit6: 1 表示 X 坐标的变化量超出－256 ~ 255的范围，0表示没有溢出
            bit5: Y 坐标变化的符号位，1表示负数，即鼠标向下移动
            bit4: X 坐标变化的符号位，1表示负数，即鼠标向左移动
            bit3: 恒为1
            bit2: 1表示中键按下
            bit1: 1表示右键按下
            bit0: 1表示左键按下
        BYTE2 – X坐标变化量，与byte的bit4组成9位符号数,负数表示向左移，正数表右移。用补码表示变化量
        BYTE3 – Y坐标变化量，与byte的bit5组成9位符号数，负数表示向下移，正数表上移。用补码表示变化量
        BYTE4 – 滚轮变化。


##### `UART_PARITY_DISABLE`

    键盘发送给PC的数据每次8个字节
        BYTE0 BYTE1 BYTE2 BYTE3 BYTE4 BYTE5 BYTE6 BYTE7
        定义分别是：
        BYTE0 --
                        |--bit0:   Left Control是否按下，按下为1
                        |--bit1:   Left Shift  是否按下，按下为1
                        |--bit2:   Left Alt    是否按下，按下为1
                        |--bit3:   Left GUI    是否按下，按下为1
                        |--bit4:   Right Control是否按下，按下为1
                        |--bit5:   Right Shift 是否按下，按下为1
                        |--bit6:   Right Alt   是否按下，按下为1
                        |--bit7:   Right GUI   是否按下，按下为1
        BYTE1 -- 暂不清楚，有的地方说是保留位
        BYTE2--BYTE7 -- 这六个为普通按键
        键盘经过测试。

##### `UART_REPORT_ID_MEDIA`

    媒体发送给PC的数据每次2个字节
        BYTE0 -- key_cmd        ->  媒体按键
        BYTE1 -- key_pressed    ->  是否按下