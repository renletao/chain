/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TOF_FUNCTION_H_
#define TOF_FUNCTION_H_

#ifdef __cplusplus

extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "base_function.h"
#include "i2c.h"

#define VL53L0X_ADDRESS           (0x29)  // VL53L0X 激光测距传感器的 I2C 地址
// VL53L0X 寄存器地址宏定义
// VL53L0X 寄存器地址宏定义
#define VL53L0X_SYSRANGE_START                        0x00  // 启动测距
#define VL53L0X_SYSRANGE_STATUS                       0x01  // 测距状态
#define VL53L0X_RESULT_RANGE_STATUS                   0x0D  // 测距结果状态
#define VL53L0X_RESULT_INTERRUPT_STATUS               0x0E  // 中断状态
#define VL53L0X_I2C_SLAVE_DEVICE_ID                   0x14  // I2C 从设备 ID
#define VL53L0X_SYSRANGE_CONFIG                       0x15  // 测距配置
#define VL53L0X_SYSTEM_SEQUENCE_CONFIG                0x17  // 系统测量序列配置
#define VL53L0X_SYSTEM_RANGE_CONFIG                   0x18  // 测距模式配置
#define VL53L0X_SYSTEM_THRESH_LOW                     0x19  // 距离下限阈值
#define VL53L0X_SYSTEM_THRESH_HIGH                    0x1A  // 距离上限阈值
#define VL53L0X_SYSTEM_MODE_ON                        0x1B  // 系统模式开启
#define VL53L0X_RESULT_INTERRUPT_STATUS               0x20  // 中断状态结果
#define VL53L0X_READOUT_CTRL                          0x21  // 读取控制
#define VL53L0X_VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV     0x27  // VHV 和 PAD 配置
#define VL53L0X_GPIO_HV_MUX_ACTIVE                    0x2E  // GPIO 高压多路复用激活
#define VL53L0X_GPIO_HV_MUX_CONFIG                    0x2F  // GPIO 高压多路复用配置
#define VL53L0X_GPIO_DDR                              0x30  // GPIO 数据方向
#define VL53L0X_GPIO_OUT                              0x31  // GPIO 输出
#define VL53L0X_GPIO_IN                               0x32  // GPIO 输入
#define VL53L0X_SPAD_READOUT_CTRL                     0x33  // SPAD 读出控制
#define VL53L0X_DYNAMIC_SPAD_NUM                      0x36  // 动态 SPAD 数量
#define VL53L0X_IDENTIFICATION_MODEL_ID               0xFF  // 设备型号 ID
#define VL53L0X_IDENTIFICATION_REVISION_ID            0xFF  // 设备版本 ID

void tof_init(void);

#ifdef __cplusplus
}
#endif

#endif /* TOF_FUNCTION_H_ */
