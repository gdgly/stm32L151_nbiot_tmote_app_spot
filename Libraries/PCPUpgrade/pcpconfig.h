#ifndef __PCP_CONFIG_H
#define   __PCP_CONFIG_H

#include "sys.h"
#include "nbiotconfig.h"

typedef struct PCP_CoAPNetTransportTypeDef	PCP_CoAPNetTransportTypeDef;

/* PCP Status */
typedef enum
{
	PCP_OK       						= 0x00,
	PCP_ERROR    						= 0x01,
	PCP_CMD_TIMEOUT					= 0x02,
	PCP_RECVPACKET_NONE					= 0x03,
	PCP_BUFFER_OVERFLOW					= 0x04
}PCP_StatusTypeDef;

/* PCP Message Data Structure */
typedef __packed struct
{
	unsigned short						StartX;											//起始标识	0xFFFE
	unsigned char						ProtocolType;										//版本号		0x01
	unsigned char						MessageType;										//消息码		0xXX
	unsigned short						CRCCheckCode;										//检验码		CRC16
	unsigned short						PacketDataLength;									//数据区长度
	unsigned char						pPacketData[1];									//数据区
}PCP_MessageDataTypeDef;

/* PCP CoAP Transport */
struct PCP_CoAPNetTransportTypeDef
{
	NBIOT_ClientsTypeDef*				NBIotStack;
	
};








#endif /* __PCP_CONFIG_H */
