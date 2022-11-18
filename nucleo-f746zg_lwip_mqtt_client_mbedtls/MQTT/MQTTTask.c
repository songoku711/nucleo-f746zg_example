/*
 * MQTTTask.c
 *
 *  Created on: Nov 16, 2022
 *      Author: cuhuy
 */

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include <string.h>
#include "lwip.h"
#include "lwip/api.h"
#include "MQTTClient.h"
#include "MQTTInterface.h"
#include "MQTTTask.h"

osThreadId mqttClientSubTaskHandle;  //mqtt client task handle
osThreadId mqttClientPubTaskHandle;  //mqtt client task handle

Network net; //mqtt network
MQTTClient mqttClient; //mqtt client

uint8_t sndBuffer[MQTT_BUFSIZE]; //mqtt send buffer
uint8_t rcvBuffer[MQTT_BUFSIZE]; //mqtt receive buffer
uint8_t msgBuffer[MQTT_BUFSIZE]; //mqtt message buffer

static int  MqttConnectBroker(void);                   //mqtt broker connect function
static void MqttMessageArrived(MessageData* msg);      //mqtt message callback function

static int MqttConnectBroker()
{
  int ret;

  net_clear();
  ret = net_init(&net);
  if(ret != MQTT_SUCCESS)
  {
    printf("net_init failed.\n");
    return -1;
  }

  ret = net_connect(&net, BROKER_IP, MQTT_PORT);
  if(ret != MQTT_SUCCESS)
  {
    printf("net_connect failed.\n");
    return -1;
  }

  MQTTClientInit(&mqttClient, &net, 1000, sndBuffer, sizeof(sndBuffer), rcvBuffer, sizeof(rcvBuffer));

  MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
  data.willFlag = 0;
  data.MQTTVersion = 3;
  data.clientID.cstring = "STM32F7";
  data.username.cstring = "STM32F7";
  data.password.cstring = "";
  data.keepAliveInterval = 60;
  data.cleansession = 1;

  ret = MQTTConnect(&mqttClient, &data);
  if(ret != MQTT_SUCCESS)
  {
    printf("MQTTConnect failed.\n");
    return ret;
  }

  ret = MQTTSubscribe(&mqttClient, "test", QOS0, MqttMessageArrived);
  if(ret != MQTT_SUCCESS)
  {
    printf("MQTTSubscribe failed.\n");
    return ret;
  }

  printf("MqttConnectBroker O.K.\n");
  return MQTT_SUCCESS;
}

static void MqttMessageArrived(MessageData* msg)
{
  HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin); //toggle pin when new message arrived

  MQTTMessage* message = msg->message;
  memset(msgBuffer, 0, sizeof(msgBuffer));
  memcpy(msgBuffer, message->payload,message->payloadlen);

  printf("MQTT MSG[%d]:%s\n", (int)message->payloadlen, msgBuffer);
}



void MqttClientSubTask(void const *argument)
{
  while(1)
  {
    if(!mqttClient.isconnected)
    {
      //try to connect to the broker
      MQTTDisconnect(&mqttClient);
      MqttConnectBroker();
      osDelay(1000);
    }
    else
    {
      MQTTYield(&mqttClient, 1000); //handle timer
      osDelay(100);
    }
  }
}

void MqttClientPubTask(void const *argument)
{
  const char* str = "MQTT message from STM32";
  MQTTMessage message;

  while(1)
  {
    if (mqttClient.isconnected)
    {
      message.payload = (void*)str;
      message.payloadlen = strlen(str);

      MQTTPublish(&mqttClient, "test", &message); //publish a message
    }

    osDelay(1000);
  }
}
