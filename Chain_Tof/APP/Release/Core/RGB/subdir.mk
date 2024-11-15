################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/RGB/RGB.c 

OBJS += \
./Core/RGB/RGB.o 

C_DEPS += \
./Core/RGB/RGB.d 


# Each subdirectory must supply rules for building sources it contributes
Core/RGB/%.o Core/RGB/%.su Core/RGB/%.cyclo: ../Core/RGB/%.c Core/RGB/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -DSTM32G031xx -c -I"C:/Users/15191/Desktop/chainV5/Chain_Tof/APP/Drivers/VL53L1X" -I../Core/Inc -I"C:/Users/15191/Desktop/chainV5/Chain_Tof/APP/Core/flash" -I"C:/Users/15191/Desktop/chainV5/Chain_Tof/APP/Core/RGB" -I"C:/Users/15191/Desktop/chainV5/Chain_Tof/APP/Core/Chain_Function" -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-RGB

clean-Core-2f-RGB:
	-$(RM) ./Core/RGB/RGB.cyclo ./Core/RGB/RGB.d ./Core/RGB/RGB.o ./Core/RGB/RGB.su

.PHONY: clean-Core-2f-RGB

