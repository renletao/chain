// #include <M5GFX.h>
// #include <M5Unified.h>

// M5GFX display;
// M5Canvas canvas(&display);

// #define SCROLL_ADDR 0x40
// #define ENCODER_REG 0x10
// #define BUTTON_REG 0x20
// #define RGB_LED_REG 0x30
// #define RESET_REG 0x40
// #define INC_ENCODER_REG 0x50
// #define BOOTLOADER_VERSION_REG 0xFC
// #define JUMP_TO_BOOTLOADER_REG 0xFD
// #define FIRMWARE_VERSION_REG 0xFE
// #define I2C_ADDRESS_REG 0xFF

// void setup() {
//   M5.begin();
//   display.begin();
//   canvas.setColorDepth(1); // mono color
//   canvas.createSprite(display.width(), display.height());
//   canvas.setTextSize(2);
//   canvas.setPaletteColor(1, GREEN);
//   canvas.setTextScroll(true);
//   canvas.println("       HELLO WORLD");
//   canvas.pushSprite(0, 0);
//   M5.Ex_I2C.begin();
// }
// void loop() {
//   int32_t value = 0;
//   uint8_t data = 0;
//   M5.Ex_I2C.readRegister(SCROLL_ADDR, ENCODER_REG, (uint8_t *)&value, 4,
//                          100000U);
//   canvas.printf("SCROLL_VALUE: %d\r\n\r\n", value);
//   canvas.pushSprite(0, 0);

//   M5.Ex_I2C.readRegister(SCROLL_ADDR, BUTTON_REG, &data, 1, 100000U);
//   canvas.printf("buffon_statue: %d\r\n\r\n", data);
//   canvas.pushSprite(0, 0);
//   delay(2000);
// }