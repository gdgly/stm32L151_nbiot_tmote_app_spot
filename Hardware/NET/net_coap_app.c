/**
  *********************************************************************************************************
  * @file    net_coap_app.c
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

#include "net_coap_app.h"
#include "platform_config.h"
#include "platform_map.h"
#include "hal_rtc.h"
#include "hal_beep.h"
#include "radar_api.h"
#include "hal_qmc5883l.h"
#include "string.h"
#include "radio_rf_app.h"

/**********************************************************************************************************
 @Function			void NET_COAP_APP_PollExecution(NBIOT_ClientsTypeDef* pClient)
 @Description			COAP_APP_PollExecution				: COAP逻辑处理
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_APP_PollExecution(NBIOT_ClientsTypeDef* pClient)
{
	switch (pClient->DictateRunCtl.dictateEvent)
	{
	case STOP_MODE:
		NET_COAP_NBIOT_Event_StopMode(pClient);
		break;
	
	case HARDWARE_REBOOT:
		NET_COAP_NBIOT_Event_HardwareReboot(pClient);
		break;
	
	case REPORT_ERROE:
		NET_COAP_NBIOT_Event_ReportError(pClient);
		break;
	
	case MODULE_CHECK:
		NET_COAP_NBIOT_Event_ModuleCheck(pClient);
		break;
	
	case PARAMETER_CONFIG:
		NET_COAP_NBIOT_Event_ParameterConfig(pClient);
		break;
	
	case ICCID_CHECK:
		NET_COAP_NBIOT_Event_SimICCIDCheck(pClient);
		break;
	
	case MISC_EQUIP_CONFIG:
		NET_COAP_NBIOT_Event_MiscEquipConfig(pClient);
		break;
	
	case ATTACH_CHECK:
		NET_COAP_NBIOT_Event_AttachCheck(pClient);
		break;
	
	case ATTACH_EXECUTE:
		NET_COAP_NBIOT_Event_AttachExecute(pClient);
		break;
	
	case ATTACH_INQUIRE:
		NET_COAP_NBIOT_Event_AttachInquire(pClient);
		break;
	
	case PARAMETER_CHECKOUT:
		NET_COAP_NBIOT_Event_ParameterCheckOut(pClient);
		break;
	
	case MINIMUM_FUNCTIONALITY:
		NET_COAP_NBIOT_Event_MinimumFunctionality(pClient);
		break;
	
	case FULL_FUNCTIONALITY:
		NET_COAP_NBIOT_Event_FullFunctionality(pClient);
		break;
	
	case CLEAR_STORED_EARFCN:
#if NBIOT_CLEAR_STORED_EARFCN_STAT
		NET_COAP_NBIOT_Event_ClearStoredEARFCN(pClient);
#else
		pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
#endif
		break;
	
	case CDP_SERVER_CHECK:
		NET_COAP_NBIOT_Event_CDPServerCheck(pClient);
		break;
	
	case CDP_SERVER_CONFIG:
		NET_COAP_NBIOT_Event_CDPServerConfig(pClient);
		break;
	
	case NBAND_MODE_CHECK:
		NET_COAP_NBIOT_Event_NbandModeCheck(pClient);
		break;
	
	case NBAND_MODE_CONFIG:
		NET_COAP_NBIOT_Event_NbandModeConfig(pClient);
		break;
	
	case SEND_DATA:
#if NBCOAP_SENDMODE_CONFIG_TYPE == NBCOAP_SENDMODE_NORMAL_MODE
		NET_COAP_NBIOT_Event_SendData(pClient);
#endif
		break;
	
	case RECV_DATA:
#if NBCOAP_SENDMODE_CONFIG_TYPE == NBCOAP_SENDMODE_NORMAL_MODE
		NET_COAP_NBIOT_Event_RecvData(pClient);
#endif
		break;
	
	case SEND_DATA_RA_NORMAL:
#if NBCOAP_SENDMODE_CONFIG_TYPE == NBCOAP_SENDMODE_RAIDLE_MODE
		NET_COAP_NBIOT_Event_SendDataRANormal(pClient);
#endif
		break;
	
	case RECV_DATA_RA_NORMAL:
#if NBCOAP_SENDMODE_CONFIG_TYPE == NBCOAP_SENDMODE_RAIDLE_MODE
		NET_COAP_NBIOT_Event_RecvDataRANormal(pClient);
#endif
		break;
	
	case EXECUT_DOWNLINK_DATA:
		NET_COAP_NBIOT_Event_ExecutDownlinkData(pClient);
		break;
	
	case MQTTSN_PROCESS_STACK:
		pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case DNS_PROCESS_STACK:
		pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case ONENET_PROCESS_STACK:
		pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case LISTEN_RUN_CTL:
		NET_COAP_Listen_PollExecution(pClient);
		break;
	
	default :
		pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	}
}

/**********************************************************************************************************
 @Function			static void COAP_NBIOT_DictateEvent_SetTime(NBIOT_ClientsTypeDef* pClient, unsigned int TimeoutSec)
 @Description			COAP_NBIOT_DictateEvent_SetTime		: 事件运行控制器注入时间(内部使用)
 @Input				pClient							: NBIOT客户端实例
					TimeoutSec						: 注入超时时间
 @Return				void
 @attention			事件运行之前判断是否需要注入时间
**********************************************************************************************************/
static void COAP_NBIOT_DictateEvent_SetTime(NBIOT_ClientsTypeDef* pClient, unsigned int TimeoutSec)
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

static unsigned char* COAP_NBIOT_GetDictateFailureCnt(NBIOT_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateNoTimeOut)
{
	unsigned char* dictateFailureCnt;
	
	switch (dictateNoTimeOut)
	{
	case HARDWARE_REBOOT:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateRebootFailureCnt;
		break;
	
	case REPORT_ERROE:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateReportErrorFailureCnt;
		break;
	
	case MODULE_CHECK:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateModuleCheckFailureCnt;
		break;
	
	case PARAMETER_CONFIG:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateParameterConfigFailureCnt;
		break;
	
	case ICCID_CHECK:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateSimICCIDCheckFailureCnt;
		break;
	
	case FULL_FUNCTIONALITY:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateFullFunctionalityFailureCnt;
		break;
	
	case MINIMUM_FUNCTIONALITY:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateMinimumFunctionalityFailureCnt;
		break;
	
	case CLEAR_STORED_EARFCN:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateClearStoredEARFCNFailureCnt;
		break;
	
	case CDP_SERVER_CHECK:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateCDPServerCheckFailureCnt;
		break;
	
	case CDP_SERVER_CONFIG:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateCDPServerConfigFailureCnt;
		break;
	
	case NBAND_MODE_CHECK:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateNbandModeCheckFailureCnt;
		break;
	
	case NBAND_MODE_CONFIG:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateNbandModeConfigFailureCnt;
		break;
	
	case MISC_EQUIP_CONFIG:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateMiscEquipConfigFailureCnt;
		break;
	
	case ATTACH_CHECK:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateAttachCheckFailureCnt;
		break;
	
	case ATTACH_EXECUTE:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateAttachExecuteFailureCnt;
		break;
	
	case ATTACH_INQUIRE:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateAttachInquireFailureCnt;
		break;
	
	case PARAMETER_CHECKOUT:
		dictateFailureCnt = &pClient->DictateRunCtl.dictatePatameterCheckOutFailureCnt;
		break;
	
#if NBCOAP_SENDMODE_CONFIG_TYPE == NBCOAP_SENDMODE_NORMAL_MODE
	case SEND_DATA:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateSendDataFailureCnt;
		break;
	
	case RECV_DATA:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateRecvDataFailureCnt;
		break;
#endif
	
#if NBCOAP_SENDMODE_CONFIG_TYPE == NBCOAP_SENDMODE_RAIDLE_MODE
	case SEND_DATA_RA_NORMAL:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateSendDataRANormalFailureCnt;
		break;
	
	case RECV_DATA_RA_NORMAL:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateRecvDataRANormalFailureCnt;
		break;
#endif
	
	default :
		dictateFailureCnt = &pClient->DictateRunCtl.dictateRebootFailureCnt;
		break;
	}
	
	return dictateFailureCnt;
}

/**********************************************************************************************************
 @Function			static void COAP_NBIOT_DictateEvent_FailExecute(NBIOT_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateTimeOut, \
																				    NBIOT_DictateEventTypeDef dictateFail, \
																				    NBIOT_DictateEventTypeDef dictateNoTimeOut)
 @Description			COAP_NBIOT_DictateEvent_FailExecute	: 事件运行控制器出错执行(内部使用)
 @Input				pClient							: NBIOT客户端实例
					dictateTimeOut						: 事假处理错误超时
					dictateFail						: 事件处理错误次数溢出
					dictateNoTimeOut					: 事假处理错误未超时
 @Return				void
**********************************************************************************************************/
static void COAP_NBIOT_DictateEvent_FailExecute(NBIOT_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateTimeOut, \
															    NBIOT_DictateEventTypeDef dictateFail, \
															    NBIOT_DictateEventTypeDef dictateNoTimeOut)
{
	unsigned char* dictateFailureCnt;
	
	dictateFailureCnt = COAP_NBIOT_GetDictateFailureCnt(pClient, dictateNoTimeOut);
	
	if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
		/* Dictate TimeOut */
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = dictateTimeOut;
		*dictateFailureCnt += 1;
		if (*dictateFailureCnt > 3) {
			*dictateFailureCnt = 0;
			pClient->DictateRunCtl.dictateEvent = dictateFail;
		}
	}
	else {
		/* Dictate isn't TimeOut */
		pClient->DictateRunCtl.dictateEvent = dictateNoTimeOut;
	}
}

/**********************************************************************************************************
 @Function			static void COAP_NBIOT_DictateEvent_SuccessExecute(NBIOT_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateSuccess, \
																					  NBIOT_DictateEventTypeDef dictateNoTimeOut)
 @Description			COAP_NBIOT_DictateEvent_SuccessExecute	: 事件运行控制器正确执行(内部使用)
 @Input				pClient							: NBIOT客户端实例
					dictateSuccess						: 事假处理正确跳转
					dictateNoTimeOut					: 事假处理错误未超时
 @Return				void
**********************************************************************************************************/
static void COAP_NBIOT_DictateEvent_SuccessExecute(NBIOT_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateSuccess, \
																  NBIOT_DictateEventTypeDef dictateNoTimeOut)
{
	unsigned char* dictateFailureCnt;
	
	dictateFailureCnt = COAP_NBIOT_GetDictateFailureCnt(pClient, dictateNoTimeOut);
	
	pClient->DictateRunCtl.dictateEnable = false;
	pClient->DictateRunCtl.dictateEvent = dictateSuccess;
	*dictateFailureCnt = 0;
}

/**********************************************************************************************************
 @Function			static void COAP_NBIOT_GetConnectTime(NBIOT_ClientsTypeDef* pClient, bool startIdleTime)
 @Description			COAP_NBIOT_GetConnectTime			: 获取NBConnect状态时间(内部使用)
 @Input				pClient							: NBIOT客户端实例
					startIdleTime						: 是否开启Idle状态计时器
 @Return				void
**********************************************************************************************************/
static void COAP_NBIOT_GetConnectTime(NBIOT_ClientsTypeDef* pClient, bool startIdleTime)
{
	unsigned int uICoapConnectTime = 0;
	
	/* Get ConnectTime */
	uICoapConnectTime = Stm32_EventRunningTime_EndMS(&pClient->ConnectTimeMS) / 1000;
	/* End ConnectTime */
	TCFG_SystemData.CoapConnectTime = pClient->CoapConnectTimeSec + uICoapConnectTime;
	pClient->CoapConnectTimeSec = TCFG_SystemData.CoapConnectTime;
	/* Start or End IdleTime */
	if (startIdleTime != false) {
		Stm32_EventRunningTime_StartMS(&pClient->IdleTimeMS);
	}
	else {
		Stm32_EventRunningTime_EndMS(&pClient->IdleTimeMS);
	}
}

/**********************************************************************************************************
 @Function			static void COAP_NBIOT_GetIdleTime(NBIOT_ClientsTypeDef* pClient, bool startConnectTime)
 @Description			COAP_NBIOT_GetIdleTime				: 获取NBIdle状态时间(内部使用)
 @Input				pClient							: NBIOT客户端实例
					startConnectTime					: 是否开启Connect状态计时器
 @Return				void
**********************************************************************************************************/
static void COAP_NBIOT_GetIdleTime(NBIOT_ClientsTypeDef* pClient, bool startConnectTime)
{
	unsigned int uICoapIdleTime = 0;
	
	/* Get IdleTime */
	uICoapIdleTime = Stm32_EventRunningTime_EndMS(&pClient->IdleTimeMS) / 1000;
	/* End IdleTime */
	TCFG_SystemData.CoapIdleTime = pClient->CoapIdleTimeSec + uICoapIdleTime;
	pClient->CoapIdleTimeSec = TCFG_SystemData.CoapIdleTime;
	/* Start or End ConnectTime */
	if (startConnectTime != false) {
		Stm32_EventRunningTime_StartMS(&pClient->ConnectTimeMS);
	}
	else {
		Stm32_EventRunningTime_EndMS(&pClient->ConnectTimeMS);
	}
}

/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_StopMode(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_StopMode			: 停止模式
 @Input				pClient							: NBIOT客户端实例
 @Return				void
 @attention			当30分钟或有数据需要发送时退出停止模式尝试重启NB,开启NB运行
**********************************************************************************************************/
void NET_COAP_NBIOT_Event_StopMode(NBIOT_ClientsTypeDef* pClient)
{
	Stm32_CalculagraphTypeDef dictateRunTime;
	static unsigned char CoapSendMessageIndex;
	
	/* It is the first time to execute */
	if (pClient->DictateRunCtl.dictateEnable != true) {
		pClient->DictateRunCtl.dictateEnable = true;
		pClient->DictateRunCtl.dictateTimeoutSec = 1800;
		Stm32_Calculagraph_CountdownSec(&dictateRunTime, pClient->DictateRunCtl.dictateTimeoutSec);
		pClient->DictateRunCtl.dictateRunTime = dictateRunTime;
		/* NBIOT Module Poweroff */
		NBIOT_Neul_NBxx_HardwarePoweroff(pClient);
		/* Clear Stored EARFCN */
		pClient->ClearStoredEARFCN = NBIOT_CLEAR_STORED_EARFCN_TRUE;
		/* Send Message Index */
		CoapSendMessageIndex = NET_Coap_Message_SendDataRear();
		
		/* Get ConnectTime & IdleTime */
		COAP_NBIOT_GetConnectTime(pClient, false);
		COAP_NBIOT_GetIdleTime(pClient, false);
	}
	
	if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
		/* Dictate TimeOut */
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
	}
	else {
		/* Dictate isn't TimeOut */
		if (NET_Coap_Message_SendDataRear() != CoapSendMessageIndex) {
			/* Have new coap message need to Send */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		}
		else {
			pClient->DictateRunCtl.dictateEvent = STOP_MODE;
		}
	}
}

/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_HardwareReboot(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_HardwareReboot	: 硬件重启
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_NBIOT_Event_HardwareReboot(NBIOT_ClientsTypeDef* pClient)
{
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (NBIOT_Neul_NBxx_HardwareReboot(pClient, 8000) == NBIOT_OK) {
		/* Dictate execute is Success */
#if NBIOT_PRINT_ERROR_CODE_TYPE
		COAP_NBIOT_DictateEvent_SuccessExecute(pClient, REPORT_ERROE, HARDWARE_REBOOT);
#else
		COAP_NBIOT_DictateEvent_SuccessExecute(pClient, MODULE_CHECK, HARDWARE_REBOOT);
#endif
		/* Get IdleTime */
		COAP_NBIOT_GetIdleTime(pClient, true);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
		COAP_DEBUG_LOG_PRINTF("NB HDRBT Ok, Baud:%d", NBIOTBaudRate.Baud);
#endif
	}
	else {
		/* Dictate execute is Fail */
		COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, HARDWARE_REBOOT);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
		COAP_DEBUG_LOG_PRINTF("NB HDRBT Fail");
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_ReportError(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_ReportError		: 错误码输出
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_NBIOT_Event_ReportError(NBIOT_ClientsTypeDef* pClient)
{
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (NBIOT_Neul_NBxx_SetReportTerminationError(pClient, CMEEnable) == NBIOT_OK) {
		/* Dictate execute is Success */
		COAP_NBIOT_DictateEvent_SuccessExecute(pClient, MODULE_CHECK, REPORT_ERROE);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
		COAP_DEBUG_LOG_PRINTF("NB ReportECde Set %d Ok", CMEEnable);
#endif
	}
	else {
		/* Dictate execute is Fail */
		COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, REPORT_ERROE);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
		COAP_DEBUG_LOG_PRINTF("NB ReportECde Set %d Fail", CMEEnable);
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_ModuleCheck(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_ModuleCheck		: 模块检测
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_NBIOT_Event_ModuleCheck(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (((NBStatus = NBIOT_Neul_NBxx_CheckReadManufacturer(pClient)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadManufacturerModel(pClient)) == NBIOT_OK) &&
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadModuleVersion(pClient)) == NBIOT_OK)) {
		/* Dictate execute is Success */
		COAP_NBIOT_DictateEvent_SuccessExecute(pClient, PARAMETER_CONFIG, MODULE_CHECK);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
		COAP_DEBUG_LOG_PRINTF("NB Module Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, MODULE_CHECK);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		COAP_DEBUG_LOG_PRINTF("NB Module Fail ECde %d", NBStatus);
	#else
		COAP_DEBUG_LOG_PRINTF("NB Module Fail");
	#endif
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_ParameterConfig(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_ParameterConfig	: 参数配置
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_NBIOT_Event_ParameterConfig(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadConfigUE(pClient)) == NBIOT_OK) {
		/* Dictate execute is Success */
		COAP_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
		COAP_DEBUG_LOG_PRINTF("NB Parameter Config Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		COAP_DEBUG_LOG_PRINTF("NB Parameter Config Fail ECde %d", NBStatus);
	#else
		COAP_DEBUG_LOG_PRINTF("NB Parameter Config Fail");
	#endif
#endif
		return;
	}
	
	if (pClient->Parameter.nconfig.autoConnect != AutoConnectVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient, AutoConnect, AutoConnectVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			COAP_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
			COAP_DEBUG_LOG_PRINTF("NB %s %d Ok", AutoConnect, AutoConnectVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
			COAP_DEBUG_LOG_PRINTF("NB %s %d Fail", AutoConnect, AutoConnectVal);
#endif
			return;
		}
	}
	
	if (pClient->Parameter.nconfig.crScrambling != CrScramblingVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient, CrScrambling, CrScramblingVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			COAP_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
			COAP_DEBUG_LOG_PRINTF("NB %s %d Ok", CrScrambling, CrScramblingVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
			COAP_DEBUG_LOG_PRINTF("NB %s %d Fail", CrScrambling, CrScramblingVal);
#endif
			return;
		}
	}
	
	if (pClient->Parameter.nconfig.crSiAvoid != CrSiAvoidVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient, CrSiAvoid, CrSiAvoidVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			COAP_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
			COAP_DEBUG_LOG_PRINTF("NB %s %d Ok", CrSiAvoid, CrSiAvoidVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
			COAP_DEBUG_LOG_PRINTF("NB %s %d Fail", CrSiAvoid, CrSiAvoidVal);
#endif
			return;
		}
	}
	
	if (pClient->Parameter.nconfig.combineAttach != CombineAttachVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient, CombineAttach, CombineAttachVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			COAP_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
			COAP_DEBUG_LOG_PRINTF("NB %s %d Ok", CombineAttach, CombineAttachVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
			COAP_DEBUG_LOG_PRINTF("NB %s %d Fail", CombineAttach, CombineAttachVal);
#endif
			return;
		}
	}
	
	if (pClient->Parameter.nconfig.cellReselection != CellReselectionVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient, CellReselection, CellReselectionVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			COAP_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
			COAP_DEBUG_LOG_PRINTF("NB %s %d Ok", CellReselection, CellReselectionVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
			COAP_DEBUG_LOG_PRINTF("NB %s %d Fail", CellReselection, CellReselectionVal);
#endif
			return;
		}
	}
	
	if (pClient->Parameter.nconfig.enableBip != EnableBipVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient, EnableBip, EnableBipVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			COAP_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
			COAP_DEBUG_LOG_PRINTF("NB %s %d Ok", EnableBip, EnableBipVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
			COAP_DEBUG_LOG_PRINTF("NB %s %d Fail", EnableBip, EnableBipVal);
#endif
			return;
		}
	}
}

/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_SimICCIDCheck(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_SimICCIDCheck		: Sim卡检测
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_NBIOT_Event_SimICCIDCheck(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadICCID(pClient)) == NBIOT_OK) {
		/* Dictate execute is Success */
		COAP_NBIOT_DictateEvent_SuccessExecute(pClient, FULL_FUNCTIONALITY, ICCID_CHECK);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
		COAP_DEBUG_LOG_PRINTF("NB ICCID Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ICCID_CHECK);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		COAP_DEBUG_LOG_PRINTF("NB ICCID Fail ECde %d", NBStatus);
	#else
		COAP_DEBUG_LOG_PRINTF("NB ICCID Fail");
	#endif
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_FullFunctionality(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_FullFunctionality	: 完整功能
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_NBIOT_Event_FullFunctionality(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadMinOrFullFunc(pClient)) == NBIOT_OK) {
		/* Dictate execute is Success */
#if NBIOT_CLEAR_STORED_EARFCN_STAT
		if (pClient->ClearStoredEARFCN != NBIOT_CLEAR_STORED_EARFCN_FALSE) {
			/* 需清除频点 */
			COAP_NBIOT_DictateEvent_SuccessExecute(pClient, MINIMUM_FUNCTIONALITY, FULL_FUNCTIONALITY);
		}
		else {
			/* 无需清除频点 */
			COAP_NBIOT_DictateEvent_SuccessExecute(pClient, CDP_SERVER_CHECK, FULL_FUNCTIONALITY);
		}
#else
		COAP_NBIOT_DictateEvent_SuccessExecute(pClient, CDP_SERVER_CHECK, FULL_FUNCTIONALITY);
#endif
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
		COAP_DEBUG_LOG_PRINTF("CoAP FullFunc Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, FULL_FUNCTIONALITY);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		COAP_DEBUG_LOG_PRINTF("CoAP FullFunc Fail ECde %d", NBStatus);
	#else
		COAP_DEBUG_LOG_PRINTF("CoAP FullFunc Fail");
	#endif
#endif
		return;
	}
	
	if (pClient->Parameter.functionality != FullFunc) {
		if ((NBStatus = NBIOT_Neul_NBxx_SetMinOrFullFunc(pClient, FullFunc)) == NBIOT_OK) {
			/* Dictate execute is Success */
#if NBIOT_CLEAR_STORED_EARFCN_STAT
			if (pClient->ClearStoredEARFCN != NBIOT_CLEAR_STORED_EARFCN_FALSE) {
				/* 需清除频点 */
				COAP_NBIOT_DictateEvent_SuccessExecute(pClient, MINIMUM_FUNCTIONALITY, FULL_FUNCTIONALITY);
			}
			else {
				/* 无需清除频点 */
				COAP_NBIOT_DictateEvent_SuccessExecute(pClient, CDP_SERVER_CHECK, FULL_FUNCTIONALITY);
			}
#else
			COAP_NBIOT_DictateEvent_SuccessExecute(pClient, CDP_SERVER_CHECK, FULL_FUNCTIONALITY);
#endif
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
			COAP_DEBUG_LOG_PRINTF("CoAP FullFunc Set Ok");
#endif
		}
		else {
			/* Dictate execute is Fail */
			COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, FULL_FUNCTIONALITY);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			COAP_DEBUG_LOG_PRINTF("CoAP FullFunc Set Fail ECde %d", NBStatus);
		#else
			COAP_DEBUG_LOG_PRINTF("CoAP FullFunc Set Fail");
		#endif
#endif
			return;
		}
	}
}

/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_MinimumFunctionality(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_MinimumFunctionality	: 最小功能
 @Input				pClient								: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_NBIOT_Event_MinimumFunctionality(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadMinOrFullFunc(pClient)) == NBIOT_OK) {
		/* Dictate execute is Success */
#if NBIOT_CLEAR_STORED_EARFCN_STAT
		if (pClient->ClearStoredEARFCN != NBIOT_CLEAR_STORED_EARFCN_FALSE) {
			/* 需清除频点 */
			COAP_NBIOT_DictateEvent_SuccessExecute(pClient, CLEAR_STORED_EARFCN, MINIMUM_FUNCTIONALITY);
		}
		else {
			/* 无需清除频点 */
			COAP_NBIOT_DictateEvent_SuccessExecute(pClient, CDP_SERVER_CONFIG, MINIMUM_FUNCTIONALITY);
		}
#else
		COAP_NBIOT_DictateEvent_SuccessExecute(pClient, CDP_SERVER_CONFIG, MINIMUM_FUNCTIONALITY);
#endif
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
		COAP_DEBUG_LOG_PRINTF("CoAP MinFunc Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, MINIMUM_FUNCTIONALITY);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		COAP_DEBUG_LOG_PRINTF("CoAP MinFunc Fail ECde %d", NBStatus);
	#else
		COAP_DEBUG_LOG_PRINTF("CoAP MinFunc Fail");
	#endif
#endif
		return;
	}
	
	if (pClient->Parameter.functionality != MinFunc) {
		if ((NBStatus = NBIOT_Neul_NBxx_SetMinOrFullFunc(pClient, MinFunc)) == NBIOT_OK) {
			/* Dictate execute is Success */
#if NBIOT_CLEAR_STORED_EARFCN_STAT
			if (pClient->ClearStoredEARFCN != NBIOT_CLEAR_STORED_EARFCN_FALSE) {
				/* 需清除频点 */
				COAP_NBIOT_DictateEvent_SuccessExecute(pClient, CLEAR_STORED_EARFCN, MINIMUM_FUNCTIONALITY);
			}
			else {
				/* 无需清除频点 */
				COAP_NBIOT_DictateEvent_SuccessExecute(pClient, CDP_SERVER_CONFIG, MINIMUM_FUNCTIONALITY);
			}
#else
			COAP_NBIOT_DictateEvent_SuccessExecute(pClient, CDP_SERVER_CONFIG, MINIMUM_FUNCTIONALITY);
#endif
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
			COAP_DEBUG_LOG_PRINTF("CoAP MinFunc Set Ok");
#endif
		}
		else {
			/* Dictate execute is Fail */
			COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, MINIMUM_FUNCTIONALITY);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			COAP_DEBUG_LOG_PRINTF("CoAP MinFunc Set Fail ECde %d", NBStatus);
		#else
			COAP_DEBUG_LOG_PRINTF("CoAP MinFunc Set Fail");
		#endif
#endif
			return;
		}
	}
}

/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_ClearStoredEARFCN(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_ClearStoredEARFCN		: 清除小区频点
 @Input				pClient								: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_NBIOT_Event_ClearStoredEARFCN(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_ClearStoredEarfcn(pClient)) == NBIOT_OK) {
		/* Dictate execute is Success */
		pClient->ClearStoredEARFCN = NBIOT_CLEAR_STORED_EARFCN_FALSE;
		
		COAP_NBIOT_DictateEvent_SuccessExecute(pClient, FULL_FUNCTIONALITY, CLEAR_STORED_EARFCN);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
		COAP_DEBUG_LOG_PRINTF("NB Clear Stored EARFCN OK");
#endif
	}
	else {
		/* Dictate execute is Fail */
		COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, CLEAR_STORED_EARFCN);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		DNS_DEBUG_LOG_PRINTF("NB Clear EARFCN Fail Ecde %d", NBStatus);
	#else
		DNS_DEBUG_LOG_PRINTF("NB Clear EARFCN Fail");
	#endif
#endif
		return;
	}
}

/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_CDPServerCheck(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_CDPServerCheck	: CDP服务器查询
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_NBIOT_Event_CDPServerCheck(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadCDPServer(pClient)) == NBIOT_OK) {
		/* Dictate execute is Success */
		COAP_NBIOT_DictateEvent_SuccessExecute(pClient, CDP_SERVER_CHECK, CDP_SERVER_CHECK);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
		COAP_DEBUG_LOG_PRINTF("CoAP CDP %s:%d Ok", pClient->Parameter.cdpserver.CDPServerHost, pClient->Parameter.cdpserver.CDPServerPort);
#endif
	}
	else {
		/* Dictate execute is Fail */
		COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, CDP_SERVER_CHECK);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		COAP_DEBUG_LOG_PRINTF("CoAP CDP Fail ECde %d", NBStatus);
	#else
		COAP_DEBUG_LOG_PRINTF("CoAP CDP Fail");
	#endif
#endif
	}
	
	if ((strcmp((const char*)pClient->Parameter.cdpserver.CDPServerHost, TCFG_EEPROM_Get_ServerIP_String()) != 0) || (pClient->Parameter.cdpserver.CDPServerPort != TCFG_EEPROM_GetServerPort())) {
		/* CDP Server Mast be Config */
		pClient->DictateRunCtl.dictateEvent = MINIMUM_FUNCTIONALITY;
	}
	else {
		/* CDP Server Needn't be Config */
		pClient->DictateRunCtl.dictateEvent = NBAND_MODE_CHECK;
	}
}

/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_CDPServerConfig(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_CDPServerConfig	: CDP服务器配置
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_NBIOT_Event_CDPServerConfig(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadCDPServer(pClient)) == NBIOT_OK) {
		/* Dictate execute is Success */
		COAP_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CONFIG, CDP_SERVER_CONFIG);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
		COAP_DEBUG_LOG_PRINTF("CoAP CDP %s:%d Ok", pClient->Parameter.cdpserver.CDPServerHost, pClient->Parameter.cdpserver.CDPServerPort);
#endif
	}
	else {
		/* Dictate execute is Fail */
		COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, CDP_SERVER_CONFIG);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		COAP_DEBUG_LOG_PRINTF("CoAP CDP Fail ECde %d", NBStatus);
	#else
		COAP_DEBUG_LOG_PRINTF("CoAP CDP Fail");
	#endif
#endif
	}
	
	if ((strcmp((const char*)pClient->Parameter.cdpserver.CDPServerHost, TCFG_EEPROM_Get_ServerIP_String()) != 0) || (pClient->Parameter.cdpserver.CDPServerPort != TCFG_EEPROM_GetServerPort())) {
		/* CDP Server Mast be Config */
		if ((NBStatus = NBIOT_Neul_NBxx_SetCDPServer(pClient, TCFG_EEPROM_Get_ServerIP_String(), TCFG_EEPROM_GetServerPort())) == NBIOT_OK) {
			/* Dictate execute is Success */
			COAP_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CONFIG, CDP_SERVER_CONFIG);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
			COAP_DEBUG_LOG_PRINTF("CoAP CDP Set %s:%d Ok", TCFG_EEPROM_Get_ServerIP_String(), TCFG_EEPROM_GetServerPort());
#endif
		}
		else {
			/* Dictate execute is Fail */
			COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, CDP_SERVER_CONFIG);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			COAP_DEBUG_LOG_PRINTF("CoAP CDP Set %s:%d Fail ECde %d", TCFG_EEPROM_Get_ServerIP_String(), TCFG_EEPROM_GetServerPort(), NBStatus);
		#else
			COAP_DEBUG_LOG_PRINTF("CoAP CDP Set %s:%d Fail", TCFG_EEPROM_Get_ServerIP_String(), TCFG_EEPROM_GetServerPort());
		#endif
#endif
			return;
		}
	}
	else {
		/* CDP Server Needn't be Config */
		pClient->DictateRunCtl.dictateEvent = NBAND_MODE_CONFIG;
	}
}

static bool NET_COAP_CheckNband(NBIOT_ClientsTypeDef* pClient, NBIOT_NBandTypeDef nbandsrc, NBIOT_NBandTypeDef nbanddst)
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
 @Function			void NET_COAP_NBIOT_Event_NbandModeCheck(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_NbandModeCheck	: NBAND模式查询
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_NBIOT_Event_NbandModeCheck(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadSupportedBands(pClient)) == NBIOT_OK) {
		/* Dictate execute is Success */
		COAP_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CHECK, NBAND_MODE_CHECK);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
		COAP_DEBUG_LOG_PRINTF("CoAP BAND %d:%d.%d.%d Ok", pClient->Parameter.band.NBandNum, \
												pClient->Parameter.band.NBandVal[0], \
												pClient->Parameter.band.NBandVal[1], \
												pClient->Parameter.band.NBandVal[2]);
#endif
	}
	else {
		/* Dictate execute is Fail */
		COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, NBAND_MODE_CHECK);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		COAP_DEBUG_LOG_PRINTF("CoAP BAND Fail ECde %d", NBStatus);
	#else
		COAP_DEBUG_LOG_PRINTF("CoAP BAND Fail");
	#endif
#endif
	}
	
	if (NET_COAP_CheckNband(pClient, pClient->Parameter.band, COAP_NBIOT_BAND_TYPE) != true) {
		/* BAND Mode Mast be Config */
		pClient->DictateRunCtl.dictateEvent = MINIMUM_FUNCTIONALITY;
	}
	else {
		/* BAND Mode Needn't be Config */
		pClient->DictateRunCtl.dictateEvent = MISC_EQUIP_CONFIG;
	}
}

/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_NbandModeConfig(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_NbandModeConfig	: NBAND模式配置
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_NBIOT_Event_NbandModeConfig(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadSupportedBands(pClient)) == NBIOT_OK) {
		/* Dictate execute is Success */
		COAP_NBIOT_DictateEvent_SuccessExecute(pClient, FULL_FUNCTIONALITY, NBAND_MODE_CONFIG);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
		COAP_DEBUG_LOG_PRINTF("CoAP BAND Read Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, NBAND_MODE_CONFIG);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		COAP_DEBUG_LOG_PRINTF("CoAP BAND Fail ECde %d", NBStatus);
	#else
		COAP_DEBUG_LOG_PRINTF("CoAP BAND Fail");
	#endif
#endif
	}
	
	if (NET_COAP_CheckNband(pClient, pClient->Parameter.band, COAP_NBIOT_BAND_TYPE) != true) {
		/* BAND Mode Mast be Config */
		if ((NBStatus = NBIOT_Neul_NBxx_SetSupportedBands(pClient, COAP_NBIOT_BAND_TYPE)) == NBIOT_OK) {
			/* Dictate execute is Success */
			COAP_NBIOT_DictateEvent_SuccessExecute(pClient, FULL_FUNCTIONALITY, NBAND_MODE_CONFIG);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
			COAP_DEBUG_LOG_PRINTF("CoAP BAND Set %d:%d.%d.%d Ok", COAP_NBIOT_BAND_TYPE.NBandNum, \
													    COAP_NBIOT_BAND_TYPE.NBandVal[0], \
													    COAP_NBIOT_BAND_TYPE.NBandVal[1], \
													    COAP_NBIOT_BAND_TYPE.NBandVal[2]);
#endif
		}
		else {
			/* Dictate execute is Fail */
			COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, NBAND_MODE_CONFIG);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			COAP_DEBUG_LOG_PRINTF("CoAP BAND Set Fail ECde %d", NBStatus);
		#else
			COAP_DEBUG_LOG_PRINTF("CoAP BAND Set Fail");
		#endif
#endif
			return;
		}
	}
	else {
		/* BAND Mode Needn't be Config */
		pClient->DictateRunCtl.dictateEvent = FULL_FUNCTIONALITY;
	}
}

/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_MiscEquipConfig(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_MiscEquipConfig	: 其他配置
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_NBIOT_Event_MiscEquipConfig(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (((NBStatus = NBIOT_Neul_NBxx_CheckReadNewMessageIndications(pClient)) == NBIOT_OK) &&
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadSentMessageIndications(pClient)) == NBIOT_OK)) {
		/* Dictate execute is Success */
		COAP_NBIOT_DictateEvent_SuccessExecute(pClient, ATTACH_CHECK, MISC_EQUIP_CONFIG);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
		COAP_DEBUG_LOG_PRINTF("CoAP Misc Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, MISC_EQUIP_CONFIG);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		COAP_DEBUG_LOG_PRINTF("CoAP Misc Fail ECde %d", NBStatus);
	#else
		COAP_DEBUG_LOG_PRINTF("CoAP Misc Fail");
	#endif
#endif
		return;
	}
	
	if (pClient->Parameter.nnmistate != CloseFunc) {
		if ((NBStatus = NBIOT_Neul_NBxx_SetNewMessageIndications(pClient, CloseFunc)) == NBIOT_OK) {
			/* Dictate execute is Success */
			COAP_NBIOT_DictateEvent_SuccessExecute(pClient, ATTACH_CHECK, MISC_EQUIP_CONFIG);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
			COAP_DEBUG_LOG_PRINTF("CoAP NNMI %d Ok", CloseFunc);
#endif
		}
		else {
			/* Dictate execute is Fail */
			COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, MISC_EQUIP_CONFIG);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			COAP_DEBUG_LOG_PRINTF("CoAP NNMI %d Fail ECde %d", CloseFunc, NBStatus);
		#else
			COAP_DEBUG_LOG_PRINTF("CoAP NNMI %d Fail", CloseFunc);
		#endif
#endif
			return;
		}
	}
	
	if (pClient->Parameter.nsmistate != CloseFunc) {
		if ((NBStatus = NBIOT_Neul_NBxx_SetSentMessageIndications(pClient, CloseFunc)) == NBIOT_OK) {
			/* Dictate execute is Success */
			COAP_NBIOT_DictateEvent_SuccessExecute(pClient, ATTACH_CHECK, MISC_EQUIP_CONFIG);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
			COAP_DEBUG_LOG_PRINTF("CoAP NSMI %d Ok", CloseFunc);
#endif
		}
		else {
			/* Dictate execute is Fail */
			COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, MISC_EQUIP_CONFIG);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			COAP_DEBUG_LOG_PRINTF("CoAP NSMI %d Fail ECde %d", CloseFunc, NBStatus);
		#else
			COAP_DEBUG_LOG_PRINTF("CoAP NSMI %d Fail", CloseFunc);
		#endif
#endif
			return;
		}
	}
}

/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_AttachCheck(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_AttachCheck		: 注网检查
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_NBIOT_Event_AttachCheck(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadAttachOrDetach(pClient)) == NBIOT_OK) {
		/* Dictate execute is Success */
		COAP_NBIOT_DictateEvent_SuccessExecute(pClient, ATTACH_CHECK, ATTACH_CHECK);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
		COAP_DEBUG_LOG_PRINTF("CoAP CGATT %d Ok", pClient->Parameter.netstate);
#endif
	}
	else {
		/* Dictate execute is Fail */
		COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_CHECK);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		COAP_DEBUG_LOG_PRINTF("CoAP CGATT %d Fail ECde %d", pClient->Parameter.netstate, NBStatus);
	#else
		COAP_DEBUG_LOG_PRINTF("CoAP CGATT %d Fail", pClient->Parameter.netstate);
	#endif
#endif
		return;
	}
	
	if (pClient->Parameter.netstate != Attach) {
		pClient->DictateRunCtl.dictateEvent = ATTACH_EXECUTE;
	}
	else {
		pClient->DictateRunCtl.dictateEvent = ATTACH_INQUIRE;
	}
}

/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_AttachExecute(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_AttachExecute		: 注网进行
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_NBIOT_Event_AttachExecute(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_SetAttachOrDetach(pClient, Attach)) == NBIOT_OK) {
		/* Dictate execute is Success */
		COAP_NBIOT_DictateEvent_SuccessExecute(pClient, ATTACH_INQUIRE, ATTACH_EXECUTE);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
		COAP_DEBUG_LOG_PRINTF("CoAP Set CGATT %d Ok", Attach);
#endif
	}
	else {
		/* Dictate execute is Fail */
		COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_EXECUTE);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		COAP_DEBUG_LOG_PRINTF("CoAP Set CGATT %d Fail ECde %d", Attach, NBStatus);
	#else
		COAP_DEBUG_LOG_PRINTF("CoAP Set CGATT %d Fail", Attach);
	#endif
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_AttachInquire(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_AttachInquire		: 注网查询
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_NBIOT_Event_AttachInquire(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	COAP_NBIOT_DictateEvent_SetTime(pClient, COAP_NBIOT_CGATT_TIME_S);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadAttachOrDetach(pClient)) == NBIOT_OK) {
		/* Dictate execute is Success */
		pClient->DictateRunCtl.dictateEvent = ATTACH_INQUIRE;
#ifdef COAP_DEBUG_LOG_RF_PRINT
		COAP_DEBUG_LOG_PRINTF("CoAP CGATT %d Ok", pClient->Parameter.netstate);
#endif
	}
	else {
		/* Dictate execute is Fail */
		COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_INQUIRE);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		COAP_DEBUG_LOG_PRINTF("CoAP CGATT %d Fail ECde %d", pClient->Parameter.netstate, NBStatus);
	#else
		COAP_DEBUG_LOG_PRINTF("CoAP CGATT %d Fail", pClient->Parameter.netstate);
	#endif
#endif
		return;
	}
	
	if (pClient->Parameter.netstate != Attach) {
		/* 未注网 */
		COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_INQUIRE);
		
		/* 开启频点清除 */
		pClient->ClearStoredEARFCN = NBIOT_CLEAR_STORED_EARFCN_TRUE;
	}
	else {
		/* 注网成功 */
		COAP_NBIOT_DictateEvent_SuccessExecute(pClient, PARAMETER_CHECKOUT, ATTACH_INQUIRE);
		
		/* 关闭频点清除 */
		pClient->ClearStoredEARFCN = NBIOT_CLEAR_STORED_EARFCN_FALSE;
		
		/* Get ConnectTime */
		COAP_NBIOT_GetConnectTime(pClient, true);
	}
}

/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_PatameterCheckOut(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_PatameterCheckOut	: 参数检出
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_NBIOT_Event_ParameterCheckOut(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
#if NBIOT_STATION_TIMESTAMP_TYPE
	unsigned int devicetimestamp = 0;
	unsigned int stationtimestamp = 0;
#endif
	
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (((NBStatus = NBIOT_Neul_NBxx_CheckReadIMEI(pClient)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadIMSI(pClient)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadCGPADDR(pClient)) == NBIOT_OK) && 
	#if NBIOT_MODULE_MODE_TYPE == NBIOT_MODULE_MODE_V120
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadCGDCONT(pClient)) == NBIOT_OK) && 
	#endif
	#if NBIOT_MODULE_MODE_TYPE == NBIOT_MODULE_MODE_V150
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadPDPContext(pClient)) == NBIOT_OK) && 
	#endif
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadRSSI(pClient)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadStatisticsRADIO(pClient)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadAreaCode(pClient)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadDateTime(pClient)) == NBIOT_OK)) {
		/* Dictate execute is Success */
		COAP_NBIOT_DictateEvent_SuccessExecute(pClient, NBCOAP_SENDMODE_TYPE, PARAMETER_CHECKOUT);
		
		pClient->Registered = true;
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
		COAP_DEBUG_LOG_PRINTF("CoAP Para Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CHECKOUT);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		COAP_DEBUG_LOG_PRINTF("CoAP Para Check Fail ECde %d", NBStatus);
	#else
		COAP_DEBUG_LOG_PRINTF("CoAP Para Check Fail");
	#endif
#endif
		return;
	}
	
#if NBIOT_STATION_TIMESTAMP_TYPE
	devicetimestamp = RTC_GetUnixTimeToStamp() + (8 * 60 * 60);
	stationtimestamp = RTC_TimeToStamp(pClient->Parameter.dataTime.year, pClient->Parameter.dataTime.month, pClient->Parameter.dataTime.day, pClient->Parameter.dataTime.hour, pClient->Parameter.dataTime.min,   pClient->Parameter.dataTime.sec);
	
	if ((((stationtimestamp >= devicetimestamp) ? (stationtimestamp - devicetimestamp) : (devicetimestamp - stationtimestamp)) > (NBIOT_STATION_TIMESTAMP_DIFFTIME)) && (pClient->Parameter.dataTime.errcount < NBIOT_STATION_TIMESTAMP_ERRCOUNT) && (BackUp != true)) {
		pClient->Parameter.dataTime.errcount++;
		COAP_NBIOT_DictateEvent_SuccessExecute(pClient, HARDWARE_REBOOT, PARAMETER_CHECKOUT);
		return;
	}
	
	pClient->Parameter.dataTime.errcount = 0;
#endif
	
	/* Set System Time */
	RTC_Set_Date(pClient->Parameter.dataTime.year, pClient->Parameter.dataTime.month, pClient->Parameter.dataTime.day);
	RTC_Set_Time(pClient->Parameter.dataTime.hour, pClient->Parameter.dataTime.min,   pClient->Parameter.dataTime.sec);
}

/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_SendData(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_SendData			: 发送数据
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_NBIOT_Event_SendData(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
#if NBCOAP_SENDDATA_NQMGSCHECK_TYPE
	int SendSentNum = 0;
#endif
	
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	/* Data packets need to be sent*/
	if (NET_Coap_Message_SendDataDequeue(pClient->Sendbuf, (unsigned short *)&pClient->Sendlen) == true) {
		/* Get IdleTime */
		COAP_NBIOT_GetIdleTime(pClient, true);
		
#if NBCOAP_SEND_BEFORE_ATTACH_TYPE
		/* Connect Check */
		if ((NBStatus = NBIOT_Neul_NBxx_CheckReadAttachOrDetach(pClient)) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEvent = SEND_DATA;
#ifdef COAP_DEBUG_LOG_RF_PRINT
			COAP_DEBUG_LOG_PRINTF("CoAP CGATT %d Ok", pClient->Parameter.netstate);
#endif
		}
		else {
			/* Dictate execute is Fail */
			COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, SEND_DATA);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			COAP_DEBUG_LOG_PRINTF("CoAP CGATT %d Fail ECde %d", pClient->Parameter.netstate, NBStatus);
		#else
			COAP_DEBUG_LOG_PRINTF("CoAP CGATT %d Fail", pClient->Parameter.netstate);
		#endif
#endif
			return;
		}
		
		if (pClient->Parameter.netstate != Attach) {
			COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, SEND_DATA);
			return;
		}
#endif
		
#if NBCOAP_SENDDATA_NQMGSCHECK_TYPE
		if (((NBStatus = NBIOT_Neul_NBxx_QuerySendMessageCOAPPayload(pClient)) == NBIOT_OK) && 
		    ((NBStatus = NBIOT_Neul_NBxx_QueryReadMessageCOAPPayload(pClient)) == NBIOT_OK)) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEvent = SEND_DATA;
#ifdef COAP_DEBUG_LOG_RF_PRINT
			COAP_DEBUG_LOG_PRINTF("CoAP NQMGS NQMGR Ok");
#endif
		}
		else {
			/* Dictate execute is Fail */
			COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, SEND_DATA);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			COAP_DEBUG_LOG_PRINTF("CoAP NQMGS NQMGR Fail ECde %d", NBStatus);
		#else
			COAP_DEBUG_LOG_PRINTF("CoAP NQMGS NQMGR Fail");
		#endif
#endif
			return;
		}
		
		SendSentNum = pClient->Parameter.coapSendMessage.sent;
#endif
		
		/* 发送负载数据 */
		if ((NBStatus = NBIOT_Neul_NBxx_SendCOAPPayload(pClient)) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEvent = SEND_DATA;
#ifdef COAP_DEBUG_LOG_RF_PRINT
			COAP_DEBUG_LOG_PRINTF("CoAP Send Payload Ok");
#endif
		}
		else {
			/* Dictate execute is Fail */
			COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, SEND_DATA);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			COAP_DEBUG_LOG_PRINTF("CoAP Send Payload Fail ECde %d", NBStatus);
		#else
			COAP_DEBUG_LOG_PRINTF("CoAP Send Payload Fail");
		#endif
#endif
			return;
		}
		
#if NBCOAP_SENDDATA_NQMGSCHECK_TYPE
		if (NBIOT_Neul_NBxx_QuerySendMessageCOAPPayload(pClient) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEvent = SEND_DATA;
		}
		else {
			/* Dictate execute is Fail */
			COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, SEND_DATA);
			return;
		}
		
		if ((SendSentNum + 1) != pClient->Parameter.coapSendMessage.sent) {
			/* Send Data Fail */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->DictateRunCtl.dictateSendDataFailureCnt++;
			if (pClient->DictateRunCtl.dictateSendDataFailureCnt > 3) {
				pClient->DictateRunCtl.dictateSendDataFailureCnt = 0;
				pClient->DictateRunCtl.dictateEvent = STOP_MODE;
			}
		}
		else {
			/* Send Data Success */
			COAP_NBIOT_DictateEvent_SuccessExecute(pClient, RECV_DATA, SEND_DATA);
		}
#else
	#if NBCOAP_EXCEED_MSGLEN_TYPE
		if (pClient->Sendlen > NBCOAP_EXCEED_MSGLEN_MAX) {
			NET_Coap_Message_SendDataOffSet();
			COAP_NBIOT_DictateEvent_SuccessExecute(pClient, SEND_DATA, SEND_DATA);
			return;
		}
	#endif
		
		/* Send Data Success */
		COAP_NBIOT_DictateEvent_SuccessExecute(pClient, RECV_DATA, SEND_DATA);
#endif
	}
	/* No packets need to be sent */
	else {
		COAP_NBIOT_DictateEvent_SuccessExecute(pClient, EXECUT_DOWNLINK_DATA, SEND_DATA);
	}
}

/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_RecvData(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_RecvData			: 接收数据
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_NBIOT_Event_RecvData(NBIOT_ClientsTypeDef* pClient)
{
	u8 COAPFeedBackData[] = {0xAA, 0xBB};									//COAP反馈包数据
	bool COAPFeedBackFlag = false;										//COAP反馈包接收标志位
	
	COAP_NBIOT_DictateEvent_SetTime(pClient, 60);
	
	if (NBIOT_Neul_NBxx_QueryReadMessageCOAPPayload(pClient) == NBIOT_OK) {
		/* Dictate execute is Success */
		pClient->DictateRunCtl.dictateEvent = RECV_DATA;
	}
	else {
		/* Dictate execute is Fail */
		COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, RECV_DATA);
		return;
	}
	
	if (pClient->Parameter.coapReadMessage.buffered != 0) {
		/* Has Data Need Receive */
		for (int index = 0; index < pClient->Parameter.coapReadMessage.buffered; index++) {
			
			/* 读取负载数据 */
			if (NBIOT_Neul_NBxx_ReadCOAPPayload(pClient) == NBIOT_OK) {
				/* Dictate execute is Success */
				pClient->DictateRunCtl.dictateEvent = pClient->DictateRunCtl.dictateEvent;
			}
			else {
				/* Dictate execute is Fail */
				if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
					/* Dictate TimeOut */
					pClient->DictateRunCtl.dictateEnable = false;
					pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
					pClient->DictateRunCtl.dictateRecvDataFailureCnt++;
					if (pClient->DictateRunCtl.dictateRecvDataFailureCnt > 3) {
						pClient->DictateRunCtl.dictateRecvDataFailureCnt = 0;
						pClient->DictateRunCtl.dictateEvent = STOP_MODE;
					}
				}
				else {
					/* Dictate isn't TimeOut */
					pClient->DictateRunCtl.dictateEvent = pClient->DictateRunCtl.dictateEvent;
				}
				return;
			}
			
			if (memcmp((const char*)pClient->Recvbuf, COAPFeedBackData, sizeof(COAPFeedBackData)) == 0) {
				/* Is Feedback */
				pClient->NetStateIdentification = true;
				if (COAPFeedBackFlag == false) {
					COAPFeedBackFlag = true;
					NET_Coap_Message_SendDataOffSet();
				}
				COAP_NBIOT_DictateEvent_SuccessExecute(pClient, SEND_DATA, RECV_DATA);
				
			#if NBCOAP_LISTEN_PARAMETER_TYPE == NBCOAP_LISTEN_PARAMETER_ENABLE
				NET_COAP_NBIOT_Listen_Enable_EnterParameter(pClient);
			#endif
				
				/* NB 继续活跃注入时间 */
				TCFG_Utility_Set_Nbiot_IdleLifetime(NBIOT_CONTINUE_LIFETIME);
				
				/* Get ConnectTime */
				COAP_NBIOT_GetConnectTime(pClient, true);
				
#ifdef COAP_DEBUG_LOG_RF_PRINT
				COAP_DEBUG_LOG_PRINTF("CoAP Recv Feedback Ok");
#endif
			}
			else {
				/* Not Feedback */
				NET_Coap_Message_RecvDataEnqueue(pClient->Recvbuf, pClient->Recvlen);
#ifdef COAP_DEBUG_LOG_RF_PRINT
				COAP_DEBUG_LOG_PRINTF("CoAP Recv Data Ok");
#endif
			}
		}
	}
	else {
		/* Not Data Need Receive */
		if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
			/* Dictate TimeOut */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->DictateRunCtl.dictateRecvDataFailureCnt++;
			if (pClient->DictateRunCtl.dictateRecvDataFailureCnt > 3) {
				pClient->DictateRunCtl.dictateRecvDataFailureCnt = 0;
				pClient->DictateRunCtl.dictateEvent = STOP_MODE;
			}
		}
		else {
			/* Dictate isn't TimeOut */
			pClient->DictateRunCtl.dictateEvent = RECV_DATA;
#ifdef COAP_DEBUG_LOG_RF_PRINT
			COAP_DEBUG_LOG_PRINTF("CoAP Wait Recv Feedback");
#endif
		}
	}
}

/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_SendDataRANormal(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_SendDataRANormal	: 发送数据RANormal
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_NBIOT_Event_SendDataRANormal(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	char* RANormal	= "0x0100";
	char* RAIdle	= "0x0101";
	char* RAState	= RAIdle;
	
#if NBCOAP_SENDDATA_NQMGSCHECK_TYPE
	int SendSentNum = 0;
#endif
	
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	/* Data packets need to be sent*/
	if (NET_Coap_Message_SendDataDequeue(pClient->Sendbuf, (unsigned short *)&pClient->Sendlen) == true) {
		/* Get IdleTime */
		COAP_NBIOT_GetIdleTime(pClient, true);
		
#if NBCOAP_SEND_BEFORE_ATTACH_TYPE
		/* Connect Check */
		if ((NBStatus = NBIOT_Neul_NBxx_CheckReadAttachOrDetach(pClient)) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEvent = SEND_DATA_RA_NORMAL;
#ifdef COAP_DEBUG_LOG_RF_PRINT
			COAP_DEBUG_LOG_PRINTF("CoAP CGATT %d Ok", pClient->Parameter.netstate);
#endif
		}
		else {
			/* Dictate execute is Fail */
			COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, SEND_DATA_RA_NORMAL);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			COAP_DEBUG_LOG_PRINTF("CoAP CGATT %d Fail ECde %d", pClient->Parameter.netstate, NBStatus);
		#else
			COAP_DEBUG_LOG_PRINTF("CoAP CGATT %d Fail", pClient->Parameter.netstate);
		#endif
#endif
			return;
		}
		
		if (pClient->Parameter.netstate != Attach) {
			COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, SEND_DATA_RA_NORMAL);
			return;
		}
#endif
		
#if NBCOAP_SENDDATA_NQMGSCHECK_TYPE
		if (((NBStatus = NBIOT_Neul_NBxx_QuerySendMessageCOAPPayload(pClient)) == NBIOT_OK) && 
		    ((NBStatus = NBIOT_Neul_NBxx_QueryReadMessageCOAPPayload(pClient)) == NBIOT_OK)) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEvent = SEND_DATA_RA_NORMAL;
#ifdef COAP_DEBUG_LOG_RF_PRINT
			COAP_DEBUG_LOG_PRINTF("CoAP NQMGS NQMGR Ok");
#endif
		}
		else {
			/* Dictate execute is Fail */
			COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, SEND_DATA_RA_NORMAL);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			COAP_DEBUG_LOG_PRINTF("CoAP NQMGS NQMGR Fail ECde %d", NBStatus);
		#else
			COAP_DEBUG_LOG_PRINTF("CoAP NQMGS NQMGR Fail");
		#endif
#endif
			return;
		}
		
		SendSentNum = pClient->Parameter.coapSendMessage.sent;
#endif
		
#if NBCOAP_RASENDMODE_TYPE == NBCOAP_RASENDMODE_NORMAL
		RAState = RANormal;
#elif NBCOAP_RASENDMODE_TYPE == NBCOAP_RASENDMODE_IDLE
		if (NBIOT_COAP_RA_NORMAL_GET_STATE(pClient) == true) {
			RAState = RANormal;
		}
		else {
			RAState = RAIdle;
		}
#endif
		
		/* 发送负载数据 */
		if ((NBStatus = NBIOT_Neul_NBxx_SendCOAPPayloadFlag(pClient, RAState)) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEvent = SEND_DATA_RA_NORMAL;
			NBIOT_COAP_RA_NORMAL_SET_STATE(pClient, false);
#ifdef COAP_DEBUG_LOG_RF_PRINT
			COAP_DEBUG_LOG_PRINTF("CoAP Send Payload Ok");
#endif
		}
		else {
			/* Dictate execute is Fail */
			COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, SEND_DATA_RA_NORMAL);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			COAP_DEBUG_LOG_PRINTF("CoAP Send Payload Fail ECde %d", NBStatus);
		#else
			COAP_DEBUG_LOG_PRINTF("CoAP Send Payload Fail");
		#endif
#endif
			return;
		}
		
#if NBCOAP_SENDDATA_NQMGSCHECK_TYPE
		if (NBIOT_Neul_NBxx_QuerySendMessageCOAPPayload(pClient) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEvent = SEND_DATA_RA_NORMAL;
		}
		else {
			/* Dictate execute is Fail */
			COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, SEND_DATA_RA_NORMAL);
			return;
		}
		
		if ((SendSentNum + 1) != pClient->Parameter.coapSendMessage.sent) {
			/* Send Data Fail */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->DictateRunCtl.dictateSendDataRANormalFailureCnt++;
			if (pClient->DictateRunCtl.dictateSendDataRANormalFailureCnt > 3) {
				pClient->DictateRunCtl.dictateSendDataRANormalFailureCnt = 0;
				pClient->DictateRunCtl.dictateEvent = STOP_MODE;
			}
		}
		else {
			/* Send Data Success */
			COAP_NBIOT_DictateEvent_SuccessExecute(pClient, RECV_DATA_RA_NORMAL, SEND_DATA_RA_NORMAL);
		}
#else
	#if NBCOAP_EXCEED_MSGLEN_TYPE
		if (pClient->Sendlen > NBCOAP_EXCEED_MSGLEN_MAX) {
			NET_Coap_Message_SendDataOffSet();
			COAP_NBIOT_DictateEvent_SuccessExecute(pClient, SEND_DATA_RA_NORMAL, SEND_DATA_RA_NORMAL);
			return;
		}
	#endif
		
		/* Send Data Success */
		COAP_NBIOT_DictateEvent_SuccessExecute(pClient, RECV_DATA_RA_NORMAL, SEND_DATA_RA_NORMAL);
#endif
	}
	/* No packets need to be sent */
	else {
		COAP_NBIOT_DictateEvent_SuccessExecute(pClient, EXECUT_DOWNLINK_DATA, SEND_DATA_RA_NORMAL);
	}
}

/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_RecvDataRANormal(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_RecvDataRANormal	: 接收数据RANormal
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_NBIOT_Event_RecvDataRANormal(NBIOT_ClientsTypeDef* pClient)
{
	COAP_NBIOT_DictateEvent_SetTime(pClient, 60);
	
	if (NBIOT_Neul_NBxx_CheckReadCONDataStatus(pClient) == NBIOT_OK) {
		/* Dictate execute is Success */
		pClient->DictateRunCtl.dictateEvent = RECV_DATA_RA_NORMAL;
	}
	else {
		/* Dictate execute is Fail */
		COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, RECV_DATA_RA_NORMAL);
		return;
	}
	
	if (pClient->Parameter.condatastate == SendSussess) {
		/* Send Data To Server Success */
		if (NBIOT_Neul_NBxx_QueryReadMessageCOAPPayload(pClient) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEvent = RECV_DATA_RA_NORMAL;
		}
		else {
			/* Dictate execute is Fail */
			COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, RECV_DATA_RA_NORMAL);
			return;
		}
		
		/* 检查是否有下行数据 */
		if (pClient->Parameter.coapReadMessage.buffered != 0) {
			/* Has Data Need Receive */
			for (int index = 0; index < pClient->Parameter.coapReadMessage.buffered; index++) {
				/* 读取负载数据 */
				if (NBIOT_Neul_NBxx_ReadCOAPPayload(pClient) == NBIOT_OK) {
					/* Dictate execute is Success */
					pClient->DictateRunCtl.dictateEvent = pClient->DictateRunCtl.dictateEvent;
				}
				else {
					/* Dictate execute is Fail */
					if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
						/* Dictate TimeOut */
						pClient->DictateRunCtl.dictateEnable = false;
						pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
						pClient->DictateRunCtl.dictateRecvDataRANormalFailureCnt++;
						if (pClient->DictateRunCtl.dictateRecvDataRANormalFailureCnt > 3) {
							pClient->DictateRunCtl.dictateRecvDataRANormalFailureCnt = 0;
							pClient->DictateRunCtl.dictateEvent = STOP_MODE;
						}
					}
					else {
						/* Dictate isn't TimeOut */
						pClient->DictateRunCtl.dictateEvent = pClient->DictateRunCtl.dictateEvent;
					}
					return;
				}
				
				NET_Coap_Message_RecvDataEnqueue(pClient->Recvbuf, pClient->Recvlen);
#ifdef COAP_DEBUG_LOG_RF_PRINT
				COAP_DEBUG_LOG_PRINTF("CoAP Recv Data Ok");
#endif
			}
		}
		
		pClient->NetStateIdentification = true;
		NET_Coap_Message_SendDataOffSet();
		COAP_NBIOT_DictateEvent_SuccessExecute(pClient, SEND_DATA_RA_NORMAL, RECV_DATA_RA_NORMAL);
		
	#if NBCOAP_LISTEN_PARAMETER_TYPE == NBCOAP_LISTEN_PARAMETER_ENABLE
		NET_COAP_NBIOT_Listen_Enable_EnterParameter(pClient);
	#endif
		
		/* NB 继续活跃注入时间 */
		TCFG_Utility_Set_Nbiot_IdleLifetime(NBIOT_CONTINUE_LIFETIME);
		
		/* Get ConnectTime */
		COAP_NBIOT_GetConnectTime(pClient, true);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
		COAP_DEBUG_LOG_PRINTF("CoAP Send Ok");
#endif
	}
	else {
		/* Not yet Send Data Success */
		if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
			/* Dictate TimeOut */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->DictateRunCtl.dictateRecvDataRANormalFailureCnt++;
			if (pClient->DictateRunCtl.dictateRecvDataRANormalFailureCnt > 3) {
				pClient->DictateRunCtl.dictateRecvDataRANormalFailureCnt = 0;
				pClient->DictateRunCtl.dictateEvent = STOP_MODE;
			}
		}
		else {
			/* Dictate isn't TimeOut */
			pClient->DictateRunCtl.dictateEvent = RECV_DATA_RA_NORMAL;
#ifdef COAP_DEBUG_LOG_RF_PRINT
			COAP_DEBUG_LOG_PRINTF("CoAP Wait Send Ok");
#endif
		}
	}
}

/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_ExecutDownlinkData(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_ExecutDownlinkData	: 下行数据处理
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_NBIOT_Event_ExecutDownlinkData(NBIOT_ClientsTypeDef* pClient)
{
	bool recvPCPUpgradeFlag = false;
	u16 recvBufOffset = 0;
	u16 recvMagicNum = 0;
	u8 ret = NETIP_OK;
	
	if (NET_Coap_Message_RecvDataDequeue(pClient->Recvbuf, (unsigned short*)&pClient->Recvlen) == true) {
		pClient->Recvbuf[pClient->Recvlen] = '\0';
		
		/* 私有普通下行数据 */
		for (int i = 0; i < pClient->Recvlen; i++) {
			if ((pClient->Recvbuf[i] == 'T') && (pClient->Recvbuf[i+1] == 'C') && (pClient->Recvbuf[i+2] == 'L') && (pClient->Recvbuf[i+3] == 'D')) {
				recvBufOffset = i;
				break;
			}
		}
		
		if (recvBufOffset != 0) {
			/* Find "TCLD" */
			if (pClient->Recvbuf[recvBufOffset + TCLOD_MSGVERSION_OFFSET] == TCLOD_MSGVERSION) {
				if (pClient->Recvbuf[recvBufOffset + TCLOD_MSGID_OFFSET] == TCLOD_CONFIG_SET) {
					BEEP_CtrlRepeat_Extend(1, 300, 0);
					TCFG_EEPROM_SetNBCmdCnt(1 + TCFG_EEPROM_GetNBCmdCnt());
					TCFG_Utility_Add_NBIot_RecvCount();
					
					/* 工作模式配置指令 */
				#if NBCOAP_DOWNLOAD_CMD_WORKMODE
					if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "Workmode") != NULL) {
						u16 mode;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, \
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
					}
				#endif
					/* 传感器灵敏度配置指令 */
				#if NBCOAP_DOWNLOAD_CMD_SENSE
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "Sense") != NULL) {
						u16 sense;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, \
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
					}
				#endif
					/* 无线心跳间隔时间配置指令 */
				#if NBCOAP_DOWNLOAD_CMD_RFHEART
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "RFHeart") != NULL) {
						u16 rfheart;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, \
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
					}
				#endif
					/* 初始化传感器指令 */
				#if NBCOAP_DOWNLOAD_CMD_BACKGROUND
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "Background") != NULL) {
						u16 backgroundval;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, \
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
					}
				#endif
					/* Reboot */
				#if NBCOAP_DOWNLOAD_CMD_REBOOT
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "Reboot") != NULL) {
						BEEP_CtrlRepeat_Extend(2, 500, 250);
						Stm32_System_Software_Reboot(RBTMODE_COAP_COMMAND);
					}
				#endif
					/* NewSn */
				#if NBCOAP_DOWNLOAD_CMD_NEWSN
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "Newsn") != NULL) {
						u32 newsnval;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, \
							"{(Newsn):{(val):%08x,(Magic):%hu}}", &newsnval, &recvMagicNum);
						if (recvMagicNum == TCLOD_MAGIC_NUM) {
							TCFG_EEPROM_Set_MAC_SN(newsnval);
						}
						else {
							ret = NETIP_UNKNOWNERROR;
						}
					}
				#endif
					/* CDP Server */
				#if NBCOAP_DOWNLOAD_CMD_CDPIP
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "Server") != NULL) {
						u16 cdpip[4];
						u16 cdpport;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, \
							"{(Server):{(IP):(%hu.%hu.%hu.%hu),(Port):%hu,(Magic):%hu}}", &cdpip[3], &cdpip[2], &cdpip[1], &cdpip[0], &cdpport, &recvMagicNum);
						if (recvMagicNum == TCLOD_MAGIC_NUM) {
							TCFG_SystemData.NBCoapCDPServer.ip.ip8[3] = cdpip[3];
							TCFG_SystemData.NBCoapCDPServer.ip.ip8[2] = cdpip[2];
							TCFG_SystemData.NBCoapCDPServer.ip.ip8[1] = cdpip[1];
							TCFG_SystemData.NBCoapCDPServer.ip.ip8[0] = cdpip[0];
							TCFG_SystemData.NBCoapCDPServer.port = cdpport;
							TCFG_EEPROM_SetServerIP(TCFG_SystemData.NBCoapCDPServer.ip.ip32);
							TCFG_EEPROM_SetServerPort(TCFG_SystemData.NBCoapCDPServer.port);
							#if NBCOAP_SENDCODE_DYNAMIC_INFO
							NETCoapNeedSendCode.DynamicInfo = 1;
							#endif
							NET_Coap_Message_RecvDataOffSet();
							NETCoapNeedSendCode.ResponseInfoErrcode = ret;
							NETCoapNeedSendCode.ResponseInfo = 1;
							NET_NBIOT_Initialization();
							return;
						}
						else {
							ret = NETIP_UNKNOWNERROR;
						}
					}
				#endif
					/* Active */
				#if NBCOAP_DOWNLOAD_CMD_ACTIVE
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "Active") != NULL) {
						u16 activeval;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, \
							"{(Active):{(val):%hu,(Magic):%hu}}", &activeval, &recvMagicNum);
						if (recvMagicNum == TCLOD_MAGIC_NUM) {
							DeviceIdleMode = true;
							TCFG_EEPROM_SetActiveDevice(activeval);
							if (activeval) {
								DeviceActivedMode = true;
								BEEP_CtrlRepeat_Extend(5,30,70);
							#if NBCOAP_SENDCODE_WORK_INFO
								NETCoapNeedSendCode.WorkInfo = 1;
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
					}
				#endif
					/* MagMod */
				#if NBCOAP_DOWNLOAD_CMD_MAGMOD
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "MagMod") != NULL) {
						u16 magmodval;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, \
							"{(MagMod):{(val):%hu,(Magic):%hu}}", &magmodval, &recvMagicNum);
						if (recvMagicNum == TCLOD_MAGIC_NUM) {
							TCFG_EEPROM_SetMagMode(magmodval);
							talgo_set_magmod(magmodval);
						}
						else {
							ret = NETIP_UNKNOWNERROR;
						}
					}
				#endif
					/* NbHeart */
				#if NBCOAP_DOWNLOAD_CMD_NBHEART
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "NbHeart") != NULL) {
						u16 nbheartval;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, \
							"{(NbHeart):{(val):%hu,(Magic):%hu}}", &nbheartval, &recvMagicNum);
						if (recvMagicNum == TCLOD_MAGIC_NUM) {
							TCFG_EEPROM_SetNbiotHeart(TCFG_EEPROM_ChangeNbiotHeart(nbheartval));
							TCFG_SystemData.NBIotHeart = TCFG_EEPROM_GetNbiotHeart();
						}
						else {
							ret = NETIP_UNKNOWNERROR;
						}
					}
				#endif
					/* InitRadar */
				#if NBCOAP_DOWNLOAD_CMD_INITRADAR
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "InitRadar") != NULL) {
						u32 i32, j32, k32, m32;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, \
							"{(InitRadar):{(v23456):%u,(v7890a):%u,(vbcdef):%u,(vg):%u,(Magic):%hu}}", &i32, &j32, &k32, &m32, &recvMagicNum);
						if (recvMagicNum == TCLOD_MAGIC_NUM) {
							Radar_InitBG_Cmd(i32, j32, k32, m32);
						}
						else {
							ret = NETIP_UNKNOWNERROR;
						}
					}
				#endif
					/* InitMag */
				#if NBCOAP_DOWNLOAD_CMD_INITMAG
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "InitMag") != NULL) {
						s16 x, y, z;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, \
							"{(InitMag):{(x):%hd,(y):%hd,(z):%hd,(Magic):%hu}}", &x, &y, &z, &recvMagicNum);
						if (recvMagicNum == TCLOD_MAGIC_NUM) {
							QMC5883L_InitBackgroud_cmd(x, y, z);
						}
						else {
							ret = NETIP_UNKNOWNERROR;
						}
					}
				#endif
					/* DisRange */
				#if NBCOAP_DOWNLOAD_CMD_DISRANGE
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "DisRange") != NULL) {
						u16 disrangeval;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, \
							"{(DisRange):{(val):%hu,(Magic):%hu}}", &disrangeval, &recvMagicNum);
						if (recvMagicNum == TCLOD_MAGIC_NUM) {
							tradar_set_distance_range(disrangeval + 4);
							TCFG_EEPROM_SetRadarRange(disrangeval);
							TCFG_SystemData.RadarRange = TCFG_EEPROM_GetRadarRange();
						}
						else {
							ret = NETIP_UNKNOWNERROR;
						}
					}
				#endif
					/* CarInDelay */
				#if NBCOAP_DOWNLOAD_CMD_CARINDELAY
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "InDelay") != NULL) {
						u16 indelayval;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, \
							"{(InDelay):{(val):%hu,(Magic):%hu}}", &indelayval, &recvMagicNum);
						if (recvMagicNum == TCLOD_MAGIC_NUM) {
							TCFG_EEPROM_SetCarInDelay(indelayval);
							TCFG_SystemData.CarInDelay = TCFG_EEPROM_GetCarInDelay();
						}
						else {
							ret = NETIP_UNKNOWNERROR;
						}
					}
				#endif
					/* RATime */
				#if NBCOAP_DOWNLOAD_CMD_RATIME
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "RATime") != NULL) {
						u16 ratime;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, \
							"{(RATime):{(val):%hu,(Magic):%hu}}", &ratime, &recvMagicNum);
						if (recvMagicNum == TCLOD_MAGIC_NUM) {
							TCFG_EEPROM_SetCoapRATimeHour(ratime);
							TCFG_SystemData.CoapRATimeHour = TCFG_EEPROM_GetCoapRATimeHour();
						}
						else {
							ret = NETIP_UNKNOWNERROR;
						}
					}
				#endif
					/* MagTempCoef */
				#if NBCOAP_DOWNLOAD_CMD_MAGTEMPCOEF
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "MagCoef") != NULL) {
						short magTempCoefX, magTempCoefY, magTempCoefZ;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, \
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
					}
				#endif
					/* SetQmcCoef */
				#if NBCOAP_DOWNLOAD_CMD_SETQMCCOEF
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "SetQmcCoef") != NULL) {
						short magTempCoefX, magTempCoefY, magTempCoefZ;
						QMC5883L_measure_qmc_coef((signed char*)&magTempCoefX, (signed char*)&magTempCoefY, (signed char*)&magTempCoefZ);
						TCFG_SystemData.MagCoefX = magTempCoefX;
						TCFG_SystemData.MagCoefY = magTempCoefY;
						TCFG_SystemData.MagCoefZ = magTempCoefZ;
						TCFG_EEPROM_SetMagTempCoef(TCFG_SystemData.MagCoefX, TCFG_SystemData.MagCoefY, TCFG_SystemData.MagCoefZ);
					}
				#endif
					/* BeepOff */
				#if NBCOAP_DOWNLOAD_CMD_BEEPOFF
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "BeepOff") != NULL) {
						u16 beepoff;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, \
							"{(BeepOff):{(val):%hu,(Magic):%hu}}", &beepoff, &recvMagicNum);
						if (recvMagicNum == TCLOD_MAGIC_NUM) {
							TCFG_SystemData.BeepCtrlOff = beepoff;
							TCFG_EEPROM_SetBeepOff(TCFG_SystemData.BeepCtrlOff);
						}
						else {
							ret = NETIP_UNKNOWNERROR;
						}
					}
				#endif
					/* Rollinit */
				#if NBCOAP_DOWNLOAD_CMD_ROLLINIT
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "RollInit") != NULL) {
						u16 rollinit;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, \
							"{(RollInit):{(val):%hu,(Magic):%hu}}", &rollinit, &recvMagicNum);
						if (recvMagicNum == TCLOD_MAGIC_NUM) {
							TCFG_SystemData.RollingOverInitSensor = rollinit;
							TCFG_EEPROM_SetRollingOverInitSensor(TCFG_SystemData.RollingOverInitSensor);
						}
						else {
							ret = NETIP_UNKNOWNERROR;
						}
					}
				#endif
					/* UpLimit */
				#if NBCOAP_DOWNLOAD_CMD_UPLIMIT
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "UpLimit") != NULL) {
						short limitRssi, limitSnr;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, \
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
					}
				#endif
					/* NBLimit */
				#if NBCOAP_DOWNLOAD_CMD_NBLIMIT
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "NBLimit") != NULL) {
						short nblimit;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, \
							"{(NBLimit):{(val):%hd,(Magic):%hu}}", &nblimit, &recvMagicNum);
						if (recvMagicNum == TCLOD_MAGIC_NUM) {
							TCFG_EEPROM_SetNBIotSentCountLimit(nblimit);
						}
						else {
							ret = NETIP_UNKNOWNERROR;
						}
					}
				#endif
					/* CoverGain */
				#if NBCOAP_DOWNLOAD_CMD_COVERGAIN
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "CoverGain") != NULL) {
						uint16_t CoverGain;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, \
							"{(CoverGain):{(val):%hu,(Magic):%hu}}", &CoverGain, &recvMagicNum);
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
					}
				#endif
					/* RadarGain */
				#if NBCOAP_DOWNLOAD_CMD_RADARGAIN
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "RadarGain") != NULL) {
						uint16_t RadarGain;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, \
							"{(RadarGain):{(val):%hu,(Magic):%hu}}", &RadarGain, &recvMagicNum);
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
					}
				#endif
					/* SensorMode */
				#if NBCOAP_DOWNLOAD_CMD_SENSORMODE
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "SensorMode") != NULL) {
						uint16_t SenseMode;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, \
							"{(SensorMode):{(val):%hu,(Magic):%hu}}", &SenseMode, &recvMagicNum);
						if (recvMagicNum == TCLOD_MAGIC_NUM) {
							if (TCFG_SystemData.SenseMode != SenseMode) {
								TCFG_SystemData.SenseMode = SenseMode;
								TCFG_EEPROM_SetSenseMode(TCFG_SystemData.SenseMode);
							}
						}
						else {
							ret = NETIP_UNKNOWNERROR;
						}
					}
				#endif
					/* SetMag */
				#if NBCOAP_DOWNLOAD_CMD_SETMAG
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "SetMag") != NULL) {
						short x[5], y[5], z[5];
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, \
							"{(SetMag):{(x):[%hd,%hd,%hd,%hd,%hd],(y):[%hd,%hd,%hd,%hd,%hd],(z):[%hd,%hd,%hd,%hd,%hd],(Magic):%hu}}", \
							&x[0],&x[1],&x[2],&x[3],&x[4],&y[0],&y[1],&y[2],&y[3],&y[4],&z[0],&z[1],&z[2],&z[3],&z[4], &recvMagicNum);
						if (recvMagicNum == TCLOD_MAGIC_NUM) {
							for (unsigned char i = 0; i < 5; i++) {
								TCFG_SystemData.MagBackManualX[i] = x[i];
								TCFG_SystemData.MagBackManualY[i] = y[i];
								TCFG_SystemData.MagBackManualZ[i] = z[i];
								TCFG_EEPROM_SetMagManualBack(i, x[i], y[i], z[i]);
								if (x[i] != 0) talgo_set_manual_back(i, x[i], y[i], z[i]);
							}
						}
						else {
							ret = NETIP_UNKNOWNERROR;
						}
					}
				#endif
					/* ConfigRadar */
				#if NBCOAP_DOWNLOAD_CMD_CFGRADAR
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "CfgRadar") != NULL) {
						unsigned short interval, highpass, vtune;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, \
							"{(CfgRadar):{(interval):%hu,(hpass):%hu,(tune):%hu,(Magic):%hu}}", &interval, &highpass, &vtune, &recvMagicNum);
						if (recvMagicNum == TCLOD_MAGIC_NUM) {
							TCFG_SystemData.RadarSampleInterval = interval;
							Radar_Set_SampleInterval(interval);
							TCFG_EEPROM_SetSampleInterval(interval);
							TCFG_SystemData.RadarHighPass = highpass;
							TCFG_EEPROM_SetHighPass(highpass);
							TCFG_SystemData.RadarGain = vtune;
							TCFG_EEPROM_SetRadarGain(TCFG_SystemData.RadarGain);
						}
						else {
							ret = NETIP_UNKNOWNERROR;
						}
					}
				#endif
					else {
						ret = NETIP_NOTSUPPORT;
					}
					/* ...... */
				}
				else if (pClient->Recvbuf[recvBufOffset + TCLOD_MSGID_OFFSET] == TCLOD_CONFIG_GET) {
					BEEP_CtrlRepeat_Extend(2, 50, 50);
					/* Workinfo */
					if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "WorkInfo") != NULL) {
				#if NBCOAP_SENDCODE_WORK_INFO
						NETCoapNeedSendCode.WorkInfo = 1;
				#endif
					}
					/* BasicInfo */
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "BasicInfo") != NULL) {
				#if NBCOAP_SENDCODE_BASIC_INFO
						NETCoapNeedSendCode.BasicInfo = 1;
				#endif
					}
					/* DynamicInfo */
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "DynamicInfo") != NULL) {
				#if NBCOAP_SENDCODE_DYNAMIC_INFO
						NETCoapNeedSendCode.DynamicInfo = 1;
				#endif
					}
					else {
						ret = NETIP_NOTSUPPORT;
					}
					/* ...... */
				}
				else {
					ret = NETIP_NOTSUPPORT;
				}
			}
			else {
				ret = NETIP_NOTSUPPORT;
			}
			
			NETCoapNeedSendCode.ResponseInfoMsgId = pClient->Recvbuf[recvBufOffset + TCLOD_MSGTYPE_OFFSET];
			NETCoapNeedSendCode.ResponseInfoErrcode = ret;
			NETCoapNeedSendCode.ResponseInfo = 1;
		}
		else {
			/* Not Valid */
			ret = NETIP_NOTVALID;
		}
		
		/* PCP升级协议下行数据 */
		for (int i = 0; i < pClient->Recvlen; i++) {
			if ((pClient->Recvbuf[i] == 0xFF) && (pClient->Recvbuf[i+1] == 0xFE)) {
				recvBufOffset = i;
				recvPCPUpgradeFlag = true;
				break;
			}
		}
		
		if (recvPCPUpgradeFlag != false) {
			/* Find "0xFFFE" */
			NET_PCP_Message_RecvDataEnqueue(pClient->Recvbuf + recvBufOffset, pClient->Recvlen - recvBufOffset);
		}
		
		NET_Coap_Message_RecvDataOffSet();
	}
	
	pClient->DictateRunCtl.dictateEnable = false;
	pClient->DictateRunCtl.dictateEvent = LISTEN_RUN_CTL;
}


/**********************************************************************************************************
 @Function			void NET_COAP_Listen_PollExecution(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_Listen_PollExecution			: COAP监听器处理
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_Listen_PollExecution(NBIOT_ClientsTypeDef* pClient)
{
	switch (pClient->ListenRunCtl.listenEvent)
	{
	case NBCOAP_LISTEN_MODE_ENTER_NONE:
		NET_COAP_NBIOT_Listen_Event_EnterNone(pClient);
		break;
	
	case NBCOAP_LISTEN_MODE_ENTER_PARAMETER:
#if NBCOAP_LISTEN_PARAMETER_TYPE == NBCOAP_LISTEN_PARAMETER_ENABLE
		NET_COAP_NBIOT_Listen_Event_EnterParameter(pClient);
#endif
		break;
	}
}

/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Listen_Event_EnterNone(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Listen_Event_EnterNone		: 事件(进入None模式)监听
 @Input				pClient								: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_NBIOT_Listen_Event_EnterNone(NBIOT_ClientsTypeDef* pClient)
{
	pClient->DictateRunCtl.dictateEnable = false;
	pClient->DictateRunCtl.dictateEvent = NBCOAP_SENDMODE_TYPE;
	pClient->ListenRunCtl.listenEvent = NBCOAP_LISTEN_DEFAULT_BOOTMODE;
	pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_PCP;
}

#if NBCOAP_LISTEN_PARAMETER_TYPE == NBCOAP_LISTEN_PARAMETER_ENABLE
/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Listen_Enable_EnterParameter(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Listen_Enable_EnterParameter	: 使能(进入NBIOT运行信息)监听
 @Input				pClient								: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_NBIOT_Listen_Enable_EnterParameter(NBIOT_ClientsTypeDef* pClient)
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
 @Function			void NET_COAP_NBIOT_Listen_Event_EnterParameter(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Listen_Event_EnterParameter	: 事件(进入NBIOT运行信息)监听
 @Input				pClient								: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_NBIOT_Listen_Event_EnterParameter(NBIOT_ClientsTypeDef* pClient)
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
			
			if ((NBIOT_Neul_NBxx_CheckReadRSSI(pClient) == NBIOT_OK) &&
			    (NBIOT_Neul_NBxx_CheckReadStatisticsRADIO(pClient) == NBIOT_OK)) {
				/* Dictate execute is Success */
				pClient->ListenRunCtl.ListenEnterParameter.EventCtl.eventEnable = false;
				pClient->ListenRunCtl.listenEvent = NBCOAP_LISTEN_MODE_ENTER_PARAMETER;
				pClient->ListenRunCtl.ListenEnterParameter.EventCtl.eventFailureCnt = 0;
#ifdef COAP_DEBUG_LOG_RF_PRINT
				COAP_DEBUG_LOG_PRINTF("CoAP Para Check Ok");
				Radio_Trf_Printf("RSSI: %d", pClient->Parameter.rssi);
				Radio_Trf_Printf("SNR: %d", pClient->Parameter.statisticsRADIO.SNR);
#endif
			}
			else {
				/* Dictate execute is Fail */
				if (Stm32_Calculagraph_IsExpiredSec(&pClient->ListenRunCtl.ListenEnterParameter.EventCtl.eventRunTime) == true) {
					/* Dictate TimeOut */
					pClient->ListenRunCtl.ListenEnterParameter.EventCtl.eventEnable = false;
					pClient->ListenRunCtl.listenEvent = NBCOAP_LISTEN_MODE_ENTER_PARAMETER;
					pClient->DictateRunCtl.dictateEnable = false;
					pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
					pClient->ListenRunCtl.ListenEnterParameter.EventCtl.eventFailureCnt++;
					if (pClient->ListenRunCtl.ListenEnterParameter.EventCtl.eventFailureCnt > 3) {
						pClient->ListenRunCtl.ListenEnterParameter.EventCtl.eventFailureCnt = 0;
						pClient->DictateRunCtl.dictateEvent = STOP_MODE;
					}
				}
				else {
					/* Dictate isn't TimeOut */
					pClient->ListenRunCtl.listenEvent = NBCOAP_LISTEN_MODE_ENTER_PARAMETER;
				}
#ifdef COAP_DEBUG_LOG_RF_PRINT
				COAP_DEBUG_LOG_PRINTF("CoAP Para Check Fail");
#endif
				return;
			}
			
			pClient->ListenRunCtl.ListenEnterParameter.listenEnable = false;
			pClient->ListenRunCtl.ListenEnterParameter.listenStatus = false;
			pClient->ListenRunCtl.listenEvent = NBCOAP_LISTEN_MODE_ENTER_PARAMETER;
		}
	}
	
	pClient->DictateRunCtl.dictateEnable = false;
	pClient->DictateRunCtl.dictateEvent = NBCOAP_SENDMODE_TYPE;
	pClient->ListenRunCtl.listenEvent = NBCOAP_LISTEN_DEFAULT_BOOTMODE;
	pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_PCP;
}
#endif

/********************************************** END OF FLEE **********************************************/
