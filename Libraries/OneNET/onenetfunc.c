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
#include "radio_rf_app.h"

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
 @Function			ONENET_StatusTypeDef NBIOT_OneNET_Related_AccessConfig_ObserveAutoack(ONENET_ClientsTypeDef* pClient, u8 obsAutoack)
 @Description			NBIOT_OneNET_Related_AccessConfig_ObserveAutoack	: 设置模块自动应答
 @Input				pClient									: ONENET客户端实例
					obsAutoack								: ObserveAutoack < only 0 or 1 >
 @Return				ONENET_StatusTypeDef						: ONENET处理状态
 @attention			Parameters will not be saved
**********************************************************************************************************/
ONENET_StatusTypeDef NBIOT_OneNET_Related_AccessConfig_ObserveAutoack(ONENET_ClientsTypeDef* pClient, u8 obsAutoack)
{
	ONENET_StatusTypeDef ONStatus = ONENET_OK;
	
	NBIOT_OneNET_Related_DictateEvent_SetTime(pClient, pClient->LWM2MStack->NBIotStack->Command_Timeout_Msec);
	
	memset((void *)pClient->LWM2MStack->NBIotStack->DataProcessStack, 0x0, pClient->LWM2MStack->NBIotStack->DataProcessStack_size);
	sprintf((char *)pClient->LWM2MStack->NBIotStack->DataProcessStack, "AT+MIPLCONFIG=3,%d\r", obsAutoack);
	
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
 @Function			ONENET_StatusTypeDef NBIOT_OneNET_Related_Create_SuiteInstance(ONENET_ClientsTypeDef* pClient, s32* refer)
 @Description			NBIOT_OneNET_Related_Create_SuiteInstance		: 创建通信套件实例
 @Input				pClient									: ONENET客户端实例
					*refer									: Instance ID of OneNET communication suite
 @Return				ONENET_StatusTypeDef						: ONENET处理状态
**********************************************************************************************************/
ONENET_StatusTypeDef NBIOT_OneNET_Related_Create_SuiteInstance(ONENET_ClientsTypeDef* pClient, s32* refer)
{
	ONENET_StatusTypeDef ONStatus = ONENET_OK;
	
	NBIOT_OneNET_Related_DictateEvent_SetTime(pClient, pClient->LWM2MStack->NBIotStack->Command_Timeout_Msec);
	
	NBIOT_OneNET_Related_ATCmd_SetCmdStack(pClient, (unsigned char*)"AT+MIPLCREATE\r", strlen("AT+MIPLCREATE\r"), "OK", "ERROR");
	
	if ((ONStatus = (ONENET_StatusTypeDef)pClient->LWM2MStack->NBIotStack->ATCmdStack->Write(pClient->LWM2MStack->NBIotStack->ATCmdStack)) == ONENET_OK) {
		if (sscanf((const char*)pClient->LWM2MStack->NBIotStack->ATCmdStack->ATRecvbuf, "%*[^+MIPLCREATE]%*[^:]:%d", refer) <= 0) {
			ONStatus = ONENET_ERROR;
		}
	}
#if NBIOT_PRINT_ERROR_CODE_TYPE
	else {
		ONStatus = NBIOT_OneNET_Related_DictateEvent_GetError(pClient);
	}
#endif
	
	return ONStatus;
}

/**********************************************************************************************************
 @Function			ONENET_StatusTypeDef NBIOT_OneNET_Related_Delete_SuiteInstance(ONENET_ClientsTypeDef* pClient, s32 refer)
 @Description			NBIOT_OneNET_Related_Delete_SuiteInstance		: 删除通信套件实例
 @Input				pClient									: ONENET客户端实例
					refer									: Instance ID of OneNET communication suite
 @Return				ONENET_StatusTypeDef						: ONENET处理状态
**********************************************************************************************************/
ONENET_StatusTypeDef NBIOT_OneNET_Related_Delete_SuiteInstance(ONENET_ClientsTypeDef* pClient, s32 refer)
{
	ONENET_StatusTypeDef ONStatus = ONENET_OK;
	
	NBIOT_OneNET_Related_DictateEvent_SetTime(pClient, pClient->LWM2MStack->NBIotStack->Command_Timeout_Msec);
	
	memset((void *)pClient->LWM2MStack->NBIotStack->DataProcessStack, 0x0, pClient->LWM2MStack->NBIotStack->DataProcessStack_size);
	sprintf((char *)pClient->LWM2MStack->NBIotStack->DataProcessStack, "AT+MIPLDELETE=%d\r", refer);
	
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
 @Function			ONENET_StatusTypeDef NBIOT_OneNET_Related_CheckRead_SuiteVersion(ONENET_ClientsTypeDef* pClient)
 @Description			NBIOT_OneNET_Related_CheckRead_SuiteVersion		: 检出通信套件版本
 @Input				pClient									: ONENET客户端实例
 @Return				ONENET_StatusTypeDef						: ONENET处理状态
**********************************************************************************************************/
ONENET_StatusTypeDef NBIOT_OneNET_Related_CheckRead_SuiteVersion(ONENET_ClientsTypeDef* pClient)
{
	ONENET_StatusTypeDef ONStatus = ONENET_OK;
	
	NBIOT_OneNET_Related_DictateEvent_SetTime(pClient, pClient->LWM2MStack->NBIotStack->Command_Timeout_Msec);
	
	NBIOT_OneNET_Related_ATCmd_SetCmdStack(pClient, (unsigned char*)"AT+MIPLVER?\r", strlen("AT+MIPLVER?\r"), "OK", "ERROR");
	
	if ((ONStatus = (ONENET_StatusTypeDef)pClient->LWM2MStack->NBIotStack->ATCmdStack->Write(pClient->LWM2MStack->NBIotStack->ATCmdStack)) == ONENET_OK) {
		memset((void *)pClient->Parameter.suiteVersion, 0x0, sizeof(pClient->Parameter.suiteVersion));
		if (sscanf((const char*)pClient->LWM2MStack->NBIotStack->ATCmdStack->ATRecvbuf, "%*[^+MIPLVER]%*[^:]:%[^\r]", pClient->Parameter.suiteVersion) <= 0) {
			ONStatus = ONENET_ERROR;
		}
	}
#if NBIOT_PRINT_ERROR_CODE_TYPE
	else {
		ONStatus = NBIOT_OneNET_Related_DictateEvent_GetError(pClient);
	}
#endif
	
	return ONStatus;
}

/**********************************************************************************************************
 @Function			ONENET_StatusTypeDef NBIOT_OneNET_Related_Add_LwM2MObject(ONENET_ClientsTypeDef* pClient, ...)
 @Description			NBIOT_OneNET_Related_Add_LwM2MObject			: 创建LwM2M对象
 @Input				pClient									: ONENET客户端实例
					refer									: Instance ID of OneNET communication suite
					objId									: Object identifier
					insCount									: Instance count
					insBitmap									: Instance bitmap
					attrCount									: Attribute count
					actCount									: Action count
 @Return				ONENET_StatusTypeDef						: ONENET处理状态
**********************************************************************************************************/
ONENET_StatusTypeDef NBIOT_OneNET_Related_Add_LwM2MObject(ONENET_ClientsTypeDef* pClient, s32 refer, u32 objId, u16 insCount, sc8* insBitmap, u16 attrCount, u16 actCount)
{
	ONENET_StatusTypeDef ONStatus = ONENET_OK;
	
	NBIOT_OneNET_Related_DictateEvent_SetTime(pClient, pClient->LWM2MStack->NBIotStack->Command_Timeout_Msec);
	
	memset((void *)pClient->LWM2MStack->NBIotStack->DataProcessStack, 0x0, pClient->LWM2MStack->NBIotStack->DataProcessStack_size);
	sprintf((char *)pClient->LWM2MStack->NBIotStack->DataProcessStack, "AT+MIPLADDOBJ=%d,%d,%d,\"%s\",%d,%d\r", refer, objId, insCount, insBitmap, attrCount, actCount);
	
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
 @Function			ONENET_StatusTypeDef NBIOT_OneNET_Related_Del_LwM2MObject(ONENET_ClientsTypeDef* pClient, s32 refer, u32 objId)
 @Description			NBIOT_OneNET_Related_Del_LwM2MObject			: 删除LwM2M对象
 @Input				pClient									: ONENET客户端实例
					refer									: Instance ID of OneNET communication suite
					objId									: Object identifier
 @Return				ONENET_StatusTypeDef						: ONENET处理状态
**********************************************************************************************************/
ONENET_StatusTypeDef NBIOT_OneNET_Related_Del_LwM2MObject(ONENET_ClientsTypeDef* pClient, s32 refer, u32 objId)
{
	ONENET_StatusTypeDef ONStatus = ONENET_OK;
	
	NBIOT_OneNET_Related_DictateEvent_SetTime(pClient, pClient->LWM2MStack->NBIotStack->Command_Timeout_Msec);
	
	memset((void *)pClient->LWM2MStack->NBIotStack->DataProcessStack, 0x0, pClient->LWM2MStack->NBIotStack->DataProcessStack_size);
	sprintf((char *)pClient->LWM2MStack->NBIotStack->DataProcessStack, "AT+MIPLDELOBJ=%d,%d\r", refer, objId);
	
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
 @Function			ONENET_StatusTypeDef NBIOT_OneNET_Related_Send_RegisterRequest(ONENET_ClientsTypeDef* pClient, s32 refer, u32 lifetime, u32 timeout)
 @Description			NBIOT_OneNET_Related_Send_RegisterRequest		: 发送注册请求
 @Input				pClient									: ONENET客户端实例
					refer									: Instance ID of OneNET communication suite
					lifetime									: Device lifetime. Range: 16-268435454. Unit: second.
					[timeout]									: Timeout of registration. Range: 30-65535. Unit: second.
 @Return				ONENET_StatusTypeDef						: ONENET处理状态
**********************************************************************************************************/
ONENET_StatusTypeDef NBIOT_OneNET_Related_Send_RegisterRequest(ONENET_ClientsTypeDef* pClient, s32 refer, u32 lifetime, u32 timeout)
{
	ONENET_StatusTypeDef ONStatus = ONENET_OK;
	u16 datalength = 0;
	
	NBIOT_OneNET_Related_DictateEvent_SetTime(pClient, pClient->LWM2MStack->NBIotStack->Command_Timeout_Msec);
	
	memset((void *)pClient->LWM2MStack->NBIotStack->DataProcessStack, 0x0, pClient->LWM2MStack->NBIotStack->DataProcessStack_size);
	sprintf((char *)pClient->LWM2MStack->NBIotStack->DataProcessStack, "AT+MIPLOPEN=%d,%d", refer, lifetime);
	
	datalength = strlen((const char*)pClient->LWM2MStack->NBIotStack->DataProcessStack);
	if (timeout != NULL) {
		sprintf((char *)(pClient->LWM2MStack->NBIotStack->DataProcessStack + datalength), ",%d", timeout);
		datalength = strlen((const char*)pClient->LWM2MStack->NBIotStack->DataProcessStack);
	}
	
	sprintf((char *)(pClient->LWM2MStack->NBIotStack->DataProcessStack + datalength), "%c", '\r');
	
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
 @Function			ONENET_StatusTypeDef NBIOT_OneNET_Related_Send_DeregisterRequest(ONENET_ClientsTypeDef* pClient, s32 refer)
 @Description			NBIOT_OneNET_Related_Send_DeregisterRequest		: 发送注销请求
 @Input				pClient									: ONENET客户端实例
					refer									: Instance ID of OneNET communication suite
 @Return				ONENET_StatusTypeDef						: ONENET处理状态
**********************************************************************************************************/
ONENET_StatusTypeDef NBIOT_OneNET_Related_Send_DeregisterRequest(ONENET_ClientsTypeDef* pClient, s32 refer)
{
	ONENET_StatusTypeDef ONStatus = ONENET_OK;
	
	NBIOT_OneNET_Related_DictateEvent_SetTime(pClient, pClient->LWM2MStack->NBIotStack->Command_Timeout_Msec);
	
	memset((void *)pClient->LWM2MStack->NBIotStack->DataProcessStack, 0x0, pClient->LWM2MStack->NBIotStack->DataProcessStack_size);
	sprintf((char *)pClient->LWM2MStack->NBIotStack->DataProcessStack, "AT+MIPLCLOSE=%d\r", refer);
	
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
 @Function			ONENET_StatusTypeDef NBIOT_OneNET_Related_Respond_DiscoverRequest(ONENET_ClientsTypeDef* pClient, ...)
 @Description			NBIOT_OneNET_Related_Respond_DiscoverRequest		: 响应Discover请求
 @Input				pClient									: ONENET客户端实例
					refer									: Instance ID of OneNET communication suite
					msgId									: The message identifier, which comes from the URC “+ MIPLDISCOVER:”.
					result									: The result of discover.
					length									: The length of <valuestring>.
					valuestring								: A string which includes the attributes of the object and should be marked with double quotation marks.
					[raiMode]									: Specifies the flag of RAI (Release Assistant Indication) of message transmission.
													NULL		: A value of 0 can be provided but not necessary.
													0x200	: Release Indicator: indicate release after the message.
													0x400	: Release Indicator: indicate release after the message has been replied.
 @Return				ONENET_StatusTypeDef						: ONENET处理状态
**********************************************************************************************************/
ONENET_StatusTypeDef NBIOT_OneNET_Related_Respond_DiscoverRequest(ONENET_ClientsTypeDef* pClient, s32 refer, u32 msgId, u8 result, u16 length, sc8* valuestring, sc8* raiMode)
{
	ONENET_StatusTypeDef ONStatus = ONENET_OK;
	u16 datalength = 0;
	
	NBIOT_OneNET_Related_DictateEvent_SetTime(pClient, pClient->LWM2MStack->NBIotStack->Command_Timeout_Msec);
	
	memset((void *)pClient->LWM2MStack->NBIotStack->DataProcessStack, 0x0, pClient->LWM2MStack->NBIotStack->DataProcessStack_size);
	sprintf((char *)pClient->LWM2MStack->NBIotStack->DataProcessStack, "AT+MIPLDISCOVERRSP=%d,%d,%d,%d,\"%s\"", refer, msgId, result, length, valuestring);
	
	datalength = strlen((const char*)pClient->LWM2MStack->NBIotStack->DataProcessStack);
	if (raiMode != NULL) {
		sprintf((char *)(pClient->LWM2MStack->NBIotStack->DataProcessStack + datalength), ",%s", raiMode);
		datalength = strlen((const char*)pClient->LWM2MStack->NBIotStack->DataProcessStack);
	}
	
	sprintf((char *)(pClient->LWM2MStack->NBIotStack->DataProcessStack + datalength), "%c", '\r');
	
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
 @Function			ONENET_StatusTypeDef NBIOT_OneNET_Related_Respond_ObserveRequest(ONENET_ClientsTypeDef* pClient, ...)
 @Description			NBIOT_OneNET_Related_Respond_ObserveRequest		: 响应Observe请求
 @Input				pClient									: ONENET客户端实例
					refer									: Instance ID of OneNET communication suite
					msgId									: The message identifier, which comes from the URC “+ MIPLOBSERVE:”.
					result									: The result of discover.
					[raiMode]									: Specifies the flag of RAI (Release Assistant Indication) of message transmission.
													NULL		: A value of 0 can be provided but not necessary.
													0x200	: Release Indicator: indicate release after the message.
													0x400	: Release Indicator: indicate release after the message has been replied.
 @Return				ONENET_StatusTypeDef						: ONENET处理状态
**********************************************************************************************************/
ONENET_StatusTypeDef NBIOT_OneNET_Related_Respond_ObserveRequest(ONENET_ClientsTypeDef* pClient, s32 refer, u32 msgId, u8 result, sc8* raiMode)
{
	ONENET_StatusTypeDef ONStatus = ONENET_OK;
	u16 datalength = 0;
	
	NBIOT_OneNET_Related_DictateEvent_SetTime(pClient, pClient->LWM2MStack->NBIotStack->Command_Timeout_Msec);
	
	memset((void *)pClient->LWM2MStack->NBIotStack->DataProcessStack, 0x0, pClient->LWM2MStack->NBIotStack->DataProcessStack_size);
	sprintf((char *)pClient->LWM2MStack->NBIotStack->DataProcessStack, "AT+MIPLOBSERVERSP=%d,%d,%d", refer, msgId, result);
	
	datalength = strlen((const char*)pClient->LWM2MStack->NBIotStack->DataProcessStack);
	if (raiMode != NULL) {
		sprintf((char *)(pClient->LWM2MStack->NBIotStack->DataProcessStack + datalength), ",%s", raiMode);
		datalength = strlen((const char*)pClient->LWM2MStack->NBIotStack->DataProcessStack);
	}
	
	sprintf((char *)(pClient->LWM2MStack->NBIotStack->DataProcessStack + datalength), "%c", '\r');
	
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
 @Function			ONENET_StatusTypeDef NBIOT_OneNET_Related_Respond_ReadRequest(ONENET_ClientsTypeDef* pClient, ...)
 @Description			NBIOT_OneNET_Related_Respond_ReadRequest		: 响应Read请求
 @Input				pClient									: ONENET客户端实例
					refer									: Instance ID of OneNET communication suite
					msgId									: The message identifier, which comes from the URC “+ MIPLOBSERVE:”.
					result									: The result of discover.
					[msg]									: The Parameter struct addr.
					[raiMode]									: Specifies the flag of RAI (Release Assistant Indication) of message transmission.
													NULL		: A value of 0 can be provided but not necessary.
													0x200	: Release Indicator: indicate release after the message.
													0x400	: Release Indicator: indicate release after the message has been replied.
 @Return				ONENET_StatusTypeDef						: ONENET处理状态
**********************************************************************************************************/
ONENET_StatusTypeDef NBIOT_OneNET_Related_Respond_ReadRequest(ONENET_ClientsTypeDef* pClient, s32 refer, u32 msgId, u8 result, ONENET_MessageParaTypeDef* msg, sc8* raiMode)
{
	ONENET_StatusTypeDef ONStatus = ONENET_OK;
	u16 datalength = 0;
	
	NBIOT_OneNET_Related_DictateEvent_SetTime(pClient, pClient->LWM2MStack->NBIotStack->Command_Timeout_Msec);
	
	memset((void *)pClient->LWM2MStack->NBIotStack->DataProcessStack, 0x0, pClient->LWM2MStack->NBIotStack->DataProcessStack_size);
	sprintf((char *)pClient->LWM2MStack->NBIotStack->DataProcessStack, "AT+MIPLREADRSP=%d,%d,%d", refer, msgId, result);
	
	datalength = strlen((const char*)pClient->LWM2MStack->NBIotStack->DataProcessStack);
	if (msg != NULL) {
		sprintf((char *)(pClient->LWM2MStack->NBIotStack->DataProcessStack + datalength), ",%d,%d,%d,%d,%d,%s,%d,%d", \
			msg->objId, msg->insId, msg->resId, msg->valueType, msg->len, msg->value, msg->index, msg->flag);
		datalength = strlen((const char*)pClient->LWM2MStack->NBIotStack->DataProcessStack);
	}
	
	if (raiMode != NULL) {
		sprintf((char *)(pClient->LWM2MStack->NBIotStack->DataProcessStack + datalength), ",%s", raiMode);
		datalength = strlen((const char*)pClient->LWM2MStack->NBIotStack->DataProcessStack);
	}
	
	sprintf((char *)(pClient->LWM2MStack->NBIotStack->DataProcessStack + datalength), "%c", '\r');
	
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
 @Function			ONENET_StatusTypeDef NBIOT_OneNET_Related_Respond_WriteRequest(ONENET_ClientsTypeDef* pClient, ...)
 @Description			NBIOT_OneNET_Related_Respond_WriteRequest		: 响应Write请求
 @Input				pClient									: ONENET客户端实例
					refer									: Instance ID of OneNET communication suite
					msgId									: The message identifier, which comes from the URC “+ MIPLOBSERVE:”.
					result									: The result of discover.
					[raiMode]									: Specifies the flag of RAI (Release Assistant Indication) of message transmission.
													NULL		: A value of 0 can be provided but not necessary.
													0x200	: Release Indicator: indicate release after the message.
													0x400	: Release Indicator: indicate release after the message has been replied.
 @Return				ONENET_StatusTypeDef						: ONENET处理状态
**********************************************************************************************************/
ONENET_StatusTypeDef NBIOT_OneNET_Related_Respond_WriteRequest(ONENET_ClientsTypeDef* pClient, s32 refer, u32 msgId, u8 result, sc8* raiMode)
{
	ONENET_StatusTypeDef ONStatus = ONENET_OK;
	u16 datalength = 0;
	
	NBIOT_OneNET_Related_DictateEvent_SetTime(pClient, pClient->LWM2MStack->NBIotStack->Command_Timeout_Msec);
	
	memset((void *)pClient->LWM2MStack->NBIotStack->DataProcessStack, 0x0, pClient->LWM2MStack->NBIotStack->DataProcessStack_size);
	sprintf((char *)pClient->LWM2MStack->NBIotStack->DataProcessStack, "AT+MIPLWRITERSP=%d,%d,%d", refer, msgId, result);
	
	datalength = strlen((const char*)pClient->LWM2MStack->NBIotStack->DataProcessStack);
	if (raiMode != NULL) {
		sprintf((char *)(pClient->LWM2MStack->NBIotStack->DataProcessStack + datalength), ",%s", raiMode);
		datalength = strlen((const char*)pClient->LWM2MStack->NBIotStack->DataProcessStack);
	}
	
	sprintf((char *)(pClient->LWM2MStack->NBIotStack->DataProcessStack + datalength), "%c", '\r');
	
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
 @Function			ONENET_StatusTypeDef NBIOT_OneNET_Related_Respond_ExecuteRequest(ONENET_ClientsTypeDef* pClient, ...)
 @Description			NBIOT_OneNET_Related_Respond_ExecuteRequest		: 响应Execute请求
 @Input				pClient									: ONENET客户端实例
					refer									: Instance ID of OneNET communication suite
					msgId									: The message identifier, which comes from the URC “+ MIPLOBSERVE:”.
					result									: The result of discover.
					[raiMode]									: Specifies the flag of RAI (Release Assistant Indication) of message transmission.
													NULL		: A value of 0 can be provided but not necessary.
													0x200	: Release Indicator: indicate release after the message.
													0x400	: Release Indicator: indicate release after the message has been replied.
 @Return				ONENET_StatusTypeDef						: ONENET处理状态
**********************************************************************************************************/
ONENET_StatusTypeDef NBIOT_OneNET_Related_Respond_ExecuteRequest(ONENET_ClientsTypeDef* pClient, s32 refer, u32 msgId, u8 result, sc8* raiMode)
{
	ONENET_StatusTypeDef ONStatus = ONENET_OK;
	u16 datalength = 0;
	
	NBIOT_OneNET_Related_DictateEvent_SetTime(pClient, pClient->LWM2MStack->NBIotStack->Command_Timeout_Msec);
	
	memset((void *)pClient->LWM2MStack->NBIotStack->DataProcessStack, 0x0, pClient->LWM2MStack->NBIotStack->DataProcessStack_size);
	sprintf((char *)pClient->LWM2MStack->NBIotStack->DataProcessStack, "AT+MIPLEXECUTERSP=%d,%d,%d", refer, msgId, result);
	
	datalength = strlen((const char*)pClient->LWM2MStack->NBIotStack->DataProcessStack);
	if (raiMode != NULL) {
		sprintf((char *)(pClient->LWM2MStack->NBIotStack->DataProcessStack + datalength), ",%s", raiMode);
		datalength = strlen((const char*)pClient->LWM2MStack->NBIotStack->DataProcessStack);
	}
	
	sprintf((char *)(pClient->LWM2MStack->NBIotStack->DataProcessStack + datalength), "%c", '\r');
	
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
 @Function			ONENET_StatusTypeDef NBIOT_OneNET_Related_Respond_WriteAttributesRequest(ONENET_ClientsTypeDef* pClient, ...)
 @Description			NBIOT_OneNET_Related_Respond_WriteAttributesRequest	: 响应WriteAttributes请求
 @Input				pClient										: ONENET客户端实例
					refer										: Instance ID of OneNET communication suite
					msgId										: The message identifier, which comes from the URC “+ MIPLOBSERVE:”.
					result										: The result of discover.
					[raiMode]										: Specifies the flag of RAI (Release Assistant Indication) of message transmission.
													NULL			: A value of 0 can be provided but not necessary.
													0x200		: Release Indicator: indicate release after the message.
													0x400		: Release Indicator: indicate release after the message has been replied.
 @Return				ONENET_StatusTypeDef							: ONENET处理状态
**********************************************************************************************************/
ONENET_StatusTypeDef NBIOT_OneNET_Related_Respond_WriteAttributesRequest(ONENET_ClientsTypeDef* pClient, s32 refer, u32 msgId, u8 result, sc8* raiMode)
{
	ONENET_StatusTypeDef ONStatus = ONENET_OK;
	u16 datalength = 0;
	
	NBIOT_OneNET_Related_DictateEvent_SetTime(pClient, pClient->LWM2MStack->NBIotStack->Command_Timeout_Msec);
	
	memset((void *)pClient->LWM2MStack->NBIotStack->DataProcessStack, 0x0, pClient->LWM2MStack->NBIotStack->DataProcessStack_size);
	sprintf((char *)pClient->LWM2MStack->NBIotStack->DataProcessStack, "AT+MIPLPARAMETERRSP=%d,%d,%d", refer, msgId, result);
	
	datalength = strlen((const char*)pClient->LWM2MStack->NBIotStack->DataProcessStack);
	if (raiMode != NULL) {
		sprintf((char *)(pClient->LWM2MStack->NBIotStack->DataProcessStack + datalength), ",%s", raiMode);
		datalength = strlen((const char*)pClient->LWM2MStack->NBIotStack->DataProcessStack);
	}
	
	sprintf((char *)(pClient->LWM2MStack->NBIotStack->DataProcessStack + datalength), "%c", '\r');
	
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
 @Function			ONENET_StatusTypeDef NBIOT_OneNET_Related_Notify_ApplicationServer(ONENET_ClientsTypeDef* pClient, ...)
 @Description			NBIOT_OneNET_Related_Notify_ApplicationServer	: 通知数据到OneNET平台
 @Input				pClient									: ONENET客户端实例
					refer									: Instance ID of OneNET communication suite
					msgId									: The message identifier, which comes from the URC “+ MIPLOBSERVE:”.
					msg										: The Parameter struct addr.
					[ackid]									: The Ack ID
					[raiMode]									: Specifies the flag of RAI (Release Assistant Indication) of message transmission.
													NULL		: A value of 0 can be provided but not necessary.
													0x200	: Release Indicator: indicate release after the message.
													0x400	: Release Indicator: indicate release after the message has been replied.
 @Return				ONENET_StatusTypeDef						: ONENET处理状态
**********************************************************************************************************/
ONENET_StatusTypeDef NBIOT_OneNET_Related_Notify_ApplicationServer(ONENET_ClientsTypeDef* pClient, s32 refer, u32 msgId, ONENET_MessageParaTypeDef* msg, u16 ackid, sc8* raiMode)
{
	ONENET_StatusTypeDef ONStatus = ONENET_OK;
	u16 datalength = 0;
	
	NBIOT_OneNET_Related_DictateEvent_SetTime(pClient, pClient->LWM2MStack->NBIotStack->Command_Timeout_Msec);
	
	memset((void *)pClient->LWM2MStack->NBIotStack->DataProcessStack, 0x0, pClient->LWM2MStack->NBIotStack->DataProcessStack_size);
	sprintf((char *)pClient->LWM2MStack->NBIotStack->DataProcessStack, "AT+MIPLNOTIFY=%d,%d,%d,%d,%d,%d,%d,%s,%d,%d,%d", \
		refer, msgId, msg->objId, msg->insId, msg->resId, msg->valueType, msg->len, msg->value, msg->index, msg->flag, ackid);
	
	datalength = strlen((const char*)pClient->LWM2MStack->NBIotStack->DataProcessStack);
	if (raiMode != NULL) {
		sprintf((char *)(pClient->LWM2MStack->NBIotStack->DataProcessStack + datalength), ",%s", raiMode);
		datalength = strlen((const char*)pClient->LWM2MStack->NBIotStack->DataProcessStack);
	}
	
	sprintf((char *)(pClient->LWM2MStack->NBIotStack->DataProcessStack + datalength), "%c", '\r');
	
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
 @Function			ONENET_StatusTypeDef NBIOT_OneNET_Related_Send_UpdateRequest(ONENET_ClientsTypeDef* pClient, ...)
 @Description			NBIOT_OneNET_Related_Send_UpdateRequest			: 发送更新生命周期
 @Input				pClient									: ONENET客户端实例
					refer									: Instance ID of OneNET communication suite
					lifetime									: Updated lifetime value.
					withObjectFlag								: Whether to update with objects list.
													0		: Update without objects list.
													1		: Update with objects list.
					[raiMode]									: Specifies the flag of RAI (Release Assistant Indication) of message transmission.
													NULL		: A value of 0 can be provided but not necessary.
													0x200	: Release Indicator: indicate release after the message.
													0x400	: Release Indicator: indicate release after the message has been replied.
 @Return				ONENET_StatusTypeDef						: ONENET处理状态
**********************************************************************************************************/
ONENET_StatusTypeDef NBIOT_OneNET_Related_Send_UpdateRequest(ONENET_ClientsTypeDef* pClient, s32 refer, u32 lifetime, u8 withObjectFlag, sc8* raiMode)
{
	ONENET_StatusTypeDef ONStatus = ONENET_OK;
	u16 datalength = 0;
	
	NBIOT_OneNET_Related_DictateEvent_SetTime(pClient, pClient->LWM2MStack->NBIotStack->Command_Timeout_Msec);
	
	memset((void *)pClient->LWM2MStack->NBIotStack->DataProcessStack, 0x0, pClient->LWM2MStack->NBIotStack->DataProcessStack_size);
	sprintf((char *)pClient->LWM2MStack->NBIotStack->DataProcessStack, "AT+MIPLUPDATE=%d,%d,%d", refer, lifetime, withObjectFlag);
	
	datalength = strlen((const char*)pClient->LWM2MStack->NBIotStack->DataProcessStack);
	if (raiMode != NULL) {
		sprintf((char *)(pClient->LWM2MStack->NBIotStack->DataProcessStack + datalength), ",%s", raiMode);
		datalength = strlen((const char*)pClient->LWM2MStack->NBIotStack->DataProcessStack);
	}
	
	sprintf((char *)(pClient->LWM2MStack->NBIotStack->DataProcessStack + datalength), "%c", '\r');
	
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
 @Function			ONENET_StatusTypeDef ONENET_ReadPacket(ONENET_ClientsTypeDef* pClient, int *relatedType)
 @Description			ONENET_ReadPacket		: ONENET接收数据包
 @Input				pClient				: OneNET客户端实例
					relatedType			: OneNET消息类型
 @Return				ONENET_StatusTypeDef	: ONENET处理状态
**********************************************************************************************************/
ONENET_StatusTypeDef ONENET_ReadPacket(ONENET_ClientsTypeDef* pClient, int *relatedType)
{
	ONENET_StatusTypeDef ONStatus = ONENET_unknown_error;
	
	if (USART1_RX_STA & 0x8000) {														//接收到期待的应答结果
		if (strstr((const char*)USART1_RX_BUF, (const char*)ONENET_PROCOTOL_DISCOVER) != NULL) {
			memcpy(pClient->Recvbuf, USART1_RX_BUF, USART1_RX_STA & 0x1FFF);
			pClient->Recvlen = USART1_RX_STA & 0x1FFF;
			*relatedType = ONENET_MIPLDISCOVER;
			ONStatus = ONENET_OK;
		}
		else if (strstr((const char*)USART1_RX_BUF, (const char*)ONENET_PROCOTOL_OBSERVE) != NULL) {
			memcpy(pClient->Recvbuf, USART1_RX_BUF, USART1_RX_STA & 0x1FFF);
			pClient->Recvlen = USART1_RX_STA & 0x1FFF;
			*relatedType = ONENET_MIPLOBSERVE;
			ONStatus = ONENET_OK;
		}
		else if (strstr((const char*)USART1_RX_BUF, (const char*)ONENET_PROCOTOL_READ) != NULL) {
			memcpy(pClient->Recvbuf, USART1_RX_BUF, USART1_RX_STA & 0x1FFF);
			pClient->Recvlen = USART1_RX_STA & 0x1FFF;
			*relatedType = ONENET_MIPLREAD;
			ONStatus = ONENET_OK;
		}
		else if (strstr((const char*)USART1_RX_BUF, (const char*)ONENET_PROCOTOL_WRITE) != NULL) {
			memcpy(pClient->Recvbuf, USART1_RX_BUF, USART1_RX_STA & 0x1FFF);
			pClient->Recvlen = USART1_RX_STA & 0x1FFF;
			*relatedType = ONENET_MIPLWRITE;
			ONStatus = ONENET_OK;
		}
		else if (strstr((const char*)USART1_RX_BUF, (const char*)ONENET_PROCOTOL_EXECUTE) != NULL) {
			memcpy(pClient->Recvbuf, USART1_RX_BUF, USART1_RX_STA & 0x1FFF);
			pClient->Recvlen = USART1_RX_STA & 0x1FFF;
			*relatedType = ONENET_MIPLEXECUTE;
			ONStatus = ONENET_OK;
		}
		else if (strstr((const char*)USART1_RX_BUF, (const char*)ONENET_PROCOTOL_PARAMETER) != NULL) {
			memcpy(pClient->Recvbuf, USART1_RX_BUF, USART1_RX_STA & 0x1FFF);
			pClient->Recvlen = USART1_RX_STA & 0x1FFF;
			*relatedType = ONENET_MIPLPARAMETER;
			ONStatus = ONENET_OK;
		}
		else if (strstr((const char*)USART1_RX_BUF, (const char*)ONENET_PROCOTOL_EVENT) != NULL) {
			memcpy(pClient->Recvbuf, USART1_RX_BUF, USART1_RX_STA & 0x1FFF);
			pClient->Recvlen = USART1_RX_STA & 0x1FFF;
			*relatedType = ONENET_MIPLEVENT;
			ONStatus = ONENET_OK;
		}
		
		USART1_RX_STA &= 0x2000;														//缓存状态清0可继续接收
		memset((void *)USART1_RX_BUF, 0x0, sizeof(USART1_RX_BUF));							//清空缓存空间
	}
	
	return ONStatus;
}

/**********************************************************************************************************
 @Function			static ONENET_StatusTypeDef ONENET_DiscoverAckEvent(ONENET_ClientsTypeDef* pClient)
 @Description			ONENET_DiscoverAckEvent
 @Input				pClient				: OneNET客户端实例
 @Return				ONENET_StatusTypeDef	: ONENET处理状态
**********************************************************************************************************/
static ONENET_StatusTypeDef ONENET_DiscoverAckEvent(ONENET_ClientsTypeDef* pClient)
{
	ONENET_StatusTypeDef ONStatus = ONENET_OK;
	int ref, msgId, objId;
	
	if (sscanf((const char*)pClient->Recvbuf, "%*[^MIPLDISCOVER]%*[^:]: %d,%d,%d", &ref, &msgId, &objId) <= 0) {
		ONStatus = ONENET_ERROR;
	}
	else {
		pClient->Parameter.discoverInfo.ref					= ref;
		pClient->Parameter.discoverInfo.msgId					= msgId;
		pClient->Parameter.discoverInfo.objId					= objId;
		Radio_Trf_Debug_Printf_Level2("MIPLDISCOVER:%d,%d,%d", ref, msgId, objId);
	}
	
	return ONStatus;
}

/**********************************************************************************************************
 @Function			static ONENET_StatusTypeDef ONENET_ObserveAckEvent(ONENET_ClientsTypeDef* pClient)
 @Description			ONENET_ObserveAckEvent
 @Input				pClient				: OneNET客户端实例
 @Return				ONENET_StatusTypeDef	: ONENET处理状态
**********************************************************************************************************/
static ONENET_StatusTypeDef ONENET_ObserveAckEvent(ONENET_ClientsTypeDef* pClient)
{
	ONENET_StatusTypeDef ONStatus = ONENET_OK;
	int ref, msgId, flag, objId, insId, resId;
	
	if (sscanf((const char*)pClient->Recvbuf, "%*[^MIPLOBSERVE]%*[^:]: %d,%d,%d,%d,%d,%d", &ref, &msgId, &flag, &objId, &insId, &resId) <= 0) {
		ONStatus = ONENET_ERROR;
	}
	else {
		for (int index = 0; index < ONENET_OBJECT_INSCOUNT; index++) {
			if (insId == index) {
				pClient->Parameter.observeInfo[index].ref		= ref;
				pClient->Parameter.observeInfo[index].msgId		= msgId;
				pClient->Parameter.observeInfo[index].flag		= flag;
				pClient->Parameter.observeInfo[index].objId		= objId;
				pClient->Parameter.observeInfo[index].insId		= insId;
				pClient->Parameter.observeInfo[index].resId		= resId;
				Radio_Trf_Debug_Printf_Level2("MIPLOBSERVE:%d,%d,%d,%d,%d,%d", ref, msgId, flag, objId, insId, resId);
			}
		}
	}
	
	return ONStatus;
}

/**********************************************************************************************************
 @Function			static ONENET_StatusTypeDef ONENET_ReadAckEvent(ONENET_ClientsTypeDef* pClient)
 @Description			ONENET_ReadAckEvent
 @Input				pClient				: OneNET客户端实例
 @Return				ONENET_StatusTypeDef	: ONENET处理状态
**********************************************************************************************************/
static ONENET_StatusTypeDef ONENET_ReadAckEvent(ONENET_ClientsTypeDef* pClient)
{
	ONENET_StatusTypeDef ONStatus = ONENET_OK;
	//Todo
	return ONStatus;
}

/**********************************************************************************************************
 @Function			static ONENET_StatusTypeDef ONENET_WriteAckEvent(ONENET_ClientsTypeDef* pClient)
 @Description			ONENET_WriteAckEvent
 @Input				pClient				: OneNET客户端实例
 @Return				ONENET_StatusTypeDef	: ONENET处理状态
**********************************************************************************************************/
static ONENET_StatusTypeDef ONENET_WriteAckEvent(ONENET_ClientsTypeDef* pClient)
{
	ONENET_StatusTypeDef ONStatus = ONENET_OK;
	//Todo
	return ONStatus;
}

/**********************************************************************************************************
 @Function			static ONENET_StatusTypeDef ONENET_ExecuteAckEvent(ONENET_ClientsTypeDef* pClient)
 @Description			ONENET_ExecuteAckEvent
 @Input				pClient				: OneNET客户端实例
 @Return				ONENET_StatusTypeDef	: ONENET处理状态
**********************************************************************************************************/
static ONENET_StatusTypeDef ONENET_ExecuteAckEvent(ONENET_ClientsTypeDef* pClient)
{
	ONENET_StatusTypeDef ONStatus = ONENET_OK;
	//Todo
	return ONStatus;
}

/**********************************************************************************************************
 @Function			static ONENET_StatusTypeDef ONENET_ParameterAckEvent(ONENET_ClientsTypeDef* pClient)
 @Description			ONENET_ParameterAckEvent
 @Input				pClient				: OneNET客户端实例
 @Return				ONENET_StatusTypeDef	: ONENET处理状态
**********************************************************************************************************/
static ONENET_StatusTypeDef ONENET_ParameterAckEvent(ONENET_ClientsTypeDef* pClient)
{
	ONENET_StatusTypeDef ONStatus = ONENET_OK;
	//Todo
	return ONStatus;
}

/**********************************************************************************************************
 @Function			static ONENET_StatusTypeDef ONENET_EventAckEvent(ONENET_ClientsTypeDef* pClient)
 @Description			ONENET_EventAckEvent
 @Input				pClient				: OneNET客户端实例
 @Return				ONENET_StatusTypeDef	: ONENET处理状态
**********************************************************************************************************/
static ONENET_StatusTypeDef ONENET_EventAckEvent(ONENET_ClientsTypeDef* pClient)
{
	ONENET_StatusTypeDef ONStatus = ONENET_OK;
	int ref, evtid, ackid;
	
	if (sscanf((const char*)pClient->Recvbuf, "%*[^MIPLEVENT]%*[^:]: %d,%d,%d", &ref, &evtid, &ackid) <= 0) {
		ONStatus = ONENET_ERROR;
	}
	else {
		pClient->Parameter.eventInfo.ref						= ref;
		pClient->Parameter.eventInfo.evtid						= evtid;
		pClient->Parameter.eventInfo.ackid						= ackid;
		Radio_Trf_Debug_Printf_Level2("MIPLEVENT:%d,%d", ref, evtid);
	}
	
	return ONStatus;
}

/**********************************************************************************************************
 @Function			ONENET_StatusTypeDef ONENET_RecvPacketProcess(ONENET_ClientsTypeDef* pClient, int *relatedType)
 @Description			ONENET_RecvPacketProcess	: ONENET接收数据包处理
 @Input				pClient				: OneNET客户端实例
					relatedType			: OneNET消息类型
 @Return				ONENET_StatusTypeDef	: ONENET处理状态
**********************************************************************************************************/
ONENET_StatusTypeDef ONENET_RecvPacketProcess(ONENET_ClientsTypeDef* pClient, int *relatedType)
{
	ONENET_StatusTypeDef ONStatus = ONENET_OK;
	int packetType = -1;
	
	ONStatus = ONENET_ReadPacket(pClient, &packetType);									//获取OneNET数据并提取消息类型
	if (ONStatus != ONENET_OK) {														//没有获取到数据
		ONStatus = ONENET_unknown_error;
		goto exit;
	}
	
	switch ((ONENET_RelatedURCsCodeTypeDef)packetType)									//根据消息类型执行不同处理
	{
	case ONENET_MIPLDISCOVER:
		ONENET_DiscoverAckEvent(pClient);
		break;
	
	case ONENET_MIPLOBSERVE:
		ONENET_ObserveAckEvent(pClient);
		break;
	
	case ONENET_MIPLREAD:
		ONENET_ReadAckEvent(pClient);
		break;
	
	case ONENET_MIPLWRITE:
		ONENET_WriteAckEvent(pClient);
		break;
	
	case ONENET_MIPLEXECUTE:
		ONENET_ExecuteAckEvent(pClient);
		break;
	
	case ONENET_MIPLPARAMETER:
		ONENET_ParameterAckEvent(pClient);
		break;
	
	case ONENET_MIPLEVENT:
		ONENET_EventAckEvent(pClient);
		break;
	
	default:
		ONStatus = ONENET_ERROR;
		break;
	}
	
	if (ONStatus == ONENET_OK) {
		*relatedType = packetType;
	}
	
exit:
	return ONStatus;
}

/**********************************************************************************************************
 @Function			ONENET_StatusTypeDef ONENET_WaitforRecvAck(ONENET_ClientsTypeDef* pClient, ONENET_RelatedURCsCodeTypeDef relatedType, Stm32_CalculagraphTypeDef* timerS)
 @Description			ONENET_WaitforRecvAck	: ONENET等待接收到相应应答消息类型
 @Input				pClient				: OneNET客户端实例
					relatedType			: OneNET消息类型
					timer				: 超时计时器
 @Return				ONENET_StatusTypeDef	: ONENET处理状态
**********************************************************************************************************/
ONENET_StatusTypeDef ONENET_WaitforRecvAck(ONENET_ClientsTypeDef* pClient, ONENET_RelatedURCsCodeTypeDef relatedType, Stm32_CalculagraphTypeDef* timerS)
{
	ONENET_StatusTypeDef ONStatus = ONENET_OK;
	int packetType = -1;
	
	Uart1_PortSerialEnable(ENABLE, DISABLE);											//开启接收中断
	USART1_RX_STA |= 0x2000;															//接收状态复位
	memset((void *)USART1_RX_BUF, 0x0, sizeof(USART1_RX_BUF));								//清空缓存空间
	
	while (packetType != relatedType) {
		if ((ONStatus = ONENET_RecvPacketProcess(pClient, &packetType)) == ONENET_ERROR) {		//接收数据处理
			Uart1_PortSerialEnable(DISABLE, DISABLE);
			goto exit;
		}
		if (Stm32_Calculagraph_IsExpiredSec(timerS) == true) {								//到达超时时间
			break;
		}
		
		OneNET_WaitforCallback(pClient);
	}
	
	Uart1_PortSerialEnable(DISABLE, DISABLE);
	
	if (packetType != relatedType) {
		ONStatus = ONENET_CMD_TIMEOUT;
	}
	else {
		ONStatus = ONENET_OK;
	}
	
exit:
	return ONStatus;
}

/********************************************** END OF FLEE **********************************************/
