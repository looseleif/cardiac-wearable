################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Chase/STM32CubeIDE/workspace_1.8.0/UART_Printf/Drivers/BSP/STM32F4xx-Nucleo/stm32f4xx_nucleo.c 

OBJS += \
./Drivers/BSP/STM32F4xx-Nucleo/stm32f4xx_nucleo.o 

C_DEPS += \
./Drivers/BSP/STM32F4xx-Nucleo/stm32f4xx_nucleo.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/STM32F4xx-Nucleo/stm32f4xx_nucleo.o: C:/Users/Chase/STM32CubeIDE/workspace_1.8.0/UART_Printf/Drivers/BSP/STM32F4xx-Nucleo/stm32f4xx_nucleo.c Drivers/BSP/STM32F4xx-Nucleo/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DUSE_STM32F4XX_NUCLEO -DUSE_IOEXPANDER -DSTM32F446xx -c -I../../../Inc -I../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../../Drivers/STM32F4xx_HAL_Driver/Inc -I../../../Drivers/BSP/STM32F4xx-Nucleo -I../../../Drivers/CMSIS/Include -Os -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-STM32F4xx-2d-Nucleo

clean-Drivers-2f-BSP-2f-STM32F4xx-2d-Nucleo:
	-$(RM) ./Drivers/BSP/STM32F4xx-Nucleo/stm32f4xx_nucleo.d ./Drivers/BSP/STM32F4xx-Nucleo/stm32f4xx_nucleo.o

.PHONY: clean-Drivers-2f-BSP-2f-STM32F4xx-2d-Nucleo

