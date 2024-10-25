################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/VL53L1/core/vl53l1_api.c \
../Drivers/VL53L1/core/vl53l1_api_calibration.c \
../Drivers/VL53L1/core/vl53l1_api_core.c \
../Drivers/VL53L1/core/vl53l1_api_debug.c \
../Drivers/VL53L1/core/vl53l1_api_preset_modes.c \
../Drivers/VL53L1/core/vl53l1_api_strings.c \
../Drivers/VL53L1/core/vl53l1_core.c \
../Drivers/VL53L1/core/vl53l1_core_support.c \
../Drivers/VL53L1/core/vl53l1_error_strings.c \
../Drivers/VL53L1/core/vl53l1_register_funcs.c \
../Drivers/VL53L1/core/vl53l1_silicon_core.c \
../Drivers/VL53L1/core/vl53l1_wait.c 

OBJS += \
./Drivers/VL53L1/core/vl53l1_api.o \
./Drivers/VL53L1/core/vl53l1_api_calibration.o \
./Drivers/VL53L1/core/vl53l1_api_core.o \
./Drivers/VL53L1/core/vl53l1_api_debug.o \
./Drivers/VL53L1/core/vl53l1_api_preset_modes.o \
./Drivers/VL53L1/core/vl53l1_api_strings.o \
./Drivers/VL53L1/core/vl53l1_core.o \
./Drivers/VL53L1/core/vl53l1_core_support.o \
./Drivers/VL53L1/core/vl53l1_error_strings.o \
./Drivers/VL53L1/core/vl53l1_register_funcs.o \
./Drivers/VL53L1/core/vl53l1_silicon_core.o \
./Drivers/VL53L1/core/vl53l1_wait.o 

C_DEPS += \
./Drivers/VL53L1/core/vl53l1_api.d \
./Drivers/VL53L1/core/vl53l1_api_calibration.d \
./Drivers/VL53L1/core/vl53l1_api_core.d \
./Drivers/VL53L1/core/vl53l1_api_debug.d \
./Drivers/VL53L1/core/vl53l1_api_preset_modes.d \
./Drivers/VL53L1/core/vl53l1_api_strings.d \
./Drivers/VL53L1/core/vl53l1_core.d \
./Drivers/VL53L1/core/vl53l1_core_support.d \
./Drivers/VL53L1/core/vl53l1_error_strings.d \
./Drivers/VL53L1/core/vl53l1_register_funcs.d \
./Drivers/VL53L1/core/vl53l1_silicon_core.d \
./Drivers/VL53L1/core/vl53l1_wait.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/VL53L1/core/%.o Drivers/VL53L1/core/%.su Drivers/VL53L1/core/%.cyclo: ../Drivers/VL53L1/core/%.c Drivers/VL53L1/core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -DSTM32G031xx -c -I"C:/Users/15191/Desktop/chain/Chain_Tof/APP/Drivers/VL53L1/core" -I"C:/Users/15191/Desktop/chain/Chain_Tof/APP/Drivers/VL53L1/platform" -I../Core/Inc -I"C:/Users/15191/Desktop/chain/Chain_Tof/APP/Core/flash" -I"C:/Users/15191/Desktop/chain/Chain_Tof/APP/Core/RGB" -I"C:/Users/15191/Desktop/chain/Chain_Tof/APP/Core/Chain_Function" -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-VL53L1-2f-core

clean-Drivers-2f-VL53L1-2f-core:
	-$(RM) ./Drivers/VL53L1/core/vl53l1_api.cyclo ./Drivers/VL53L1/core/vl53l1_api.d ./Drivers/VL53L1/core/vl53l1_api.o ./Drivers/VL53L1/core/vl53l1_api.su ./Drivers/VL53L1/core/vl53l1_api_calibration.cyclo ./Drivers/VL53L1/core/vl53l1_api_calibration.d ./Drivers/VL53L1/core/vl53l1_api_calibration.o ./Drivers/VL53L1/core/vl53l1_api_calibration.su ./Drivers/VL53L1/core/vl53l1_api_core.cyclo ./Drivers/VL53L1/core/vl53l1_api_core.d ./Drivers/VL53L1/core/vl53l1_api_core.o ./Drivers/VL53L1/core/vl53l1_api_core.su ./Drivers/VL53L1/core/vl53l1_api_debug.cyclo ./Drivers/VL53L1/core/vl53l1_api_debug.d ./Drivers/VL53L1/core/vl53l1_api_debug.o ./Drivers/VL53L1/core/vl53l1_api_debug.su ./Drivers/VL53L1/core/vl53l1_api_preset_modes.cyclo ./Drivers/VL53L1/core/vl53l1_api_preset_modes.d ./Drivers/VL53L1/core/vl53l1_api_preset_modes.o ./Drivers/VL53L1/core/vl53l1_api_preset_modes.su ./Drivers/VL53L1/core/vl53l1_api_strings.cyclo ./Drivers/VL53L1/core/vl53l1_api_strings.d ./Drivers/VL53L1/core/vl53l1_api_strings.o ./Drivers/VL53L1/core/vl53l1_api_strings.su ./Drivers/VL53L1/core/vl53l1_core.cyclo ./Drivers/VL53L1/core/vl53l1_core.d ./Drivers/VL53L1/core/vl53l1_core.o ./Drivers/VL53L1/core/vl53l1_core.su ./Drivers/VL53L1/core/vl53l1_core_support.cyclo ./Drivers/VL53L1/core/vl53l1_core_support.d ./Drivers/VL53L1/core/vl53l1_core_support.o ./Drivers/VL53L1/core/vl53l1_core_support.su ./Drivers/VL53L1/core/vl53l1_error_strings.cyclo ./Drivers/VL53L1/core/vl53l1_error_strings.d ./Drivers/VL53L1/core/vl53l1_error_strings.o ./Drivers/VL53L1/core/vl53l1_error_strings.su ./Drivers/VL53L1/core/vl53l1_register_funcs.cyclo ./Drivers/VL53L1/core/vl53l1_register_funcs.d ./Drivers/VL53L1/core/vl53l1_register_funcs.o ./Drivers/VL53L1/core/vl53l1_register_funcs.su ./Drivers/VL53L1/core/vl53l1_silicon_core.cyclo ./Drivers/VL53L1/core/vl53l1_silicon_core.d ./Drivers/VL53L1/core/vl53l1_silicon_core.o ./Drivers/VL53L1/core/vl53l1_silicon_core.su ./Drivers/VL53L1/core/vl53l1_wait.cyclo ./Drivers/VL53L1/core/vl53l1_wait.d ./Drivers/VL53L1/core/vl53l1_wait.o ./Drivers/VL53L1/core/vl53l1_wait.su

.PHONY: clean-Drivers-2f-VL53L1-2f-core

