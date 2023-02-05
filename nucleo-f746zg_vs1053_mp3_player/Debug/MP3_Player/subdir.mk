################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MP3_Player/MP3_Player.c 

OBJS += \
./MP3_Player/MP3_Player.o 

C_DEPS += \
./MP3_Player/MP3_Player.d 


# Each subdirectory must supply rules for building sources it contributes
MP3_Player/%.o MP3_Player/%.su: ../MP3_Player/%.c MP3_Player/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DVS1053_SUPPORT_FLAC -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../MP3_Player -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/Third_Party/VS1053 -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-MP3_Player

clean-MP3_Player:
	-$(RM) ./MP3_Player/MP3_Player.d ./MP3_Player/MP3_Player.o ./MP3_Player/MP3_Player.su

.PHONY: clean-MP3_Player

