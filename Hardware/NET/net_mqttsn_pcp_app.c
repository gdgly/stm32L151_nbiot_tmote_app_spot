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
		
		break;
	
	case MQTTSN_PCP_EVENT_INITIALIZED:
		NET_MqttSN_PCP_NBIOT_Event_Initialized(pClient);
		break;
	
	case MQTTSN_PCP_EVENT_READY:
		NET_MqttSN_PCP_NBIOT_Event_Ready(pClient);
		break;
	
	case MQTTSN_PCP_EVENT_FRAME_RECV:
		
		break;
	
	case MQTTSN_PCP_EVENT_FRAME_SEND:
		
		break;
	
	case MQTTSN_PCP_EVENT_EXECUTE:
		NET_MqttSN_PCP_NBIOT_Event_Execute(pClient);
		break;
	
	case MQTTSN_PCP_EVENT_ACTIVEUPLOAD:
		
		break;
	}
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






















































/********************************************** END OF FLEE **********************************************/
