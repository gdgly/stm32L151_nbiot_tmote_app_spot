/**
  *********************************************************************************************************
  * @file    net_mqttsn_app.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-03-12
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "net_mqttsn_app.h"
#include "platform_config.h"
#include "platform_map.h"
#include "hal_rtc.h"
#include "hal_beep.h"
#include "radar_api.h"
#include "hal_qmc5883l.h"
#include "string.h"
#include "radio_rf_app.h"

/**********************************************************************************************************
 @Function			void NET_MQTTSN_APP_PollExecution(MQTTSN_ClientsTypeDef* pClient)
 @Description			MQTTSN_APP_PollExecution				: MqttSN逻辑处理
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_APP_PollExecution(MQTTSN_ClientsTypeDef* pClient)
{
	switch (pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent)
	{
	case STOP_MODE:
		NET_MQTTSN_NBIOT_Event_StopMode(pClient);
		break;
		
	case HARDWARE_REBOOT:
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_DISABLE
		NET_MQTTSN_NBIOT_Event_HardwareReboot(pClient);
#endif
		break;
	
	case REPORT_ERROE:
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_DISABLE
		NET_MQTTSN_NBIOT_Event_ReportError(pClient);
#endif
		break;
	
	case MODULE_CHECK:
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_DISABLE
		NET_MQTTSN_NBIOT_Event_ModuleCheck(pClient);
#endif
		break;
	
	case PARAMETER_CONFIG:
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_DISABLE
		NET_MQTTSN_NBIOT_Event_ParameterConfig(pClient);
#endif
		break;
	
	case ICCID_CHECK:
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_DISABLE
		NET_MQTTSN_NBIOT_Event_SimICCIDCheck(pClient);
#endif
		break;
	
	case MISC_EQUIP_CONFIG:
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_DISABLE
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
#endif
		break;
	
	case ATTACH_CHECK:
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_DISABLE
		NET_MQTTSN_NBIOT_Event_AttachCheck(pClient);
#endif
		break;
	
	case ATTACH_EXECUTE:
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_DISABLE
		NET_MQTTSN_NBIOT_Event_AttachExecute(pClient);
#endif
		break;
	
	case ATTACH_INQUIRE:
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_DISABLE
		NET_MQTTSN_NBIOT_Event_AttachInquire(pClient);
#endif
		break;
	
	case PARAMETER_CHECKOUT:
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_DISABLE
		NET_MQTTSN_NBIOT_Event_ParameterCheckOut(pClient);
#endif
		break;
	
	case MINIMUM_FUNCTIONALITY:
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_DISABLE
		NET_MQTTSN_NBIOT_Event_MinimumFunctionality(pClient);
#endif
		break;
	
	case FULL_FUNCTIONALITY:
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_DISABLE
		NET_MQTTSN_NBIOT_Event_FullFunctionality(pClient);
#endif
		break;
	
	case CDP_SERVER_CHECK:
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_DISABLE
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
#endif
		break;
	
	case CDP_SERVER_CONFIG:
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_DISABLE
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
#endif
		break;
	
	case NBAND_MODE_CHECK:
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_DISABLE
		NET_MQTTSN_NBIOT_Event_NbandModeCheck(pClient);
#endif
		break;
	
	case NBAND_MODE_CONFIG:
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_DISABLE
		NET_MQTTSN_NBIOT_Event_NbandModeConfig(pClient);
#endif
		break;
	
	case SEND_DATA:
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case RECV_DATA:
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case SEND_DATA_RA_NORMAL:
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case RECV_DATA_RA_NORMAL:
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case EXECUT_DOWNLINK_DATA:
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case MQTTSN_PROCESS_STACK:
		NET_MQTTSN_APP_ProcessExecution(pClient);
		break;
	
	case DNS_PROCESS_STACK:
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case ONENET_PROCESS_STACK:
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case LISTEN_RUN_CTL:
		NET_MQTTSN_Listen_PollExecution(pClient);
		break;
	
	default :
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_APP_ProcessExecution(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_APP_ProcessExecution		: MqttSN协议逻辑处理
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_APP_ProcessExecution(MQTTSN_ClientsTypeDef* pClient)
{
	switch (pClient->SubState)
	{
	case MQTTSN_SUBSTATE_INIT:
		NET_MQTTSN_Event_Init(pClient);
		break;
	
	case MQTTSN_SUBSTATE_DISCONNECT:
		NET_MQTTSN_Event_Disconnect(pClient);
		break;
	
	case MQTTSN_SUBSTATE_ACTIVE:
		NET_MQTTSN_Event_Active(pClient);
		break;
	
	case MQTTSN_SUBSTATE_SLEEP:
		NET_MQTTSN_Event_Sleep(pClient);
		break;
	
	case MQTTSN_SUBSTATE_AWAKE:
		NET_MQTTSN_Event_Aweak(pClient);
		break;
	
	case MQTTSN_SUBSTATE_LOST:
		NET_MQTTSN_Event_Lost(pClient);
		break;
	}
}

/**********************************************************************************************************
 @Function			static void MQTTSN_NBIOT_DictateEvent_SetTime(MQTTSN_ClientsTypeDef* pClient, unsigned int TimeoutSec)
 @Description			MQTTSN_NBIOT_DictateEvent_SetTime		: 事件运行控制器注入时间(内部使用)
 @Input				pClient							: MQTTSN客户端实例
					TimeoutSec						: 注入超时时间
 @Return				void
 @attention			事件运行之前判断是否需要注入时间
**********************************************************************************************************/
static void MQTTSN_NBIOT_DictateEvent_SetTime(MQTTSN_ClientsTypeDef* pClient, unsigned int TimeoutSec)
{
	Stm32_CalculagraphTypeDef dictateRunTime;
	
	/* It is the first time to execute */
	if (pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEnable != true) {
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEnable = true;
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateTimeoutSec = TimeoutSec;
		Stm32_Calculagraph_CountdownSec(&dictateRunTime, pClient->SocketStack->NBIotStack->DictateRunCtl.dictateTimeoutSec);
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateRunTime = dictateRunTime;
	}
}

/**********************************************************************************************************
 @Function			static void MQTTSN_DictateEvent_SetTime(MQTTSN_ClientsTypeDef* pClient, unsigned int TimeoutSec)
 @Description			MQTTSN_DictateEvent_SetTime			: 事件运行控制器注入时间(内部使用)
 @Input				pClient							: MQTTSN客户端实例
					TimeoutSec						: 注入超时时间
 @Return				void
 @attention			事件运行之前判断是否需要注入时间
**********************************************************************************************************/
static void MQTTSN_DictateEvent_SetTime(MQTTSN_ClientsTypeDef* pClient, unsigned int TimeoutSec)
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
 @Function			static void MQTTSN_NormalDictateEvent_SetTime(MQTTSN_ClientsTypeDef* pClient, Stm32_CalculagraphTypeDef* pTimer, unsigned int TimeoutSec)
 @Description			MQTTSN_NormalDictateEvent_SetTime		: 事件运行控制器注入时间(内部使用)
 @Input				pClient							: MQTTSN客户端实例
					TimeoutSec						: 注入超时时间
 @Return				void
 @attention			事件运行之前判断是否需要注入时间
**********************************************************************************************************/
static void MQTTSN_NormalDictateEvent_SetTime(MQTTSN_ClientsTypeDef* pClient, Stm32_CalculagraphTypeDef* pTimer, unsigned int TimeoutSec)
{
	Stm32_CalculagraphTypeDef dictateRunTime;
	
	pClient->DictateRunCtl.dictateTimeoutSec = TimeoutSec;
	Stm32_Calculagraph_CountdownSec(&dictateRunTime, pClient->DictateRunCtl.dictateTimeoutSec);
	*pTimer = dictateRunTime;
}

static unsigned char* MQTTSN_NBIOT_GetDictateFailureCnt(MQTTSN_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateNoTimeOut)
{
	unsigned char* dictateFailureCnt;
	
	switch (dictateNoTimeOut)
	{
	case HARDWARE_REBOOT:
		dictateFailureCnt = &pClient->SocketStack->NBIotStack->DictateRunCtl.dictateRebootFailureCnt;
		break;
	
	case REPORT_ERROE:
		dictateFailureCnt = &pClient->SocketStack->NBIotStack->DictateRunCtl.dictateReportErrorFailureCnt;
		break;
	
	case MODULE_CHECK:
		dictateFailureCnt = &pClient->SocketStack->NBIotStack->DictateRunCtl.dictateModuleCheckFailureCnt;
		break;
	
	case PARAMETER_CONFIG:
		dictateFailureCnt = &pClient->SocketStack->NBIotStack->DictateRunCtl.dictateParameterConfigFailureCnt;
		break;
	
	case ICCID_CHECK:
		dictateFailureCnt = &pClient->SocketStack->NBIotStack->DictateRunCtl.dictateSimICCIDCheckFailureCnt;
		break;
	
	case ATTACH_CHECK:
		dictateFailureCnt = &pClient->SocketStack->NBIotStack->DictateRunCtl.dictateAttachCheckFailureCnt;
		break;
	
	case ATTACH_EXECUTE:
		dictateFailureCnt = &pClient->SocketStack->NBIotStack->DictateRunCtl.dictateAttachExecuteFailureCnt;
		break;
	
	case ATTACH_INQUIRE:
		dictateFailureCnt = &pClient->SocketStack->NBIotStack->DictateRunCtl.dictateAttachInquireFailureCnt;
		break;
	
	case PARAMETER_CHECKOUT:
		dictateFailureCnt = &pClient->SocketStack->NBIotStack->DictateRunCtl.dictatePatameterCheckOutFailureCnt;
		break;
	
	case MINIMUM_FUNCTIONALITY:
		dictateFailureCnt = &pClient->SocketStack->NBIotStack->DictateRunCtl.dictateMinimumFunctionalityFailureCnt;
		break;
	
	case FULL_FUNCTIONALITY:
		dictateFailureCnt = &pClient->SocketStack->NBIotStack->DictateRunCtl.dictateFullFunctionalityFailureCnt;
		break;
	
	case NBAND_MODE_CHECK:
		dictateFailureCnt = &pClient->SocketStack->NBIotStack->DictateRunCtl.dictateNbandModeCheckFailureCnt;
		break;
	
	case NBAND_MODE_CONFIG:
		dictateFailureCnt = &pClient->SocketStack->NBIotStack->DictateRunCtl.dictateNbandModeConfigFailureCnt;
		break;
	
	default :
		dictateFailureCnt = &pClient->SocketStack->NBIotStack->DictateRunCtl.dictateRebootFailureCnt;
		break;
	}
	
	return dictateFailureCnt;
}

static unsigned char* MQTTSN_GetDictateFailureCnt(MQTTSN_ClientsTypeDef* pClient, MQTTSN_SubStateTypeDef dictateNoTimeOut)
{
	unsigned char* dictateFailureCnt;
	
	switch (dictateNoTimeOut)
	{
	case MQTTSN_SUBSTATE_INIT:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateInitFailureCnt;
		break;
	
	case MQTTSN_SUBSTATE_DISCONNECT:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateDisconnectFailureCnt;
		break;
	
	case MQTTSN_SUBSTATE_ACTIVE:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateActiveFailureCnt;
		break;
	
	case MQTTSN_SUBSTATE_SLEEP:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateSleepFailureCnt;
		break;
	
	case MQTTSN_SUBSTATE_AWAKE:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateAweakFailureCnt;
		break;
	
	case MQTTSN_SUBSTATE_LOST:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateLostFailureCnt;
		break;
	}
	
	return dictateFailureCnt;
}

/**********************************************************************************************************
 @Function			static void MQTTSN_NBIOT_DictateEvent_FailExecute(MQTTSN_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateTimeOut, \
																					  NBIOT_DictateEventTypeDef dictateFail, \
																					  NBIOT_DictateEventTypeDef dictateNoTimeOut)
 @Description			MQTTSN_NBIOT_DictateEvent_FailExecute	: 事件运行控制器出错执行(内部使用)
 @Input				pClient							: MQTTSN客户端实例
					dictateTimeOut						: 事件处理错误超时
					dictateFail						: 事件处理错误次数溢出
					dictateNoTimeOut					: 事件处理错误未超时
 @Return				void
**********************************************************************************************************/
static void MQTTSN_NBIOT_DictateEvent_FailExecute(MQTTSN_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateTimeOut, \
																  NBIOT_DictateEventTypeDef dictateFail, \
																  NBIOT_DictateEventTypeDef dictateNoTimeOut)
{
	unsigned char* dictateFailureCnt;
	
	dictateFailureCnt = MQTTSN_NBIOT_GetDictateFailureCnt(pClient, dictateNoTimeOut);
	
	if (Stm32_Calculagraph_IsExpiredSec(&pClient->SocketStack->NBIotStack->DictateRunCtl.dictateRunTime) == true) {
		/* Dictate TimeOut */
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEnable = false;
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = dictateTimeOut;
		*dictateFailureCnt += 1;
		if (*dictateFailureCnt > 3) {
			*dictateFailureCnt = 0;
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = dictateFail;
		}
	}
	else {
		/* Dictate isn't TimeOut */
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = dictateNoTimeOut;
	}
}

/**********************************************************************************************************
 @Function			static void MQTTSN_NBIOT_DictateEvent_SuccessExecute(MQTTSN_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateSuccess, \
																						NBIOT_DictateEventTypeDef dictateNoTimeOut)
 @Description			MQTTSN_NBIOT_DictateEvent_SuccessExecute: 事件运行控制器正确执行(内部使用)
 @Input				pClient							: MQTTSN客户端实例
					dictateSuccess						: 事件处理正确
					dictateNoTimeOut					: 事件处理错误未超时
 @Return				void
**********************************************************************************************************/
static void MQTTSN_NBIOT_DictateEvent_SuccessExecute(MQTTSN_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateSuccess, \
																	NBIOT_DictateEventTypeDef dictateNoTimeOut)
{
	unsigned char* dictateFailureCnt;
	
	dictateFailureCnt = MQTTSN_NBIOT_GetDictateFailureCnt(pClient, dictateNoTimeOut);
	
	pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEnable = false;
	pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = dictateSuccess;
	*dictateFailureCnt = 0;
}

/**********************************************************************************************************
 @Function			static void MQTTSN_DictateEvent_FailExecute(MQTTSN_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateTimeOut, \
																				 MQTTSN_SubStateTypeDef dictateSubTimeOut, \
																				 MQTTSN_SubStateTypeDef dictateSubNoTimeOut)
 @Description			MQTTSN_DictateEvent_FailExecute		: 事件运行控制器出错执行(内部使用)
 @Input				pClient							: MQTTSN客户端实例
					dictateTimeOut						: 事件处理错误超时
					dictateSubTimeOut					: 事件处理错误次数溢出
					dictateNoTimeOut					: 事件处理错误未超时
 @Return				void
**********************************************************************************************************/
static void MQTTSN_DictateEvent_FailExecute(MQTTSN_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateTimeOut, \
															 MQTTSN_SubStateTypeDef dictateSubTimeOut, \
															 MQTTSN_SubStateTypeDef dictateSubNoTimeOut)
{
	unsigned char* dictateFailureCnt;
	
	dictateFailureCnt = MQTTSN_GetDictateFailureCnt(pClient, dictateSubNoTimeOut);
	
	if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
		/* Dictate TimeOut */
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = dictateTimeOut;
		pClient->SubState = dictateSubTimeOut;
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_DISABLE
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
#endif
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_ENABLE
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_DNS;
#endif
		*dictateFailureCnt += 1;
		if (*dictateFailureCnt > 3) {
			*dictateFailureCnt = 0;
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
			pClient->SubState = MQTTSN_SUBSTATE_LOST;
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
		}
	}
	else {
		/* Dictate isn't TimeOut */
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
		pClient->SubState = dictateSubNoTimeOut;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
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
static void MQTTSN_DictateEvent_SuccessExecute(MQTTSN_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateSuccess, \
															    MQTTSN_SubStateTypeDef dictateSubSuccess, \
															    MQTTSN_SubStateTypeDef dictateSubNoTimeOut, \
															    bool dictateFailureCntState)
{
	unsigned char* dictateFailureCnt;
	
	if (dictateFailureCntState) {
		dictateFailureCnt = MQTTSN_GetDictateFailureCnt(pClient, dictateSubNoTimeOut);
		pClient->DictateRunCtl.dictateEnable = false;
		*dictateFailureCnt = 0;
	}
	
	pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = dictateSuccess;
	pClient->SubState = dictateSubSuccess;
	pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
}

/**********************************************************************************************************
 @Function			static void MQTTSN_NBIOT_GetConnectTime(MQTTSN_ClientsTypeDef* pClient, bool startIdleTime)
 @Description			MQTTSN_NBIOT_GetConnectTime			: 获取NBConnect状态时间(内部使用)
 @Input				pClient							: MqttSN客户端实例
					startIdleTime						: 是否开启Idle状态计时器
 @Return				void
**********************************************************************************************************/
static void MQTTSN_NBIOT_GetConnectTime(MQTTSN_ClientsTypeDef* pClient, bool startIdleTime)
{
	unsigned int uICoapConnectTime = 0;
	
	/* Get ConnectTime */
	uICoapConnectTime = Stm32_EventRunningTime_EndMS(&pClient->SocketStack->NBIotStack->ConnectTimeMS) / 1000;
	/* End ConnectTime */
	TCFG_SystemData.CoapConnectTime = pClient->SocketStack->NBIotStack->CoapConnectTimeSec + uICoapConnectTime;
	pClient->SocketStack->NBIotStack->CoapConnectTimeSec = TCFG_SystemData.CoapConnectTime;
	/* Start or End IdleTime */
	if (startIdleTime != false) {
		Stm32_EventRunningTime_StartMS(&pClient->SocketStack->NBIotStack->IdleTimeMS);
	}
	else {
		Stm32_EventRunningTime_EndMS(&pClient->SocketStack->NBIotStack->IdleTimeMS);
	}
}

/**********************************************************************************************************
 @Function			static void MQTTSN_NBIOT_GetIdleTime(MQTTSN_ClientsTypeDef* pClient, bool startConnectTime)
 @Description			MQTTSN_NBIOT_GetIdleTime				: 获取NBIdle状态时间(内部使用)
 @Input				pClient							: MqttSN客户端实例
					startConnectTime					: 是否开启Connect状态计时器
 @Return				void
**********************************************************************************************************/
static void MQTTSN_NBIOT_GetIdleTime(MQTTSN_ClientsTypeDef* pClient, bool startConnectTime)
{
	unsigned int uICoapIdleTime = 0;
	
	/* Get IdleTime */
	uICoapIdleTime = Stm32_EventRunningTime_EndMS(&pClient->SocketStack->NBIotStack->IdleTimeMS) / 1000;
	/* End IdleTime */
	TCFG_SystemData.CoapIdleTime = pClient->SocketStack->NBIotStack->CoapIdleTimeSec + uICoapIdleTime;
	pClient->SocketStack->NBIotStack->CoapIdleTimeSec = TCFG_SystemData.CoapIdleTime;
	/* Start or End ConnectTime */
	if (startConnectTime != false) {
		Stm32_EventRunningTime_StartMS(&pClient->SocketStack->NBIotStack->ConnectTimeMS);
	}
	else {
		Stm32_EventRunningTime_EndMS(&pClient->SocketStack->NBIotStack->ConnectTimeMS);
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Event_StopMode(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Event_StopMode		: 停止模式
 @Input				pClient							: MqttSN客户端实例
 @Return				void
 @attention			当30分钟或有数据需要发送时退出停止模式尝试重启NB,开启NB运行
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Event_StopMode(MQTTSN_ClientsTypeDef* pClient)
{
	Stm32_CalculagraphTypeDef dictateRunTime;
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
	static unsigned char MqttSNStatusBasicIndex;
	static unsigned char MqttSNStatusExtendIndex;
	static unsigned char MqttSNInfoWorkIndex;
	static unsigned char MqttSNInfoBasicIndex;
	static unsigned char MqttSNInfoDynamicIndex;
	static unsigned char MqttSNInfoResponseIndex;
#endif
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM
	static unsigned char MqttSNByteStreamIndex;
#endif
	
	/* It is the first time to execute */
	if (pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEnable != true) {
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEnable = true;
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateTimeoutSec = 1800;
		Stm32_Calculagraph_CountdownSec(&dictateRunTime, pClient->SocketStack->NBIotStack->DictateRunCtl.dictateTimeoutSec);
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateRunTime = dictateRunTime;
		/* NBIOT Module Poweroff */
		NBIOT_Neul_NBxx_HardwarePoweroff(pClient->SocketStack->NBIotStack);
		/* Init Message Index */
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
		MqttSNStatusBasicIndex = NET_MqttSN_Message_StatusBasicRear();
		MqttSNStatusExtendIndex = NET_MqttSN_Message_StatusExtendRear();
		MqttSNInfoWorkIndex = NET_MqttSN_Message_InfoWorkRear();
		MqttSNInfoBasicIndex = NET_MqttSN_Message_InfoBasicRear();
		MqttSNInfoDynamicIndex = NET_MqttSN_Message_InfoDynamicRear();
		MqttSNInfoResponseIndex = NET_MqttSN_Message_InfoResponseRear();
#endif
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM
		MqttSNByteStreamIndex = NET_MqttSN_Message_SendDataRear();
#endif
		/* Get ConnectTime & IdleTime */
		MQTTSN_NBIOT_GetConnectTime(pClient, false);
		MQTTSN_NBIOT_GetIdleTime(pClient, false);
	}
	
	if (Stm32_Calculagraph_IsExpiredSec(&pClient->SocketStack->NBIotStack->DictateRunCtl.dictateRunTime) == true) {
		/* Dictate TimeOut */
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEnable = false;
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		pClient->SubState = MQTTSN_SUBSTATE_INIT;
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_DISABLE
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
#endif
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_ENABLE
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_DNS;
#endif
	}
	else {
		/* Dictate isn't TimeOut */
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
		if ( (NET_MqttSN_Message_StatusBasicRear() != MqttSNStatusBasicIndex) || 
			(NET_MqttSN_Message_StatusExtendRear() != MqttSNStatusExtendIndex) ||
			(NET_MqttSN_Message_InfoWorkRear() != MqttSNInfoWorkIndex) ||
			(NET_MqttSN_Message_InfoBasicRear() != MqttSNInfoBasicIndex) ||
			(NET_MqttSN_Message_InfoDynamicRear() != MqttSNInfoDynamicIndex) ||
			(NET_MqttSN_Message_InfoResponseRear() != MqttSNInfoResponseIndex) ) {
#endif
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM
		if ( NET_MqttSN_Message_SendDataRear() != MqttSNByteStreamIndex ) {
#endif
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEnable = false;
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->SubState = MQTTSN_SUBSTATE_INIT;
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_DISABLE
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
#endif
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_ENABLE
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_DNS;
#endif
		}
		else {
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = STOP_MODE;
			pClient->SubState = MQTTSN_SUBSTATE_INIT;
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
		}
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Event_HardwareReboot(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Event_HardwareReboot	: 硬件重启
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Event_HardwareReboot(MQTTSN_ClientsTypeDef* pClient)
{
	MQTTSN_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (NBIOT_Neul_NBxx_HardwareReboot(pClient->SocketStack->NBIotStack, 8000) == NBIOT_OK) {
		/* Dictate execute is Success */
#if NBIOT_PRINT_ERROR_CODE_TYPE
		MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, REPORT_ERROE, HARDWARE_REBOOT);
#else
		MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, MODULE_CHECK, HARDWARE_REBOOT);
#endif
		/* Get IdleTime */
		MQTTSN_NBIOT_GetIdleTime(pClient, true);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
		MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB HDRBT Ok, Baud:%d", NBIOTBaudRate.Baud);
#endif
	}
	else {
		/* Dictate execute is Fail */
		MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, HARDWARE_REBOOT);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
		MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB HDRBT Fail");
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Event_ReportError(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Event_ReportError		: 错误码输出
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Event_ReportError(MQTTSN_ClientsTypeDef* pClient)
{
	MQTTSN_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBIOT_Neul_NBxx_SetReportTerminationError(pClient->SocketStack->NBIotStack, CMEEnable) == NBIOT_OK)) {
		/* Dictate execute is Success */
		MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, MODULE_CHECK, REPORT_ERROE);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
		MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB ReportECde Set %d Ok", CMEEnable);
#endif
	}
	else {
		/* Dictate execute is Fail */
		MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, REPORT_ERROE);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
		MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB ReportECde Set %d Fail", CMEEnable);
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Event_ModuleCheck(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Event_ModuleCheck		: 模块检测
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Event_ModuleCheck(MQTTSN_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	MQTTSN_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (((NBStatus = NBIOT_Neul_NBxx_CheckReadManufacturer(pClient->SocketStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadManufacturerModel(pClient->SocketStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadModuleVersion(pClient->SocketStack->NBIotStack)) == NBIOT_OK)) {
		/* Dictate execute is Success */
		MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, PARAMETER_CONFIG, MODULE_CHECK);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
		MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB Module Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, MODULE_CHECK);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB Module Check Fail ECde %d", NBStatus);
	#else
		MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB Module Check Fail");
	#endif
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Event_ParameterConfig(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Event_ParameterConfig	: 参数配置
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Event_ParameterConfig(MQTTSN_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	MQTTSN_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadConfigUE(pClient->SocketStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
		MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB Para Config Read Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB Para Config Read Fail ECde %d", NBStatus);
	#else
		MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB Para Config Read Fail");
	#endif
#endif
		return;
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.nconfig.autoConnect != AutoConnectVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->SocketStack->NBIotStack, AutoConnect, AutoConnectVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB %s %d Ok", AutoConnect, AutoConnectVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB %s %d Fail", AutoConnect, AutoConnectVal);
#endif
			return;
		}
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.nconfig.crScrambling != CrScramblingVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->SocketStack->NBIotStack, CrScrambling, CrScramblingVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB %s %d Ok", CrScrambling, CrScramblingVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB %s %d Fail", CrScrambling, CrScramblingVal);
#endif
			return;
		}
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.nconfig.crSiAvoid != CrSiAvoidVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->SocketStack->NBIotStack, CrSiAvoid, CrSiAvoidVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB %s %d Ok", CrSiAvoid, CrSiAvoidVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB %s %d Fail", CrSiAvoid, CrSiAvoidVal);
#endif
			return;
		}
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.nconfig.combineAttach != CombineAttachVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->SocketStack->NBIotStack, CombineAttach, CombineAttachVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB %s %d Ok", CombineAttach, CombineAttachVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB %s %d Fail", CombineAttach, CombineAttachVal);
#endif
			return;
		}
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.nconfig.cellReselection != CellReselectionVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->SocketStack->NBIotStack, CellReselection, CellReselectionVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB %s %d Ok", CellReselection, CellReselectionVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB %s %d Fail", CellReselection, CellReselectionVal);
#endif
			return;
		}
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.nconfig.enableBip != EnableBipVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->SocketStack->NBIotStack, EnableBip, EnableBipVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB %s %d Ok", EnableBip, EnableBipVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB %s %d Fail", EnableBip, EnableBipVal);
#endif
			return;
		}
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Event_SimICCIDCheck(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Event_SimICCIDCheck	: Sim卡检测
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Event_SimICCIDCheck(MQTTSN_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	MQTTSN_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadICCID(pClient->SocketStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, FULL_FUNCTIONALITY, ICCID_CHECK);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
		MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB ICCID Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ICCID_CHECK);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB ICCID Fail ECde %d", NBStatus);
	#else
		MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB ICCID Fail");
	#endif
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Event_FullFunctionality(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Event_FullFunctionality	: 完整功能
 @Input				pClient								: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Event_FullFunctionality(MQTTSN_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	MQTTSN_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadMinOrFullFunc(pClient->SocketStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CHECK, FULL_FUNCTIONALITY);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
		MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB FullFunc Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, FULL_FUNCTIONALITY);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB FullFunc Check Fail ECde %d", NBStatus);
	#else
		MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB FullFunc Check Fail");
	#endif
#endif
		return;
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.functionality != FullFunc) {
		if ((NBStatus = NBIOT_Neul_NBxx_SetMinOrFullFunc(pClient->SocketStack->NBIotStack, FullFunc)) == NBIOT_OK) {
			/* Dictate execute is Success */
			MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CHECK, FULL_FUNCTIONALITY);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB FullFunc Set Ok");
#endif
		}
		else {
			/* Dictate execute is Fail */
			MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, FULL_FUNCTIONALITY);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB FullFunc Set Fail ECde %d", NBStatus);
		#else
			MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB FullFunc Set Fail");
		#endif
#endif
			return;
		}
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Event_MinimumFunctionality(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Event_MinimumFunctionality	: 最小功能
 @Input				pClient								: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Event_MinimumFunctionality(MQTTSN_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	MQTTSN_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadMinOrFullFunc(pClient->SocketStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CONFIG, MINIMUM_FUNCTIONALITY);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
		MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB MinFunc Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, MINIMUM_FUNCTIONALITY);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB MinFunc Check Fail ECde %d", NBStatus);
	#else
		MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB MinFunc Check Fail");
	#endif
#endif
		return;
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.functionality != MinFunc) {
		if ((NBStatus = NBIOT_Neul_NBxx_SetMinOrFullFunc(pClient->SocketStack->NBIotStack, MinFunc)) == NBIOT_OK) {
			/* Dictate execute is Success */
			MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CONFIG, MINIMUM_FUNCTIONALITY);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB MinFunc Set Ok");
#endif
		}
		else {
			/* Dictate execute is Fail */
			MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, MINIMUM_FUNCTIONALITY);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB MinFunc Set Fail ECde %d", NBStatus);
		#else
			MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB MinFunc Set Fail");
		#endif
#endif
			return;
		}
	}
}

static bool NET_MQTTSN_CheckNband(MQTTSN_ClientsTypeDef* pClient, NBIOT_NBandTypeDef nbandsrc, NBIOT_NBandTypeDef nbanddst)
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
 @Function			void NET_MQTTSN_NBIOT_Event_NbandModeCheck(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Event_NbandModeCheck		: NBAND模式查询
 @Input				pClient								: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Event_NbandModeCheck(MQTTSN_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	MQTTSN_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadSupportedBands(pClient->SocketStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CHECK, NBAND_MODE_CHECK);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
		MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB BAND Read %d:%d.%d.%d Ok", pClient->SocketStack->NBIotStack->Parameter.band.NBandNum, \
														 pClient->SocketStack->NBIotStack->Parameter.band.NBandVal[0], \
														 pClient->SocketStack->NBIotStack->Parameter.band.NBandVal[1], \
														 pClient->SocketStack->NBIotStack->Parameter.band.NBandVal[2]);
#endif
	}
	else {
		/* Dictate execute is Fail */
		MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, NBAND_MODE_CHECK);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB BAND Read Fail ECde %d", NBStatus);
	#else
		MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB BAND Read Fail");
	#endif
#endif
	}
	
	if (NET_MQTTSN_CheckNband(pClient, pClient->SocketStack->NBIotStack->Parameter.band, MQTTSN_NBIOT_BAND_TYPE) != true) {
		/* BAND Mode Mast be Config */
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MINIMUM_FUNCTIONALITY;
	}
	else {
		/* BAND Mode Needn't be Config */
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = ATTACH_CHECK;
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Event_NbandModeConfig(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Event_NbandModeConfig		: NBAND模式配置
 @Input				pClient								: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Event_NbandModeConfig(MQTTSN_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	MQTTSN_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadSupportedBands(pClient->SocketStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, FULL_FUNCTIONALITY, NBAND_MODE_CONFIG);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
		MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB BAND Read Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, NBAND_MODE_CONFIG);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB BAND Read Fail ECde %d", NBStatus);
	#else
		MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB BAND Read Fail");
	#endif
#endif
	}
	
	if (NET_MQTTSN_CheckNband(pClient, pClient->SocketStack->NBIotStack->Parameter.band, MQTTSN_NBIOT_BAND_TYPE) != true) {
		/* BAND Mode Mast be Config */
		if ((NBStatus = NBIOT_Neul_NBxx_SetSupportedBands(pClient->SocketStack->NBIotStack, MQTTSN_NBIOT_BAND_TYPE)) == NBIOT_OK) {
			/* Dictate execute is Success */
			MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, FULL_FUNCTIONALITY, NBAND_MODE_CONFIG);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB BAND Set %d:%d.%d.%d Ok", MQTTSN_NBIOT_BAND_TYPE.NBandNum, \
															MQTTSN_NBIOT_BAND_TYPE.NBandVal[0], \
															MQTTSN_NBIOT_BAND_TYPE.NBandVal[1], \
															MQTTSN_NBIOT_BAND_TYPE.NBandVal[2]);
#endif
		}
		else {
			/* Dictate execute is Fail */
			MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, NBAND_MODE_CONFIG);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB BAND Set Fail ECde %d", NBStatus);
		#else
			MQTTSN_DEBUG_LOG_PRINTF_BEFORE("NB BAND Set Fail");
		#endif
#endif
			return;
		}
	}
	else {
		/* BAND Mode Needn't be Config */
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = FULL_FUNCTIONALITY;
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Event_AttachCheck(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Event_AttachCheck		: 注网检查
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Event_AttachCheck(MQTTSN_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	MQTTSN_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadAttachOrDetach(pClient->SocketStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, ATTACH_CHECK, ATTACH_CHECK);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
		MQTTSN_DEBUG_LOG_PRINTF("NB CGATT %d Ok", pClient->SocketStack->NBIotStack->Parameter.netstate);
#endif
	}
	else {
		/* Dictate execute is Fail */
		MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_CHECK);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		MQTTSN_DEBUG_LOG_PRINTF("NB CGATT %d Fail ECde %d", pClient->SocketStack->NBIotStack->Parameter.netstate, NBStatus);
	#else
		MQTTSN_DEBUG_LOG_PRINTF("NB CGATT %d Fail", pClient->SocketStack->NBIotStack->Parameter.netstate);
	#endif
#endif
		return;
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.netstate != Attach) {
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = ATTACH_EXECUTE;
	}
	else {
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = ATTACH_INQUIRE;
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Event_AttachExecute(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Event_AttachExecute	: 注网进行
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Event_AttachExecute(MQTTSN_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	MQTTSN_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_SetAttachOrDetach(pClient->SocketStack->NBIotStack, Attach)) == NBIOT_OK) {
		/* Dictate execute is Success */
		MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, ATTACH_INQUIRE, ATTACH_EXECUTE);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
		MQTTSN_DEBUG_LOG_PRINTF("NB Set CGATT %d Ok", Attach);
#endif
	}
	else {
		/* Dictate execute is Fail */
		MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_EXECUTE);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		MQTTSN_DEBUG_LOG_PRINTF("NB Set CGATT %d Fail ECde %d", Attach, NBStatus);
	#else
		MQTTSN_DEBUG_LOG_PRINTF("NB Set CGATT %d Fail", Attach);
	#endif
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Event_AttachInquire(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Event_AttachInquire	: 注网查询
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Event_AttachInquire(MQTTSN_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	MQTTSN_NBIOT_DictateEvent_SetTime(pClient, 60);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadAttachOrDetach(pClient->SocketStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = ATTACH_INQUIRE;
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
		MQTTSN_DEBUG_LOG_PRINTF("NB CGATT %d Ok", pClient->SocketStack->NBIotStack->Parameter.netstate);
#endif
	}
	else {
		/* Dictate execute is Fail */
		MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_INQUIRE);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		MQTTSN_DEBUG_LOG_PRINTF("NB CGATT %d Fail ECde %d", pClient->SocketStack->NBIotStack->Parameter.netstate, NBStatus);
	#else
		MQTTSN_DEBUG_LOG_PRINTF("NB CGATT %d Fail", pClient->SocketStack->NBIotStack->Parameter.netstate);
	#endif
#endif
		return;
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.netstate != Attach) {
		MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_INQUIRE);
	}
	else {
		/* 注网成功 */
		MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, PARAMETER_CHECKOUT, ATTACH_INQUIRE);
		
		/* Get ConnectTime */
		MQTTSN_NBIOT_GetConnectTime(pClient, true);
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Event_PatameterCheckOut(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Event_PatameterCheckOut: 参数检出
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Event_ParameterCheckOut(MQTTSN_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	MQTTSN_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (((NBStatus = NBIOT_Neul_NBxx_CheckReadIMEI(pClient->SocketStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadIMSI(pClient->SocketStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadCGPADDR(pClient->SocketStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadCGDCONT(pClient->SocketStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadRSSI(pClient->SocketStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadStatisticsRADIO(pClient->SocketStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadAreaCode(pClient->SocketStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadDateTime(pClient->SocketStack->NBIotStack)) == NBIOT_OK)) {
		/* Dictate execute is Success */
		MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, MQTTSN_PROCESS_STACK, PARAMETER_CHECKOUT);
		
		pClient->SocketStack->NBIotStack->Registered = true;
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
		MQTTSN_DEBUG_LOG_PRINTF("NB Para Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CHECKOUT);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		MQTTSN_DEBUG_LOG_PRINTF("NB Para Check Fail ECde %d", NBStatus);
	#else
		MQTTSN_DEBUG_LOG_PRINTF("NB Para Check Fail");
	#endif
#endif
		return;
	}
	
	/* Set System Time */
	RTC_Set_Date(pClient->SocketStack->NBIotStack->Parameter.dataTime.year, pClient->SocketStack->NBIotStack->Parameter.dataTime.month, pClient->SocketStack->NBIotStack->Parameter.dataTime.day);
	RTC_Set_Time(pClient->SocketStack->NBIotStack->Parameter.dataTime.hour, pClient->SocketStack->NBIotStack->Parameter.dataTime.min,   pClient->SocketStack->NBIotStack->Parameter.dataTime.sec);
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_Event_Init(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_Event_Init				: INIT
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_Event_Init(MQTTSN_ClientsTypeDef* pClient)
{
	Stm32_CalculagraphTypeDef dictateRunTime;
	
	MQTTSN_DictateEvent_SetTime(pClient, 30);
	
	/* Creat UDP Socket */
	if (pClient->SocketStack->Open(pClient->SocketStack) == MQTTSN_OK) {
		/* Dictate execute is Success */
		MQTTSN_DictateEvent_SuccessExecute(pClient, MQTTSN_PROCESS_STACK, MQTTSN_SUBSTATE_DISCONNECT, MQTTSN_SUBSTATE_INIT, true);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
		MQTTSN_DEBUG_LOG_PRINTF("MqttSN Creat UDP Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		MQTTSN_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, MQTTSN_SUBSTATE_INIT, MQTTSN_SUBSTATE_INIT);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
		MQTTSN_DEBUG_LOG_PRINTF("MqttSN Creat UDP Fail");
#endif
		return;
	}
	
	/* Initiation MQTTSN Status */
	pClient->DictateRunCtl.dictateSubscribeStatus = false;
	/* Set Pingreg Duration */
	pClient->DictateRunCtl.dictateTimeoutSec = TNET_MQTTSN_PINGREG_DURATION;
	Stm32_Calculagraph_CountdownSec(&dictateRunTime, pClient->DictateRunCtl.dictateTimeoutSec);
	pClient->PingTimer = dictateRunTime;
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_Event_Disconnect(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_Event_Disconnect			: DISCONNECT
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_Event_Disconnect(MQTTSN_ClientsTypeDef* pClient)
{
	MQTTSNPacket_connectData options = MQTTSNPacket_connectData_initializer;
	
	MQTTSN_DictateEvent_SetTime(pClient, 60);
	
	/* Connecting MQTTSN Server */
	options.clientID.cstring = MQTTSN_CLIENT_ID;
	options.duration = TNET_MQTTSN_ACTIVE_DURATION + 40;
	options.cleansession = false;
	if (MQTTSN_Connect(pClient, &options) != MQTTSN_OK) {
		/* Dictate execute is Fail */
		MQTTSN_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, MQTTSN_SUBSTATE_INIT, MQTTSN_SUBSTATE_DISCONNECT);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
		MQTTSN_DEBUG_LOG_PRINTF("MqttSN Cont Fail");
#endif
	}
	else {
		/* Dictate execute is Success */
		MQTTSN_DictateEvent_SuccessExecute(pClient, MQTTSN_PROCESS_STACK, MQTTSN_SUBSTATE_ACTIVE, MQTTSN_SUBSTATE_DISCONNECT, true);
		/* Set Active Duration */
		MQTTSN_NormalDictateEvent_SetTime(pClient, &pClient->ActiveTimer, TNET_MQTTSN_ACTIVE_DURATION);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
		MQTTSN_DEBUG_LOG_PRINTF("MqttSN Cont Ok");
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_Event_Active(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_Event_Active				: ACTIVE
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_Event_Active(MQTTSN_ClientsTypeDef* pClient)
{
	MQTTSN_DictateEvent_SetTime(pClient, 90);
	
	/* Subscribing Topic */
	if (pClient->DictateRunCtl.dictateSubscribeStatus != true) {
		if (MQTTSN_Subscribe(pClient, MQTTSN_SUBSCRIBE_ID, QOS2, messageHandlerFunction) != MQTTSN_OK) {
			/* Dictate execute is Fail */
			MQTTSN_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, MQTTSN_SUBSTATE_INIT, MQTTSN_SUBSTATE_ACTIVE);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			MQTTSN_DEBUG_LOG_PRINTF("MqttSN Sub %s Fail", MQTTSN_SUBSCRIBE_ID);
#endif
			return;
		}
		else {
			/* Dictate execute is Success */
			MQTTSN_DictateEvent_SuccessExecute(pClient, MQTTSN_PROCESS_STACK, MQTTSN_SUBSTATE_ACTIVE, MQTTSN_SUBSTATE_ACTIVE, false);
			pClient->DictateRunCtl.dictateSubscribeStatus = true;
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			MQTTSN_DEBUG_LOG_PRINTF("MqttSN Sub %s Ok", MQTTSN_SUBSCRIBE_ID);
#endif
		}
	}
	
	/* Whether the query has data needs to be sent */
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
	/* OBJECT_TYPE_TMOTES_STATUS_BASIC_PUT */
#if NBMQTTSN_SENDCODE_STATUS_BASIC
	if (NET_MqttSN_Message_StatusBasicisEmpty() != true) {
		pClient->MessageSendCtl.messageStatusBasic = true;
		/* Get IdleTime */
		MQTTSN_NBIOT_GetIdleTime(pClient, true);
	}
	if (pClient->MessageSendCtl.messageStatusBasic != false) {
		if (NET_MQTTSN_SendPayloadPacket(pClient, OBJECT_TYPE_TMOTES_STATUS_BASIC_PUT) != MQTTSN_OK) {
			/* Dictate execute is Fail */
			MQTTSN_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, MQTTSN_SUBSTATE_INIT, MQTTSN_SUBSTATE_ACTIVE);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			MQTTSN_DEBUG_LOG_PRINTF("MqttSN Send StatusBasic Fail");
#endif
			return;
		}
		else {
			/* Dictate execute is Success */
			MQTTSN_DictateEvent_SuccessExecute(pClient, MQTTSN_PROCESS_STACK, MQTTSN_SUBSTATE_ACTIVE, MQTTSN_SUBSTATE_ACTIVE, true);
			pClient->MessageSendCtl.messageStatusBasic = false;
			pClient->SocketStack->NBIotStack->NetStateIdentification = true;
			NET_MqttSN_Message_StatusBasicOffSet();
			/* Set Active Duration */
			MQTTSN_NormalDictateEvent_SetTime(pClient, &pClient->ActiveTimer, TNET_MQTTSN_ACTIVE_DURATION);
		#if NBMQTTSN_LISTEN_PARAMETER_TYPE == NBMQTTSN_LISTEN_PARAMETER_ENABLE
			NET_MQTTSN_NBIOT_Listen_Enable_EnterParameter(pClient);
		#endif
			/* NB 继续活跃注入时间 */
			TCFG_Utility_Set_Nbiot_IdleLifetime(NBIOT_CONTINUE_LIFETIME);
			/* Get ConnectTime */
			MQTTSN_NBIOT_GetConnectTime(pClient, true);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			MQTTSN_DEBUG_LOG_PRINTF("MqttSN Send StatusBasic Ok");
#endif
		}
	}
#endif
	
	/* OBJECT_TYPE_TMOTES_STATUS_EXTEND_PUT */
#if NBMQTTSN_SENDCODE_STATUS_EXTEND
	if (NET_MqttSN_Message_StatusExtendisEmpty() != true) {
		pClient->MessageSendCtl.messageStatusExtend = true;
		/* Get IdleTime */
		MQTTSN_NBIOT_GetIdleTime(pClient, true);
	}
	if (pClient->MessageSendCtl.messageStatusExtend != false) {
		if (NET_MQTTSN_SendPayloadPacket(pClient, OBJECT_TYPE_TMOTES_STATUS_EXTEND_PUT) != MQTTSN_OK) {
			/* Dictate execute is Fail */
			MQTTSN_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, MQTTSN_SUBSTATE_INIT, MQTTSN_SUBSTATE_ACTIVE);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			MQTTSN_DEBUG_LOG_PRINTF("MqttSN Send StatusExtend Fail");
#endif
			return;
		}
		else {
			/* Dictate execute is Success */
			MQTTSN_DictateEvent_SuccessExecute(pClient, MQTTSN_PROCESS_STACK, MQTTSN_SUBSTATE_ACTIVE, MQTTSN_SUBSTATE_ACTIVE, true);
			pClient->MessageSendCtl.messageStatusExtend = false;
			pClient->SocketStack->NBIotStack->NetStateIdentification = true;
			NET_MqttSN_Message_StatusExtendOffSet();
			/* Set Active Duration */
			MQTTSN_NormalDictateEvent_SetTime(pClient, &pClient->ActiveTimer, TNET_MQTTSN_ACTIVE_DURATION);
		#if NBMQTTSN_LISTEN_PARAMETER_TYPE == NBMQTTSN_LISTEN_PARAMETER_ENABLE
			NET_MQTTSN_NBIOT_Listen_Enable_EnterParameter(pClient);
		#endif
			/* NB 继续活跃注入时间 */
			TCFG_Utility_Set_Nbiot_IdleLifetime(NBIOT_CONTINUE_LIFETIME);
			/* Get ConnectTime */
			MQTTSN_NBIOT_GetConnectTime(pClient, true);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			MQTTSN_DEBUG_LOG_PRINTF("MqttSN Send StatusExtend Ok");
#endif
		}
	}
#endif
	
	/* OBJECT_TYPE_TMOTES_INFO_WORK_PUT */
#if NBMQTTSN_SENDCODE_WORK_INFO
	if (NET_MqttSN_Message_InfoWorkisEmpty() != true) {
		pClient->MessageSendCtl.messageInfoWork = true;
		/* Get IdleTime */
		MQTTSN_NBIOT_GetIdleTime(pClient, true);
	}
	if (pClient->MessageSendCtl.messageInfoWork != false) {
		if (NET_MQTTSN_SendPayloadPacket(pClient, OBJECT_TYPE_TMOTES_INFO_WORK_PUT) != MQTTSN_OK) {
			/* Dictate execute is Fail */
			MQTTSN_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, MQTTSN_SUBSTATE_INIT, MQTTSN_SUBSTATE_ACTIVE);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			MQTTSN_DEBUG_LOG_PRINTF("MqttSN Send InfoWork Fail");
#endif
			return;
		}
		else {
			/* Dictate execute is Success */
			MQTTSN_DictateEvent_SuccessExecute(pClient, MQTTSN_PROCESS_STACK, MQTTSN_SUBSTATE_ACTIVE, MQTTSN_SUBSTATE_ACTIVE, true);
			pClient->MessageSendCtl.messageInfoWork = false;
			pClient->SocketStack->NBIotStack->NetStateIdentification = true;
			NET_MqttSN_Message_InfoWorkOffSet();
			/* Set Active Duration */
			MQTTSN_NormalDictateEvent_SetTime(pClient, &pClient->ActiveTimer, TNET_MQTTSN_ACTIVE_DURATION);
		#if NBMQTTSN_LISTEN_PARAMETER_TYPE == NBMQTTSN_LISTEN_PARAMETER_ENABLE
			NET_MQTTSN_NBIOT_Listen_Enable_EnterParameter(pClient);
		#endif
			/* NB 继续活跃注入时间 */
			TCFG_Utility_Set_Nbiot_IdleLifetime(NBIOT_CONTINUE_LIFETIME);
			/* Get ConnectTime */
			MQTTSN_NBIOT_GetConnectTime(pClient, true);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			MQTTSN_DEBUG_LOG_PRINTF("MqttSN Send InfoWork Ok");
#endif
		}
	}
#endif
	
	/* OBJECT_TYPE_TMOTES_INFO_BASIC_PUT */
#if NBMQTTSN_SENDCODE_BASIC_INFO
	if (NET_MqttSN_Message_InfoBasicisEmpty() != true) {
		pClient->MessageSendCtl.messageInfoBasic = true;
		/* Get IdleTime */
		MQTTSN_NBIOT_GetIdleTime(pClient, true);
	}
	if (pClient->MessageSendCtl.messageInfoBasic != false) {
		if (NET_MQTTSN_SendPayloadPacket(pClient, OBJECT_TYPE_TMOTES_INFO_BASIC_PUT) != MQTTSN_OK) {
			/* Dictate execute is Fail */
			MQTTSN_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, MQTTSN_SUBSTATE_INIT, MQTTSN_SUBSTATE_ACTIVE);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			MQTTSN_DEBUG_LOG_PRINTF("MqttSN Send InfoBasic Fail");
#endif
			return;
		}
		else {
			/* Dictate execute is Success */
			MQTTSN_DictateEvent_SuccessExecute(pClient, MQTTSN_PROCESS_STACK, MQTTSN_SUBSTATE_ACTIVE, MQTTSN_SUBSTATE_ACTIVE, true);
			pClient->MessageSendCtl.messageInfoBasic = false;
			pClient->SocketStack->NBIotStack->NetStateIdentification = true;
			NET_MqttSN_Message_InfoBasicOffSet();
			/* Set Active Duration */
			MQTTSN_NormalDictateEvent_SetTime(pClient, &pClient->ActiveTimer, TNET_MQTTSN_ACTIVE_DURATION);
		#if NBMQTTSN_LISTEN_PARAMETER_TYPE == NBMQTTSN_LISTEN_PARAMETER_ENABLE
			NET_MQTTSN_NBIOT_Listen_Enable_EnterParameter(pClient);
		#endif
			/* NB 继续活跃注入时间 */
			TCFG_Utility_Set_Nbiot_IdleLifetime(NBIOT_CONTINUE_LIFETIME);
			/* Get ConnectTime */
			MQTTSN_NBIOT_GetConnectTime(pClient, true);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			MQTTSN_DEBUG_LOG_PRINTF("MqttSN Send InfoBasic Ok");
#endif
		}
	}
#endif
	
	/* OBJECT_TYPE_TMOTES_INFO_DYNAMIC_PUT */
#if NBMQTTSN_SENDCODE_DYNAMIC_INFO
	if (NET_MqttSN_Message_InfoDynamicisEmpty() != true) {
		pClient->MessageSendCtl.messageInfoDynamic = true;
		/* Get IdleTime */
		MQTTSN_NBIOT_GetIdleTime(pClient, true);
	}
	if (pClient->MessageSendCtl.messageInfoDynamic != false) {
		if (NET_MQTTSN_SendPayloadPacket(pClient, OBJECT_TYPE_TMOTES_INFO_DYNAMIC_PUT) != MQTTSN_OK) {
			/* Dictate execute is Fail */
			MQTTSN_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, MQTTSN_SUBSTATE_INIT, MQTTSN_SUBSTATE_ACTIVE);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			MQTTSN_DEBUG_LOG_PRINTF("MqttSN Send InfoDynamic Fail");
#endif
			return;
		}
		else {
			/* Dictate execute is Success */
			MQTTSN_DictateEvent_SuccessExecute(pClient, MQTTSN_PROCESS_STACK, MQTTSN_SUBSTATE_ACTIVE, MQTTSN_SUBSTATE_ACTIVE, true);
			pClient->MessageSendCtl.messageInfoDynamic = false;
			pClient->SocketStack->NBIotStack->NetStateIdentification = true;
			NET_MqttSN_Message_InfoDynamicOffSet();
			/* Set Active Duration */
			MQTTSN_NormalDictateEvent_SetTime(pClient, &pClient->ActiveTimer, TNET_MQTTSN_ACTIVE_DURATION);
		#if NBMQTTSN_LISTEN_PARAMETER_TYPE == NBMQTTSN_LISTEN_PARAMETER_ENABLE
			NET_MQTTSN_NBIOT_Listen_Enable_EnterParameter(pClient);
		#endif
			/* NB 继续活跃注入时间 */
			TCFG_Utility_Set_Nbiot_IdleLifetime(NBIOT_CONTINUE_LIFETIME);
			/* Get ConnectTime */
			MQTTSN_NBIOT_GetConnectTime(pClient, true);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			MQTTSN_DEBUG_LOG_PRINTF("MqttSN Send InfoDynamic Ok");
#endif
		}
	}
#endif
	
	/* OBJECT_TYPE_TMOTES_INFO_RESPONSE_PUT */
	if (NET_MqttSN_Message_InfoResponseisEmpty() != true) {
		pClient->MessageSendCtl.messageInfoResponse = true;
		/* Get IdleTime */
		MQTTSN_NBIOT_GetIdleTime(pClient, true);
	}
	if (pClient->MessageSendCtl.messageInfoResponse != false) {
		if (NET_MQTTSN_SendPayloadPacket(pClient, OBJECT_TYPE_TMOTES_INFO_RESPONSE_PUT) != MQTTSN_OK) {
			/* Dictate execute is Fail */
			MQTTSN_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, MQTTSN_SUBSTATE_INIT, MQTTSN_SUBSTATE_ACTIVE);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			MQTTSN_DEBUG_LOG_PRINTF("MqttSN Send InfoResponse Fail");
#endif
			return;
		}
		else {
			/* Dictate execute is Success */
			MQTTSN_DictateEvent_SuccessExecute(pClient, MQTTSN_PROCESS_STACK, MQTTSN_SUBSTATE_ACTIVE, MQTTSN_SUBSTATE_ACTIVE, true);
			pClient->MessageSendCtl.messageInfoResponse = false;
			pClient->SocketStack->NBIotStack->NetStateIdentification = true;
			NET_MqttSN_Message_InfoResponseOffSet();
			/* Set Active Duration */
			MQTTSN_NormalDictateEvent_SetTime(pClient, &pClient->ActiveTimer, TNET_MQTTSN_ACTIVE_DURATION);
		#if NBMQTTSN_LISTEN_PARAMETER_TYPE == NBMQTTSN_LISTEN_PARAMETER_ENABLE
			NET_MQTTSN_NBIOT_Listen_Enable_EnterParameter(pClient);
		#endif
			/* NB 继续活跃注入时间 */
			TCFG_Utility_Set_Nbiot_IdleLifetime(NBIOT_CONTINUE_LIFETIME);
			/* Get ConnectTime */
			MQTTSN_NBIOT_GetConnectTime(pClient, true);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			MQTTSN_DEBUG_LOG_PRINTF("MqttSN Send InfoResponse Ok");
#endif
		}
	}
#endif
	
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM
	/* OBJECT_TYPE_TMOTES_BYTE_STREAM_PUT */
	if (NET_MqttSN_Message_SendDataisEmpty() != true) {
		pClient->MessageSendCtl.messageByteStream = true;
		/* Get IdleTime */
		MQTTSN_NBIOT_GetIdleTime(pClient, true);
	}
	if (pClient->MessageSendCtl.messageByteStream != false) {
		if (NET_MQTTSN_SendPayloadPacket(pClient, OBJECT_TYPE_TMOTES_BYTE_STREAM_PUT) != MQTTSN_OK) {
			/* Dictate execute is Fail */
			MQTTSN_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, MQTTSN_SUBSTATE_INIT, MQTTSN_SUBSTATE_ACTIVE);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			MQTTSN_DEBUG_LOG_PRINTF("MqttSN Send Payload Fail");
#endif
		}
		else {
			/* Dictate execute is Success */
			MQTTSN_DictateEvent_SuccessExecute(pClient, MQTTSN_PROCESS_STACK, MQTTSN_SUBSTATE_ACTIVE, MQTTSN_SUBSTATE_ACTIVE, true);
			pClient->MessageSendCtl.messageByteStream = false;
			pClient->SocketStack->NBIotStack->NetStateIdentification = true;
			NET_MqttSN_Message_SendDataOffSet();
			/* Set Active Duration */
			MQTTSN_NormalDictateEvent_SetTime(pClient, &pClient->ActiveTimer, TNET_MQTTSN_ACTIVE_DURATION);
		#if NBMQTTSN_LISTEN_PARAMETER_TYPE == NBMQTTSN_LISTEN_PARAMETER_ENABLE
			NET_MQTTSN_NBIOT_Listen_Enable_EnterParameter(pClient);
		#endif
			/* NB 继续活跃注入时间 */
			TCFG_Utility_Set_Nbiot_IdleLifetime(NBIOT_CONTINUE_LIFETIME);
			/* Get ConnectTime */
			MQTTSN_NBIOT_GetConnectTime(pClient, true);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			MQTTSN_DEBUG_LOG_PRINTF("MqttSN Send Payload Ok");
#endif
		}
	}
#endif
	
	/* Keep active for Active seconds before to Sleep, so we can send messsage contiguously */
	if (Stm32_Calculagraph_IsExpiredSec(&pClient->ActiveTimer) == true) {
		/* Arrival time for Sleep */
		if (MQTTSN_DisConnect(pClient, TNET_MQTTSN_SLEEP_DURATION) != MQTTSN_OK) {
			/* Dictate execute is Fail */
			MQTTSN_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, MQTTSN_SUBSTATE_INIT, MQTTSN_SUBSTATE_ACTIVE);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			MQTTSN_DEBUG_LOG_PRINTF("MqttSN DisCont Fail");
#endif
		}
		else {
			/* Dictate execute is Success */
			MQTTSN_DictateEvent_SuccessExecute(pClient, MQTTSN_PROCESS_STACK, MQTTSN_SUBSTATE_AWAKE, MQTTSN_SUBSTATE_ACTIVE, true);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			MQTTSN_DEBUG_LOG_PRINTF("MqttSN DisCont Ok");
#endif
		}
	}
	else {
		/* Keep Active */
		MQTTSN_RecvAck(pClient);
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
		pClient->SubState = MQTTSN_SUBSTATE_ACTIVE;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_PCP;
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_Event_Sleep(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_Event_Sleep				: SLEEP
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_Event_Sleep(MQTTSN_ClientsTypeDef* pClient)
{
	MQTTSNPacket_connectData options = MQTTSNPacket_connectData_initializer;
	
	MQTTSN_DictateEvent_SetTime(pClient, 60);
	
	/* Whether the query has data needs to be sent */
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
	if (NET_MqttSN_Message_StatusBasicisEmpty() != true) {
		pClient->MessageSendCtl.messageStatusBasic = true;
	}
	if (NET_MqttSN_Message_StatusExtendisEmpty() != true) {
		pClient->MessageSendCtl.messageStatusExtend = true;
	}
	if (NET_MqttSN_Message_InfoWorkisEmpty() != true) {
		pClient->MessageSendCtl.messageInfoWork = true;
	}
	if (NET_MqttSN_Message_InfoBasicisEmpty() != true) {
		pClient->MessageSendCtl.messageInfoBasic = true;
	}
	if (NET_MqttSN_Message_InfoDynamicisEmpty() != true) {
		pClient->MessageSendCtl.messageInfoDynamic = true;
	}
	if (NET_MqttSN_Message_InfoResponseisEmpty() != true) {
		pClient->MessageSendCtl.messageInfoResponse = true;
	}
#endif
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM
	if (NET_MqttSN_Message_SendDataisEmpty() != true) {
		pClient->MessageSendCtl.messageByteStream = true;
	}
#endif
	
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
	if ( (pClient->MessageSendCtl.messageStatusBasic != false) || 
		(pClient->MessageSendCtl.messageStatusExtend != false) ||
		(pClient->MessageSendCtl.messageInfoWork != false) ||
		(pClient->MessageSendCtl.messageInfoBasic != false) ||
		(pClient->MessageSendCtl.messageInfoDynamic != false) ||
		(pClient->MessageSendCtl.messageInfoResponse != false) ) {
#endif
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM
	if ( pClient->MessageSendCtl.messageByteStream != false ) {
#endif
		options.clientID.cstring = MQTTSN_CLIENT_ID;
		options.duration = TNET_MQTTSN_ACTIVE_DURATION + 40;
		options.cleansession = false;
		if (MQTTSN_Connect(pClient, &options) != MQTTSN_OK) {
			/* Dictate execute is Fail */
			MQTTSN_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, MQTTSN_SUBSTATE_INIT, MQTTSN_SUBSTATE_SLEEP);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			MQTTSN_DEBUG_LOG_PRINTF("MqttSN Cont Fail");
#endif
			return;
		}
		else {
			/* Dictate execute is Success */
			MQTTSN_DictateEvent_SuccessExecute(pClient, MQTTSN_PROCESS_STACK, MQTTSN_SUBSTATE_ACTIVE, MQTTSN_SUBSTATE_SLEEP, true);
			/* Set Active Duration */
			MQTTSN_NormalDictateEvent_SetTime(pClient, &pClient->ActiveTimer, TNET_MQTTSN_ACTIVE_DURATION);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			MQTTSN_DEBUG_LOG_PRINTF("MqttSN Cont Ok");
#endif
			return;
		}
	}
	
	/* If time to Aweak, then send a pingreq */
	if (Stm32_Calculagraph_IsExpiredSec(&pClient->PingTimer) == true) {
		MQTTSN_DictateEvent_SuccessExecute(pClient, MQTTSN_PROCESS_STACK, MQTTSN_SUBSTATE_AWAKE, MQTTSN_SUBSTATE_SLEEP, true);
	}
	else {
		MQTTSN_DictateEvent_SuccessExecute(pClient, LISTEN_RUN_CTL, MQTTSN_SUBSTATE_SLEEP, MQTTSN_SUBSTATE_SLEEP, true);
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_Event_Aweak(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_Event_Aweak				: AWAKE
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_Event_Aweak(MQTTSN_ClientsTypeDef* pClient)
{
	MQTTSNString clientid = MQTTSNString_initializer;
	
	MQTTSN_DictateEvent_SetTime(pClient, 60);
	
	/* Pingreqing MQTTSN Server */
	clientid.cstring = MQTTSN_CLIENT_ID;
	if (MQTTSN_Pingreq(pClient, &clientid) != MQTTSN_OK) {
		/* Dictate execute is Fail */
		MQTTSN_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, MQTTSN_SUBSTATE_INIT, MQTTSN_SUBSTATE_AWAKE);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
		MQTTSN_DEBUG_LOG_PRINTF("MqttSN Ping Fail");
#endif
		return;
	}
	else {
		/* Dictate execute is Success */
		MQTTSN_DictateEvent_SuccessExecute(pClient, MQTTSN_PROCESS_STACK, MQTTSN_SUBSTATE_SLEEP, MQTTSN_SUBSTATE_AWAKE, true);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
		MQTTSN_DEBUG_LOG_PRINTF("MqttSN Ping Ok");
#endif
	}
	
	/* Set Pingreg Duration */
	MQTTSN_NormalDictateEvent_SetTime(pClient, &pClient->PingTimer, TNET_MQTTSN_PINGREG_DURATION);
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_Event_Lost(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_Event_Lost				: LOST
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_Event_Lost(MQTTSN_ClientsTypeDef* pClient)
{
	MQTTSN_DictateEvent_SuccessExecute(pClient, STOP_MODE, MQTTSN_SUBSTATE_INIT, MQTTSN_SUBSTATE_LOST, true);
}

/**********************************************************************************************************
 @Function			MQTTSN_StatusTypeDef messageHandlerFunction(MQTTSN_ClientsTypeDef* pClient, MQTTSN_MessageDataTypeDef* messageHandler)
 @Description			messageHandlerFunction		: MQTTSN接收处理
 @Input				pClient					: MqttSN客户端实例
					messageHandler				: 下行消息
 @Return				MQTTSN_StatusTypeDef		: MQTTSN处理状态
**********************************************************************************************************/
MQTTSN_StatusTypeDef messageHandlerFunction(MQTTSN_ClientsTypeDef* pClient, MQTTSN_MessageDataTypeDef* messageHandler)
{
	bool recvPCPUpgradeFlag = false;
	bool recvEffective = false;
	u16 recvBufOffset = 0;
	u16 recvMagicNum = 0;
	u16 recvIndex = 0;
	u8 ret = NETIP_OK;
	
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
	Radio_Trf_Printf("MqttSN Rv Filter:%s, topicid:%d", pClient->messageHandlers[0].topicFilter, pClient->messageHandlers[0].topicid);
	Radio_Trf_Printf("MqttSN Rv Len:%d", messageHandler->message->payloadlen);
#endif
	
	messageHandler->message->payload[messageHandler->message->payloadlen] = '\0';
	
	/* 私有普通下行数据 */
	for (int i = 0; i < messageHandler->message->payloadlen; i++) {
		if ((messageHandler->message->payload[i] == 'T') && (messageHandler->message->payload[i+1] == 'C') && \
		    (messageHandler->message->payload[i+2] == 'L') && (messageHandler->message->payload[i+3] == 'D')) {
			recvBufOffset = i;
			recvEffective = true;
			break;
		}
	}
	
	if (recvEffective != false) {
		/* Find "TCLD" */
		if (messageHandler->message->payload[recvBufOffset + TCLOD_MSGVERSION_OFFSET] == TCLOD_MSGVERSION) {
			if (messageHandler->message->payload[recvBufOffset + TCLOD_MSGID_OFFSET] == TCLOD_CONFIG_SET) {
				BEEP_CtrlRepeat_Extend(1, 300, 0);
				TCFG_EEPROM_SetNBCmdCnt(1 + TCFG_EEPROM_GetNBCmdCnt());
				
				/* 工作模式配置指令 */
				if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "Workmode") != NULL) {
			#if MQTTSN_DOWNLOAD_CMD_WORKMODE
					u16 mode;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, \
						"{(Workmode):{(val):%hu,(Magic):%hu}}", &mode, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						TCFG_SystemData.WorkMode = mode;
						if ((TCFG_SystemData.WorkMode != DEBUG_WORK) && (TCFG_SystemData.WorkMode != NORMAL_WORK)) {
							TCFG_SystemData.WorkMode = NORMAL_WORK;
							TCFG_EEPROM_SetWorkMode(TCFG_SystemData.WorkMode);
							ret = NETIP_ERRORPARAM;
						}
						else {
							TCFG_EEPROM_SetWorkMode(TCFG_SystemData.WorkMode);
						}
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
			#endif
				}
				/* 传感器灵敏度配置指令 */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "Sense") != NULL) {
			#if MQTTSN_DOWNLOAD_CMD_SENSE
					u16 sense;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, \
						"{(Sense):{(val):%hu,(Magic):%hu}}", &sense, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						TCFG_SystemData.Sensitivity = sense;
						if ((TCFG_SystemData.Sensitivity > SENSE_LOWEST) || (TCFG_SystemData.Sensitivity < SENSE_HIGHEST)) {
							TCFG_SystemData.Sensitivity = SENSE_MIDDLE;
							TCFG_EEPROM_SetSavedSensitivity(TCFG_SystemData.Sensitivity);
							ret = NETIP_ERRORPARAM;
						}
						else {
							TCFG_EEPROM_SetSavedSensitivity(TCFG_SystemData.Sensitivity);
						}
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
			#endif
				}
				/* 无线心跳间隔时间配置指令 */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "RFHeart") != NULL) {
			#if MQTTSN_DOWNLOAD_CMD_RFHEART
					u16 rfheart;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, \
						"{(RFHeart):{(val):%hu,(Magic):%hu}}", &rfheart, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						TCFG_SystemData.Heartinterval = rfheart;
						if ((TCFG_SystemData.Heartinterval > 120) || (TCFG_SystemData.Heartinterval < 1)) {
							TCFG_SystemData.Heartinterval = HEART_INTERVAL;
							TCFG_EEPROM_SetHeartinterval(TCFG_SystemData.Heartinterval);
							ret = NETIP_ERRORPARAM;
						}
						else {
							TCFG_EEPROM_SetHeartinterval(TCFG_SystemData.Heartinterval);
						}
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
			#endif
				}
				/* 初始化传感器指令 */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "Background") != NULL) {
			#if MQTTSN_DOWNLOAD_CMD_BACKGROUND
					u16 backgroundval;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, \
						"{(Background):{(val):%hu,(Magic):%hu}}", &backgroundval, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						if (((radar_targetinfo.strenth_total_diff > (backgroundval-5)) && (radar_targetinfo.strenth_total_diff < (backgroundval+5))) || (backgroundval == 0)) {
							Radar_InitBackground(TO_SAVE_RADAR_BACKGROUND);
							QMC5883L_InitBackgroud();
						}
						else {
							ret = NETIP_ERRORPARAM;
						}
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
			#endif
				}
				/* Reboot */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "Reboot") != NULL) {
			#if MQTTSN_DOWNLOAD_CMD_REBOOT
					BEEP_CtrlRepeat_Extend(2, 500, 250);
					Stm32_System_Software_Reboot(RBTMODE_MQTT_COMMAND);
			#endif
				}
				/* NewSn */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "Newsn") != NULL) {
			#if MQTTSN_DOWNLOAD_CMD_NEWSN
					u32 newsnval;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, \
						"{(Newsn):{(val):%08x,(Magic):%hu}}", &newsnval, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						TCFG_EEPROM_Set_MAC_SN(newsnval);
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
			#endif
				}
				/* Active */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "Active") != NULL) {
			#if MQTTSN_DOWNLOAD_CMD_ACTIVE
					u16 activeval;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, \
						"{(Active):{(val):%hu,(Magic):%hu}}", &activeval, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						TCFG_EEPROM_SetActiveDevice(activeval);
						if (activeval) {
							DeviceActivedMode = true;
							BEEP_CtrlRepeat_Extend(5,30,70);
						#if NBMQTTSN_SENDCODE_WORK_INFO
							NETMqttSNNeedSendCode.InfoWork = 1;
						#endif
						}
						else {
							DeviceActivedMode = false;
							BEEP_CtrlRepeat_Extend(1,500,0);
						}
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
			#endif
				}
				/* MagMod */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "MagMod") != NULL) {
			#if MQTTSN_DOWNLOAD_CMD_MAGMOD
					u16 magmodval;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, \
						"{(MagMod):{(val):%hu,(Magic):%hu}}", &magmodval, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						TCFG_EEPROM_SetMagMode(magmodval);
						talgo_set_magmod(magmodval);
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
			#endif
				}
				/* NbHeart */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "NbHeart") != NULL) {
			#if MQTTSN_DOWNLOAD_CMD_NBHEART
					u16 nbheartval;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, \
						"{(NbHeart):{(val):%hu,(Magic):%hu}}", &nbheartval, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						TCFG_EEPROM_SetNbiotHeart(nbheartval);
						TCFG_SystemData.NBIotHeart = TCFG_EEPROM_GetNbiotHeart();
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
			#endif
				}
				/* InitRadar */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "InitRadar") != NULL) {
			#if MQTTSN_DOWNLOAD_CMD_INITRADAR
					u32 i32, j32, k32, m32;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, \
						"{(InitRadar):{(v23456):%u,(v7890a):%u,(vbcdef):%u,(vg):%u,(Magic):%hu}}", &i32, &j32, &k32, &m32, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						Radar_InitBG_Cmd(i32, j32, k32, m32);
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
			#endif
				}
				/* InitMag */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "InitMag") != NULL) {
			#if MQTTSN_DOWNLOAD_CMD_INITMAG
					s16 x, y, z;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, \
						"{(InitMag):{(x):%hd,(y):%hd,(z):%hd,(Magic):%hu}}", &x, &y, &z, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						QMC5883L_InitBackgroud_cmd(x, y, z);
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
			#endif
				}
				/* DisRange */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "DisRange") != NULL) {
			#if MQTTSN_DOWNLOAD_CMD_DISRANGE
					u16 disrangeval;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, \
						"{(DisRange):{(val):%hu,(Magic):%hu}}", &disrangeval, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						tradar_set_distance_range(disrangeval + 4);
						TCFG_EEPROM_SetRadarRange(disrangeval);
						TCFG_SystemData.RadarRange = TCFG_EEPROM_GetRadarRange();
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
			#endif
				}
				/* CarInDelay */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "InDelay") != NULL) {
			#if MQTTSN_DOWNLOAD_CMD_CARINDELAY
					u16 indelayval;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, \
						"{(InDelay):{(val):%hu,(Magic):%hu}}", &indelayval, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						TCFG_EEPROM_SetCarInDelay(indelayval);
						TCFG_SystemData.CarInDelay = TCFG_EEPROM_GetCarInDelay();
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
			#endif
				}
				/* MagTempCoef */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "MagCoef") != NULL) {
			#if MQTTSN_DOWNLOAD_CMD_MAGTEMPCOEF
					short magTempCoefX, magTempCoefY, magTempCoefZ;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, \
						"{(MagCoef):{(x):%hd,(y):%hd,(z):%hd,(Magic):%hu}}", &magTempCoefX, &magTempCoefY, &magTempCoefZ, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						TCFG_SystemData.MagCoefX = magTempCoefX;
						TCFG_SystemData.MagCoefY = magTempCoefY;
						TCFG_SystemData.MagCoefZ = magTempCoefZ;
						TCFG_EEPROM_SetMagTempCoef(TCFG_SystemData.MagCoefX, TCFG_SystemData.MagCoefY, TCFG_SystemData.MagCoefZ);
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
			#endif
				}
				/* SetQmcCoef */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "SetQmcCoef") != NULL) {
			#if MQTTSN_DOWNLOAD_CMD_SETQMCCOEF
					short magTempCoefX, magTempCoefY, magTempCoefZ;
					QMC5883L_measure_qmc_coef((signed char*)&magTempCoefX, (signed char*)&magTempCoefY, (signed char*)&magTempCoefZ);
					TCFG_SystemData.MagCoefX = magTempCoefX;
					TCFG_SystemData.MagCoefY = magTempCoefY;
					TCFG_SystemData.MagCoefZ = magTempCoefZ;
					TCFG_EEPROM_SetMagTempCoef(TCFG_SystemData.MagCoefX, TCFG_SystemData.MagCoefY, TCFG_SystemData.MagCoefZ);
			#endif
				}
				/* BeepOff */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "BeepOff") != NULL) {
			#if MQTTSN_DOWNLOAD_CMD_BEEPOFF
					u16 beepoff;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, \
						"{(BeepOff):{(val):%hu,(Magic):%hu}}", &beepoff, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						TCFG_SystemData.BeepCtrlOff = beepoff;
						TCFG_EEPROM_SetBeepOff(TCFG_SystemData.BeepCtrlOff);
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
			#endif
				}
				/* UpLimit */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "UpLimit") != NULL) {
			#if MQTTSN_DOWNLOAD_CMD_UPLIMIT
					short limitRssi, limitSnr;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, \
						"{(UpLimit):{(rssi):%hd,(snr):%hd,(Magic):%hu}}", &limitRssi, &limitSnr, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						TCFG_SystemData.UpgradeLimitRssi = limitRssi;
						TCFG_SystemData.UpgradeLimitSnr = limitSnr;
						TCFG_EEPROM_SetUpgradeLimitRssi(TCFG_SystemData.UpgradeLimitRssi);
						TCFG_EEPROM_SetUpgradeLimitSnr(TCFG_SystemData.UpgradeLimitSnr);
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
			#endif
				}
				/* CoverGain */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "CoverGain") != NULL) {
			#if MQTTSN_DOWNLOAD_CMD_COVERGAIN
					uint16_t CoverGain;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, \
						"{(CoverGain):{(val):%hu,(Magic):%hu}", &CoverGain, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						if ((CoverGain < RADAR_COVERGAIN_LOW) || (CoverGain > RADAR_COVERGAIN_HIGH)) {
							CoverGain = RADAR_COVERGAIN_DEFAULT;
						}
						if (TCFG_SystemData.CoverGain != CoverGain) {
							Radar_UpdateBG_Cmd(TCFG_SystemData.CoverGain, CoverGain);
							TCFG_SystemData.CoverGain = CoverGain;
							TCFG_EEPROM_SetCoverGain(TCFG_SystemData.CoverGain);
						}
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
			#endif
				}
				/* RadarGain */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "RadarGain") != NULL) {
			#if MQTTSN_DOWNLOAD_CMD_RADARGAIN
					uint16_t RadarGain;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, \
						"{(RadarGain):{(val):%hu,(Magic):%hu}", &RadarGain, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						if ((RadarGain < TRADAR_GAIN_LOWEST) || (RadarGain > TRADAR_GAIN_HIGHEST)) {
							RadarGain = TRADAR_GAIN_DEFAULT;
						}
						if (TCFG_SystemData.RadarGain != RadarGain) {
							TCFG_SystemData.RadarGain = RadarGain;
							TCFG_EEPROM_SetRadarGain(TCFG_SystemData.RadarGain);
						}
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
			#endif
				}
				/* SenseMode */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "RadarGain") != NULL) {
			#if MQTTSN_DOWNLOAD_CMD_SENSEMODE
					uint16_t SenseMode;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, \
						"{(SenseMode):{(val):%hu,(Magic):%hu}", &SenseMode, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						if (TCFG_SystemData.SenseMode != SenseMode) {
							TCFG_SystemData.SenseMode = SenseMode;
							TCFG_EEPROM_SetSenseMode(TCFG_SystemData.SenseMode);
						}
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
			#endif
				}
				/* ...... */
			}
			else if (messageHandler->message->payload[recvBufOffset + TCLOD_MSGID_OFFSET] == TCLOD_CONFIG_GET) {
				BEEP_CtrlRepeat_Extend(2, 50, 50);
				/* InfoWork */
				if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "Workinfo") != NULL) {
			#if NBMQTTSN_SENDCODE_WORK_INFO
					NETMqttSNNeedSendCode.InfoWork = 1;
			#endif
				}
				/* InfoBasic */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "BasicInfo") != NULL) {
			#if NBMQTTSN_SENDCODE_BASIC_INFO
					NETMqttSNNeedSendCode.InfoBasic = 1;
			#endif
				}
				/* InfoDynamic */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "DynamicInfo") != NULL) {
			#if NBMQTTSN_SENDCODE_DYNAMIC_INFO
					NETMqttSNNeedSendCode.InfoDynamic = 1;
			#endif
				}
				/* ...... */
			}
		}
		else {
			ret = NETIP_NOTSUPPORT;
		}
		
		NETMqttSNNeedSendCode.InfoResponseErrcode = ret;
		NETMqttSNNeedSendCode.InfoResponse = 1;
	}
	else {
		/* Not Valid */
		ret = NETIP_NOTVALID;
	}
	
	/* MqttSNPCP升级协议下行数据 */
	for (int i = 0; i < messageHandler->message->payloadlen; i++) {
		if ((messageHandler->message->payload[recvIndex] == 0xFF) && (messageHandler->message->payload[recvIndex+1] == 0xFE)) {
			recvBufOffset = recvIndex;
			recvPCPUpgradeFlag = true;
			break;
		}
		recvIndex++;
	}
	
	if (recvPCPUpgradeFlag != false) {
		/* Find "0xFFFE" */
		NET_MqttSN_PCP_Message_RecvDataEnqueue(messageHandler->message->payload + recvBufOffset, messageHandler->message->payloadlen - recvBufOffset);
	}
	
	return MQTTSN_OK;
}

/**********************************************************************************************************
 @Function			MQTTSN_StatusTypeDef NET_MQTTSN_SendPayloadPacket(MQTTSN_ClientsTypeDef* pClient, NET_MQTTSN_ObjectPacketTypeDef ObjectPacket)
 @Description			NET_MQTTSN_SendPayloadPacket	: MQTTSN发送负载包
 @Input				pClient					: MqttSN客户端实例
					ObjectPacket				: 对象类型
 @Return				MQTTSN_StatusTypeDef		: MQTTSN处理状态
**********************************************************************************************************/
MQTTSN_StatusTypeDef NET_MQTTSN_SendPayloadPacket(MQTTSN_ClientsTypeDef* pClient, NET_MQTTSN_ObjectPacketTypeDef ObjectPacket)
{
	MQTTSN_StatusTypeDef MQTTSNStatus = MQTTSN_OK;
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
	NET_Message_TcldMsgTypeDef *pMsg = (NET_Message_TcldMsgTypeDef*)pClient->DataProcessStack;
#endif
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM
	unsigned char* pMsg = pClient->DataProcessStack;
	unsigned short pMsgLen = 0;
#endif
	MQTTSN_MessageTypeDef message;
	MQTTSN_topicid topic;
	
	memset(pClient->DataProcessStack, 0x0, pClient->DataProcessStack_size);
	
	message.qos		= QOS1;
	message.msgid		= 1;
	message.dup		= 0;
	message.retained	= 0;
	message.payload	= pClient->DataProcessStack;
	
	/* seclect different data and send */
	switch (ObjectPacket)
	{
		case OBJECT_TYPE_TMOTES_STATUS_BASIC_PUT:
		{
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
#if NBMQTTSN_SENDCODE_STATUS_BASIC
			NET_MESSAGE_GET_MAGICNUM(pMsg->MsgHead.MagicNum);
			pMsg->MsgHead.MsgType		=	MSG_JSON;
			pMsg->MsgHead.Version		=	MESSAGE_VERSION;
			pMsg->MsgHead.EncryptMode	=	ENCRYPT_NONE;
			pMsg->MsgHead.MsgId			=	MSGID_PUT;
			
			message.payloadlen			=	NET_Message_Operate_Creat_Json_MoteStatus_Basic(pMsg->pData) + sizeof(NET_Message_TcldHeadTypeDef);
			
			topic.type = MQTTSN_TOPIC_TYPE_PREDEFINED;
			topic.data.id = TOPICID_MOTESTATUS;
			if ((MQTTSNStatus = MQTTSN_Publish(pClient, topic, &message)) != MQTTSN_OK) {
				//Todo
			}
#endif
#endif
			break;
		}
		
		case OBJECT_TYPE_TMOTES_STATUS_EXTEND_PUT:
		{
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
#if NBMQTTSN_SENDCODE_STATUS_EXTEND
			NET_MESSAGE_GET_MAGICNUM(pMsg->MsgHead.MagicNum);
			pMsg->MsgHead.MsgType		=	MSG_JSON;
			pMsg->MsgHead.Version		=	MESSAGE_VERSION;
			pMsg->MsgHead.EncryptMode	=	ENCRYPT_NONE;
			pMsg->MsgHead.MsgId			=	MSGID_PUT;
			
			message.payloadlen			=	NET_Message_Operate_Creat_Json_MoteStatus_Extend(pMsg->pData) + sizeof(NET_Message_TcldHeadTypeDef);
			
			topic.type = MQTTSN_TOPIC_TYPE_PREDEFINED;
			topic.data.id = TOPICID_MOTESTATUS;
			if ((MQTTSNStatus = MQTTSN_Publish(pClient, topic, &message)) != MQTTSN_OK) {
				//Todo
			}
#endif
#endif
			break;
		}
		
		case OBJECT_TYPE_TMOTES_INFO_WORK_PUT:
		{
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
#if NBMQTTSN_SENDCODE_WORK_INFO
			NET_MESSAGE_GET_MAGICNUM(pMsg->MsgHead.MagicNum);
			pMsg->MsgHead.MsgType		=	MSG_JSON;
			pMsg->MsgHead.Version		=	MESSAGE_VERSION;
			pMsg->MsgHead.EncryptMode	=	ENCRYPT_NONE;
			pMsg->MsgHead.MsgId			=	MSGID_PUT;
			
			message.payloadlen			=	NET_Message_Operate_Creat_Json_MoteInfo_Work(pMsg->pData) + sizeof(NET_Message_TcldHeadTypeDef);
			
			topic.type = MQTTSN_TOPIC_TYPE_PREDEFINED;
			topic.data.id = TOPICID_MOTEINFO;
			if ((MQTTSNStatus = MQTTSN_Publish(pClient, topic, &message)) != MQTTSN_OK) {
				//Todo
			}
#endif
#endif
			break;
		}
		
		case OBJECT_TYPE_TMOTES_INFO_BASIC_PUT:
		{
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
#if NBMQTTSN_SENDCODE_BASIC_INFO
			NET_MESSAGE_GET_MAGICNUM(pMsg->MsgHead.MagicNum);
			pMsg->MsgHead.MsgType		=	MSG_JSON;
			pMsg->MsgHead.Version		=	MESSAGE_VERSION;
			pMsg->MsgHead.EncryptMode	=	ENCRYPT_NONE;
			pMsg->MsgHead.MsgId			=	MSGID_PUT;
			
			message.payloadlen			=	NET_Message_Operate_Creat_Json_MoteInfo_Basic(pMsg->pData) + sizeof(NET_Message_TcldHeadTypeDef);
			
			topic.type = MQTTSN_TOPIC_TYPE_PREDEFINED;
			topic.data.id = TOPICID_MOTEINFO;
			if ((MQTTSNStatus = MQTTSN_Publish(pClient, topic, &message)) != MQTTSN_OK) {
				//Todo
			}
#endif
#endif
			break;
		}
		
		case OBJECT_TYPE_TMOTES_INFO_DYNAMIC_PUT:
		{
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
#if NBMQTTSN_SENDCODE_DYNAMIC_INFO
			NET_MESSAGE_GET_MAGICNUM(pMsg->MsgHead.MagicNum);
			pMsg->MsgHead.MsgType		=	MSG_JSON;
			pMsg->MsgHead.Version		=	MESSAGE_VERSION;
			pMsg->MsgHead.EncryptMode	=	ENCRYPT_NONE;
			pMsg->MsgHead.MsgId			=	MSGID_PUT;
			
			message.payloadlen			=	NET_Message_Operate_Creat_Json_MoteInfo_Dynamic(pMsg->pData) + sizeof(NET_Message_TcldHeadTypeDef);
			
			topic.type = MQTTSN_TOPIC_TYPE_PREDEFINED;
			topic.data.id = TOPICID_MOTEINFO;
			if ((MQTTSNStatus = MQTTSN_Publish(pClient, topic, &message)) != MQTTSN_OK) {
				//Todo
			}
#endif
#endif
			break;
		}
		
		case OBJECT_TYPE_TMOTES_INFO_RESPONSE_PUT:
		{
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
			NET_MESSAGE_GET_MAGICNUM(pMsg->MsgHead.MagicNum);
			pMsg->MsgHead.MsgType		=	MSG_JSON;
			pMsg->MsgHead.Version		=	MESSAGE_VERSION;
			pMsg->MsgHead.EncryptMode	=	ENCRYPT_NONE;
			pMsg->MsgHead.MsgId			=	MSGID_PUT;
			
			message.payloadlen			=	NET_Message_Operate_Creat_Json_MoteInfo_Response(pMsg->pData) + sizeof(NET_Message_TcldHeadTypeDef);
			
			topic.type = MQTTSN_TOPIC_TYPE_PREDEFINED;
			topic.data.id = TOPICID_MOTEINFO;
			if ((MQTTSNStatus = MQTTSN_Publish(pClient, topic, &message)) != MQTTSN_OK) {
				//Todo
			}
#endif
			break;
		}
		
		case OBJECT_TYPE_TMOTES_BYTE_STREAM_PUT:
		{
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM
			NET_MqttSN_Message_SendDataDequeue(pMsg, &pMsgLen);
			
			message.payloadlen			=	pMsgLen;
			
			topic.type = MQTTSN_TOPIC_TYPE_PREDEFINED;
			topic.data.id = TOPICID_STANDARD;
			if ((MQTTSNStatus = MQTTSN_Publish(pClient, topic, &message)) != MQTTSN_OK) {
				//Todo
			}
#endif
			break;
		}
	}
	
	return MQTTSNStatus;
}


/**********************************************************************************************************
 @Function			void NET_MQTTSN_Listen_PollExecution(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_Listen_PollExecution		: MQTTSN监听器处理
 @Input				pClient							: MQTTSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_Listen_PollExecution(MQTTSN_ClientsTypeDef* pClient)
{
	switch (pClient->ListenRunCtl.listenEvent)
	{
	case NBMQTTSN_LISTEN_MODE_ENTER_NONE:
		NET_MQTTSN_NBIOT_Listen_Enable_EnterNone(pClient);
		break;
	
	case NBMQTTSN_LISTEN_MODE_ENTER_PARAMETER:
#if NBMQTTSN_LISTEN_PARAMETER_TYPE == NBMQTTSN_LISTEN_PARAMETER_ENABLE
		NET_MQTTSN_NBIOT_Listen_Event_EnterParameter(pClient);
#endif
		break;
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Listen_Enable_EnterNone(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Listen_Enable_EnterNone	: 事件(进入None模式)监听
 @Input				pClient								: MQTTSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Listen_Enable_EnterNone(MQTTSN_ClientsTypeDef* pClient)
{
	pClient->DictateRunCtl.dictateEnable = false;
	pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
	pClient->SubState = MQTTSN_SUBSTATE_SLEEP;
	pClient->ListenRunCtl.listenEvent = NBMQTTSN_LISTEN_DEFAULT_BOOTMODE;
}

#if NBMQTTSN_LISTEN_PARAMETER_TYPE == NBMQTTSN_LISTEN_PARAMETER_ENABLE
/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Listen_Enable_EnterParameter(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Listen_Enable_EnterParameter: 使能(进入NBIOT运行信息)监听
 @Input				pClient								: MQTTSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Listen_Enable_EnterParameter(MQTTSN_ClientsTypeDef* pClient)
{
	Stm32_CalculagraphTypeDef listenRunTime;
	
	/* Listen Enable */
	if (pClient->ListenRunCtl.ListenEnterParameter.listenEnable == true) {
		pClient->ListenRunCtl.ListenEnterParameter.listenStatus = true;
		Stm32_Calculagraph_CountdownSec(&listenRunTime, pClient->ListenRunCtl.ListenEnterParameter.listenTimereachSec);
		pClient->ListenRunCtl.ListenEnterParameter.listenRunTime = listenRunTime;
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Listen_Event_EnterParameter(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Listen_Event_EnterParameter	: 事件(进入NBIOT运行信息)监听
 @Input				pClient								: MQTTSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Listen_Event_EnterParameter(MQTTSN_ClientsTypeDef* pClient)
{
	Stm32_CalculagraphTypeDef eventRunTime;
	
	if ((pClient->ListenRunCtl.ListenEnterParameter.listenEnable == true) && (pClient->ListenRunCtl.ListenEnterParameter.listenStatus == true)) {
		if (Stm32_Calculagraph_IsExpiredSec(&pClient->ListenRunCtl.ListenEnterParameter.listenRunTime) == true) {
			
			/* It is the first time to execute */
			if (pClient->ListenRunCtl.ListenEnterParameter.EventCtl.eventEnable != true) {
				pClient->ListenRunCtl.ListenEnterParameter.EventCtl.eventEnable = true;
				pClient->ListenRunCtl.ListenEnterParameter.EventCtl.eventTimeoutSec = 30;
				Stm32_Calculagraph_CountdownSec(&eventRunTime, pClient->ListenRunCtl.ListenEnterParameter.EventCtl.eventTimeoutSec);
				pClient->ListenRunCtl.ListenEnterParameter.EventCtl.eventRunTime = eventRunTime;
			}
			
			if ((NBIOT_Neul_NBxx_CheckReadRSSI(pClient->SocketStack->NBIotStack) == NBIOT_OK) &&
			    (NBIOT_Neul_NBxx_CheckReadStatisticsRADIO(pClient->SocketStack->NBIotStack) == NBIOT_OK)) {
				/* Dictate execute is Success */
				pClient->ListenRunCtl.ListenEnterParameter.EventCtl.eventEnable = false;
				pClient->ListenRunCtl.listenEvent = NBMQTTSN_LISTEN_MODE_ENTER_PARAMETER;
				pClient->ListenRunCtl.ListenEnterParameter.EventCtl.eventFailureCnt = 0;
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
				MQTTSN_DEBUG_LOG_PRINTF("MqttSN Para Check Ok");
				Radio_Trf_Printf("RSSI:%d", pClient->SocketStack->NBIotStack->Parameter.rssi);
				Radio_Trf_Printf("SNR:%d", pClient->SocketStack->NBIotStack->Parameter.statisticsRADIO.SNR);
#endif
			}
			else {
				/* Dictate execute is Fail */
				if (Stm32_Calculagraph_IsExpiredSec(&pClient->ListenRunCtl.ListenEnterParameter.EventCtl.eventRunTime) == true) {
					/* Dictate TimeOut */
					pClient->ListenRunCtl.ListenEnterParameter.EventCtl.eventEnable = false;
					pClient->ListenRunCtl.listenEvent = NBMQTTSN_LISTEN_MODE_ENTER_PARAMETER;
					pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEnable = false;
					pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
					pClient->SubState = MQTTSN_SUBSTATE_INIT;
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_DISABLE
					pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
#endif
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_ENABLE
					pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_DNS;
#endif
					pClient->ListenRunCtl.ListenEnterParameter.EventCtl.eventFailureCnt++;
					if (pClient->ListenRunCtl.ListenEnterParameter.EventCtl.eventFailureCnt > 3) {
						pClient->ListenRunCtl.ListenEnterParameter.EventCtl.eventFailureCnt = 0;
						pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
						pClient->SubState = MQTTSN_SUBSTATE_LOST;
						pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
					}
				}
				else {
					/* Dictate isn't TimeOut */
					pClient->ListenRunCtl.listenEvent = NBMQTTSN_LISTEN_MODE_ENTER_PARAMETER;
				}
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
				MQTTSN_DEBUG_LOG_PRINTF("MqttSN Para Check Fail");
#endif
				return;
			}
			
			pClient->ListenRunCtl.ListenEnterParameter.listenEnable = false;
			pClient->ListenRunCtl.ListenEnterParameter.listenStatus = false;
			pClient->ListenRunCtl.listenEvent = NBMQTTSN_LISTEN_MODE_ENTER_PARAMETER;
		}
	}
	
	pClient->DictateRunCtl.dictateEnable = false;
	pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
	pClient->SubState = MQTTSN_SUBSTATE_SLEEP;
	pClient->ListenRunCtl.listenEvent = NBMQTTSN_LISTEN_DEFAULT_BOOTMODE;
}
#endif

/********************************************** END OF FLEE **********************************************/
