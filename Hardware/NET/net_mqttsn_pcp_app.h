#ifndef __NET_MQTTSN_PCP_APP_H
#define   __NET_MQTTSN_PCP_APP_H

#include "sys.h"
#include "mqttpcpconfig.h"
#include "mqttpcpupgrade.h"
#include "mqttpcpfunc.h"
#include "mqttpcptransport.h"
#include "mqttpcpcrccheck.h"
#include "mqttpcpsock.h"
#include "nbiotconfig.h"
#include "nbiotfunc.h"
#include "nbiottransport.h"
#include "net_mqttsn_pcp_message_operate.h"
#include "net_mqttsn_message_operate.h"
#include "platform_map.h"

void NET_MqttSN_PCP_APP_PollExecution(MqttSNPCP_ClientsTypeDef* pClient);									//PCP逻辑处理
MqttSNPCP_StatusTypeDef NET_MqttSN_PCP_NBIOT_Event_StopMode(MqttSNPCP_ClientsTypeDef* pClient);				//停止模式
MqttSNPCP_StatusTypeDef NET_MqttSN_PCP_NBIOT_Event_Initialized(MqttSNPCP_ClientsTypeDef* pClient);				//初始化PCP
MqttSNPCP_StatusTypeDef NET_MqttSN_PCP_NBIOT_Event_Ready(MqttSNPCP_ClientsTypeDef* pClient);					//PCP准备中


MqttSNPCP_StatusTypeDef NET_MqttSN_PCP_NBIOT_Event_Execute(MqttSNPCP_ClientsTypeDef* pClient);					//PCP数据处理
MqttSNPCP_StatusTypeDef NET_MqttSN_PCP_NBIOT_Event_ActiveUpload(MqttSNPCP_ClientsTypeDef* pClient);				//PCP主动上传数据

#endif /* __NET_MQTTSN_PCP_APP_H */
