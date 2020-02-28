#ifndef __NET_CTWING_MESSAGE_OPERATE_H
#define   __NET_CTWING_MESSAGE_OPERATE_H

#include "sys.h"

//#define CTWING_MSG_DBUG
#define CTWING_MSG_DBUG_PRINTF			Radio_Trf_Debug_Printf_Level2

#define CTWING_STATUS_MSG_VERSION_33BYTE_V1	0
#define CTWING_STATUS_MSG_VERSION_77BYTE_V2	1
#define CTWING_STATUS_MSG_VERSION_TYPE		CTWING_STATUS_MSG_VERSION_77BYTE_V2	//CTWING状态包版本

#define CTWING_MSGTYPE_TYPE_SHORT_STATUS	0x37
#define CTWING_MSGTYPE_TYPE_LONG_STATUS		0x3A
#define CTWING_MSGTYPE_TYPE_INFO			0x35
#define CTWING_MSGTYPE_TYPE_WORK_INFO		0x35
#define CTWING_MSGTYPE_TYPE_BASIC_INFO		0x3C
#define CTWING_MSGTYPE_TYPE_DYNAMIC_INFO	0x3D
#define CTWING_MSGTYPE_TYPE_QMCDATA_INFO	0x3F

typedef __packed struct
{
	u8	DataLen;
	u8	ProtocolType:3;
	u8	Reserved1:2;
	u8	ProtocolVersion:3;
	u8	Reserved2:4;
	u8	PacketType:4;
	u8	PacketNumber;
	u32	DeviceSN;
}CTWING_PacketHeadTypeDef;												//数据包头8Byte

typedef __packed struct
{
	u32	DestSN;
	u8	Version;
	u8	Type;
}CTWING_PacketMsgTypeDef;												//消息包头6Byte

typedef __packed struct
{
	CTWING_PacketHeadTypeDef HeadPacket;
	CTWING_PacketMsgTypeDef MsgPacket;
	u32	DateTime;
	u16	SpotCount;
	u8	SpotStatus;
}CTWING_PacketShortTypeDef;												//短包21Byte

typedef __packed struct
{
	CTWING_PacketHeadTypeDef HeadPacket;
	CTWING_PacketMsgTypeDef MsgPacket;
	u32	DateTime;
	u16	SpotCount;
	u8	SpotStatus;
	u16	MagneticX;
	u16	MagneticY;
	u16	MagneticZ;
	u16	MagneticDiff;
	u8	RadarDistance;
	u8	RadarStrength;
	u8	RadarCoverCount;
	u8	RadarDiff;
#if CTWING_STATUS_MSG_VERSION_TYPE == CTWING_STATUS_MSG_VERSION_77BYTE_V2
	u8	NBRssi;
	s8	NBSnr;
	s8	MCUTemp;
	s8	QMCTemp;
	s16	MagneticBackX;
	s16	MagneticBackY;
	s16	MagneticBackZ;
	s16	Debugval;
	u8	Radarval[16];
	u8	Radarback[16];
#endif
}CTWING_PacketLongTypeDef;												//长包77Byte

typedef __packed struct
{
	CTWING_PacketHeadTypeDef HeadPacket;
	CTWING_PacketMsgTypeDef MsgPacket;
	u8	InfoData[450];
}CTWING_PacketInfoTypeDef;												//Info包464Byte

#define CTWING_SEND_BUFFER_SIZE			320
#define CTWING_RECV_BUFFER_SIZE			128
#define CTWING_SEND_PARK_NUM				10
#define CTWING_RECV_PARK_NUM				3

/* 发送数据队列 */
typedef struct
{
	unsigned char						Front;
	unsigned char						Rear;
	
	struct CTWingSendParkTypeDef
	{
		unsigned char					Buffer[CTWING_SEND_BUFFER_SIZE];
		unsigned short					Length;
	}Park[CTWING_SEND_PARK_NUM];
}CTWING_SwapSendDataTypeDef;

/* 接收数据队列 */
typedef struct
{
	unsigned char						Front;
	unsigned char						Rear;
	
	struct CTWingRecvParkTypeDef
	{
		unsigned char					Buffer[CTWING_RECV_BUFFER_SIZE];
		unsigned short					Length;
	}Park[CTWING_RECV_PARK_NUM];
}CTWING_SwapRecvDataTypeDef;


int NET_CTWING_Message_Operate_Creat_Json_Work_Info(char* outBuffer);
int NET_CTWING_Message_Operate_Creat_Json_Basic_Info(char* outBuffer);
int NET_CTWING_Message_Operate_Creat_Json_Dynamic_Info(char* outBuffer);
int NET_CTWING_Message_Operate_Creat_Json_Response_Info(char* outBuffer, u16 errcode, char msgId);

void NET_CTWing_FifoSendMessageInit(void);																//发送数据Fifo初始化
void NET_CTWing_FifoRecvMessageInit(void);																//接收数据Fifo初始化
bool NET_CTWing_Message_SendDataisEmpty(void);															//检查发送队列是否已空
bool NET_CTWing_Message_RecvDataisEmpty(void);															//检查接收队列是否已空
void NET_CTWing_Message_SendDataEnqueue(unsigned char* dataBuf, unsigned short dataLength);						//发送数据写入队列
void NET_CTWing_Message_RecvDataEnqueue(unsigned char* dataBuf, unsigned short dataLength);						//接收数据写入队列
bool NET_CTWing_Message_SendDataDequeue(unsigned char* dataBuf, unsigned short* dataLength);						//发送数据读出队列
bool NET_CTWing_Message_RecvDataDequeue(unsigned char* dataBuf, unsigned short* dataLength);						//接收数据读出队列
bool NET_CTWing_Message_SendDataOffSet(void);															//发送数据队列(队列头偏移1)
bool NET_CTWing_Message_RecvDataOffSet(void);															//接收数据队列(队列头偏移1)
unsigned char NET_CTWing_Message_SendDataRear(void);														//发送数据队尾值
unsigned char NET_CTWing_Message_RecvDataRear(void);														//接收数据队尾值

#endif /* __NET_CTWING_MESSAGE_OPERATE_H */
