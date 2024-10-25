// #include "M5Unified.h"
// #include <BOOTLOADER_UPGRADE.h>
// #include <UNIT_I2C.h>

// BOOTLOADER_UPGRADE btl_upgrade;
// uint8_t data1[6] = {0};

// void setup() {
//   auto cfg = M5.config();
//   M5.begin(cfg);
//   M5.Power.begin();
//   M5.Display.setTextFont(&fonts::efontCN_16);
//   M5.Display.fillScreen(BLACK);
//   M5.Display.setTextScroll(true);
//   M5.Display.printf("IAP TEST\n");
// }

// void loop() {
//   M5.update();
//   if (M5.BtnB.wasReleased() || M5.BtnB.pressedFor(500)) 
//   {
//     M5.Ex_I2C.begin();
//     data1[0] = 0x01;
//     M5.Ex_I2C.writeRegister(0x3A, 0xFD, data1, 1, 100000U);
//     pinMode(M5.Ex_I2C.getSCL(), OUTPUT);
//     pinMode(M5.Ex_I2C.getSDA(), OUTPUT);
//     digitalWrite(M5.Ex_I2C.getSCL(), LOW);
//     digitalWrite(M5.Ex_I2C.getSDA(), LOW);
//     delay(1000);
//     gpio_reset_pin((gpio_num_t)M5.Ex_I2C.getSCL());
//     gpio_reset_pin((gpio_num_t)M5.Ex_I2C.getSDA());
//     delay(100);
//     btl_upgrade.begin(&Wire, BOOTLOADER_UPGRADE_ADDR, M5.Ex_I2C.getSDA(),
//                       M5.Ex_I2C.getSCL(), 100000U);
//     btl_upgrade.func_iap_upgrade(appHexData);
//     M5.Display.printf("Software update completed\n");
//   }
// }