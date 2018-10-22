/**
  *********************************************************************************************************
  * @file    onenettransport.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-08-31
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "onenettransport.h"
#include "platform_config.h"
#include "platform_map.h"
#include "stm32l1xx_config.h"
#include "nbiotfunc.h"
#include "string.h"

/**********************************************************************************************************
 @Function			ONENET_StatusTypeDef ONENET_Transport_Write(ONENET_ClientsTypeDef* pClient, ONENET_ObserveParaTypeDef observeInfo, const char *buf, u16 sendlen, u16 ackid)
 @Description			ONENET_Transport_Write	: OneNET连接发送一条负载数据
 @Input				pClient				: OneNET客户端实例
					observeInfo			: OneNET实例消息
					buf					: 负载数据
					sendlen 				: 数据长度
					ackid				: The Ack ID
 @Return				MQTTSN_StatusTypeDef	: MQTTSN处理状态
**********************************************************************************************************/
ONENET_StatusTypeDef ONENET_Transport_Write(ONENET_ClientsTypeDef* pClient, ONENET_ObserveParaTypeDef observeInfo, const char *buf, u16 sendlen, u16 ackid)
{
	ONENET_StatusTypeDef OneNETStatus = ONENET_OK;
	ONENET_MessageParaTypeDef msg;
	
	memset((void *)pClient->DataProcessStack, 0x0, pClient->DataProcessStack_size);
	for (int i = 0; i < sendlen; i++) {
		sprintf((char *)(pClient->DataProcessStack + i * 2), "%02X", buf[i]);
	}
	
	msg.objId				= ONENET_OBJECT_OBJID;
	msg.insId				= observeInfo.insId;
	msg.resId				= ONENET_DISCOVER_VALUE;
	msg.valueType			= ONENET_Opaque;
	msg.len				= sendlen;
	msg.value				= (sc8*)pClient->DataProcessStack;
	msg.index				= 0;
	msg.flag				= 0;
	
	OneNETStatus = NBIOT_OneNET_Related_Notify_ApplicationServer(pClient, pClient->Parameter.suiteRefer, observeInfo.msgId, &msg, ackid, NULL);
	
	return OneNETStatus;
}
















/**********************************************************************************************************
 @Function			ONENET_StatusTypeDef ONENET_Transport_Init(ONENET_LWM2MTransportTypeDef* NetLWM2M, NBIOT_ClientsTypeDef* NBIotStack)
 @Description			ONENET_Transport_Init	: Initiative初始化ONENET数据传输接口
 @Input				NetLWM2M				: LWM2M Struct*
					NBIotStack			: NBIot协议栈
 @Return				ONENET_StatusTypeDef	: ONENET处理状态
**********************************************************************************************************/
ONENET_StatusTypeDef ONENET_Transport_Init(ONENET_LWM2MTransportTypeDef* NetLWM2M, NBIOT_ClientsTypeDef* NBIotStack)
{
	ONENET_StatusTypeDef OneNETStatus = ONENET_OK;
	
	NetLWM2M->NBIotStack = NBIotStack;
	NetLWM2M->Write = ONENET_Transport_Write;
	
	
	return OneNETStatus;
}

/********************************************** END OF FLEE **********************************************/
