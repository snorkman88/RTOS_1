################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include freertos/src/subdir.mk
-include example/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: RTOS_1_D2.elf

# Tool invocations
RTOS_1_D2.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"/home/snorkman/LPCXpresso/borra/lpc_chip_43xx/Debug" -L"/home/snorkman/LPCXpresso/borra/lpc_board_nxp_lpcxpresso_4337/Debug" -Xlinker -Map="RTOS_1_D2.map" -Xlinker --gc-sections -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -T "RTOS_1_A4_con_taskYIELD_Debug.ld" -o "RTOS_1_D2.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) RTOS_1_D2.elf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "RTOS_1_D2.elf";  arm-none-eabi-objcopy -v -O binary "RTOS_1_D2.elf" "RTOS_1_D2.bin" ; checksum -p LPC4337 -d "RTOS_1_D2.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
