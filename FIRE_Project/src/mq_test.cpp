// #include "M5Unified.h"

// #define I2C1_ADDR_BASE 0x11
// #define MQ_CFG_REG_ADDR 0x00
// #define LED_CFG_REG_ADDR 0x01
// #define MQ_HEAT_PIN_CFG_REG_ADDR 0x10
// #define MQ_8B_ADC_REG_ADDR 0x20
// #define MQ_12B_ADC_REG_ADDR 0x30
// #define MQ_ADC_VALID_TAGS_REG_ADDR 0x40
// #define INT_TEMP_8B_ADC_REG_ADDR 0x50
// #define INT_TEMP_12B_ADC_REG_ADDR 0x60
// #define INT_TEMP_VAL_REG_ADDR 0x70
// #define SW_VER_REG_ADDR 0xFE
// #define I2C_ADDR_REG_ADDR 0xFF

// bool sate[128];
// uint8_t data[6] = {0};
// uint8_t data1[6] = {0};
// uint8_t i2c_adr = 0x11;
// uint8_t i2c_adr_cfg = 0x3A;
// void setup() {
//   auto cfg = M5.config();
//   M5.begin(cfg);
//   Serial.begin(115200);
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
//   // data1[0] = 0x11;
//   // M5.Ex_I2C.writeRegister(i2c_adr, I2C_ADDR_REG_ADDR, data1, 1, 400000U);
// //   data1[0] = 0x80;
// //   data1[1] = 0x10;
// //   M5.Ex_I2C.writeRegister(i2c_adr, MQ_HEAT_PIN_CFG_REG_ADDR, data1, 2,
// //   100000U);
// //     data1[0] = 0x02;
// //     data1[1] = 0x01;
// //     M5.Ex_I2C.writeRegister(i2c_adr, MQ_CFG_REG_ADDR, data1, 2, 100000U);

// //     M5.Ex_I2C.writeRegister(i2c_adr, I2C_ADDR_REG_ADDR, &i2c_adr_cfg, 1, 100000U);
// //     i2c_adr = i2c_adr_cfg;
// //     delay(3000);
// }
// void loop() {
// //   data1[0] = 0x01;
// //    M5.Ex_I2C.writeRegister(i2c_adr_cfg, 0xFD, data1, 1, 100000U);
// //   Serial.println("***********");
// //   M5.Display.println("***********");

// //   M5.Ex_I2C.readRegister(i2c_adr, MQ_CFG_REG_ADDR, data, 1, 400000U);
// //   Serial.printf("MQ_CFG_REG_ADDR: 0x%02x \n", data[0]);
// //   M5.Display.printf("MQ_CFG_REG_ADDR: 0x%02x \n", data[0]);

// //   M5.Ex_I2C.readRegister(i2c_adr, LED_CFG_REG_ADDR, data, 1, 400000U);
// //   Serial.printf("LED_CFG_REG_ADDR: 0x%02x \n", data[0]);
// //   M5.Display.printf("LED_CFG_REG_ADDR: 0x%02x \n", data[0]);

// //   M5.Ex_I2C.readRegister(i2c_adr, MQ_HEAT_PIN_CFG_REG_ADDR, data, 2, 400000U);
// //   Serial.printf("MQ_HEAT_PIN_CFG_REG_ADDR: 0x%02x  0x%02x\n", data[0], data[1]);
// //   M5.Display.printf("MQ_HEAT_PIN_CFG_REG_ADDR: 0x%02x  0x%02x\n", data[0], data[1]);

// //   M5.Ex_I2C.readRegister(i2c_adr, MQ_8B_ADC_REG_ADDR, data, 1, 400000U);
// //   Serial.printf("mq 8bit ADC: 0x%02x \n", data[0]);
// //   M5.Display.printf("mq 8bit ADC: 0x%02x \n", data[0]);

// //   M5.Ex_I2C.readRegister(i2c_adr, MQ_12B_ADC_REG_ADDR, data, 2, 400000U);
// //   Serial.printf("mq 12bit ADC: 0x%02x  0x%02x\n", data[0], data[1]);
// //   M5.Display.printf("mq 12bit ADC: 0x%02x  0x%02x\n", data[0], data[1]);

// //   M5.Ex_I2C.readRegister(i2c_adr, MQ_ADC_VALID_TAGS_REG_ADDR, data, 1, 400000U);
// //   Serial.printf("mq adc valid tags: 0x%02x \n", data[0]);
// //   M5.Display.printf("mq adc valid tags: 0x%02x \n", data[0]);

// //   M5.Ex_I2C.readRegister(i2c_adr, INT_TEMP_8B_ADC_REG_ADDR, data, 1, 400000U);
// //   Serial.printf("NTC 8bit ADC: 0x%02x\n", data[0]);
// //   M5.Display.printf("NTC 8bit ADC: 0x%02x\n", data[0]);

// //   M5.Ex_I2C.readRegister(i2c_adr, INT_TEMP_12B_ADC_REG_ADDR, data, 2, 400000U);
// //   Serial.printf("NTC 12bit ADC: 0x%02x  0x%02x\n", data[0], data[1]);
// //   M5.Display.printf("NTC 12bit ADC: 0x%02x  0x%02x\n", data[0], data[1]);

// //   M5.Ex_I2C.readRegister(i2c_adr, INT_TEMP_VAL_REG_ADDR, data, 2, 400000U);
// //   Serial.printf("Internal temperature value: 0x%02x  0x%02x \n", data[0],
// //                 data[1]);
// //   Serial.printf("Internal temperature value: %d.%d \n", data[0], data[1]);
// //   M5.Display.printf("Internal temperature value: 0x%02x  0x%02x \n", data[0],
// //                 data[1]);
// //   M5.Display.printf("Internal temperature value: %d.%d \n", data[0], data[1]);

// //   M5.Ex_I2C.readRegister(i2c_adr, SW_VER_REG_ADDR, data, 1, 400000U);
// //   Serial.printf("software version: 0x%02x \n", data[0]);
// //   M5.Display.printf("software version: 0x%02x \n", data[0]);

// //   M5.Ex_I2C.readRegister(i2c_adr, I2C_ADDR_REG_ADDR, data, 1, 400000U);
// //   Serial.printf("i2c addr: 0x%02x \n", data[0]);
// //   Serial.println("***********");
// //   M5.Display.printf("i2c addr: 0x%02x \n", data[0]);
// //   M5.Display.println("***********");
//   delay(3000);
// }

