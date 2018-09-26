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
 @Function			
 @Description									: MqttSN发送一条负载数据
 @Input										: 
					buf						: 负载数据
					sendlen 					: 数据长度
 @Return										: 
**********************************************************************************************************/




/**********************************************************************************************************
 @Function			
 @Description									: MqttSN读取一条负载数据
 @Input										: 
					buf						: 读取数据缓存
					recvlen					: 读取数据长度地址
 @Return										: 
**********************************************************************************************************/






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
	
	
	return PCPStatus;
}

/********************************************** END OF FLEE **********************************************/
