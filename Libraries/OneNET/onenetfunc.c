/**
  *********************************************************************************************************
  * @file    onenetfunc.c
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

#include "onenetfunc.h"
#include "platform_config.h"
#include "platform_map.h"
#include "stm32l1xx_config.h"
#include "delay.h"
#include "usart.h"
#include "string.h"

/**********************************************************************************************************
 @Function			static void NBIOT_OneNET_Related_DictateEvent_SetTime(ONENET_ClientsTypeDef* pClient, unsigned int TimeoutMsec)
 @Description			NBIOT_OneNET_Related_DictateEvent_SetTime		: 事件运行控制器注入时间(内部使用)
 @Input				pClient									: ONENET客户端实例
					TimeoutMsec								: 注入超时时间
 @Return				void
**********************************************************************************************************/
static void NBIOT_OneNET_Related_DictateEvent_SetTime(ONENET_ClientsTypeDef* pClient, unsigned int TimeoutMsec)
{
	Stm32_CalculagraphTypeDef ATCmd_timer_Ms;
	
	Stm32_Calculagraph_CountdownMS(&ATCmd_timer_Ms, TimeoutMsec);
	pClient->LWM2MStack->NBIotStack->ATCmdStack->CmdWaitTime = ATCmd_timer_Ms;
}

/**********************************************************************************************************
 @Function			static void NBIOT_OneNET_Related_ATCmd_SetCmdStack(ONENET_ClientsTypeDef* pClient, ...)
 @Description			NBIOT_OneNET_Related_ATCmd_SetCmdStack			: AT指令设置(内部使用)
 @Input				pClient									: ONENET客户端实例
					...
 @Return				void
**********************************************************************************************************/
static void NBIOT_OneNET_Related_ATCmd_SetCmdStack(ONENET_ClientsTypeDef* pClient, unsigned char* Sendbuf, unsigned int Sendlen, char* ATack, char* ATNack)
{
	memcpy(pClient->LWM2MStack->NBIotStack->ATCmdStack->ATSendbuf, Sendbuf, Sendlen);
	pClient->LWM2MStack->NBIotStack->ATCmdStack->ATSendlen = Sendlen;
	pClient->LWM2MStack->NBIotStack->ATCmdStack->ATack = ATack;
	pClient->LWM2MStack->NBIotStack->ATCmdStack->ATNack = ATNack;
}

#if NBIOT_PRINT_ERROR_CODE_TYPE
/**********************************************************************************************************
 @Function			static ONENET_StatusTypeDef NBIOT_OneNET_Related_DictateEvent_GetError(ONENET_ClientsTypeDef* pClient)
 @Description			NBIOT_OneNET_Related_DictateEvent_GetError		: 事件运行控制器获取错误码(内部使用)
 @Input				pClient									: ONENET客户端实例
 @Return				ONENET_StatusTypeDef						: ONENET处理状态
**********************************************************************************************************/
static ONENET_StatusTypeDef NBIOT_OneNET_Related_DictateEvent_GetError(ONENET_ClientsTypeDef* pClient)
{
	ONENET_StatusTypeDef ONStatus = ONENET_OK;
	int retErrorCode;
	
	if (sscanf((const char*)pClient->LWM2MStack->NBIotStack->ATCmdStack->ATRecvbuf, "\r\n+CME ERROR: %d\r\n", &retErrorCode) > 0) {
		ONStatus = (ONENET_StatusTypeDef)retErrorCode;
	}
	
	return ONStatus;
}
#endif

/**********************************************************************************************************
 @Function			ONENET_StatusTypeDef NBIOT_OneNET_Related_AccessConfig_LwM2MServer(ONENET_ClientsTypeDef* pClient, u8 mode, sc8* host, u16 port)
 @Description			NBIOT_OneNET_Related_AccessConfig_LwM2MServer	: 设置LwM2M服务器
 @Input				pClient									: ONENET客户端实例
					mode										: Option Mode < Only 0 or 1 >
					host										: LwM2M Server Address
					port										: LwM2M Server Port
 @Return				ONENET_StatusTypeDef						: ONENET处理状态
 @attention			Parameters will not be saved
**********************************************************************************************************/
ONENET_StatusTypeDef NBIOT_OneNET_Related_AccessConfig_LwM2MServer(ONENET_ClientsTypeDef* pClient, u8 mode, sc8* host, u16 port)
{
	ONENET_StatusTypeDef ONStatus = ONENET_OK;
	
	NBIOT_OneNET_Related_DictateEvent_SetTime(pClient, pClient->LWM2MStack->NBIotStack->Command_Timeout_Msec);
	
	memset((void *)pClient->LWM2MStack->NBIotStack->DataProcessStack, 0x0, pClient->LWM2MStack->NBIotStack->DataProcessStack_size);
	sprintf((char *)pClient->LWM2MStack->NBIotStack->DataProcessStack, "AT+MIPLCONFIG=%d,%s,%d\r", mode, host, port);
	
	NBIOT_OneNET_Related_ATCmd_SetCmdStack(pClient, pClient->LWM2MStack->NBIotStack->DataProcessStack, strlen((char *)pClient->LWM2MStack->NBIotStack->DataProcessStack), "OK", "ERROR");
	
#if NBIOT_PRINT_ERROR_CODE_TYPE
	if ((ONStatus = (ONENET_StatusTypeDef)pClient->LWM2MStack->NBIotStack->ATCmdStack->Write(pClient->LWM2MStack->NBIotStack->ATCmdStack)) == ONENET_ERROR) {
		ONStatus = NBIOT_OneNET_Related_DictateEvent_GetError(pClient);
	}
#else
	ONStatus = (ONENET_StatusTypeDef)pClient->LWM2MStack->NBIotStack->ATCmdStack->Write(pClient->LWM2MStack->NBIotStack->ATCmdStack);
#endif
	
	return ONStatus;
}

/**********************************************************************************************************
 @Function			ONENET_StatusTypeDef NBIOT_OneNET_Related_AccessConfig_AckTimeout(ONENET_ClientsTypeDef* pClient, u8 rspTimeout)
 @Description			NBIOT_OneNET_Related_AccessConfig_AckTimeout		: 设置应答超时时间
 @Input				pClient									: ONENET客户端实例
					rspTimeout								: AckTimeout < only 2~20 >
 @Return				ONENET_StatusTypeDef						: ONENET处理状态
 @attention			Parameters will not be saved
**********************************************************************************************************/
ONENET_StatusTypeDef NBIOT_OneNET_Related_AccessConfig_AckTimeout(ONENET_ClientsTypeDef* pClient, u8 rspTimeout)
{
	ONENET_StatusTypeDef ONStatus = ONENET_OK;
	
	NBIOT_OneNET_Related_DictateEvent_SetTime(pClient, pClient->LWM2MStack->NBIotStack->Command_Timeout_Msec);
	
	memset((void *)pClient->LWM2MStack->NBIotStack->DataProcessStack, 0x0, pClient->LWM2MStack->NBIotStack->DataProcessStack_size);
	sprintf((char *)pClient->LWM2MStack->NBIotStack->DataProcessStack, "AT+MIPLCONFIG=2,1,%d\r", rspTimeout);
	
	NBIOT_OneNET_Related_ATCmd_SetCmdStack(pClient, pClient->LWM2MStack->NBIotStack->DataProcessStack, strlen((char *)pClient->LWM2MStack->NBIotStack->DataProcessStack), "OK", "ERROR");
	
#if NBIOT_PRINT_ERROR_CODE_TYPE
	if ((ONStatus = (ONENET_StatusTypeDef)pClient->LWM2MStack->NBIotStack->ATCmdStack->Write(pClient->LWM2MStack->NBIotStack->ATCmdStack)) == ONENET_ERROR) {
		ONStatus = NBIOT_OneNET_Related_DictateEvent_GetError(pClient);
	}
#else
	ONStatus = (ONENET_StatusTypeDef)pClient->LWM2MStack->NBIotStack->ATCmdStack->Write(pClient->LWM2MStack->NBIotStack->ATCmdStack);
#endif
	
	return ONStatus;
}

/**********************************************************************************************************
 @Function			ONENET_StatusTypeDef NBIOT_OneNET_Related_AccessConfig_AckTimeout(ONENET_ClientsTypeDef* pClient, u8 rspTimeout)
 @Description			NBIOT_OneNET_Related_AccessConfig_AckTimeout		: 设置应答超时时间
 @Input				pClient									: ONENET客户端实例
					rspTimeout								: AckTimeout < only 2~20 >
 @Return				ONENET_StatusTypeDef						: ONENET处理状态
 @attention			Parameters will not be saved
**********************************************************************************************************/
//ONENET_StatusTypeDef NBIOT_OneNET_Related_AccessConfig_AckTimeout(ONENET_ClientsTypeDef* pClient, u8 rspTimeout)
//{
//	
//	
//	
//	
//}





























































/********************************************** END OF FLEE **********************************************/
