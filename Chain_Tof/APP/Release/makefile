################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/VL53L0X/Src/subdir.mk
-include Drivers/STM32G0xx_HAL_Driver/Src/subdir.mk
-include Core/flash/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include Core/RGB/subdir.mk
-include Core/Chain_Function/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Chain_Tof
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Chain_Tof.elf \

MAP_FILES += \
Chain_Tof.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Chain_Tof.list \

OBJCOPY_HEX += \
Chain_Tof.hex \

OBJCOPY_BIN += \
Chain_Tof.bin \


# All Target
all: main-build

# Main-build Target
main-build: Chain_Tof.elf secondary-outputs

# Tool invocations
Chain_Tof.elf Chain_Tof.map: $(OBJS) $(USER_OBJS) C:\Users\15191\Desktop\chain\Chain_Tof\APP\STM32G031G8UX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "Chain_Tof.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m0plus -T"C:\Users\15191\Desktop\chain\Chain_Tof\APP\STM32G031G8UX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Chain_Tof.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Chain_Tof.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Chain_Tof.list"
	@echo 'Finished building: $@'
	@echo ' '

Chain_Tof.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "Chain_Tof.hex"
	@echo 'Finished building: $@'
	@echo ' '

Chain_Tof.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Chain_Tof.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Chain_Tof.bin Chain_Tof.elf Chain_Tof.hex Chain_Tof.list Chain_Tof.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
