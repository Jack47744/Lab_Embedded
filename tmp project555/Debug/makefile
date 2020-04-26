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
tmp\ project555.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
tmp\ project555.list \

OBJCOPY_BIN += \
tmp\ project555.bin \


# All Target
all: tmp\ project555.elf secondary-outputs

# Tool invocations
tmp\ project555.elf: $(OBJS) $(USER_OBJS) /Users/metis_sotangkur/STM32CubeIDE/workspace_1.2.0/tmp\ project555/STM32F411RETX_FLASH.ld
	arm-none-eabi-gcc -o "tmp project555.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"/Users/metis_sotangkur/STM32CubeIDE/workspace_1.2.0/tmp project555/STM32F411RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="tmp project555.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

tmp\ project555.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "tmp project555.list"
	@echo 'Finished building: $@'
	@echo ' '

tmp\ project555.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "tmp project555.bin"
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
