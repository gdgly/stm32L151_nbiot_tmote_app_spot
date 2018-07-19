/**
  *********************************************************************************************************
  * @file    net_pcp_app.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-07-18
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "net_pcp_app.h"
#include "platform_config.h"
#include "platform_map.h"
#include "string.h"
#include "radio_rf_app.h"

/**********************************************************************************************************
 @Function			void NET_PCP_APP_PollExecution(PCP_ClientsTypeDef* pClient)
 @Description			NET_PCP_APP_PollExecution			: PCP逻辑处理
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
void NET_PCP_APP_PollExecution(PCP_ClientsTypeDef* pClient)
{
	switch (pClient->DictateRunCtl.dictateEvent)
	{
	case PCP_EVENT_STOP:
		NET_PCP_NBIOT_Event_StopMode(pClient);
		break;
	
	case PCP_EVENT_INITIALIZED:
		NET_PCP_NBIOT_Event_Initialized(pClient);
		break;
	
	case PCP_EVENT_READY:
		NET_PCP_NBIOT_Event_Ready(pClient);
		break;
	
	case PCP_EVENT_FRAME_RECV:
		NET_PCP_NBIOT_Event_Recv(pClient);
		break;
	
	case PCP_EVENT_FRAME_SEND:
		
		break;
	
	case PCP_EVENT_EXECUTE:
		NET_PCP_NBIOT_Event_Execute(pClient);
		break;
	
	case PCP_EVENT_ACTIVEUPLOAD:
		
		break;
	}
}

/**********************************************************************************************************
 @Function			static void PCP_NBIOT_DictateEvent_SetTime(PCP_ClientsTypeDef* pClient, unsigned int TimeoutSec)
 @Description			PCP_NBIOT_DictateEvent_SetTime		: 事件运行控制器注入时间(内部使用)
 @Input				pClient							: PCP客户端实例
					TimeoutSec						: 注入超时时间
 @Return				void
 @attention			事件运行之前判断是否需要注入时间
**********************************************************************************************************/
static void PCP_NBIOT_DictateEvent_SetTime(PCP_ClientsTypeDef* pClient, unsigned int TimeoutSec)
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
 @Function			PCP_StatusTypeDef NET_PCP_NBIOT_Event_StopMode(PCP_ClientsTypeDef* pClient)
 @Description			NET_PCP_NBIOT_Event_StopMode			: 停止模式
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
PCP_StatusTypeDef NET_PCP_NBIOT_Event_StopMode(PCP_ClientsTypeDef* pClient)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	Stm32_CalculagraphTypeDef dictateRunTime;
	static unsigned char PcpSendMessageIndex;
	static unsigned char PcpRecvMessageIndex;
	
	/* It is the first time to execute */
	if (pClient->DictateRunCtl.dictateEnable != true) {
		pClient->DictateRunCtl.dictateEnable = true;
		pClient->DictateRunCtl.dictateTimeoutSec = 1800;
		Stm32_Calculagraph_CountdownSec(&dictateRunTime, pClient->DictateRunCtl.dictateTimeoutSec);
		pClient->DictateRunCtl.dictateRunTime = dictateRunTime;
		/* Send Message Index */
		PcpSendMessageIndex = NET_PCP_Message_SendDataRear();
		/* Recv Message Index */
		PcpRecvMessageIndex = NET_PCP_Message_RecvDataRear();
	}
	
	if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
		/* Dictate TimeOut */
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = PCP_EVENT_READY;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
	}
	else {
		/* Dictate isn't TimeOut */
		if ((NET_PCP_Message_SendDataRear() != PcpSendMessageIndex) || (NET_PCP_Message_RecvDataRear() != PcpRecvMessageIndex)) {
			/* Have new pcp message need to execute */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = PCP_EVENT_READY;
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
		}
		else {
			pClient->DictateRunCtl.dictateEvent = PCP_EVENT_STOP;
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
		}
	}
	
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef NET_PCP_NBIOT_Event_Initialized(PCP_ClientsTypeDef* pClient)
 @Description			NET_PCP_NBIOT_Event_Initialized		: 初始化PCP
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
PCP_StatusTypeDef NET_PCP_NBIOT_Event_Initialized(PCP_ClientsTypeDef* pClient)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	
	memset((void*)pClient->Sendbuf, 0x0, pClient->Sendbuf_size);
	memset((void*)pClient->Recvbuf, 0x0, pClient->Recvbuf_size);
	pClient->Sendlen = 0;
	pClient->Recvlen = 0;
	memset((void*)pClient->DataProcessStack, 0x0, pClient->DataProcessStack_size);
	
	pClient->DictateRunCtl.dictateEnable = false;
	pClient->DictateRunCtl.dictateEvent = PCP_EVENT_READY;
	pClient->DictateRunCtl.dictateInitializedFailureCnt = 0;
	pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
	
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef NET_PCP_NBIOT_Event_Ready(PCP_ClientsTypeDef* pClient)
 @Description			NET_PCP_NBIOT_Event_Ready			: PCP准备中
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
PCP_StatusTypeDef NET_PCP_NBIOT_Event_Ready(PCP_ClientsTypeDef* pClient)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	
	/* 平台----->设备 */
	if (NET_PCP_Message_RecvDataisEmpty() != true) {											//接收到平台下发设备数据
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = PCP_EVENT_FRAME_RECV;
		pClient->DictateRunCtl.dictateReadyFailureCnt = 0;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
	}
	/* 平台--x-->设备 */
	else {
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = PCP_EVENT_READY;
		pClient->DictateRunCtl.dictateReadyFailureCnt = 0;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
	}
	
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef NET_PCP_NBIOT_Event_Recv(PCP_ClientsTypeDef* pClient)
 @Description			NET_PCP_NBIOT_Event_Recv				: PCP接收数据
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
PCP_StatusTypeDef NET_PCP_NBIOT_Event_Recv(PCP_ClientsTypeDef* pClient)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	
	/* Data has been received */
	if (NET_PCP_Message_RecvDataisEmpty() != true) {
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = PCP_EVENT_EXECUTE;
		pClient->DictateRunCtl.dictateRecvFailureCnt = 0;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
	}
	/* Receiving data */
	else {
		PCP_NBIOT_DictateEvent_SetTime(pClient, 60);
		
		if (NBIOT_Neul_NBxx_CheckReadCONDataStatus(pClient->CoAPStack->NBIotStack) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEvent = PCP_EVENT_FRAME_RECV;
		}
		else {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = PCP_EVENT_READY;
				pClient->DictateRunCtl.dictateRecvFailureCnt++;
				pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_PCP;
				if (pClient->DictateRunCtl.dictateRecvFailureCnt > 3) {
					pClient->DictateRunCtl.dictateRecvFailureCnt = 0;
					pClient->DictateRunCtl.dictateEvent = PCP_EVENT_STOP;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->DictateRunCtl.dictateEvent = PCP_EVENT_FRAME_RECV;
				pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_PCP;
			}
			goto exit;
		}
		
		
		
		
		
		
		
		
		
		
	}
	
	
	
	
	
	
	
	
exit:
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef NET_PCP_NBIOT_Event_Execute(PCP_ClientsTypeDef* pClient)
 @Description			NET_PCP_NBIOT_Event_Execute			: PCP数据处理
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
PCP_StatusTypeDef NET_PCP_NBIOT_Event_Execute(PCP_ClientsTypeDef* pClient)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	
	if (NET_PCP_Message_RecvDataDequeue(pClient->Recvbuf, (unsigned short *)&pClient->Recvlen) != true) {
		PCPStatus = PCP_Frame_None;
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = PCP_EVENT_READY;
		pClient->DictateRunCtl.dictateExecuteFailureCnt = 0;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
		goto exit;
	}
	
	if ((PCPStatus = PCP_Func_FrameCheck(pClient->Recvbuf, pClient->Recvlen)) != PCP_OK) {
		NET_PCP_Message_RecvDataOffSet();
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = PCP_EVENT_READY;
		pClient->DictateRunCtl.dictateExecuteFailureCnt = 0;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
		goto exit;
	}
	
	
	
	
	
	
	
exit:
	return PCPStatus;
}













































/********************************************** END OF FLEE **********************************************/
