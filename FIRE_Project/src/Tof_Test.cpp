// /**
//  * @file Continuous.ino
//  * @author SeanKwok (shaoxiang@m5stack.com)
//  * @brief M5Unit ToF4M Continuous Read Data Demo.
//  * @version 0.1
//  * @date 2023-11-23
//  *
//  *
//  * @Hardwares: M5Unit ToF4M
//  * @Platform Version: Arduino M5Stack Board Manager v2.0.7
//  * @Dependent Library:
//  * VL53L1X: https://github.com/pololu/vl53l1x-arduino
//  * M5Unified: https://github.com/m5stack/M5Unified
//  */

// #include "M5Unified.h"
// #include <Wire.h>
// #include <VL53L1X.h>

// VL53L1X sensor;

// void setup() {
//     M5.begin();
//     M5.Ex_I2C.begin(21, 22, 400000);
//     sensor.setBus(&Wire);

//     sensor.init();
// }

// void loop() {
//     uint16_t data = sensor.read();
    
//     if(data >=0){
//         Serial.printf("distance: %d\r\n",data);
//     }
//     delay(100);
// }