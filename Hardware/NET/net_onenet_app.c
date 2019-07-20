/**
  *********************************************************************************************************
  * @file    net_onenet_app.c
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

#include "net_onenet_app.h"
#include "platform_config.h"
#include "platform_map.h"
#include "hal_rtc.h"
#include "string.h"
#include "radio_rf_app.h"

/**********************************************************************************************************
 @Function			void NET_ONENET_APP_PollExecution(ONENET_ClientsTypeDef* pClient)
 @Description			ONENET_APP_PollExecution				: OneNET逻辑处理
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_APP_PollExecution(ONENET_ClientsTypeDef* pClient)
{
	switch (pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent)
	{
	case STOP_MODE:
		NET_ONENET_NBIOT_Event_StopMode(pClient);
		break;
	
	case HARDWARE_REBOOT:
		NET_ONENET_NBIOT_Event_HardwareReboot(pClient);
		break;
	
	case REPORT_ERROE:
		NET_ONENET_NBIOT_Event_ReportError(pClient);
		break;
	
	case MODULE_CHECK:
		NET_ONENET_NBIOT_Event_ModuleCheck(pClient);
		break;
	
	case PARAMETER_CONFIG:
		NET_ONENET_NBIOT_Event_ParameterConfig(pClient);
		break;
	
	case ICCID_CHECK:
		NET_ONENET_NBIOT_Event_SimICCIDCheck(pClient);
		break;
	
	case MISC_EQUIP_CONFIG:
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case ATTACH_CHECK:
		NET_ONENET_NBIOT_Event_AttachCheck(pClient);
		break;
	
	case ATTACH_EXECUTE:
		NET_ONENET_NBIOT_Event_AttachExecute(pClient);
		break;
	
	case ATTACH_INQUIRE:
		NET_ONENET_NBIOT_Event_AttachInquire(pClient);
		break;
	
	case PARAMETER_CHECKOUT:
		NET_ONENET_NBIOT_Event_ParameterCheckOut(pClient);
		break;
	
	case MINIMUM_FUNCTIONALITY:
		NET_ONENET_NBIOT_Event_MinimumFunctionality(pClient);
		break;
	
	case FULL_FUNCTIONALITY:
		NET_ONENET_NBIOT_Event_FullFunctionality(pClient);
		break;
	
	case CLEAR_STORED_EARFCN:
#if NBIOT_CLEAR_STORED_EARFCN_STAT
		NET_ONENET_NBIOT_Event_ClearStoredEARFCN(pClient);
#else
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
#endif
		break;
	
	case CDP_SERVER_CHECK:
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case CDP_SERVER_CONFIG:
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case NBAND_MODE_CHECK:
		NET_ONENET_NBIOT_Event_NbandModeCheck(pClient);
		break;
	
	case NBAND_MODE_CONFIG:
		NET_ONENET_NBIOT_Event_NbandModeConfig(pClient);
		break;
	
	case SEND_DATA:
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case RECV_DATA:
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case SEND_DATA_RA_NORMAL:
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case RECV_DATA_RA_NORMAL:
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case EXECUT_DOWNLINK_DATA:
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case MQTTSN_PROCESS_STACK:
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case DNS_PROCESS_STACK:
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case ONENET_PROCESS_STACK:
		NET_ONENET_APP_ProcessExecution(pClient);
		break;
	
	case LISTEN_RUN_CTL:
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	default :
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_APP_ProcessExecution(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_APP_ProcessExecution		: OneNET协议逻辑处理
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_APP_ProcessExecution(ONENET_ClientsTypeDef* pClient)
{
	switch (pClient->ProcessState)
	{
	case ONENET_PROCESSSTATE_INIT:
		NET_ONENET_Event_Init(pClient);
		break;
	
	case ONENET_PROCESSSTATE_SUITE:
		NET_ONENET_Event_Suite(pClient);
		break;
	
	case ONENET_PROCESSSTATE_OBJECT:
		NET_ONENET_Event_Object(pClient);
		break;
	
	case ONENET_PROCESSSTATE_REGISTER:
		NET_ONENET_Event_Register(pClient);
		break;
	
	case ONENET_PROCESSSTATE_ACTIVE:
		NET_ONENET_Event_Active(pClient);
		break;
	
	case ONENET_PROCESSSTATE_SLEEP:
		NET_ONENET_Event_Sleep(pClient);
		break;
	
	case ONENET_PROCESSSTATE_AWEAK:
		NET_ONENET_Event_Aweak(pClient);
		break;
	
	case ONENET_PROCESSSTATE_LOST:
		NET_ONENET_Event_Lost(pClient);
		break;
	}
}

/**********************************************************************************************************
 @Function			static void ONENET_NBIOT_DictateEvent_SetTime(ONENET_ClientsTypeDef* pClient, unsigned int TimeoutSec)
 @Description			ONENET_NBIOT_DictateEvent_SetTime		: 事件运行控制器注入时间(内部使用)
 @Input				pClient							: OneNET客户端实例
					TimeoutSec						: 注入超时时间
 @Return				void
 @attention			事件运行之前判断是否需要注入时间
**********************************************************************************************************/
static void ONENET_NBIOT_DictateEvent_SetTime(ONENET_ClientsTypeDef* pClient, unsigned int TimeoutSec)
{
	Stm32_CalculagraphTypeDef dictateRunTime;
	
	/* It is the first time to execute */
	if (pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEnable != true) {
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEnable = true;
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateTimeoutSec = TimeoutSec;
		Stm32_Calculagraph_CountdownSec(&dictateRunTime, pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateTimeoutSec);
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateRunTime = dictateRunTime;
	}
}

/**********************************************************************************************************
 @Function			static void ONENET_DictateEvent_SetTime(ONENET_ClientsTypeDef* pClient, unsigned int TimeoutSec)
 @Description			ONENET_DictateEvent_SetTime			: 事件运行控制器注入时间(内部使用)
 @Input				pClient							: OneNET客户端实例
					TimeoutSec						: 注入超时时间
 @Return				void
 @attention			事件运行之前判断是否需要注入时间
**********************************************************************************************************/
static void ONENET_DictateEvent_SetTime(ONENET_ClientsTypeDef* pClient, unsigned int TimeoutSec)
{
	Stm32_CalculagraphTypeDef dictateRunTime;
	
	/* It is the first time to execute */
	if (pClient->DictateRunCtl.dictateEnable != true) {
		pClient->DictateRunCtl.dictateEnable = true;
		pClient->DictateRunCtl.dictateTimeoutSec = TimeoutSec;
		Stm32_Calculagraph_CountdownSec(&dictateRunTime, pClient->DictateRunCtl.dictateTimeoutSec);
		pClient->DictateRunCtl.dictateRunTime = dictateRunTime;
	}
}

/**********************************************************************************************************
 @Function			static void ONENET_NormalDictateEvent_SetTime(ONENET_ClientsTypeDef* pClient, Stm32_CalculagraphTypeDef* pTimer, unsigned int TimeoutSec)
 @Description			ONENET_NormalDictateEvent_SetTime		: 事件运行控制器注入时间(内部使用)
 @Input				pClient							: OneNET客户端实例
					TimeoutSec						: 注入超时时间
 @Return				void
 @attention			事件运行之前判断是否需要注入时间
**********************************************************************************************************/
static void ONENET_NormalDictateEvent_SetTime(ONENET_ClientsTypeDef* pClient, Stm32_CalculagraphTypeDef* pTimer, unsigned int TimeoutSec)
{
	Stm32_CalculagraphTypeDef dictateRunTime;
	
	pClient->DictateRunCtl.dictateTimeoutSec = TimeoutSec;
	Stm32_Calculagraph_CountdownSec(&dictateRunTime, pClient->DictateRunCtl.dictateTimeoutSec);
	*pTimer = dictateRunTime;
}

static unsigned char* ONENET_NBIOT_GetDictateFailureCnt(ONENET_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateNoTimeOut)
{
	unsigned char* dictateFailureCnt;
	
	switch (dictateNoTimeOut)
	{
	case HARDWARE_REBOOT:
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateRebootFailureCnt;
		break;
	
	case REPORT_ERROE:
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateReportErrorFailureCnt;
		break;
	
	case MODULE_CHECK:
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateModuleCheckFailureCnt;
		break;
	
	case PARAMETER_CONFIG:
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateParameterConfigFailureCnt;
		break;
	
	case ICCID_CHECK:
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateSimICCIDCheckFailureCnt;
		break;
	
	case ATTACH_CHECK:
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateAttachCheckFailureCnt;
		break;
	
	case ATTACH_EXECUTE:
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateAttachExecuteFailureCnt;
		break;
	
	case ATTACH_INQUIRE:
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateAttachInquireFailureCnt;
		break;
	
	case PARAMETER_CHECKOUT:
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictatePatameterCheckOutFailureCnt;
		break;
	
	case MINIMUM_FUNCTIONALITY:
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateMinimumFunctionalityFailureCnt;
		break;
	
	case FULL_FUNCTIONALITY:
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateFullFunctionalityFailureCnt;
		break;
	
	case CLEAR_STORED_EARFCN:
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateClearStoredEARFCNFailureCnt;
		break;
	
	case NBAND_MODE_CHECK:
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateNbandModeCheckFailureCnt;
		break;
	
	case NBAND_MODE_CONFIG:
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateNbandModeConfigFailureCnt;
		break;
	
	default :
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateRebootFailureCnt;
		break;
	}
	
	return dictateFailureCnt;
}

static unsigned char* ONENET_GetDictateFailureCnt(ONENET_ClientsTypeDef* pClient, ONENET_ProcessStateTypeDef dictateNoTimeOut)
{
	unsigned char* dictateFailureCnt;
	
	switch (dictateNoTimeOut)
	{
	case ONENET_PROCESSSTATE_INIT:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateInitFailureCnt;
		break;
	
	case ONENET_PROCESSSTATE_SUITE:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateSuiteFailureCnt;
		break;
	
	case ONENET_PROCESSSTATE_OBJECT:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateObjectFailureCnt;
		break;
	
	case ONENET_PROCESSSTATE_REGISTER:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateRegisterFailureCnt;
		break;
	
	case ONENET_PROCESSSTATE_ACTIVE:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateActiveFailureCnt;
		break;
	
	case ONENET_PROCESSSTATE_SLEEP:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateSleepFailureCnt;
		break;
	
	case ONENET_PROCESSSTATE_AWEAK:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateAweakFailureCnt;
		break;
	
	case ONENET_PROCESSSTATE_LOST:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateLostFailureCnt;
		break;
	}
	
	return dictateFailureCnt;
}

/**********************************************************************************************************
 @Function			static void ONENET_NBIOT_DictateEvent_FailExecute(ONENET_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateTimeOut, \
																					  NBIOT_DictateEventTypeDef dictateFail, \
																					  NBIOT_DictateEventTypeDef dictateNoTimeOut)
 @Description			ONENET_NBIOT_DictateEvent_FailExecute	: 事件运行控制器出错执行(内部使用)
 @Input				pClient							: OneNET客户端实例
					dictateTimeOut						: 事假处理错误超时
					dictateFail						: 事件处理错误次数溢出
					dictateNoTimeOut					: 事假处理错误未超时
 @Return				void
**********************************************************************************************************/
static void ONENET_NBIOT_DictateEvent_FailExecute(ONENET_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateTimeOut, \
																  NBIOT_DictateEventTypeDef dictateFail, \
																  NBIOT_DictateEventTypeDef dictateNoTimeOut)
{
	unsigned char* dictateFailureCnt;
	
	dictateFailureCnt = ONENET_NBIOT_GetDictateFailureCnt(pClient, dictateNoTimeOut);
	
	if (Stm32_Calculagraph_IsExpiredSec(&pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateRunTime) == true) {
		/* Dictate TimeOut */
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEnable = false;
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = dictateTimeOut;
		*dictateFailureCnt += 1;
		if (*dictateFailureCnt > 3) {
			*dictateFailureCnt = 0;
			pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = dictateFail;
		}
	}
	else {
		/* Dictate isn't TimeOut */
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = dictateNoTimeOut;
	}
}

/**********************************************************************************************************
 @Function			static void ONENET_NBIOT_DictateEvent_SuccessExecute(ONENET_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateSuccess, \
																						NBIOT_DictateEventTypeDef dictateNoTimeOut)
 @Description			ONENET_NBIOT_DictateEvent_SuccessExecute: 事件运行控制器正确执行(内部使用)
 @Input				pClient							: OneNET客户端实例
					dictateSuccess						: 事假处理正确
					dictateNoTimeOut					: 事假处理错误未超时
 @Return				void
**********************************************************************************************************/
static void ONENET_NBIOT_DictateEvent_SuccessExecute(ONENET_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateSuccess, \
																	NBIOT_DictateEventTypeDef dictateNoTimeOut)
{
	unsigned char* dictateFailureCnt;
	
	dictateFailureCnt = ONENET_NBIOT_GetDictateFailureCnt(pClient, dictateNoTimeOut);
	
	pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEnable = false;
	pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = dictateSuccess;
	*dictateFailureCnt = 0;
}

/**********************************************************************************************************
 @Function			static void ONENET_DictateEvent_FailExecute(ONENET_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateTimeOut, \
																				 ONENET_ProcessStateTypeDef dictateSubTimeOut, \
																				 ONENET_ProcessStateTypeDef dictateSubNoTimeOut)
 @Description			ONENET_DictateEvent_FailExecute		: 事件运行控制器出错执行(内部使用)
 @Input				pClient							: OneNET客户端实例
					dictateTimeOut						: 事件处理错误超时
					dictateSubTimeOut					: 事件处理错误次数溢出
					dictateNoTimeOut					: 事件处理错误未超时
 @Return				void
**********************************************************************************************************/
static void ONENET_DictateEvent_FailExecute(ONENET_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateTimeOut, \
															 ONENET_ProcessStateTypeDef dictateSubTimeOut, \
															 ONENET_ProcessStateTypeDef dictateSubNoTimeOut)
{
	unsigned char* dictateFailureCnt;
	
	dictateFailureCnt = ONENET_GetDictateFailureCnt(pClient, dictateSubNoTimeOut);
	
	if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
		/* Dictate TimeOut */
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = dictateTimeOut;
		pClient->ProcessState = dictateSubTimeOut;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_ONENET;
		*dictateFailureCnt += 1;
		if (*dictateFailureCnt > 3) {
			*dictateFailureCnt = 0;
			pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = ONENET_PROCESS_STACK;
			pClient->ProcessState = ONENET_PROCESSSTATE_LOST;
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_ONENET;
		}
	}
	else {
		/* Dictate isn't TimeOut */
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = ONENET_PROCESS_STACK;
		pClient->ProcessState = dictateSubNoTimeOut;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_ONENET;
	}
}

/**********************************************************************************************************
 @Function			static void MQTTSN_DictateEvent_SuccessExecute(MQTTSN_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateSuccess, \
																				    MQTTSN_SubStateTypeDef dictateSubSuccess, \
																				    MQTTSN_SubStateTypeDef dictateSubNoTimeOut, \
																				    bool dictateFailureCntState)
 @Description			MQTTSN_DictateEvent_SuccessExecute		: 事件运行控制器正确执行(内部使用)
 @Input				pClient							: MQTTSN客户端实例
					dictateSuccess						: 事件处理正确
					dictateSubSuccess					: 事件处理正确
					dictateNoTimeOut					: 事件处理错误未超时
 @Return				void
**********************************************************************************************************/
static void ONENET_DictateEvent_SuccessExecute(ONENET_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateSuccess, \
															    ONENET_ProcessStateTypeDef dictateSubSuccess, \
															    ONENET_ProcessStateTypeDef dictateSubNoTimeOut, \
															    bool dictateFailureCntState)
{
	unsigned char* dictateFailureCnt;
	
	if (dictateFailureCntState) {
		dictateFailureCnt = ONENET_GetDictateFailureCnt(pClient, dictateSubNoTimeOut);
		pClient->DictateRunCtl.dictateEnable = false;
		*dictateFailureCnt = 0;
	}
	
	pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = dictateSuccess;
	pClient->ProcessState = dictateSubSuccess;
	pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_ONENET;
}

/**********************************************************************************************************
 @Function			static void ONENET_NBIOT_GetConnectTime(ONENET_ClientsTypeDef* pClient, bool startIdleTime)
 @Description			ONENET_NBIOT_GetConnectTime			: 获取NBConnect状态时间(内部使用)
 @Input				pClient							: OneNET客户端实例
					startIdleTime						: 是否开启Idle状态计时器
 @Return				void
**********************************************************************************************************/
static void ONENET_NBIOT_GetConnectTime(ONENET_ClientsTypeDef* pClient, bool startIdleTime)
{
	unsigned int uICoapConnectTime = 0;
	
	/* Get ConnectTime */
	uICoapConnectTime = Stm32_EventRunningTime_EndMS(&pClient->LWM2MStack->NBIotStack->ConnectTimeMS) / 1000;
	/* End ConnectTime */
	TCFG_SystemData.CoapConnectTime = pClient->LWM2MStack->NBIotStack->CoapConnectTimeSec + uICoapConnectTime;
	pClient->LWM2MStack->NBIotStack->CoapConnectTimeSec = TCFG_SystemData.CoapConnectTime;
	/* Start or End IdleTime */
	if (startIdleTime != false) {
		Stm32_EventRunningTime_StartMS(&pClient->LWM2MStack->NBIotStack->IdleTimeMS);
	}
	else {
		Stm32_EventRunningTime_EndMS(&pClient->LWM2MStack->NBIotStack->IdleTimeMS);
	}
}

/**********************************************************************************************************
 @Function			static void ONENET_NBIOT_GetIdleTime(ONENET_ClientsTypeDef* pClient, bool startConnectTime)
 @Description			ONENET_NBIOT_GetIdleTime				: 获取NBIdle状态时间(内部使用)
 @Input				pClient							: OneNET客户端实例
					startConnectTime					: 是否开启Connect状态计时器
 @Return				void
**********************************************************************************************************/
static void ONENET_NBIOT_GetIdleTime(ONENET_ClientsTypeDef* pClient, bool startConnectTime)
{
	unsigned int uICoapIdleTime = 0;
	
	/* Get IdleTime */
	uICoapIdleTime = Stm32_EventRunningTime_EndMS(&pClient->LWM2MStack->NBIotStack->IdleTimeMS) / 1000;
	/* End IdleTime */
	TCFG_SystemData.CoapIdleTime = pClient->LWM2MStack->NBIotStack->CoapIdleTimeSec + uICoapIdleTime;
	pClient->LWM2MStack->NBIotStack->CoapIdleTimeSec = TCFG_SystemData.CoapIdleTime;
	/* Start or End ConnectTime */
	if (startConnectTime != false) {
		Stm32_EventRunningTime_StartMS(&pClient->LWM2MStack->NBIotStack->ConnectTimeMS);
	}
	else {
		Stm32_EventRunningTime_EndMS(&pClient->LWM2MStack->NBIotStack->ConnectTimeMS);
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_NBIOT_Event_StopMode(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_StopMode		: 停止模式
 @Input				pClient							: OneNET客户端实例
 @Return				void
 @attention			当30分钟或有数据需要发送时退出停止模式尝试重启NB,开启NB运行
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_StopMode(ONENET_ClientsTypeDef* pClient)
{
	Stm32_CalculagraphTypeDef dictateRunTime;
	static unsigned char OneNETSendMessageIndex;
	
	/* It is the first time to execute */
	if (pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEnable != true) {
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEnable = true;
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateTimeoutSec = 1800;
		Stm32_Calculagraph_CountdownSec(&dictateRunTime, pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateTimeoutSec);
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateRunTime = dictateRunTime;
		/* NBIOT Module Poweroff */
		NBIOT_Neul_NBxx_HardwarePoweroff(pClient->LWM2MStack->NBIotStack);
		/* Clear Stored EARFCN */
		pClient->LWM2MStack->NBIotStack->ClearStoredEARFCN = NBIOT_CLEAR_STORED_EARFCN_TRUE;
		/* Init Message Index */
		OneNETSendMessageIndex = NET_OneNET_Message_SendDataRear();
		/* Get ConnectTime & IdleTime */
		ONENET_NBIOT_GetConnectTime(pClient, false);
		ONENET_NBIOT_GetIdleTime(pClient, false);
	}
	
	if (Stm32_Calculagraph_IsExpiredSec(&pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateRunTime) == true) {
		/* Dictate TimeOut */
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEnable = false;
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		pClient->ProcessState = ONENET_PROCESSSTATE_INIT;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_ONENET;
	}
	else {
		/* Dictate isn't TimeOut */
		if (NET_OneNET_Message_SendDataRear() != OneNETSendMessageIndex) {
			/* Have new coap message need to Send */
			pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEnable = false;
			pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->ProcessState = ONENET_PROCESSSTATE_INIT;
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_ONENET;
		}
		else {
			pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = STOP_MODE;
			pClient->ProcessState = ONENET_PROCESSSTATE_INIT;
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_ONENET;
		}
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_NBIOT_Event_HardwareReboot(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_HardwareReboot	: 硬件重启
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_HardwareReboot(ONENET_ClientsTypeDef* pClient)
{
	ONENET_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (NBIOT_Neul_NBxx_HardwareReboot(pClient->LWM2MStack->NBIotStack, 8000) == NBIOT_OK) {
		/* Dictate execute is Success */
#if NBIOT_PRINT_ERROR_CODE_TYPE
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, REPORT_ERROE, HARDWARE_REBOOT);
#else
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, MODULE_CHECK, HARDWARE_REBOOT);
#endif
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		ONENET_DEBUG_LOG_PRINTF("NB HDRBT Ok, Baud:%d", NBIOTBaudRate.Baud);
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, HARDWARE_REBOOT);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		ONENET_DEBUG_LOG_PRINTF("NB HDRBT Fail");
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_NBIOT_Event_ReportError(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_ReportError		: 错误码输出
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_ReportError(ONENET_ClientsTypeDef* pClient)
{
	ONENET_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (NBIOT_Neul_NBxx_SetReportTerminationError(pClient->LWM2MStack->NBIotStack, CMEEnable) == NBIOT_OK) {
		/* Dictate execute is Success */
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, MODULE_CHECK, REPORT_ERROE);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		ONENET_DEBUG_LOG_PRINTF("NB ReportECde Set %d Ok", CMEEnable);
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, REPORT_ERROE);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		ONENET_DEBUG_LOG_PRINTF("NB ReportECde Set %d Fail", CMEEnable);
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_NBIOT_Event_ModuleCheck(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_ModuleCheck		: 模块检测
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_ModuleCheck(ONENET_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	ONENET_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (((NBStatus = NBIOT_Neul_NBxx_CheckReadManufacturer(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadManufacturerModel(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) &&
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadModuleVersion(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK)) {
		/* Dictate execute is Success */
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, PARAMETER_CONFIG, MODULE_CHECK);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		ONENET_DEBUG_LOG_PRINTF("NB Module Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, MODULE_CHECK);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		ONENET_DEBUG_LOG_PRINTF("NB Module Check Fail ECde %d", NBStatus);
	#else
		ONENET_DEBUG_LOG_PRINTF("NB Module Check Fail");
	#endif
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_NBIOT_Event_ParameterConfig(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_ParameterConfig	: 参数配置
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_ParameterConfig(ONENET_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	ONENET_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadConfigUE(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		ONENET_DEBUG_LOG_PRINTF("NB Para Config Read Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		ONENET_DEBUG_LOG_PRINTF("NB Para Config Read Fail ECde %d", NBStatus);
	#else
		ONENET_DEBUG_LOG_PRINTF("NB Para Config Read Fail");
	#endif
#endif
		return;
	}
	
	if (pClient->LWM2MStack->NBIotStack->Parameter.nconfig.autoConnect != AutoConnectVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->LWM2MStack->NBIotStack, AutoConnect, AutoConnectVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			ONENET_DEBUG_LOG_PRINTF("NB %s %d Ok", AutoConnect, AutoConnectVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			ONENET_DEBUG_LOG_PRINTF("NB %s %d Fail", AutoConnect, AutoConnectVal);
#endif
			return;
		}
	}
	
	if (pClient->LWM2MStack->NBIotStack->Parameter.nconfig.crScrambling != CrScramblingVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->LWM2MStack->NBIotStack, CrScrambling, CrScramblingVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			ONENET_DEBUG_LOG_PRINTF("NB %s %d Ok", CrScrambling, CrScramblingVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			ONENET_DEBUG_LOG_PRINTF("NB %s %d Fail", CrScrambling, CrScramblingVal);
#endif
			return;
		}
	}
	
	if (pClient->LWM2MStack->NBIotStack->Parameter.nconfig.crSiAvoid != CrSiAvoidVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->LWM2MStack->NBIotStack, CrSiAvoid, CrSiAvoidVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			ONENET_DEBUG_LOG_PRINTF("NB %s %d Ok", CrSiAvoid, CrSiAvoidVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			ONENET_DEBUG_LOG_PRINTF("NB %s %d Fail", CrSiAvoid, CrSiAvoidVal);
#endif
			return;
		}
	}
	
	if (pClient->LWM2MStack->NBIotStack->Parameter.nconfig.combineAttach != CombineAttachVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->LWM2MStack->NBIotStack, CombineAttach, CombineAttachVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			ONENET_DEBUG_LOG_PRINTF("NB %s %d Ok", CombineAttach, CombineAttachVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			ONENET_DEBUG_LOG_PRINTF("NB %s %d Fail", CombineAttach, CombineAttachVal);
#endif
			return;
		}
	}
	
	if (pClient->LWM2MStack->NBIotStack->Parameter.nconfig.cellReselection != CellReselectionVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->LWM2MStack->NBIotStack, CellReselection, CellReselectionVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			ONENET_DEBUG_LOG_PRINTF("NB %s %d Ok", CellReselection, CellReselectionVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			ONENET_DEBUG_LOG_PRINTF("NB %s %d Fail", CellReselection, CellReselectionVal);
#endif
			return;
		}
	}
	
	if (pClient->LWM2MStack->NBIotStack->Parameter.nconfig.enableBip != EnableBipVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->LWM2MStack->NBIotStack, EnableBip, EnableBipVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			ONENET_DEBUG_LOG_PRINTF("NB %s %d Ok", EnableBip, EnableBipVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			ONENET_DEBUG_LOG_PRINTF("NB %s %d Fail", EnableBip, EnableBipVal);
#endif
			return;
		}
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_NBIOT_Event_SimICCIDCheck(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_SimICCIDCheck	: Sim卡检测
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_SimICCIDCheck(ONENET_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	ONENET_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadICCID(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, FULL_FUNCTIONALITY, ICCID_CHECK);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		ONENET_DEBUG_LOG_PRINTF("NB ICCID Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ICCID_CHECK);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		ONENET_DEBUG_LOG_PRINTF("NB ICCID Fail ECde %d", NBStatus);
	#else
		ONENET_DEBUG_LOG_PRINTF("NB ICCID Fail");
	#endif
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_NBIOT_Event_FullFunctionality(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_FullFunctionality: 完整功能
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_FullFunctionality(ONENET_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	ONENET_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadMinOrFullFunc(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
#if NBIOT_CLEAR_STORED_EARFCN_STAT
		if (pClient->LWM2MStack->NBIotStack->ClearStoredEARFCN != NBIOT_CLEAR_STORED_EARFCN_FALSE) {
			/* 需清除频点 */
			ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, MINIMUM_FUNCTIONALITY, FULL_FUNCTIONALITY);
		}
		else {
			/* 无需清除频点 */
			ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CHECK, FULL_FUNCTIONALITY);
		}
#else
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CHECK, FULL_FUNCTIONALITY);
#endif
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		ONENET_DEBUG_LOG_PRINTF("NB FullFunc Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, FULL_FUNCTIONALITY);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		ONENET_DEBUG_LOG_PRINTF("NB FullFunc Check Fail ECde %d", NBStatus);
	#else
		ONENET_DEBUG_LOG_PRINTF("NB FullFunc Check Fail");
	#endif
#endif
		return;
	}
	
	if (pClient->LWM2MStack->NBIotStack->Parameter.functionality != FullFunc) {
		if ((NBStatus = NBIOT_Neul_NBxx_SetMinOrFullFunc(pClient->LWM2MStack->NBIotStack, FullFunc)) == NBIOT_OK) {
			/* Dictate execute is Success */
#if NBIOT_CLEAR_STORED_EARFCN_STAT
			if (pClient->LWM2MStack->NBIotStack->ClearStoredEARFCN != NBIOT_CLEAR_STORED_EARFCN_FALSE) {
				/* 需清除频点 */
				ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, MINIMUM_FUNCTIONALITY, FULL_FUNCTIONALITY);
			}
			else {
				/* 无需清除频点 */
				ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CHECK, FULL_FUNCTIONALITY);
			}
#else
			ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CHECK, FULL_FUNCTIONALITY);
#endif
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			ONENET_DEBUG_LOG_PRINTF("NB FullFunc Set Ok");
#endif
		}
		else {
			/* Dictate execute is Fail */
			ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, FULL_FUNCTIONALITY);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			ONENET_DEBUG_LOG_PRINTF("NB FullFunc Set Fail ECde %d", NBStatus);
		#else
			ONENET_DEBUG_LOG_PRINTF("NB FullFunc Set Fail");
		#endif
#endif
			return;
		}
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_NBIOT_Event_MinimumFunctionality(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_MinimumFunctionality	: 最小功能
 @Input				pClient								: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_MinimumFunctionality(ONENET_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	ONENET_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadMinOrFullFunc(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
#if NBIOT_CLEAR_STORED_EARFCN_STAT
		if (pClient->LWM2MStack->NBIotStack->ClearStoredEARFCN != NBIOT_CLEAR_STORED_EARFCN_FALSE) {
			/* 需清除频点 */
			ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, CLEAR_STORED_EARFCN, MINIMUM_FUNCTIONALITY);
		}
		else {
			/* 无需清除频点 */
			ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CONFIG, MINIMUM_FUNCTIONALITY);
		}
#else
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CONFIG, MINIMUM_FUNCTIONALITY);
#endif
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		ONENET_DEBUG_LOG_PRINTF("NB MinFunc Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, MINIMUM_FUNCTIONALITY);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		ONENET_DEBUG_LOG_PRINTF("NB MinFunc Check Fail ECde %d", NBStatus);
	#else
		ONENET_DEBUG_LOG_PRINTF("NB MinFunc Check Fail");
	#endif
#endif
		return;
	}
	
	if (pClient->LWM2MStack->NBIotStack->Parameter.functionality != MinFunc) {
		if ((NBStatus = NBIOT_Neul_NBxx_SetMinOrFullFunc(pClient->LWM2MStack->NBIotStack, MinFunc)) == NBIOT_OK) {
			/* Dictate execute is Success */
#if NBIOT_CLEAR_STORED_EARFCN_STAT
			if (pClient->LWM2MStack->NBIotStack->ClearStoredEARFCN != NBIOT_CLEAR_STORED_EARFCN_FALSE) {
				/* 需清除频点 */
				ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, CLEAR_STORED_EARFCN, MINIMUM_FUNCTIONALITY);
			}
			else {
				/* 无需清除频点 */
				ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CONFIG, MINIMUM_FUNCTIONALITY);
			}
#else
			ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CONFIG, MINIMUM_FUNCTIONALITY);
#endif
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			ONENET_DEBUG_LOG_PRINTF("NB MinFunc Set Ok");
#endif
		}
		else {
			/* Dictate execute is Fail */
			ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, MINIMUM_FUNCTIONALITY);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			ONENET_DEBUG_LOG_PRINTF("NB MinFunc Set Fail ECde %d", NBStatus);
		#else
			ONENET_DEBUG_LOG_PRINTF("NB MinFunc Set Fail");
		#endif
#endif
			return;
		}
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_NBIOT_Event_ClearStoredEARFCN(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_ClearStoredEARFCN: 清除小区频点
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_ClearStoredEARFCN(ONENET_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	ONENET_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_ClearStoredEarfcn(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		pClient->LWM2MStack->NBIotStack->ClearStoredEARFCN = NBIOT_CLEAR_STORED_EARFCN_FALSE;
		
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, FULL_FUNCTIONALITY, CLEAR_STORED_EARFCN);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		ONENET_DEBUG_LOG_PRINTF("NB Clear Stored EARFCN OK");
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, CLEAR_STORED_EARFCN);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		ONENET_DEBUG_LOG_PRINTF("NB Clear Stored EARFCN Fail Ecde %d", NBStatus);
	#else
		ONENET_DEBUG_LOG_PRINTF("NB Clear Stored EARFCN Fail");
	#endif
#endif
		return;
	}
}

static bool NET_ONENET_CheckNband(ONENET_ClientsTypeDef* pClient, NBIOT_NBandTypeDef nbandsrc, NBIOT_NBandTypeDef nbanddst)
{
	if (nbandsrc.NBandNum != nbanddst.NBandNum) {
		return false;
	}
	
	for (int index = 0; index < nbanddst.NBandNum; index++) {
		if (nbandsrc.NBandVal[index] != nbanddst.NBandVal[index]) {
			return false;
		}
	}
	
	return true;
}

/**********************************************************************************************************
 @Function			void NET_ONENET_NBIOT_Event_NbandModeCheck(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_NbandModeCheck	: NBAND模式查询
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_NbandModeCheck(ONENET_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	ONENET_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadSupportedBands(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CHECK, NBAND_MODE_CHECK);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		ONENET_DEBUG_LOG_PRINTF("NB BAND Read %d:%d.%d.%d Ok", pClient->LWM2MStack->NBIotStack->Parameter.band.NBandNum, \
														 pClient->LWM2MStack->NBIotStack->Parameter.band.NBandVal[0], \
														 pClient->LWM2MStack->NBIotStack->Parameter.band.NBandVal[1], \
														 pClient->LWM2MStack->NBIotStack->Parameter.band.NBandVal[2]);
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, NBAND_MODE_CHECK);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		ONENET_DEBUG_LOG_PRINTF("NB BAND Read Fail ECde %d", NBStatus);
	#else
		ONENET_DEBUG_LOG_PRINTF("NB BAND Read Fail");
	#endif
#endif
	}
	
	if (NET_ONENET_CheckNband(pClient, pClient->LWM2MStack->NBIotStack->Parameter.band, ONENET_NBIOT_BAND_TYPE) != true) {
		/* BAND Mode Mast be Config */
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = MINIMUM_FUNCTIONALITY;
	}
	else {
		/* BAND Mode Needn't be Config */
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = ATTACH_CHECK;
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_NBIOT_Event_NbandModeConfig(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_NbandModeConfig	: NBAND模式配置
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_NbandModeConfig(ONENET_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	ONENET_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadSupportedBands(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, FULL_FUNCTIONALITY, NBAND_MODE_CONFIG);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		ONENET_DEBUG_LOG_PRINTF("NB BAND Read Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, NBAND_MODE_CONFIG);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		ONENET_DEBUG_LOG_PRINTF("NB BAND Read Fail ECde %d", NBStatus);
	#else
		ONENET_DEBUG_LOG_PRINTF("NB BAND Read Fail");
	#endif
#endif
	}
	
	if (NET_ONENET_CheckNband(pClient, pClient->LWM2MStack->NBIotStack->Parameter.band, ONENET_NBIOT_BAND_TYPE) != true) {
		/* BAND Mode Mast be Config */
		if ((NBStatus = NBIOT_Neul_NBxx_SetSupportedBands(pClient->LWM2MStack->NBIotStack, ONENET_NBIOT_BAND_TYPE)) == NBIOT_OK) {
			/* Dictate execute is Success */
			ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, FULL_FUNCTIONALITY, NBAND_MODE_CONFIG);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			ONENET_DEBUG_LOG_PRINTF("NB BAND Set %d:%d.%d.%d Ok", ONENET_NBIOT_BAND_TYPE.NBandNum, \
															ONENET_NBIOT_BAND_TYPE.NBandVal[0], \
															ONENET_NBIOT_BAND_TYPE.NBandVal[1], \
															ONENET_NBIOT_BAND_TYPE.NBandVal[2]);
#endif
		}
		else {
			/* Dictate execute is Fail */
			ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, NBAND_MODE_CONFIG);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			ONENET_DEBUG_LOG_PRINTF("NB BAND Set Fail ECde %d", NBStatus);
		#else
			ONENET_DEBUG_LOG_PRINTF("NB BAND Set Fail");
		#endif
#endif
			return;
		}
	}
	else {
		/* BAND Mode Needn't be Config */
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = FULL_FUNCTIONALITY;
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_NBIOT_Event_AttachCheck(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_AttachCheck		: 注网检查
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_AttachCheck(ONENET_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	ONENET_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadAttachOrDetach(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, ATTACH_CHECK, ATTACH_CHECK);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		ONENET_DEBUG_LOG_PRINTF("NB CGATT %d Ok", pClient->LWM2MStack->NBIotStack->Parameter.netstate);
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_CHECK);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		ONENET_DEBUG_LOG_PRINTF("NB CGATT %d Fail ECde %d", pClient->LWM2MStack->NBIotStack->Parameter.netstate, NBStatus);
	#else
		ONENET_DEBUG_LOG_PRINTF("NB CGATT %d Fail", pClient->LWM2MStack->NBIotStack->Parameter.netstate);
	#endif
#endif
		return;
	}
	
	if (pClient->LWM2MStack->NBIotStack->Parameter.netstate != Attach) {
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = ATTACH_EXECUTE;
	}
	else {
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = ATTACH_INQUIRE;
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_NBIOT_Event_AttachExecute(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_AttachExecute	: 注网进行
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_AttachExecute(ONENET_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	ONENET_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_SetAttachOrDetach(pClient->LWM2MStack->NBIotStack, Attach)) == NBIOT_OK) {
		/* Dictate execute is Success */
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, ATTACH_INQUIRE, ATTACH_EXECUTE);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		ONENET_DEBUG_LOG_PRINTF("NB Set CGATT %d Ok", Attach);
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_EXECUTE);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		ONENET_DEBUG_LOG_PRINTF("NB Set CGATT %d Fail ECde %d", Attach, NBStatus);
	#else
		ONENET_DEBUG_LOG_PRINTF("NB Set CGATT %d Fail", Attach);
	#endif
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_NBIOT_Event_AttachInquire(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_AttachInquire	: 注网查询
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_AttachInquire(ONENET_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	ONENET_NBIOT_DictateEvent_SetTime(pClient, ONENET_NBIOT_CGATT_TIME_S);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadAttachOrDetach(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = ATTACH_INQUIRE;
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		ONENET_DEBUG_LOG_PRINTF("NB CGATT %d Ok", pClient->LWM2MStack->NBIotStack->Parameter.netstate);
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_INQUIRE);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		ONENET_DEBUG_LOG_PRINTF("NB CGATT %d Fail ECde %d", pClient->LWM2MStack->NBIotStack->Parameter.netstate, NBStatus);
	#else
		ONENET_DEBUG_LOG_PRINTF("NB CGATT %d Fail", pClient->LWM2MStack->NBIotStack->Parameter.netstate);
	#endif
#endif
		return;
	}
	
	if (pClient->LWM2MStack->NBIotStack->Parameter.netstate != Attach) {
		/* 未注网 */
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_INQUIRE);
		
		/* 开启频点清除 */
		pClient->LWM2MStack->NBIotStack->ClearStoredEARFCN = NBIOT_CLEAR_STORED_EARFCN_TRUE;
	}
	else {
		/* 注网成功 */
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, PARAMETER_CHECKOUT, ATTACH_INQUIRE);
		
		/* 关闭频点清除 */
		pClient->LWM2MStack->NBIotStack->ClearStoredEARFCN = NBIOT_CLEAR_STORED_EARFCN_FALSE;
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_NBIOT_Event_ParameterCheckOut(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_ParameterCheckOut: 参数检出
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_ParameterCheckOut(ONENET_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
#if NBIOT_STATION_TIMESTAMP_TYPE
	unsigned int devicetimestamp = 0;
	unsigned int stationtimestamp = 0;
#endif
	
	ONENET_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (((NBStatus = NBIOT_Neul_NBxx_CheckReadIMEI(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadIMSI(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadCGPADDR(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadPDPContext(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadRSSI(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadStatisticsRADIO(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadAreaCode(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadDateTime(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK)) {
		/* Dictate execute is Success */
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, ONENET_PROCESS_STACK, PARAMETER_CHECKOUT);
		
		pClient->LWM2MStack->NBIotStack->Registered = true;
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		ONENET_DEBUG_LOG_PRINTF("NB Para Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CHECKOUT);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		ONENET_DEBUG_LOG_PRINTF("NB Para Check Fail ECde %d", NBStatus);
	#else
		ONENET_DEBUG_LOG_PRINTF("NB Para Check Fail");
	#endif
#endif
		return;
	}
	
#if NBIOT_STATION_TIMESTAMP_TYPE
	devicetimestamp = RTC_GetUnixTimeToStamp() + (8 * 60 * 60);
	stationtimestamp = RTC_TimeToStamp(pClient->LWM2MStack->NBIotStack->Parameter.dataTime.year, pClient->LWM2MStack->NBIotStack->Parameter.dataTime.month, pClient->LWM2MStack->NBIotStack->Parameter.dataTime.day, \
								pClient->LWM2MStack->NBIotStack->Parameter.dataTime.hour, pClient->LWM2MStack->NBIotStack->Parameter.dataTime.min,   pClient->LWM2MStack->NBIotStack->Parameter.dataTime.sec);
	
	if ((((stationtimestamp >= devicetimestamp) ? (stationtimestamp - devicetimestamp) : (devicetimestamp - stationtimestamp)) > (NBIOT_STATION_TIMESTAMP_DIFFTIME)) && (pClient->LWM2MStack->NBIotStack->Parameter.dataTime.errcount < NBIOT_STATION_TIMESTAMP_ERRCOUNT) && (BackUp != true)) {
		pClient->LWM2MStack->NBIotStack->Parameter.dataTime.errcount++;
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, HARDWARE_REBOOT, PARAMETER_CHECKOUT);
		return;
	}
	
	pClient->LWM2MStack->NBIotStack->Parameter.dataTime.errcount = 0;
#endif
	
	/* Set System Time */
	RTC_Set_Date(pClient->LWM2MStack->NBIotStack->Parameter.dataTime.year, pClient->LWM2MStack->NBIotStack->Parameter.dataTime.month, pClient->LWM2MStack->NBIotStack->Parameter.dataTime.day);
	RTC_Set_Time(pClient->LWM2MStack->NBIotStack->Parameter.dataTime.hour, pClient->LWM2MStack->NBIotStack->Parameter.dataTime.min,   pClient->LWM2MStack->NBIotStack->Parameter.dataTime.sec);
}

/**********************************************************************************************************
 @Function			void NET_ONENET_Event_Init(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_Event_Init				: INIT
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_Event_Init(ONENET_ClientsTypeDef* pClient)
{
	ONENET_StatusTypeDef ONStatus = ONStatus;
	
	ONENET_DictateEvent_SetTime(pClient, 30);
	
	/* Access Configuration LwM2MServer AckTimeout and ObserveAutoack */
	if (((ONStatus = NBIOT_OneNET_Related_AccessConfig_LwM2MServer(pClient, ONENET_LWM2MSERVER_MODE, (sc8*)ONENET_LWM2MSERVER_ADDR, ONENET_LWM2MSERVER_PORT)) == ONENET_OK) && 
	    ((ONStatus = NBIOT_OneNET_Related_AccessConfig_AckTimeout(pClient, ONENET_LWM2MSERVER_ACKTIMEOUT)) == ONENET_OK) && 
	    ((ONStatus = NBIOT_OneNET_Related_AccessConfig_ObserveAutoack(pClient, ONENET_OBSAUTOACK_TYPE)) == ONENET_OK)) {
		/* Dictate execute is Success */
		ONENET_DictateEvent_SuccessExecute(pClient, ONENET_PROCESS_STACK, ONENET_PROCESSSTATE_SUITE, ONENET_PROCESSSTATE_INIT, true);
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		ONENET_DEBUG_LOG_PRINTF("OneNET Access Config Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, ONENET_PROCESSSTATE_INIT, ONENET_PROCESSSTATE_INIT);
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		ONENET_DEBUG_LOG_PRINTF("OneNET Access Config Fail ECde %d", ONStatus);
	#else
		ONENET_DEBUG_LOG_PRINTF("OneNET Access Config Fail");
	#endif
#endif
		return;
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_Event_Suite(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_Event_Suite				: SUITE
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_Event_Suite(ONENET_ClientsTypeDef* pClient)
{
	ONENET_StatusTypeDef ONStatus = ONStatus;
	
	ONENET_DictateEvent_SetTime(pClient, 30);
	
	/* Create SuiteInstance and Check Read SuiteVersion */
	if (((ONStatus = NBIOT_OneNET_Related_Create_SuiteInstance(pClient, &pClient->Parameter.suiteRefer)) == ONENET_OK) && 
	    ((ONStatus = NBIOT_OneNET_Related_CheckRead_SuiteVersion(pClient)) == ONENET_OK)) {
		/* Dictate execute is Success */
		ONENET_DictateEvent_SuccessExecute(pClient, ONENET_PROCESS_STACK, ONENET_PROCESSSTATE_OBJECT, ONENET_PROCESSSTATE_SUITE, true);
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		ONENET_DEBUG_LOG_PRINTF("OneNET Suite Create %d.%s Ok", pClient->Parameter.suiteRefer, pClient->Parameter.suiteVersion);
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, ONENET_PROCESSSTATE_INIT, ONENET_PROCESSSTATE_SUITE);
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		ONENET_DEBUG_LOG_PRINTF("OneNET Suite Create Fail ECde %d", ONStatus);
	#else
		ONENET_DEBUG_LOG_PRINTF("OneNET Suite Create Fail");
	#endif
#endif
		return;
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_Event_Object(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_Event_Object				: OBJECT
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_Event_Object(ONENET_ClientsTypeDef* pClient)
{
	ONENET_StatusTypeDef ONStatus = ONStatus;
	
	ONENET_DictateEvent_SetTime(pClient, 30);
	
	/* Add LwM2M Object */
	if ((ONStatus = NBIOT_OneNET_Related_Add_LwM2MObject(pClient, pClient->Parameter.suiteRefer, \
													  pClient->Parameter.objectInfo.objId, \
													  pClient->Parameter.objectInfo.insCount, \
													  pClient->Parameter.objectInfo.insBitmap, \
													  pClient->Parameter.objectInfo.attrCount, \
													  pClient->Parameter.objectInfo.actCount)) == ONENET_OK) {
		/* Dictate execute is Success */
		ONENET_DictateEvent_SuccessExecute(pClient, ONENET_PROCESS_STACK, ONENET_PROCESSSTATE_REGISTER, ONENET_PROCESSSTATE_OBJECT, true);
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		ONENET_DEBUG_LOG_PRINTF("OneNET LMObject Add Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, ONENET_PROCESSSTATE_INIT, ONENET_PROCESSSTATE_OBJECT);
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		ONENET_DEBUG_LOG_PRINTF("OneNET LMObject Add Fail ECde %d", ONStatus);
	#else
		ONENET_DEBUG_LOG_PRINTF("OneNET LMObject Add Fail");
	#endif
#endif
		return;
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_Event_Register(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_Event_Register			: REGISTER
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_Event_Register(ONENET_ClientsTypeDef* pClient)
{
	ONENET_StatusTypeDef ONStatus = ONStatus;
	Stm32_CalculagraphTypeDef WaitforRecv_timer_s;
	
	ONENET_DictateEvent_SetTime(pClient, ONENET_REGISTER_TIMEOUT + 20);
	
	/* Configuration Calculagraph for WaitforRecv Timer */
	Stm32_Calculagraph_CountdownSec(&WaitforRecv_timer_s, ONENET_REGISTER_TIMEOUT);
	
	/* Clean observeInfo */
	for (int index = 0; index < ONENET_OBJECT_INSCOUNT; index++) {
		pClient->Parameter.observeInfo[index].ref		= 0;
		pClient->Parameter.observeInfo[index].msgId		= 0;
		pClient->Parameter.observeInfo[index].flag		= 0;
		pClient->Parameter.observeInfo[index].objId		= 0;
		pClient->Parameter.observeInfo[index].insId		= 0;
		pClient->Parameter.observeInfo[index].resId		= 0;
	}
	
	/* Send Register Request */
	if ((ONStatus = NBIOT_OneNET_Related_Send_RegisterRequest(pClient, pClient->Parameter.suiteRefer, ONENET_REGISTER_LIFETIME + 2 * 3600, ONENET_REGISTER_TIMEOUT + 20)) == ONENET_OK) {
		/* Dictate execute is Success */
		ONENET_DictateEvent_SuccessExecute(pClient, ONENET_PROCESS_STACK, ONENET_PROCESSSTATE_REGISTER, ONENET_PROCESSSTATE_REGISTER, false);
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		ONENET_DEBUG_LOG_PRINTF("OneNET Register Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, ONENET_PROCESSSTATE_INIT, ONENET_PROCESSSTATE_REGISTER);
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		ONENET_DEBUG_LOG_PRINTF("OneNET Register Fail ECde %d", ONStatus);
	#else
		ONENET_DEBUG_LOG_PRINTF("OneNET Register Fail");
	#endif
#endif
		return;
	}
	
	/* This will be a blocking call, wait for the ONENET_MIPLDISCOVER */
	if ((ONStatus = ONENET_WaitforRecvAck(pClient, ONENET_MIPLDISCOVER, &WaitforRecv_timer_s)) == ONENET_OK) {
		/* Dictate execute is Success */
		ONENET_DictateEvent_SuccessExecute(pClient, ONENET_PROCESS_STACK, ONENET_PROCESSSTATE_REGISTER, ONENET_PROCESSSTATE_REGISTER, false);
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		ONENET_DEBUG_LOG_PRINTF("OneNET Discover %d Ok", pClient->Parameter.discoverInfo.msgId);
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, ONENET_PROCESSSTATE_INIT, ONENET_PROCESSSTATE_REGISTER);
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		ONENET_DEBUG_LOG_PRINTF("OneNET Discover Fail ECde %d", ONStatus);
	#else
		ONENET_DEBUG_LOG_PRINTF("OneNET Discover Fail");
	#endif
#endif
		return;
	}
	
	/* Check observeInfo */
	for (int index = 0; index < ONENET_OBJECT_INSCOUNT; index++) {
		if (pClient->Parameter.observeInfo[index].msgId == 0) {
			/* Dictate execute is Fail */
			ONENET_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, ONENET_PROCESSSTATE_INIT, ONENET_PROCESSSTATE_REGISTER);
			return;
		}
	}
	
	/* Send Discover Request */
	if ((ONStatus = NBIOT_OneNET_Related_Respond_DiscoverRequest(pClient, pClient->Parameter.suiteRefer, \
															pClient->Parameter.discoverInfo.msgId, \
															ONENET_Content, \
															ONENET_DISCOVER_LENGTH, \
															(sc8*)ONENET_DISCOVER_VALSTR, \
															NULL)) == ONENET_OK) {
		/* Dictate execute is Success */
		ONENET_DictateEvent_SuccessExecute(pClient, ONENET_PROCESS_STACK, ONENET_PROCESSSTATE_ACTIVE, ONENET_PROCESSSTATE_REGISTER, true);
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		ONENET_DEBUG_LOG_PRINTF("OneNET Discover Rsp Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, ONENET_PROCESSSTATE_INIT, ONENET_PROCESSSTATE_REGISTER);
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		ONENET_DEBUG_LOG_PRINTF("OneNET Discover Rsp Fail ECde %d", ONStatus);
	#else
		ONENET_DEBUG_LOG_PRINTF("OneNET Discover Rsp Fail");
	#endif
#endif
		return;
	}
	
	/* Set Update Duration */
	ONENET_NormalDictateEvent_SetTime(pClient, &pClient->UpdateTimer, ONENET_REGISTER_LIFETIME);
	/* Set Active Duration */
	ONENET_NormalDictateEvent_SetTime(pClient, &pClient->ActiveTimer, 40);
}

/**********************************************************************************************************
 @Function			void NET_ONENET_Event_Active(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_Event_Active				: ACTIVE
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_Event_Active(ONENET_ClientsTypeDef* pClient)
{
	ONENET_StatusTypeDef ONStatus = ONStatus;
	ONENET_PacketPrivateTypeDef* pMsg = (ONENET_PacketPrivateTypeDef*)pClient->Sendbuf;
	ONENET_ObserveParaTypeDef observeInfo;
	Stm32_CalculagraphTypeDef WaitforRecv_timer_s;
	
	ONENET_DictateEvent_SetTime(pClient, 60);
	
	/* Configuration Calculagraph for WaitforRecv Timer */
	Stm32_Calculagraph_CountdownSec(&WaitforRecv_timer_s, 20);
	
	/* Data packets need to be sent*/
	if (NET_OneNET_Message_SendDataDequeue(pClient->Sendbuf, (unsigned short *)&pClient->Sendlen) == true) {
		/* 判断数据包类型 */
		if ((pMsg->MsgPacket.Type == ONENET_MSGTYPE_TYPE_SHORT_STATUS) || (pMsg->MsgPacket.Type == ONENET_MSGTYPE_TYPE_LONG_STATUS)) {
			observeInfo = pClient->Parameter.observeInfo[0];
		}
		else {
			observeInfo = pClient->Parameter.observeInfo[1];
		}
		
		/* 发送负载数据 */
		if ((ONStatus = pClient->LWM2MStack->Write(pClient, observeInfo, (const char *)pClient->Sendbuf, pClient->Sendlen, 255)) == ONENET_OK) {
			/* Dictate execute is Success */
			ONENET_DictateEvent_SuccessExecute(pClient, ONENET_PROCESS_STACK, ONENET_PROCESSSTATE_ACTIVE, ONENET_PROCESSSTATE_ACTIVE, false);
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			ONENET_DEBUG_LOG_PRINTF("OneNET Send Payload Ok");
#endif
		}
		else {
			/* Dictate execute is Fail */
			ONENET_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, ONENET_PROCESSSTATE_INIT, ONENET_PROCESSSTATE_ACTIVE);
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
			ONENET_DEBUG_LOG_PRINTF("OneNET Send Payload Fail ECde %d", ONStatus);
	#else
			ONENET_DEBUG_LOG_PRINTF("OneNET Send Payload Fail");
	#endif
#endif
			return;
		}
		
		/* This will be a blocking call, wait for the ONENET_MIPLEVENT */
		if ((ONStatus = ONENET_WaitforRecvAck(pClient, ONENET_MIPLEVENT, &WaitforRecv_timer_s)) == ONENET_OK) {
			/* Dictate execute is Success */
			if ((pClient->Parameter.eventInfo.ackid == 255) && (pClient->Parameter.eventInfo.evtid == EVENT_NOTIFY_SUCCESS)) {
				ONENET_DictateEvent_SuccessExecute(pClient, ONENET_PROCESS_STACK, ONENET_PROCESSSTATE_ACTIVE, ONENET_PROCESSSTATE_ACTIVE, true);
				NET_OneNET_Message_SendDataOffSet();
				/* Set Active Duration */
				ONENET_NormalDictateEvent_SetTime(pClient, &pClient->ActiveTimer, 40);
#ifdef ONENET_DEBUG_LOG_RF_PRINT
				ONENET_DEBUG_LOG_PRINTF("OneNET Ackid %d Ok", pClient->Parameter.eventInfo.ackid);
#endif
			}
			else {
				ONENET_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, ONENET_PROCESSSTATE_INIT, ONENET_PROCESSSTATE_ACTIVE);
#ifdef ONENET_DEBUG_LOG_RF_PRINT
				ONENET_DEBUG_LOG_PRINTF("OneNET Ackid %d Fail", pClient->Parameter.eventInfo.ackid);
#endif
				return;
			}
		}
		else {
			/* Dictate execute is Fail */
			ONENET_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, ONENET_PROCESSSTATE_INIT, ONENET_PROCESSSTATE_ACTIVE);
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
			ONENET_DEBUG_LOG_PRINTF("OneNET Ackid Fail ECde %d", ONStatus);
	#else
			ONENET_DEBUG_LOG_PRINTF("OneNET Ackid Fail");
	#endif
#endif
			return;
		}
		
		/* Set Update Duration */
		ONENET_NormalDictateEvent_SetTime(pClient, &pClient->UpdateTimer, ONENET_REGISTER_LIFETIME);
		
		return;
	}
	
	/* Keep active for Active seconds before to Sleep, so we can send messsage contiguously */
	if (Stm32_Calculagraph_IsExpiredSec(&pClient->ActiveTimer) == true) {
		/* Arrival time for Sleep */
		ONENET_DictateEvent_SuccessExecute(pClient, ONENET_PROCESS_STACK, ONENET_PROCESSSTATE_SLEEP, ONENET_PROCESSSTATE_ACTIVE, true);
	}
	else {
		/* Keep Active */
		//Todo
		ONENET_DictateEvent_SuccessExecute(pClient, ONENET_PROCESS_STACK, ONENET_PROCESSSTATE_ACTIVE, ONENET_PROCESSSTATE_ACTIVE, false);
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_Event_Sleep(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_Event_Sleep				: SLEEP
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_Event_Sleep(ONENET_ClientsTypeDef* pClient)
{
	ONENET_StatusTypeDef ONStatus = ONStatus;
	
	if (NET_OneNET_Message_SendDataisEmpty() != true) {
		/* Set Active Duration */
		ONENET_NormalDictateEvent_SetTime(pClient, &pClient->ActiveTimer, 40);
		ONENET_DictateEvent_SuccessExecute(pClient, ONENET_PROCESS_STACK, ONENET_PROCESSSTATE_ACTIVE, ONENET_PROCESSSTATE_SLEEP, true);
		return;
	}
	
	/* If time to Aweak, then send a pingreq */
	if (Stm32_Calculagraph_IsExpiredSec(&pClient->UpdateTimer) == true) {
		ONENET_DictateEvent_SuccessExecute(pClient, ONENET_PROCESS_STACK, ONENET_PROCESSSTATE_AWEAK, ONENET_PROCESSSTATE_SLEEP, true);
	}
	else {
		ONENET_DictateEvent_SuccessExecute(pClient, ONENET_PROCESS_STACK, ONENET_PROCESSSTATE_SLEEP, ONENET_PROCESSSTATE_SLEEP, true);
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_Event_Aweak(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_Event_Aweak				: AWEAK
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_Event_Aweak(ONENET_ClientsTypeDef* pClient)
{
	ONENET_StatusTypeDef ONStatus = ONStatus;
	
	ONENET_DictateEvent_SetTime(pClient, 30);
	
	/* Send Update Request */
	if ((ONStatus = NBIOT_OneNET_Related_Send_UpdateRequest(pClient, pClient->Parameter.suiteRefer, ONENET_REGISTER_LIFETIME + 2 * 3600, 0, NULL)) == ONENET_OK) {
		/* Dictate execute is Success */
		ONENET_DictateEvent_SuccessExecute(pClient, ONENET_PROCESS_STACK, ONENET_PROCESSSTATE_AWEAK, ONENET_PROCESSSTATE_AWEAK, false);
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		ONENET_DEBUG_LOG_PRINTF("OneNET Update Request Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, ONENET_PROCESSSTATE_INIT, ONENET_PROCESSSTATE_AWEAK);
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		ONENET_DEBUG_LOG_PRINTF("OneNET Update Request Fail ECde %d", ONStatus);
	#else
		ONENET_DEBUG_LOG_PRINTF("OneNET Update Request Fail");
	#endif
#endif
		return;
	}
	
	/* Set Update Duration */
	ONENET_NormalDictateEvent_SetTime(pClient, &pClient->UpdateTimer, ONENET_REGISTER_LIFETIME);
	
	ONENET_DictateEvent_SuccessExecute(pClient, ONENET_PROCESS_STACK, ONENET_PROCESSSTATE_SLEEP, ONENET_PROCESSSTATE_AWEAK, true);
}

/**********************************************************************************************************
 @Function			void NET_ONENET_Event_Lost(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_Event_Lost				: LOST
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_Event_Lost(ONENET_ClientsTypeDef* pClient)
{
	ONENET_DictateEvent_SuccessExecute(pClient, STOP_MODE, ONENET_PROCESSSTATE_INIT, ONENET_PROCESSSTATE_LOST, true);
}

/********************************************** END OF FLEE **********************************************/
