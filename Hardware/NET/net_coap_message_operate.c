/**
  *********************************************************************************************************
  * @file    net_coap_message_operate.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-03-02
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "net_coap_message_operate.h"
#include "inspectmessageoperate.h"
#include "platform_config.h"
#include "platform_map.h"
#include "stm32l1xx_config.h"
#include "fifomessage.h"
#include "radar_api.h"
#include "tmesh_algorithm.h"
#include "string.h"

#ifdef COAP_MSG_DBUG
#include "radio_hal_rf.h"
#include "radio_rf_app.h"
#endif

#if NETFIFOMESSAGETYPE == NETFIFOMESSAGEDISABLE
COAP_SwapSendDataTypeDef		NETCoapMessageSendPark;
COAP_SwapRecvDataTypeDef		NETCoapMessageRecvPark;
#endif

#if NETFIFOMESSAGETYPE == NETFIFOMESSAGEENABLE
#define MESSAGEFIFO_SENDPARKNUM_MAX			NETFIFO_COAPSENDPARKNUM_MAX
#define MESSAGEFIFO_RECVPARKNUM_MAX			NETFIFO_COAPRECVPARKNUM_MAX
#define MESSAGEFIFO_SENDPARKSIZE_MAX			NETFIFO_COAPSENDPARKSIZE_MAX
#define MESSAGEFIFO_RECVPARKSIZE_MAX			NETFIFO_COAPRECVPARKSIZE_MAX

MessageFifoTypeDef			NETCoapFifoMessageSendPark;
MessageFifoTypeDef			NETCoapFifoMessageRecvPark;

unsigned char				FifoMessageSendBuf[MESSAGEFIFO_SENDPARKSIZE_MAX];
unsigned char				FifoMessageRecvBuf[MESSAGEFIFO_RECVPARKSIZE_MAX];
#endif

#ifdef COAP_MSG_DBUG
/**********************************************************************************************************
 @Function			static void NET_Coap_Message_Debug(char* name, char* obj, char* fun, int len)
 @Description			NET_Coap_Message_Debug			: 消息队列调试信息
 @Input				name
					obj
					fun
					len
 @Return				void
**********************************************************************************************************/
static void NET_Coap_Message_Debug(char* name, char* obj, char* fun, int len)
{
	COAP_MSG_DBUG_PRINTF("DB: %s %s %s %d", name, obj, fun, len);
}
#endif

/**********************************************************************************************************
 @Function			int NET_COAP_Message_Operate_Creat_Json_Work_Info(char* outBuffer)
 @Description			NET_COAP_Message_Operate_Creat_Json_Work_Info
 @Input				outBuffer
 @Return				Length
 @attention			!!<<< MaxLength 450Byte >>>!!
**********************************************************************************************************/
int NET_COAP_Message_Operate_Creat_Json_Work_Info(char* outBuffer)
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
				"\"APN\":\"%s\","
				"\"PCP\":\"%d.%d-%d.%d.V%d.%d\","
				"\"Coef\":\"%d.%d.%d\","
				"\"Rollinit\":%d,"
				"\"RadioRv\":%d"
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
		TCFG_Utility_Get_Nbiot_PDPContext_APN(),
		TCFG_Utility_Get_Nbiot_PCPUpgradeStartTimes(), TCFG_Utility_Get_Nbiot_PCPUpgradePackSliceIndex(), TCFG_Utility_Get_Nbiot_PCPUpgradePackSliceNum(),
		TCFG_Utility_Get_Nbiot_PCPUpgradePackSliceSize(), TCFG_Utility_Get_Nbiot_PCPPlatformSoftVersionMajor(), TCFG_Utility_Get_Nbiot_PCPPlatformSoftVersionSub(),
		TCFG_SystemData.MagCoefX, TCFG_SystemData.MagCoefY, TCFG_SystemData.MagCoefZ,
		TCFG_EEPROM_GetRollingOverInitSensor(),
		TCFG_Utility_Get_RadioGatewayNearby()
	);
	
	return strlen(outBuffer);
}

/**********************************************************************************************************
 @Function			int NET_COAP_Message_Operate_Creat_Json_Basic_Info(char* outBuffer)
 @Description			NET_COAP_Message_Operate_Creat_Json_Basic_Info
 @Input				outBuffer
 @Return				Length
 @attention			!!<<< MaxLength 450Byte >>>!!
**********************************************************************************************************/
int NET_COAP_Message_Operate_Creat_Json_Basic_Info(char* outBuffer)
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
				"\"Imsi\":\"%s\","
				"\"Nbvender\":\"%s\","
				"\"Nbmode\":\"%s\","
				"\"Boot\":\"%d.%d.%d\","
				"\"Ver\":\"%s\","
				"\"Rmold\":%d,"
				"\"Rvcc\":%d"
			"}"
		"}",
		
		TCFG_EEPROM_Get_MAC_SN(),
		TCFG_Utility_Get_Mvb_ModelType(),
		TCFG_EEPROM_Get_Vender_String(),
		TCFG_Utility_Get_Hardwear_Version_String(),
		TCFG_EEPROM_GetBootVersion(), TCFG_Utility_Get_Major_Softnumber(), TCFG_Utility_Get_Sub_Softnumber(),
		TCFG_Utility_Get_Nbiot_Iccid_String(),
		TCFG_Utility_Get_Nbiot_Imei_String(),
		TCFG_Utility_Get_Nbiot_Imsi_String(),
		TCFG_Utility_Get_Nbiot_Manufacturer(),
		TCFG_Utility_Get_Nbiot_Manufacturermode(),
		TCFG_Utility_Get_SoftResetFlag(), TCFG_Utility_Get_Device_BootCount(), TCFG_EEPROM_GetDeviceRbtMode(),
		TCFG_Utility_Get_Nbiot_ModelVersion(),
		Radar_GetModel(),
		Radar_Get_RadarVcc()
	);
	
	return strlen(outBuffer);
}

/**********************************************************************************************************
 @Function			int NET_COAP_Message_Operate_Creat_Json_Dynamic_Info(char* outBuffer)
 @Description			NET_COAP_Message_Operate_Creat_Json_Dynamic_Info
 @Input				outBuffer
 @Return				Length
 @attention			!!<<< MaxLength 450Byte >>>!!
**********************************************************************************************************/
int NET_COAP_Message_Operate_Creat_Json_Dynamic_Info(char* outBuffer)
{
	sprintf(outBuffer, 
		"{"
			"\"SN\":\"%08x\","
			"\"TMoteInfo\":"
			"{"
				"\"RT\":%d,"
				"\"Batt\":%d,"
				"\"RAlib\":[%d,%d],"
				"\"Rcnt\":%d,"
				"\"Temp\":%d,"
				"\"Qmcrbt\":%d,"
				"\"Nbrun\":[%d,%d,%d,%d,%d],"
				"\"Indelay\":%d,"
				"\"Gain\":[%d,%d],"
				"\"Smode\":%d,"
				"\"Sinter\":%d,"
				"\"hpass\":%d,"
				"\"x\":[%d,%d,%d,%d,%d],"
				"\"y\":[%d,%d,%d,%d,%d],"
				"\"z\":[%d,%d,%d,%d,%d]"
			"}"
		"}",
		
		TCFG_EEPROM_Get_MAC_SN(),
		TCFG_Utility_Get_Run_Time() / 60 / 60,
		TCFG_Utility_Get_Device_Batt_ShortVal(),
		TCFG_Utility_Get_RadarLibNum(),
		TCFG_Utility_Get_AlgoLibNum(),
		TCFG_GetRadarCount(),
		TCFG_Utility_Get_Device_Temperature(),
		TCFG_Utility_Get_ReInitModuleCount(),
		TCFG_Utility_Get_Nbiot_BootCount(), TCFG_Utility_Get_Nbiot_SentCount(), TCFG_Utility_Get_Nbiot_RecvCount(), TCFG_Utility_Get_NBIot_SentCountLimit(), (TCFG_EEPROM_GetNbiotHeart() * 15),
		TCFG_EEPROM_GetCarInDelay(),
		TCFG_Utility_Get_GainCover(), TCFG_EEPROM_GetRadarGain(),
		TCFG_EEPROM_GetSenseMode(),
		Radar_Get_SampleInterval(),
		tradar_get_highpass(),
		TCFG_EEPROM_GetMagManualBack(0, TCFG_X_AXIS),
		TCFG_EEPROM_GetMagManualBack(1, TCFG_X_AXIS),
		TCFG_EEPROM_GetMagManualBack(2, TCFG_X_AXIS),
		TCFG_EEPROM_GetMagManualBack(3, TCFG_X_AXIS),
		TCFG_EEPROM_GetMagManualBack(4, TCFG_X_AXIS),
		TCFG_EEPROM_GetMagManualBack(0, TCFG_Y_AXIS),
		TCFG_EEPROM_GetMagManualBack(1, TCFG_Y_AXIS),
		TCFG_EEPROM_GetMagManualBack(2, TCFG_Y_AXIS),
		TCFG_EEPROM_GetMagManualBack(3, TCFG_Y_AXIS),
		TCFG_EEPROM_GetMagManualBack(4, TCFG_Y_AXIS),
		Qmc5883lHistoryBack.Z_Back[0],
		Qmc5883lHistoryBack.Z_Back[1],
		Qmc5883lHistoryBack.Z_Back[2],
		Qmc5883lHistoryBack.Z_Back[3],
		Qmc5883lHistoryBack.Z_Back[4]
#if 0
		TCFG_EEPROM_GetMagManualBack(0, TCFG_Z_AXIS),
		TCFG_EEPROM_GetMagManualBack(1, TCFG_Z_AXIS),
		TCFG_EEPROM_GetMagManualBack(2, TCFG_Z_AXIS),
		TCFG_EEPROM_GetMagManualBack(3, TCFG_Z_AXIS),
		TCFG_EEPROM_GetMagManualBack(4, TCFG_Z_AXIS)
#endif
	);
	
	return strlen(outBuffer);
}

/**********************************************************************************************************
 @Function			int NET_COAP_Message_Operate_Creat_Json_Response_Info(char* outBuffer, u16 errcode, char msgId)
 @Description			NET_COAP_Message_Operate_Creat_Json_Response_Info
 @Input				outBuffer
					errcode
 @Return				Length
 @attention			!!<<< MaxLength 450Byte >>>!!
**********************************************************************************************************/
int NET_COAP_Message_Operate_Creat_Json_Response_Info(char* outBuffer, u16 errcode, char msgId)
{
	sprintf(outBuffer, 
		"{"
			"\"SN\":\"%08x\","
			"\"ResponseInfo\":"
			"{"
				"\"ret\":%d,"
				"\"msgid\":\"%c\""
			"}"
		"}",
		
		TCFG_EEPROM_Get_MAC_SN(),
		errcode,
		msgId
	);
	
	return strlen(outBuffer);
}

#if NETFIFOMESSAGETYPE == NETFIFOMESSAGEDISABLE
/**********************************************************************************************************
 @Function			static bool NET_Coap_Message_SendDataisFull(void)
 @Description			NET_Coap_Message_SendDataisFull	: 检查发送队列是否已满
 @Input				void
 @Return				true							: 已满
					false						: 未满
**********************************************************************************************************/
static bool NET_Coap_Message_SendDataisFull(void)
{
	bool MessageState;
	
	if ((NETCoapMessageSendPark.Rear + 1) % COAP_SEND_PARK_NUM == NETCoapMessageSendPark.Front) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			static bool NET_Coap_Message_RecvDataisFull(void)
 @Description			NET_Coap_Message_SendDataisFull	: 检查接收队列是否已满
 @Input				void
 @Return				true							: 已满
					false						: 未满
**********************************************************************************************************/
static bool NET_Coap_Message_RecvDataisFull(void)
{
	bool MessageState;
	
	if ((NETCoapMessageRecvPark.Rear + 1) % COAP_RECV_PARK_NUM == NETCoapMessageRecvPark.Front) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			static bool NET_Coap_Message_SendDataisEmpty(void)
 @Description			NET_Coap_Message_SendDataisEmpty	: 检查发送队列是否已空
 @Input				void
 @Return				true							: 已空
					false						: 未空
**********************************************************************************************************/
static bool NET_Coap_Message_SendDataisEmpty(void)
{
	bool MessageState;
	
	if (NETCoapMessageSendPark.Front == NETCoapMessageSendPark.Rear) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			static bool NET_Coap_Message_RecvDataisEmpty(void)
 @Description			NET_Coap_Message_SendDataisEmpty	: 检查接收队列是否已空
 @Input				void
 @Return				true							: 已空
					false						: 未空
**********************************************************************************************************/
static bool NET_Coap_Message_RecvDataisEmpty(void)
{
	bool MessageState;
	
	if (NETCoapMessageRecvPark.Front == NETCoapMessageRecvPark.Rear) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}
#endif

/**********************************************************************************************************
 @Function			void NET_Coap_FifoSendMessageInit(void)
 @Description			NET_Coap_FifoSendMessageInit		: 发送数据Fifo初始化
 @Input				void
 @Return				void
**********************************************************************************************************/
void NET_Coap_FifoSendMessageInit(void)
{
#if NETFIFOMESSAGETYPE == NETFIFOMESSAGEENABLE
	netMessageFifoInit(&NETCoapFifoMessageSendPark, FifoMessageSendBuf, sizeof(FifoMessageSendBuf), MESSAGEFIFO_SENDPARKNUM_MAX);
	
#ifdef COAP_MSG_DBUG
	NET_Coap_Message_Debug("CoAP", "SP", "Init", sizeof(FifoMessageSendBuf));
#endif
#endif
}

/**********************************************************************************************************
 @Function			void NET_Coap_FifoRecvMessageInit(void)
 @Description			NET_Coap_FifoRecvMessageInit		: 接收数据Fifo初始化
 @Input				void
 @Return				void
**********************************************************************************************************/
void NET_Coap_FifoRecvMessageInit(void)
{
#if NETFIFOMESSAGETYPE == NETFIFOMESSAGEENABLE
	netMessageFifoInit(&NETCoapFifoMessageRecvPark, FifoMessageRecvBuf, sizeof(FifoMessageRecvBuf), MESSAGEFIFO_RECVPARKNUM_MAX);
	
#ifdef COAP_MSG_DBUG
	NET_Coap_Message_Debug("CoAP", "RP", "Init", sizeof(FifoMessageRecvBuf));
#endif
#endif
}

/**********************************************************************************************************
 @Function			void NET_Coap_Message_SendDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
 @Description			NET_Coap_Message_SendDataEnqueue	: 发送数据写入队列
 @Input				dataBuf	 		 			: 需写入数据
					dataLength					: 需写入数据长度
 @Return				void
**********************************************************************************************************/
void NET_Coap_Message_SendDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
{
#if NETFIFOMESSAGETYPE == NETFIFOMESSAGEDISABLE
	if ((dataBuf == NULL) || (dataLength > COAP_SEND_BUFFER_SIZE)) {
		return;
	}
	
	NETCoapMessageSendPark.Rear = (NETCoapMessageSendPark.Rear + 1) % COAP_SEND_PARK_NUM;					//队尾偏移1
	memset((u8 *)&NETCoapMessageSendPark.Park[NETCoapMessageSendPark.Rear], 0x0, sizeof(NETCoapMessageSendPark.Park[NETCoapMessageSendPark.Rear]));
	memcpy(NETCoapMessageSendPark.Park[NETCoapMessageSendPark.Rear].Buffer, dataBuf, dataLength);
	NETCoapMessageSendPark.Park[NETCoapMessageSendPark.Rear].Length = dataLength;
	
	if (NET_Coap_Message_SendDataisFull() == true) {													//队列已满
		NETCoapMessageSendPark.Front = (NETCoapMessageSendPark.Front + 1) % COAP_SEND_PARK_NUM;				//队头偏移1
	}
#endif
	
#if NETFIFOMESSAGETYPE == NETFIFOMESSAGEENABLE
	netMessageFifoEnqueue(&NETCoapFifoMessageSendPark, dataBuf, dataLength);
	
#ifdef COAP_MSG_DBUG
	NET_Coap_Message_Debug("CoAP", "SP", "In", dataLength);
#endif
#endif
}

/**********************************************************************************************************
 @Function			void NET_Coap_Message_RecvDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
 @Description			NET_Coap_Message_RecvDataEnqueue	: 接收数据写入队列
 @Input				dataBuf	 		 			: 需写入数据
					dataLength					: 需写入数据长度
 @Return				void
**********************************************************************************************************/
void NET_Coap_Message_RecvDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
{
#if NETFIFOMESSAGETYPE == NETFIFOMESSAGEDISABLE
	if ((dataBuf == NULL) || (dataLength > COAP_RECV_BUFFER_SIZE)) {
		return;
	}
	
	NETCoapMessageRecvPark.Rear = (NETCoapMessageRecvPark.Rear + 1) % COAP_RECV_PARK_NUM;					//队尾偏移1
	memset((u8 *)&NETCoapMessageRecvPark.Park[NETCoapMessageRecvPark.Rear], 0x0, sizeof(NETCoapMessageRecvPark.Park[NETCoapMessageRecvPark.Rear]));
	memcpy(NETCoapMessageRecvPark.Park[NETCoapMessageRecvPark.Rear].Buffer, dataBuf, dataLength);
	NETCoapMessageRecvPark.Park[NETCoapMessageRecvPark.Rear].Length = dataLength;
	
	if (NET_Coap_Message_RecvDataisFull() == true) {													//队列已满
		NETCoapMessageRecvPark.Front = (NETCoapMessageRecvPark.Front + 1) % COAP_RECV_PARK_NUM;				//队头偏移1
	}
#endif
	
#if NETFIFOMESSAGETYPE == NETFIFOMESSAGEENABLE
	netMessageFifoEnqueue(&NETCoapFifoMessageRecvPark, dataBuf, dataLength);
	
#ifdef COAP_MSG_DBUG
	NET_Coap_Message_Debug("CoAP", "RP", "In", dataLength);
#endif
#endif
}

/**********************************************************************************************************
 @Function			bool NET_Coap_Message_SendDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
 @Description			NET_Coap_Message_SendDataDequeue	: 发送数据读出队列
 @Input				dataBuf	 		 			: 需读出数据地址
					dataLength					: 需读出数据长度地址
 @Return				true							: 未空
					false						: 已空
**********************************************************************************************************/
bool NET_Coap_Message_SendDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
{
#ifdef COAP_MSG_DBUG
	bool MessageState;
#endif
	
#if NETFIFOMESSAGETYPE == NETFIFOMESSAGEDISABLE
	bool MessageState;
	unsigned char front;
	
	if (NET_Coap_Message_SendDataisEmpty() == true) {													//队列已空
		MessageState = false;
	}
	else {																					//队列未空
		front = (NETCoapMessageSendPark.Front + 1) % COAP_SEND_PARK_NUM;									//队头偏移1
		memcpy(dataBuf, NETCoapMessageSendPark.Park[front].Buffer, NETCoapMessageSendPark.Park[front].Length);
		*dataLength = NETCoapMessageSendPark.Park[front].Length;
		MessageState = true;
	}
	
	return MessageState;
#endif
	
#if NETFIFOMESSAGETYPE == NETFIFOMESSAGEENABLE
#ifdef COAP_MSG_DBUG
	MessageState = netMessageFifoDequeue(&NETCoapFifoMessageSendPark, dataBuf, dataLength);
	if (MessageState) NET_Coap_Message_Debug("CoAP", "SP", "Out", *dataLength);
	return MessageState;
#else
	return netMessageFifoDequeue(&NETCoapFifoMessageSendPark, dataBuf, dataLength);
#endif
#endif
}

/**********************************************************************************************************
 @Function			bool NET_Coap_Message_RecvDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
 @Description			NET_Coap_Message_RecvDataDequeue	: 接收数据读出队列
 @Input				dataBuf	 		 			: 需读出数据地址
					dataLength					: 需读出数据长度地址
 @Return				true							: 未空
					false						: 已空
**********************************************************************************************************/
bool NET_Coap_Message_RecvDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
{
#ifdef COAP_MSG_DBUG
	bool MessageState;
#endif
	
#if NETFIFOMESSAGETYPE == NETFIFOMESSAGEDISABLE
	bool MessageState;
	unsigned char front;
	
	if (NET_Coap_Message_RecvDataisEmpty() == true) {													//队列已空
		MessageState = false;
	}
	else {																					//队列未空
		front = (NETCoapMessageRecvPark.Front + 1) % COAP_RECV_PARK_NUM;									//队头偏移1
		memcpy(dataBuf, NETCoapMessageRecvPark.Park[front].Buffer, NETCoapMessageRecvPark.Park[front].Length);
		*dataLength = NETCoapMessageRecvPark.Park[front].Length;
		MessageState = true;
	}
	
	return MessageState;
#endif
	
#if NETFIFOMESSAGETYPE == NETFIFOMESSAGEENABLE
#ifdef COAP_MSG_DBUG
	MessageState = netMessageFifoDequeue(&NETCoapFifoMessageRecvPark, dataBuf, dataLength);
	if (MessageState) NET_Coap_Message_Debug("CoAP", "RP", "Out", *dataLength);
	return MessageState;
#else
	return netMessageFifoDequeue(&NETCoapFifoMessageRecvPark, dataBuf, dataLength);
#endif
#endif
}

/**********************************************************************************************************
 @Function			bool NET_Coap_Message_SendDataOffSet(void)
 @Description			NET_Coap_Message_SendDataOffSet	: 发送数据队列(队列头偏移1)
 @Input				void
 @Return				true							: 未空
					false						: 已空
**********************************************************************************************************/
bool NET_Coap_Message_SendDataOffSet(void)
{
#if NETFIFOMESSAGETYPE == NETFIFOMESSAGEDISABLE
	bool MessageState;
	
	if (NET_Coap_Message_SendDataisEmpty() == true) {													//队列已空
		MessageState = false;
	}
	else {																					//队列未空
		NETCoapMessageSendPark.Front = (NETCoapMessageSendPark.Front + 1) % COAP_SEND_PARK_NUM;
		MessageState = true;
	}
	
	return MessageState;
#endif
	
#if NETFIFOMESSAGETYPE == NETFIFOMESSAGEENABLE
	return netMessageFifoDiscard(&NETCoapFifoMessageSendPark);
#endif
}

/**********************************************************************************************************
 @Function			bool NET_Coap_Message_RecvDataOffSet(void)
 @Description			NET_Coap_Message_RecvDataOffSet	: 接收数据队列(队列头偏移1)
 @Input				void
 @Return				true							: 未空
					false						: 已空
**********************************************************************************************************/
bool NET_Coap_Message_RecvDataOffSet(void)
{
#if NETFIFOMESSAGETYPE == NETFIFOMESSAGEDISABLE
	bool MessageState;
	
	if (NET_Coap_Message_RecvDataisEmpty() == true) {													//队列已空
		MessageState = false;
	}
	else {																					//队列未空
		NETCoapMessageRecvPark.Front = (NETCoapMessageRecvPark.Front + 1) % COAP_RECV_PARK_NUM;
		MessageState = true;
	}
	
	return MessageState;
#endif
	
#if NETFIFOMESSAGETYPE == NETFIFOMESSAGEENABLE
	return netMessageFifoDiscard(&NETCoapFifoMessageRecvPark);
#endif
}

/**********************************************************************************************************
 @Function			unsigned char NET_Coap_Message_SendDataRear(void)
 @Description			NET_Coap_Message_SendDataRear		: 发送数据队尾值
 @Input				void
 @Return				发送数据队尾值
**********************************************************************************************************/
unsigned char NET_Coap_Message_SendDataRear(void)
{
#if NETFIFOMESSAGETYPE == NETFIFOMESSAGEDISABLE
	return NETCoapMessageSendPark.Rear;
#endif
	
#if NETFIFOMESSAGETYPE == NETFIFOMESSAGEENABLE
	return netMessageFifoRear(&NETCoapFifoMessageSendPark);
#endif
}

/**********************************************************************************************************
 @Function			unsigned char NET_Coap_Message_RecvDataRear(void)
 @Description			NET_Coap_Message_SendDataRear		: 接收数据队尾值
 @Input				void
 @Return				接收数据队尾值
**********************************************************************************************************/
unsigned char NET_Coap_Message_RecvDataRear(void)
{
#if NETFIFOMESSAGETYPE == NETFIFOMESSAGEDISABLE
	return NETCoapMessageRecvPark.Rear;
#endif
	
#if NETFIFOMESSAGETYPE == NETFIFOMESSAGEENABLE
	return netMessageFifoRear(&NETCoapFifoMessageRecvPark);
#endif
}

/********************************************** END OF FLEE **********************************************/
