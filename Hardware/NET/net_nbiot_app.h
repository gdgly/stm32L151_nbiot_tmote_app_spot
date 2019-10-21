#ifndef __NET_NBIOT_APP_H
#define   __NET_NBIOT_APP_H

#include "sys.h"

#define NETCoapNeedSendCode_initializer		{ 0, 0, 1, 0, 1, 1, 0, NETIP_OK, 0 }
#define NETMqttSNNeedSendCode_initializer	{ 0, 0, 1, 0, 1, 1, 0, NETIP_OK, 0 }
#define NETOneNETNeedSendCode_initializer	{ 0, 0, 1, 0, 1, 1, 0, NETIP_OK, 0 }
#define NETCTWingNeedSendCode_initializer	{ 0, 0, 1, 0, 1, 1, 0, NETIP_OK, 0 }

typedef struct NET_NBIOT_ClientsTypeDef		NET_NBIOT_ClientsTypeDef;

typedef struct
{
	char								ShortStatus;
	char								LongStatus;
	char								WorkInfo;
	char								WorkInfoWait;
	char								BasicInfo;
	char								DynamicInfo;
	char								ResponseInfo;
	u16								ResponseInfoErrcode;
	char								ResponseInfoMsgId;
}NETCoapNeedSendCodeTypeDef;

extern NETCoapNeedSendCodeTypeDef NETCoapNeedSendCode;

typedef struct
{
	char								StatusBasic;
	char								StatusExtend;
	char								InfoWork;
	char								InfoWorkWait;
	char								InfoBasic;
	char								InfoDynamic;
	char								InfoResponse;
	u16								InfoResponseErrcode;
	char								InfoResponseMsgId;
}NETMqttSNNeedSendCodeTypeDef;

extern NETMqttSNNeedSendCodeTypeDef NETMqttSNNeedSendCode;

typedef struct
{
	char								ShortStatus;
	char								LongStatus;
	char								WorkInfo;
	char								WorkInfoWait;
	char								BasicInfo;
	char								DynamicInfo;
	char								ResponseInfo;
	u16								ResponseInfoErrcode;
	char								ResponseInfoMsgId;
}NETOneNETNeedSendCodeTypeDef;

extern NETOneNETNeedSendCodeTypeDef NETOneNETNeedSendCode;

typedef struct
{
	char								ShortStatus;
	char								LongStatus;
	char								WorkInfo;
	char								WorkInfoWait;
	char								BasicInfo;
	char								DynamicInfo;
	char								ResponseInfo;
	u16								ResponseInfoErrcode;
	char								ResponseInfoMsgId;
}NETCTWingNeedSendCodeTypeDef;

extern NETCTWingNeedSendCodeTypeDef NETCTWingNeedSendCode;

/* NET Is PollExecution */
typedef enum
{
	NET_POLL_EXECUTION_COAP				= 0x00,
	NET_POLL_EXECUTION_DNS				= 0x01,
	NET_POLL_EXECUTION_MQTTSN			= 0x02,
	NET_POLL_EXECUTION_PCP				= 0x03,
	NET_POLL_EXECUTION_ONENET			= 0x04,
	NET_POLL_EXECUTION_CTWING			= 0x05
}NET_NBIOT_PollExecutionTypeDef;

/* NET Clients */
struct NET_NBIOT_ClientsTypeDef
{
	NET_NBIOT_PollExecutionTypeDef		PollExecution;
};

void NET_NBIOT_Client_Init(NET_NBIOT_ClientsTypeDef* pClient);					//初始化NET NBIOT客户端

void NET_NBIOT_Initialization(void);										//NET初始化
void NET_NBIOT_FIFOMessage_Initialization(void);								//NET数据缓存初始化
void NET_NBIOT_DataProcessing(NET_NBIOT_ClientsTypeDef* pClient);				//NET数据处理
void NET_NBIOT_TaskProcessing(NET_NBIOT_ClientsTypeDef* pClient);				//NET工作处理
void NET_NBIOT_App_Task(void);											//NET处理
void NET_NBIOT_BackupCurrentApp_Task(void);									//NET备份固件

#endif /* __NET_NBIOT_APP_H */
