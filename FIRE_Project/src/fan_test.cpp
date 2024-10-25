// #include "M5Unified.h"
// bool sate[128];
// #define FAN_CONTROL_REG_ADDR (0x00)
// #define PWM_FREQUENCY_REG_ADDR (0x10)
// #define PWM_DUTY_CYCLE_REG_ADDR (0x20)
// #define FAN_RPM_REG_ADDR (0x30)
// #define FAN_SIGNAL_FREQUENCY_REG_ADDR (0x40)
// #define FIRMWARE_VERAION_REG_ADDR (0xFE)
// #define I2C_ADDRESS_REG_ADDR (0xFF)

// uint8_t pwm_frequency = 2;

// uint8_t fan_enable = 1;
// uint8_t fan_disable = 0;
// uint8_t i2c_addr = 0x18;
// uint8_t i2c_addr_update = 0x19; 
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
// //   M5.Ex_I2C.writeRegister(i2c_addr, PWM_FREQUENCY_REG_ADDR, &pwm_frequency, 1, 100000U);
// //   delay(100);

//   uint8_t pwm_duty = 100;
//   M5.Ex_I2C.writeRegister(i2c_addr, PWM_DUTY_CYCLE_REG_ADDR, &pwm_duty, 1, 100000U);
//   delay(100);
//   M5.Ex_I2C.writeRegister(i2c_addr, FAN_CONTROL_REG_ADDR, &fan_enable, 1, 100000U);
//   delay(200);
//   M5.Ex_I2C.writeRegister(i2c_addr, I2C_ADDRESS_REG_ADDR, &i2c_addr_update, 1, 100000U);
//   delay(100);

// }
// uint32_t freq;
// uint16_t ans = 0;
// uint8_t data[10];

// void loop() {
//   i2c_addr = i2c_addr_update;
//   M5.Display.printf("\n");
//   M5.Display.printf("         >>>>>******start******<<<<<\n");
//   M5.Ex_I2C.readRegister(i2c_addr, FAN_CONTROL_REG_ADDR, data, 1, 100000U);
//   M5.Display.printf(">>> FAN_CONTROL_REG: 0x%02x\n", data[0]);
//   delay(500);

//   M5.Ex_I2C.readRegister(i2c_addr, PWM_FREQUENCY_REG_ADDR, data, 1, 100000U);
//   M5.Display.printf(">>> PWM_FREQUENCY_REG: 0x%02x\n", data[0]);
//   delay(500);

//   M5.Ex_I2C.readRegister(i2c_addr, PWM_DUTY_CYCLE_REG_ADDR, data, 1, 100000U);
//   M5.Display.printf(">>> PWM_DUTY_CYCLE_REG: 0x%02x\n", data[0]);
//   delay(500);

//   M5.Ex_I2C.readRegister(i2c_addr, FAN_RPM_REG_ADDR, data, 2, 100000U);
//   M5.Display.printf(">>> FAN_RPM_REG: %d\n", ((data[1] << 8) | data[0]));
//   delay(500);
 
//   M5.Ex_I2C.readRegister(i2c_addr, FAN_SIGNAL_FREQUENCY_REG_ADDR, data, 2, 100000U);
//   M5.Display.printf(">>> FAN_SIGNAL_FREQUENCY: %d\n", ((data[1] << 8) | data[0]));
//   delay(500);


//   M5.Ex_I2C.readRegister(i2c_addr, FIRMWARE_VERAION_REG_ADDR, data, 1, 100000U);
//   M5.Display.printf(">>> SW_VER_REG: 0x%02x\n", data[0]);
//   delay(500);

//   M5.Ex_I2C.readRegister(i2c_addr, I2C_ADDRESS_REG_ADDR, data, 1, 100000U);
//   M5.Display.printf(">>> I2C_ADDR_REG: 0x%02x\n", data[0]);
//   M5.Display.printf("         >>>>>******end******<<<<<\n");
//   delay(1000);
// }