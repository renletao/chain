################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Chain_Base_Function/packet.c 

OBJS += \
./Core/Chain_Base_Function/packet.o 

C_DEPS += \
./Core/Chain_Base_Function/packet.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Chain_Base_Function/%.o Core/Chain_Base_Function/%.su Core/Chain_Base_Function/%.cyclo: ../Core/Chain_Base_Function/%.c Core/Chain_Base_Function/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -DSTM32G031xx -c -I"C:/Users/15191/STM32CubeIDE/workspace_1.16.0/Chain/Core/Chain_Base_Function" -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Chain_Base_Function

clean-Core-2f-Chain_Base_Function:
	-$(RM) ./Core/Chain_Base_Function/packet.cyclo ./Core/Chain_Base_Function/packet.d ./Core/Chain_Base_Function/packet.o ./Core/Chain_Base_Function/packet.su

.PHONY: clean-Core-2f-Chain_Base_Function

