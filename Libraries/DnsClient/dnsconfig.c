/**
  *********************************************************************************************************
  * @file    dnsconfig.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-05-15
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "dnsconfig.h"
#include "dnsfunc.h"
#include "dnstransport.h"

unsigned char DNS_SendBuf[DNS_BUFFER_SIZE];
unsigned char DNS_RecvBuf[DNS_BUFFER_SIZE];
unsigned char DNS_DataStack[DNS_DATASTACK_SIZE];

/**********************************************************************************************************
 @Function			void DNS_Client_Init(DNS_ClientsTypeDef* pClient, DNS_SocketNetTypeDef* NetSock, NET_NBIOT_ClientsTypeDef* NetNbiotStack)
 @Description			DNS_Client_Init						: 初始化DNS客户端
 @Input				pClient								: DNS客户端实例
					NetSock								: DNS Socket 协议栈
					NetNbiotStack							: NET NBIOT 协议栈
 @Return				void
**********************************************************************************************************/
void DNS_Client_Init(DNS_ClientsTypeDef* pClient, DNS_SocketNetTypeDef* NetSock, NET_NBIOT_ClientsTypeDef* NetNbiotStack)
{
	pClient->Sendbuf									= DNS_SendBuf;
	pClient->Recvbuf									= DNS_RecvBuf;
	pClient->Sendbuf_size								= sizeof(DNS_SendBuf);
	pClient->Recvbuf_size								= sizeof(DNS_RecvBuf);
	pClient->Sendlen									= 0;
	pClient->Recvlen									= 0;
	pClient->DataProcessStack							= DNS_DataStack;
	pClient->DataProcessStack_size						= sizeof(DNS_DataStack);
	
	pClient->Command_Timeout_Sec							= DNS_COMMAND_TIMEOUT_SEC;
	pClient->Command_Failure_Cnt							= DNS_COMMAND_FAILURE_CNT;
	
	pClient->LetterCounter								= 0;
	
	pClient->AnalysisTick								= 0;
#if DNS_ANALYSIS_TYPE1 == DNS_ANALYSIS_ENABLE
	pClient->AnalysisData[0].hostnameAddr					= (unsigned char*)DNS_ANALYSIS_HOSTNAME1;
	pClient->AnalysisData[0].hostIPAddr					= (unsigned char*)DNS_ANALYSIS_HOSTIP1;
#endif
#if DNS_ANALYSIS_TYPE2 == DNS_ANALYSIS_ENABLE
	pClient->AnalysisData[1].hostnameAddr					= (unsigned char*)DNS_ANALYSIS_HOSTNAME2;
	pClient->AnalysisData[1].hostIPAddr					= (unsigned char*)DNS_ANALYSIS_HOSTIP2;
#endif
#if DNS_ANALYSIS_TYPE3 == DNS_ANALYSIS_ENABLE
	pClient->AnalysisData[2].hostnameAddr					= (unsigned char*)DNS_ANALYSIS_HOSTNAME3;
	pClient->AnalysisData[2].hostIPAddr					= (unsigned char*)DNS_ANALYSIS_HOSTIP3;
#endif
	for (unsigned char i = 0; i < DNS_ANALYSIS_DATA; i++) {
		sprintf((char *)pClient->AnalysisData[i].hostname, "%s", pClient->AnalysisData[i].hostnameAddr);
		sprintf((char *)pClient->AnalysisData[i].hostIP, "%s", pClient->AnalysisData[i].hostIPAddr);
	}
	
	pClient->DictateRunCtl.dictateEnable					= false;
	pClient->DictateRunCtl.dictateTimeoutSec				= 0;
	pClient->DictateRunCtl.dictateCreatUDPSocketFailureCnt		= 0;
	pClient->DictateRunCtl.dictateSendDnsStructDataFailureCnt	= 0;
	pClient->DictateRunCtl.dictateRecvDnsStructDataFailureCnt	= 0;
	pClient->DictateRunCtl.dictateCloseUDPSocketFailureCnt		= 0;
	pClient->DictateRunCtl.dictateOverDnsAnalysis			= 0;
	
	pClient->dnsAnalysisState							= true;
	
	pClient->ProcessState								= DNS_PROCESS_CREAT_UDP_SOCKET;
	pClient->SocketStack								= NetSock;
	pClient->NetNbiotStack								= NetNbiotStack;
}

/**********************************************************************************************************
 @Function			void DNS_Analysis_Set_CtrlState(DNS_ClientsTypeDef* pClient, bool state)
 @Description			DNS_Analysis_Set_CtrlState				: 设置DNS解析控制状态
 @Input				pClient								: DNS客户端实例
					state								: 使能状态
 @Return				void
**********************************************************************************************************/
void DNS_Analysis_Set_CtrlState(DNS_ClientsTypeDef* pClient, bool state)
{
	pClient->dnsAnalysisState							= state;
}

/**********************************************************************************************************
 @Function			bool DNS_Analysis_Get_CtrlState(DNS_ClientsTypeDef* pClient)
 @Description			DNS_Analysis_Get_CtrlState				: 获取DNS解析控制状态
 @Input				pClient								: DNS客户端实例
 @Return				state								: 使能状态
**********************************************************************************************************/
bool DNS_Analysis_Get_CtrlState(DNS_ClientsTypeDef* pClient)
{
	return pClient->dnsAnalysisState;
}

/********************************************** END OF FLEE **********************************************/
