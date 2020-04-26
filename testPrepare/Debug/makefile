################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
testPrepare.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
testPrepare.list \

OBJCOPY_BIN += \
testPrepare.bin \


# All Target
all: testPrepare.elf secondary-outputs

# Tool invocations
testPrepare.elf: $(OBJS) $(USER_OBJS) /Users/metis_sotangkur/STM32CubeIDE/workspace_1.2.0/testPrepare/STM32F411RETX_FLASH.ld
	arm-none-eabi-gcc -o "testPrepare.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"/Users/metis_sotangkur/STM32CubeIDE/workspace_1.2.0/testPrepare/STM32F411RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="testPrepare.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

testPrepare.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "testPrepare.list"
	@echo 'Finished building: $@'
	@echo ' '

testPrepare.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "testPrepare.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
