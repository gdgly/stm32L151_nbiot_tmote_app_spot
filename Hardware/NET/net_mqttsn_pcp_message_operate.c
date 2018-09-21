/**
  *********************************************************************************************************
  * @file    net_mqttsn_pcp_message_operate.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-09-21
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "net_mqttsn_pcp_message_operate.h"
#include "platform_config.h"
#include "platform_map.h"
#include "stm32l1xx_config.h"
#include "string.h"

MqttSNPCP_SwapSendDataTypeDef		NETMqttSNPcpMessageSendPark;
MqttSNPCP_SwapRecvDataTypeDef		NETMqttSNPcpMessageRecvPark;

/**********************************************************************************************************
 @Function			static bool NET_MqttSN_PCP_Message_SendDataisFull(void)
 @Description			NET_MqttSN_PCP_Message_SendDataisFull	: 检查发送队列是否已满
 @Input				void
 @Return				true								: 已满
					false							: 未满
**********************************************************************************************************/
bool NET_MqttSN_PCP_Message_SendDataisFull(void)
{
	bool MessageState;
	
	if ((NETMqttSNPcpMessageSendPark.Rear + 1) % MQTTSN_PCP_SEND_PACK_NUM == NETMqttSNPcpMessageSendPark.Front) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			static bool NET_MqttSN_PCP_Message_RecvDataisFull(void)
 @Description			NET_MqttSN_PCP_Message_RecvDataisFull	: 检查接收队列是否已满
 @Input				void
 @Return				true								: 已满
					false							: 未满
**********************************************************************************************************/
bool NET_MqttSN_PCP_Message_RecvDataisFull(void)
{
	bool MessageState;
	
	if ((NETMqttSNPcpMessageRecvPark.Rear + 1) % MQTTSN_PCP_RECV_PACK_NUM == NETMqttSNPcpMessageRecvPark.Front) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			static bool NET_MqttSN_PCP_Message_SendDataisEmpty(void)
 @Description			NET_MqttSN_PCP_Message_SendDataisEmpty	: 检查发送队列是否已空
 @Input				void
 @Return				true								: 已空
					false							: 未空
**********************************************************************************************************/
bool NET_MqttSN_PCP_Message_SendDataisEmpty(void)
{
	bool MessageState;
	
	if (NETMqttSNPcpMessageSendPark.Front == NETMqttSNPcpMessageSendPark.Rear) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			static bool NET_MqttSN_PCP_Message_RecvDataisEmpty(void)
 @Description			NET_MqttSN_PCP_Message_RecvDataisEmpty	: 检查接收队列是否已空
 @Input				void
 @Return				true								: 已空
					false							: 未空
**********************************************************************************************************/
bool NET_MqttSN_PCP_Message_RecvDataisEmpty(void)
{
	bool MessageState;
	
	if (NETMqttSNPcpMessageRecvPark.Front == NETMqttSNPcpMessageRecvPark.Rear) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			void NET_MqttSN_PCP_Message_SendDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
 @Description			NET_MqttSN_PCP_Message_SendDataEnqueue	: 发送数据写入队列
 @Input				dataBuf	 		 				: 需写入数据
					dataLength						: 需写入数据长度
 @Return				void
**********************************************************************************************************/
void NET_MqttSN_PCP_Message_SendDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
{
	if ((dataBuf == NULL) || (dataLength > MQTTSN_PCP_SEND_BUFFER_SIZE)) {
		return;
	}
	
	NETMqttSNPcpMessageSendPark.Rear = (NETMqttSNPcpMessageSendPark.Rear + 1) % MQTTSN_PCP_SEND_PACK_NUM;		//队尾偏移1
	memset((u8 *)&NETMqttSNPcpMessageSendPark.Park[NETMqttSNPcpMessageSendPark.Rear], 0x0, sizeof(NETMqttSNPcpMessageSendPark.Park[NETMqttSNPcpMessageSendPark.Rear]));
	memcpy(NETMqttSNPcpMessageSendPark.Park[NETMqttSNPcpMessageSendPark.Rear].Buffer, dataBuf, dataLength);
	NETMqttSNPcpMessageSendPark.Park[NETMqttSNPcpMessageSendPark.Rear].Length = dataLength;
	
	if (NET_MqttSN_PCP_Message_SendDataisFull() == true) {												//队列已满
		NETMqttSNPcpMessageSendPark.Front = (NETMqttSNPcpMessageSendPark.Front + 1) % MQTTSN_PCP_SEND_PACK_NUM;	//队头偏移1
	}
}

/**********************************************************************************************************
 @Function			void NET_MqttSN_PCP_Message_RecvDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
 @Description			NET_MqttSN_PCP_Message_RecvDataEnqueue	: 接收数据写入队列
 @Input				dataBuf	 		 				: 需写入数据
					dataLength						: 需写入数据长度
 @Return				void
**********************************************************************************************************/
void NET_MqttSN_PCP_Message_RecvDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
{
	if ((dataBuf == NULL) || (dataLength > MQTTSN_PCP_RECV_BUFFER_SIZE)) {
		return;
	}
	
	NETMqttSNPcpMessageRecvPark.Rear = (NETMqttSNPcpMessageRecvPark.Rear + 1) % MQTTSN_PCP_RECV_PACK_NUM;		//队尾偏移1
	memset((u8 *)&NETMqttSNPcpMessageRecvPark.Park[NETMqttSNPcpMessageRecvPark.Rear], 0x0, sizeof(NETMqttSNPcpMessageRecvPark.Park[NETMqttSNPcpMessageRecvPark.Rear]));
	memcpy(NETMqttSNPcpMessageRecvPark.Park[NETMqttSNPcpMessageRecvPark.Rear].Buffer, dataBuf, dataLength);
	NETMqttSNPcpMessageRecvPark.Park[NETMqttSNPcpMessageRecvPark.Rear].Length = dataLength;
	
	if (NET_MqttSN_PCP_Message_RecvDataisFull() == true) {												//队列已满
		NETMqttSNPcpMessageRecvPark.Front = (NETMqttSNPcpMessageRecvPark.Front + 1) % MQTTSN_PCP_RECV_PACK_NUM;	//队头偏移1
	}
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_PCP_Message_SendDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
 @Description			NET_MqttSN_PCP_Message_SendDataDequeue	: 发送数据读出队列
 @Input				dataBuf	 		 				: 需读出数据地址
					dataLength						: 需读出数据长度地址
 @Return				true								: 未空
					false							: 已空
**********************************************************************************************************/
bool NET_MqttSN_PCP_Message_SendDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
{
	bool MessageState;
	unsigned char front;
	
	if (NET_MqttSN_PCP_Message_SendDataisEmpty() == true) {											//队列已空
		MessageState = false;
	}
	else {																					//队列未空
		front = (NETMqttSNPcpMessageSendPark.Front + 1) % MQTTSN_PCP_SEND_PACK_NUM;						//队头偏移1
		memcpy(dataBuf, NETMqttSNPcpMessageSendPark.Park[front].Buffer, NETMqttSNPcpMessageSendPark.Park[front].Length);
		*dataLength = NETMqttSNPcpMessageSendPark.Park[front].Length;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_PCP_Message_RecvDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
 @Description			NET_MqttSN_PCP_Message_RecvDataDequeue	: 接收数据读出队列
 @Input				dataBuf	 		 				: 需读出数据地址
					dataLength						: 需读出数据长度地址
 @Return				true								: 未空
					false							: 已空
**********************************************************************************************************/
bool NET_MqttSN_PCP_Message_RecvDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
{
	bool MessageState;
	unsigned char front;
	
	if (NET_MqttSN_PCP_Message_RecvDataisEmpty() == true) {											//队列已空
		MessageState = false;
	}
	else {																					//队列未空
		front = (NETMqttSNPcpMessageRecvPark.Front + 1) % MQTTSN_PCP_RECV_PACK_NUM;						//队头偏移1
		memcpy(dataBuf, NETMqttSNPcpMessageRecvPark.Park[front].Buffer, NETMqttSNPcpMessageRecvPark.Park[front].Length);
		*dataLength = NETMqttSNPcpMessageRecvPark.Park[front].Length;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_PCP_Message_SendDataOffSet(void)
 @Description			NET_MqttSN_PCP_Message_SendDataOffSet	: 发送数据队列(队列头偏移1)
 @Input				void
 @Return				true								: 未空
					false							: 已空
**********************************************************************************************************/
bool NET_MqttSN_PCP_Message_SendDataOffSet(void)
{
	bool MessageState;
	
	if (NET_MqttSN_PCP_Message_SendDataisEmpty() == true) {											//队列已空
		MessageState = false;
	}
	else {																					//队列未空
		NETMqttSNPcpMessageSendPark.Front = (NETMqttSNPcpMessageSendPark.Front + 1) % MQTTSN_PCP_SEND_PACK_NUM;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_PCP_Message_RecvDataOffSet(void)
 @Description			NET_MqttSN_PCP_Message_RecvDataOffSet	: 接收数据队列(队列头偏移1)
 @Input				void
 @Return				true								: 未空
					false							: 已空
**********************************************************************************************************/
bool NET_MqttSN_PCP_Message_RecvDataOffSet(void)
{
	bool MessageState;
	
	if (NET_MqttSN_PCP_Message_RecvDataisEmpty() == true) {											//队列已空
		MessageState = false;
	}
	else {																					//队列未空
		NETMqttSNPcpMessageRecvPark.Front = (NETMqttSNPcpMessageRecvPark.Front + 1) % MQTTSN_PCP_RECV_PACK_NUM;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			unsigned char NET_MqttSN_PCP_Message_SendDataRear(void)
 @Description			NET_MqttSN_PCP_Message_SendDataRear	: 发送数据队尾值
 @Input				void
 @Return				发送数据队尾值
**********************************************************************************************************/
unsigned char NET_MqttSN_PCP_Message_SendDataRear(void)
{
	return NETMqttSNPcpMessageSendPark.Rear;
}

/**********************************************************************************************************
 @Function			unsigned char NET_MqttSN_PCP_Message_RecvDataRear(void)
 @Description			NET_MqttSN_PCP_Message_RecvDataRear	: 接收数据队尾值
 @Input				void
 @Return				接收数据队尾值
**********************************************************************************************************/
unsigned char NET_MqttSN_PCP_Message_RecvDataRear(void)
{
	return NETMqttSNPcpMessageRecvPark.Rear;
}

/********************************************** END OF FLEE **********************************************/
