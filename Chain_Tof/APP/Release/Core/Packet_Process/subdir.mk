################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Packet_Process/packet.c 

OBJS += \
./Core/Packet_Process/packet.o 

C_DEPS += \
./Core/Packet_Process/packet.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Packet_Process/%.o Core/Packet_Process/%.su Core/Packet_Process/%.cyclo: ../Core/Packet_Process/%.c Core/Packet_Process/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -DSTM32G031xx -c -I../Core/Inc -I"C:/Users/15191/STM32CubeIDE/workspace_1.16.0/Chain/Core/Packet_Process" -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Packet_Process

clean-Core-2f-Packet_Process:
	-$(RM) ./Core/Packet_Process/packet.cyclo ./Core/Packet_Process/packet.d ./Core/Packet_Process/packet.o ./Core/Packet_Process/packet.su

.PHONY: clean-Core-2f-Packet_Process

