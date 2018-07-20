#ifndef __PLATFORM_CONFIG_H
#define   __PLATFORM_CONFIG_H

#include "sys.h"
#include "net_coap_app.h"
#include "net_pcp_app.h"
#include "net_mqttsn_app.h"
#include "net_dns_app.h"

//#define	MVB_SUBSN						0x81011000						//设备号
//#define	MVB_BRAND						"mvb"							//厂牌

#define	MVB_STATICMAGNETIC_MODEL			51
#define	MVB_FLOWMAGNETIC_MODEL			52
#define	MVB_SPOT_LOCKER				53
#define	MVB_MODEL_TYPE					MVB_STATICMAGNETIC_MODEL				//设备类型

#define	SYSTEMCLOCKMSI					4194000
#define	SYSTEMCLOCKHSI					32000000
#define	SYSTEMCLOCK					SYSTEMCLOCKMSI						//系统时钟

#define	NETCOAP						0
#define	NETMQTTSN						1
#define	NETPROTOCAL					NETCOAP							//网络模式

#define	NBCOAP_SENDMODE_NORMAL			SEND_DATA
#define	NBCOAP_SENDMODE_RAIDLE			SEND_DATA_RA_NORMAL
#define	NBCOAP_SENDMODE_TYPE			NBCOAP_SENDMODE_RAIDLE				//COAP发送模式

#define	NBCOAP_QUOTA_TIME_3YEAR			1972
#define	NBCOAP_QUOTA_TIME_5YEAR			1183
#define	NBCOAP_COAP_QUOTA_TIME_TYPE		NBCOAP_QUOTA_TIME_3YEAR				//Coap一天使用配额时间

#define	NBIOT_PRINT_ERROR_CODE_DISABLE	0
#define	NBIOT_PRINT_ERROR_CODE_ENABLE		1
#define	NBIOT_PRINT_ERROR_CODE_TYPE		NBIOT_PRINT_ERROR_CODE_ENABLE			//NBIOT输出错误码模式

#define	LOWPOWERDISABLE				0
#define	LOWPOWERENABLE					1
#define	LOWPOWERMODE					LOWPOWERENABLE						//低功耗模式

#define	RADAR_MODEL_V1					1
#define	RADAR_MODEL_V2					2
#define	RADAR_MODEL_TYPE				RADAR_MODEL_V2						//雷达类型

#define	SOFTWAREMAJOR					20								//主固件版本
#define	SOFTWARESUB					103								//从固件版本
#define	HARDWAREMAJOR_V1				2								//主硬件版本
#define	HARDWAREMAJOR_V2				12								//主硬件版本

#define	RADIO_SI4438													//无线开启
#define	RF_DPRINT_LV					2								//无线调试信息输出等级

#define	IDLE_WORK						4								//休眠
#define	NOTACTIVE_WORK					3								//不工作

#define	DEBUG_WORK					1								//调试工作模式
#define	NORMAL_WORK					0								//正常工作模式

#define	TIME_TO_MODULE_INIT				3600*24*3							//间隔时间初始化模块

#define	NBIOT_HEART_DATA_HOURS			4								//NB心跳数据包时间

#define	DNS_SERVER_HOST_IP				"114.114.114.114"					//DNS服务器默认配置
#define	DNS_SERVER_LOCAL_PORT			5000
#define	DNS_SERVER_TELE_PORT			53

#define	MQTTSN_SERVER_HOST_NAME			"mqtt.movebroad.cn"					//MQTTSN服务器默认配置
#define	MQTTSN_SERVER_HOST_IP			"47.98.140.145"
#define	MQTTSN_SERVER_LOCAL_PORT			4000
#define	MQTTSN_SERVER_TELE_PORT			1884

extern bool BootUp;														//BootUp
extern bool DeviceIdleMode;												//IDLE MODE
extern bool DeviceActivedMode;											//ACTIVED MODE

extern COAP_PacketShortTypeDef			CoapShortStructure;					//Coap Short Packet
extern COAP_PacketLongTypeDef				CoapLongStructure;					//Coap Long Packet
extern COAP_PacketInfoTypeDef				CoapInfoStructure;					//Coap Info Packet

extern MQTTSN_StatusBasicTypeDef			MqttSNStatusBasicStructure;			//MqttSN Basic Status Packet
extern MQTTSN_StatusExtendTypeDef			MqttSNStatusExtendStructure;			//MqttSN Extend Status Packet
extern MQTTSN_InfoWorkTypeDef				MqttSNInfoWorkStructure;				//MqttSN Work Info Packet
extern MQTTSN_InfoBasicTypeDef			MqttSNInfoBasicStructure;			//MqttSN Basic Info Packet
extern MQTTSN_InfoDynamicTypeDef			MqttSNInfoDynamicStructure;			//MqttSN Dynamic Info Packet
extern MQTTSN_InfoRadarTypeDef			MqttSNInfoRadarStructure;			//MqttSN Radar Info Packet
extern MQTTSN_InfoResponseTypeDef			MqttSNInfoResponseStructure;			//MqttSN Response Info Packet

extern NET_NBIOT_ClientsTypeDef			NetNbiotClientHandler;				//NET NBIOT Clinet Handler
extern NBIOT_ATCmdTypeDef				NbiotATCmdHandler;					//NBIOT AT Cmd Handler
extern NBIOT_ClientsTypeDef				NbiotClientHandler;					//Coap Clinet Handler
extern PCP_CoAPNetTransportTypeDef			PCPCoAPNetHandler;					//PCP Net Handler
extern PCP_ClientsTypeDef				PCPClientHandler;					//PCP Clinet Handler
extern MQTTSN_SocketNetTypeDef			MqttSNSocketNetHandler;				//MqttSN Net Handler
extern MQTTSN_ClientsTypeDef				MqttSNClientHandler;				//MqttSN Clinet Handler
extern DNS_SocketNetTypeDef				DNSSocketNetHandler;				//DNS Net Handler
extern DNS_ClientsTypeDef				DNSClientHandler;					//DNS Clinet Handler










#endif
