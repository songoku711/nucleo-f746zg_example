################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/VS1053/VS1053.c \
../Middlewares/Third_Party/VS1053/vs1053b-patches-flac.c 

OBJS += \
./Middlewares/Third_Party/VS1053/VS1053.o \
./Middlewares/Third_Party/VS1053/vs1053b-patches-flac.o 

C_DEPS += \
./Middlewares/Third_Party/VS1053/VS1053.d \
./Middlewares/Third_Party/VS1053/vs1053b-patches-flac.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/VS1053/%.o Middlewares/Third_Party/VS1053/%.su: ../Middlewares/Third_Party/VS1053/%.c Middlewares/Third_Party/VS1053/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DVS1053_SUPPORT_FLAC -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../MP3_Player -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/Third_Party/VS1053 -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-VS1053

clean-Middlewares-2f-Third_Party-2f-VS1053:
	-$(RM) ./Middlewares/Third_Party/VS1053/VS1053.d ./Middlewares/Third_Party/VS1053/VS1053.o ./Middlewares/Third_Party/VS1053/VS1053.su ./Middlewares/Third_Party/VS1053/vs1053b-patches-flac.d ./Middlewares/Third_Party/VS1053/vs1053b-patches-flac.o ./Middlewares/Third_Party/VS1053/vs1053b-patches-flac.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-VS1053

