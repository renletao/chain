################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Chain_Function/base_function.c \
../Core/Chain_Function/uart_function.c 

OBJS += \
./Core/Chain_Function/base_function.o \
./Core/Chain_Function/uart_function.o 

C_DEPS += \
./Core/Chain_Function/base_function.d \
./Core/Chain_Function/uart_function.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Chain_Function/%.o Core/Chain_Function/%.su Core/Chain_Function/%.cyclo: ../Core/Chain_Function/%.c Core/Chain_Function/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -DSTM32G031xx -c -I../Core/Inc -I"C:/Users/15191/Desktop/chainV5/Chain_Uart/APP/Core/RGB" -I"C:/Users/15191/Desktop/chainV5/Chain_Uart/APP/Core/flash" -I"C:/Users/15191/Desktop/chainV5/Chain_Uart/APP/Core/Chain_Function" -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Chain_Function

clean-Core-2f-Chain_Function:
	-$(RM) ./Core/Chain_Function/base_function.cyclo ./Core/Chain_Function/base_function.d ./Core/Chain_Function/base_function.o ./Core/Chain_Function/base_function.su ./Core/Chain_Function/uart_function.cyclo ./Core/Chain_Function/uart_function.d ./Core/Chain_Function/uart_function.o ./Core/Chain_Function/uart_function.su

.PHONY: clean-Core-2f-Chain_Function

