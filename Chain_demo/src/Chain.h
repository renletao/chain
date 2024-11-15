/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef CHAIN_H
#define CHAIN_H

#include <Arduino.h>

#define PACK_HEAD_HIGH (0xAA) // Packet header
#define PACK_HEAD_LOW (0x55)  // Packet header
#define PACK_END_HIGH (0x55)  // Packet tail
#define PACK_END_LOW (0xAA)   // Packet tail

#define CHECK_PASS (1)  // Check passed
#define CHECK_ERROR (0) // Check not passed

#define PACKET_BUFFER_SIZE (256) // Buffer size
#define BUFFER_SIZE (2048)       // Buffer size
#define MAX_PACKETS (20)         // 定义存储数据包的最大个数

#define TIMEOUT_MS (1) // 超时时间，单位为毫秒

typedef enum
{
  CHAIN_GET_BOOTLOADER_VERSION = 0xF9, // Query bootloader version number
  CHAIN_GET_VERSION_DEVICE = 0xFA,     // Query device software version number
  CHAIN_GET_DEVICE_TYPE = 0xFB,        // Query device type
  CHAIN_ENUM_PLEASE = 0xFC,            // Enumeration request
  CHAIN_HEARTBEAT = 0xFD,              // Heartbeat packet
  CHAIN_ENUM = 0xFE,                   // Enumeration
} chain_base_cmd_t;                    // Command base type

typedef enum
{
  CHAIN_SET_RGB_VALUE = 0x20, // Command to set RGB values
  CHAIN_GET_RGB_VALUE = 0x21, // Command to get RGB values
  CHAIN_SET_RGB_LIGHT = 0x22, // Command to set the brightness of the RGB light
  CHAIN_GET_RGB_LIGHT = 0x23, // Command to get the brightness of the RGB light
} chain_rgb_cmd_t;            // chain_rgb_cmd_t defines various command types

typedef enum
{
  CHAIN_ENCODER_GET_VALUE = 0x10,       // Get encoder value
  CHAIN_ENCODER_GET_INC_VALUE = 0x11,   // Get encoder increment value
  CHAIN_ENCODER_RESET_VALUE = 0x13,     // Reset encoder value
  CHAIN_ENCODER_RESET_INC_VALUE = 0x14, // Reset encoder increment value
  CHAIN_ENCODER_SET_AB_STATUS = 0x15,   // Set AB status, 0->AB, 1->BA
  CHAIN_ENCODER_GET_AB_STATUS = 0x16,   // Get AB status, 0->AB, 1->BA
  CHAIN_ENCODER_BUTTON_PRESS = 0x40,    // Get button state
} chain_encoder_cmd_t;                  // Command types

typedef enum {
  CHAIN_JOYSTICK_GET_16ADC = 0x30, // Command to get 16-bit ADC values
  CHAIN_JOYSTICK_GET_8ADC = 0x31,  // Command to get 8-bit ADC values
  CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_RANGE =
      0x32, // Command to get mapped range for X and Y axes
  CHAIN_JOYSTICK_SET_ADC_XY_MAPPED_RANGE =
      0x33, // Command to set mapped range for X and Y axes
  CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_INT16_VALUE =
      0x34, // Command to get 16-bit mapped values for X and Y
  CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_INT8_VALUE =
      0x35, // Command to get 8-bit mapped values for X and Y
  CHAIN_JOYSTICK_BUTTON_PRESS = 0x40, // Command for joystick button press event
} chain_joystick_cmd_t;               // Enumeration for joystick commands


typedef enum {
    CHAIN_ANGLE_GET_12ADC = 0x30,          // CHAIN_ANGLE_GET_12ADC is a command to get the latest 12-bit ADC value
    CHAIN_ANGLE_PGET_8ADC = 0x31,           // CHAIN_ANGLE_PGET_8ADC is a command to get the latest 8-bit mapped ADC value
    CHAIN_ANGLE_GET_CLOCKWISE_STATUS = 0x32,// CHAIN_ANGLE_GET_CLOCKWISE_STATUS is a command to get the current clockwise direction status
    CHAIN_ANGLE_SET_CLOCKWISE_STATUS = 0x33,// CHAIN_ANGLE_SET_CLOCKWISE_STATUS is a command to set the clockwise direction status
} chain_angle_cmd_t; // chain_angle_cmd_t is the command type for angle measurement operations


typedef enum {
	CHAIN_TOF_GET_DISTANCE = 0x50, // Command to get the distance measurement
	CHAIN_TOF_SET_MODE = 0x51,	   // Command to set the mode of the TOF sensor
	CHAIN_TOF_GET_MODE = 0x52,	// Command to get the current mode of the TOF sensor
} chain_tof_cmd_t;



typedef enum {
    CHAIN_I2C_INIT = 0x10,                   // Initialize I2C communication
    CHAIN_I2C_READ = 0x11,                   // Read data from I2C device
    CHAIN_I2C_WRITE = 0x12,                  // Write data to I2C device
    CHAIN_I2C_MEM_READ = 0x13,               // Read data from specific address on I2C device
    CHAIN_I2C_MEM_WRITE = 0x14,              // Write data to specific address on I2C device
    CHAIN_I2C_GET_DEVICE_ADDR = 0x15,       // Retrieve address of connected I2C device
    CHAIN_GPIO_OUTPUT_INIT = 0x30,           // Initialize GPIO for output
    CHAIN_GPIO_INPUT_INIT = 0x40,            // Initialize GPIO for input
    CHAIN_GPIO_READ_GPIO_LEVEL = 0x41,       // Read the level of input GPIO pin
    CHAIN_GPIO_EXTERNAL_NVIC_INIT = 0x50,    // Initialize external interrupt for GPIO
    CHAIN_GPIO_EXTERNAL_NVIC_RETURN = 0x51,  // Return from external interrupt for GPIO
    CHAIN_GPIO_ADC_INIT = 0x60,              // Initialize ADC for GPIO
    CHAIN_GPIO_ADC_READ = 0x61,              // Read data from GPIO ADC
    CHAIN_GET_WORK_STATION = 0x70,           // Query the working status of the system
} uart_cmd_t; // Command type for UART communication


typedef enum
{
  RGB_OPERATION_FAIL = 0x00, // Operation failed, indicating that the RGB
                             // operation was not successful
  RGB_OPERATION_SUCCESS      // Operation successful, indicating that the RGB
                             // operation was successful
} rgb_operation_t;           // rgb_operation_t is used to indicate the status of the
                             // operation

/* Structure to define the size of RGB color components for a single LED */
typedef struct
{
  uint8_t R;         // Red component, range 0-255
  uint8_t G;         // Green component, range 0-255
  uint8_t B;         // Blue component, range 0-255
} RGB_Color_TypeDef; // RGB_Color_TypeDef is used to store the RGB color values
                     // of the LED

typedef struct
{
  uint8_t data[PACKET_BUFFER_SIZE]; // 数据包内容
  size_t size;                      // 数据包大小
  uint8_t type;
} Packet;

typedef enum
{
  CHAIN_ENCODER = 0x0001,
  CHAIN_ANGLE = 0x0002,
  CHAIN_KEY = 0x0003,
  CHAIN_JOYSTICK = 0x0004,
  CHAIN_TOF = 0x0005,
  CHAIN_UART = 0x0006,
} device_t;

typedef struct
{
  uint8_t id = 0; // 设备ID
  uint8_t blVersion = 0;
  uint8_t fwVersion = 0;
  uint16_t device_type = 0; // 设备类型
} device;


typedef struct
{
  uint8_t count;   // 设备个数
  device *devices; // 设备数组，使用柔性数组成员
} deviceList;

const uint8_t enum_buf[10] = {0xAA, 0x55, 0x04, 0x00, 0xFF,
                              0xFE, 0x00, 0xFD, 0x55, 0xAA};
const uint8_t heartbeat_buf[9] = {0xAA, 0x55, 0x04, 0x00, 0xFF,
                                  0xFD, 0xFC, 0x55, 0xAA};

typedef struct {
    int deviceId;          
    bool isPressed;    
} Key;

class CHAIN
{
private:
  HardwareSerial *_serial = nullptr;
  uint8_t _buffer[BUFFER_SIZE]; // 接收缓冲区
  size_t _bufferSize;           // 当前缓冲区大小
  Packet _packets[MAX_PACKETS]; // 存储解析后的数据包
  size_t _packetCount;          // 当前数据包数量
  Packet _validPacket;          // 存储符合条件的数据包
  deviceList *_myDeviceList;    // 动态分配设备数组
  size_t _deviceCount;          // 当前设备数量
  size_t _enumPlease;
  Key _key;
  void addPackHead(void);
  void addPackEnd(void);
  uint8_t headEndCheck(uint8_t *buffer, uint16_t size);
  uint8_t crcCheck(uint8_t *buffer, uint16_t size);
  uint8_t packCheck(uint8_t *buffer, uint16_t size);
  uint8_t crcConstruct(uint8_t *buffer, uint16_t size);
  bool waitForData(uint8_t cmd, uint8_t id, uint32_t timeout);
  void clearPacket(void);

public:
  void begin(HardwareSerial *serial, unsigned long baud, int8_t rxPin = -1,
             int8_t txPin = -1);
  void send(const char *buffer, size_t size);
  void chainSendPacket(uint8_t cmd, uint8_t *buffer, uint16_t size, uint8_t index);
  bool available(void);
  void receive(void);
  void parsePackets(void);
  bool parsePackets(uint8_t cmd, uint8_t id);
  Key getKeyStatus(void);
  const Packet *getPackets(size_t &count) const; // 获取数据包
  void clearPackets(void);                       // 清空数据包
  bool isDeviceConnected(void);
  size_t getEnumPleaseNums(void);
  void clearEnumPleaseNums(void);
  uint8_t getBootloaderVersion(uint8_t id, unsigned long timeout = 100);
  uint8_t getFirmwareVersion(uint8_t id, unsigned long timeout = 100);
  uint16_t getDeviceType(uint8_t id, unsigned long timeout = 100);
  uint8_t getDeviceNum(unsigned long timeout = 100);
  bool getDeviceList(deviceList *list, unsigned long timeout = 3000);
  bool getDeviceDetails(uint8_t id, unsigned long timeout, device *dev);

  // RGB相关控制
  bool setRGBValue(uint8_t id, RGB_Color_TypeDef rgb, unsigned long timeout = 100);
  RGB_Color_TypeDef getRGBValue(uint8_t id, unsigned long timeout = 100);
  bool setRGBLight(uint8_t id, uint8_t light, unsigned long timeout = 100);
  uint8_t getRGBLight(uint8_t id, unsigned long timeout = 100);

  // encoder
  bool getEncoderValue(uint8_t id, int16_t *value, unsigned long timeout = 100);
  bool getEncoderIncValue(uint8_t id, int16_t *incValue, unsigned long timeout = 100);
  bool resetEncoderValue(uint8_t id, unsigned long timeout = 100);
  bool resetEncoderIncValue(uint8_t id, unsigned long timeout = 100);
  bool setEncoderABStatus(uint8_t id, uint8_t status, unsigned long timeout = 100);
  bool getEncoderABStatus(uint8_t id, uint8_t *status,unsigned long timeout = 100);

  // angle
  bool getAngle12Adc(uint8_t id,uint16_t *adcValue,unsigned long timeout = 100);
  bool getAngle8Adc(uint8_t id,uint8_t *adcValue,unsigned long timeout = 100);
  bool setAngleClockWise(uint8_t id,uint8_t direction,unsigned long timeout = 100);
  bool getAngleClockWise(uint8_t id,uint8_t *direction,unsigned long timeout = 100);

  // joystick
  bool getJoystick16Adc(uint8_t id, uint16_t *adcXValue,uint16_t *adcYValue,unsigned long timeout = 100);
  bool getJoystick8Adc(uint8_t id,uint8_t *adcValue,uint8_t *adcYValue,unsigned long timeout = 100);
  bool setJoystickMappedRange(uint8_t id,int16_t *buf,uint8_t size,unsigned long timeout = 100);
  bool getJoystickMappedRange(uint8_t id,int16_t *buf,uint8_t size,unsigned long timeout = 100);
  bool getJoystickMappedInt16Value(uint8_t id,int16_t *x,int16_t *y,unsigned long timeout = 100);
  bool getJoystickMappedInt8Value(uint8_t id,int8_t *x,int8_t *y,unsigned long timeout = 100);


  // Tof
  bool getTofDistance(uint8_t id,uint16_t *distance,unsigned long timeout = 100);
  bool setTofMode(uint8_t id ,uint8_t mode, unsigned long timeout = 100);
  bool getTofMode(uint8_t id,uint8_t *mode,unsigned long timeout = 100);


  // uart
  bool chianUartI2cInit(uint8_t id ,uint8_t *status,unsigned long timeout = 100);
  bool chainUartI2cRead(uint8_t id,uint8_t i2cAddr,uint8_t readLength,uint8_t *buffer,uint8_t *status,unsigned long timeout = 100);
  bool chainUartI2cWrite(uint8_t id,uint8_t i2cAddr,uint8_t write,uint8_t *buffer,uint8_t *status,unsigned long timeout = 100);
  bool chainUartI2cMemRead(uint8_t id,uint8_t i2cAddr,uint16_t regAddr,uint8_t regLength,uint8_t readLength,uint8_t *buffer,uint8_t *status,unsigned long timeout = 100);
  bool chainUartI2cMemWrite(uint8_t id,uint8_t i2cAddr,uint16_t regAddr,uint8_t regLength,uint8_t writeLength,uint8_t *buffer,uint8_t *status,unsigned long timeout = 100);
  bool getChainUartI2cDeviceAddr(uint8_t id,uint8_t *i2cAddrNums,uint8_t *buffer,uint8_t *status,unsigned long timeout = 100);
  bool chainUartOutputInit(uint8_t id,uint8_t gpio,uint8_t gpioLevel,uint8_t gpioMode,uint8_t gpioUp,uint8_t gpioSpeed,uint8_t *status,unsigned long timeout = 100);
  bool chainUartInputInit(uint8_t id, uint8_t gpio,uint8_t gpioUp,uint8_t *status,unsigned long timeout = 100);
  bool chainUartReadLevel(uint8_t id,uint8_t gpio,uint8_t *status,unsigned long timeout = 100);
  bool chainUartNvicInit(uint8_t id,uint8_t gpio,uint8_t gpioUp,uint8_t triggerMode,uint8_t *status,unsigned long timeout = 100);
  bool chainUartAdcInit(uint8_t id,uint8_t adcChannel1,uint8_t adcChannel2,uint8_t *status,unsigned long timeout = 100);
  bool getAdcValue(uint8_t id,uint8_t *adcChannelNums,uint8_t *buffer,uint8_t *status,unsigned long timeout = 100);
};

#endif // CHAIN_H
