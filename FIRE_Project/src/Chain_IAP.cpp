#include <M5Unified.h>
#include <M5GFX.h>
#include "driver/uart.h"
#include <UNIT_I2C.h>

M5GFX display;
M5Canvas canvas(&display);

#define TXD_PIN GPIO_NUM_21        // 串口发送端口(Tx)
#define RXD_PIN GPIO_NUM_22        // 串口接收端口(Rx)
#define UART_NUM UART_NUM_2        // 串口号
#define TX_BUFFER_SIZE 2060        // 发送缓冲区的大小
#define RECEIVE_START 0x99         // 开始接收数据
#define RECEIVE_ENABLE 0x88        // 告诉主机准备好接收数据包
#define RECEIVE_SUCCESS 0x77       // 接收成功
#define RECEIVE_FAIL 0x66          // 接收失败,重新发送
#define RECEIVE_ACCOMPLISH 0x55    // 接收完成,即将跳转APP
#define PACKET_DATA_CONTINUE 0x01  // 表示还有后续包
#define PACKET_DATA_END 0x02       // 表示最后一个包
#define SEPARATION_CHARACTER 0xff  // 分隔符
#define STM32G0xx_PAGE_SIZE 0x800  // FLASH的页大小
#define PACK_HEAD_HIGH 0xA0        // 包头
#define PACK_HEAD_LOW 0x55         // 包头
#define CHECK_PASS 1               // 检测通过
#define CHECK_ERROR 0              // 检测未通过
#define OPERATE_SUCCESS 1          // 操作成功
#define OPERATE_FAIL 0             // 操作失败
extern unsigned long appHexLength; // app的长度
unsigned long app_start_addr = 0x8002000; // app的起始地址
extern const unsigned char appHexData[]; // app的bin文件转换的unsigned char数组
uint8_t tx_buffer[TX_BUFFER_SIZE];        // 发送缓存区
uint16_t tx_size = 0;                     // 发送长度
uint8_t page_num = (appHexLength / 2048); // 页数从来0开始 范围：0~page_num
uint8_t iap_update_cmd[10] = {0xAA, 0x55, 0x04, 0x00, 0x01, 0xFF,
                              0x86, 0x86, 0x55, 0xAA};

// 更新APP
void uart_init(void);
void app_update(void);

// 串口初始化
void uart_init() {
  uart_config_t uart_config = {
      .baud_rate = 115200,                   // 波特率
      .data_bits = UART_DATA_8_BITS,         // 数据位
      .parity = UART_PARITY_DISABLE,         // 奇偶校验
      .stop_bits = UART_STOP_BITS_1,         // 停止位
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, // 流控
      .source_clk = UART_SCLK_APB            // 时钟

  };
  // --------------2. 设置UART引脚------------------------
  uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE,
               UART_PIN_NO_CHANGE);
  // --------------3. 初始化UART------------------------
  uart_param_config(UART_NUM, &uart_config);
  // --------------4. 安装驱动程序------------------------
  uart_driver_install(UART_NUM, 1024 * 2, 0, 0, NULL, 0);
}
void split_32bit_to_8bit(uint32_t add, uint8_t *bytes) {
  uint8_t index = 3;
  for (int i = 0; i < 4; i++) {
    bytes[index--] = (add >> (i * 8)) & 0xFF;
  }
}

void split_16bit_to_8bit(uint16_t value, uint8_t *buff) {
  // 高字节为16位整数的高8位
  buff[0] = (value >> 8) & 0xFF;
  // 低字节为16位整数的低8位
  buff[1] = value & 0xFF;
}

// 发送数据包
void send_update_data_pack(uint32_t add, uint16_t length, uint8_t page) {
  tx_size = 0;
  // 增加控制指令
  if (page < (page_num - 1)) // 表示不是最后一个包
  {
    tx_buffer[tx_size++] = PACKET_DATA_CONTINUE;
  } else // 表示是最后一个包
  {
    tx_buffer[tx_size++] = PACKET_DATA_END;
  }
  // 增加本次数据包flash起始地址
  uint8_t byte[4];
  split_32bit_to_8bit(add, byte);
  for (uint8_t i = 0; i < 4; i++) {
    tx_buffer[tx_size++] = byte[i];
  }
  // 增加本次传输的数据长度
  uint8_t le[2];
  split_16bit_to_8bit(length, le);
  for (uint8_t i = 0; i < 2; i++) {
    tx_buffer[tx_size++] = le[i];
  }

  // 增加分隔符
  tx_buffer[tx_size++] = SEPARATION_CHARACTER;
  // 增加APP数据
  uint8_t *ptr = (uint8_t *)(appHexData + page * STM32G0xx_PAGE_SIZE);
  for (uint16_t i = 0; i < length; i++) {
    tx_buffer[tx_size++] = ptr[i];
  }
  // 发送数据包
  uart_write_bytes(UART_NUM, tx_buffer, tx_size);
}

void setup(void) {
  M5.begin();
  uart_init();
  display.begin();
  canvas.setColorDepth(1);
  canvas.createSprite(display.width(), display.height());
  canvas.setTextSize(1.5);
  canvas.setPaletteColor(1, GREEN);
  canvas.setTextScroll(true);
  canvas.printf("    This update %d bytes of data \r\n", appHexLength);
  canvas.pushSprite(0, 0);
  canvas.printf("    Starting address 0x%x\r\n", app_start_addr);
  canvas.pushSprite(0, 0);
  canvas.printf("\r\n    Totaling %d packets\r\n", (page_num));
  canvas.pushSprite(0, 0);
}

void loop(void) {
  M5.update();
  uint8_t data_buf[256];
  // 出厂下载
  if (M5.BtnB.wasReleased() || M5.BtnB.pressedFor(500)) {
    uart_write_bytes(UART_NUM, iap_update_cmd, sizeof(iap_update_cmd));
    while (1) {
      delay(30);
      memset(data_buf, 0, sizeof(data_buf));
      int len = uart_read_bytes(UART_NUM, data_buf, 256, 10);
      if (len == 10 && data_buf[5] == 0xFF && data_buf[6] == 0x86 &&
          data_buf[7] == 0x86) {
        break;
      }
      uart_write_bytes(UART_NUM, iap_update_cmd, sizeof(iap_update_cmd));
    }
    uart_driver_delete(UART_NUM); // 关闭并卸载 UART 驱动
    // 唤醒IAP进入APP升级
    // 设置rx，tx引脚为低电平
    pinMode(TXD_PIN, OUTPUT);
    pinMode(RXD_PIN, OUTPUT);
    digitalWrite(TXD_PIN, LOW);
    digitalWrite(RXD_PIN, LOW);
    delay(1000);
    uart_init();
    app_update();
  }
}

// 更新APP
void app_update(void) {
  canvas.setCursor(0, 0);
  canvas.fillScreen(BLACK);
  canvas.setTextScroll(true);
  size_t size = 0;
  uint8_t start = 0x99;
  uint8_t data;
  uint8_t i;
  uart_write_bytes(UART_NUM, &start, 1);
  while (1) {
    delay(80);
    size = 0;
    uart_get_buffered_data_len(UART_NUM, &size); // 判断接收缓冲区是否接收到数据
    if (size > 0) {
      Serial.printf(" rx_data");
      for (i = 0; i < size; i++) {
        uart_read_bytes(UART_NUM, &data, 1, 10);
        Serial.printf(" %02X ", data);
        if (data == RECEIVE_ENABLE) // 从机成功接收到数据包
        {
          canvas.printf("ENABLE SUCCESS\r\n");
          canvas.pushSprite(0, 0);
          break;
        }
      }
      if (i != size) {
        break;
      }
    }
    uart_write_bytes(UART_NUM, &start, 1);
    Serial.printf("tx_data 0x%02x\r\n", start);
  }
  for (int i = 0; i < page_num; i++) {
    send_update_data_pack(app_start_addr + i * STM32G0xx_PAGE_SIZE,
                          STM32G0xx_PAGE_SIZE, i);
    size = 0;
    while (1) {
      uart_get_buffered_data_len(UART_NUM,
                                 &size); // 判断接收缓冲区是否接收到数据
      if (size > 0) {
        uart_read_bytes(UART_NUM, &data, 1, 0);
        if (data == RECEIVE_SUCCESS) {
          canvas.printf("data packet%d : send successful\r\n", (i + 1));
          canvas.pushSprite(0, 0);
          break;
        } else {
          canvas.printf("data packet%d : send fail\r\n", (i + 1));
          canvas.pushSprite(0, 0);
          send_update_data_pack(app_start_addr + i * STM32G0xx_PAGE_SIZE,
                                STM32G0xx_PAGE_SIZE, i); // 重新发送数据包
        }
      }
    }
  }
  while (1) {
    uart_get_buffered_data_len(UART_NUM, &size);
    if (size > 0) {
      uart_read_bytes(UART_NUM, &data, 1, 10);
      if (data == RECEIVE_ACCOMPLISH) {
        canvas.printf("Update completed\r\n");
        canvas.pushSprite(0, 0);
        break;
      }
    }
  }
  canvas.setTextScroll(false);
  delay(3000);
}
