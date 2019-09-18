#ifndef __NET_MQTTSN_MESSAGE_OPERATE_H
#define   __NET_MQTTSN_MESSAGE_OPERATE_H

#include "sys.h"

#define MQTTSN_STATUS_MSG_VERSION_V1		0
#define MQTTSN_STATUS_MSG_VERSION_V2		1
#define MQTTSN_STATUS_MSG_VERSION_TYPE		MQTTSN_STATUS_MSG_VERSION_V2									//MQTTSN状态包版本

#define MQTTSN_MSGTYPE_TYPE_SHORT_STATUS	0x37
#define MQTTSN_MSGTYPE_TYPE_LONG_STATUS		0x3A
#define MQTTSN_MSGTYPE_TYPE_INFO			0x35
#define MQTTSN_MSGTYPE_TYPE_WORK_INFO		0x35
#define MQTTSN_MSGTYPE_TYPE_BASIC_INFO		0x3C
#define MQTTSN_MSGTYPE_TYPE_DYNAMIC_INFO	0x3D
#define MQTTSN_MSGTYPE_TYPE_QMC_DATA		0x3F

#define NET_MESSAGE_GET_MAGICNUM(Byte)		{Byte[0]='T';Byte[1]='C';Byte[2]='L';Byte[3]='D';}
#define MESSAGE_VERSION					'1'

typedef __packed struct
{
	u8								DataLen;
	u8								ProtocolType:3;
	u8								Reserved1:2;
	u8								ProtocolVersion:3;
	u8								Reserved2:4;
	u8								PacketType:4;
	u8								PacketNumber;
	u32								DeviceSN;
}MQTTSN_PacketHeadTypeDef;																		//数据包头8Byte

typedef __packed struct
{
	u32								DestSN;
	u8								Version;
	u8								Type;
}MQTTSN_PacketMsgTypeDef;																		//消息包头6Byte

typedef __packed struct
{
	MQTTSN_PacketHeadTypeDef				HeadPacket;
	MQTTSN_PacketMsgTypeDef				MsgPacket;
	u32								DateTime;
	u16								SpotCount;
	u8								SpotStatus;
}MQTTSN_PacketShortTypeDef;																		//短包21Byte

typedef __packed struct
{
	MQTTSN_PacketHeadTypeDef				HeadPacket;
	MQTTSN_PacketMsgTypeDef				MsgPacket;
	u32								DateTime;
	u16								SpotCount;
	u8								SpotStatus;
	u16								MagneticX;
	u16								MagneticY;
	u16								MagneticZ;
	u16								MagneticDiff;
	u8								RadarDistance;
	u8								RadarStrength;
	u8								RadarCoverCount;
	u8								RadarDiff;
#if MQTTSN_STATUS_MSG_VERSION_TYPE == MQTTSN_STATUS_MSG_VERSION_V2
	u8								NBRssi;
	s8								NBSnr;
	s8								MCUTemp;
	s8								QMCTemp;
	s16								MagneticBackX;
	s16								MagneticBackY;
	s16								MagneticBackZ;
	s16								Debugval;
	u8								Radarval[16];
	u8								Radarback[16];
#endif
}MQTTSN_PacketLongTypeDef;																		//长包77Byte

typedef __packed struct
{
	MQTTSN_PacketHeadTypeDef				HeadPacket;
	MQTTSN_PacketMsgTypeDef				MsgPacket;
	u8								InfoData[450];
}MQTTSN_PacketInfoTypeDef;																		//Info包464Byte

#define MQTTSN_SEND_BUFFER_SIZE			320
#define MQTTSN_SEND_PARK_NUM				10

/* 发送数据队列 */
typedef struct
{
	unsigned char						Front;
	unsigned char						Rear;
	
	struct MqttSNSendParkTypeDef
	{
		unsigned char					Buffer[MQTTSN_SEND_BUFFER_SIZE];
		unsigned short					Length;
	}Park[MQTTSN_SEND_PARK_NUM];
}MQTTSN_SwapSendDataTypeDef;

int NET_MQTTSN_Message_Operate_Creat_Json_Work_Info(char* outBuffer);
int NET_MQTTSN_Message_Operate_Creat_Json_Basic_Info(char* outBuffer);
int NET_MQTTSN_Message_Operate_Creat_Json_Dynamic_Info(char* outBuffer);
int NET_MQTTSN_Message_Operate_Creat_Json_Response_Info(char* outBuffer, u16 errcode, char msgId);
int NET_MQTTSN_Message_Operate_Creat_Qmc5883L_Data(unsigned char* outBuffer);

void NET_MqttSN_FifoSendMessageInit(void);															//发送数据Fifo初始化
bool NET_MqttSN_Message_SendDataisEmpty(void);														//检查发送队列是否已空
void NET_MqttSN_Message_SendDataEnqueue(unsigned char* dataBuf, unsigned short dataLength);					//发送数据写入队列
bool NET_MqttSN_Message_SendDataDequeue(unsigned char* dataBuf, unsigned short* dataLength);					//发送数据读出队列
bool NET_MqttSN_Message_SendDataOffSet(void);														//发送数据队列(队列头偏移1)
unsigned char NET_MqttSN_Message_SendDataRear(void);													//发送数据队尾值

#endif /* __NET_MQTTSN_MESSAGE_OPERATE_H */
