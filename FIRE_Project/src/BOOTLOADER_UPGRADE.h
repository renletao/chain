#ifndef __BOOTLOADER_UPGRADE_H
#define __BOOTLOADER_UPGRADE_H

#include "Arduino.h"
#include "Wire.h"

#define  OPC_READ       (uint8_t)(0x03)
#define  OPC_WREN       (uint8_t)(0x06)
#define  OPC_ERPG       (uint8_t)(0x20)
#define  OPC_ERUSM      (uint8_t)(0x44)
#define  OPC_USRCD      (uint8_t)(0x77)

#define BOOTLOADER_UPGRADE_ADDR                           0x54
#define JUMP_TO_BOOTLOADER_REG                            0xFD
#define FIRMWARE_VERSION_REG                              0xFE
#define I2C_ADDRESS_REG                                   0xFF
#define FW_SIZE  0x6800

class BOOTLOADER_UPGRADE {
   private:
    uint8_t _addr;
    TwoWire* _wire;
    uint8_t _scl;
    uint8_t _sda;
    uint8_t _speed;
    void writeBytes(uint8_t addr, uint8_t reg, uint8_t* buffer, uint8_t length);
    void readBytes(uint8_t addr, uint8_t reg, uint8_t* buffer, uint8_t length);

   public:
    static constexpr const uint8_t DMM_EXE_CMD_REG = 20;
    static constexpr const uint8_t DMM_EXE_CMD_REG2 = 21;
    static constexpr const uint8_t DMM_SCRATCH_PAD_REG1 = 24;   
    typedef enum {
    CMD_LOAD_CFG_FROM_FLASH = 0xDA,

    CMD_WRITE_CALIB_TO_CFG = 0xC3,

    CMD_DISABLE_SYNC_FUNC = 0xB0,

    CMD_ENABLE_SYNC_FUNC = 0xB1,

    CMD_ALARM_LIMIT_UPPER = 0xF0,

    CMD_ALARM_LIMIT_LOWER = 0xF1,

    // Require correct magic
    CMD_WRITE_CFG_TO_FLASH = 0xDB,
    CMD_JMP_BOOTLOADER = 0xDE
    } DMM_CMD;   
    uint32_t process_status = 0;    
    bool begin(TwoWire* wire = &Wire, uint8_t addr = BOOTLOADER_UPGRADE_ADDR,
               uint8_t sda = 21, uint8_t scl = 22, uint32_t speed = 400000L);
    uint32_t compute_cfg_crc32(const unsigned char *cfg, uint16_t len);
    void func_iap_update(void);
    void func_iap_upgrade(const unsigned char *app_buffer);
    bool checkUpgrade(void);
    uint8_t getFirmwareVersion(void);
    uint8_t setI2CAddress(uint8_t addr);
    uint8_t getI2CAddress(void);    
    void jumpBootloader(void);
};

#endif
