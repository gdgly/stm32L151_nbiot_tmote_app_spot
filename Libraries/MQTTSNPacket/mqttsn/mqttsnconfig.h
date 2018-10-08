#ifndef __MQTTSN_CONFIG_H
#define   __MQTTSN_CONFIG_H

#include "sys.h"
#include "MQTTSNPacket.h"
#include "nbiotconfig.h"
#include "net_nbiot_app.h"

#define MAX_MSG_ID						65535											/* according to the MQTT specification - do not change! */
#define MAX_MESSAGE_HANDLERS				5												/* redefinable - how many subscriptions do you want? */

#define MQTTSN_COMMAND_TIMEOUT_SEC			30
#define MQTTSN_COMMAND_FAILURE_CNT			3

/* MQTTSN 协议栈开辟缓存大小 */
#define MQTTSN_BUFFER_SIZE				520
#define MQTTSN_DATASTACK_SIZE				520

/* MQTTSN 协议工作时间 */
#define MQTTSN_EVENT_ACTIVE_DURATION		60												//Active	00:01:00
#define MQTTSN_EVENT_SLEEP_DURATION		28800											//Sleep	08:00:00
#define MQTTSN_EVENT_PINGREG_DURATION		21600											//Ping	06:00:00
#define MQTTSN_EVENT_LOST_DURATION			300												//Lost	00:05:00

/* MQTTSN 事件监听器配置 */
#define NBMQTTSN_LISTEN_MODE_ENTER_NONE		MQTTSN_ENTER_NONE
#define NBMQTTSN_LISTEN_MODE_ENTER_PARAMETER	MQTTSN_ENTER_PARAMETER_CHECKOUT
#define NBMQTTSN_LISTEN_DEFAULT_BOOTMODE	NBMQTTSN_LISTEN_MODE_ENTER_PARAMETER					//MqttSN监听NB默认起始模式

#define NBMQTTSN_LISTEN_ENTER_PARAMETER_SEC	45												//MqttSN监听NB进入参数检查等待时间

#define NBMQTTSN_LISTEN_PARAMETER_DISABLE	0
#define NBMQTTSN_LISTEN_PARAMETER_ENABLE	1
#define NBMQTTSN_LISTEN_PARAMETER_TYPE		NBMQTTSN_LISTEN_PARAMETER_ENABLE						//MqttSN监听NB进入参数检查模式

typedef struct MQTTSN_SocketNetTypeDef		MQTTSN_SocketNetTypeDef;
typedef struct MQTTSN_MessageTypeDef		MQTTSN_MessageTypeDef;
typedef struct MQTTSN_MessageDataTypeDef	MQTTSN_MessageDataTypeDef;
typedef struct MQTTSN_ClientsTypeDef		MQTTSN_ClientsTypeDef;

/* MQTTSN QoS */
enum QoS
{
	QOS0,
	QOS1,
	QOS2
};

/* MQTTSN Status */
typedef enum
{
	MQTTSN_OK       					= 0x00,
	MQTTSN_ERROR    					= 0x01,
	MQTTSN_BUFFER_OVERFLOW				= 0x02,
	MQTTSN_RECVPACKET_NONE				= 0x03,
	MQTTSN_CMD_TIMEOUT					= 0x04,
	MQTTSN_MESSAGE_HANDLERS_FULL			= 0x05
}MQTTSN_StatusTypeDef;

/* MQTTSN Is SubState */
typedef enum
{
	MQTTSN_SUBSTATE_INIT				= 0x00,
	MQTTSN_SUBSTATE_DISCONNECT			= 0x01,
	MQTTSN_SUBSTATE_ACTIVE				= 0x02,
	MQTTSN_SUBSTATE_SLEEP				= 0x03,
	MQTTSN_SUBSTATE_AWAKE				= 0x04,
	MQTTSN_SUBSTATE_LOST				= 0x05
}MQTTSN_SubStateTypeDef;

/* MQTTSN Listen Event */
typedef enum
{
	MQTTSN_ENTER_NONE					= 0x00,											//无监听
	MQTTSN_ENTER_PARAMETER_CHECKOUT		= 0x01											//进入NBIOT运行信息监听
}MQTTSN_ListenEventTypeDef;

/* MQTTSN Socket */
struct MQTTSN_SocketNetTypeDef
{
	int								SocketHandler;
	unsigned short						LocalPort;
	unsigned short						ServerPort;
	unsigned char						ServerHost[16];
	NBIOT_ClientsTypeDef*				NBIotStack;
	MQTTSN_StatusTypeDef				(*Open)(MQTTSN_SocketNetTypeDef*);
	MQTTSN_StatusTypeDef				(*Close)(MQTTSN_SocketNetTypeDef*);
	MQTTSN_StatusTypeDef				(*Write)(MQTTSN_SocketNetTypeDef*, const char*, u16);
	MQTTSN_StatusTypeDef				(*Read)(MQTTSN_SocketNetTypeDef*, char*, u16, int*, int*);
};

/* MQTTSN Message */
struct MQTTSN_MessageTypeDef
{
	enum QoS 							qos;
	unsigned short						msgid;
	unsigned char						retained;
	unsigned char						dup;
	unsigned char*						payload;
	int								payloadlen;
};

/* MQTTSN MessageData */
struct MQTTSN_MessageDataTypeDef
{
	MQTTSN_MessageTypeDef*				message;
	MQTTSNString*						topicName;
};

/* MQTTSN Clients */
struct MQTTSN_ClientsTypeDef
{
	unsigned char*						Sendbuf;
	unsigned char*						Recvbuf;
	size_t							Sendbuf_size;
	size_t							Recvbuf_size;
	short							Sendlen;
	short							Recvlen;
	unsigned char*						DataProcessStack;
	size_t							DataProcessStack_size;
	unsigned int						MsgId;
	unsigned short						Command_Timeout_Sec;
	unsigned short						Command_Failure_Cnt;
	
	struct MQTTSNDictateRuningCtlTypeDef
	{
		bool							dictateEnable;
		unsigned int					dictateTimeoutSec;
		unsigned char					dictateInitFailureCnt;
		unsigned char					dictateDisconnectFailureCnt;
		unsigned char					dictateActiveFailureCnt;
		unsigned char					dictateSleepFailureCnt;
		unsigned char					dictateAweakFailureCnt;
		unsigned char					dictateLostFailureCnt;
		Stm32_CalculagraphTypeDef		dictateRunTime;
		bool							dictateSubscribeStatus;
	}DictateRunCtl;
	
	/* 事件运行监听器 */
	struct MQTTSNListenRuningCtlTypeDef
	{
#if NBMQTTSN_LISTEN_PARAMETER_TYPE == NBMQTTSN_LISTEN_PARAMETER_ENABLE
		struct MQTTSNListenEnterParameterTypeDef
		{
			bool						listenEnable;
			bool						listenStatus;
			unsigned int				listenTimereachSec;
			Stm32_CalculagraphTypeDef	listenRunTime;
			
			struct MQTTSNEventCtlParameterTypedef
			{
				bool						eventEnable;
				unsigned int				eventTimeoutSec;
				unsigned char				eventFailureCnt;
				Stm32_CalculagraphTypeDef	eventRunTime;
			}EventCtl;
		}ListenEnterParameter;
#endif
		MQTTSN_ListenEventTypeDef		listenEvent;
	}ListenRunCtl;
	
	struct MQTTSNMessageSendCtlTypeDef
	{
		bool							messageStatusBasic;
		bool							messageStatusExtend;
		bool							messageInfoWork;
		bool							messageInfoBasic;
		bool							messageInfoDynamic;
		bool							messageInfoResponse;
		bool							messageByteStream;
	}MessageSendCtl;
	
	struct MessageHandlersTypeDef
	{
		const char*					topicFilter;
		unsigned short					topicid;
		MQTTSN_StatusTypeDef			(*fp)(MQTTSN_ClientsTypeDef*, MQTTSN_MessageDataTypeDef*);
	}messageHandlers[MAX_MESSAGE_HANDLERS];
	
	MQTTSN_StatusTypeDef				(*defaultMessageHandler)(MQTTSN_ClientsTypeDef*, MQTTSN_MessageDataTypeDef*);
	
	MQTTSN_SubStateTypeDef				SubState;
	MQTTSN_SocketNetTypeDef*				SocketStack;
	NET_NBIOT_ClientsTypeDef*			NetNbiotStack;
	
	Stm32_CalculagraphTypeDef			PingTimer;
	Stm32_CalculagraphTypeDef			ActiveTimer;
};

typedef MQTTSN_StatusTypeDef (*messageHandlerFp)(MQTTSN_ClientsTypeDef*, MQTTSN_MessageDataTypeDef*);

/* Application Programming Interface */
void MQTTSN_WaitforCallback(MQTTSN_ClientsTypeDef* pClient);															//MQTTSN数据等待接收回调函数
void MQTTSN_Client_Init(MQTTSN_ClientsTypeDef* pClient, MQTTSN_SocketNetTypeDef* NetSock, NET_NBIOT_ClientsTypeDef* NetNbiotStack);	//MQTTSN客户端初始化

#endif /* __MQTTSN_CONFIG_H */
