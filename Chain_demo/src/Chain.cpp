#include "Chain.h"

static uint8_t sSendBuf[BUFFER_SIZE] = {0}; // Data packet buffer
static uint8_t sSendLen = 0;                // Length of the data to be sent
static uint8_t cmdBuf[20] = {0};
static uint8_t cmdSize = 0;
void CHAIN::addPackHead(void) {
  sSendBuf[sSendLen++] = PACK_HEAD_HIGH; // Add high byte of header
  sSendBuf[sSendLen++] = PACK_HEAD_LOW;  // Add low byte of header
}
void CHAIN::addPackEnd(void) {
  sSendBuf[sSendLen++] = PACK_END_HIGH; // Add high byte of footer
  sSendBuf[sSendLen++] = PACK_END_LOW;  // Add low byte of footer
}
uint8_t CHAIN::headEndCheck(uint8_t *buffer, uint16_t size) {
  // Check if the first two bytes are the header and the last two bytes are the
  // footer
  if (buffer[0] == PACK_HEAD_HIGH && buffer[1] == PACK_HEAD_LOW &&
      buffer[size - 1] == PACK_END_LOW && buffer[size - 2] == PACK_END_HIGH) {
    return CHECK_PASS; // Return success if checks pass
  }
  return CHECK_ERROR; // Return error if checks fail
}
uint8_t CHAIN::crcCheck(uint8_t *buffer, uint16_t size) {
  uint8_t crc = buffer[size - 3]; // Extract the CRC from the packet
  uint8_t temp = 0;

  // Calculate CRC for the data (excluding header, footer, and CRC)
  for (uint16_t i = 0; i < (((buffer[3] << 8) | buffer[2]) - 1); i++) {
    temp += buffer[i + 4]; // Start after header, offset by 4
  }

  // Compare calculated CRC with the provided CRC
  if (crc == temp) {
    return CHECK_PASS; // CRC matches
  }
  return CHECK_ERROR; // CRC does not match
}
uint8_t CHAIN::packCheck(uint8_t *buffer, uint16_t size) {
  // Ensure the buffer is large enough to contain a valid packet
  if (size <= 4) {
    return CHECK_ERROR; // Packet is too small
  }

  // Check the packet header/footer and CRC
  if (headEndCheck(buffer, size) == CHECK_PASS &&
      crcCheck(buffer, size) == CHECK_PASS) {
    return CHECK_PASS; // Packet is valid
  }
  return CHECK_ERROR; // Packet is invalid
}
uint8_t CHAIN::crcConstruct(uint8_t *buffer, uint16_t size) {
  uint8_t temp = 0;

  // Calculate the CRC by summing all bytes in the buffer
  for (uint16_t i = 0; i < size; i++) {
    temp += buffer[i];
  }

  return temp; // Return the calculated CRC
}
void CHAIN::clearPacket(void) {
  _validPacket.size = 0; // 将大小设置为 0
  memset(_validPacket.data, 0, sizeof(_validPacket.data)); // 清空数据
}
bool CHAIN::waitForData(uint8_t cmd, uint8_t id, uint32_t timeout) {
  clearPacket();
  uint32_t startTime = millis(); // 获取当前时间
  while (millis() - startTime < timeout) {
    // 检查是否收到数据
    if (available()) {
      receive();
      bool status = parsePackets(cmd, id);
      if (status) {
        return true;
      }
    }
    delay(3);
  }
  return false; // 超时
}
void CHAIN::begin(HardwareSerial *serial, unsigned long baud, int8_t rxPin,
                  int8_t txPin) {
  _serial = serial;
  // 使用指定的引脚初始化串口
  _serial->begin(baud, SERIAL_8N1, rxPin, txPin);
}
void CHAIN::send(const char *buffer, size_t size) {
  if (_serial) {
    _serial->write(buffer, size);
  }
}
void CHAIN::chainSendPacket(uint8_t cmd, uint8_t *buffer, uint16_t size,
                            uint8_t index) {
  // Clear sending length
  sSendLen = 0;
  // Calculate sending length
  uint16_t len = size + 3;
  // Add packet header
  addPackHead();
  sSendBuf[sSendLen++] = (uint8_t)(len & 0xFF);
  sSendBuf[sSendLen++] = (uint8_t)((len >> 8) & 0xFF);
  sSendBuf[sSendLen++] = index;
  sSendBuf[sSendLen++] = cmd;
  for (uint8_t i = 0; i < size; i++) {
    sSendBuf[sSendLen++] = buffer[i]; // Add data from buffer
  }
  sSendBuf[sSendLen] = crcConstruct((uint8_t *)(sSendBuf + 4),
                                    (sSendLen - 4)); // Calculate CRC
  sSendLen++;
  addPackEnd();
  send((const char *)sSendBuf, sSendLen); // Send the complete packet
}
bool CHAIN::available(void) { return _serial->available() > 0; }

void CHAIN::receive(void) {
  unsigned long startTime = millis();

  while (true) // 无限循环
  {
    // 检查超时
    if (millis() - startTime >= TIMEOUT_MS) {
      break; // 超时后退出循环
    }

    // 检查是否有可用数据
    while (_serial->available()) {
      _buffer[_bufferSize++] = _serial->read();
      startTime = millis(); // 重置时间
    }
  }
  // Serial.printf("rve >>> ");
  // for(uint8_t i = 0;i<_bufferSize;i++){
  //   Serial.printf("0x%02X ", _buffer[i]);
  // }
  // Serial.printf("\r\n");
}

void CHAIN::parsePackets(void) {
  size_t startIndex = 0;

  while (startIndex < _bufferSize) {
    // 查找包头
    if (_buffer[startIndex] == PACK_HEAD_HIGH &&
        _buffer[startIndex + 1] == PACK_HEAD_LOW) {
      // 查找包尾
      size_t endIndex = startIndex + 2;
      while (endIndex + 1 < _bufferSize &&
             !(_buffer[endIndex] == PACK_END_HIGH &&
               _buffer[endIndex + 1] == PACK_END_LOW)) {
        endIndex++;
      }
      if (endIndex + 1 < _bufferSize) {
        // 找到完整数据包
        size_t packetSize = endIndex - startIndex + 2; // 包含头和尾
        if (_packetCount < sizeof(_packets) / sizeof(_packets[0])) {
          if ((_buffer + startIndex)[5] == 0xFC &&
              packCheck((_buffer + startIndex), packetSize)) {
            _enumPlease++;

          } else if ((_buffer + startIndex)[5] == 0x40 &&
                     (_buffer + startIndex)[6] == 0x11) {
            if (packCheck((_buffer + startIndex), packetSize)) {
              _key.isPressed = true;
              _key.deviceId = (_buffer + startIndex)[4];
            }
          } else {
            memcpy(_packets[_packetCount].data, _buffer + startIndex,
                   packetSize);
            _packets[_packetCount].size = packetSize;
            _packetCount++;
          }
        }
        startIndex = endIndex + 2; // 跳过当前包的尾部
      } else {
        // 没有找到尾部，退出循环，等待后续数据
        break;
      }
    } else {
      startIndex++; // 移动到下一个字节
    }
  }

  // 清空已解析的数据，保留未处理的数据
  if (startIndex > 0) {
    memmove(_buffer, _buffer + startIndex, _bufferSize - startIndex);
    _bufferSize -= startIndex;
  }
}
bool CHAIN::parsePackets(uint8_t cmd, uint8_t id) {
  size_t startIndex = 0;
  bool packetFound = false; // 标记是否找到符合条件的数据包

  while (startIndex < _bufferSize) {
    // 查找包头
    if (_buffer[startIndex] == PACK_HEAD_HIGH && _buffer[startIndex + 1] == PACK_HEAD_LOW) {
      // 查找包尾
      size_t endIndex = startIndex + 2;
      while (endIndex + 1 < _bufferSize &&
             !(_buffer[endIndex] == PACK_END_HIGH && _buffer[endIndex + 1] == PACK_END_LOW)) {
        endIndex++;
      }

      // 检查是否找到包尾
      if (endIndex + 1 < _bufferSize) {
        size_t packetSize = endIndex - startIndex + 2; // 包含头和尾
        uint8_t packetId = _buffer[startIndex + 4];    // 第四个数据
        uint8_t packetCmd = _buffer[startIndex + 5];   // 第五个数据

        // 如果 cmd 和 id 匹配，处理数据包
        if (packetCmd == cmd && packetId == id) {
          memcpy(_validPacket.data, _buffer + startIndex, packetSize);
          _validPacket.size = packetSize;
          packetFound = true; // 标记找到符合条件的数据包
        }

        // 如果是 CHAIN_ENUM_PLEASE 类型的数据包
        else if (packetCmd == CHAIN_ENUM_PLEASE && packCheck(_buffer + startIndex, packetSize)) {
          _enumPlease++; // 计数器增加
        }
        
        // 如果是特定类型的数据包 (0x40 0x11)，且符合 packCheck
        else if ((_buffer[startIndex + 5] == 0x40) && (_buffer[startIndex + 6] == 0x11) &&
                 packCheck(_buffer + startIndex, packetSize)) {
          _key.isPressed = true;
          _key.deviceId = _buffer[startIndex + 4];
        }

        // 清理已解析的数据，保留未处理的数据
        size_t dataAfterPacket = _bufferSize - (endIndex + 2);
        if (dataAfterPacket > 0) {
          memmove(_buffer, _buffer + endIndex + 2, dataAfterPacket);
        }
        _bufferSize = dataAfterPacket; // 更新缓冲区大小

        // 重置 startIndex，重新开始检查新的数据包
        startIndex = 0;
      } else {
        // 没有找到包尾，退出循环，等待后续数据
        break;
      }
    } else {
      // 不是包头，移动到下一个字节
      startIndex++;
    }
  }

  // 如果在整个缓冲区处理完成后，找到了符合条件的数据包，返回 true
  return packetFound;
}

Key CHAIN::getKeyStatus(void) {
  Key keyStatus = _key;
  _key.deviceId = 0;
  _key.isPressed = false;
  return keyStatus;
}

void CHAIN::clearPackets(void) {
  _packetCount = 0; // 清空数据包
}

bool CHAIN::isDeviceConnected(void) {
  cmdSize = 0;
  chainSendPacket(CHAIN_HEARTBEAT, cmdBuf, cmdSize, 0xFF);
  bool status = waitForData(CHAIN_HEARTBEAT, 0xFF, 10);
  if (status) {
    if (packCheck(_validPacket.data, _validPacket.size)) {
      return true;
    }
  }
  return false;
}

size_t CHAIN::getEnumPleaseNums(void) { return _enumPlease; }

void CHAIN::clearEnumPleaseNums(void) { _enumPlease = 0; }
uint8_t CHAIN::getBootloaderVersion(uint8_t id, unsigned long timeout) {
  cmdSize = 0;
  chainSendPacket(CHAIN_GET_BOOTLOADER_VERSION, cmdBuf, cmdSize, id);
  bool status = waitForData(CHAIN_GET_BOOTLOADER_VERSION, id, timeout);
  if (status) {
    if (packCheck(_validPacket.data, _validPacket.size)) {
      return _validPacket.data[6];
    }
  }
  return 0;
}
uint8_t CHAIN::getFirmwareVersion(uint8_t id, unsigned long timeout) {
  cmdSize = 0;
  chainSendPacket(CHAIN_GET_VERSION_DEVICE, cmdBuf, cmdSize, id);
  bool status = waitForData(CHAIN_GET_VERSION_DEVICE, id, timeout);
  if (status) {
    if (packCheck(_validPacket.data, _validPacket.size)) {
      return _validPacket.data[6];
    }
  }
  return 0;
}
uint16_t CHAIN::getDeviceType(uint8_t id, unsigned long timeout) {
  cmdSize = 0;
  chainSendPacket(CHAIN_GET_DEVICE_TYPE, cmdBuf, cmdSize, id);
  bool status = waitForData(CHAIN_GET_DEVICE_TYPE, id, timeout);
  if (status) {
    if (packCheck(_validPacket.data, _validPacket.size)) {
      return (_validPacket.data[7] << 8) | _validPacket.data[6];
    }
  }
  return 0;
}
uint8_t CHAIN::getDeviceNum(unsigned long timeout) {
  cmdSize = 0;
  cmdBuf[cmdSize++] = 0;
  chainSendPacket(CHAIN_ENUM, cmdBuf, cmdSize, 0xFF);
  bool status = waitForData(CHAIN_ENUM, 0xFF, timeout);
  if (status) {
    if (packCheck(_validPacket.data, _validPacket.size)) {
      return _validPacket.data[6];
    }
  }
  return 0;
}

bool CHAIN::getDeviceList(deviceList *list, unsigned long timeout) {
  unsigned long startTime = millis();
  for (uint8_t i = 0; i < list->count; i++) { // 注意 ID 从 0 开始
    list->devices[i].id = i + 1;              // 设备编号从 1 开始

    // 使用辅助函数获取版本和设备类型
    if (!getDeviceDetails(i + 1, timeout, &list->devices[i])) { // ID 从 1 开始
      return false; // 失败则返回 false
    }
  }

  return true; // 成功获取设备列表
}

bool CHAIN::getDeviceDetails(uint8_t id, unsigned long timeout, device *dev) {
  unsigned long startTime;

  // // 获取引导加载程序版本
  // startTime = millis();
  // dev->blVersion = 0; // 初始化
  // while ((dev->blVersion = getBootloaderVersion(id)) == 0 &&
  //        (millis() - startTime < timeout)) {
  //   delay(5);
  // }
  // if (dev->blVersion == 0)
  //   return false;

  // // 获取固件版本
  // startTime = millis();
  // dev->fwVersion = 0; // 初始化
  // while ((dev->fwVersion = getFirmwareVersion(id)) == 0 &&
  //        (millis() - startTime < timeout)) {
  //   delay(5);
  // }
  // if (dev->fwVersion == 0)
  //   return false;

  // 获取设备类型
  startTime = millis();
  dev->device_type = 0; // 初始化
  while ((dev->device_type = getDeviceType(id)) == 0 &&
         (millis() - startTime < timeout)) {
    delay(1);
  }
  if (dev->device_type == 0)
    return false;
  return true; // 返回成功状态
}

bool CHAIN::setRGBValue(uint8_t id, RGB_Color_TypeDef rgb,
                        unsigned long timeout) {
  cmdSize = 0;
  cmdBuf[cmdSize++] = rgb.R;
  cmdBuf[cmdSize++] = rgb.G;
  cmdBuf[cmdSize++] = rgb.B;
  chainSendPacket(CHAIN_SET_RGB_VALUE, cmdBuf, cmdSize, id);
  bool status = waitForData(CHAIN_SET_RGB_VALUE, id, timeout);
  if (status) {
    if (packCheck(_validPacket.data, _validPacket.size)) {
      if (_validPacket.data[6] == 1) {
        return true;
      }
    }
  }
  return false;
}

RGB_Color_TypeDef CHAIN::getRGBValue(uint8_t id, unsigned long timeout) {
  RGB_Color_TypeDef rgb = {0, 0, 0};
  cmdSize = 0;
  bool status = waitForData(CHAIN_GET_RGB_VALUE, id, timeout);
  if (status) {
    if (packCheck(_validPacket.data, _validPacket.size)) {
      rgb.R = _validPacket.data[6];
      rgb.G = _validPacket.data[7];
      rgb.B = _validPacket.data[8];
      return rgb;
    }
  }
  return rgb;
}

bool CHAIN::setRGBLight(uint8_t id, uint8_t light, unsigned long timeout) {
  cmdSize = 0;
  cmdBuf[cmdSize++] = light;
  chainSendPacket(CHAIN_SET_RGB_LIGHT, cmdBuf, cmdSize, id);
  bool status = waitForData(CHAIN_SET_RGB_LIGHT, id, timeout);
  if (status) {
    if (packCheck(_validPacket.data, _validPacket.size)) {
      return _validPacket.data[6];
    }
  }
  return 0;
}

uint8_t CHAIN::getRGBLight(uint8_t id, unsigned long timeout) {
  cmdSize = 0;
  chainSendPacket(CHAIN_GET_RGB_LIGHT, cmdBuf, cmdSize, id);
  bool status = waitForData(CHAIN_GET_RGB_LIGHT, id, timeout);
  if (status) {
    if (packCheck(_validPacket.data, _validPacket.size)) {
      return _validPacket.data[6];
    }
  }
  return 0;
}

bool CHAIN::getEncoderValue(uint8_t id, int16_t *value, unsigned long timeout) {
  cmdSize = 0;
  chainSendPacket(CHAIN_ENCODER_GET_VALUE, cmdBuf, cmdSize, id);
  bool status = waitForData(CHAIN_ENCODER_GET_VALUE, id, timeout);
  if (status) {
    if (packCheck(_validPacket.data, _validPacket.size)) {
      *value = int16_t(_validPacket.data[7] << 8) | _validPacket.data[6];
      return true;
    }
  }
  return false;
}

bool CHAIN::getEncoderIncValue(uint8_t id, int16_t *incValue,
                               unsigned long timeout) {
  cmdSize = 0;
  chainSendPacket(CHAIN_ENCODER_GET_INC_VALUE, cmdBuf, cmdSize, id);
  bool status = waitForData(CHAIN_ENCODER_GET_INC_VALUE, id, timeout);
  if (status) {
    if (packCheck(_validPacket.data, _validPacket.size)) {
      *incValue = int16_t(_validPacket.data[7] << 8) | _validPacket.data[6];
      return true;
    }
  }
  return false;
}
bool CHAIN::resetEncoderValue(uint8_t id, unsigned long timeout) {
  cmdSize = 0;
  chainSendPacket(CHAIN_ENCODER_RESET_VALUE, cmdBuf, cmdSize, id);
  bool status = waitForData(CHAIN_ENCODER_RESET_VALUE, id, timeout);
  if (status) {
    if (packCheck(_validPacket.data, _validPacket.size)) {
      return true;
    }
  }
  return false;
}
bool CHAIN::resetEncoderIncValue(uint8_t id, unsigned long timeout) {
  cmdSize = 0;
  chainSendPacket(CHAIN_ENCODER_RESET_INC_VALUE, cmdBuf, cmdSize, id);
  bool status = waitForData(CHAIN_ENCODER_RESET_INC_VALUE, id, timeout);
  if (status) {
    if (packCheck(_validPacket.data, _validPacket.size)) {
      return true;
    }
  }
  return false;
}
bool CHAIN::setEncoderABStatus(uint8_t id, uint8_t status,
                               unsigned long timeout) {
  cmdSize = 0;
  cmdBuf[cmdSize++] = status;
  chainSendPacket(CHAIN_ENCODER_SET_AB_STATUS, cmdBuf, cmdSize, id);
  bool st = waitForData(CHAIN_ENCODER_SET_AB_STATUS, id, timeout);
  if (st) {
    if (packCheck(_validPacket.data, _validPacket.size)) {
      return true;
    }
  }
  return false;
}
bool CHAIN::getEncoderABStatus(uint8_t id, uint8_t *status,
                               unsigned long timeout) {
  cmdSize = 0;
  chainSendPacket(CHAIN_ENCODER_GET_AB_STATUS, cmdBuf, cmdSize, id);
  bool st = waitForData(CHAIN_ENCODER_GET_AB_STATUS, id, timeout);
  if (st) {
    if (packCheck(_validPacket.data, _validPacket.size) &&
        _validPacket.data[6] == 0x01) {
      *status = _validPacket.data[6];
      return true;
    }
  }
  return false;
}

bool CHAIN::getAngle12Adc(uint8_t id, uint16_t *adcValue,
                          unsigned long timeout) {
  cmdSize = 0;
  chainSendPacket(CHAIN_ANGLE_GET_12ADC, cmdBuf, cmdSize, id);
  bool status = waitForData(CHAIN_ANGLE_GET_12ADC, id, timeout);
  if (status) {
    if (packCheck(_validPacket.data, _validPacket.size)) {
      *adcValue = (_validPacket.data[7] << 8) | _validPacket.data[6];
      return true;
    }
  }
  return false;
}
bool CHAIN::getAngle8Adc(uint8_t id, uint8_t *adcValue, unsigned long timeout) {
  cmdSize = 0;
  chainSendPacket(CHAIN_ANGLE_PGET_8ADC, cmdBuf, cmdSize, id);
  bool status = waitForData(CHAIN_ANGLE_PGET_8ADC, id, timeout);
  if (status) {
    if (packCheck(_validPacket.data, _validPacket.size)) {
      *adcValue = _validPacket.data[6];
      return true;
    }
  }
  return false;
}
bool CHAIN::setAngleClockWise(uint8_t id, uint8_t direction,
                              unsigned long timeout) {
  cmdSize = 0;
  cmdBuf[cmdSize++] = direction;
  chainSendPacket(CHAIN_ANGLE_SET_CLOCKWISE_STATUS, cmdBuf, cmdSize, id);
  bool status = waitForData(CHAIN_ANGLE_SET_CLOCKWISE_STATUS, id, timeout);
  if (status) {
    if (packCheck(_validPacket.data, _validPacket.size) &&
        _validPacket.data[6] == 0x01) {
      return true;
    }
  }
  return false;
}
bool CHAIN::getAngleClockWise(uint8_t id, uint8_t *direction,
                              unsigned long timeout) {
  cmdSize = 0;
  chainSendPacket(CHAIN_ANGLE_GET_CLOCKWISE_STATUS, cmdBuf, cmdSize, id);
  bool status = waitForData(CHAIN_ANGLE_GET_CLOCKWISE_STATUS, id, timeout);
  if (status) {
    if (packCheck(_validPacket.data, _validPacket.size)) {
      *direction = _validPacket.data[6];
      return true;
    }
  }
  return false;
}

bool CHAIN::getJoystick16Adc(uint8_t id, uint16_t *adcXValue,
                             uint16_t *adcYValue, unsigned long timeout) {
  cmdSize = 0;
  chainSendPacket(CHAIN_JOYSTICK_GET_16ADC, cmdBuf, cmdSize, id);
  bool status = waitForData(CHAIN_JOYSTICK_GET_16ADC, id, timeout);
  if (status) {
    if (packCheck(_validPacket.data, _validPacket.size)) {
      *adcXValue = uint16_t(_validPacket.data[7] << 8) | _validPacket.data[6];
      *adcYValue = uint16_t(_validPacket.data[9] << 8) | _validPacket.data[8];
      return true;
    }
  }
  return false;
}
bool CHAIN::getJoystick8Adc(uint8_t id, uint8_t *adcXValue, uint8_t *adcYValue,
                            unsigned long timeout) {
  cmdSize = 0;
  chainSendPacket(CHAIN_JOYSTICK_GET_8ADC, cmdBuf, cmdSize, id);
  bool status = waitForData(CHAIN_JOYSTICK_GET_8ADC, id, timeout);
  if (status) {
    if (packCheck(_validPacket.data, _validPacket.size)) {
      *adcXValue = _validPacket.data[6];
      *adcYValue = _validPacket.data[6];
      return true;
    }
  }
  return false;
}
bool CHAIN::setJoystickMappedRange(uint8_t id, int16_t *buf, uint8_t size,
                                   unsigned long timeout) {
  cmdSize = 0;
  for (uint8_t i = 0; i < size; i++) {
    cmdBuf[cmdSize++] = (uint8_t)buf[i] & 0xFF;
    cmdBuf[cmdSize++] = (uint8_t)(buf[i] >> 8) & 0xFF;
  }
  chainSendPacket(CHAIN_JOYSTICK_SET_ADC_XY_MAPPED_RANGE, cmdBuf, cmdSize, id);
  bool status =
      waitForData(CHAIN_JOYSTICK_SET_ADC_XY_MAPPED_RANGE, id, timeout);
  if (status) {
    if (packCheck(_validPacket.data, _validPacket.size) &&
        _validPacket.data[6] == 0x01) {
      return true;
    }
  }
  return false;
}
bool CHAIN::getJoystickMappedRange(uint8_t id, int16_t *buf, uint8_t size,
                                   unsigned long timeout) {
  cmdSize = 0;
  chainSendPacket(CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_RANGE, cmdBuf, cmdSize, id);
  bool status =
      waitForData(CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_RANGE, id, timeout);
  if (status) {
    if (packCheck(_validPacket.data, _validPacket.size)) {
      for (uint8_t i = 0; i < size; i++) {
        buf[i] = (int16_t)(_validPacket.data[7 + i * 2] << 8) |
                 _validPacket.data[6 + i * 2];
      }
      return true;
    }
  }
  return false;
}
bool CHAIN::getJoystickMappedInt16Value(uint8_t id, int16_t *x, int16_t *y,
                                        unsigned long timeout) {
  cmdSize = 0;
  chainSendPacket(CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_INT16_VALUE, cmdBuf, cmdSize,
                  id);
  bool status =
      waitForData(CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_INT16_VALUE, id, timeout);
  if (status) {
    if (packCheck(_validPacket.data, _validPacket.size)) {
      *x = int16_t(_validPacket.data[7] << 8) | _validPacket.data[6];
      *y = int16_t(_validPacket.data[9] << 8) | _validPacket.data[8];
      // Serial.printf("JoyStick16 x: %d  JoyStick16 Y: %d\n",
      // int16_t(_validPacket.data[7] << 8) | _validPacket.data[6],
      // int16_t(_validPacket.data[9] << 8));
      // Serial.printf("rx_data:");
      // for(uint8_t i =0;i<_validPacket.size;i++){
      //   Serial.printf(" 0x%02x ",_validPacket.data[i]);
      // }
      // Serial.println();
      return true;
    }
  }
  return false;
}
bool CHAIN::getJoystickMappedInt8Value(uint8_t id, int8_t *x, int8_t *y,
                                       unsigned long timeout) {
  cmdSize = 0;
  chainSendPacket(CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_INT8_VALUE, cmdBuf, cmdSize,
                  id);
  bool status =
      waitForData(CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_INT8_VALUE, id, timeout);
  if (status) {
    if (packCheck(_validPacket.data, _validPacket.size)) {
      *x = (int8_t)_validPacket.data[6];
      *y = (int8_t)_validPacket.data[7];
      return true;
    }
  }
  return false;
}

bool CHAIN::getTofDistance(uint8_t id, uint16_t *distance,
                           unsigned long timeout) {
  cmdSize = 0;
  chainSendPacket(CHAIN_TOF_GET_DISTANCE, cmdBuf, cmdSize, id);
  bool status = waitForData(CHAIN_TOF_GET_DISTANCE, id, timeout);
  if (status) {
    if (packCheck(_validPacket.data, _validPacket.size)) {
      *distance = (uint16_t)(_validPacket.data[7] << 8) | _validPacket.data[6];
      return true;
    }
  }
  return false;
}
bool CHAIN::setTofMode(uint8_t id, uint8_t mode, unsigned long timeout) {
  cmdSize = 0;
  cmdBuf[cmdSize++] = mode;
  chainSendPacket(CHAIN_TOF_SET_MODE, cmdBuf, cmdSize, id);
  bool status = waitForData(CHAIN_TOF_SET_MODE, id, timeout);
  if (status) {
    if (packCheck(_validPacket.data, _validPacket.size) &&
        _validPacket.data[6] == 0x01) {
      return true;
    }
  }
  return false;
}
bool CHAIN::getTofMode(uint8_t id, uint8_t *mode, unsigned long timeout) {
  cmdSize = 0;
  chainSendPacket(CHAIN_TOF_GET_MODE, cmdBuf, cmdSize, id);
  bool status = waitForData(CHAIN_TOF_GET_MODE, id, timeout);
  if (status) {
    if (packCheck(_validPacket.data, _validPacket.size)) {
      *mode = (int8_t)_validPacket.data[6];
      return true;
    }
  }
  return false;
}

bool CHAIN::chianUartI2cInit(uint8_t id, uint8_t *status,
                             unsigned long timeout) {
  return false;
}

bool CHAIN::chainUartI2cRead(uint8_t id, uint8_t i2cAddr, uint8_t readLength,
                             uint8_t *buffer, uint8_t *status,
                             unsigned long timeout) {
  return false;
}
bool CHAIN::chainUartI2cWrite(uint8_t id, uint8_t i2cAddr, uint8_t write,
                              uint8_t *buffer, uint8_t *status,
                              unsigned long timeout) {
  return false;
}
bool CHAIN::chainUartI2cMemRead(uint8_t id, uint8_t i2cAddr, uint16_t regAddr,
                                uint8_t regLength, uint8_t readLength,
                                uint8_t *buffer, uint8_t *status,
                                unsigned long timeout) {
  return false;
}
bool CHAIN::chainUartI2cMemWrite(uint8_t id, uint8_t i2cAddr, uint16_t regAddr,
                                 uint8_t regLength, uint8_t writeLength,
                                 uint8_t *buffer, uint8_t *status,
                                 unsigned long timeout) {
  return false;
}
bool CHAIN::getChainUartI2cDeviceAddr(uint8_t id, uint8_t *i2cAddrNums,
                                      uint8_t *buffer, uint8_t *status,
                                      unsigned long timeout) {
  return false;
}
bool CHAIN::chainUartOutputInit(uint8_t id, uint8_t gpio, uint8_t gpioLevel,
                                uint8_t gpioMode, uint8_t gpioUp,
                                uint8_t gpioSpeed, uint8_t *status,
                                unsigned long timeout) {
  return false;
}
bool CHAIN::chainUartInputInit(uint8_t id, uint8_t gpio, uint8_t gpioUp,
                               uint8_t *status, unsigned long timeout) {
  return false;
}
bool CHAIN::chainUartReadLevel(uint8_t id, uint8_t gpio, uint8_t *status,
                               unsigned long timeout) {
  return false;
}
bool CHAIN::chainUartNvicInit(uint8_t id, uint8_t gpio, uint8_t gpioUp,
                              uint8_t triggerMode, uint8_t *status,
                              unsigned long timeout) {
  return false;
}
bool CHAIN::chainUartAdcInit(uint8_t id, uint8_t adcChannel1,
                             uint8_t adcChannel2, uint8_t *status,
                             unsigned long timeout) {
  cmdSize = 0;
  cmdBuf[cmdSize++] = adcChannel1;
  cmdBuf[cmdSize++] = adcChannel2;
  chainSendPacket(CHAIN_GPIO_ADC_INIT, cmdBuf, cmdSize, id);
  bool sta = waitForData(CHAIN_GPIO_ADC_INIT, id, timeout);
  if (sta) {
    if (packCheck(_validPacket.data, _validPacket.size) &&
        _validPacket.data[6] == 0x01) {
      *status = 1;
      return true;
    }
  }
  *status = 0;
  return false;
}
bool CHAIN::getAdcValue(uint8_t id, uint8_t *adcChannelNums, uint8_t *buffer,
                        uint8_t *status, unsigned long timeout) {
  cmdSize = 0;
  chainSendPacket(CHAIN_GPIO_ADC_READ, cmdBuf, cmdSize, id);
  bool sta = waitForData(CHAIN_GPIO_ADC_READ, id, timeout);
  if (sta) {
    if (packCheck(_validPacket.data, _validPacket.size) &&
        _validPacket.data[6] == 0x01) {
      *status = 1;
      *adcChannelNums = _validPacket.data[7];
      for (uint8_t i = 0; i < (*adcChannelNums) * 3; i++) {
        buffer[i] = _validPacket.data[8 + i];
      }
      return true;
    }
  }
  *status = 0;
  return false;
}