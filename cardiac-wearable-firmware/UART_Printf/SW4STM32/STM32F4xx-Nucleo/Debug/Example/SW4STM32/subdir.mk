################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
C:/Users/Chase/STM32CubeIDE/workspace_1.8.0/UART_Printf/SW4STM32/startup_stm32f446xx.s 

C_SRCS += \
C:/Users/Chase/STM32CubeIDE/workspace_1.8.0/UART_Printf/SW4STM32/syscalls.c 

OBJS += \
./Example/SW4STM32/startup_stm32f446xx.o \
./Example/SW4STM32/syscalls.o 

S_DEPS += \
./Example/SW4STM32/startup_stm32f446xx.d 

C_DEPS += \
./Example/SW4STM32/syscalls.d 


# Each subdirectory must supply rules for building sources it contributes
Example/SW4STM32/startup_stm32f446xx.o: C:/Users/Chase/STM32CubeIDE/workspace_1.8.0/UART_Printf/SW4STM32/startup_stm32f446xx.s Example/SW4STM32/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Example/SW4STM32/syscalls.o: C:/Users/Chase/STM32CubeIDE/workspace_1.8.0/UART_Printf/SW4STM32/syscalls.c Example/SW4STM32/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DUSE_STM32F4XX_NUCLEO -DUSE_IOEXPANDER -DSTM32F446xx -c -I../../../Inc -I../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../../Drivers/STM32F4xx_HAL_Driver/Inc -I../../../Drivers/BSP/STM32F4xx-Nucleo -I../../../Drivers/CMSIS/Include -Os -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Example-2f-SW4STM32

clean-Example-2f-SW4STM32:
	-$(RM) ./Example/SW4STM32/startup_stm32f446xx.d ./Example/SW4STM32/startup_stm32f446xx.o ./Example/SW4STM32/syscalls.d ./Example/SW4STM32/syscalls.o

.PHONY: clean-Example-2f-SW4STM32

