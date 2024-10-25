// #include "M5Unified.h"

// bool sate[128];

// #define LIN_MIC_PC_EN_ADDR (0x00)
// #define HP_MODE_ADDR (0x10)
// #define HP_DET_ADDR (0x20)
// #define RGB_LIGHT_ADDR (0x30)
// #define RGB1_STATUS_ADDR (0x40)
// #define RGB2_STATUS_ADDR (0x43)
// #define RGB3_STATUS_ADDR (0x46)
// #define SW_VER_REG_ADDR (0xFE)
// #define I2C_ADDR_REG_ADDR (0xFF)
// uint8_t color1_led[3] = {0x00, 0xFF, 0x00};
// uint8_t color2_led[3] = {0xFF, 0x00, 0x00};
// uint8_t color3_led[3] = {0x00, 0x00, 0xFF};
// uint8_t colors[9] = {0xFF, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0xFF};
// uint8_t light = 50;

// uint8_t mic_pc_enable = 1;
// uint8_t mic_pc_disable = 0;
// uint8_t i2c_addr = 0x33;
// uint8_t i2c_addr_update = 0x38;
// void setup() {
//   auto cfg = M5.config();
//   M5.begin(cfg);
//   M5.Ex_I2C.begin();
//   M5.Display.setTextFont(&fonts::efontCN_16);
//   M5.Display.fillScreen(BLACK);
//   M5.Display.setTextScroll(true);
//   M5.Display.printf("Module-FAN Test\n");
//   M5.Ex_I2C.scanID(sate);
//   for (int i = 0; i < 128; i++) {
//     if (sate[i]) {
//       M5.Display.printf("I2C Address: 0x%02x\n", i);
//     }
//   }
//   // M5.Ex_I2C.writeRegister(i2c_addr, RGB1_STATUS_ADDR, colors, 9, 100000U);
//   // delay(100);
//   M5.Ex_I2C.writeRegister(i2c_addr, RGB2_STATUS_ADDR, color2_led, 3,
//   100000U);
//   delay(100);
//   M5.Ex_I2C.writeRegister(i2c_addr, RGB3_STATUS_ADDR, color3_led, 3,
//   100000U);
//   delay(100);
//     M5.Ex_I2C.writeRegister(i2c_addr, RGB1_STATUS_ADDR, color1_led, 3,
//   100000U);
// //   delay(100);
// //   // M5.Ex_I2C.writeRegister(i2c_addr, LIN_MIC_PC_EN_ADDR, &mic_pc_enable, 1,
// //   //                         100000U);
// //   // delay(100);
//   M5.Ex_I2C.writeRegister(i2c_addr, I2C_ADDR_REG_ADDR, &i2c_addr_update, 1,
//                           100000U);
//   delay(100);
// }
// uint32_t freq;
// uint16_t ans = 0;
// uint8_t data[10];

// void loop() {
//   uint8_t i2c_addr = 0x38;
//   M5.Display.printf("\n");
//   M5.Display.printf("         >>>>>******start******<<<<<\n");
//   M5.Ex_I2C.readRegister(i2c_addr, LIN_MIC_PC_EN_ADDR, data, 1, 100000U);
//   M5.Display.printf(">>> LIN_MIC_PC_EN: 0x%02x\n", data[0]);
//   delay(500);

//   M5.Ex_I2C.readRegister(i2c_addr, HP_MODE_ADDR, data, 1, 100000U);
//   M5.Display.printf(">>> HP_MODE: 0x%02x\n", data[0]);
//   delay(500);

//   M5.Ex_I2C.readRegister(i2c_addr, HP_DET_ADDR, data, 1, 100000U);
//   M5.Display.printf(">>> HP_DET: 0x%02x\n", data[0]);
//   delay(500);

//   M5.Ex_I2C.readRegister(i2c_addr, RGB_LIGHT_ADDR, data, 1, 100000U);
//   M5.Display.printf(">>> RGB_LIGHT: 0x%02x\n", data[0]);
//   delay(500);

//   M5.Ex_I2C.readRegister(i2c_addr, RGB1_STATUS_ADDR, data, 9, 100000U);
//   M5.Display.printf(">>> RGB1_STATUS: R:0x%02x  G:0x%02x  B:0x%02x\n", data[0],
//                     data[1], data[2]);
//   delay(500);

//   // M5.Ex_I2C.readRegister(i2c_addr, RGB2_STATUS_ADDR, data, 3, 100000U);
//   M5.Display.printf(">>> RGB1_STATUS: R:0x%02x  G:0x%02x  B:0x%02x\n", data[3],
//                     data[4], data[5]);
//   delay(500);

//   // M5.Ex_I2C.readRegister(i2c_addr, RGB3_STATUS_ADDR, data, 3, 100000U);
//   M5.Display.printf(">>> RGB1_STATUS: R:0x%02x  G:0x%02x  B:0x%02x\n", data[6],
//                     data[7], data[8]);
//   delay(500);

//   M5.Ex_I2C.readRegister(i2c_addr, SW_VER_REG_ADDR, data, 1, 100000U);
//   M5.Display.printf(">>> SW_VER_REG: 0x%02x\n", data[0]);
//   delay(500);

//   M5.Ex_I2C.readRegister(i2c_addr, I2C_ADDR_REG_ADDR, data, 1, 100000U);
//   M5.Display.printf(">>> I2C_ADDR_REG: 0x%02x\n", data[0]);
//   M5.Display.printf("         >>>>>******end******<<<<<\n");
//   delay(1000);
//   M5.Ex_I2C.writeRegister(i2c_addr, RGB_LIGHT_ADDR, &light, 1, 100000U);
//   delay(20);
// }
