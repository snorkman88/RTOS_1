################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../example/src/button_timer.c \
../example/src/cr_startup_lpc43xx.c \
../example/src/fsm_debounce.c \
../example/src/sysinit.c 

OBJS += \
./example/src/button_timer.o \
./example/src/cr_startup_lpc43xx.o \
./example/src/fsm_debounce.o \
./example/src/sysinit.o 

C_DEPS += \
./example/src/button_timer.d \
./example/src/cr_startup_lpc43xx.d \
./example/src/fsm_debounce.d \
./example/src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
example/src/%.o: ../example/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -D__USE_LPCOPEN -D__REDLIB__ -DCORE_M4 -D__MULTICORE_NONE -I"/home/snorkman/LPCXpresso/borra/lpc_chip_43xx/inc" -I"/home/snorkman/LPCXpresso/borra/lpc_board_nxp_lpcxpresso_4337/inc" -I"/home/snorkman/LPCXpresso/borra/RTOS_1_D4/example/inc" -I"/home/snorkman/LPCXpresso/borra/RTOS_1_D4/freertos/inc" -O0 -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fsingle-precision-constant -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


