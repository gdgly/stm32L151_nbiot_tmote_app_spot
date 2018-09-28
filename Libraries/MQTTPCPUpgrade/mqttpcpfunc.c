/**
  *********************************************************************************************************
  * @file    mqttpcpfunc.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-09-27
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "mqttpcpfunc.h"
#include "platform_config.h"
#include "platform_map.h"
#include "stm32l1xx_config.h"
#include "delay.h"
#include "usart.h"
#include "radio_rf_app.h"

/**********************************************************************************************************
 @Function			MqttSNPCP_StatusTypeDef MqttPCP_Func_FrameCheck(unsigned char* DataFrame, short DataLength)
 @Description			MqttPCP_Func_FrameCheck				: PCP数据帧检查
 @Input				DataFrame							: PCP数据
					DataLength						: PCP数据长度
 @Return				void
**********************************************************************************************************/
MqttSNPCP_StatusTypeDef MqttPCP_Func_FrameCheck(unsigned char* DataFrame, short DataLength)
{
	MqttSNPCP_StatusTypeDef PCPStatus = MQTTSN_PCP_OK;
	MqttSNPCP_MessageDataTypeDef* PCPMessage = (MqttSNPCP_MessageDataTypeDef*)DataFrame;
	unsigned short PCPReadCRCCheckCode;
	
	if (DataLength < 8) {
		PCPStatus = MQTTSN_PCP_Frame_Format_Error;
		goto exit;
	}
	
	PCPReadCRCCheckCode = PCPMessage->CRCCheckCode;
	PCPMessage->CRCCheckCode = 0;
	
	if (MqttPCPSock_ntohs(PCPReadCRCCheckCode) != MqttPCPCrcCheck_getCrcCheckCode(DataFrame, DataLength)) {
		PCPMessage->CRCCheckCode = PCPReadCRCCheckCode;
		PCPStatus = MQTTSN_PCP_Frame_CheckCode_Error;
		goto exit;
	}
	else {
		PCPMessage->CRCCheckCode = PCPReadCRCCheckCode;
		PCPStatus = MQTTSN_PCP_OK;
	}
	
exit:
	return PCPStatus;
}


/**********************************************************************************************************
 @Function			MqttSNPCP_StatusTypeDef MqttPCP_Func_SelectMessageExecuteCmd(MqttSNPCP_ClientsTypeDef* pClient)
 @Description			MqttPCP_Func_SelectMessageExecuteCmd	: PCP判断不同消息码处理不同命令
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
MqttSNPCP_StatusTypeDef MqttPCP_Func_SelectMessageExecuteCmd(MqttSNPCP_ClientsTypeDef* pClient)
{
	MqttSNPCP_StatusTypeDef PCPStatus = MQTTSN_PCP_MessageType_Error;
	MqttSNPCP_MessageDataTypeDef* PCPMessageRecv = (MqttSNPCP_MessageDataTypeDef*)pClient->Recvbuf;
	
	pClient->Parameter.ProtocolType = PCPMessageRecv->ProtocolType;
	pClient->Parameter.MessageType = PCPMessageRecv->MessageType;
	
	/* 查询设备版本 */
	if (pClient->Parameter.MessageType == MQTTSN_PCP_QueryDeviceVersion) {
		PCPStatus = MqttPCP_Func_AckQueryDeviceVersion(pClient);
#ifdef MQTTSN_PCP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("PCP RvQueryDevVer %s", pClient->UpgradeExecution.DeviceSoftVersion);
#endif
		goto exit;
	}
	
	/* 新版本通知 */
	if (pClient->Parameter.MessageType == MQTTSN_PCP_NewVersionNotice) {
		PCPStatus = MqttPCP_Func_AckNewVersionNotice(pClient);
#ifdef MQTTSN_PCP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("PCP RvNewVerNotice %s", pClient->Parameter.PlatformSoftVersion);
#endif
		goto exit;
	}
	
	/* 请求升级包应答 */
	if (pClient->Parameter.MessageType == MQTTSN_PCP_RequestUpgradePackage) {
		PCPStatus = MqttPCP_Func_AckRequestUpgradePackage(pClient);
#ifdef MQTTSN_PCP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("PCP RvRequestUpgradeAck %d", pClient->Parameter.UpgradePackSliceIndex);
#endif
		goto exit;
	}
	
	/* 上报升级包下载状态应答 */
	if (pClient->Parameter.MessageType == MQTTSN_PCP_ReportDownloadStatus) {
		PCPStatus = MqttPCP_Func_AckReportDownloadStatus(pClient);
#ifdef MQTTSN_PCP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("PCP RvReportDownloadStatusAck %d", PCPStatus);
#endif
		goto exit;
	}
	
	/* 执行升级 */
	if (pClient->Parameter.MessageType == MQTTSN_PCP_PerformUpgrade) {
		PCPStatus = MqttPCP_Func_AckPerformUpgrade(pClient);
#ifdef MQTTSN_PCP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("PCP RvPerformUpgrade %d", PCPStatus);
#endif
		goto exit;
	}
	
	/* 上报升级结果应答 */
	if (pClient->Parameter.MessageType == MQTTSN_PCP_ReportUpgrades) {
		PCPStatus = MqttPCP_Func_AckReportUpgrades(pClient);
#ifdef MQTTSN_PCP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("PCP RvReportUpgradesAck %d", PCPStatus);
#endif
		goto exit;
	}
	
exit:
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			MqttSNPCP_StatusTypeDef MqttPCP_Func_AckQueryDeviceVersion(MqttSNPCP_ClientsTypeDef* pClient)
 @Description			MqttPCP_Func_AckQueryDeviceVersion		: PCP查询设备版本应答
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
MqttSNPCP_StatusTypeDef MqttPCP_Func_AckQueryDeviceVersion(MqttSNPCP_ClientsTypeDef* pClient)
{
	MqttSNPCP_StatusTypeDef PCPStatus = MQTTSN_PCP_OK;
	MqttSNPCP_MessageDataTypeDef* PCPMessageProcess = (MqttSNPCP_MessageDataTypeDef*)pClient->DataProcessStack;
	MqttSNPCP_AckQueryDeviceVersionTypeDef* PCPAckQueryDeviceVersion = (MqttSNPCP_AckQueryDeviceVersionTypeDef*)PCPMessageProcess->pPacketData;
	
	memset((void*)pClient->DataProcessStack, 0x0, pClient->DataProcessStack_size);
	
	/* 写入查询设备版本应答 */
	PCPMessageProcess->StartX = MqttPCPSock_htons(MQTTSN_PCP_START_X);
	PCPMessageProcess->ProtocolType = MQTTSN_PCP_PROTOCOL_TYPE;
	PCPMessageProcess->MessageType = MQTTSN_PCP_QueryDeviceVersion;
	PCPMessageProcess->CRCCheckCode = 0x0000;
	PCPMessageProcess->PacketDataLength = MqttPCPSock_htons(sizeof(MqttSNPCP_AckQueryDeviceVersionTypeDef));
	
	PCPAckQueryDeviceVersion->ResultCode = MQTTSN_PCP_ExecuteSuccess;
	memcpy(PCPAckQueryDeviceVersion->DeviceSoftVersion, pClient->UpgradeExecution.DeviceSoftVersion, sizeof(PCPAckQueryDeviceVersion->DeviceSoftVersion));
	
	PCPMessageProcess->CRCCheckCode = \
	MqttPCPSock_htons(MqttPCPCrcCheck_getCrcCheckCode(pClient->DataProcessStack, sizeof(MqttSNPCP_MessageDataTypeDef) + sizeof(MqttSNPCP_AckQueryDeviceVersionTypeDef) - 1));
	
	NET_MqttSN_PCP_Message_SendDataEnqueue(pClient->DataProcessStack, sizeof(MqttSNPCP_MessageDataTypeDef) + sizeof(MqttSNPCP_AckQueryDeviceVersionTypeDef) - 1);
	NET_MqttSN_PCP_Message_RecvDataOffSet();
	
	pClient->UpgradeExecution.upgradeStatus = MQTTSN_PCP_UPGRADE_QUERYVERSION;
	
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			MqttSNPCP_StatusTypeDef MqttPCP_Func_AckNewVersionNotice(MqttSNPCP_ClientsTypeDef* pClient)
 @Description			MqttPCP_Func_AckNewVersionNotice		: PCP新版本通知应答
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
MqttSNPCP_StatusTypeDef MqttPCP_Func_AckNewVersionNotice(MqttSNPCP_ClientsTypeDef* pClient)
{
	MqttSNPCP_StatusTypeDef PCPStatus = MQTTSN_PCP_OK;
	MqttSNPCP_ResultCodeTypeDef PCPResultCodeStatus = MQTTSN_PCP_ExecuteSuccess;
	MqttSNPCP_MessageDataTypeDef* PCPMessageRecv = (MqttSNPCP_MessageDataTypeDef*)pClient->Recvbuf;
	MqttSNPCP_RckNewVersionNoticeTypeDef* PCPRckNewVersionNotice = (MqttSNPCP_RckNewVersionNoticeTypeDef*)PCPMessageRecv->pPacketData;
	MqttSNPCP_MessageDataTypeDef* PCPMessageProcess = (MqttSNPCP_MessageDataTypeDef*)pClient->DataProcessStack;
	
	pClient->DictateRunCtl.dictateUpgradeQueryVersionCnt = 0;
	
	/* 获取新版本通知数据 */
	memcpy(pClient->Parameter.PlatformSoftVersion, PCPRckNewVersionNotice->PlatformSoftVersion, sizeof(pClient->Parameter.PlatformSoftVersion));
	pClient->Parameter.UpgradePackSliceSize = MqttPCPSock_ntohs(PCPRckNewVersionNotice->UpgradePackSliceSize);
	pClient->Parameter.UpgradePackSliceNum = MqttPCPSock_ntohs(PCPRckNewVersionNotice->UpgradePackSliceNum);
	pClient->Parameter.UpgradePackCheckCode = MqttPCPSock_ntohs(PCPRckNewVersionNotice->UpgradePackCheckCode);
	
	/* 参数写入升级运行管理器 */
	memcpy(pClient->UpgradeExecution.PlatformSoftVersion, pClient->Parameter.PlatformSoftVersion, sizeof(pClient->UpgradeExecution.PlatformSoftVersion));
	pClient->UpgradeExecution.PackSliceIndex = 0;
	pClient->UpgradeExecution.PackSliceSize = pClient->Parameter.UpgradePackSliceSize;
	pClient->UpgradeExecution.PackSliceNum = pClient->Parameter.UpgradePackSliceNum;
	pClient->UpgradeExecution.PackCheckCode = pClient->Parameter.UpgradePackCheckCode;
	
	/* 新版本通知处理回调函数 */
	PCPResultCodeStatus = MqttPCP_UpgradeDataNewVersionNotice_Callback(pClient);
	if (PCPResultCodeStatus != MQTTSN_PCP_ExecuteSuccess) {
		pClient->UpgradeExecution.upgradeStatus = MQTTSN_PCP_UPGRADE_STANDBY;
	}
	else {
		pClient->UpgradeExecution.upgradeStatus = MQTTSN_PCP_UPGRADE_DOWNLOAD;
	}
	
	memset((void*)pClient->DataProcessStack, 0x0, pClient->DataProcessStack_size);
	
	/* 写入新版本通知应答 */
	PCPMessageProcess->StartX = MqttPCPSock_htons(MQTTSN_PCP_START_X);
	PCPMessageProcess->ProtocolType = MQTTSN_PCP_PROTOCOL_TYPE;
	PCPMessageProcess->MessageType = MQTTSN_PCP_NewVersionNotice;
	PCPMessageProcess->CRCCheckCode = 0x0000;
	PCPMessageProcess->PacketDataLength = MqttPCPSock_htons(1);
	PCPMessageProcess->pPacketData[0] = PCPResultCodeStatus;
	
	PCPMessageProcess->CRCCheckCode = MqttPCPSock_htons(MqttPCPCrcCheck_getCrcCheckCode(pClient->DataProcessStack, sizeof(MqttSNPCP_MessageDataTypeDef)));
	
	NET_MqttSN_PCP_Message_SendDataEnqueue(pClient->DataProcessStack, sizeof(MqttSNPCP_MessageDataTypeDef));
	NET_MqttSN_PCP_Message_RecvDataOffSet();
	
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			MqttSNPCP_StatusTypeDef MqttPCP_Func_AckRequestUpgradePackage(MqttSNPCP_ClientsTypeDef* pClient)
 @Description			MqttPCP_Func_AckRequestUpgradePackage	: PCP请求升级包应答
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
MqttSNPCP_StatusTypeDef MqttPCP_Func_AckRequestUpgradePackage(MqttSNPCP_ClientsTypeDef* pClient)
{
	MqttSNPCP_StatusTypeDef PCPStatus = MQTTSN_PCP_OK;
	MqttSNPCP_MessageDataTypeDef* PCPMessageRecv = (MqttSNPCP_MessageDataTypeDef*)pClient->Recvbuf;
	MqttSNPCP_AckRequestUpgradePackageTypeDef* PCPAckRequestUpgradePackage = (MqttSNPCP_AckRequestUpgradePackageTypeDef*)PCPMessageRecv->pPacketData;
	
	pClient->DictateRunCtl.dictateUpgradeDownloadCnt = 0;
	
	/* 判断请求升级包结果码 */
	if (PCPAckRequestUpgradePackage->ResultCode != MQTTSN_PCP_ExecuteSuccess) {
		NET_MqttSN_PCP_Message_RecvDataOffSet();
		PCPStatus = MQTTSN_PCP_UpgradePack_Error;
		pClient->UpgradeExecution.upgradeStatus = MQTTSN_PCP_UPGRADE_FAILED;
		goto exit;
	}
	
	/* 获取请求升级包响应数据 */
	pClient->Parameter.UpgradePackSliceIndex = MqttPCPSock_ntohs(PCPAckRequestUpgradePackage->UpgradePackSliceIndex);
	
	/* 升级包下载处理回调 */
	MqttPCP_UpgradeDataDownload_Callback(pClient, MqttPCPSock_ntohs(PCPAckRequestUpgradePackage->UpgradePackSliceIndex), \
										 PCPAckRequestUpgradePackage->pUpgradeData, \
										 MqttPCPSock_ntohs(PCPMessageRecv->PacketDataLength) - (sizeof(MqttSNPCP_AckRequestUpgradePackageTypeDef) - 1));
	
	NET_MqttSN_PCP_Message_RecvDataOffSet();
	
	/* 判断升级包是否下载完成 */
	if (pClient->UpgradeExecution.PackSliceIndex >= pClient->UpgradeExecution.PackSliceNum) {
		pClient->UpgradeExecution.upgradeStatus = MQTTSN_PCP_UPGRADE_ASSEMBLE;
	}
	
exit:
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			MqttSNPCP_StatusTypeDef MqttPCP_Func_AckReportDownloadStatus(MqttSNPCP_ClientsTypeDef* pClient)
 @Description			MqttPCP_Func_AckReportDownloadStatus	: PCP上报升级包下载状态应答
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
MqttSNPCP_StatusTypeDef MqttPCP_Func_AckReportDownloadStatus(MqttSNPCP_ClientsTypeDef* pClient)
{
	MqttSNPCP_StatusTypeDef PCPStatus = MQTTSN_PCP_OK;
	MqttSNPCP_MessageDataTypeDef* PCPMessageRecv = (MqttSNPCP_MessageDataTypeDef*)pClient->Recvbuf;
	
	if (PCPMessageRecv->pPacketData[0] != MQTTSN_PCP_ExecuteSuccess) {
		NET_MqttSN_PCP_Message_RecvDataOffSet();
		pClient->UpgradeExecution.upgradeStatus = MQTTSN_PCP_UPGRADE_FAILED;
		PCPStatus = MQTTSN_PCP_UpgradeDown_Error;
		goto exit;
	}
	
	NET_MqttSN_PCP_Message_RecvDataOffSet();
	
	if (pClient->UpgradeExecution.upgradeStatus != MQTTSN_PCP_UPGRADE_INSTALL) {
		pClient->UpgradeExecution.upgradeStatus = MQTTSN_PCP_UPGRADE_ASSEMBLE;
	}
	
exit:
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			MqttSNPCP_StatusTypeDef MqttPCP_Func_AckPerformUpgrade(MqttSNPCP_ClientsTypeDef* pClient)
 @Description			MqttPCP_Func_AckPerformUpgrade		: PCP执行升级
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
MqttSNPCP_StatusTypeDef MqttPCP_Func_AckPerformUpgrade(MqttSNPCP_ClientsTypeDef* pClient)
{
	MqttSNPCP_StatusTypeDef PCPStatus = MQTTSN_PCP_OK;
	MqttSNPCP_MessageDataTypeDef* PCPMessageProcess = (MqttSNPCP_MessageDataTypeDef*)pClient->DataProcessStack;
	
	memset((void*)pClient->DataProcessStack, 0x0, pClient->DataProcessStack_size);
	
	/* 写入执行升级应答 */
	PCPMessageProcess->StartX = MqttPCPSock_htons(MQTTSN_PCP_START_X);
	PCPMessageProcess->ProtocolType = MQTTSN_PCP_PROTOCOL_TYPE;
	PCPMessageProcess->MessageType = MQTTSN_PCP_PerformUpgrade;
	PCPMessageProcess->CRCCheckCode = 0x0000;
	PCPMessageProcess->PacketDataLength = MqttPCPSock_htons(1);
	PCPMessageProcess->pPacketData[0] = MQTTSN_PCP_ExecuteSuccess;
	
	PCPMessageProcess->CRCCheckCode = MqttPCPSock_htons(MqttPCPCrcCheck_getCrcCheckCode(pClient->DataProcessStack, sizeof(MqttSNPCP_MessageDataTypeDef)));
	
	NET_MqttSN_PCP_Message_SendDataEnqueue(pClient->DataProcessStack, sizeof(MqttSNPCP_MessageDataTypeDef));
	NET_MqttSN_PCP_Message_RecvDataOffSet();
	
	pClient->UpgradeExecution.upgradeStatus = MQTTSN_PCP_UPGRADE_INSTALL;
	pClient->DictateRunCtl.dictateUpgradeAssembleCnt = 0;
	
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef PCP_Func_AckReportUpgrades(PCP_ClientsTypeDef* pClient)
 @Description			MqttPCP_Func_AckReportUpgrades		: PCP上报升级结果应答
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
MqttSNPCP_StatusTypeDef MqttPCP_Func_AckReportUpgrades(MqttSNPCP_ClientsTypeDef* pClient)
{
	MqttSNPCP_StatusTypeDef PCPStatus = MQTTSN_PCP_OK;
	MqttSNPCP_MessageDataTypeDef* PCPMessageRecv = (MqttSNPCP_MessageDataTypeDef*)pClient->Recvbuf;
	
	if (PCPMessageRecv->pPacketData[0] != MQTTSN_PCP_ExecuteSuccess) {
		NET_MqttSN_PCP_Message_RecvDataOffSet();
		pClient->UpgradeExecution.upgradeStatus = MQTTSN_PCP_UPGRADE_FAILED;
		PCPStatus = MQTTSN_PCP_Upgrade_Error;
		goto exit;
	}
	
	pClient->DictateRunCtl.dictateUpgradeInstallCnt = 0;
	
	NET_MqttSN_PCP_Message_RecvDataOffSet();
	
	pClient->UpgradeExecution.upgradeStatus = MQTTSN_PCP_UPGRADE_STANDBY;
	
	MqttPCP_UpgradeDataReportUpgrades_Callback(pClient);
	
exit:
	return PCPStatus;
}


/**********************************************************************************************************
 @Function			MqttSNPCP_StatusTypeDef MqttPCP_Func_SelectUpgradeStatusExecuteCmd(MqttSNPCP_ClientsTypeDef* pClient)
 @Description			MqttPCP_Func_SelectUpgradeStatusExecuteCmd	: PCP判断不同主动上传数据码处理不同命令
 @Input				pClient								: PCP客户端实例
 @Return				void
**********************************************************************************************************/
MqttSNPCP_StatusTypeDef MqttPCP_Func_SelectUpgradeStatusExecuteCmd(MqttSNPCP_ClientsTypeDef* pClient)
{
	MqttSNPCP_StatusTypeDef PCPStatus = MQTTSN_PCP_OK;
	
	/* 查询设备版本应答包 */
	if (pClient->UpgradeExecution.upgradeStatus == MQTTSN_PCP_UPGRADE_QUERYVERSION) {
		PCPStatus = MqttPCP_Func_QueryDeviceVersion(pClient);
#ifdef MQTTSN_PCP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("PCP SdQueryDeviceVersion %s", pClient->UpgradeExecution.DeviceSoftVersion);
#endif
		goto exit;
	}
	
	/* 请求升级包 */
	if (pClient->UpgradeExecution.upgradeStatus == MQTTSN_PCP_UPGRADE_DOWNLOAD) {
		PCPStatus = MqttPCP_Func_RequestUpgradePackage(pClient);
#ifdef MQTTSN_PCP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("PCP SdRequestUpgradePackage %d", pClient->UpgradeExecution.PackSliceIndex);
#endif
		goto exit;
	}
	
	/* 上报升级包下载状态 */
	if (pClient->UpgradeExecution.upgradeStatus == MQTTSN_PCP_UPGRADE_ASSEMBLE) {
		PCPStatus = MqttPCP_Func_ReportDownloadStatus(pClient);
#ifdef MQTTSN_PCP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("PCP SdReportDownloadStatus");
#endif
		goto exit;
	}
	
	/* 上报升级结果 */
	if (pClient->UpgradeExecution.upgradeStatus == MQTTSN_PCP_UPGRADE_INSTALL) {
		PCPStatus = MqttPCP_Func_ReportUpgrades(pClient);
#ifdef MQTTSN_PCP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("PCP SdReportUpgrades");
#endif
		goto exit;
	}
	
exit:
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			MqttSNPCP_StatusTypeDef MqttPCP_Func_QueryDeviceVersion(MqttSNPCP_ClientsTypeDef* pClient)
 @Description			MqttPCP_Func_QueryDeviceVersion		: PCP查询设备版本应答包
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
MqttSNPCP_StatusTypeDef MqttPCP_Func_QueryDeviceVersion(MqttSNPCP_ClientsTypeDef* pClient)
{
	MqttSNPCP_StatusTypeDef PCPStatus = MQTTSN_PCP_OK;
	MqttSNPCP_MessageDataTypeDef* PCPMessageProcess = (MqttSNPCP_MessageDataTypeDef*)pClient->DataProcessStack;
	MqttSNPCP_AckQueryDeviceVersionTypeDef* PCPAckQueryDeviceVersion = (MqttSNPCP_AckQueryDeviceVersionTypeDef*)PCPMessageProcess->pPacketData;
	
	/* 查询设备版本应答包尝试次数 */
	pClient->DictateRunCtl.dictateUpgradeQueryVersionCnt++;
	if (pClient->DictateRunCtl.dictateUpgradeQueryVersionCnt > 2) {
		/* 确实没有需要升级包 */
		pClient->DictateRunCtl.dictateUpgradeQueryVersionCnt = 0;
		pClient->UpgradeExecution.upgradeStatus = MQTTSN_PCP_UPGRADE_STANDBY;
		goto exit;
	}
	
	memset((void*)pClient->DataProcessStack, 0x0, pClient->DataProcessStack_size);
	
	/* 写入查询设备版本应答 */
	PCPMessageProcess->StartX = MqttPCPSock_htons(MQTTSN_PCP_START_X);
	PCPMessageProcess->ProtocolType = MQTTSN_PCP_PROTOCOL_TYPE;
	PCPMessageProcess->MessageType = MQTTSN_PCP_QueryDeviceVersion;
	PCPMessageProcess->CRCCheckCode = 0x0000;
	PCPMessageProcess->PacketDataLength = MqttPCPSock_htons(sizeof(MqttSNPCP_AckQueryDeviceVersionTypeDef));
	
	PCPAckQueryDeviceVersion->ResultCode = MQTTSN_PCP_ExecuteSuccess;
	memcpy(PCPAckQueryDeviceVersion->DeviceSoftVersion, pClient->UpgradeExecution.DeviceSoftVersion, sizeof(PCPAckQueryDeviceVersion->DeviceSoftVersion));
	
	PCPMessageProcess->CRCCheckCode = \
	MqttPCPSock_htons(MqttPCPCrcCheck_getCrcCheckCode(pClient->DataProcessStack, sizeof(MqttSNPCP_MessageDataTypeDef) + sizeof(MqttSNPCP_AckQueryDeviceVersionTypeDef) - 1));
	
	NET_MqttSN_PCP_Message_SendDataEnqueue(pClient->DataProcessStack, sizeof(MqttSNPCP_MessageDataTypeDef) + sizeof(MqttSNPCP_AckQueryDeviceVersionTypeDef) - 1);
	
exit:
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			MqttSNPCP_StatusTypeDef MqttPCP_Func_RequestUpgradePackage(MqttSNPCP_ClientsTypeDef* pClient)
 @Description			MqttPCP_Func_RequestUpgradePackage		: PCP请求升级包
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
MqttSNPCP_StatusTypeDef MqttPCP_Func_RequestUpgradePackage(MqttSNPCP_ClientsTypeDef* pClient)
{
	MqttSNPCP_StatusTypeDef PCPStatus = MQTTSN_PCP_OK;
	MqttSNPCP_MessageDataTypeDef* PCPMessageProcess = (MqttSNPCP_MessageDataTypeDef*)pClient->DataProcessStack;
	MqttSNPCP_RequestUpgradePackageTypeDef* PCPRequestUpgradePackage = (MqttSNPCP_RequestUpgradePackageTypeDef*)PCPMessageProcess->pPacketData;
	
	/* 请求升级包尝试次数 */
	pClient->DictateRunCtl.dictateUpgradeDownloadCnt++;
	if (pClient->DictateRunCtl.dictateUpgradeDownloadCnt > 5) {
		pClient->DictateRunCtl.dictateUpgradeDownloadCnt = 0;
		pClient->UpgradeExecution.upgradeStatus = MQTTSN_PCP_UPGRADE_FAILED;
		goto exit;
	}
	
	memset((void*)pClient->DataProcessStack, 0x0, pClient->DataProcessStack_size);
	
	/* 写入请求升级包 */
	PCPMessageProcess->StartX = MqttPCPSock_htons(MQTTSN_PCP_START_X);
	PCPMessageProcess->ProtocolType = MQTTSN_PCP_PROTOCOL_TYPE;
	PCPMessageProcess->MessageType = MQTTSN_PCP_RequestUpgradePackage;
	PCPMessageProcess->CRCCheckCode = 0x0000;
	PCPMessageProcess->PacketDataLength = MqttPCPSock_htons(sizeof(MqttSNPCP_RequestUpgradePackageTypeDef));
	
	memcpy(PCPRequestUpgradePackage->PlatformSoftVersion, pClient->UpgradeExecution.PlatformSoftVersion, sizeof(PCPRequestUpgradePackage->PlatformSoftVersion));
	PCPRequestUpgradePackage->UpgradePackSliceIndex = MqttPCPSock_htons(pClient->UpgradeExecution.PackSliceIndex);
	
	PCPMessageProcess->CRCCheckCode = \
	MqttPCPSock_htons(MqttPCPCrcCheck_getCrcCheckCode(pClient->DataProcessStack, sizeof(MqttSNPCP_MessageDataTypeDef) + sizeof(MqttSNPCP_RequestUpgradePackageTypeDef) - 1));
	
	NET_MqttSN_PCP_Message_SendDataEnqueue(pClient->DataProcessStack, sizeof(MqttSNPCP_MessageDataTypeDef) + sizeof(MqttSNPCP_RequestUpgradePackageTypeDef) - 1);
	
exit:
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			MqttSNPCP_StatusTypeDef MqttPCP_Func_ReportDownloadStatus(MqttSNPCP_ClientsTypeDef* pClient)
 @Description			MqttPCP_Func_ReportDownloadStatus		: PCP上报升级包下载状态
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
MqttSNPCP_StatusTypeDef MqttPCP_Func_ReportDownloadStatus(MqttSNPCP_ClientsTypeDef* pClient)
{
	MqttSNPCP_StatusTypeDef PCPStatus = MQTTSN_PCP_OK;
	MqttSNPCP_ResultCodeTypeDef PCPResultCodeStatus = MQTTSN_PCP_ExecuteSuccess;
	MqttSNPCP_MessageDataTypeDef* PCPMessageProcess = (MqttSNPCP_MessageDataTypeDef*)pClient->DataProcessStack;
	
	/* 上报升级包下载状态尝试次数 */
	pClient->DictateRunCtl.dictateUpgradeAssembleCnt++;
	if (pClient->DictateRunCtl.dictateUpgradeAssembleCnt > 5) {
		pClient->DictateRunCtl.dictateUpgradeAssembleCnt = 0;
		pClient->UpgradeExecution.upgradeStatus = MQTTSN_PCP_UPGRADE_FAILED;
		goto exit;
	}
	
	/* 组装升级包处理回调 */
	PCPResultCodeStatus = MqttPCP_UpgradeDataAssemble_Callback(pClient);
	if (PCPResultCodeStatus != MQTTSN_PCP_ExecuteSuccess) {
		pClient->UpgradeExecution.upgradeStatus = MQTTSN_PCP_UPGRADE_STANDBY;
	}
	
	memset((void*)pClient->DataProcessStack, 0x0, pClient->DataProcessStack_size);
	
	/* 写入上报升级包下载状态 */
	PCPMessageProcess->StartX = MqttPCPSock_htons(MQTTSN_PCP_START_X);
	PCPMessageProcess->ProtocolType = MQTTSN_PCP_PROTOCOL_TYPE;
	PCPMessageProcess->MessageType = MQTTSN_PCP_ReportDownloadStatus;
	PCPMessageProcess->CRCCheckCode = 0x0000;
	PCPMessageProcess->PacketDataLength = MqttPCPSock_htons(1);
	PCPMessageProcess->pPacketData[0] = PCPResultCodeStatus;
	
	PCPMessageProcess->CRCCheckCode = MqttPCPSock_htons(MqttPCPCrcCheck_getCrcCheckCode(pClient->DataProcessStack, sizeof(MqttSNPCP_MessageDataTypeDef)));
	
	NET_MqttSN_PCP_Message_SendDataEnqueue(pClient->DataProcessStack, sizeof(MqttSNPCP_MessageDataTypeDef));
	
exit:
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			MqttSNPCP_StatusTypeDef MqttPCP_Func_ReportUpgrades(MqttSNPCP_ClientsTypeDef* pClient)
 @Description			MqttPCP_Func_ReportUpgrades			: PCP上报升级结果
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
MqttSNPCP_StatusTypeDef MqttPCP_Func_ReportUpgrades(MqttSNPCP_ClientsTypeDef* pClient)
{
	MqttSNPCP_StatusTypeDef PCPStatus = MQTTSN_PCP_OK;
	MqttSNPCP_MessageDataTypeDef* PCPMessageProcess = (MqttSNPCP_MessageDataTypeDef*)pClient->DataProcessStack;
	MqttSNPCP_ReportUpgradesTypeDef* PCPReportUpgrades = (MqttSNPCP_ReportUpgradesTypeDef*)PCPMessageProcess->pPacketData;
	
	/* 上报升级结果尝试次数 */
	pClient->DictateRunCtl.dictateUpgradeInstallCnt++;
	if (pClient->DictateRunCtl.dictateUpgradeInstallCnt > 5) {
		pClient->DictateRunCtl.dictateUpgradeInstallCnt = 0;
		pClient->UpgradeExecution.upgradeStatus = MQTTSN_PCP_UPGRADE_FAILED;
		goto exit;
	}
	
	memset((void*)pClient->DataProcessStack, 0x0, pClient->DataProcessStack_size);
	
	/* 写入上报升级结果 */
	PCPMessageProcess->StartX = MqttPCPSock_htons(MQTTSN_PCP_START_X);
	PCPMessageProcess->ProtocolType = MQTTSN_PCP_PROTOCOL_TYPE;
	PCPMessageProcess->MessageType = MQTTSN_PCP_ReportUpgrades;
	PCPMessageProcess->CRCCheckCode = 0x0000;
	PCPMessageProcess->PacketDataLength = MqttPCPSock_htons(sizeof(MqttSNPCP_ReportUpgradesTypeDef));
	
	PCPReportUpgrades->ResultCode = MQTTSN_PCP_ExecuteSuccess;
	memcpy(PCPReportUpgrades->DeviceSoftVersion, pClient->UpgradeExecution.PlatformSoftVersion, sizeof(PCPReportUpgrades->DeviceSoftVersion));
	
	PCPMessageProcess->CRCCheckCode = \
	MqttPCPSock_htons(MqttPCPCrcCheck_getCrcCheckCode(pClient->DataProcessStack, sizeof(MqttSNPCP_MessageDataTypeDef) + sizeof(MqttSNPCP_ReportUpgradesTypeDef) - 1));
	
	NET_MqttSN_PCP_Message_SendDataEnqueue(pClient->DataProcessStack, sizeof(MqttSNPCP_MessageDataTypeDef) + sizeof(MqttSNPCP_ReportUpgradesTypeDef) - 1);
	
exit:
	return PCPStatus;
}

/********************************************** END OF FLEE **********************************************/
