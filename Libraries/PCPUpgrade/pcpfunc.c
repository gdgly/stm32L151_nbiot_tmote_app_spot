/**
  *********************************************************************************************************
  * @file    pcpfunc.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-07-19
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "pcpfunc.h"
#include "platform_config.h"
#include "platform_map.h"
#include "stm32l1xx_config.h"
#include "delay.h"
#include "usart.h"
#include "radio_rf_app.h"

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef PCP_Func_FrameCheck(unsigned char* DataFrame, short DataLength)
 @Description			PCP_Func_FrameCheck					: PCP数据帧检查
 @Input				DataFrame							: PCP数据
					DataLength						: PCP数据长度
 @Return				void
**********************************************************************************************************/
PCP_StatusTypeDef PCP_Func_FrameCheck(unsigned char* DataFrame, short DataLength)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	PCP_MessageDataTypeDef* PCPMessage = (PCP_MessageDataTypeDef*)DataFrame;
	unsigned short PCPReadCRCCheckCode;
	
	if (DataLength < 8) {
		PCPStatus = PCP_Frame_Format_Error;
		goto exit;
	}
	
	PCPReadCRCCheckCode = PCPMessage->CRCCheckCode;
	PCPMessage->CRCCheckCode = 0;
	
	if (PCPSock_ntohs(PCPReadCRCCheckCode) != PCPCrcCheck_getCrcCheckCode(DataFrame, DataLength)) {
		PCPMessage->CRCCheckCode = PCPReadCRCCheckCode;
		PCPStatus = PCP_Frame_CheckCode_Error;
		goto exit;
	}
	else {
		PCPMessage->CRCCheckCode = PCPReadCRCCheckCode;
		PCPStatus = PCP_OK;
	}
	
exit:
	return PCPStatus;
}


/**********************************************************************************************************
 @Function			PCP_StatusTypeDef PCP_Func_SelectMessageExecuteCmd(PCP_ClientsTypeDef* pClient)
 @Description			PCP_Func_SelectMessageExecuteCmd		: PCP判断不同消息码处理不同命令
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
PCP_StatusTypeDef PCP_Func_SelectMessageExecuteCmd(PCP_ClientsTypeDef* pClient)
{
	PCP_StatusTypeDef PCPStatus = PCP_MessageType_Error;
	PCP_MessageDataTypeDef* PCPMessageRecv = (PCP_MessageDataTypeDef*)pClient->Recvbuf;
	
	pClient->Parameter.ProtocolType = PCPMessageRecv->ProtocolType;
	pClient->Parameter.MessageType = PCPMessageRecv->MessageType;
	
	/* 查询设备版本 */
	if (pClient->Parameter.MessageType == PCP_QueryDeviceVersion) {
		PCPStatus = PCP_Func_AckQueryDeviceVersion(pClient);
#ifdef PCP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("PCP AckQDevVer %d", PCPStatus);
#endif
		goto exit;
	}
	
	/* 新版本通知 */
	if (pClient->Parameter.MessageType == PCP_NewVersionNotice) {
		PCPStatus = PCP_Func_AckNewVersionNotice(pClient);
#ifdef PCP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("PCP AckNVerNotice %d", PCPStatus);
		Radio_Trf_Debug_Printf_Level2("PlatVer %s", pClient->Parameter.PlatformSoftVersion);
		Radio_Trf_Debug_Printf_Level2("SliceSize %d", pClient->Parameter.UpgradePackSliceSize);
		Radio_Trf_Debug_Printf_Level2("SliceNum %d", pClient->Parameter.UpgradePackSliceNum);
		Radio_Trf_Debug_Printf_Level2("CheckCode %d", pClient->Parameter.UpgradePackCheckCode);
#endif
		goto exit;
	}
	
	/* 请求升级包应答 */
	if (pClient->Parameter.MessageType == PCP_RequestUpgradePackage) {
		PCPStatus = PCP_Func_AckRequestUpgradePackage(pClient);
#ifdef PCP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("PCP AckRequestUpgrade %d", PCPStatus);
		Radio_Trf_Debug_Printf_Level2("SliceIndex %d", pClient->Parameter.UpgradePackSliceIndex);
#endif
		goto exit;
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
exit:
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef PCP_Func_AckQueryDeviceVersion(PCP_ClientsTypeDef* pClient)
 @Description			PCP_Func_AckQueryDeviceVersion		: PCP查询设备版本应答
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
PCP_StatusTypeDef PCP_Func_AckQueryDeviceVersion(PCP_ClientsTypeDef* pClient)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	PCP_MessageDataTypeDef* PCPMessageProcess = (PCP_MessageDataTypeDef*)pClient->DataProcessStack;
	PCP_AckQueryDeviceVersionTypeDef* PCPAckQueryDeviceVersion = (PCP_AckQueryDeviceVersionTypeDef*)PCPMessageProcess->pPacketData;
	
	memset((void*)pClient->DataProcessStack, 0x0, pClient->DataProcessStack_size);
	
	/* 写入查询设备版本应答 */
	PCPMessageProcess->StartX = PCPSock_htons(PCP_START_X);
	PCPMessageProcess->ProtocolType = PCP_PROTOCOL_TYPE;
	PCPMessageProcess->MessageType = PCP_QueryDeviceVersion;
	PCPMessageProcess->CRCCheckCode = 0x0000;
	PCPMessageProcess->PacketDataLength = PCPSock_htons(sizeof(PCP_AckQueryDeviceVersionTypeDef));
	
	PCPAckQueryDeviceVersion->ResultCode = PCP_ExecuteSuccess;
	memcpy(PCPAckQueryDeviceVersion->DeviceSoftVersion, pClient->UpgradeExecution.DeviceSoftVersion, sizeof(PCPAckQueryDeviceVersion->DeviceSoftVersion));
	
	PCPMessageProcess->CRCCheckCode = PCPSock_htons(PCPCrcCheck_getCrcCheckCode(pClient->DataProcessStack, sizeof(PCP_MessageDataTypeDef) + sizeof(PCP_AckQueryDeviceVersionTypeDef) - 1));
	
	NET_PCP_Message_SendDataEnqueue(pClient->DataProcessStack, sizeof(PCP_MessageDataTypeDef) + sizeof(PCP_AckQueryDeviceVersionTypeDef) - 1);
	NET_PCP_Message_RecvDataOffSet();
	
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef PCP_Func_AckNewVersionNotice(PCP_ClientsTypeDef* pClient)
 @Description			PCP_Func_AckNewVersionNotice			: PCP新版本通知应答
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
PCP_StatusTypeDef PCP_Func_AckNewVersionNotice(PCP_ClientsTypeDef* pClient)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	PCP_MessageDataTypeDef* PCPMessageRecv = (PCP_MessageDataTypeDef*)pClient->Recvbuf;
	PCP_RckNewVersionNoticeTypeDef* PCPRckNewVersionNotice = (PCP_RckNewVersionNoticeTypeDef*)PCPMessageRecv->pPacketData;
	PCP_MessageDataTypeDef* PCPMessageProcess = (PCP_MessageDataTypeDef*)pClient->DataProcessStack;
	
	memset((void*)pClient->DataProcessStack, 0x0, pClient->DataProcessStack_size);
	
	/* 获取新版本通知数据 */
	memcpy(pClient->Parameter.PlatformSoftVersion, PCPRckNewVersionNotice->PlatformSoftVersion, sizeof(pClient->Parameter.PlatformSoftVersion));
	pClient->Parameter.UpgradePackSliceSize = PCPSock_ntohs(PCPRckNewVersionNotice->UpgradePackSliceSize);
	pClient->Parameter.UpgradePackSliceNum = PCPSock_ntohs(PCPRckNewVersionNotice->UpgradePackSliceNum);
	pClient->Parameter.UpgradePackCheckCode = PCPSock_ntohs(PCPRckNewVersionNotice->UpgradePackCheckCode);
	
	/* 参数写入升级运行管理器 */
	pClient->UpgradeExecution.upgradeStatus = PCP_UPGRADE_DOWNLOAD;
	memcpy(pClient->UpgradeExecution.PlatformSoftVersion, pClient->Parameter.PlatformSoftVersion, sizeof(pClient->UpgradeExecution.PlatformSoftVersion));
	pClient->UpgradeExecution.PackSliceIndex = 0;
	pClient->UpgradeExecution.PackSliceSize = pClient->Parameter.UpgradePackSliceSize;
	pClient->UpgradeExecution.PackSliceNum = pClient->Parameter.UpgradePackSliceNum;
	pClient->UpgradeExecution.PackCheckCode = pClient->Parameter.UpgradePackCheckCode;
	
	/* 写入新版本通知应答 */
	PCPMessageProcess->StartX = PCPSock_htons(PCP_START_X);
	PCPMessageProcess->ProtocolType = PCP_PROTOCOL_TYPE;
	PCPMessageProcess->MessageType = PCP_NewVersionNotice;
	PCPMessageProcess->CRCCheckCode = 0x0000;
	PCPMessageProcess->PacketDataLength = PCPSock_htons(1);
	PCPMessageProcess->pPacketData[0] = PCP_ExecuteSuccess;
	
	PCPMessageProcess->CRCCheckCode = PCPSock_htons(PCPCrcCheck_getCrcCheckCode(pClient->DataProcessStack, sizeof(PCP_MessageDataTypeDef)));
	
	NET_PCP_Message_SendDataEnqueue(pClient->DataProcessStack, sizeof(PCP_MessageDataTypeDef));
	NET_PCP_Message_RecvDataOffSet();
	
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef PCP_Func_AckRequestUpgradePackage(PCP_ClientsTypeDef* pClient)
 @Description			PCP_Func_AckRequestUpgradePackage		: PCP请求升级包应答
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
PCP_StatusTypeDef PCP_Func_AckRequestUpgradePackage(PCP_ClientsTypeDef* pClient)
{
	static u8 UpgradePackError = 0;
	
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	PCP_MessageDataTypeDef* PCPMessageRecv = (PCP_MessageDataTypeDef*)pClient->Recvbuf;
	PCP_AckRequestUpgradePackageTypeDef* PCPAckRequestUpgradePackage = (PCP_AckRequestUpgradePackageTypeDef*)PCPMessageRecv->pPacketData;
	
	if (PCPAckRequestUpgradePackage->ResultCode != PCP_ExecuteSuccess) {
		NET_PCP_Message_RecvDataOffSet();
		PCPStatus = PCP_UpgradePack_Error;
		UpgradePackError++;
		if (UpgradePackError >= 3) {
			pClient->UpgradeExecution.upgradeStatus = PCP_UPGRADE_FAILED;
		}
		goto exit;
	}
	
	/* 获取请求升级包响应数据 */
	pClient->Parameter.UpgradePackSliceIndex = PCPSock_ntohs(PCPAckRequestUpgradePackage->UpgradePackSliceIndex);
	
	/* 升级包下载处理回调 */
	PCP_UpgradeDataDownload_Callback(pClient, PCPSock_ntohs(PCPAckRequestUpgradePackage->UpgradePackSliceIndex), \
									  PCPAckRequestUpgradePackage->pUpgradeData, \
									  PCPMessageRecv->PacketDataLength - (sizeof(PCP_AckRequestUpgradePackageTypeDef) - 1));
	
	NET_PCP_Message_RecvDataOffSet();
	pClient->UpgradeExecution.PackSliceIndex = pClient->Parameter.UpgradePackSliceIndex + 1;
	/* 判断升级包是否下载完成 */
	if (pClient->UpgradeExecution.PackSliceIndex >= pClient->UpgradeExecution.PackSliceNum) {
		pClient->UpgradeExecution.upgradeStatus = PCP_UPGRADE_ASSEMBLE;
	}
	
exit:
	return PCPStatus;
}


/**********************************************************************************************************
 @Function			PCP_StatusTypeDef PCP_Func_SelectUpgradeStatusExecuteCmd(PCP_ClientsTypeDef* pClient)
 @Description			PCP_Func_SelectUpgradeStatusExecuteCmd	: PCP判断不同主动上传数据码处理不同命令
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
PCP_StatusTypeDef PCP_Func_SelectUpgradeStatusExecuteCmd(PCP_ClientsTypeDef* pClient)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	
	/* 请求升级包 */
	if (pClient->UpgradeExecution.upgradeStatus == PCP_UPGRADE_DOWNLOAD) {
		PCPStatus = PCP_Func_RequestUpgradePackage(pClient);
#ifdef PCP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("PCP RequestUpgradePackage %d", pClient->UpgradeExecution.PackSliceIndex);
#endif
		goto exit;
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
exit:
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef PCP_Func_RequestUpgradePackage(PCP_ClientsTypeDef* pClient)
 @Description			PCP_Func_RequestUpgradePackage		: PCP请求升级包
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
PCP_StatusTypeDef PCP_Func_RequestUpgradePackage(PCP_ClientsTypeDef* pClient)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	PCP_MessageDataTypeDef* PCPMessageProcess = (PCP_MessageDataTypeDef*)pClient->DataProcessStack;
	PCP_RequestUpgradePackageTypeDef* PCPRequestUpgradePackage = (PCP_RequestUpgradePackageTypeDef*)PCPMessageProcess->pPacketData;
	
	memset((void*)pClient->DataProcessStack, 0x0, pClient->DataProcessStack_size);
	
	/* 写入请求升级包 */
	PCPMessageProcess->StartX = PCPSock_htons(PCP_START_X);
	PCPMessageProcess->ProtocolType = PCP_PROTOCOL_TYPE;
	PCPMessageProcess->MessageType = PCP_RequestUpgradePackage;
	PCPMessageProcess->CRCCheckCode = 0x0000;
	PCPMessageProcess->PacketDataLength = PCPSock_htons(sizeof(PCP_RequestUpgradePackageTypeDef));
	
	memcpy(PCPRequestUpgradePackage->PlatformSoftVersion, pClient->UpgradeExecution.PlatformSoftVersion, sizeof(PCPRequestUpgradePackage->PlatformSoftVersion));
	PCPRequestUpgradePackage->UpgradePackSliceIndex = PCPSock_htons(pClient->UpgradeExecution.PackSliceIndex);
	
	PCPMessageProcess->CRCCheckCode = PCPSock_htons(PCPCrcCheck_getCrcCheckCode(pClient->DataProcessStack, sizeof(PCP_MessageDataTypeDef) + sizeof(PCP_RequestUpgradePackageTypeDef) - 1));
	
	NET_PCP_Message_SendDataEnqueue(pClient->DataProcessStack, sizeof(PCP_MessageDataTypeDef) + sizeof(PCP_RequestUpgradePackageTypeDef) - 1);
	
	return PCPStatus;
}












































/********************************************** END OF FLEE **********************************************/
