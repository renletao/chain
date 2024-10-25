// #ifndef __BOOTLOADER_UPGRADE_H
// #define __BOOTLOADER_UPGRADE_H

// #include "Arduino.h"
// #include "Wire.h"

// #define  OPC_READ       (uint8_t)(0x03)
// #define  OPC_WREN       (uint8_t)(0x06)
// #define  OPC_ERPG       (uint8_t)(0x20)
// #define  OPC_ERUSM      (uint8_t)(0x44)
// #define  OPC_USRCD      (uint8_t)(0x77)

// #define BOOTLOADER_UPGRADE_ADDR                           0x54
// #define JUMP_TO_BOOTLOADER_REG                            0xFD
// #define FIRMWARE_VERSION_REG                              0xFE
// #define I2C_ADDRESS_REG                                   0xFF
// #define FW_SIZE 0x3000

// class BOOTLOADER_UPGRADE {
//    private:
//     uint8_t _addr;
//     TwoWire* _wire;
//     uint8_t _scl;
//     uint8_t _sda;
//     uint8_t _speed;
//     void writeBytes(uint8_t addr, uint8_t reg, uint8_t* buffer, uint8_t length);
//     void readBytes(uint8_t addr, uint8_t reg, uint8_t* buffer, uint8_t length);

//    public:
//     bool begin(TwoWire* wire = &Wire, uint8_t addr = BOOTLOADER_UPGRADE_ADDR,
//                uint8_t sda = 21, uint8_t scl = 22, uint32_t speed = 400000L);
//     void func_iap_upgrade(const unsigned char *app_buffer);
//     bool checkUpgrade(void);
//     uint8_t getFirmwareVersion(void);
//     uint8_t setI2CAddress(uint8_t addr);
//     uint8_t getI2CAddress(void);    
//     void jumpBootloader(void);
// };

// #endif
