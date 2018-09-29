/**
  *********************************************************************************************************
  * @file    mqttpcptransport.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-09-26
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "mqttpcptransport.h"
#include "platform_config.h"
#include "platform_map.h"
#include "stm32l1xx_config.h"
#include "delay.h"
#include "usart.h"

/**********************************************************************************************************
 @Function			MqttSNPCP_StatusTypeDef MqttPCP_Transport_Write(MqttSNPCP_ClientsTypeDef* pClient, MqttSNPCP_MqttNetTransportTypeDef* NetMqttSN, const char *buf, u16 sendlen)
 @Description			MqttPCP_Transport_Write		: MqttSN发送一条负载数据
 @Input				NetMqttSN					: MqttSNNet Struct*
					buf						: 负载数据
					sendlen 					: 数据长度
 @Return				MqttSNPCP_StatusTypeDef		: MqttSN PCP处理状态
**********************************************************************************************************/
MqttSNPCP_StatusTypeDef MqttPCP_Transport_Write(MqttSNPCP_MqttNetTransportTypeDef* NetMqttSN, const char *buf, u16 sendlen)
{
	MqttSNPCP_StatusTypeDef PCPStatus = MQTTSN_PCP_OK;
	
	unsigned char* pMsg = NetMqttSN->MqttSNStack->DataProcessStack;
	unsigned short pMsgLen = 0;
	unsigned int macsn = 0;
	
	MQTTSN_MessageTypeDef message;
	MQTTSN_topicid topic;
	
	memset(NetMqttSN->MqttSNStack->DataProcessStack, 0x0, NetMqttSN->MqttSNStack->DataProcessStack_size);
	
	macsn = TCFG_EEPROM_Get_MAC_SN();
	pMsg[0] = (macsn & 0xFF000000) >> 3*8;
	pMsg[1] = (macsn & 0x00FF0000) >> 2*8;
	pMsg[2] = (macsn & 0x0000FF00) >> 1*8;
	pMsg[3] = (macsn & 0x000000FF) >> 0*8;
	pMsgLen += 4;
	
	memcpy(pMsg + pMsgLen, buf, sendlen);
	pMsgLen += sendlen;
	
	message.qos		= QOS1;
	message.msgid		= 1;
	message.dup		= 0;
	message.retained	= 0;
	message.payload	= pMsg;
	message.payloadlen	= pMsgLen;
	
	topic.type		= MQTTSN_TOPIC_TYPE_PREDEFINED;
	topic.data.id		= TOPICID_UPGRADE;
	
	if (MQTTSN_Publish(NetMqttSN->MqttSNStack, topic, &message) != MQTTSN_OK) {
		PCPStatus = MQTTSN_PCP_ERROR;
	}
	
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			MqttSNPCP_StatusTypeDef MqttPCP_Transport_Read(MqttSNPCP_ClientsTypeDef* pClient, MqttSNPCP_MqttNetTransportTypeDef* NetMqttSN, char *buf, u16* recvlen)
 @Description									: MqttSN读取一条负载数据
 @Input				NetMqttSN					: MqttSNNet Struct*
					buf						: 读取数据缓存
					recvlen					: 读取数据长度地址
 @Return				MqttSNPCP_StatusTypeDef		: MqttSN PCP处理状态
**********************************************************************************************************/
MqttSNPCP_StatusTypeDef MqttPCP_Transport_Read(MqttSNPCP_MqttNetTransportTypeDef* NetMqttSN, char *buf, u16* recvlen)
{
	MqttSNPCP_StatusTypeDef PCPStatus = MQTTSN_PCP_OK;
	
	MQTTSN_RecvAck(NetMqttSN->MqttSNStack);
	
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			MqttSNPCP_StatusTypeDef MqttPCP_Transport_Init(MqttSNPCP_MqttNetTransportTypeDef* NetMqttSN, MQTTSN_ClientsTypeDef* MqttSNStack)
 @Description			MqttPCP_Transport_Init		: Initiative初始化MqttSN PCP数据传输接口
 @Input				NetMqttSN					: MqttSNNet Struct*
					NBIotStack				: MqttSN协议栈
 @Return				MqttSNPCP_StatusTypeDef		: MqttSN PCP处理状态
**********************************************************************************************************/
MqttSNPCP_StatusTypeDef MqttPCP_Transport_Init(MqttSNPCP_MqttNetTransportTypeDef* NetMqttSN, MQTTSN_ClientsTypeDef* MqttSNStack)
{
	MqttSNPCP_StatusTypeDef PCPStatus = MQTTSN_PCP_OK;
	
	NetMqttSN->MqttSNStack = MqttSNStack;
	NetMqttSN->Write = MqttPCP_Transport_Write;
	NetMqttSN->Read = MqttPCP_Transport_Read;
	
	return PCPStatus;
}

/********************************************** END OF FLEE **********************************************/
