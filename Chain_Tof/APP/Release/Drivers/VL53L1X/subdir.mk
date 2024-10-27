################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/VL53L1X/VL53L1X.c 

OBJS += \
./Drivers/VL53L1X/VL53L1X.o 

C_DEPS += \
./Drivers/VL53L1X/VL53L1X.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/VL53L1X/%.o Drivers/VL53L1X/%.su Drivers/VL53L1X/%.cyclo: ../Drivers/VL53L1X/%.c Drivers/VL53L1X/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -DSTM32G031xx -c -I"C:/Users/19344/Desktop/chain-main/Chain_Tof/APP/Drivers/VL53L1X" -I../Core/Inc -I"C:/Users/19344/Desktop/chain-main/Chain_Tof/APP/Core/flash" -I"C:/Users/19344/Desktop/chain-main/Chain_Tof/APP/Core/RGB" -I"C:/Users/19344/Desktop/chain-main/Chain_Tof/APP/Core/Chain_Function" -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-VL53L1X

clean-Drivers-2f-VL53L1X:
	-$(RM) ./Drivers/VL53L1X/VL53L1X.cyclo ./Drivers/VL53L1X/VL53L1X.d ./Drivers/VL53L1X/VL53L1X.o ./Drivers/VL53L1X/VL53L1X.su

.PHONY: clean-Drivers-2f-VL53L1X

