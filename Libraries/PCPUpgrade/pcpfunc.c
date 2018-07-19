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
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	PCP_MessageDataTypeDef* PCPMessageRecv = (PCP_MessageDataTypeDef*)pClient->Recvbuf;
	
	pClient->Parameter.ProtocolType = PCPMessageRecv->ProtocolType;
	pClient->Parameter.MessageType = PCPMessageRecv->MessageType;
	
	/* 查询设备版本 */
	if (pClient->Parameter.MessageType == PCP_QueryDeviceVersion) {
		
		
		
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
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
	
	memset((void*)pClient->DataProcessStack, 0x0, pClient->DataProcessStack_size);
	
	PCPMessageProcess->StartX = PCPSock_htons(0xFFFE);
	PCPMessageProcess->ProtocolType = pClient->Parameter.ProtocolType;
	PCPMessageProcess->MessageType = PCP_QueryDeviceVersion;
	PCPMessageProcess->CRCCheckCode = 0x0000;
	PCPMessageProcess->PacketDataLength = PCPSock_htons(17);
	//Todo
	
	
	
	return PCPStatus;
}



















/********************************************** END OF FLEE **********************************************/
