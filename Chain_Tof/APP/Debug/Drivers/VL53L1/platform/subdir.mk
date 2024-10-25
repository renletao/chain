################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/VL53L1/platform/IOI2C.c \
../Drivers/VL53L1/platform/vl53l1_platform.c \
../Drivers/VL53L1/platform/vl53l1x.c 

OBJS += \
./Drivers/VL53L1/platform/IOI2C.o \
./Drivers/VL53L1/platform/vl53l1_platform.o \
./Drivers/VL53L1/platform/vl53l1x.o 

C_DEPS += \
./Drivers/VL53L1/platform/IOI2C.d \
./Drivers/VL53L1/platform/vl53l1_platform.d \
./Drivers/VL53L1/platform/vl53l1x.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/VL53L1/platform/%.o Drivers/VL53L1/platform/%.su Drivers/VL53L1/platform/%.cyclo: ../Drivers/VL53L1/platform/%.c Drivers/VL53L1/platform/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -DSTM32G031xx -c -I"C:/Users/15191/Desktop/chain/Chain_Tof/APP/Drivers/VL53L1/core" -I"C:/Users/15191/Desktop/chain/Chain_Tof/APP/Drivers/VL53L1/platform" -I../Core/Inc -I"C:/Users/15191/Desktop/chain/Chain_Tof/APP/Core/flash" -I"C:/Users/15191/Desktop/chain/Chain_Tof/APP/Core/RGB" -I"C:/Users/15191/Desktop/chain/Chain_Tof/APP/Core/Chain_Function" -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-VL53L1-2f-platform

clean-Drivers-2f-VL53L1-2f-platform:
	-$(RM) ./Drivers/VL53L1/platform/IOI2C.cyclo ./Drivers/VL53L1/platform/IOI2C.d ./Drivers/VL53L1/platform/IOI2C.o ./Drivers/VL53L1/platform/IOI2C.su ./Drivers/VL53L1/platform/vl53l1_platform.cyclo ./Drivers/VL53L1/platform/vl53l1_platform.d ./Drivers/VL53L1/platform/vl53l1_platform.o ./Drivers/VL53L1/platform/vl53l1_platform.su ./Drivers/VL53L1/platform/vl53l1x.cyclo ./Drivers/VL53L1/platform/vl53l1x.d ./Drivers/VL53L1/platform/vl53l1x.o ./Drivers/VL53L1/platform/vl53l1x.su

.PHONY: clean-Drivers-2f-VL53L1-2f-platform

