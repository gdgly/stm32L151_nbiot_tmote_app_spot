/**
  *********************************************************************************************************
  * @file    pcpconfig.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-07-16
  * @brief   PCP	: 设备和中国电信物联网开放平台之间应用层升级协议
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "pcpconfig.h"
#include "pcptransport.h"
#include "pcpcrccheck.h"
#include "pcpsock.h"

unsigned char PCP_SendBuf[PCP_BUFFER_SIZE];
unsigned char PCP_RecvBuf[PCP_BUFFER_SIZE];
unsigned char PCP_DataStack[PCP_DATASTACK_SIZE];

/**********************************************************************************************************
 @Function			void PCP_Client_Init(PCP_ClientsTypeDef* pClient, PCP_CoAPNetTransportTypeDef* NetSock, NET_NBIOT_ClientsTypeDef* NetNbiotStack)
 @Description			PCP_Client_Init						: 初始化PCP客户端
 @Input				pClient								: PCP客户端实例
					NetSock								: PCP CoAP 协议栈
					NetNbiotStack							: NET NBIOT 协议栈
 @Return				void
**********************************************************************************************************/
void PCP_Client_Init(PCP_ClientsTypeDef* pClient, PCP_CoAPNetTransportTypeDef* NetSock, NET_NBIOT_ClientsTypeDef* NetNbiotStack)
{
	pClient->Sendbuf									= PCP_SendBuf;
	pClient->Recvbuf									= PCP_RecvBuf;
	pClient->Sendbuf_size								= sizeof(PCP_SendBuf);
	pClient->Recvbuf_size								= sizeof(PCP_RecvBuf);
	pClient->Sendlen									= 0;
	pClient->Recvlen									= 0;
	pClient->DataProcessStack							= PCP_DataStack;
	pClient->DataProcessStack_size						= sizeof(PCP_DataStack);
	
	pClient->Command_Timeout_Sec							= PCP_COMMAND_TIMEOUT_SEC;
	pClient->Command_Failure_Cnt							= PCP_COMMAND_FAILURE_CNT;
	
	
	pClient->CoAPStack									= NetSock;
	pClient->NetNbiotStack								= NetNbiotStack;
}

/********************************************** END OF FLEE **********************************************/
