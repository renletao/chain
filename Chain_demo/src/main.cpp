#include "M5Unified.h"
#include "Chain.h"

CHAIN chain;
deviceList *devices;
uint8_t dev_nums = 0;

Key keyStatus;

uint16_t tof_distance = 0;

#define TXD_PIN GPIO_NUM_17 // 串口发送端口(Tx)
#define RXD_PIN GPIO_NUM_16 // 串口接收端口(Rx)

/// 定义屏幕尺寸
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// 圆心坐标和半径
#define CIRCLE_X (SCREEN_WIDTH / 2)
#define CIRCLE_Y (SCREEN_HEIGHT / 2)
#define CIRCLE_RADIUS 115

// 小球半径
#define BALL_RADIUS 10

// 小球的初始位置
int ballX = CIRCLE_X;
int ballY = CIRCLE_Y;

// 上一帧小球的位置
int lastBallX = ballX;
int lastBallY = ballY;

const int startX = 5;                              // 表格左上角 x 坐标
const int startY = 40;                             // 表格左上角 y 坐标
const int endX = 315;                              // 表格右下角 x 坐标
const int endY = 220;                              // 表格右下角 y 坐标
const int rows = 10;                               // 行数
const int cols = 6;                                // 列数
const int colWidths[6] = {30, 80, 60, 60, 40, 40}; // 列宽数组
uint16_t headerColors[6] = {
    M5.Display.color565(0, 0, 0), // 第一列颜色（红色）
    M5.Display.color565(0, 0, 0), // 第二列颜色（绿色）
    M5.Display.color565(0, 0, 0), // 第三列颜色（蓝色）
    M5.Display.color565(0, 0, 0), // 第四列颜色（黄色）
    M5.Display.color565(0, 0, 0), // 第五列颜色（橙色）
    M5.Display.color565(0, 0, 0)  // 第五列颜色（橙色）
};
uint16_t bgColor = M5.Display.color565(0, 0, 0); // 表格背景颜色（黑色）
uint16_t defaultHeaderColor = M5.Display.color565(0, 0, 0);

RGB_Color_TypeDef rgb = {0x00, 0x00, 0x00};

void viewMenu(void);
void viewIndexUpadte(void);
void viewEncoderTest(uint8_t id);
void viewKeyTest(uint8_t id);
void viewAngleTest(uint8_t id);
void viewJoystickTest(uint8_t id);
void viewTofTest(uint8_t id);
void viewUartTest(uint8_t id);
void setCellColor(int row, int col, uint16_t color);
void drawTable(void);
void addDataToCell(int row, int col, String data, uint16_t color);
void addSwitchToCell(int row, int col, bool isOn);
void clearTableCell(int row, int col);
void fillTable(const deviceList *devices);
void printDeviceList(deviceList *devices);
long myMap(long x, long in_min, long in_max, long out_min, long out_max);
void changeRGB(int16_t encodeIncValue, uint8_t rgb_change_status,
               RGB_Color_TypeDef *rgb);
void drawSwitch(int x, int y, uint8_t switchState);
void drawCenteredText(const char *text, int x, int y, uint16_t color);
void drawProgressBar(int adcValue, int x, int y, int width, int height,
                     int radius, const char *text);
void drawCircleAndBall();
void updateBallPosition(int joyX, int joyY);
void moveBallToCenter();

void displayHeartBasedOnDistance(uint16_t distance);
void drawFilledHeart(int x, int y, float size);

bool checkConnection(void);

uint8_t test_index = 0;
bool operationStatus;

void setup() {
  M5.begin();
  Serial.begin(115200);
  chain.begin(&Serial2, 115200, RXD_PIN, TXD_PIN);
  while (!chain.isDeviceConnected()) {
    delay(10);
  }
  delay(200);
  dev_nums = chain.getDeviceNum();
  while (!dev_nums) { // 确保设备获取成功
    dev_nums = chain.getDeviceNum();
    delay(10);
  }
  Serial.printf("数量：%d \r\n", dev_nums);
  devices = (deviceList *)malloc(sizeof(deviceList)); // 分配内存
  devices->count = dev_nums;
  devices->devices =
      (device *)malloc(sizeof(device) * dev_nums); // 分配设备数组的内存
  chain.getDeviceList(devices);
  printDeviceList(devices);
  chain.clearEnumPleaseNums();
  // 关闭所有RGB值
  for (uint8_t i = 1; i <= devices->count; i++) {
    while (!chain.setRGBValue(i, rgb)) {
      delay(1);
    }
    if (devices->devices[i - 1].device_type == CHAIN_UART) {
      uint8_t status = 0;
      operationStatus = chain.chainUartAdcInit(devices->devices[i - 1].id, 0x01,
                                               0x00, &status);
      while (!operationStatus && !status) {
        operationStatus = chain.chainUartAdcInit(devices->devices[i - 1].id,
                                                 0x01, 0x00, &status);
        delay(1);
      }
    }
  }
  viewMenu();

  //  M5.Display.setTextFont(&fonts::efontCN_24);
  //  M5.Display.printf("  enum device test 10ms");
}

bool encodeBtonStatus = 0;
bool keyBtonStatus = 0;
bool joystickBtonStatus = 0;

int16_t encodeIncValue = 0;
int16_t encodeValue = 0;

uint16_t angleAdc12 = 0;
uint16_t adcTemp16 = 0;

int16_t temp = 0;
int16_t temp1 = 0;

int16_t joyStickX16 = 0;
int16_t joyStickY16 = 0;

uint16_t tofDistance = 0;
uint16_t tofTemp = 0;

uint8_t buf[6] = {0};
uint16_t uartAdc = 0;

uint32_t ans1 = 0;
uint32_t ans2 = 0;
uint8_t num = 0;

uint8_t update_nums = 0;
void loop() {
  M5.update();
  // num = 0;
  // num = chain.getDeviceNum();
  // Serial.printf("device num %d\r\n", num);
  // if (num > 0) {
  //   ans1++;
  // } else {
  //   // M5.Speaker.tone(990, 200);
  //   ans2++;
  // }
  // M5.Display.fillRect(180, 80, 120, 180, BLACK);
  // M5.Display.setCursor(80, 80);
  // M5.Display.printf("success: %d", ans1);
  // M5.Display.setCursor(80, 120);
  // M5.Display.printf("   fail: %d", ans2);
  // M5.Display.setCursor(80, 160);
  // M5.Display.printf("    num: %d", num);
  // delay(10);
  // ******
  if (checkConnection()) {
    if (M5.BtnB.wasReleased() || M5.BtnB.pressedFor(500)) {
      switch (devices->devices[test_index].device_type) {
      case CHAIN_ENCODER:
        viewEncoderTest(devices->devices[test_index].id);
        break;
      case CHAIN_ANGLE:
        viewAngleTest(devices->devices[test_index].id);
        break;
      case CHAIN_KEY:
        viewKeyTest(devices->devices[test_index].id);
        break;
      case CHAIN_JOYSTICK:
        viewJoystickTest(devices->devices[test_index].id);
        break;
      case CHAIN_TOF:
        viewTofTest(devices->devices[test_index].id);
        break;
      case CHAIN_UART:
        viewUartTest(devices->devices[test_index].id);
        break;
      default:
        break;
      }
    }
    if (M5.BtnC.wasReleased() || M5.BtnC.pressedFor(500)) {
      viewIndexUpadte();
    }
    for (uint8_t i = 0; i < devices->count; i++) {
      if (devices->devices[i].device_type == CHAIN_TOF) {
        operationStatus =
            chain.getTofDistance(devices->devices[i].id, &tofTemp);
        if (operationStatus) {
          tofDistance = tofTemp;
          ans2++;
        } else {
          ans1++;
          Serial.printf("TOF ERROR\r\n");
        }
        // Serial.printf("distance : %d\n", tofDistance);
      } else if (devices->devices[i].device_type == CHAIN_ENCODER) {

        operationStatus =
            chain.getEncoderValue(devices->devices[i].id, &temp, 30);
        if (operationStatus) {
          encodeValue = temp;
          ans2++;
        } else {
           Serial.printf("ENCODE ERROR\r\n");
          ans1++;
        }
        operationStatus =
            chain.getEncoderIncValue(devices->devices[i].id, &temp, 30);
        if (operationStatus) {
          encodeIncValue = temp;
          ans2++;
        } else {
          Serial.printf("ENCODE INC ERROR\r\n");
          ans1++;
        }
        // Serial.printf("Incremental Value: %d\n", encodeIncValue);
        // Serial.printf("Encoder Value: %d\n", encodeValue);
      } else if (devices->devices[i].device_type == CHAIN_ANGLE) {
        operationStatus =
            chain.getAngle12Adc(devices->devices[i].id, &adcTemp16, 30);
        if (operationStatus) {
          angleAdc12 = adcTemp16;
          ans2++;
        } else {
          Serial.printf("ANGLE ERROR\r\n");
          ans1++;
        }
        // Serial.printf("Angle 12Value: %d\n", angleAdc12);
      } else if (devices->devices[i].device_type == CHAIN_JOYSTICK) {
        operationStatus = chain.getJoystickMappedInt16Value(
            devices->devices[i].id, &temp, &temp1, 30);
        if (operationStatus) {
          joyStickX16 = temp;
          joyStickY16 = temp1;
          ans2++;
        } else {
          Serial.printf("JOYSTICK ERROR\r\n");
          ans1++;
        }
        // Serial.printf("JoyStick16 x: %d  JoyStick16 Y: %d\n", joyStickX16,
        //               joyStickY16);
      } else if (devices->devices[i].device_type == CHAIN_UART) {
        uint8_t status = 0;
        uint8_t adcNums = 0;
        operationStatus =
            chain.getAdcValue(devices->devices[i].id, &adcNums, buf, &status);
        if (operationStatus && status) {
          uartAdc = (buf[1] << 8) | buf[2];
          ans2++;
        } else {
          Serial.printf("UART ERROR\r\n");
          ans1++;
        }
        // Serial.printf("Uart 12Value: %d\n", uartAdc);
      }
    }
    if (chain.available()) {
      chain.receive();
      chain.parsePackets();
    }
    keyStatus = chain.getKeyStatus();
    if (keyStatus.isPressed) {
      if (devices->devices[keyStatus.deviceId - 1].device_type ==
          CHAIN_ENCODER) {
        M5.Speaker.tone(990, 200);
        encodeBtonStatus = 1 - encodeBtonStatus;
      } else if (devices->devices[keyStatus.deviceId - 1].device_type ==
                 CHAIN_KEY) {
        M5.Speaker.tone(990, 200);
        keyBtonStatus = 1 - keyBtonStatus;
      } else if (devices->devices[keyStatus.deviceId - 1].device_type ==
                 CHAIN_JOYSTICK) {
        M5.Speaker.tone(990, 200);
        joystickBtonStatus = 1 - joystickBtonStatus;
      }
    }

    for (uint8_t i = 0; i < devices->count; i++) {
      if (devices->devices[i].device_type == CHAIN_TOF) {
        addDataToCell(devices->devices[i].id, 2, String(tofDistance), GREEN);
        addDataToCell(devices->devices[i].id, 3, "", GREEN);
        addDataToCell(devices->devices[i].id, 4, "", GREEN);
      } else if (devices->devices[i].device_type == CHAIN_ENCODER) {
        addDataToCell(devices->devices[i].id, 2, String(encodeValue), GREEN);
        addDataToCell(devices->devices[i].id, 3, String(encodeIncValue), GREEN);
        addSwitchToCell(devices->devices[i].id, 4, encodeBtonStatus);
      } else if (devices->devices[i].device_type == CHAIN_ANGLE) {
        addDataToCell(devices->devices[i].id, 2, String(angleAdc12), GREEN);
        addDataToCell(devices->devices[i].id, 3, "", GREEN);
        addDataToCell(devices->devices[i].id, 4, "", GREEN);
      } else if (devices->devices[i].device_type == CHAIN_JOYSTICK) {
        addDataToCell(devices->devices[i].id, 2, String(joyStickX16), GREEN);
        addDataToCell(devices->devices[i].id, 3, String(joyStickY16), GREEN);
        addSwitchToCell(devices->devices[i].id, 4, joystickBtonStatus);
      } else if (devices->devices[i].device_type == CHAIN_KEY) {
        addDataToCell(devices->devices[i].id, 2, "", GREEN);
        addDataToCell(devices->devices[i].id, 3, "", GREEN);
        addSwitchToCell(devices->devices[i].id, 4, keyBtonStatus);
      } else if (devices->devices[i].device_type == CHAIN_UART) {
        addDataToCell(devices->devices[i].id, 2, String(uartAdc), GREEN);
        addDataToCell(devices->devices[i].id, 3, "", GREEN);
        addDataToCell(devices->devices[i].id, 4, "", GREEN);
      }
    }
    addDataToCell(7, 1, String(ans2), GREEN);
    addDataToCell(8, 1, String(ans1), RED);
    // addDataToCell(8, 2, String(update_nums), RED);
    if (chain.getEnumPleaseNums() >= 1) {
      update_nums++;
      delay(100);
      dev_nums = chain.getDeviceNum();
      while (!dev_nums) { // 确保设备获取成功
        dev_nums = chain.getDeviceNum();
      }
      free(devices->devices);
      devices->count = dev_nums;
      devices->devices =
          (device *)malloc(sizeof(device) * dev_nums); // 分配设备数组的内存
      chain.getDeviceList(devices);
      viewMenu();
      chain.clearEnumPleaseNums();
      for (uint8_t i = 1; i <= devices->count; i++) {
        while (!chain.setRGBValue(i, rgb)) {
          delay(1);
        }
        if (devices->devices[i - 1].device_type == CHAIN_UART) {
          uint8_t status = 0;
          operationStatus = chain.chainUartAdcInit(devices->devices[i - 1].id,
                                                   0x01, 0x00, &status);
          while (!operationStatus && !status) {
            operationStatus = chain.chainUartAdcInit(devices->devices[i - 1].id,
                                                     0x01, 0x00, &status);
            delay(1);
          }
        }
      }
    }
  } else {
    M5.Display.fillScreen(BLACK);
    M5.Display.setTextFont(&fonts::efontCN_24);
    M5.Display.setCursor(5, 80);
    M5.Display.printf("  Device Disconnected\r\n");
    M5.Display.printf("    Please reconnect");
    while (!checkConnection()) {
      delay(10);
    }
    delay(200);
    dev_nums = chain.getDeviceNum();
    while (!dev_nums) { // 确保设备获取成功
      dev_nums = chain.getDeviceNum();
      delay(10);
    }
    Serial.printf("数量：%d \r\n", dev_nums);
    devices = (deviceList *)malloc(sizeof(deviceList)); // 分配内存
    devices->count = dev_nums;
    devices->devices =
        (device *)malloc(sizeof(device) * dev_nums); // 分配设备数组的内存
    chain.getDeviceList(devices);
    printDeviceList(devices);
    chain.clearEnumPleaseNums();
    viewMenu();
  }
  delay(30);
}

// 设置单元格颜色
void setCellColor(int row, int col, uint16_t color) {
  if (row < 0 || row >= rows || col < 0 || col >= cols)
    return; // 检查有效性

  int cellHeight = (endY - startY) / rows; // 单元格高度
  int xPos = startX;

  // 计算单元格的x坐标
  for (int j = 0; j < col; j++) {
    xPos += colWidths[j];
  }

  // 填充单元格颜色
  M5.Display.fillRect(xPos + 1, startY + row * cellHeight + 1,
                      colWidths[col] - 2, cellHeight - 2, color);
}

void drawTable(void) {
  int cellHeight = (endY - startY) / rows;

  // // 绘制表格背景
  M5.Display.fillRect(startX, startY, endX - startX, endY - startY, bgColor);

  // 绘制水平线
  for (int i = 0; i <= rows; i++) {
    M5.Display.drawLine(startX, startY + i * cellHeight, endX,
                        startY + i * cellHeight, WHITE);
  }

  // 绘制垂直线
  int currentX = startX;
  for (int j = 0; j <= cols; j++) {
    M5.Display.drawLine(currentX, startY, currentX, endY, WHITE);
    currentX += (j < cols) ? colWidths[j] : 0; // 更新x坐标
  }

  // 设置第一行颜色（可以自定义），其他行使用每列的颜色
  currentX = startX;
  for (int col = 0; col < cols; col++) {
    M5.Display.fillRect(currentX + 1, startY + 1, colWidths[col] - 2,
                        cellHeight - 2, defaultHeaderColor); // 第一行的颜色
    currentX += colWidths[col];
  }

  // 绘制其他行
  for (int row = 1; row < rows; row++) { // 从第二行开始
    currentX = startX;
    for (int col = 0; col < cols; col++) {
      M5.Display.fillRect(currentX + 1, startY + row * cellHeight + 1,
                          colWidths[col] - 2, cellHeight - 2,
                          headerColors[col]); // 其他行的颜色
      currentX += colWidths[col];
    }
  }
  M5.Display.setTextFont(&fonts::efontCN_14);
  addDataToCell(0, 0, "ID", (uint16_t)M5.Display.color565(0, 211, 211));
  addDataToCell(0, 1, "   TYPE", (uint16_t)M5.Display.color565(0, 211, 211));
  addDataToCell(0, 2, " VALUE", (uint16_t)M5.Display.color565(0, 211, 211));
  addDataToCell(0, 3, " VALUE", (uint16_t)M5.Display.color565(0, 211, 211));
  addDataToCell(0, 4, "BTON", (uint16_t)M5.Display.color565(0, 211, 211));
  addDataToCell(0, 5, "TEST", (uint16_t)M5.Display.color565(0, 211, 211));
}

void addDataToCell(int row, int col, String data, uint16_t color) {
  clearTableCell(row, col);
  if (row < 0 || row >= rows || col < 0 || col >= cols)
    return; // 检查有效性

  int cellHeight = (endY - startY) / rows; // 单元格高度

  // 计算单元格的x坐标
  int xPos = startX;
  for (int j = 0; j < col; j++) {
    xPos += colWidths[j];
  }

  // 在指定单元格中插入数据
  M5.Display.setTextSize(1.1);
  M5.Display.setCursor(xPos + 6, startY + row * cellHeight + 1);
  M5.Display.setTextColor(color); // 使用传入的颜色参数
  M5.Display.print(data);
}

void addSwitchToCell(int row, int col, bool isOn) {
  clearTableCell(row, col);
  if (row < 0 || row >= rows || col < 0 || col >= cols)
    return; // 检查有效性

  int cellHeight = (endY - startY) / rows; // 单元格高度

  // 计算单元格的x坐标
  int xPos = startX;
  for (int j = 0; j < col; j++) {
    xPos += colWidths[j];
  }

  // 设置开关的宽度和高度
  int switchWidth = colWidths[col] / 2; // 开关宽度占单元格的一半
  int switchHeight = cellHeight / 2;    // 开关高度占单元格的一半

  // 开关的x和y坐标
  int switchX = xPos + (colWidths[col] - switchWidth) / 2;
  int switchY = startY + row * cellHeight + (cellHeight - switchHeight) / 2;

  if (isOn) {
    // 显示“开”状态的图案
    M5.Display.fillRoundRect(switchX - 4, switchY - 2, switchWidth + 8,
                             switchHeight + 5, 5, GREEN); // 绿色开关
    M5.Display.setTextColor(WHITE);
    M5.Display.setCursor(xPos + 11, startY + row * cellHeight);
    M5.Display.setTextSize(1.1);
    M5.Display.print("ON");
  } else {
    // 显示“关”状态的图案
    M5.Display.fillRoundRect(switchX - 4, switchY - 2, switchWidth + 8,
                             switchHeight + 5, 5, RED); // 红色开关
    M5.Display.setTextColor(WHITE);
    M5.Display.setCursor(xPos + 9, startY + row * cellHeight);
    M5.Display.setTextSize(1.1);
    M5.Display.print("OFF");
  }
}
void clearTableCell(int row, int col) {
  if (row < 0 || row >= rows || col < 0 || col >= cols)
    return; // 检查有效性

  int cellHeight = (endY - startY) / rows; // 单元格高度

  // 计算单元格的x坐标
  int xPos = startX;
  for (int j = 0; j < col; j++) {
    xPos += colWidths[j];
  }

  // 清除单元格
  M5.Display.fillRect(xPos + 1, startY + row * cellHeight + 1,
                      colWidths[col] - 2, cellHeight - 2, bgColor);
}

const char *deviceTypeToString(uint8_t type) {
  switch (type) {
  case 1:
    return "ENCODER";
  case 2:
    return "ANGLE";
  case 3:
    return "KEY";
  case 4:
    return "JOYSTICK";
  case 5:
    return "TOF";
  case 6:
    return "UART";
  default:
    return "UNKNOWN";
  }
}

void fillTable(const deviceList *devices) {
  M5.Display.setTextFont(&fonts::efontCN_14);
  for (int i = 0; i < devices->count; i++) {
    const device &dev = devices->devices[i]; // 引用简化访问
    addDataToCell(i + 1, 0, String(dev.id),
                  (uint16_t)M5.Display.color565(0, 211, 211));
    addDataToCell(i + 1, 1, deviceTypeToString(dev.device_type),
                  (uint16_t)M5.Display.color565(0, 211, 211)); // 使用转换函数
  }
}

void printDeviceList(deviceList *devices) {
  if (devices == NULL) {
    Serial.println("设备列表为空。");
    return;
  }

  Serial.print("设备数量: ");
  Serial.println(devices->count);

  for (uint8_t i = 0; i < devices->count; i++) {
    Serial.print("设备 ID: ");
    Serial.println(devices->devices[i].id);
    Serial.print("引导加载程序版本: ");
    Serial.println(devices->devices[i].blVersion);
    Serial.print("固件版本: ");
    Serial.println(devices->devices[i].fwVersion);
    Serial.print("设备类型: ");
    Serial.println(devices->devices[i].device_type);
    Serial.println();
  }
}

long myMap(long x, long in_min, long in_max, long out_min, long out_max) {
  long result;

  // Perform the mapping calculation
  result = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

  // Clamp the result to ensure it's within the output range
  if (result < out_min) {
    result = out_min; // Set to out_min if below range
  } else if (result > out_max) {
    result = out_max; // Set to out_max if above range
  }

  return result; // Return the mapped value
}

void changeRGB(int16_t encodeIncValue, uint8_t rgb_change_status,
               RGB_Color_TypeDef *rgb) {
  if (encodeIncValue != 0) {
    switch (rgb_change_status) {
    case 0: // Change Red
      if (encodeIncValue > 0 && rgb->R + encodeIncValue <= 255) {
        rgb->R += encodeIncValue;
      } else if (encodeIncValue < 0 && rgb->R + encodeIncValue >= 0) {
        rgb->R += encodeIncValue;
      }
      break;
    case 1: // Change Green
      if (encodeIncValue > 0 && rgb->G + encodeIncValue <= 255) {
        rgb->G += encodeIncValue;
      } else if (encodeIncValue < 0 && rgb->G + encodeIncValue >= 0) {
        rgb->G += encodeIncValue;
      }
      break;
    case 2: // Change Blue
      if (encodeIncValue > 0 && rgb->B + encodeIncValue <= 255) {
        rgb->B += encodeIncValue;
      } else if (encodeIncValue < 0 && rgb->B + encodeIncValue >= 0) {
        rgb->B += encodeIncValue;
      }
      break;
    default:
      break;
    }
    for (uint8_t i = 1; i <= devices->count; i++) {
      while (!chain.setRGBValue(i, *rgb)) {
        delay(1);
      }
    }
  }
}

void viewMenu(void) {
  // test_index = 0;
  // encodeBtonStatus = 0;
  // keyBtonStatus = 0;
  // joystickBtonStatus = 0;

  printDeviceList(devices);
  M5.Display.fillScreen(BLACK);
  M5.Display.setTextSize(1);
  M5.Display.setTextFont(&fonts::efontCN_24);
  // 显示设备表格
  M5.Display.setCursor(26, 10);
  M5.Display.setTextColor(WHITE);
  M5.Display.printf("CHAIN DEVICE NUMS IS %d", dev_nums);
  drawTable(); // 绘制表格
  fillTable(devices);
  addDataToCell(test_index + 1, 5, "<<<", RED);
  M5.Display.setTextFont(&fonts::efontCN_16);
  M5.Display.setCursor(20, 220);
  M5.Display.setTextColor(WHITE);
  M5.Display.printf("   back       comfirm      next");
}

void viewIndexUpadte(void) {
  M5.Speaker.tone(990, 200);
  clearTableCell(test_index + 1, 5);
  test_index++;
  test_index = test_index % devices->count;
  addDataToCell(test_index + 1, 5, "<<<", RED);
}

// 函数：根据给定位置绘制开关
void drawSwitch(int x, int y, uint8_t switchState) {
  // 清除开关之前的图形
  M5.Display.fillRect(x, y, 60, 30, BLACK); // 先清空区域

  // 根据开关状态绘制
  if (switchState) {
    // 开关打开状态
    M5.Display.fillRoundRect(x, y, 60, 30, 15, GREEN); // 绘制绿色的开关
    drawCenteredText("ON", x + 30, y + 15, BLACK); // 在开关中间绘制 "ON"
  } else {
    // 开关关闭状态
    M5.Display.fillRoundRect(x, y, 60, 30, 15, RED); // 绘制红色的开关
    drawCenteredText("OFF", x + 30, y + 15, WHITE); // 在开关中间绘制 "OFF"
  }
}

// 函数：在指定位置绘制居中文本
void drawCenteredText(const char *text, int x, int y, uint16_t color) {
  M5.Display.setTextColor(color);
  M5.Display.setTextFont(&fonts::efontCN_24);

  int16_t textWidth = M5.Display.textWidth(text); // 获取文本宽度
  int16_t textHeight = M5.Display.fontHeight();   // 获取文本高度

  // 计算文本的中心位置
  int16_t textX = x - (textWidth / 2);
  int16_t textY = y - (textHeight / 2);

  // 设置光标位置并绘制文本
  M5.Display.setCursor(textX, textY);
  M5.Display.print(text);
}

void viewEncoderTest(uint8_t id) {
  M5.Speaker.tone(990, 200);
  RGB_Color_TypeDef rgb = {0x00, 0x00, 0x00};
  uint8_t rgb_change_status = 0;
  int16_t encodeValue = 0;
  int16_t encodeIncValue = 0;
  uint8_t rgb_test = 0; // 默认不增加
  uint16_t angleAdc12 = 0;
  uint8_t light = 0;

  for (uint8_t i = 1; i <= devices->count; i++) {
    while (!chain.setRGBLight(i, light)) {
      delay(1);
    }
    while (!chain.setRGBValue(i, rgb)) {
      delay(1);
    }
  }

  for (uint8_t i = 0; i < devices->count; i++) {
    if (devices->devices[i].device_type == CHAIN_ANGLE) {
      while (!chain.getAngle12Adc(devices->devices[i].id, &angleAdc12)) {
        delay(1);
      }
    }
  }

  M5.Display.fillScreen(BLACK);
  M5.Display.setTextFont(&fonts::efontCN_24);
  // 显示设备表格
  M5.Display.setCursor(25, 10);
  M5.Display.setTextColor(WHITE);
  M5.Display.printf("CHAIN ENCODE ID IS %d", id);
  M5.Display.setCursor(5, 80);
  M5.Display.setTextColor(WHITE);
  M5.Display.printf("PRESS KEY TEST RGB");
  drawSwitch(250, 80, rgb_test);
  M5.Display.setTextFont(&fonts::efontCN_16);
  for (uint8_t i = 0; i < devices->count; i++) {
    if (devices->devices[i].device_type == CHAIN_ANGLE) {
      while (!chain.getAngle12Adc(devices->devices[i].id, &angleAdc12)) {
        delay(1);
      }
    }
  }
  drawProgressBar(angleAdc12, 20, 180, 280, 25, 10, "rgb light");
  light = myMap(angleAdc12, 0, 4095, 0, 100);
  for (uint8_t i = 1; i <= devices->count; i++) {
    while (!chain.setRGBLight(i, light)) {
      delay(1);
    }
  }
  M5.Display.setTextFont(&fonts::efontCN_16);
  M5.Display.setCursor(20, 220);
  M5.Display.setTextColor(WHITE);
  M5.Display.printf("   back       comfirm      next");
  while (1) {
    M5.update();
    uint16_t lastAngleAdc12 = angleAdc12;
    if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(500)) {
      M5.Speaker.tone(990, 200);
      viewMenu();
      return;
    }
    encodeIncValue = 0;
    encodeValue = 0;
    while (!chain.getEncoderIncValue(id, &encodeIncValue)) {
      delay(1);
    }
    while (!chain.getEncoderValue(id, &encodeValue)) {
      delay(1);
    }
    for (uint8_t i = 0; i < devices->count; i++) {
      if (devices->devices[i].device_type == CHAIN_ANGLE) {
        while (!chain.getAngle12Adc(devices->devices[i].id, &angleAdc12)) {
          delay(1);
        }
      }
    }
    M5.Display.fillRect(185, 40, 180, 40, BLACK);
    M5.Display.setTextFont(&fonts::efontCN_16);
    M5.Display.setTextColor(GREEN);
    M5.Display.setCursor(45, 40);
    M5.Display.printf("Incremental Value: %d\n", encodeIncValue);
    M5.Display.setCursor(45, 60);
    M5.Display.printf("    Encoder Value: %d\n", encodeValue);
    // 按键控制模块
    keyStatus = chain.getKeyStatus();
    if (keyStatus.isPressed) {
      if (devices->devices[keyStatus.deviceId - 1].device_type == CHAIN_KEY) {
        M5.Speaker.tone(990, 200);
        rgb_test = 1 - rgb_test;
        drawSwitch(250, 80, rgb_test);
      } else if (rgb_test == 1 &&
                 devices->devices[keyStatus.deviceId - 1].device_type ==
                     CHAIN_ENCODER) {
        M5.Speaker.tone(990, 200);
        rgb_change_status++;
        rgb_change_status = rgb_change_status % 3;
      }
    }
    if (rgb_test == 1 && encodeIncValue != 0) {
      changeRGB(encodeIncValue, rgb_change_status, &rgb);
    }
    if (lastAngleAdc12 > (angleAdc12 + 2) ||
        lastAngleAdc12 < (angleAdc12 - 2)) {
      drawProgressBar(angleAdc12, 20, 180, 280, 25, 10, "rgb light");
      light = myMap(angleAdc12, 0, 4095, 0, 100);
      for (uint8_t i = 1; i <= devices->count; i++) {
        while (!chain.setRGBLight(i, light)) {
          delay(1);
        }
      }
    }
    M5.Display.fillRect(185, 110, 100, 70, BLACK);
    M5.Display.setTextFont(&fonts::efontCN_16);
    M5.Display.setTextColor(GREEN);
    M5.Display.setCursor(45, 110 + 20 * rgb_change_status);
    M5.Display.printf("                          <-\n");
    M5.Display.setCursor(45, 110);
    M5.Display.printf("    RGB Values: R: %d\n", rgb.R);
    M5.Display.setCursor(45, 130);
    M5.Display.printf("    RGB Values: G: %d\n", rgb.G);
    M5.Display.setCursor(45, 150);
    M5.Display.printf("    RGB Values: B: %d\n", rgb.B);
    delay(20);
  }
}

// 绘制带圆角的进度条函数，正中间显示字符串
void drawProgressBar(int adcValue, int x, int y, int width, int height,
                     int radius, const char *text) {
  // 将 ADC 值映射到进度条宽度范围
  int progressWidth = map(adcValue, 0, 4095, 0, width);

  // 清除进度条区域
  M5.Display.fillRoundRect(x, y, width, height, radius, BLACK);

  // 绘制进度条边框（带圆角）
  M5.Display.drawRoundRect(x, y, width, height, radius, WHITE);

  // 绘制带圆角的进度条填充部分
  M5.Display.fillRoundRect(x, y, progressWidth, height, radius, GREEN);

  // 设置文本的颜色和大小
  M5.Display.setTextColor(RED);

  // 计算文本的位置，使其在进度条中间
  int textWidth = M5.Display.textWidth(text); // 获取文本宽度
  int textX = x + (width - textWidth) / 2;    // 文本水平居中
  int textY = y + (height - 16) / 2; // 文本垂直居中（假设字符高度约为16像素）

  // 绘制字符串
  M5.Display.setCursor(textX, textY);
  M5.Display.print(text);
}

void viewAngleTest(uint8_t id) {
  M5.Speaker.tone(990, 200);
  RGB_Color_TypeDef rgb = {0x00, 0x00, 0x00};
  uint8_t angleAdc8 = 0;
  uint16_t angleAdc12 = 0;
  uint8_t clockWise = 1;
  uint8_t light = 0;
  M5.Display.fillScreen(BLACK);
  M5.Display.setTextFont(&fonts::efontCN_24);
  M5.Display.setTextSize(1);
  M5.Display.setCursor(37, 10);
  M5.Display.setTextColor(WHITE);
  M5.Display.printf("CHAIN ANGLE ID IS %d", id);

  M5.Display.setTextFont(&fonts::efontCN_16);
  for (uint8_t i = 0; i < devices->count; i++) {
    if (devices->devices[i].device_type == CHAIN_ANGLE) {
      while (!chain.getAngle12Adc(devices->devices[i].id, &angleAdc12)) {
        delay(1);
      }
    }
  }
  drawProgressBar(angleAdc12, 20, 120, 280, 20, 10, "angle value");
  light = myMap(angleAdc12, 0, 4095, 0, 100);
  for (uint8_t i = 1; i <= devices->count; i++) {
    while (!chain.setRGBLight(i, light)) {
      delay(1);
    }
  }
  M5.Display.setTextFont(&fonts::efontCN_16);
  M5.Display.setCursor(20, 220);
  M5.Display.setTextColor(WHITE);
  M5.Display.printf("   back       comfirm      next");
  while (1) {
    M5.update();
    uint16_t lastAngleAdc12 = angleAdc12;
    if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(500)) {
      M5.Speaker.tone(990, 200);
      viewMenu();
      return;
    }
    angleAdc8 = 0;
    angleAdc12 = 0;
    while (!chain.getAngle12Adc(id, &angleAdc12)) {
      delay(1);
    }
    while (!chain.getAngle8Adc(id, &angleAdc8)) {
      delay(1);
    }
    M5.Display.fillRect(180, 40, 230, 60, BLACK);
    M5.Display.setTextFont(&fonts::efontCN_16);
    M5.Display.setTextColor(GREEN);
    M5.Display.setCursor(62, 50);
    M5.Display.printf("angle 12adc Value: %d\n", angleAdc12);
    M5.Display.setCursor(62, 80);
    M5.Display.printf(" angle 8adc Value: %d\n", angleAdc8);
    // 按键控制模块
    keyStatus = chain.getKeyStatus();
    if (keyStatus.isPressed) {
      if (devices->devices[keyStatus.deviceId - 1].device_type == CHAIN_KEY) {
        M5.Speaker.tone(990, 200);
        clockWise = 1 - clockWise;
        while (!chain.setAngleClockWise(id, clockWise)) {
          delay(1);
        }
      }
    }
    if (lastAngleAdc12 > (angleAdc12 + 2) ||
        lastAngleAdc12 < (angleAdc12 - 2)) {
      drawProgressBar(angleAdc12, 20, 120, 280, 20, 10, "angle value");
    }
    delay(100);
  }
}

void viewKeyTest(uint8_t id) {

  uint8_t key_status = 0;
  uint8_t encode_key_status = 0;
  uint8_t angle_key_status = 0;
  M5.Speaker.tone(990, 200);
  RGB_Color_TypeDef rgb = {0x00, 0x00, 0x00};

  M5.Display.fillScreen(BLACK);
  M5.Display.setTextSize(1);
  M5.Display.setTextFont(&fonts::efontCN_24);
  // 显示设备表格
  M5.Display.setCursor(42, 10);
  M5.Display.setTextColor(WHITE);
  M5.Display.printf(" CHAIN KEY ID IS %d", id);
  M5.Display.setCursor(10, 100);
  M5.Display.setTextColor(WHITE);
  drawSwitch(45, 100, encode_key_status);
  drawSwitch(125, 100, key_status);
  drawSwitch(205, 100, angle_key_status);
  M5.Display.setTextFont(&fonts::efontCN_16);
  // 显示设备表格
  M5.Display.setCursor(45, 80);
  M5.Display.setTextColor(GREEN);
  M5.Display.printf(" ENCODE     KEY     JOYSTICK ");
  M5.Display.setTextFont(&fonts::efontCN_16);
  M5.Display.setCursor(20, 220);
  M5.Display.setTextColor(WHITE);
  M5.Display.printf("   back       comfirm      next");
  while (1) {
    M5.update();
    if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(500)) {
      M5.Speaker.tone(990, 200);
      viewMenu();
      return;
    }
    if (chain.available()) {
      chain.receive();
      chain.parsePackets();
      chain.clearPackets();
    }
    keyStatus = chain.getKeyStatus();
    if (keyStatus.isPressed) {
      if (devices->devices[keyStatus.deviceId - 1].device_type == CHAIN_KEY) {
        M5.Speaker.tone(990, 200);
        key_status = 1 - key_status;
        drawSwitch(125, 100, key_status);
      } else if (devices->devices[keyStatus.deviceId - 1].device_type ==
                 CHAIN_ENCODER) {
        M5.Speaker.tone(990, 200);
        encode_key_status = 1 - encode_key_status;
        drawSwitch(45, 100, encode_key_status);
      } else if (devices->devices[keyStatus.deviceId - 1].device_type ==
                 CHAIN_JOYSTICK) {
        M5.Speaker.tone(990, 200);
        angle_key_status = 1 - angle_key_status;
        drawSwitch(205, 100, angle_key_status);
      }
    }
    delay(100);
  }
}

// 绘制圆和小球
void drawCircleAndBall() {
  M5.Lcd.fillCircle(CIRCLE_X, CIRCLE_Y, CIRCLE_RADIUS, DARKGREY); // 圆形

  // 清除上一个位置的小球
  M5.Lcd.fillCircle(lastBallX, lastBallY, BALL_RADIUS, BLACK);

  // 绘制新的小球位置
  M5.Lcd.fillCircle(ballX, ballY, BALL_RADIUS, RED);

  // 更新小球的前一帧位置
  lastBallX = ballX;
  lastBallY = ballY;
}

// 更新小球位置
void updateBallPosition(int joyX, int joyY) {
  // 将摇杆值映射到[-1, 1]区间
  // 反转X轴，确保X为正时小球右移，X为负时小球左移
  float mappedJoyX = -(float)joyX / 4095.0f; // 反向映射X轴
  float mappedJoyY = (float)joyY / 4095.0f;  // 保持Y轴正常映射

  int moveSpeed = 5; // 小球移动的速度

  if (abs(joyX) > 100 || abs(joyY) > 100) { // 摇杆有效输入
    ballX += mappedJoyX * moveSpeed;
    ballY += mappedJoyY * moveSpeed;
  } else { // 摇杆静止时，小球向中心靠拢
    moveBallToCenter();
  }

  // 限制小球不能离开圆形范围
  int dx = ballX - CIRCLE_X;
  int dy = ballY - CIRCLE_Y;
  if (dx * dx + dy * dy > CIRCLE_RADIUS * CIRCLE_RADIUS) {
    // 计算小球与圆心的方向
    float angle = atan2(dy, dx);
    ballX = CIRCLE_X + CIRCLE_RADIUS * cos(angle);
    ballY = CIRCLE_Y + CIRCLE_RADIUS * sin(angle);
  }
}

// 小球向圆心靠拢
void moveBallToCenter() {
  int moveSpeed = 10;
  if (ballX < CIRCLE_X)
    ballX += moveSpeed;
  if (ballX > CIRCLE_X)
    ballX -= moveSpeed;
  if (ballY < CIRCLE_Y)
    ballY += moveSpeed;
  if (ballY > CIRCLE_Y)
    ballY -= moveSpeed;
}

void viewJoystickTest(uint8_t id) {
  M5.Speaker.tone(990, 200);
  int16_t joyStickX16 = 0;
  int16_t joyStickY16 = 0;
  int8_t joyStickX8 = 0;
  int8_t joyStickY8 = 0;
  uint8_t joystickKeyStatus = 0;
  M5.Display.fillScreen(BLACK);
  M5.Display.setTextFont(&fonts::efontCN_24);
  // 显示设备表格
  M5.Display.setCursor(20, 10);
  M5.Display.setTextColor(WHITE);
  M5.Display.printf("CHAIN JOYSYICK ID IS %d", id);
  M5.Display.setTextFont(&fonts::efontCN_16);
  M5.Display.setCursor(20, 220);
  M5.Display.setTextColor(WHITE);
  M5.Display.printf("   back       comfirm      next");
  while (1) {
    M5.update();
    if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(500)) {
      M5.Speaker.tone(990, 200);
      viewMenu();
      return;
    }
    while (!chain.getJoystickMappedInt8Value(id, &joyStickX8, &joyStickY8)) {
      delay(1);
    }

    while (!chain.getJoystickMappedInt16Value(id, &joyStickX16, &joyStickY16)) {
      delay(1);
    }
    keyStatus = chain.getKeyStatus();
    if (keyStatus.isPressed) {
      if (devices->devices[keyStatus.deviceId - 1].device_type ==
          CHAIN_JOYSTICK) {
        joystickKeyStatus = 1 - joystickKeyStatus;
        if (joystickKeyStatus == 1) {
          M5.Display.fillScreen(BLACK);
          drawCircleAndBall(); // 初始绘制圆和小球
        } else {
          M5.Display.fillScreen(BLACK);
          M5.Display.setTextFont(&fonts::efontCN_24);
          // 显示设备表格
          M5.Display.setCursor(20, 10);
          M5.Display.setTextColor(WHITE);
          M5.Display.printf("CHAIN JOYSYICK ID IS %d", id);
          M5.Display.setTextFont(&fonts::efontCN_16);
          M5.Display.setCursor(20, 220);
          M5.Display.setTextColor(WHITE);
          M5.Display.printf("   Back       comfirm      next");
        }
      }
    }
    if (joystickKeyStatus == 0) {
      M5.Display.fillRect(30, 80, 280, 120, BLACK);
      M5.Display.setTextFont(&fonts::efontCN_16);
      M5.Display.setTextColor(GREEN);
      M5.Display.setCursor(50, 80);
      M5.Display.printf("16ADC x: %d", joyStickX16);
      M5.Display.setCursor(170, 80);
      M5.Display.printf("16ADC y: %d", joyStickY16);
      M5.Display.setCursor(50, 150);
      M5.Display.printf(" 8ADC x: %d", joyStickX8);
      M5.Display.setCursor(170, 150);
      M5.Display.printf(" 8ADC y: %d", joyStickY8);
      delay(100);
    } else {
      M5.Display.setTextFont(&fonts::efontCN_16);
      M5.Display.setTextColor(GREEN);
      M5.Display.setCursor(5, 5);
      M5.Display.fillRect(5, 5, 80, 20, BLACK);
      M5.Display.printf("X:%d", joyStickX16);
      M5.Display.setCursor(260, 5);
      M5.Display.fillRect(260, 5, 80, 20, BLACK);
      M5.Display.printf("Y:%d", joyStickY16);

      int xAxis = joyStickX16; // X轴
      int yAxis = joyStickY16; // Y轴

      // 更新小球的位置
      updateBallPosition((-xAxis), (-yAxis));

      // 清屏并重新绘制圆和小球
      drawCircleAndBall();

      delay(20); // 控制帧率
    }
  }
}

// 函数定义：根据距离显示爱心图案
void displayHeartBasedOnDistance(uint16_t distance) {
  // 将距离映射到图案大小，使用浮点数进行映射
  float maxSize = 6.0; // 最大图案大小
  float minSize = 1.0; // 最小图案大小

  float heartSize = map(distance, 0, 1000, minSize * 100, maxSize * 100) /
                    100.0; // 根据距离调整图案大小
  if (distance >= 1000) {
    heartSize = 6;
  }

  // 清空屏幕
  M5.Display.fillRect(0, 30, 320, 192, BLACK);

  // 计算心形的中心位置
  int centerX = M5.Display.width() / 2;
  int centerY = M5.Display.height() / 2;

  // 画一个实心爱心
  drawFilledHeart(centerX, centerY, heartSize);
}

// 辅助函数：在给定位置绘制实心爱心图案
void drawFilledHeart(int x, int y, float size) {
  // 填充心形的上半部分和下半部分
  for (float t = 0; t < 2 * PI; t += 0.01) {
    int heartX = size * (16 * pow(sin(t), 3));
    int heartY =
        size * (-13 * cos(t) + 5 * cos(2 * t) + 2 * cos(3 * t) + cos(4 * t));

    // 确保坐标在屏幕范围内
    if (x + heartX >= 0 && x + heartX < 320 && y + heartY >= 0 &&
        y + heartY < 240) {
      M5.Display.drawPixel(x + heartX, y + heartY, RED); // 绘制心形的边界
    }
  }
}

void viewTofTest(uint8_t id) {
  M5.Speaker.tone(990, 200);
  M5.Display.fillScreen(BLACK);
  uint16_t tofDistance = 0;
  M5.Display.setTextFont(&fonts::efontCN_16);
  M5.Display.setTextColor(WHITE);
  M5.Display.setCursor(95, 5);
  M5.Display.print("Distance: ");
  M5.Display.print("    mm");
  M5.Display.setTextFont(&fonts::efontCN_16);
  M5.Display.setCursor(20, 220);
  M5.Display.setTextColor(WHITE);
  M5.Display.printf("   back       comfirm      next");
  while (1) {
    M5.update();
    if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(500)) {
      M5.Speaker.tone(990, 200);
      viewMenu();
      return;
    }
    while (!chain.getTofDistance(id, &tofDistance)) {
      delay(1);
    }
    M5.Display.fillRect(170, 5, 35, 20, BLACK);
    M5.Display.setTextColor(GREEN);
    M5.Display.setCursor(170, 5);
    M5.Display.print(tofDistance);

    displayHeartBasedOnDistance(tofDistance);

    delay(100); // 控制帧率
  }
}

void viewUartTest(uint8_t id) {

  M5.Speaker.tone(990, 200);
  uint8_t buf[6] = {0};
  uint16_t uartAdc = 0;
  bool operationStatus;
  M5.Display.fillScreen(BLACK);
  M5.Display.setTextFont(&fonts::efontCN_24);
  // 显示设备表格
  M5.Display.setCursor(45, 10);
  M5.Display.setTextColor(WHITE);
  M5.Display.setTextSize(1);
  M5.Display.printf("CHAIN UART ID IS %d", id);

  M5.Display.setTextFont(&fonts::efontCN_16);

  drawProgressBar(uartAdc, 20, 120, 280, 20, 10, "uart adc value");
  M5.Display.setTextFont(&fonts::efontCN_16);
  M5.Display.setCursor(20, 220);
  M5.Display.setTextColor(WHITE);
  M5.Display.printf("   back       comfirm      next");
  while (1) {
    M5.update();
    uint16_t lastAdc12 = uartAdc;
    if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(500)) {
      M5.Speaker.tone(990, 200);
      viewMenu();
      return;
    }
    uint8_t status = 0;
    operationStatus = chain.chainUartAdcInit(id, 0x01, 0x00, &status);
    if (operationStatus && status) {
      uint8_t adcNums = 0;
      operationStatus = chain.getAdcValue(id, &adcNums, buf, &status);
      if (operationStatus && status) {
        uartAdc = (buf[1] << 8) | buf[2];
        M5.Display.fillRect(180, 40, 230, 60, BLACK);
        M5.Display.setTextFont(&fonts::efontCN_16);
        M5.Display.setTextColor(GREEN);
        M5.Display.setCursor(60, 60);
        M5.Display.printf("uart 12adc Value: %d\n", uartAdc);
        if (lastAdc12 > (uartAdc + 3) || lastAdc12 < (uartAdc - 3)) {
          drawProgressBar(uartAdc, 20, 120, 280, 20, 10, "uart adc value");
        }
      }
    }
    delay(100);
  }
}

bool checkConnection(void) {
  int attempts = 0;    // 尝试次数计数器
  bool result = false; // 存储检查结果

  while (attempts < 3) {
    result = chain.isDeviceConnected(); // 调用检查函数
    if (result) {                       // 如果成功连接，返回 true
      return true;
    }
    attempts++; // 增加尝试次数
  }

  return false; // 如果三次都没有成功，返回 false
}