################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/MQTTClient/packet/MQTTConnectClient.c \
../Middlewares/Third_Party/MQTTClient/packet/MQTTConnectServer.c \
../Middlewares/Third_Party/MQTTClient/packet/MQTTDeserializePublish.c \
../Middlewares/Third_Party/MQTTClient/packet/MQTTFormat.c \
../Middlewares/Third_Party/MQTTClient/packet/MQTTPacket.c \
../Middlewares/Third_Party/MQTTClient/packet/MQTTSerializePublish.c \
../Middlewares/Third_Party/MQTTClient/packet/MQTTSubscribeClient.c \
../Middlewares/Third_Party/MQTTClient/packet/MQTTSubscribeServer.c \
../Middlewares/Third_Party/MQTTClient/packet/MQTTUnsubscribeClient.c \
../Middlewares/Third_Party/MQTTClient/packet/MQTTUnsubscribeServer.c 

OBJS += \
./Middlewares/Third_Party/MQTTClient/packet/MQTTConnectClient.o \
./Middlewares/Third_Party/MQTTClient/packet/MQTTConnectServer.o \
./Middlewares/Third_Party/MQTTClient/packet/MQTTDeserializePublish.o \
./Middlewares/Third_Party/MQTTClient/packet/MQTTFormat.o \
./Middlewares/Third_Party/MQTTClient/packet/MQTTPacket.o \
./Middlewares/Third_Party/MQTTClient/packet/MQTTSerializePublish.o \
./Middlewares/Third_Party/MQTTClient/packet/MQTTSubscribeClient.o \
./Middlewares/Third_Party/MQTTClient/packet/MQTTSubscribeServer.o \
./Middlewares/Third_Party/MQTTClient/packet/MQTTUnsubscribeClient.o \
./Middlewares/Third_Party/MQTTClient/packet/MQTTUnsubscribeServer.o 

C_DEPS += \
./Middlewares/Third_Party/MQTTClient/packet/MQTTConnectClient.d \
./Middlewares/Third_Party/MQTTClient/packet/MQTTConnectServer.d \
./Middlewares/Third_Party/MQTTClient/packet/MQTTDeserializePublish.d \
./Middlewares/Third_Party/MQTTClient/packet/MQTTFormat.d \
./Middlewares/Third_Party/MQTTClient/packet/MQTTPacket.d \
./Middlewares/Third_Party/MQTTClient/packet/MQTTSerializePublish.d \
./Middlewares/Third_Party/MQTTClient/packet/MQTTSubscribeClient.d \
./Middlewares/Third_Party/MQTTClient/packet/MQTTSubscribeServer.d \
./Middlewares/Third_Party/MQTTClient/packet/MQTTUnsubscribeClient.d \
./Middlewares/Third_Party/MQTTClient/packet/MQTTUnsubscribeServer.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/MQTTClient/packet/%.o Middlewares/Third_Party/MQTTClient/packet/%.su: ../Middlewares/Third_Party/MQTTClient/packet/%.c Middlewares/Third_Party/MQTTClient/packet/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../LWIP/App -I../LWIP/Target -I../MQTT -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/MQTTClient -I../Middlewares/Third_Party/MQTTClient/packet -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Drivers/BSP/Components/lan8742 -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-MQTTClient-2f-packet

clean-Middlewares-2f-Third_Party-2f-MQTTClient-2f-packet:
	-$(RM) ./Middlewares/Third_Party/MQTTClient/packet/MQTTConnectClient.d ./Middlewares/Third_Party/MQTTClient/packet/MQTTConnectClient.o ./Middlewares/Third_Party/MQTTClient/packet/MQTTConnectClient.su ./Middlewares/Third_Party/MQTTClient/packet/MQTTConnectServer.d ./Middlewares/Third_Party/MQTTClient/packet/MQTTConnectServer.o ./Middlewares/Third_Party/MQTTClient/packet/MQTTConnectServer.su ./Middlewares/Third_Party/MQTTClient/packet/MQTTDeserializePublish.d ./Middlewares/Third_Party/MQTTClient/packet/MQTTDeserializePublish.o ./Middlewares/Third_Party/MQTTClient/packet/MQTTDeserializePublish.su ./Middlewares/Third_Party/MQTTClient/packet/MQTTFormat.d ./Middlewares/Third_Party/MQTTClient/packet/MQTTFormat.o ./Middlewares/Third_Party/MQTTClient/packet/MQTTFormat.su ./Middlewares/Third_Party/MQTTClient/packet/MQTTPacket.d ./Middlewares/Third_Party/MQTTClient/packet/MQTTPacket.o ./Middlewares/Third_Party/MQTTClient/packet/MQTTPacket.su ./Middlewares/Third_Party/MQTTClient/packet/MQTTSerializePublish.d ./Middlewares/Third_Party/MQTTClient/packet/MQTTSerializePublish.o ./Middlewares/Third_Party/MQTTClient/packet/MQTTSerializePublish.su ./Middlewares/Third_Party/MQTTClient/packet/MQTTSubscribeClient.d ./Middlewares/Third_Party/MQTTClient/packet/MQTTSubscribeClient.o ./Middlewares/Third_Party/MQTTClient/packet/MQTTSubscribeClient.su ./Middlewares/Third_Party/MQTTClient/packet/MQTTSubscribeServer.d ./Middlewares/Third_Party/MQTTClient/packet/MQTTSubscribeServer.o ./Middlewares/Third_Party/MQTTClient/packet/MQTTSubscribeServer.su ./Middlewares/Third_Party/MQTTClient/packet/MQTTUnsubscribeClient.d ./Middlewares/Third_Party/MQTTClient/packet/MQTTUnsubscribeClient.o ./Middlewares/Third_Party/MQTTClient/packet/MQTTUnsubscribeClient.su ./Middlewares/Third_Party/MQTTClient/packet/MQTTUnsubscribeServer.d ./Middlewares/Third_Party/MQTTClient/packet/MQTTUnsubscribeServer.o ./Middlewares/Third_Party/MQTTClient/packet/MQTTUnsubscribeServer.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-MQTTClient-2f-packet

