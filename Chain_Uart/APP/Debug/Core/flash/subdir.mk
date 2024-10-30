################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/flash/myflash.c 

OBJS += \
./Core/flash/myflash.o 

C_DEPS += \
./Core/flash/myflash.d 


# Each subdirectory must supply rules for building sources it contributes
Core/flash/%.o Core/flash/%.su Core/flash/%.cyclo: ../Core/flash/%.c Core/flash/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -DSTM32G031xx -c -I../Core/Inc -I"C:/Users/15191/Desktop/Chain_Uart/APP/Core/RGB" -I"C:/Users/15191/Desktop/Chain_Uart/APP/Core/flash" -I"C:/Users/15191/Desktop/Chain_Uart/APP/Core/Chain_Function" -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-flash

clean-Core-2f-flash:
	-$(RM) ./Core/flash/myflash.cyclo ./Core/flash/myflash.d ./Core/flash/myflash.o ./Core/flash/myflash.su

.PHONY: clean-Core-2f-flash

