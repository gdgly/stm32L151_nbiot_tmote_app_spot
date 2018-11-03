/**
  *********************************************************************************************************
  * @file    net_mqttsn_pcp_app.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-09-26
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "net_mqttsn_pcp_app.h"
#include "platform_config.h"
#include "platform_map.h"
#include "string.h"
#include "radio_rf_app.h"

/**********************************************************************************************************
 @Function			void NET_MqttSN_PCP_APP_PollExecution(MqttSNPCP_ClientsTypeDef* pClient)
 @Description			NET_MqttSN_PCP_APP_PollExecution			: PCP逻辑处理
 @Input				pClient								: PCP客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MqttSN_PCP_APP_PollExecution(MqttSNPCP_ClientsTypeDef* pClient)
{
	switch (pClient->DictateRunCtl.dictateEvent)
	{
	case MQTTSN_PCP_EVENT_STOP:
		NET_MqttSN_PCP_NBIOT_Event_StopMode(pClient);
		break;
	
	case MQTTSN_PCP_EVENT_INITIALIZED:
		NET_MqttSN_PCP_NBIOT_Event_Initialized(pClient);
		break;
	
	case MQTTSN_PCP_EVENT_READY:
		NET_MqttSN_PCP_NBIOT_Event_Ready(pClient);
		break;
	
	case MQTTSN_PCP_EVENT_FRAME_RECV:
		NET_MqttSN_PCP_NBIOT_Event_Recv(pClient);
		break;
	
	case MQTTSN_PCP_EVENT_FRAME_SEND:
		NET_MqttSN_PCP_NBIOT_Event_Send(pClient);
		break;
	
	case MQTTSN_PCP_EVENT_EXECUTE:
		NET_MqttSN_PCP_NBIOT_Event_Execute(pClient);
		break;
	
	case MQTTSN_PCP_EVENT_ACTIVEUPLOAD:
		NET_MqttSN_PCP_NBIOT_Event_ActiveUpload(pClient);
		break;
	}
}

/**********************************************************************************************************
 @Function			static void MqttPCP_NBIOT_DictateEvent_SetTime(MqttSNPCP_ClientsTypeDef* pClient, unsigned int TimeoutSec)
 @Description			MqttPCP_NBIOT_DictateEvent_SetTime		: 事件运行控制器注入时间(内部使用)
 @Input				pClient							: PCP客户端实例
					TimeoutSec						: 注入超时时间
 @Return				void
 @attention			事件运行之前判断是否需要注入时间
**********************************************************************************************************/
static void MqttPCP_NBIOT_DictateEvent_SetTime(MqttSNPCP_ClientsTypeDef* pClient, unsigned int TimeoutSec)
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
 @Function			static void MqttPCP_NormalDictateEvent_SetTime(MqttSNPCP_ClientsTypeDef* pClient, Stm32_CalculagraphTypeDef* pTimer, unsigned int TimeoutSec)
 @Description			MqttPCP_NormalDictateEvent_SetTime		: 事件运行控制器注入时间(内部使用)
 @Input				pClient							: MQTTSN客户端实例
					TimeoutSec						: 注入超时时间
 @Return				void
 @attention			事件运行之前判断是否需要注入时间
**********************************************************************************************************/
static void MqttPCP_NormalDictateEvent_SetTime(MqttSNPCP_ClientsTypeDef* pClient, Stm32_CalculagraphTypeDef* pTimer, unsigned int TimeoutSec)
{
	Stm32_CalculagraphTypeDef dictateRunTime;
	
	pClient->MqttSNStack->MqttSNStack->DictateRunCtl.dictateTimeoutSec = TimeoutSec;
	Stm32_Calculagraph_CountdownSec(&dictateRunTime, pClient->MqttSNStack->MqttSNStack->DictateRunCtl.dictateTimeoutSec);
	*pTimer = dictateRunTime;
}

/**********************************************************************************************************
 @Function			MqttSNPCP_StatusTypeDef NET_MqttSN_PCP_NBIOT_Event_StopMode(MqttSNPCP_ClientsTypeDef* pClient)
 @Description			NET_MqttSN_PCP_NBIOT_Event_StopMode		: 停止模式
 @Input				pClient								: PCP客户端实例
 @Return				void
**********************************************************************************************************/
MqttSNPCP_StatusTypeDef NET_MqttSN_PCP_NBIOT_Event_StopMode(MqttSNPCP_ClientsTypeDef* pClient)
{
	MqttSNPCP_StatusTypeDef PCPStatus = MQTTSN_PCP_OK;
	Stm32_CalculagraphTypeDef dictateRunTime;
	static unsigned char MqttSNPcpSendMessageIndex;
	static unsigned char MqttSNPcpRecvMessageIndex;
	
	/* It is the first time to execute */
	if (pClient->DictateRunCtl.dictateEnable != true) {
		pClient->DictateRunCtl.dictateEnable = true;
		pClient->DictateRunCtl.dictateTimeoutSec = 1800;
		Stm32_Calculagraph_CountdownSec(&dictateRunTime, pClient->DictateRunCtl.dictateTimeoutSec);
		pClient->DictateRunCtl.dictateRunTime = dictateRunTime;
		pClient->UpgradeExecution.upgradeStatus = MQTTSN_PCP_UPGRADE_FAILED;
		/* Send Message Index */
		MqttSNPcpSendMessageIndex = NET_MqttSN_PCP_Message_SendDataRear();
		/* Recv Message Index */
		MqttSNPcpRecvMessageIndex = NET_MqttSN_PCP_Message_RecvDataRear();
#ifdef MQTTSN_PCP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("PCP Enter Stop");
#endif
	}
	
	if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
		/* Dictate TimeOut */
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = MQTTSN_PCP_EVENT_READY;
		pClient->UpgradeExecution.upgradeStatus = MQTTSN_PCP_UPGRADE_STANDBY;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
	}
	else {
		/* Dictate isn't TimeOut */
		if ((NET_MqttSN_PCP_Message_SendDataRear() != MqttSNPcpSendMessageIndex) || (NET_MqttSN_PCP_Message_RecvDataRear() != MqttSNPcpRecvMessageIndex)) {
			/* Have new pcp message need to execute */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = MQTTSN_PCP_EVENT_READY;
			pClient->UpgradeExecution.upgradeStatus = MQTTSN_PCP_UPGRADE_STANDBY;
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
		}
		else {
			pClient->DictateRunCtl.dictateEvent = MQTTSN_PCP_EVENT_STOP;
			pClient->UpgradeExecution.upgradeStatus = MQTTSN_PCP_UPGRADE_STANDBY;
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
		}
	}
	
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			MqttSNPCP_StatusTypeDef NET_MqttSN_PCP_NBIOT_Event_Initialized(MqttSNPCP_ClientsTypeDef* pClient)
 @Description			NET_MqttSN_PCP_NBIOT_Event_Initialized		: 初始化PCP
 @Input				pClient								: PCP客户端实例
 @Return				void
**********************************************************************************************************/
MqttSNPCP_StatusTypeDef NET_MqttSN_PCP_NBIOT_Event_Initialized(MqttSNPCP_ClientsTypeDef* pClient)
{
	MqttSNPCP_StatusTypeDef PCPStatus = MQTTSN_PCP_OK;
	
	memset((void*)pClient->Sendbuf, 0x0, pClient->Sendbuf_size);
	memset((void*)pClient->Recvbuf, 0x0, pClient->Recvbuf_size);
	pClient->Sendlen = 0;
	pClient->Recvlen = 0;
	memset((void*)pClient->DataProcessStack, 0x0, pClient->DataProcessStack_size);
	
	pClient->DictateRunCtl.dictateEnable = false;
	pClient->DictateRunCtl.dictateEvent = MQTTSN_PCP_EVENT_READY;
	pClient->DictateRunCtl.dictateInitializedFailureCnt = 0;
	pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
	
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			MqttSNPCP_StatusTypeDef NET_MqttSN_PCP_NBIOT_Event_Ready(MqttSNPCP_ClientsTypeDef* pClient)
 @Description			NET_MqttSN_PCP_NBIOT_Event_Ready			: PCP准备中
 @Input				pClient								: PCP客户端实例
 @Return				void
**********************************************************************************************************/
MqttSNPCP_StatusTypeDef NET_MqttSN_PCP_NBIOT_Event_Ready(MqttSNPCP_ClientsTypeDef* pClient)
{
	MqttSNPCP_StatusTypeDef PCPStatus = MQTTSN_PCP_OK;
	
	/* 平台----->设备 */
	if (NET_MqttSN_PCP_Message_RecvDataisEmpty() != true) {									//接收到平台下发设备数据
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = MQTTSN_PCP_EVENT_EXECUTE;
		pClient->DictateRunCtl.dictateReadyFailureCnt = 0;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
	}
	/* 设备----->平台 */
	else if (NET_MqttSN_PCP_Message_SendDataisEmpty() != true) {								//需要发送数据到平台
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = MQTTSN_PCP_EVENT_FRAME_SEND;
		pClient->DictateRunCtl.dictateReadyFailureCnt = 0;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
	}
	/* 设备--ActiveUpload-->平台 */
	else if ((pClient->UpgradeExecution.upgradeStatus == MQTTSN_PCP_UPGRADE_QUERYVERSION) || 
		    (pClient->UpgradeExecution.upgradeStatus == MQTTSN_PCP_UPGRADE_DOWNLOAD) || 
		    (pClient->UpgradeExecution.upgradeStatus == MQTTSN_PCP_UPGRADE_ASSEMBLE) || 
		    (pClient->UpgradeExecution.upgradeStatus == MQTTSN_PCP_UPGRADE_INSTALL)) {				//需要主动发送数据到平台
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = MQTTSN_PCP_EVENT_ACTIVEUPLOAD;
		pClient->DictateRunCtl.dictateReadyFailureCnt = 0;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
	}
	/* 平台--x-->设备 */
	else {																			//等待数据交互
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = MQTTSN_PCP_EVENT_READY;
		pClient->DictateRunCtl.dictateReadyFailureCnt = 0;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
	}
	
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			MqttSNPCP_StatusTypeDef NET_MqttSN_PCP_NBIOT_Event_Recv(MqttSNPCP_ClientsTypeDef* pClient)
 @Description			NET_MqttSN_PCP_NBIOT_Event_Recv		: PCP接收数据
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
MqttSNPCP_StatusTypeDef NET_MqttSN_PCP_NBIOT_Event_Recv(MqttSNPCP_ClientsTypeDef* pClient)
{
	MqttSNPCP_StatusTypeDef PCPStatus = MQTTSN_PCP_OK;
	
	pClient->MqttSNStack->Read(pClient->MqttSNStack, (char *)pClient->Recvbuf, (u16 *)&pClient->Recvlen);
	
	pClient->DictateRunCtl.dictateEnable = false;
	pClient->DictateRunCtl.dictateEvent = MQTTSN_PCP_EVENT_READY;
	pClient->DictateRunCtl.dictateRecvFailureCnt = 0;
	pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_PCP;
	
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			MqttSNPCP_StatusTypeDef NET_MqttSN_PCP_NBIOT_Event_Send(MqttSNPCP_ClientsTypeDef* pClient)
 @Description			NET_MqttSN_PCP_NBIOT_Event_Send		: PCP发送数据
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
MqttSNPCP_StatusTypeDef NET_MqttSN_PCP_NBIOT_Event_Send(MqttSNPCP_ClientsTypeDef* pClient)
{
	MqttSNPCP_StatusTypeDef PCPStatus = MQTTSN_PCP_OK;
	
	MqttPCP_NBIOT_DictateEvent_SetTime(pClient, 60);
	
	/* Data packets need to be sent*/
	if (NET_MqttSN_PCP_Message_SendDataDequeue(pClient->Sendbuf, (unsigned short *)&pClient->Sendlen) == true) {
		/* 发送负载数据 */
		if ((PCPStatus = pClient->MqttSNStack->Write(pClient->MqttSNStack, (char *)pClient->Sendbuf, pClient->Sendlen)) != PCP_OK) {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = MQTTSN_PCP_EVENT_READY;
				pClient->DictateRunCtl.dictateSendFailureCnt++;
				pClient->MqttSNStack->MqttSNStack->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_DISABLE
				pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
#endif
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_ENABLE
				pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_DNS;
#endif
				if (pClient->DictateRunCtl.dictateSendFailureCnt > 3) {
					pClient->DictateRunCtl.dictateSendFailureCnt = 0;
					pClient->DictateRunCtl.dictateEvent = MQTTSN_PCP_EVENT_STOP;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->DictateRunCtl.dictateEvent = MQTTSN_PCP_EVENT_FRAME_SEND;
				pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_PCP;
			}
#ifdef MQTTSN_PCP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("PCP Send Payload Fail ECde %d", PCPStatus);
#endif
			goto exit;
		}
		else {
			/* Send Data Success */
			NET_MqttSN_PCP_Message_SendDataOffSet();
			MqttPCP_NormalDictateEvent_SetTime(pClient, &pClient->MqttSNStack->MqttSNStack->ActiveTimer, TNET_MQTTSN_ACTIVE_DURATION);
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = MQTTSN_PCP_EVENT_FRAME_RECV;
			pClient->DictateRunCtl.dictateSendFailureCnt = 0;
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_PCP;
#ifdef MQTTSN_PCP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("PCP Send Payload Ok");
#endif
		}
	}
	/* No packets need to be sent */
	else {
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = MQTTSN_PCP_EVENT_READY;
		pClient->DictateRunCtl.dictateSendFailureCnt = 0;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_PCP;
	}
	
exit:
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			MqttSNPCP_StatusTypeDef NET_MqttSN_PCP_NBIOT_Event_Execute(MqttSNPCP_ClientsTypeDef* pClient)
 @Description			NET_PCP_NBIOT_Event_Execute			: PCP数据处理
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
MqttSNPCP_StatusTypeDef NET_MqttSN_PCP_NBIOT_Event_Execute(MqttSNPCP_ClientsTypeDef* pClient)
{
	MqttSNPCP_StatusTypeDef PCPStatus = MQTTSN_PCP_OK;
	
	/* 读取接收区数据 */
	if (NET_MqttSN_PCP_Message_RecvDataDequeue(pClient->Recvbuf, (unsigned short *)&pClient->Recvlen) != true) {
		PCPStatus = MQTTSN_PCP_Frame_None;
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = MQTTSN_PCP_EVENT_READY;
		pClient->DictateRunCtl.dictateExecuteFailureCnt = 0;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
		goto exit;
	}
	
	/* 检测数据帧并校验数据帧 */
	if ((PCPStatus = MqttPCP_Func_FrameCheck(pClient->Recvbuf, pClient->Recvlen)) != MQTTSN_PCP_OK) {
		NET_MqttSN_PCP_Message_RecvDataOffSet();
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = MQTTSN_PCP_EVENT_READY;
		pClient->DictateRunCtl.dictateExecuteFailureCnt = 0;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
		goto exit;
	}
	
	/* 判断不同消息码处理不同命令 */
	if ((PCPStatus = MqttPCP_Func_SelectMessageExecuteCmd(pClient)) != MQTTSN_PCP_OK) {
		NET_MqttSN_PCP_Message_RecvDataOffSet();
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = MQTTSN_PCP_EVENT_READY;
		pClient->DictateRunCtl.dictateExecuteFailureCnt = 0;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
		goto exit;
	}
	
	/* 命令处理完成 */
	pClient->DictateRunCtl.dictateEnable = false;
	pClient->DictateRunCtl.dictateEvent = MQTTSN_PCP_EVENT_READY;
	pClient->DictateRunCtl.dictateExecuteFailureCnt = 0;
	pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
	
exit:
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			MqttSNPCP_StatusTypeDef NET_MqttSN_PCP_NBIOT_Event_ActiveUpload(MqttSNPCP_ClientsTypeDef* pClient)
 @Description			NET_MqttSN_PCP_NBIOT_Event_ActiveUpload	: PCP主动上传数据
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
MqttSNPCP_StatusTypeDef NET_MqttSN_PCP_NBIOT_Event_ActiveUpload(MqttSNPCP_ClientsTypeDef* pClient)
{
	MqttSNPCP_StatusTypeDef PCPStatus = MQTTSN_PCP_OK;
	
	/* 判断不同主动上传数据码处理不同命令 */
	if ((PCPStatus = MqttPCP_Func_SelectUpgradeStatusExecuteCmd(pClient)) != MQTTSN_PCP_OK) {
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = MQTTSN_PCP_EVENT_READY;
		pClient->DictateRunCtl.dictateActiveUploadFailureCnt = 0;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
		goto exit;
	}
	
	/* 命令处理完成 */
	pClient->DictateRunCtl.dictateEnable = false;
	pClient->DictateRunCtl.dictateEvent = MQTTSN_PCP_EVENT_READY;
	pClient->DictateRunCtl.dictateActiveUploadFailureCnt = 0;
	pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
	
exit:
	return PCPStatus;
}

/********************************************** END OF FLEE **********************************************/
