/*
 * MQTTTask.h
 *
 *  Created on: Nov 16, 2022
 *      Author: cuhuy
 */

#ifndef MQTTTASK_H_
#define MQTTTASK_H_

#include "main.h"

#define BROKER_IP   "192.168.10.12"
#define MQTT_PORT   8883
#define MQTT_BUFSIZE  1024

void MqttClientSubTask(void const *argument);   //mqtt client subscribe task function
void MqttClientPubTask(void const *argument);   //mqtt client publish task function



#endif /* MQTTTASK_H_ */
