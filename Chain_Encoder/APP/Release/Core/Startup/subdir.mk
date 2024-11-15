################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32g031g8ux.s 

OBJS += \
./Core/Startup/startup_stm32g031g8ux.o 

S_DEPS += \
./Core/Startup/startup_stm32g031g8ux.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m0plus -c -I"C:/Users/15191/Desktop/chainV5/Chain_Encoder/APP/Core/Chain_Function" -I"C:/Users/15191/Desktop/chainV5/Chain_Encoder/APP/Core/flash" -I"C:/Users/15191/Desktop/chainV5/Chain_Encoder/APP/Core/RGB" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32g031g8ux.d ./Core/Startup/startup_stm32g031g8ux.o

.PHONY: clean-Core-2f-Startup

