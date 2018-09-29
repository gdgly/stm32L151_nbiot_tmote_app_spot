#ifndef __MQTT_PCP_TRANSPORT_H
#define   __MQTT_PCP_TRANSPORT_H

#include "sys.h"
#include "mqttpcpconfig.h"

MqttSNPCP_StatusTypeDef MqttPCP_Transport_Write(MqttSNPCP_MqttNetTransportTypeDef* NetMqttSN, const char *buf, u16 sendlen);		//MqttSN发送一条负载数据
MqttSNPCP_StatusTypeDef MqttPCP_Transport_Read(MqttSNPCP_MqttNetTransportTypeDef* NetMqttSN, char *buf, u16* recvlen);			//MqttSN读取一条负载数据

MqttSNPCP_StatusTypeDef MqttPCP_Transport_Init(MqttSNPCP_MqttNetTransportTypeDef* NetMqttSN, MQTTSN_ClientsTypeDef* MqttSNStack);	//初始化MqttSN PCP数据传输接口

#endif /* __MQTT_PCP_TRANSPORT_H */
