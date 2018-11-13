/**
  *********************************************************************************************************
  * @file    net_onenet_message_operate.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-08-31
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "net_onenet_message_operate.h"
#include "inspectmessageoperate.h"
#include "platform_config.h"
#include "platform_map.h"
#include "stm32l1xx_config.h"
#include "radar_api.h"
#include "tmesh_algorithm.h"
#include "string.h"

ONENET_SwapSendDataTypeDef	NETOneNETMessageSendPark;
ONENET_SwapRecvDataTypeDef	NETOneNETMessageRecvPark;

/**********************************************************************************************************
 @Function			int NET_ONENET_Message_Operate_Creat_Json_Work_Info(char* outBuffer)
 @Description			NET_ONENET_Message_Operate_Creat_Json_Work_Info
 @Input				outBuffer
 @Return				Length
 @attention			!!<<< MaxLength 300Byte >>>!!
**********************************************************************************************************/
int NET_ONENET_Message_Operate_Creat_Json_Work_Info(char* outBuffer)
{
	TCFG_EEPROM_GetMagTempCoef(&TCFG_SystemData.MagCoefX, &TCFG_SystemData.MagCoefY, &TCFG_SystemData.MagCoefZ);
	
	sprintf(outBuffer, 
		"{"
			"\"SN\":\"%08x\","
			"\"WorkInfo\":"
			"{"
				"\"Sense\":%d,"
				"\"Mode\":\"%s\","
				"\"Channel\":%d,"
				"\"Range\":%d,"
				"\"Earfcn\":%d,"
				"\"Tac\":\"%X\","
				"\"Ci\":\"%X\","
				"\"Cmdcnt\":\"%d.%d\","
				"\"Nbruntime\":\"%d.%d\","
				"\"PCP\":\"%d.%d-%d.%d.V%d.%d\","
				"\"Coef\":\"%d.%d.%d\","
				"\"Beepoff\":\"%d\""
			"}"
		"}",
		
		TCFG_EEPROM_Get_MAC_SN(),
		TCFG_EEPROM_GetSavedSensitivity(),
		TCFG_EEPROM_Get_WorkMode_String(),
		TCFG_EEPROM_GetRfChannel(),
		TCFG_Utility_Get_DistanceRange(),
		TCFG_Utility_Get_Nbiot_RadioEARFCN(),
		TCFG_Utility_Get_Nbiot_NetworkRegStatusTac(),
		TCFG_Utility_Get_Nbiot_NetworkRegStatusCellID(),
		TCFG_EEPROM_GetRFCmdCnt(), TCFG_EEPROM_GetNBCmdCnt(),
		TCFG_Utility_GetCoapConnectTime(), TCFG_Utility_GetCoapIdleTime(),
		TCFG_Utility_Get_Nbiot_PCPUpgradeStartTimes(), TCFG_Utility_Get_Nbiot_PCPUpgradePackSliceIndex(), TCFG_Utility_Get_Nbiot_PCPUpgradePackSliceNum(),
		TCFG_Utility_Get_Nbiot_PCPUpgradePackSliceSize(), TCFG_Utility_Get_Nbiot_PCPPlatformSoftVersionMajor(), TCFG_Utility_Get_Nbiot_PCPPlatformSoftVersionSub(),
		TCFG_SystemData.MagCoefX, TCFG_SystemData.MagCoefY, TCFG_SystemData.MagCoefZ,
		TCFG_EEPROM_GetBeepOff()
	);
	
	return strlen(outBuffer);
}

/**********************************************************************************************************
 @Function			int NET_ONENET_Message_Operate_Creat_Json_Basic_Info(char* outBuffer)
 @Description			NET_ONENET_Message_Operate_Creat_Json_Basic_Info
 @Input				outBuffer
 @Return				Length
 @attention			!!<<< MaxLength 300Byte >>>!!
**********************************************************************************************************/
int NET_ONENET_Message_Operate_Creat_Json_Basic_Info(char* outBuffer)
{
	sprintf(outBuffer, 
		"{"
			"\"SN\":\"%08x\","
			"\"TMoteInfo\":"
			"{"
				"\"Type\":\"%d.1\","
				"\"Vender\":\"%s\","
				"\"Hard\":\"%s\","
				"\"Soft\":\"%d:%d.%d\","
				"\"Sim\":\"%s\","
				"\"Imei\":\"%s\","
				"\"Nbvender\":\"%s\","
				"\"Nbmode\":\"%s\","
				"\"Nbcgd\":\"%s,%s\","
				"\"Boot\":\"%d.%d.%d\","
				"\"Ver\":\"%s\","
				"\"Rmold\":\"%d\""
			"}"
		"}",
		
		TCFG_EEPROM_Get_MAC_SN(),
		TCFG_Utility_Get_Mvb_ModelType(),
		TCFG_EEPROM_Get_Vender_String(),
		TCFG_Utility_Get_Hardwear_Version_String(),
		TCFG_EEPROM_GetBootVersion(), TCFG_Utility_Get_Major_Softnumber(), TCFG_Utility_Get_Sub_Softnumber(),
		TCFG_Utility_Get_Nbiot_Iccid_String(),
		TCFG_Utility_Get_Nbiot_Imei_String(),
		TCFG_Utility_Get_Nbiot_Manufacturer(),
		TCFG_Utility_Get_Nbiot_Manufacturermode(),
		TCFG_Utility_Get_Nbiot_PDPType(), TCFG_Utility_Get_Nbiot_APN(),
		TCFG_Utility_Get_SoftResetFlag(), TCFG_Utility_Get_Device_BootCount(), TCFG_EEPROM_GetDeviceRbtMode(),
		TCFG_Utility_Get_Nbiot_ModelVersion(),
		Radar_GetModel()
	);
	
	return strlen(outBuffer);
}

/**********************************************************************************************************
 @Function			int NET_ONENET_Message_Operate_Creat_Json_Dynamic_Info(char* outBuffer)
 @Description			NET_ONENET_Message_Operate_Creat_Json_Dynamic_Info
 @Input				outBuffer
 @Return				Length
 @attention			!!<<< MaxLength 300Byte >>>!!
**********************************************************************************************************/
int NET_ONENET_Message_Operate_Creat_Json_Dynamic_Info(char* outBuffer)
{
	sprintf(outBuffer, 
		"{"
			"\"SN\":\"%08x\","
			"\"TMoteInfo\":"
			"{"
				"\"Runtime\":%d,"
				"\"Rssi\":%d,"
				"\"Snr\":%d,"
				"\"Batt\":%d,"
				"\"Rlib\":\"%d\","
				"\"Rcnt\":%d,"
				"\"Temp\":%d,"
				"\"Algo\":%d,"
				"\"Qmcrbt\":%d,"
				"\"Nbboot\":%d,"
				"\"Nbsent\":%d,"
				"\"Nbrecv\":%d,"
				"\"Indelay\":%d,"
				"\"Nbheart\":%d"
			"}"
		"}",
		
		TCFG_EEPROM_Get_MAC_SN(),
		TCFG_Utility_Get_Run_Time(),
		TCFG_Utility_Get_Nbiot_Rssi_IntVal(),
		TCFG_Utility_Get_Nbiot_RadioSNR(),
		TCFG_Utility_Get_Device_Batt_ShortVal(),
		TCFG_Utility_Get_RadarLibNum(),
		TCFG_GetRadarCount(),
		TCFG_Utility_Get_Device_Temperature(),
		TCFG_Utility_Get_AlgoLibNum(),
		TCFG_Utility_Get_ReInitModuleCount(),
		TCFG_Utility_Get_Nbiot_BootCount(),
		TCFG_Utility_Get_Nbiot_SentCount(),
		TCFG_Utility_Get_Nbiot_RecvCount(),
		TCFG_EEPROM_GetCarInDelay(),
		TCFG_EEPROM_GetNbiotHeart()
	);
	
	return strlen(outBuffer);
}

/**********************************************************************************************************
 @Function			int NET_ONENET_Message_Operate_Creat_Json_Response_Info(char* outBuffer, u16 errcode)
 @Description			NET_ONENET_Message_Operate_Creat_Json_Response_Info
 @Input				outBuffer
					errcode
 @Return				Length
 @attention			!!<<< MaxLength 300Byte >>>!!
**********************************************************************************************************/
int NET_ONENET_Message_Operate_Creat_Json_Response_Info(char* outBuffer, u16 errcode)
{
	sprintf(outBuffer, 
		"{"
			"\"SN\":\"%08x\","
			"\"ResponseInfo\":"
			"{"
				"\"ret\":%d"
			"}"
		"}",
		
		TCFG_EEPROM_Get_MAC_SN(),
		errcode
	);
	
	return strlen(outBuffer);
}

/**********************************************************************************************************
 @Function			int NET_ONENET_Message_Operate_Creat_Qmc5883L_Data(unsigned char* outBuffer)
 @Description			NET_ONENET_Message_Operate_Creat_Qmc5883L_Data
 @Input				outBuffer
 @Return				Length
 @attention			!!<<< MaxLength 160Byte >>>!!
**********************************************************************************************************/
int NET_ONENET_Message_Operate_Creat_Qmc5883L_Data(unsigned char* outBuffer)
{
	Qmc5883LStatusDataTypeDef QmcStatusData;
	unsigned int bufoffset = 0;
	
	outBuffer[0] = 0;
	bufoffset += 1;
	
	for (int packIndex = 0; packIndex < UPLOAD_QMCDATA_MAXPACK; packIndex++) {
		if (Inspect_Message_QmcStatusisEmpty() != true) {
			Inspect_Message_QmcStatusDequeue(&QmcStatusData);
			memcpy(outBuffer + bufoffset, &QmcStatusData, sizeof(QmcStatusData));
			bufoffset += sizeof(QmcStatusData);
			outBuffer[0] += 1;
			Inspect_Message_QmcStatusOffSet();
		}
		else {
			break;
		}
	}
	
	return bufoffset;
}

/**********************************************************************************************************
 @Function			bool NET_OneNET_Message_SendDataisFull(void)
 @Description			NET_OneNET_Message_SendDataisFull	: 检查发送队列是否已满
 @Input				void
 @Return				true							: 已满
					false						: 未满
**********************************************************************************************************/
bool NET_OneNET_Message_SendDataisFull(void)
{
	bool MessageState;
	
	if ((NETOneNETMessageSendPark.Rear + 1) % ONENET_SEND_PARK_NUM == NETOneNETMessageSendPark.Front) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_OneNET_Message_RecvDataisFull(void)
 @Description			NET_OneNET_Message_RecvDataisFull	: 检查接收队列是否已满
 @Input				void
 @Return				true							: 已满
					false						: 未满
**********************************************************************************************************/
bool NET_OneNET_Message_RecvDataisFull(void)
{
	bool MessageState;
	
	if ((NETOneNETMessageRecvPark.Rear + 1) % ONENET_RECV_PARK_NUM == NETOneNETMessageRecvPark.Front) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_OneNET_Message_SendDataisEmpty(void)
 @Description			NET_OneNET_Message_SendDataisEmpty	: 检查发送队列是否已空
 @Input				void
 @Return				true							: 已空
					false						: 未空
**********************************************************************************************************/
bool NET_OneNET_Message_SendDataisEmpty(void)
{
	bool MessageState;
	
	if (NETOneNETMessageSendPark.Front == NETOneNETMessageSendPark.Rear) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_OneNET_Message_RecvDataisEmpty(void)
 @Description			NET_OneNET_Message_RecvDataisEmpty	: 检查接收队列是否已空
 @Input				void
 @Return				true							: 已空
					false						: 未空
**********************************************************************************************************/
bool NET_OneNET_Message_RecvDataisEmpty(void)
{
	bool MessageState;
	
	if (NETOneNETMessageRecvPark.Front == NETOneNETMessageRecvPark.Rear) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			void NET_OneNET_Message_SendDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
 @Description			NET_OneNET_Message_SendDataEnqueue	: 发送数据写入队列
 @Input				dataBuf	 		 			: 需写入数据
					dataLength					: 需写入数据长度
 @Return				void
**********************************************************************************************************/
void NET_OneNET_Message_SendDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
{
	if ((dataBuf == NULL) || (dataLength > ONENET_SEND_BUFFER_SIZE)) {
		return;
	}
	
	NETOneNETMessageSendPark.Rear = (NETOneNETMessageSendPark.Rear + 1) % ONENET_SEND_PARK_NUM;				//队尾偏移1
	memset((u8 *)&NETOneNETMessageSendPark.Park[NETOneNETMessageSendPark.Rear], 0x0, sizeof(NETOneNETMessageSendPark.Park[NETOneNETMessageSendPark.Rear]));
	memcpy(NETOneNETMessageSendPark.Park[NETOneNETMessageSendPark.Rear].Buffer, dataBuf, dataLength);
	NETOneNETMessageSendPark.Park[NETOneNETMessageSendPark.Rear].Length = dataLength;
	
	if (NET_OneNET_Message_SendDataisFull() == true) {												//队列已满
		NETOneNETMessageSendPark.Front = (NETOneNETMessageSendPark.Front + 1) % ONENET_SEND_PARK_NUM;			//队头偏移1
	}
}

/**********************************************************************************************************
 @Function			void NET_OneNET_Message_RecvDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
 @Description			NET_OneNET_Message_RecvDataEnqueue	: 接收数据写入队列
 @Input				dataBuf	 		 			: 需写入数据
					dataLength					: 需写入数据长度
 @Return				void
**********************************************************************************************************/
void NET_OneNET_Message_RecvDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
{
	if ((dataBuf == NULL) || (dataLength > ONENET_RECV_BUFFER_SIZE)) {
		return;
	}
	
	NETOneNETMessageRecvPark.Rear = (NETOneNETMessageRecvPark.Rear + 1) % ONENET_RECV_PARK_NUM;				//队尾偏移1
	memset((u8 *)&NETOneNETMessageRecvPark.Park[NETOneNETMessageRecvPark.Rear], 0x0, sizeof(NETOneNETMessageRecvPark.Park[NETOneNETMessageRecvPark.Rear]));
	memcpy(NETOneNETMessageRecvPark.Park[NETOneNETMessageRecvPark.Rear].Buffer, dataBuf, dataLength);
	NETOneNETMessageRecvPark.Park[NETOneNETMessageRecvPark.Rear].Length = dataLength;
	
	if (NET_OneNET_Message_RecvDataisFull() == true) {												//队列已满
		NETOneNETMessageRecvPark.Front = (NETOneNETMessageRecvPark.Front + 1) % ONENET_RECV_PARK_NUM;			//队头偏移1
	}
}

/**********************************************************************************************************
 @Function			bool NET_OneNET_Message_SendDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
 @Description			NET_OneNET_Message_SendDataDequeue	: 发送数据读出队列
 @Input				dataBuf	 		 			: 需读出数据地址
					dataLength					: 需读出数据长度地址
 @Return				true							: 未空
					false						: 已空
**********************************************************************************************************/
bool NET_OneNET_Message_SendDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
{
	bool MessageState;
	unsigned char front;
	
	if (NET_OneNET_Message_SendDataisEmpty() == true) {												//队列已空
		MessageState = false;
	}
	else {																					//队列未空
		front = (NETOneNETMessageSendPark.Front + 1) % ONENET_SEND_PARK_NUM;								//队头偏移1
		memcpy(dataBuf, NETOneNETMessageSendPark.Park[front].Buffer, NETOneNETMessageSendPark.Park[front].Length);
		*dataLength = NETOneNETMessageSendPark.Park[front].Length;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_OneNET_Message_RecvDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
 @Description			NET_OneNET_Message_RecvDataDequeue	: 接收数据读出队列
 @Input				dataBuf	 		 			: 需读出数据地址
					dataLength					: 需读出数据长度地址
 @Return				true							: 未空
					false						: 已空
**********************************************************************************************************/
bool NET_OneNET_Message_RecvDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
{
	bool MessageState;
	unsigned char front;
	
	if (NET_OneNET_Message_RecvDataisEmpty() == true) {												//队列已空
		MessageState = false;
	}
	else {																					//队列未空
		front = (NETOneNETMessageRecvPark.Front + 1) % ONENET_RECV_PARK_NUM;								//队头偏移1
		memcpy(dataBuf, NETOneNETMessageRecvPark.Park[front].Buffer, NETOneNETMessageRecvPark.Park[front].Length);
		*dataLength = NETOneNETMessageRecvPark.Park[front].Length;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_OneNET_Message_SendDataOffSet(void)
 @Description			NET_OneNET_Message_SendDataOffSet	: 发送数据队列(队列头偏移1)
 @Input				void
 @Return				true							: 未空
					false						: 已空
**********************************************************************************************************/
bool NET_OneNET_Message_SendDataOffSet(void)
{
	bool MessageState;
	
	if (NET_OneNET_Message_SendDataisEmpty() == true) {												//队列已空
		MessageState = false;
	}
	else {																					//队列未空
		NETOneNETMessageSendPark.Front = (NETOneNETMessageSendPark.Front + 1) % ONENET_SEND_PARK_NUM;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_OneNET_Message_RecvDataOffSet(void)
 @Description			NET_OneNET_Message_RecvDataOffSet	: 接收数据队列(队列头偏移1)
 @Input				void
 @Return				true							: 未空
					false						: 已空
**********************************************************************************************************/
bool NET_OneNET_Message_RecvDataOffSet(void)
{
	bool MessageState;
	
	if (NET_OneNET_Message_RecvDataisEmpty() == true) {												//队列已空
		MessageState = false;
	}
	else {																					//队列未空
		NETOneNETMessageRecvPark.Front = (NETOneNETMessageRecvPark.Front + 1) % ONENET_RECV_PARK_NUM;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			unsigned char NET_OneNET_Message_SendDataRear(void)
 @Description			NET_OneNET_Message_SendDataRear	: 发送数据队尾值
 @Input				void
 @Return				发送数据队尾值
**********************************************************************************************************/
unsigned char NET_OneNET_Message_SendDataRear(void)
{
	return NETOneNETMessageSendPark.Rear;
}

/**********************************************************************************************************
 @Function			unsigned char NET_OneNET_Message_RecvDataRear(void)
 @Description			NET_OneNET_Message_RecvDataRear	: 接收数据队尾值
 @Input				void
 @Return				接收数据队尾值
**********************************************************************************************************/
unsigned char NET_OneNET_Message_RecvDataRear(void)
{
	return NETOneNETMessageRecvPark.Rear;
}

/********************************************** END OF FLEE **********************************************/
