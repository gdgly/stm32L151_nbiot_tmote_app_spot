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
#include "string.h"
#include "radio_rf_app.h"

/**********************************************************************************************************
 @Function			void NET_COAP_APP_PollExecution(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_APP_PollExecution			: COAP逻辑处理
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
		NET_COAP_NBIOT_Event_PatameterCheckOut(pClient);
		break;
	
	case MINIMUM_FUNCTIONALITY:
		NET_COAP_NBIOT_Event_MinimumFunctionality(pClient);
		break;
	
	case FULL_FUNCTIONALITY:
		NET_COAP_NBIOT_Event_FullFunctionality(pClient);
		break;
	
	case CDP_SERVER_CHECK:
		NET_COAP_NBIOT_Event_CDPServerCheck(pClient);
		break;
	
	case CDP_SERVER_CONFIG:
		NET_COAP_NBIOT_Event_CDPServerConfig(pClient);
		break;
	
	case SEND_DATA:
		NET_COAP_NBIOT_Event_SendData(pClient);
		break;
	
	case RECV_DATA:
		NET_COAP_NBIOT_Event_RecvData(pClient);
		break;
	
	case SEND_DATA_RA_NORMAL:
		NET_COAP_NBIOT_Event_SendDataRANormal(pClient);
		break;
	
	case RECV_DATA_RA_NORMAL:
		NET_COAP_NBIOT_Event_RecvDataRANormal(pClient);
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
		/* Send Message Index */
		CoapSendMessageIndex = NET_Coap_Message_SendDataRear();
		/* Into ConnectTime & IdleTime */
		TCFG_SystemData.CoapConnectTime = TCFG_EEPROM_GetCoapConnectTime() + Stm32_EventRunningTime_EndMS(&pClient->ConnectTimeMS) / 1000;
		TCFG_EEPROM_SetCoapConnectTime(TCFG_SystemData.CoapConnectTime);
		TCFG_SystemData.CoapIdleTime = TCFG_EEPROM_GetCoapIdleTime() + Stm32_EventRunningTime_EndMS(&pClient->IdleTimeMS) / 1000;
		TCFG_EEPROM_SetCoapIdleTime(TCFG_SystemData.CoapIdleTime);
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
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = MODULE_CHECK;
		pClient->DictateRunCtl.dictateRebootFailureCnt = 0;
		/* End IdleTime */
		TCFG_SystemData.CoapIdleTime = TCFG_EEPROM_GetCoapIdleTime() + Stm32_EventRunningTime_EndMS(&pClient->IdleTimeMS) / 1000;
		TCFG_EEPROM_SetCoapIdleTime(TCFG_SystemData.CoapIdleTime);
		/* Start ConnectTime */
		Stm32_EventRunningTime_StartMS(&pClient->ConnectTimeMS);
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB HDRBT Ok, Baud:%d", NBIOTBaudRate.Baud);
#endif
	}
	else {
		/* Dictate execute is Fail */
		if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
			/* Dictate TimeOut */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->DictateRunCtl.dictateRebootFailureCnt++;
			if (pClient->DictateRunCtl.dictateRebootFailureCnt > 3) {
				pClient->DictateRunCtl.dictateRebootFailureCnt = 0;
				pClient->DictateRunCtl.dictateEvent = STOP_MODE;
			}
		}
		else {
			/* Dictate isn't TimeOut */
			pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		}
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB HDRBT Fail");
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
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBIOT_Neul_NBxx_CheckReadManufacturer(pClient) == NBIOT_OK) && 
	    (NBIOT_Neul_NBxx_CheckReadManufacturerModel(pClient) == NBIOT_OK) &&
	    (NBIOT_Neul_NBxx_CheckReadModuleVersion(pClient) == NBIOT_OK)) {
		/* Dictate execute is Success */
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = PARAMETER_CONFIG;
		pClient->DictateRunCtl.dictateModuleCheckFailureCnt = 0;
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB Module Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
			/* Dictate TimeOut */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->DictateRunCtl.dictateModuleCheckFailureCnt++;
			if (pClient->DictateRunCtl.dictateModuleCheckFailureCnt > 3) {
				pClient->DictateRunCtl.dictateModuleCheckFailureCnt = 0;
				pClient->DictateRunCtl.dictateEvent = STOP_MODE;
			}
		}
		else {
			/* Dictate isn't TimeOut */
			pClient->DictateRunCtl.dictateEvent = MODULE_CHECK;
		}
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB Module Check Fail");
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
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (NBIOT_Neul_NBxx_CheckReadConfigUE(pClient) == NBIOT_OK) {
		/* Dictate execute is Success */
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = ICCID_CHECK;
		pClient->DictateRunCtl.dictateParameterConfigFailureCnt = 0;
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB Parameter Config Read Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
			/* Dictate TimeOut */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->DictateRunCtl.dictateParameterConfigFailureCnt++;
			if (pClient->DictateRunCtl.dictateParameterConfigFailureCnt > 3) {
				pClient->DictateRunCtl.dictateParameterConfigFailureCnt = 0;
				pClient->DictateRunCtl.dictateEvent = STOP_MODE;
			}
		}
		else {
			/* Dictate isn't TimeOut */
			pClient->DictateRunCtl.dictateEvent = PARAMETER_CONFIG;
		}
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB Parameter Config Read Fail");
#endif
		return;
	}
	
	if (pClient->Parameter.nconfig.autoConnect != AutoConnectVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient, AutoConnect, AutoConnectVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = ICCID_CHECK;
			pClient->DictateRunCtl.dictateParameterConfigFailureCnt = 0;
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Ok", AutoConnect, AutoConnectVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->DictateRunCtl.dictateParameterConfigFailureCnt++;
				if (pClient->DictateRunCtl.dictateParameterConfigFailureCnt > 3) {
					pClient->DictateRunCtl.dictateParameterConfigFailureCnt = 0;
					pClient->DictateRunCtl.dictateEvent = STOP_MODE;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->DictateRunCtl.dictateEvent = PARAMETER_CONFIG;
			}
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Fail", AutoConnect, AutoConnectVal);
#endif
			return;
		}
	}
	
	if (pClient->Parameter.nconfig.crScrambling != CrScramblingVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient, CrScrambling, CrScramblingVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = ICCID_CHECK;
			pClient->DictateRunCtl.dictateParameterConfigFailureCnt = 0;
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Ok", CrScrambling, CrScramblingVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->DictateRunCtl.dictateParameterConfigFailureCnt++;
				if (pClient->DictateRunCtl.dictateParameterConfigFailureCnt > 3) {
					pClient->DictateRunCtl.dictateParameterConfigFailureCnt = 0;
					pClient->DictateRunCtl.dictateEvent = STOP_MODE;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->DictateRunCtl.dictateEvent = PARAMETER_CONFIG;
			}
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Fail", CrScrambling, CrScramblingVal);
#endif
			return;
		}
	}
	
	if (pClient->Parameter.nconfig.crSiAvoid != CrSiAvoidVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient, CrSiAvoid, CrSiAvoidVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = ICCID_CHECK;
			pClient->DictateRunCtl.dictateParameterConfigFailureCnt = 0;
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Ok", CrSiAvoid, CrSiAvoidVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->DictateRunCtl.dictateParameterConfigFailureCnt++;
				if (pClient->DictateRunCtl.dictateParameterConfigFailureCnt > 3) {
					pClient->DictateRunCtl.dictateParameterConfigFailureCnt = 0;
					pClient->DictateRunCtl.dictateEvent = STOP_MODE;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->DictateRunCtl.dictateEvent = PARAMETER_CONFIG;
			}
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Fail", CrSiAvoid, CrSiAvoidVal);
#endif
			return;
		}
	}
	
	if (pClient->Parameter.nconfig.combineAttach != CombineAttachVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient, CombineAttach, CombineAttachVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = ICCID_CHECK;
			pClient->DictateRunCtl.dictateParameterConfigFailureCnt = 0;
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Ok", CombineAttach, CombineAttachVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->DictateRunCtl.dictateParameterConfigFailureCnt++;
				if (pClient->DictateRunCtl.dictateParameterConfigFailureCnt > 3) {
					pClient->DictateRunCtl.dictateParameterConfigFailureCnt = 0;
					pClient->DictateRunCtl.dictateEvent = STOP_MODE;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->DictateRunCtl.dictateEvent = PARAMETER_CONFIG;
			}
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Fail", CombineAttach, CombineAttachVal);
#endif
			return;
		}
	}
	
	if (pClient->Parameter.nconfig.cellReselection != CellReselectionVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient, CellReselection, CellReselectionVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = ICCID_CHECK;
			pClient->DictateRunCtl.dictateParameterConfigFailureCnt = 0;
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Ok", CellReselection, CellReselectionVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->DictateRunCtl.dictateParameterConfigFailureCnt++;
				if (pClient->DictateRunCtl.dictateParameterConfigFailureCnt > 3) {
					pClient->DictateRunCtl.dictateParameterConfigFailureCnt = 0;
					pClient->DictateRunCtl.dictateEvent = STOP_MODE;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->DictateRunCtl.dictateEvent = PARAMETER_CONFIG;
			}
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Fail", CellReselection, CellReselectionVal);
#endif
			return;
		}
	}
	
	if (pClient->Parameter.nconfig.enableBip != EnableBipVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient, EnableBip, EnableBipVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = ICCID_CHECK;
			pClient->DictateRunCtl.dictateParameterConfigFailureCnt = 0;
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Ok", EnableBip, EnableBipVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->DictateRunCtl.dictateParameterConfigFailureCnt++;
				if (pClient->DictateRunCtl.dictateParameterConfigFailureCnt > 3) {
					pClient->DictateRunCtl.dictateParameterConfigFailureCnt = 0;
					pClient->DictateRunCtl.dictateEvent = STOP_MODE;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->DictateRunCtl.dictateEvent = PARAMETER_CONFIG;
			}
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Fail", EnableBip, EnableBipVal);
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
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (NBIOT_Neul_NBxx_CheckReadICCID(pClient) == NBIOT_OK) {
		/* Dictate execute is Success */
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = FULL_FUNCTIONALITY;
		pClient->DictateRunCtl.dictateSimICCIDCheckFailureCnt = 0;
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB ICCID Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
			/* Dictate TimeOut */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->DictateRunCtl.dictateSimICCIDCheckFailureCnt++;
			if (pClient->DictateRunCtl.dictateSimICCIDCheckFailureCnt > 3) {
				pClient->DictateRunCtl.dictateSimICCIDCheckFailureCnt = 0;
				pClient->DictateRunCtl.dictateEvent = STOP_MODE;
			}
		}
		else {
			/* Dictate isn't TimeOut */
			pClient->DictateRunCtl.dictateEvent = ICCID_CHECK;
		}
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB ICCID Check Fail");
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
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (NBIOT_Neul_NBxx_CheckReadMinOrFullFunc(pClient) == NBIOT_OK) {
		/* Dictate execute is Success */
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = CDP_SERVER_CHECK;
		pClient->DictateRunCtl.dictateFullFunctionalityFailureCnt = 0;
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("Coap FullFunc Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
			/* Dictate TimeOut */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->DictateRunCtl.dictateFullFunctionalityFailureCnt++;
			if (pClient->DictateRunCtl.dictateFullFunctionalityFailureCnt > 3) {
				pClient->DictateRunCtl.dictateFullFunctionalityFailureCnt = 0;
				pClient->DictateRunCtl.dictateEvent = STOP_MODE;
			}
		}
		else {
			/* Dictate isn't TimeOut */
			pClient->DictateRunCtl.dictateEvent = FULL_FUNCTIONALITY;
		}
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("Coap FullFunc Check Fail");
#endif
		return;
	}
	
	if (pClient->Parameter.functionality != FullFunc) {
		if (NBIOT_Neul_NBxx_SetMinOrFullFunc(pClient, FullFunc) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = CDP_SERVER_CHECK;
			pClient->DictateRunCtl.dictateFullFunctionalityFailureCnt = 0;
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("Coap FullFunc Set Ok");
#endif
		}
		else {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->DictateRunCtl.dictateFullFunctionalityFailureCnt++;
				if (pClient->DictateRunCtl.dictateFullFunctionalityFailureCnt > 3) {
					pClient->DictateRunCtl.dictateFullFunctionalityFailureCnt = 0;
					pClient->DictateRunCtl.dictateEvent = STOP_MODE;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->DictateRunCtl.dictateEvent = FULL_FUNCTIONALITY;
			}
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("Coap FullFunc Set Fail");
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
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (NBIOT_Neul_NBxx_CheckReadMinOrFullFunc(pClient) == NBIOT_OK) {
		/* Dictate execute is Success */
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = CDP_SERVER_CONFIG;
		pClient->DictateRunCtl.dictateMinimumFunctionalityFailureCnt = 0;
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("Coap MinFunc Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
			/* Dictate TimeOut */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->DictateRunCtl.dictateMinimumFunctionalityFailureCnt++;
			if (pClient->DictateRunCtl.dictateMinimumFunctionalityFailureCnt > 3) {
				pClient->DictateRunCtl.dictateMinimumFunctionalityFailureCnt = 0;
				pClient->DictateRunCtl.dictateEvent = STOP_MODE;
			}
		}
		else {
			/* Dictate isn't TimeOut */
			pClient->DictateRunCtl.dictateEvent = MINIMUM_FUNCTIONALITY;
		}
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("Coap MinFunc Check Fail");
#endif
		return;
	}
	
	if (pClient->Parameter.functionality != MinFunc) {
		if (NBIOT_Neul_NBxx_SetMinOrFullFunc(pClient, MinFunc) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = CDP_SERVER_CONFIG;
			pClient->DictateRunCtl.dictateMinimumFunctionalityFailureCnt = 0;
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("Coap MinFunc Set Ok");
#endif
		}
		else {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->DictateRunCtl.dictateMinimumFunctionalityFailureCnt++;
				if (pClient->DictateRunCtl.dictateMinimumFunctionalityFailureCnt > 3) {
					pClient->DictateRunCtl.dictateMinimumFunctionalityFailureCnt = 0;
					pClient->DictateRunCtl.dictateEvent = STOP_MODE;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->DictateRunCtl.dictateEvent = MINIMUM_FUNCTIONALITY;
			}
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("Coap MinFunc Set Fail");
#endif
			return;
		}
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
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (NBIOT_Neul_NBxx_CheckReadCDPServer(pClient) == NBIOT_OK) {
		/* Dictate execute is Success */
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = CDP_SERVER_CHECK;
		pClient->DictateRunCtl.dictateCDPServerCheckFailureCnt = 0;
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("Coap CDP Read %s:%d Ok", pClient->Parameter.cdpserver.CDPServerHost, pClient->Parameter.cdpserver.CDPServerPort);
#endif
	}
	else {
		/* Dictate execute is Fail */
		if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
			/* Dictate TimeOut */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->DictateRunCtl.dictateCDPServerCheckFailureCnt++;
			if (pClient->DictateRunCtl.dictateCDPServerCheckFailureCnt > 3) {
				pClient->DictateRunCtl.dictateCDPServerCheckFailureCnt = 0;
				pClient->DictateRunCtl.dictateEvent = STOP_MODE;
			}
		}
		else {
			/* Dictate isn't TimeOut */
			pClient->DictateRunCtl.dictateEvent = CDP_SERVER_CHECK;
		}
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("Coap CDP Read %s:%d Fail", pClient->Parameter.cdpserver.CDPServerHost, pClient->Parameter.cdpserver.CDPServerPort);
#endif
	}
	
	if ((strcmp((const char*)pClient->Parameter.cdpserver.CDPServerHost, TCFG_EEPROM_Get_ServerIP_String()) != 0) || (pClient->Parameter.cdpserver.CDPServerPort != TCFG_EEPROM_GetServerPort())) {
		/* CDP Server Mast be Config */
		pClient->DictateRunCtl.dictateEvent = MINIMUM_FUNCTIONALITY;
	}
	else {
		/* CDP Server Needn't be Config */
		pClient->DictateRunCtl.dictateEvent = MISC_EQUIP_CONFIG;
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
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (NBIOT_Neul_NBxx_CheckReadCDPServer(pClient) == NBIOT_OK) {
		/* Dictate execute is Success */
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = FULL_FUNCTIONALITY;
		pClient->DictateRunCtl.dictateCDPServerConfigFailureCnt = 0;
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("Coap CDP Read %s:%d Ok", pClient->Parameter.cdpserver.CDPServerHost, pClient->Parameter.cdpserver.CDPServerPort);
#endif
	}
	else {
		/* Dictate execute is Fail */
		if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
			/* Dictate TimeOut */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->DictateRunCtl.dictateCDPServerConfigFailureCnt++;
			if (pClient->DictateRunCtl.dictateCDPServerConfigFailureCnt > 3) {
				pClient->DictateRunCtl.dictateCDPServerConfigFailureCnt = 0;
				pClient->DictateRunCtl.dictateEvent = STOP_MODE;
			}
		}
		else {
			/* Dictate isn't TimeOut */
			pClient->DictateRunCtl.dictateEvent = CDP_SERVER_CONFIG;
		}
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("Coap CDP Read %s:%d Fail", pClient->Parameter.cdpserver.CDPServerHost, pClient->Parameter.cdpserver.CDPServerPort);
#endif
	}
	
	if ((strcmp((const char*)pClient->Parameter.cdpserver.CDPServerHost, TCFG_EEPROM_Get_ServerIP_String()) != 0) || (pClient->Parameter.cdpserver.CDPServerPort != TCFG_EEPROM_GetServerPort())) {
		/* CDP Server Mast be Config */
		if (NBIOT_Neul_NBxx_SetCDPServer(pClient, TCFG_EEPROM_Get_ServerIP_String(), TCFG_EEPROM_GetServerPort()) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = FULL_FUNCTIONALITY;
			pClient->DictateRunCtl.dictateCDPServerConfigFailureCnt = 0;
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("Coap CDP Set %s:%d Ok", TCFG_EEPROM_Get_ServerIP_String(), TCFG_EEPROM_GetServerPort());
#endif
		}
		else {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->DictateRunCtl.dictateCDPServerConfigFailureCnt++;
				if (pClient->DictateRunCtl.dictateCDPServerConfigFailureCnt > 3) {
					pClient->DictateRunCtl.dictateCDPServerConfigFailureCnt = 0;
					pClient->DictateRunCtl.dictateEvent = STOP_MODE;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->DictateRunCtl.dictateEvent = CDP_SERVER_CONFIG;
			}
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("Coap CDP Set %s:%d Fail", TCFG_EEPROM_Get_ServerIP_String(), TCFG_EEPROM_GetServerPort());
#endif
			return;
		}
	}
	else {
		/* CDP Server Needn't be Config */
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
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBIOT_Neul_NBxx_CheckReadSupportedBands(pClient) == NBIOT_OK) &&
	    (NBIOT_Neul_NBxx_CheckReadNewMessageIndications(pClient) == NBIOT_OK) &&
	    (NBIOT_Neul_NBxx_CheckReadSentMessageIndications(pClient) == NBIOT_OK)) {
		/* Dictate execute is Success */
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = ATTACH_CHECK;
		pClient->DictateRunCtl.dictateMiscEquipConfigFailureCnt = 0;
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("Coap MiscEquip Read Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
			/* Dictate TimeOut */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->DictateRunCtl.dictateMiscEquipConfigFailureCnt++;
			if (pClient->DictateRunCtl.dictateMiscEquipConfigFailureCnt > 3) {
				pClient->DictateRunCtl.dictateMiscEquipConfigFailureCnt = 0;
				pClient->DictateRunCtl.dictateEvent = STOP_MODE;
			}
		}
		else {
			/* Dictate isn't TimeOut */
			pClient->DictateRunCtl.dictateEvent = MISC_EQUIP_CONFIG;
		}
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("Coap MiscEquip Read Fail");
#endif
		return;
	}
	
	if (pClient->Parameter.band != COAP_NBIOT_BAND) {
		if (NBIOT_Neul_NBxx_SetSupportedBands(pClient, COAP_NBIOT_BAND) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = ATTACH_CHECK;
			pClient->DictateRunCtl.dictateMiscEquipConfigFailureCnt = 0;
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("Coap Band Set %d Ok", COAP_NBIOT_BAND);
#endif
		}
		else {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->DictateRunCtl.dictateMiscEquipConfigFailureCnt++;
				if (pClient->DictateRunCtl.dictateMiscEquipConfigFailureCnt > 3) {
					pClient->DictateRunCtl.dictateMiscEquipConfigFailureCnt = 0;
					pClient->DictateRunCtl.dictateEvent = STOP_MODE;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->DictateRunCtl.dictateEvent = MISC_EQUIP_CONFIG;
			}
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("Coap Band Set %d Fail", COAP_NBIOT_BAND);
#endif
			return;
		}
	}
	
	if (pClient->Parameter.nnmistate != CloseFunc) {
		if (NBIOT_Neul_NBxx_SetNewMessageIndications(pClient, CloseFunc) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = ATTACH_CHECK;
			pClient->DictateRunCtl.dictateMiscEquipConfigFailureCnt = 0;
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("Coap NNMI Set %d Ok", CloseFunc);
#endif
		}
		else {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->DictateRunCtl.dictateMiscEquipConfigFailureCnt++;
				if (pClient->DictateRunCtl.dictateMiscEquipConfigFailureCnt > 3) {
					pClient->DictateRunCtl.dictateMiscEquipConfigFailureCnt = 0;
					pClient->DictateRunCtl.dictateEvent = STOP_MODE;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->DictateRunCtl.dictateEvent = MISC_EQUIP_CONFIG;
			}
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("Coap NNMI Set %d Fail", CloseFunc);
#endif
			return;
		}
	}
	
	if (pClient->Parameter.nsmistate != CloseFunc) {
		if (NBIOT_Neul_NBxx_SetSentMessageIndications(pClient, CloseFunc) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = ATTACH_CHECK;
			pClient->DictateRunCtl.dictateMiscEquipConfigFailureCnt = 0;
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("Coap NSMI Set %d Ok", CloseFunc);
#endif
		}
		else {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->DictateRunCtl.dictateMiscEquipConfigFailureCnt++;
				if (pClient->DictateRunCtl.dictateMiscEquipConfigFailureCnt > 3) {
					pClient->DictateRunCtl.dictateMiscEquipConfigFailureCnt = 0;
					pClient->DictateRunCtl.dictateEvent = STOP_MODE;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->DictateRunCtl.dictateEvent = MISC_EQUIP_CONFIG;
			}
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("Coap NSMI Set %d Fail", CloseFunc);
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
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (NBIOT_Neul_NBxx_CheckReadAttachOrDetach(pClient) == NBIOT_OK) {
		/* Dictate execute is Success */
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = ATTACH_CHECK;
		pClient->DictateRunCtl.dictateAttachCheckFailureCnt = 0;
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("Coap CGATT %d Ok", pClient->Parameter.netstate);
#endif
	}
	else {
		/* Dictate execute is Fail */
		if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
			/* Dictate TimeOut */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->DictateRunCtl.dictateAttachCheckFailureCnt++;
			if (pClient->DictateRunCtl.dictateAttachCheckFailureCnt > 3) {
				pClient->DictateRunCtl.dictateAttachCheckFailureCnt = 0;
				pClient->DictateRunCtl.dictateEvent = STOP_MODE;
			}
		}
		else {
			/* Dictate isn't TimeOut */
			pClient->DictateRunCtl.dictateEvent = ATTACH_CHECK;
		}
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("Coap CGATT %d Fail", pClient->Parameter.netstate);
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
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (NBIOT_Neul_NBxx_SetAttachOrDetach(pClient, Attach) == NBIOT_OK) {
		/* Dictate execute is Success */
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = ATTACH_INQUIRE;
		pClient->DictateRunCtl.dictateAttachExecuteFailureCnt = 0;
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("Coap Set CGATT %d Ok", Attach);
#endif
	}
	else {
		/* Dictate execute is Fail */
		if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
			/* Dictate TimeOut */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->DictateRunCtl.dictateAttachExecuteFailureCnt++;
			if (pClient->DictateRunCtl.dictateAttachExecuteFailureCnt > 3) {
				pClient->DictateRunCtl.dictateAttachExecuteFailureCnt = 0;
				pClient->DictateRunCtl.dictateEvent = STOP_MODE;
			}
		}
		else {
			/* Dictate isn't TimeOut */
			pClient->DictateRunCtl.dictateEvent = ATTACH_EXECUTE;
		}
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("Coap Set CGATT %d Fail", Attach);
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
	COAP_NBIOT_DictateEvent_SetTime(pClient, 60);
	
	if (NBIOT_Neul_NBxx_CheckReadAttachOrDetach(pClient) == NBIOT_OK) {
		/* Dictate execute is Success */
		pClient->DictateRunCtl.dictateEvent = ATTACH_INQUIRE;
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("Coap CGATT %d Ok", pClient->Parameter.netstate);
#endif
	}
	else {
		/* Dictate execute is Fail */
		if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
			/* Dictate TimeOut */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->DictateRunCtl.dictateAttachInquireFailureCnt++;
			if (pClient->DictateRunCtl.dictateAttachInquireFailureCnt > 3) {
				pClient->DictateRunCtl.dictateAttachInquireFailureCnt = 0;
				pClient->DictateRunCtl.dictateEvent = STOP_MODE;
			}
		}
		else {
			/* Dictate isn't TimeOut */
			pClient->DictateRunCtl.dictateEvent = ATTACH_INQUIRE;
		}
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("Coap CGATT %d Fail", pClient->Parameter.netstate);
#endif
		return;
	}
	
	if (pClient->Parameter.netstate != Attach) {
		if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->DictateRunCtl.dictateAttachInquireFailureCnt++;
			if (pClient->DictateRunCtl.dictateAttachInquireFailureCnt > 3) {
				pClient->DictateRunCtl.dictateAttachInquireFailureCnt = 0;
				pClient->DictateRunCtl.dictateEvent = STOP_MODE;
			}
		}
		else {
			pClient->DictateRunCtl.dictateEvent = ATTACH_INQUIRE;
		}
	}
	else {
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = PARAMETER_CHECKOUT;
		pClient->DictateRunCtl.dictateAttachInquireFailureCnt = 0;
	}
}

/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_PatameterCheckOut(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_PatameterCheckOut	: 参数检出
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_NBIOT_Event_PatameterCheckOut(NBIOT_ClientsTypeDef* pClient)
{
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBIOT_Neul_NBxx_CheckReadIMEI(pClient) == NBIOT_OK) && 
	    (NBIOT_Neul_NBxx_CheckReadIMEISV(pClient) == NBIOT_OK) && 
	    (NBIOT_Neul_NBxx_CheckReadIMSI(pClient) == NBIOT_OK) && 
	    (NBIOT_Neul_NBxx_CheckReadCGPADDR(pClient) == NBIOT_OK) && 
	    (NBIOT_Neul_NBxx_CheckReadCGDCONT(pClient) == NBIOT_OK) && 
	    (NBIOT_Neul_NBxx_CheckReadRSSI(pClient) == NBIOT_OK) && 
	    (NBIOT_Neul_NBxx_CheckReadStatisticsRADIO(pClient) == NBIOT_OK) && 
	    (NBIOT_Neul_NBxx_CheckReadStatisticsCELL(pClient) == NBIOT_OK) && 
	    (NBIOT_Neul_NBxx_CheckReadDateTime(pClient) == NBIOT_OK)) {
		/* Dictate execute is Success */
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = NBCOAP_SENDMODE_TYPE;
		pClient->DictateRunCtl.dictatePatameterCheckOutFailureCnt = 0;
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("Coap Patameter Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
			/* Dictate TimeOut */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->DictateRunCtl.dictatePatameterCheckOutFailureCnt++;
			if (pClient->DictateRunCtl.dictatePatameterCheckOutFailureCnt > 3) {
				pClient->DictateRunCtl.dictatePatameterCheckOutFailureCnt = 0;
				pClient->DictateRunCtl.dictateEvent = STOP_MODE;
			}
		}
		else {
			/* Dictate isn't TimeOut */
			pClient->DictateRunCtl.dictateEvent = PARAMETER_CHECKOUT;
		}
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("Coap Patameter Check Fail");
#endif
		return;
	}
	
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
	int SendSentNum = 0;
	
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	/* Data packets need to be sent*/
	if (NET_Coap_Message_SendDataDequeue(pClient->Sendbuf, (unsigned short *)&pClient->Sendlen) == true) {
		/* End IdleTime */
		TCFG_SystemData.CoapIdleTime = TCFG_EEPROM_GetCoapIdleTime() + Stm32_EventRunningTime_EndMS(&pClient->IdleTimeMS) / 1000;
		TCFG_EEPROM_SetCoapIdleTime(TCFG_SystemData.CoapIdleTime);
		/* Start ConnectTime */
		Stm32_EventRunningTime_StartMS(&pClient->ConnectTimeMS);
		/* Connect Check */
		if (NBIOT_Neul_NBxx_CheckReadAttachOrDetach(pClient) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEvent = SEND_DATA;
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("Coap CGATT %d Ok", pClient->Parameter.netstate);
#endif
		}
		else {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->DictateRunCtl.dictateSendDataFailureCnt++;
				if (pClient->DictateRunCtl.dictateSendDataFailureCnt > 3) {
					pClient->DictateRunCtl.dictateSendDataFailureCnt = 0;
					pClient->DictateRunCtl.dictateEvent = STOP_MODE;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->DictateRunCtl.dictateEvent = SEND_DATA;
			}
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("Coap CGATT %d Fail", pClient->Parameter.netstate);
#endif
			return;
		}
		
		if (pClient->Parameter.netstate != Attach) {
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->DictateRunCtl.dictateSendDataFailureCnt++;
				if (pClient->DictateRunCtl.dictateSendDataFailureCnt > 3) {
					pClient->DictateRunCtl.dictateSendDataFailureCnt = 0;
					pClient->DictateRunCtl.dictateEvent = STOP_MODE;
				}
			}
			else {
				pClient->DictateRunCtl.dictateEvent = SEND_DATA;
			}
			return;
		}
		
		if ((NBIOT_Neul_NBxx_QuerySendMessageCOAPPayload(pClient) == NBIOT_OK) && 
		    (NBIOT_Neul_NBxx_QueryReadMessageCOAPPayload(pClient) == NBIOT_OK)) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEvent = SEND_DATA;
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("Coap NQMGS NQMGR Ok");
#endif
		}
		else {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->DictateRunCtl.dictateSendDataFailureCnt++;
				if (pClient->DictateRunCtl.dictateSendDataFailureCnt > 3) {
					pClient->DictateRunCtl.dictateSendDataFailureCnt = 0;
					pClient->DictateRunCtl.dictateEvent = STOP_MODE;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->DictateRunCtl.dictateEvent = SEND_DATA;
			}
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("Coap NQMGS NQMGR Fail");
#endif
			return;
		}
		
		SendSentNum = pClient->Parameter.coapSendMessage.sent;
		
		/* 发送负载数据 */
		if (NBIOT_Neul_NBxx_SendCOAPPayload(pClient) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEvent = SEND_DATA;
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("Coap Send Payload Ok");
#endif
		}
		else {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->DictateRunCtl.dictateSendDataFailureCnt++;
				if (pClient->DictateRunCtl.dictateSendDataFailureCnt > 3) {
					pClient->DictateRunCtl.dictateSendDataFailureCnt = 0;
					pClient->DictateRunCtl.dictateEvent = STOP_MODE;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->DictateRunCtl.dictateEvent = SEND_DATA;
			}
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("Coap Send Payload Fail");
#endif
			return;
		}
		
		if (NBIOT_Neul_NBxx_QuerySendMessageCOAPPayload(pClient) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEvent = SEND_DATA;
		}
		else {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->DictateRunCtl.dictateSendDataFailureCnt++;
				if (pClient->DictateRunCtl.dictateSendDataFailureCnt > 3) {
					pClient->DictateRunCtl.dictateSendDataFailureCnt = 0;
					pClient->DictateRunCtl.dictateEvent = STOP_MODE;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->DictateRunCtl.dictateEvent = SEND_DATA;
			}
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
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = RECV_DATA;
			pClient->DictateRunCtl.dictateSendDataFailureCnt = 0;
		}
	}
	/* No packets need to be sent */
	else {
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = EXECUT_DOWNLINK_DATA;
		pClient->DictateRunCtl.dictateSendDataFailureCnt = 0;
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
	
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (NBIOT_Neul_NBxx_QueryReadMessageCOAPPayload(pClient) == NBIOT_OK) {
		/* Dictate execute is Success */
		pClient->DictateRunCtl.dictateEvent = RECV_DATA;
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
			pClient->DictateRunCtl.dictateEvent = RECV_DATA;
		}
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
				if (COAPFeedBackFlag == false) {
					COAPFeedBackFlag = true;
					NET_Coap_Message_SendDataOffSet();
				}
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = SEND_DATA;
				pClient->DictateRunCtl.dictateRecvDataFailureCnt = 0;
				NET_COAP_NBIOT_Listen_Enable_EnterIdleMode(pClient);
				/* End ConnectTime */
				TCFG_SystemData.CoapConnectTime = TCFG_EEPROM_GetCoapConnectTime() + Stm32_EventRunningTime_EndMS(&pClient->ConnectTimeMS) / 1000;
				TCFG_EEPROM_SetCoapConnectTime(TCFG_SystemData.CoapConnectTime);
				/* Start IdleTime */
				Stm32_EventRunningTime_StartMS(&pClient->IdleTimeMS);
#ifdef COAP_DEBUG_LOG_RF_PRINT
				Radio_Trf_Debug_Printf_Level2("Coap Recv Feedback Ok");
#endif
			}
			else {
				/* Not Feedback */
				NET_Coap_Message_RecvDataEnqueue(pClient->Recvbuf, pClient->Recvlen);
#ifdef COAP_DEBUG_LOG_RF_PRINT
				Radio_Trf_Debug_Printf_Level2("Coap Recv Data Ok");
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
			Radio_Trf_Debug_Printf_Level2("Coap Wait Recv Feedback");
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
	int SendSentNum = 0;
	char* RANormal	= "0x0100";
	char* RAIdle	= "0x0101";
	char* RAState	= RAIdle;
	
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	/* Data packets need to be sent*/
	if (NET_Coap_Message_SendDataDequeue(pClient->Sendbuf, (unsigned short *)&pClient->Sendlen) == true) {
		/* End IdleTime */
		TCFG_SystemData.CoapIdleTime = TCFG_EEPROM_GetCoapIdleTime() + Stm32_EventRunningTime_EndMS(&pClient->IdleTimeMS) / 1000;
		TCFG_EEPROM_SetCoapIdleTime(TCFG_SystemData.CoapIdleTime);
		/* Start ConnectTime */
		Stm32_EventRunningTime_StartMS(&pClient->ConnectTimeMS);
		/* Connect Check */
		if (NBIOT_Neul_NBxx_CheckReadAttachOrDetach(pClient) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEvent = SEND_DATA_RA_NORMAL;
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("Coap CGATT %d Ok", pClient->Parameter.netstate);
#endif
		}
		else {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->DictateRunCtl.dictateSendDataRANormalFailureCnt++;
				if (pClient->DictateRunCtl.dictateSendDataRANormalFailureCnt > 3) {
					pClient->DictateRunCtl.dictateSendDataRANormalFailureCnt = 0;
					pClient->DictateRunCtl.dictateEvent = STOP_MODE;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->DictateRunCtl.dictateEvent = SEND_DATA_RA_NORMAL;
			}
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("Coap CGATT %d Fail", pClient->Parameter.netstate);
#endif
			return;
		}
		
		if (pClient->Parameter.netstate != Attach) {
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->DictateRunCtl.dictateSendDataRANormalFailureCnt++;
				if (pClient->DictateRunCtl.dictateSendDataRANormalFailureCnt > 3) {
					pClient->DictateRunCtl.dictateSendDataRANormalFailureCnt = 0;
					pClient->DictateRunCtl.dictateEvent = STOP_MODE;
				}
			}
			else {
				pClient->DictateRunCtl.dictateEvent = SEND_DATA_RA_NORMAL;
			}
			return;
		}
		
		if ((NBIOT_Neul_NBxx_QuerySendMessageCOAPPayload(pClient) == NBIOT_OK) && 
		    (NBIOT_Neul_NBxx_QueryReadMessageCOAPPayload(pClient) == NBIOT_OK)) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEvent = SEND_DATA_RA_NORMAL;
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("Coap NQMGS NQMGR Ok");
#endif
		}
		else {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->DictateRunCtl.dictateSendDataRANormalFailureCnt++;
				if (pClient->DictateRunCtl.dictateSendDataRANormalFailureCnt > 3) {
					pClient->DictateRunCtl.dictateSendDataRANormalFailureCnt = 0;
					pClient->DictateRunCtl.dictateEvent = STOP_MODE;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->DictateRunCtl.dictateEvent = SEND_DATA_RA_NORMAL;
			}
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("Coap NQMGS NQMGR Fail");
#endif
			return;
		}
		
		SendSentNum = pClient->Parameter.coapSendMessage.sent;
		
		if (NBIOT_COAP_RA_NORMAL_GET_STATE(pClient) == true) {
			RAState = RANormal;
		}
		else {
			RAState = RAIdle;
		}
		
		/* 发送负载数据 */
		if (NBIOT_Neul_NBxx_SendCOAPPayloadFlag(pClient, RAState) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEvent = SEND_DATA_RA_NORMAL;
			NBIOT_COAP_RA_NORMAL_SET_STATE(pClient, false);
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("Coap Send Payload Ok");
#endif
		}
		else {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->DictateRunCtl.dictateSendDataRANormalFailureCnt++;
				if (pClient->DictateRunCtl.dictateSendDataRANormalFailureCnt > 3) {
					pClient->DictateRunCtl.dictateSendDataRANormalFailureCnt = 0;
					pClient->DictateRunCtl.dictateEvent = STOP_MODE;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->DictateRunCtl.dictateEvent = SEND_DATA_RA_NORMAL;
			}
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("Coap Send Payload Fail");
#endif
			return;
		}
		
		if (NBIOT_Neul_NBxx_QuerySendMessageCOAPPayload(pClient) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEvent = SEND_DATA_RA_NORMAL;
		}
		else {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->DictateRunCtl.dictateSendDataRANormalFailureCnt++;
				if (pClient->DictateRunCtl.dictateSendDataRANormalFailureCnt > 3) {
					pClient->DictateRunCtl.dictateSendDataRANormalFailureCnt = 0;
					pClient->DictateRunCtl.dictateEvent = STOP_MODE;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->DictateRunCtl.dictateEvent = SEND_DATA_RA_NORMAL;
			}
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
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = RECV_DATA_RA_NORMAL;
			pClient->DictateRunCtl.dictateSendDataRANormalFailureCnt = 0;
		}
	}
	/* No packets need to be sent */
	else {
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = EXECUT_DOWNLINK_DATA;
		pClient->DictateRunCtl.dictateSendDataRANormalFailureCnt = 0;
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
	COAP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (NBIOT_Neul_NBxx_CheckReadCONDataStatus(pClient) == NBIOT_OK) {
		/* Dictate execute is Success */
		pClient->DictateRunCtl.dictateEvent = RECV_DATA_RA_NORMAL;
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
			pClient->DictateRunCtl.dictateEvent = RECV_DATA_RA_NORMAL;
		}
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
			}
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
				Radio_Trf_Debug_Printf_Level2("Coap Recv Data Ok");
#endif
			}
		}
		
		NET_Coap_Message_SendDataOffSet();
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = SEND_DATA_RA_NORMAL;
		pClient->DictateRunCtl.dictateRecvDataRANormalFailureCnt = 0;
		NET_COAP_NBIOT_Listen_Enable_EnterIdleMode(pClient);
		/* End ConnectTime */
		TCFG_SystemData.CoapConnectTime = TCFG_EEPROM_GetCoapConnectTime() + Stm32_EventRunningTime_EndMS(&pClient->ConnectTimeMS) / 1000;
		TCFG_EEPROM_SetCoapConnectTime(TCFG_SystemData.CoapConnectTime);
		/* Start IdleTime */
		Stm32_EventRunningTime_StartMS(&pClient->IdleTimeMS);
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("Coap Send Success");
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
			Radio_Trf_Debug_Printf_Level2("Coap Wait Send Success");
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
	u16 recvBufOffset = 0;
	u16 recvMagicNum = 0;
	u8 ret = NETIP_OK;
	
	if (NET_Coap_Message_RecvDataDequeue(pClient->Recvbuf, (unsigned short*)&pClient->Recvlen) == true) {
		pClient->Recvbuf[pClient->Recvlen] = '\0';
		for (int i = 0; i < pClient->Recvlen; i++) {
			if ((pClient->Recvbuf[i] == 'T') && (pClient->Recvbuf[i+1] == 'C') && (pClient->Recvbuf[i+2] == 'L') && (pClient->Recvbuf[i+3] == 'D')) {
				recvBufOffset = i;
			}
		}
		
		if (recvBufOffset != 0) {
			/* Find "TCLD" */
			if (pClient->Recvbuf[recvBufOffset + TCLOD_MSGVERSION_OFFSET] == TCLOD_MSGVERSION) {
				if (pClient->Recvbuf[recvBufOffset + TCLOD_MSGID_OFFSET] == TCLOD_CONFIG_SET) {
					BEEP_CtrlRepeat_Extend(1, 300, 0);
					TCFG_EEPROM_SetNBCmdCnt(1 + TCFG_EEPROM_GetNBCmdCnt());
					
					/* 工作模式配置指令 */
					if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "Workmode") != NULL) {
						u16 mode;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "{(Workmode):{(val):%hu,(Magic):%hu}}", &mode, &recvMagicNum);
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
						__NOP();
					}
					/* 传感器灵敏度配置指令 */
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "Sense") != NULL) {
						u16 sense;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "{(Sense):{(val):%hu,(Magic):%hu}}", &sense, &recvMagicNum);
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
						__NOP();
					}
					/* 无线心跳间隔时间配置指令 */
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "RFHeart") != NULL) {
						u16 rfheart;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "{(RFHeart):{(val):%hu,(Magic):%hu}}", &rfheart, &recvMagicNum);
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
						__NOP();
					}
					/* 初始化传感器指令 */
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "Background") != NULL) {
						u16 backgroundval;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "{(Background):{(val):%hu,(Magic):%hu}}", &backgroundval, &recvMagicNum);
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
						__NOP();
					}
					/* Reboot */
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "Reboot") != NULL) {
						BEEP_CtrlRepeat_Extend(2, 500, 250);
						Stm32_System_Software_Reboot();
						__NOP();
					}
					/* NewSn */
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "Newsn") != NULL) {
						u32 newsnval;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "{(Newsn):{(val):%08x,(Magic):%hu}}", &newsnval, &recvMagicNum);
						if (recvMagicNum == TCLOD_MAGIC_NUM) {
							TCFG_EEPROM_Set_MAC_SN(newsnval);
						}
						else {
							ret = NETIP_UNKNOWNERROR;
						}
						__NOP();
					}
					/* CDP Server */
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
							NETCoapNeedSendCode.DynamicInfo = 1;
							NET_Coap_Message_RecvDataOffSet();
							NETCoapNeedSendCode.ResponseInfoErrcode = ret;
							NETCoapNeedSendCode.ResponseInfo = 1;
							NET_NBIOT_Initialization();
							return;
						}
						else {
							ret = NETIP_UNKNOWNERROR;
						}
						__NOP();
					}
					/* Active */
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "Active") != NULL) {
						u16 activeval;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "{(Active):{(val):%hu,(Magic):%hu}}", &activeval, &recvMagicNum);
						if (recvMagicNum == TCLOD_MAGIC_NUM) {
							TCFG_EEPROM_SetActiveDevice(activeval);
							if (activeval) {
								DeviceActivedMode = true;
								BEEP_CtrlRepeat_Extend(5,30,70);
							}
							else {
								DeviceActivedMode = false;
								BEEP_CtrlRepeat_Extend(1,500,0);
							}
						}
						else {
							ret = NETIP_UNKNOWNERROR;
						}
						__NOP();
					}
					/* RadarDbg */
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "RadarDbg") != NULL) {
						u16 radarDbgval;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "{(RadarDbg):{(val):%hu,(Magic):%hu}}", &radarDbgval, &recvMagicNum);
						if (recvMagicNum == TCLOD_MAGIC_NUM) {
							TCFG_EEPROM_SetRadarDbgMode(radarDbgval);
							TCFG_SystemData.RadarDbgMode = TCFG_EEPROM_GetRadarDbgMode();
							NETCoapNeedSendCode.DynamicInfo = 1;
						}
						else {
							ret = NETIP_UNKNOWNERROR;
						}
						__NOP();
					}
					/* MagMod */
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "MagMod") != NULL) {
						u16 magmodval;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "{(MagMod):{(val):%hu,(Magic):%hu}}", &magmodval, &recvMagicNum);
						if (recvMagicNum == TCLOD_MAGIC_NUM) {
							TCFG_EEPROM_SetMagMode(magmodval);
							talgo_set_magmod(magmodval);
						}
						else {
							ret = NETIP_UNKNOWNERROR;
						}
						__NOP();
					}
					/* NbHeart */
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "NbHeart") != NULL) {
						u16 nbheartval;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "{(NbHeart):{(val):%hu,(Magic):%hu}}", &nbheartval, &recvMagicNum);
						if (recvMagicNum == TCLOD_MAGIC_NUM) {
							TCFG_EEPROM_SetNbiotHeart(nbheartval);
							TCFG_SystemData.NBIotHeart = TCFG_EEPROM_GetNbiotHeart();
						}
						else {
							ret = NETIP_UNKNOWNERROR;
						}
						__NOP();
					}
					/* InitRadar */
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "InitRadar") != NULL) {
						u32 i32, j32, k32, m32;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, \
							"{(InitRadar):{(v23456):%u,(v7890a):%u,(vbcdef):%u,(vg):%u,(Magic):%hu}}", &i32, &j32, &k32, &m32, &recvMagicNum);
						if (recvMagicNum == TCLOD_MAGIC_NUM) {
							Radar_InitBG_Cmd(i32, j32, k32, m32);
							NETCoapNeedSendCode.RadarInfo = 1;
						}
						else {
							ret = NETIP_UNKNOWNERROR;
						}
						__NOP();
					}
					/* InitMag */
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
						__NOP();
					}
					/* DisRange */
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "DisRange") != NULL) {
						u16 disrangeval;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "{(DisRange):{(val):%hu,(Magic):%hu}}", &disrangeval, &recvMagicNum);
						if (recvMagicNum == TCLOD_MAGIC_NUM) {
							tradar_set_distance_range(disrangeval + 4);
							TCFG_EEPROM_SetRadarRange(disrangeval);
							TCFG_SystemData.RadarRange = TCFG_EEPROM_GetRadarRange();
						}
						else {
							ret = NETIP_UNKNOWNERROR;
						}
						__NOP();
					}
					/* CarInDelay */
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "InDelay") != NULL) {
						u16 indelayval;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "{(InDelay):{(val):%hu,(Magic):%hu}}", &indelayval, &recvMagicNum);
						if (recvMagicNum == TCLOD_MAGIC_NUM) {
							TCFG_EEPROM_SetCarInDelay(indelayval);
							TCFG_SystemData.CarInDelay = TCFG_EEPROM_GetCarInDelay();
						}
						else {
							ret = NETIP_UNKNOWNERROR;
						}
						__NOP();
					}
					/* RATime */
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "RATime") != NULL) {
						u16 ratime;
						sscanf((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "{(RATime):{(val):%hu,(Magic):%hu}}", &ratime, &recvMagicNum);
						if (recvMagicNum == TCLOD_MAGIC_NUM) {
							TCFG_EEPROM_SetCoapRATimeHour(ratime);
							TCFG_SystemData.CoapRATimeHour = TCFG_EEPROM_GetCoapRATimeHour();
						}
						else {
							ret = NETIP_UNKNOWNERROR;
						}
						__NOP();
					}
					/* ...... */
				}
				else if (pClient->Recvbuf[recvBufOffset + TCLOD_MSGID_OFFSET] == TCLOD_CONFIG_GET) {
					BEEP_CtrlRepeat_Extend(2, 50, 50);
					/* Workinfo */
					if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "Workinfo") != NULL) {
						NETCoapNeedSendCode.WorkInfo = 1;
						__NOP();
					}
					/* BasicInfo */
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "BasicInfo") != NULL) {
						NETCoapNeedSendCode.BasicInfo = 1;
						__NOP();
					}
					/* DynamicInfo */
					else if (strstr((char *)pClient->Recvbuf + recvBufOffset + TCLOD_DATA_OFFSET, "DynamicInfo") != NULL) {
						NETCoapNeedSendCode.DynamicInfo = 1;
						__NOP();
					}
					/* ...... */
				}
			}
			else {
				ret = NETIP_NOTSUPPORT;
			}
			
			NETCoapNeedSendCode.ResponseInfoErrcode = ret;
			NETCoapNeedSendCode.ResponseInfo = 1;
		}
		else {
			/* Not Valid */
			ret = NETIP_NOTVALID;
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
	case ENTER_IDLE_MODE:
		NET_COAP_NBIOT_Listen_Event_EnterIdleMode(pClient);
		break;
	}
}

/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Listen_Enable_EnterIdleMode(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Listen_Enable_EnterIdleMode	: 使能(进入IDLE模式)监听
 @Input				pClient								: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_NBIOT_Listen_Enable_EnterIdleMode(NBIOT_ClientsTypeDef* pClient)
{
	Stm32_CalculagraphTypeDef listenRunTime;
	
	/* Listen Enable */
	if (pClient->ListenRunCtl.ListenEnterIdle.listenEnable == true) {
		pClient->ListenRunCtl.ListenEnterIdle.listenStatus = true;
		Stm32_Calculagraph_CountdownSec(&listenRunTime, pClient->ListenRunCtl.ListenEnterIdle.listenTimereachSec);
		pClient->ListenRunCtl.ListenEnterIdle.listenRunTime = listenRunTime;
	}
}

/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Listen_Event_EnterIdleMode(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Listen_Event_EnterIdleMode	: 事件(进入IDLE模式)监听
 @Input				pClient								: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_COAP_NBIOT_Listen_Event_EnterIdleMode(NBIOT_ClientsTypeDef* pClient)
{
	Stm32_CalculagraphTypeDef eventRunTime;
	
	if ((pClient->ListenRunCtl.ListenEnterIdle.listenEnable == true) && (pClient->ListenRunCtl.ListenEnterIdle.listenStatus == true)) {
		if (Stm32_Calculagraph_IsExpiredSec(&pClient->ListenRunCtl.ListenEnterIdle.listenRunTime) == true) {
			
			/* It is the first time to execute */
			if (pClient->ListenRunCtl.ListenEnterIdle.EventCtl.eventEnable != true) {
				pClient->ListenRunCtl.ListenEnterIdle.EventCtl.eventEnable = true;
				pClient->ListenRunCtl.ListenEnterIdle.EventCtl.eventTimeoutSec = 30;
				Stm32_Calculagraph_CountdownSec(&eventRunTime, pClient->ListenRunCtl.ListenEnterIdle.EventCtl.eventTimeoutSec);
				pClient->ListenRunCtl.ListenEnterIdle.EventCtl.eventRunTime = eventRunTime;
			}
			
			if (NBIOT_Neul_NBxx_CheckReadSignalConnectionStatus(pClient) == NBIOT_OK) {
				/* Dictate execute is Success */
				pClient->ListenRunCtl.ListenEnterIdle.EventCtl.eventEnable = false;
				pClient->ListenRunCtl.listenEvent = ENTER_IDLE_MODE;
				pClient->ListenRunCtl.ListenEnterIdle.EventCtl.eventFailureCnt = 0;
			}
			else {
				/* Dictate execute is Fail */
				if (Stm32_Calculagraph_IsExpiredSec(&pClient->ListenRunCtl.ListenEnterIdle.EventCtl.eventRunTime) == true) {
					/* Dictate TimeOut */
					pClient->ListenRunCtl.ListenEnterIdle.EventCtl.eventEnable = false;
					pClient->ListenRunCtl.listenEvent = ENTER_IDLE_MODE;
					pClient->DictateRunCtl.dictateEnable = false;
					pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
					pClient->ListenRunCtl.ListenEnterIdle.EventCtl.eventFailureCnt++;
					if (pClient->ListenRunCtl.ListenEnterIdle.EventCtl.eventFailureCnt > 3) {
						pClient->ListenRunCtl.ListenEnterIdle.EventCtl.eventFailureCnt = 0;
						pClient->DictateRunCtl.dictateEvent = STOP_MODE;
					}
				}
				else {
					/* Dictate isn't TimeOut */
					pClient->ListenRunCtl.listenEvent = ENTER_IDLE_MODE;
				}
				return;
			}
			
			if (pClient->Parameter.connectedstate == IdleMode) {
				/* Entered Idle Mode */
				pClient->ListenRunCtl.ListenEnterIdle.listenEnable = false;
				pClient->ListenRunCtl.ListenEnterIdle.listenStatus = false;
				pClient->ListenRunCtl.listenEvent = ENTER_IDLE_MODE;
#ifdef COAP_DEBUG_LOG_RF_PRINT
				Radio_Trf_Debug_Printf_Level2("NB Enter IDLE Mode");
#endif
			}
			else {
				/* Not Entered Idle Mode */
				pClient->ListenRunCtl.ListenEnterIdle.listenEnable = false;
				pClient->ListenRunCtl.ListenEnterIdle.listenStatus = false;
				pClient->ListenRunCtl.listenEvent = ENTER_IDLE_MODE;
				NETCoapNeedSendCode.WorkInfo = 1;
#ifdef COAP_DEBUG_LOG_RF_PRINT
				Radio_Trf_Debug_Printf_Level2("NB Not Enter IDLE Mode");
#endif
			}
		}
	}
	
	pClient->DictateRunCtl.dictateEnable = false;
	pClient->DictateRunCtl.dictateEvent = NBCOAP_SENDMODE_TYPE;
}

/********************************************** END OF FLEE **********************************************/
