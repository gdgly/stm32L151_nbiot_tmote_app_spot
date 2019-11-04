/**
  *********************************************************************************************************
  * @file    ctwingconfig.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2019-10-14
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "main.h"
#include "ctwingconfig.h"
#include "ctwingfunc.h"
#include "ctwingtransport.h"
#include "ctwingaepcodes.h"
#include "platform_config.h"
#include "platform_map.h"
#include "string.h"

unsigned char CTWing_SendBuf[CTWING_BUFFER_SIZE];
unsigned char CTWing_RecvBuf[CTWING_BUFFER_SIZE];
unsigned char CTWing_DataStack[CTWING_DATASTACK_SIZE];
unsigned char CTWing_AepMallocStack[CTWING_DATASTACK_SIZE];

/**********************************************************************************************************
 @Function			void CTWing_Client_Init(CTWING_ClientsTypeDef* pClient, CTWING_LWM2MTransportTypeDef* NetSock, NET_NBIOT_ClientsTypeDef* NetNbiotStack)
 @Description			CTWing_Client_Init						: 初始化CTWing客户端
 @Input				pClient								: CTWing客户端实例
					NetSock								: CTWing Transport 协议栈
					NetNbiotStack							: NET NBIOT 协议栈
 @Return				void
**********************************************************************************************************/
void CTWing_Client_Init(CTWING_ClientsTypeDef* pClient, CTWING_LWM2MTransportTypeDef* NetSock, NET_NBIOT_ClientsTypeDef* NetNbiotStack)
{
	pClient->Sendbuf											= CTWing_SendBuf;
	pClient->Recvbuf											= CTWing_RecvBuf;
	pClient->Sendbuf_size										= sizeof(CTWing_SendBuf);
	pClient->Recvbuf_size										= sizeof(CTWing_RecvBuf);
	pClient->Sendlen											= 0;
	pClient->Recvlen											= 0;
	pClient->DataProcessStack									= CTWing_DataStack;
	pClient->DataProcessStack_size								= sizeof(CTWing_DataStack);
	pClient->AepMallocProcessStack								= CTWing_AepMallocStack;
	pClient->AepMallocProcessStack_size							= sizeof(CTWing_AepMallocStack);
	
	pClient->Command_Timeout_Sec									= CTWING_COMMAND_TIMEOUT_SEC;
	pClient->Command_Failure_Cnt									= CTWING_COMMAND_FAILURE_CNT;
	
	/* 事件运行监听器 */
#if NBCTWING_LISTEN_PARAMETER_TYPE == NBCTWING_LISTEN_PARAMETER_ENABLE
	pClient->ListenRunCtl.ListenEnterParameter.listenEnable			= false;
	pClient->ListenRunCtl.ListenEnterParameter.listenStatus			= false;
	pClient->ListenRunCtl.ListenEnterParameter.listenTimereachSec		= NBCTWING_LISTEN_ENTER_PARAMETER_SEC;
	pClient->ListenRunCtl.ListenEnterParameter.EventCtl.eventEnable		= false;
	pClient->ListenRunCtl.ListenEnterParameter.EventCtl.eventTimeoutSec	= 0;
	pClient->ListenRunCtl.ListenEnterParameter.EventCtl.eventFailureCnt	= 0;
#endif
	pClient->ListenRunCtl.listenEvent								= NBCTWING_LISTEN_DEFAULT_BOOTMODE;
	
	pClient->LWM2MStack											= NetSock;
	pClient->NetNbiotStack										= NetNbiotStack;
}

/********************************************** END OF FLEE **********************************************/
