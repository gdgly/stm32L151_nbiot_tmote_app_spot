#ifndef __NET_ONENET_MESSAGE_OPERATE_H
#define   __NET_ONENET_MESSAGE_OPERATE_H

#include "sys.h"

#define ONENET_STATUS_MSG_VERSION_33BYTE_V1	0
#define ONENET_STATUS_MSG_VERSION_77BYTE_V2	1
#define ONENET_STATUS_MSG_VERSION_TYPE		ONENET_STATUS_MSG_VERSION_77BYTE_V2	//ONENET状态包版本

#define ONENET_MSGTYPE_TYPE_SHORT_STATUS	0x37
#define ONENET_MSGTYPE_TYPE_LONG_STATUS		0x3A
#define ONENET_MSGTYPE_TYPE_INFO			0x35
#define ONENET_MSGTYPE_TYPE_WORK_INFO		0x35
#define ONENET_MSGTYPE_TYPE_BASIC_INFO		0x3C
#define ONENET_MSGTYPE_TYPE_DYNAMIC_INFO	0x3D
#define ONENET_MSGTYPE_TYPE_QMCDATA_INFO	0x3F

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
}ONENET_PacketHeadTypeDef;												//数据包头8Byte

typedef __packed struct
{
	u32	DestSN;
	u8	Version;
	u8	Type;
}ONENET_PacketMsgTypeDef;												//消息包头6Byte

typedef __packed struct
{
	ONENET_PacketHeadTypeDef HeadPacket;
	ONENET_PacketMsgTypeDef MsgPacket;
	u32	DateTime;
	u16	SpotCount;
	u8	SpotStatus;
}ONENET_PacketShortTypeDef;												//短包21Byte

typedef __packed struct
{
	ONENET_PacketHeadTypeDef HeadPacket;
	ONENET_PacketMsgTypeDef MsgPacket;
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
#if ONENET_STATUS_MSG_VERSION_TYPE == ONENET_STATUS_MSG_VERSION_77BYTE_V2
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
}ONENET_PacketLongTypeDef;												//长包77Byte

typedef __packed struct
{
	ONENET_PacketHeadTypeDef HeadPacket;
	ONENET_PacketMsgTypeDef MsgPacket;
	u8	InfoData[450];
}ONENET_PacketInfoTypeDef;												//Info包464Byte

#define ONENET_SEND_BUFFER_SIZE			320
#define ONENET_RECV_BUFFER_SIZE			128
#define ONENET_SEND_PARK_NUM				10
#define ONENET_RECV_PARK_NUM				3

/* 发送数据队列 */
typedef struct
{
	unsigned char						Front;
	unsigned char						Rear;
	
	struct OneNETSendParkTypeDef
	{
		unsigned char					Buffer[ONENET_SEND_BUFFER_SIZE];
		unsigned short					Length;
	}Park[ONENET_SEND_PARK_NUM];
}ONENET_SwapSendDataTypeDef;

/* 接收数据队列 */
typedef struct
{
	unsigned char						Front;
	unsigned char						Rear;
	
	struct OneNETRecvParkTypeDef
	{
		unsigned char					Buffer[ONENET_RECV_BUFFER_SIZE];
		unsigned short					Length;
	}Park[ONENET_RECV_PARK_NUM];
}ONENET_SwapRecvDataTypeDef;


int NET_ONENET_Message_Operate_Creat_Json_Work_Info(char* outBuffer);
int NET_ONENET_Message_Operate_Creat_Json_Basic_Info(char* outBuffer);
int NET_ONENET_Message_Operate_Creat_Json_Dynamic_Info(char* outBuffer);
int NET_ONENET_Message_Operate_Creat_Json_Response_Info(char* outBuffer, u16 errcode, char msgId);

void NET_OneNET_FifoSendMessageInit(void);																//发送数据Fifo初始化
void NET_OneNET_FifoRecvMessageInit(void);																//接收数据Fifo初始化
bool NET_OneNET_Message_SendDataisEmpty(void);															//检查发送队列是否已空
bool NET_OneNET_Message_RecvDataisEmpty(void);															//检查接收队列是否已空
void NET_OneNET_Message_SendDataEnqueue(unsigned char* dataBuf, unsigned short dataLength);						//发送数据写入队列
void NET_OneNET_Message_RecvDataEnqueue(unsigned char* dataBuf, unsigned short dataLength);						//接收数据写入队列
bool NET_OneNET_Message_SendDataDequeue(unsigned char* dataBuf, unsigned short* dataLength);						//发送数据读出队列
bool NET_OneNET_Message_RecvDataDequeue(unsigned char* dataBuf, unsigned short* dataLength);						//接收数据读出队列
bool NET_OneNET_Message_SendDataOffSet(void);															//发送数据队列(队列头偏移1)
bool NET_OneNET_Message_RecvDataOffSet(void);															//接收数据队列(队列头偏移1)
unsigned char NET_OneNET_Message_SendDataRear(void);														//发送数据队尾值
unsigned char NET_OneNET_Message_RecvDataRear(void);														//接收数据队尾值

#endif /* __NET_ONENET_MESSAGE_OPERATE_H */
