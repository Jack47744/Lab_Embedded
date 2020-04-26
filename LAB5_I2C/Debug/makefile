################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include USB_HOST/Target/subdir.mk
-include USB_HOST/App/subdir.mk
-include Middlewares/ST/STM32_USB_Host_Library/Core/Src/subdir.mk
-include Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Src/subdir.mk
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
LAB5_I2C.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
LAB5_I2C.list \

OBJCOPY_BIN += \
LAB5_I2C.bin \


# All Target
all: LAB5_I2C.elf secondary-outputs

# Tool invocations
LAB5_I2C.elf: $(OBJS) $(USER_OBJS) /Users/metis_sotangkur/STM32CubeIDE/workspace_1.2.0/LAB5_I2C/STM32F407VGTX_FLASH.ld
	arm-none-eabi-gcc -o "LAB5_I2C.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"/Users/metis_sotangkur/STM32CubeIDE/workspace_1.2.0/LAB5_I2C/STM32F407VGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="LAB5_I2C.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

LAB5_I2C.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "LAB5_I2C.list"
	@echo 'Finished building: $@'
	@echo ' '

LAB5_I2C.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "LAB5_I2C.bin"
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
