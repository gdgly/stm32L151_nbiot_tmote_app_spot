/**
  *********************************************************************************************************
  * @file    mqttpcpconfig.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-09-25
  * @brief   PCP	: 设备和私有PCP协议平台之间应用层升级协议
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "mqttpcpconfig.h"
#include "mqttpcpfunc.h"
#include "mqttpcpupgrade.h"
#include "mqttpcptransport.h"
#include "mqttpcpcrccheck.h"
#include "mqttpcpsock.h"
#include "platform_config.h"
#include "platform_map.h"
#include "hal_spiflash.h"
#include "hal_rtc.h"
#include "hal_beep.h"
#include "delay.h"
#include "usart.h"
#include "radio_rf_app.h"

unsigned char MqttSNPCP_SendBuf[MQTTSN_PCP_BUFFER_SIZE];
unsigned char MqttSNPCP_RecvBuf[MQTTSN_PCP_BUFFER_SIZE];
unsigned char MqttSNPCP_DataStack[MQTTSN_PCP_DATASTACK_SIZE];

/**********************************************************************************************************
 @Function			void MqttPCP_Client_Init(MqttSNPCP_ClientsTypeDef* pClient, MqttSNPCP_MqttNetTransportTypeDef* NetSock, NET_NBIOT_ClientsTypeDef* NetNbiotStack)
 @Description			MqttPCP_Client_Init						: 初始化PCP客户端
 @Input				pClient								: PCP客户端实例
					NetSock								: PCP MqttSN 协议栈
					NetNbiotStack							: NET NBIOT 协议栈
 @Return				void
**********************************************************************************************************/
void MqttPCP_Client_Init(MqttSNPCP_ClientsTypeDef* pClient, MqttSNPCP_MqttNetTransportTypeDef* NetSock, NET_NBIOT_ClientsTypeDef* NetNbiotStack)
{
	pClient->Sendbuf										= MqttSNPCP_SendBuf;
	pClient->Recvbuf										= MqttSNPCP_RecvBuf;
	pClient->Sendbuf_size									= sizeof(MqttSNPCP_SendBuf);
	pClient->Recvbuf_size									= sizeof(MqttSNPCP_RecvBuf);
	pClient->Sendlen										= 0;
	pClient->Recvlen										= 0;
	pClient->DataProcessStack								= MqttSNPCP_DataStack;
	pClient->DataProcessStack_size							= sizeof(MqttSNPCP_DataStack);
	
	pClient->Command_Timeout_Sec								= MQTTSN_PCP_COMMAND_TIMEOUT_SEC;
	pClient->Command_Failure_Cnt								= MQTTSN_PCP_COMMAND_FAILURE_CNT;
	
	pClient->DictateRunCtl.dictateEnable						= false;
	pClient->DictateRunCtl.dictateTimeoutSec					= 0;
	pClient->DictateRunCtl.dictateInitializedFailureCnt			= 0;
	pClient->DictateRunCtl.dictateReadyFailureCnt				= 0;
	pClient->DictateRunCtl.dictateRecvFailureCnt					= 0;
	pClient->DictateRunCtl.dictateSendFailureCnt					= 0;
	pClient->DictateRunCtl.dictateExecuteFailureCnt				= 0;
	pClient->DictateRunCtl.dictateActiveUploadFailureCnt			= 0;
	pClient->DictateRunCtl.dictateUpgradeQueryVersionCnt			= 0;
	pClient->DictateRunCtl.dictateUpgradeDownloadCnt				= 0;
	pClient->DictateRunCtl.dictateUpgradeAssembleCnt				= 0;
	pClient->DictateRunCtl.dictateUpgradeInstallCnt				= 0;
	pClient->DictateRunCtl.dictateEvent						= MQTTSN_PCP_EVENT_INITIALIZED;
	
	pClient->UpgradeExecution.upgradeStatus						= MQTTSN_PCP_UPGRADE_STANDBY;
	sprintf((char*)pClient->UpgradeExecution.DeviceSoftVersion, "V%d.%d", TCFG_Utility_Get_Major_Softnumber(), TCFG_Utility_Get_Sub_Softnumber());
	sprintf((char*)pClient->UpgradeExecution.PlatformSoftVersion, "V%d.%d", TCFG_Utility_Get_Major_Softnumber(), TCFG_Utility_Get_Sub_Softnumber());
	pClient->UpgradeExecution.PackSliceIndex					= 0;
	pClient->UpgradeExecution.PackSliceSize						= 0;
	pClient->UpgradeExecution.PackLastSliceSize					= 0;
	pClient->UpgradeExecution.PackSliceNum						= 0;
	pClient->UpgradeExecution.PackCheckCode						= 0;
	
	pClient->MqttSNStack									= NetSock;
	pClient->NetNbiotStack									= NetNbiotStack;
}

/**********************************************************************************************************
 @Function			MqttSNPCP_ResultCodeTypeDef MqttPCP_UpgradeDataNewVersionNotice_Callback(MqttSNPCP_ClientsTypeDef* pClient)
 @Description			MqttPCP_UpgradeDataNewVersionNotice_Callback	: PCP新版本通知处理回调函数
 @Input				pClient								: PCP客户端实例
 @Return				void
**********************************************************************************************************/
MqttSNPCP_ResultCodeTypeDef MqttPCP_UpgradeDataNewVersionNotice_Callback(MqttSNPCP_ClientsTypeDef* pClient)
{
	MqttSNPCP_ResultCodeTypeDef PCPResultCodeStatus = MQTTSN_PCP_ExecuteSuccess;
	
#ifdef MQTTSN_PCP_DEBUG_LOG_RF_PRINT
	Radio_Trf_Debug_Printf_Level2("PlatSoftVer: %s", pClient->Parameter.PlatformSoftVersion);
	Radio_Trf_Debug_Printf_Level2("PackSliceSize: %d", pClient->Parameter.UpgradePackSliceSize);
	Radio_Trf_Debug_Printf_Level2("PackSliceNum: %d", pClient->Parameter.UpgradePackSliceNum);
	Radio_Trf_Debug_Printf_Level2("CheckCode: %X", MqttSNCalculateStringToHex(pClient->Parameter.UpgradePackCheckCode>>8, pClient->Parameter.UpgradePackCheckCode&0xFF));
#endif
	
	PCPResultCodeStatus = MqttPCP_Upgrade_NewVersionNotice(pClient);
	
	return PCPResultCodeStatus;
}

/**********************************************************************************************************
 @Function			MqttSNPCP_ResultCodeTypeDef MqttPCP_UpgradeDataDownload_Callback(MqttSNPCP_ClientsTypeDef* pClient, u16 SliceIndex, u8* UpgradeData, u16 UpgradeDataLength)
 @Description			MqttPCP_UpgradeDataDownload_Callback		: PCP升级包下载处理回调函数
 @Input				pClient								: PCP客户端实例
					SliceIndex							: PCP分片序号
					UpgradeData							: 升级包数据
					UpgradeDataLength						: 升级包长度
 @Return				void
**********************************************************************************************************/
MqttSNPCP_ResultCodeTypeDef MqttPCP_UpgradeDataDownload_Callback(MqttSNPCP_ClientsTypeDef* pClient, u16 SliceIndex, u8* UpgradeData, u16 UpgradeDataLength)
{
	MqttSNPCP_ResultCodeTypeDef PCPResultCodeStatus = MQTTSN_PCP_ExecuteSuccess;
	
#ifdef MQTTSN_PCP_DEBUG_LOG_RF_PRINT
	Radio_Trf_Debug_Printf_Level2("Down%d.%d: OK", SliceIndex, UpgradeDataLength);
#endif
	
	PCPResultCodeStatus = MqttPCP_Upgrade_DataDownload(pClient, SliceIndex, UpgradeData, UpgradeDataLength);
	
	return PCPResultCodeStatus;
}

/**********************************************************************************************************
 @Function			MqttSNPCP_ResultCodeTypeDef MqttPCP_UpgradeDataAssemble_Callback(MqttSNPCP_ClientsTypeDef* pClient)
 @Description			MqttPCP_UpgradeDataAssemble_Callback		: PCP升级包组装处理回调函数
 @Input				pClient								: PCP客户端实例
 @Return				void
**********************************************************************************************************/
MqttSNPCP_ResultCodeTypeDef MqttPCP_UpgradeDataAssemble_Callback(MqttSNPCP_ClientsTypeDef* pClient)
{
	MqttSNPCP_ResultCodeTypeDef PCPResultCodeStatus = MQTTSN_PCP_ExecuteSuccess;
	
#ifdef MQTTSN_PCP_DEBUG_LOG_RF_PRINT
	Radio_Trf_Debug_Printf_Level2("Download Over!!");
#endif
	
	PCPResultCodeStatus = MqttPCP_Upgrade_DataAssemble(pClient);
	
	return PCPResultCodeStatus;
}

/**********************************************************************************************************
 @Function			MqttSNPCP_ResultCodeTypeDef MqttPCP_UpgradeDataReportUpgrades_Callback(MqttSNPCP_ClientsTypeDef* pClient)
 @Description			MqttPCP_UpgradeDataReportUpgrades_Callback	: PCP上报升级结果处理回调函数
 @Input				pClient								: PCP客户端实例
 @Return				void
**********************************************************************************************************/
MqttSNPCP_ResultCodeTypeDef MqttPCP_UpgradeDataReportUpgrades_Callback(MqttSNPCP_ClientsTypeDef* pClient)
{
	MqttSNPCP_ResultCodeTypeDef PCPResultCodeStatus = MQTTSN_PCP_ExecuteSuccess;
	
#ifdef MQTTSN_PCP_DEBUG_LOG_RF_PRINT
	Radio_Trf_Debug_Printf_Level2("Upgrade Over!!");
#endif
	
	PCPResultCodeStatus = MqttPCP_Upgrade_AfterUpdata(pClient);
	
	return PCPResultCodeStatus;
}

/********************************************** END OF FLEE **********************************************/
