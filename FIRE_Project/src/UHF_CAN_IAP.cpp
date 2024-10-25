// // 用小黑32开发板，通过CAN总线连接ESP32，接收CAN数据，显示在屏幕上

// #include <CAN_config.h>
// #include <ESP32CAN.h>
// #include <M5GFX.h>
// #include <M5Unified.h>
// #include <UNIT_I2C.h>

// #define TXD_PIN GPIO_NUM_17       // 串口发送端口(Tx)
// #define RXD_PIN GPIO_NUM_16       // 串口接收端口(Rx)
// #define UART_NUM UART_NUM_2       // 串口号
// #define TX_BUFFER_SIZE 2060       // 发送缓冲区的大小
// #define RECEIVE_START 0x99        // 开始接收数据
// #define RECEIVE_ENABLE 0x88       // 告诉主机准备好接收数据包
// #define RECEIVE_SUCCESS 0x77      // 接收成功
// #define RECEIVE_FAIL 0x66         // 接收失败,重新发送
// #define RECEIVE_ACCOMPLISH 0x55   // 接收完成,即将跳转APP
// #define PACKET_DATA_CONTINUE 0x01 // 表示还有后续包
// #define PACKET_DATA_END 0x02      // 表示最后一个包
// #define SEPARATION_CHARACTER 0xff // 分隔符
// #define STM32G0xx_PAGE_SIZE 0x800 // FLASH的页大小
// #define PACK_HEAD_HIGH 0xA0       // 包头
// #define PACK_HEAD_LOW 0x55        // 包头
// #define CHECK_PASS 1              // 检测通过
// #define CHECK_ERROR 0             // 检测未通过
// #define OPERATE_SUCCESS 1         // 操作成功
// #define OPERATE_FAIL 0            // 操作失败
// uint32_t can_id = 0x302;
// extern unsigned long appHexLength;        // app的长度
// unsigned long app_start_addr = 0x8003000; // app的起始地址
// extern const unsigned char appHexData[]; // app的bin文件转换的unsigned char数组
// uint8_t tx_buffer[TX_BUFFER_SIZE];        // 发送缓存区
// uint16_t tx_size = 0;                     // 发送长度
// uint8_t page_num = (appHexLength / 2048); // 页数从来0开始 范围：0~page_num
// uint8_t start_cmd[3] = {0xAA, 0x55, 0x99};
// CAN_device_t CAN_cfg;             // CAN Config
// unsigned long previousMillis = 0; // will store last time a CAN Message was send
// const int interval = 1000; // interval at which send CAN Messages (milliseconds)
// const int rx_queue_size = 10; // Receive Queue size
// uint32_t can_rec_id = 0;
// uint8_t can_rec_buf[20];
// uint8_t count = 0;

// M5GFX display;
// M5Canvas canvas(&display);

// uint8_t iap_update_cmd[5] = {0xA0, 0x55, 0x02, 0xFF, 0x0A};
// uint8_t iap_raw_off[5] = {0xA0, 0x03, 0x01, 0xE1, 0x7B};
// uint8_t iap_raw_on[5] = {0xA0, 0x55, 0x02, 0xE0, 0x29};

// // 更新APP
// void can_init(void);
// void view_display(void);
// void app_update(void);

// void can_init(void) {
//   CAN_cfg.speed = CAN_SPEED_1000KBPS;
//   CAN_cfg.tx_pin_id = TXD_PIN;
//   CAN_cfg.rx_pin_id = RXD_PIN;
//   CAN_cfg.rx_queue = xQueueCreate(rx_queue_size, sizeof(CAN_frame_t));
//   // Init CAN Module
//   ESP32Can.CANInit();
// }
// uint8_t can_receive(uint8_t *buf, uint32_t *id) {
//   static CAN_frame_t rx_frame;
//   if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) ==
//       pdTRUE) {
//     if (rx_frame.FIR.B.FF == CAN_frame_std) {
//       //       Serial.println("receive standard frame");
//     } else {
//       //       Serial.println("receive extended frame");
//     }

//     if (rx_frame.FIR.B.RTR == CAN_RTR) {
//       //       Serial.printf("RTR from 0x%08X, DLC %d\r\n", rx_frame.MsgID,
//       //                     rx_frame.FIR.B.DLC);
//     } else {
//       //       Serial.printf("from 0x%08X, DLC %d, Data \r\n", rx_frame.MsgID,
//       //                     rx_frame.FIR.B.DLC);
//       can_rec_id = rx_frame.MsgID;
//       for (int i = 0; i < rx_frame.FIR.B.DLC; i++) {
//         buf[i] = rx_frame.data.u8[i];
//         //         Serial.printf("0x%02X ", rx_frame.data.u8[i]);
//       }
//       //       Serial.println("\n");
//       return rx_frame.FIR.B.DLC;
//     }
//   }
//   return 0;
// }

// void can_send(uint8_t *buf, uint16_t size) {
//   uint16_t index = 0;
//   CAN_frame_t tx_frame;
//   tx_frame.FIR.B.FF = CAN_frame_std;
//   tx_frame.MsgID = can_id;
//   while (1) {
//     if (size > 8) {
//       tx_frame.FIR.B.DLC = 8;
//       for (uint8_t i = 0; i < 8; i++) {
//         tx_frame.data.u8[i] = buf[index++];
//       }
//       ESP32Can.CANWriteFrame(&tx_frame);
//       size = size - 8;
//     } else {
//       tx_frame.FIR.B.DLC = size;
//       for (uint8_t i = 0; i < size; i++) {
//         tx_frame.data.u8[i] = buf[index];
//         index++;
//       }
//       ESP32Can.CANWriteFrame(&tx_frame);
//       break;
//     }
//   }
// }

// void split_32bit_to_8bit(uint32_t add, uint8_t *bytes) {
//   uint8_t index = 3;
//   for (int i = 0; i < 4; i++) {
//     bytes[index--] = (add >> (i * 8)) & 0xFF;
//   }
// }

// void split_16bit_to_8bit(uint16_t value, uint8_t *buff) {
//   // 高字节为16位整数的高8位
//   buff[0] = (value >> 8) & 0xFF;
//   // 低字节为16位整数的低8位
//   buff[1] = value & 0xFF;
// }

// // 发送数据包
// void send_update_data_pack(uint32_t add, uint16_t length, uint8_t page) {
//   tx_size = 0;
//   // 增加控制指令
//   if (page < (page_num - 1)) // 表示不是最后一个包
//   {
//     tx_buffer[tx_size++] = PACKET_DATA_CONTINUE;
//   } else // 表示是最后一个包
//   {
//     tx_buffer[tx_size++] = PACKET_DATA_END;
//   }
//   // 增加本次数据包flash起始地址
//   uint8_t byte[4];
//   split_32bit_to_8bit(add, byte);
//   for (uint8_t i = 0; i < 4; i++) {
//     tx_buffer[tx_size++] = byte[i];
//   }
//   // 增加本次传输的数据长度
//   uint8_t le[2];
//   split_16bit_to_8bit(length, le);
//   for (uint8_t i = 0; i < 2; i++) {
//     tx_buffer[tx_size++] = le[i];
//   }
//   // 增加分隔符
//   tx_buffer[tx_size++] = SEPARATION_CHARACTER;
//   // 增加APP数据
//   uint8_t *ptr = (uint8_t *)(appHexData + page * STM32G0xx_PAGE_SIZE);
//   for (uint16_t i = 0; i < length; i++) {
//     tx_buffer[tx_size++] = ptr[i];
//   }
//   // 发送数据包
//   can_send(tx_buffer, tx_size);
// }

// void setup() {
//   M5.begin();
//   can_init();
//   display.begin();
//   canvas.setColorDepth(1); // mono color
//   canvas.createSprite(display.width(), display.height());
//   canvas.setTextSize(1.5);
//   canvas.setPaletteColor(1, GREEN);
//   canvas.setTextScroll(true);
//   canvas.println("Init CAN Module.....");
//   canvas.println("Waiting Frame");
//   canvas.println(
//       "If there are phenomena such as failure of normal communication");
//   canvas.println(
//       "Please check the communication rate or add a terminal resistance "
//       "between the H and L phases");
//   canvas.pushSprite(0, 0);
// }

// void loop() {
//   M5.update();
//   uint8_t len = 0;
//   // 出厂下载
//   if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(500)) {
//     can_id = 0x678;
//     can_send(iap_update_cmd, sizeof(iap_update_cmd));
//     len = can_receive(can_rec_buf, &can_rec_id);
//     while (!(len == 6 && can_rec_id == can_id && can_rec_buf[3] == 0xFF &&
//              can_rec_buf[4] == 0x10)) {
//       delay(40);
//       len = can_receive(can_rec_buf, &can_rec_id);
//       can_send(iap_update_cmd, sizeof(iap_update_cmd));
//     }
//     can_id = 0x302;
//     app_update();
//   }
//   // 功能检测···待完善
//   if (M5.BtnB.wasReleased() || M5.BtnB.pressedFor(500)) {
//     can_id = 0x678;
//     can_send(iap_raw_on, sizeof(iap_raw_on));
//     len = 0;
//     while (!(len == 6 && can_rec_id == can_id && can_rec_buf[3] == 0xE0 &&
//              can_rec_buf[4] == 0x10)) {
//       delay(40);
//       len = can_receive(can_rec_buf, &can_rec_id);
//       can_send(iap_raw_on, sizeof(iap_raw_on));
//     }
//   }
//   // 固件更新
//   if (M5.BtnC.wasReleased() || M5.BtnC.pressedFor(500)) {
//     can_id = 0x678;
//     can_send(iap_raw_off, sizeof(iap_raw_off));
//     len = 0;
//     while (!(len == 6 && can_rec_id == can_id && can_rec_buf[3] == 0xE1 && can_rec_buf[4] == 0x10)) {
//       delay(40);
//       len = can_receive(can_rec_buf, &can_rec_id);
//       can_send(iap_raw_off, sizeof(iap_raw_off));
//     }
//     delay(400);
//     can_id = 0x678;
//     can_send(iap_update_cmd, sizeof(iap_update_cmd));
//     len = can_receive(can_rec_buf, &can_rec_id);
//     while (!(len == 6 && can_rec_id == can_id && can_rec_buf[3] == 0xFF &&
//              can_rec_buf[4] == 0x10)) {
//       delay(40);
//       len = can_receive(can_rec_buf, &can_rec_id);
//       can_send(iap_update_cmd, sizeof(iap_update_cmd));
//     }
//     can_id = 0x302;
//     app_update();
//   }
// }

// // 更新APP
// void app_update(void) {
//   canvas.printf("\r\n          START UPDATE\r\n");
//   canvas.printf("\r\n       \r\n");
//   canvas.pushSprite(0, 0);
//   uint8_t len = 0;
//   can_send(start_cmd, sizeof(start_cmd));
//   delay(40);
//   len = can_receive(can_rec_buf, &can_rec_id);
//   while (
//       !(len == 1 && can_rec_id == can_id && can_rec_buf[0] == RECEIVE_ENABLE)) {
//     len = can_receive(can_rec_buf, &can_rec_id);
//     can_send(start_cmd, sizeof(start_cmd));
//     delay(40);
//   }
//   canvas.printf("  enable success\r\n");
//   canvas.pushSprite(0, 0);
//   for (int i = 0; i < page_num; i++) {
//     send_update_data_pack(app_start_addr + i * STM32G0xx_PAGE_SIZE,
//                           STM32G0xx_PAGE_SIZE, i);
//     delay(20);
//     len = can_receive(can_rec_buf, &can_rec_id);
//     while (!(len == 1 && can_rec_id == can_id &&
//              can_rec_buf[0] == RECEIVE_SUCCESS)) {
//       len = can_receive(can_rec_buf, &can_rec_id);
//       delay(20);
//     }
//     canvas.printf(" data packet%d : send successful\r\n", (i + 1));
//     canvas.pushSprite(0, 0);
//   }
//   canvas.printf("\r\n          UPDATE SUCCESS\r\n");
//   canvas.printf("\r\n       \r\n");
//   canvas.pushSprite(0, 0);
//   //   delay(3000);
// }
