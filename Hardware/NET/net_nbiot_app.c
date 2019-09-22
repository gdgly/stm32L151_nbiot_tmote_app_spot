/**
  *********************************************************************************************************
  * @file    net_nbiot_app.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-04-26
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "net_nbiot_app.h"
#include "net_coap_app.h"
#include "net_pcp_app.h"
#include "net_dns_app.h"
#include "net_mqttsn_app.h"
#include "net_mqttsn_pcp_app.h"
#include "net_onenet_app.h"
#include "stm32l1xx_config.h"
#include "platform_config.h"
#include "platform_map.h"
#include "inspectmessageoperate.h"
#include "hal_rtc.h"
#include "radar_api.h"
#include "string.h"

NETCoapNeedSendCodeTypeDef	NETCoapNeedSendCode = NETCoapNeedSendCode_initializer;
NETMqttSNNeedSendCodeTypeDef	NETMqttSNNeedSendCode = NETMqttSNNeedSendCode_initializer;
NETOneNETNeedSendCodeTypeDef	NETOneNETNeedSendCode = NETOneNETNeedSendCode_initializer;

/**********************************************************************************************************
 @Function			void NET_NBIOT_FIFOMessage_Initialization(void)
 @Description			NET_NBIOT_FIFOMessage_Initialization			: NET数据缓存初始化
 @Input				void
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_FIFOMessage_Initialization(void)
{
#ifndef NETPROTOCAL
	#error No Define NETPROTOCAL!
#else
#if (NETPROTOCAL == NETCOAP)
	
	/* NBIOT发送数据FIFO初始化 */
	NET_Coap_FifoSendMessageInit();
	/* NBIOT接收数据FIFO初始化 */
	NET_Coap_FifoRecvMessageInit();
	
	/* PCP发送数据FIFO初始化 */
	NET_PCP_FifoSendMessageInit();
	/* PCP接收数据FIFO初始化 */
	NET_PCP_FifoRecvMessageInit();
	
#elif (NETPROTOCAL == NETMQTTSN)
	
	/* MqttSN发送数据FIFO初始化 */
	NET_MqttSN_FifoSendMessageInit();
	
	/* MQTTSN PCP发送数据FIFO初始化 */
	NET_MqttSN_PCP_FifoSendMessageInit();
	/* MQTTSN PCP接收数据FIFO初始化 */
	NET_MqttSN_PCP_FifoRecvMessageInit();
	
#elif (NETPROTOCAL == NETONENET)
	
	/* ONENET发送数据FIFO初始化 */
	NET_OneNET_FifoSendMessageInit();
	/* ONENET接收数据FIFO初始化 */
	NET_OneNET_FifoRecvMessageInit();
	
#else
	#error NETPROTOCAL Define Error
#endif
#endif
}

/**********************************************************************************************************
 @Function			void NET_NBIOT_Initialization(void)
 @Description			NET_NBIOT_Initialization						: NET初始化
 @Input				void
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_Initialization(void)
{
	/* NET NBIOT客户端初始化 */
	NET_NBIOT_Client_Init(&NetNbiotClientHandler);
	/* NET NBIOT数据缓存初始化 */
	NET_NBIOT_FIFOMessage_Initialization();
	
	/* NBIOT数据传输接口初始化 */
	NBIOT_Transport_Init(&NbiotATCmdHandler);
	/* NBIOT客户端初始化 */
	NBIOT_Client_Init(&NbiotClientHandler, &NbiotATCmdHandler, &NetNbiotClientHandler);
	
#ifndef NETPROTOCAL
	#error No Define NETPROTOCAL!
#else
#if (NETPROTOCAL == NETCOAP)
	
	/* PCP数据传输接口初始化 */
	PCP_Transport_Init(&PCPCoAPNetHandler, &NbiotClientHandler);
	/* PCP客户端初始化 */
	PCP_Client_Init(&PCPClientHandler, &PCPCoAPNetHandler, &NetNbiotClientHandler);
	
#elif (NETPROTOCAL == NETMQTTSN)
	
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_ENABLE
	/* DNS数据传输接口初始化 */
	DNS_Transport_Init(&DNSSocketNetHandler, &NbiotClientHandler, DNS_SERVER_LOCAL_PORT, DNS_SERVER_HOST_IP, DNS_SERVER_TELE_PORT);
	/* DNS客户端初始化 */
	DNS_Client_Init(&DNSClientHandler, &DNSSocketNetHandler, &NetNbiotClientHandler);
#endif
	
	/* MqttSN数据传输接口初始化 */
	MQTTSN_Transport_Init(&MqttSNSocketNetHandler, &NbiotClientHandler, MQTTSN_SERVER_LOCAL_PORT, MQTTSN_SERVER_HOST_IP, MQTTSN_SERVER_TELE_PORT);
	/* MQTTSN客户端初始化 */
	MQTTSN_Client_Init(&MqttSNClientHandler, &MqttSNSocketNetHandler, &NetNbiotClientHandler);
	
	/* MQTTSN PCP数据传输接口初始化 */
	MqttPCP_Transport_Init(&MqttSNPCPMqttNetHandler, &MqttSNClientHandler);
	/* MQTTSN PCP客户端初始化 */
	MqttPCP_Client_Init(&MqttSNPCPClientHandler, &MqttSNPCPMqttNetHandler, &NetNbiotClientHandler);
	
#elif (NETPROTOCAL == NETONENET)
	
	/* ONENET数据传输接口初始化 */
	ONENET_Transport_Init(&OneNETLWM2MNetHandler, &NbiotClientHandler);
	/* ONENET客户端初始化 */
	OneNET_Client_Init(&OneNETClientHandler, &OneNETLWM2MNetHandler, &NetNbiotClientHandler);
	
#else
	#error NETPROTOCAL Define Error
#endif
#endif
}

/**********************************************************************************************************
 @Function			void NET_NBIOT_Client_Init(NET_NBIOT_ClientsTypeDef* pClient)
 @Description			NET_NBIOT_Client_Init					: 初始化NET NBIOT客户端
 @Input				pClient								: NET NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_Client_Init(NET_NBIOT_ClientsTypeDef* pClient)
{
#if NETPROTOCAL == NETCOAP
	
	pClient->PollExecution								= NET_POLL_EXECUTION_COAP;
	
#elif NETPROTOCAL == NETMQTTSN
	
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_DISABLE
	pClient->PollExecution								= NET_POLL_EXECUTION_MQTTSN;
#endif
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_ENABLE
	pClient->PollExecution								= NET_POLL_EXECUTION_DNS;
#endif
	
#elif NETPROTOCAL == NETONENET
	
	pClient->PollExecution								= NET_POLL_EXECUTION_ONENET;
	
#endif
}

#if NETPROTOCAL == NETCOAP
void NET_NBIOT_CoapShortStructureInit(void)
{
	CoapShortStructure.HeadPacket.DeviceSN					= TCFG_EEPROM_Get_MAC_SN();
	CoapShortStructure.HeadPacket.DataLen					= 0x00;
	CoapShortStructure.HeadPacket.ProtocolType				= 0x00;
	CoapShortStructure.HeadPacket.Reserved1					= 0x00;
	CoapShortStructure.HeadPacket.ProtocolVersion			= 0x00;
	CoapShortStructure.HeadPacket.Reserved2					= 0x00;
	CoapShortStructure.HeadPacket.PacketType				= 0x05;
	CoapShortStructure.HeadPacket.PacketNumber				= 0x00;
	CoapShortStructure.MsgPacket.DestSN					= 0x00;
	CoapShortStructure.MsgPacket.Version					= 0x01;
}

void NET_NBIOT_CoapLongStructureInit(void)
{
	CoapLongStructure.HeadPacket.DeviceSN					= TCFG_EEPROM_Get_MAC_SN();
	CoapLongStructure.HeadPacket.DataLen					= 0x00;
	CoapLongStructure.HeadPacket.ProtocolType				= 0x00;
	CoapLongStructure.HeadPacket.Reserved1					= 0x00;
	CoapLongStructure.HeadPacket.ProtocolVersion				= 0x00;
	CoapLongStructure.HeadPacket.Reserved2					= 0x00;
	CoapLongStructure.HeadPacket.PacketType					= 0x05;
	CoapLongStructure.HeadPacket.PacketNumber				= 0x00;
	CoapLongStructure.MsgPacket.DestSN						= 0x00;
#if NBIOT_STATUS_MSG_VERSION_TYPE == NBIOT_STATUS_MSG_VERSION_33BYTE_V1
	CoapLongStructure.MsgPacket.Version					= 0x01;
#elif NBIOT_STATUS_MSG_VERSION_TYPE == NBIOT_STATUS_MSG_VERSION_77BYTE_V2
	CoapLongStructure.MsgPacket.Version					= 0x02;
#endif
}

void NET_NBIOT_CoapInfoStructureInit(void)
{
	memset((void*)&CoapInfoStructure.InfoData, 0, sizeof(CoapInfoStructure.InfoData));
	CoapInfoStructure.HeadPacket.DeviceSN					= TCFG_EEPROM_Get_MAC_SN();
	CoapInfoStructure.HeadPacket.DataLen					= 0x00;
	CoapInfoStructure.HeadPacket.ProtocolType				= 0x00;
	CoapInfoStructure.HeadPacket.Reserved1					= 0x00;
	CoapInfoStructure.HeadPacket.ProtocolVersion				= 0x00;
	CoapInfoStructure.HeadPacket.Reserved2					= 0x00;
	CoapInfoStructure.HeadPacket.PacketType					= 0x05;
	CoapInfoStructure.HeadPacket.PacketNumber				= 0x00;
	CoapInfoStructure.MsgPacket.DestSN						= 0x00;
	CoapInfoStructure.MsgPacket.Version					= 0x01;
}

void NET_NBIOT_CoapSentDataAfterExexution(void)
{
	TCFG_Utility_Add_NBIot_SentCount();
	TCFG_Utility_Add_NBIot_SentCountDay();
#if NBCOAP_LISTEN_PARAMETER_TYPE == NBCOAP_LISTEN_PARAMETER_ENABLE
	NbiotClientHandler.ListenRunCtl.ListenEnterParameter.listenEnable = true;
#endif
}
#endif

#if NETPROTOCAL == NETMQTTSN
void NET_NBIOT_MqttSNShortStructureInit(void)
{
	MqttSNShortStructure.HeadPacket.DeviceSN				= TCFG_EEPROM_Get_MAC_SN();
	MqttSNShortStructure.HeadPacket.DataLen					= 0x00;
	MqttSNShortStructure.HeadPacket.ProtocolType				= 0x00;
	MqttSNShortStructure.HeadPacket.Reserved1				= 0x00;
	MqttSNShortStructure.HeadPacket.ProtocolVersion			= 0x00;
	MqttSNShortStructure.HeadPacket.Reserved2				= 0x00;
	MqttSNShortStructure.HeadPacket.PacketType				= 0x05;
	MqttSNShortStructure.HeadPacket.PacketNumber				= 0x00;
	MqttSNShortStructure.MsgPacket.DestSN					= 0x00;
	MqttSNShortStructure.MsgPacket.Version					= 0x01;
}

void NET_NBIOT_MqttSNLongStructureInit(void)
{
	MqttSNLongStructure.HeadPacket.DeviceSN					= TCFG_EEPROM_Get_MAC_SN();
	MqttSNLongStructure.HeadPacket.DataLen					= 0x00;
	MqttSNLongStructure.HeadPacket.ProtocolType				= 0x00;
	MqttSNLongStructure.HeadPacket.Reserved1				= 0x00;
	MqttSNLongStructure.HeadPacket.ProtocolVersion			= 0x00;
	MqttSNLongStructure.HeadPacket.Reserved2				= 0x00;
	MqttSNLongStructure.HeadPacket.PacketType				= 0x05;
	MqttSNLongStructure.HeadPacket.PacketNumber				= 0x00;
	MqttSNLongStructure.MsgPacket.DestSN					= 0x00;
#if MQTTSN_STATUS_MSG_VERSION_TYPE == MQTTSN_STATUS_MSG_VERSION_V1
	MqttSNLongStructure.MsgPacket.Version					= 0x01;
#elif MQTTSN_STATUS_MSG_VERSION_TYPE == MQTTSN_STATUS_MSG_VERSION_V2
	MqttSNLongStructure.MsgPacket.Version					= 0x02;
#endif
}

void NET_NBIOT_MqttSNInfoStructureInit(void)
{
	memset((void*)&MqttSNInfoStructure.InfoData, 0, sizeof(MqttSNInfoStructure.InfoData));
	MqttSNInfoStructure.HeadPacket.DeviceSN					= TCFG_EEPROM_Get_MAC_SN();
	MqttSNInfoStructure.HeadPacket.DataLen					= 0x00;
	MqttSNInfoStructure.HeadPacket.ProtocolType				= 0x00;
	MqttSNInfoStructure.HeadPacket.Reserved1				= 0x00;
	MqttSNInfoStructure.HeadPacket.ProtocolVersion			= 0x00;
	MqttSNInfoStructure.HeadPacket.Reserved2				= 0x00;
	MqttSNInfoStructure.HeadPacket.PacketType				= 0x05;
	MqttSNInfoStructure.HeadPacket.PacketNumber				= 0x00;
	MqttSNInfoStructure.MsgPacket.DestSN					= 0x00;
	MqttSNInfoStructure.MsgPacket.Version					= 0x01;
}

void NET_NBIOT_MqttSNSentDataAfterExexution(void)
{
	TCFG_Utility_Add_NBIot_SentCount();
	TCFG_Utility_Add_NBIot_SentCountDay();
#if NBMQTTSN_LISTEN_PARAMETER_TYPE == NBMQTTSN_LISTEN_PARAMETER_ENABLE
	MqttSNClientHandler.ListenRunCtl.ListenEnterParameter.listenEnable = true;
#endif
}
#endif

#if NETPROTOCAL == NETONENET
void NET_NBIOT_OneNETShortStructureInit(void)
{
	OneNETShortStructure.HeadPacket.DeviceSN				= TCFG_EEPROM_Get_MAC_SN();
	OneNETShortStructure.HeadPacket.DataLen					= 0x00;
	OneNETShortStructure.HeadPacket.ProtocolType				= 0x00;
	OneNETShortStructure.HeadPacket.Reserved1				= 0x00;
	OneNETShortStructure.HeadPacket.ProtocolVersion			= 0x00;
	OneNETShortStructure.HeadPacket.Reserved2				= 0x00;
	OneNETShortStructure.HeadPacket.PacketType				= 0x05;
	OneNETShortStructure.HeadPacket.PacketNumber				= 0x00;
	OneNETShortStructure.MsgPacket.DestSN					= 0x00;
	OneNETShortStructure.MsgPacket.Version					= 0x01;
}

void NET_NBIOT_OneNETLongStructureInit(void)
{
	OneNETLongStructure.HeadPacket.DeviceSN					= TCFG_EEPROM_Get_MAC_SN();
	OneNETLongStructure.HeadPacket.DataLen					= 0x00;
	OneNETLongStructure.HeadPacket.ProtocolType				= 0x00;
	OneNETLongStructure.HeadPacket.Reserved1				= 0x00;
	OneNETLongStructure.HeadPacket.ProtocolVersion			= 0x00;
	OneNETLongStructure.HeadPacket.Reserved2				= 0x00;
	OneNETLongStructure.HeadPacket.PacketType				= 0x05;
	OneNETLongStructure.HeadPacket.PacketNumber				= 0x00;
	OneNETLongStructure.MsgPacket.DestSN					= 0x00;
#if ONENET_STATUS_MSG_VERSION_TYPE == ONENET_STATUS_MSG_VERSION_33BYTE_V1
	OneNETLongStructure.MsgPacket.Version					= 0x01;
#elif ONENET_STATUS_MSG_VERSION_TYPE == ONENET_STATUS_MSG_VERSION_77BYTE_V2
	OneNETLongStructure.MsgPacket.Version					= 0x02;
#endif
}

void NET_NBIOT_OneNETInfoStructureInit(void)
{
	memset((void*)&OneNETInfoStructure.InfoData, 0, sizeof(OneNETInfoStructure.InfoData));
	OneNETInfoStructure.HeadPacket.DeviceSN					= TCFG_EEPROM_Get_MAC_SN();
	OneNETInfoStructure.HeadPacket.DataLen					= 0x00;
	OneNETInfoStructure.HeadPacket.ProtocolType				= 0x00;
	OneNETInfoStructure.HeadPacket.Reserved1				= 0x00;
	OneNETInfoStructure.HeadPacket.ProtocolVersion			= 0x00;
	OneNETInfoStructure.HeadPacket.Reserved2				= 0x00;
	OneNETInfoStructure.HeadPacket.PacketType				= 0x05;
	OneNETInfoStructure.HeadPacket.PacketNumber				= 0x00;
	OneNETInfoStructure.MsgPacket.DestSN					= 0x00;
	OneNETInfoStructure.MsgPacket.Version					= 0x01;
}

void NET_NBIOT_OneNETSentDataAfterExexution(void)
{
	TCFG_Utility_Add_NBIot_SentCount();
	TCFG_Utility_Add_NBIot_SentCountDay();
#if NBONENET_LISTEN_PARAMETER_TYPE == NBONENET_LISTEN_PARAMETER_ENABLE
	OneNETClientHandler.ListenRunCtl.ListenEnterParameter.listenEnable = true;
#endif
}
#endif

/**********************************************************************************************************
 @Function			void NET_NBIOT_DataProcessing(NET_NBIOT_ClientsTypeDef* pClient)
 @Description			NET_NBIOT_DataProcessing						: NET数据处理
 @Input				void
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_DataProcessing(NET_NBIOT_ClientsTypeDef* pClient)
{
#if NETPROTOCAL == NETCOAP
	
	u32 len = 0;
	SpotStatusTypedef SpotStatusData;
	
	/* 检查是否有数据需要发送 */
	if (Inspect_Message_SpotStatusisEmpty() == false) {
	#if NBCOAP_SENDCODE_LONG_STATUS
		NETCoapNeedSendCode.LongStatus = 1;
	#endif
	}
	
	/* COAP SHORT STATUS DATA ENQUEUE */
	if (NETCoapNeedSendCode.ShortStatus) {
#if NBCOAP_SENDCODE_SHORT_STATUS
		Inspect_Message_SpotStatusDequeue(&SpotStatusData);
		NET_NBIOT_CoapShortStructureInit();
		CoapShortStructure.MsgPacket.Type					= COAP_MSGTYPE_TYPE_SHORT_STATUS;
		CoapShortStructure.DateTime						= SpotStatusData.unixTime;
		CoapShortStructure.SpotStatus						= SpotStatusData.spot_status;
		CoapShortStructure.SpotCount						= SpotStatusData.spot_count;
		NET_Coap_Message_SendDataEnqueue((unsigned char *)&CoapShortStructure, sizeof(CoapShortStructure));
		NETCoapNeedSendCode.ShortStatus = 0;
		Inspect_Message_SpotStatusOffSet();
		NET_NBIOT_CoapSentDataAfterExexution();
#endif
	}
	/* COAP LONG STATUS DATA ENQUEUE */
	else if (NETCoapNeedSendCode.LongStatus) {
#if NBCOAP_SENDCODE_LONG_STATUS
		Inspect_Message_SpotStatusDequeue(&SpotStatusData);
		NET_NBIOT_CoapLongStructureInit();
		CoapLongStructure.MsgPacket.Type					= COAP_MSGTYPE_TYPE_LONG_STATUS;
		CoapLongStructure.DateTime						= SpotStatusData.unixTime;
		CoapLongStructure.SpotStatus						= SpotStatusData.spot_status;
		CoapLongStructure.SpotCount						= SpotStatusData.spot_count;
		CoapLongStructure.MagneticX						= SpotStatusData.qmc5883lData.X_Now;
		CoapLongStructure.MagneticY						= SpotStatusData.qmc5883lData.Y_Now;
		CoapLongStructure.MagneticZ						= SpotStatusData.qmc5883lData.Z_Now;
		CoapLongStructure.MagneticDiff					= SpotStatusData.qmc5883lDiff.BackVal_Diff > 65535 ? 65535 : SpotStatusData.qmc5883lDiff.BackVal_Diff;
		CoapLongStructure.RadarDistance					= SpotStatusData.radarData.DisVal;
		CoapLongStructure.RadarStrength					= SpotStatusData.radarData.MagVal;
		CoapLongStructure.RadarCoverCount					= SpotStatusData.radarData.Diff_v2;
		CoapLongStructure.RadarDiff						= SpotStatusData.radarData.Diff;
#if NBIOT_STATUS_MSG_VERSION_TYPE == NBIOT_STATUS_MSG_VERSION_77BYTE_V2
		CoapLongStructure.NBRssi							= TCFG_Utility_Get_Nbiot_Rssi_IntVal();
		CoapLongStructure.NBSnr							= TCFG_Utility_Get_Nbiot_RadioSNR();
		CoapLongStructure.MCUTemp						= TCFG_Utility_Get_Device_Temperature();
		CoapLongStructure.QMCTemp						= Qmc5883lData.temp_now;
		CoapLongStructure.MagneticBackX					= Qmc5883lData.X_Back;
		CoapLongStructure.MagneticBackY					= Qmc5883lData.Y_Back;
		CoapLongStructure.MagneticBackZ					= Qmc5883lData.Z_Back;
		CoapLongStructure.Debugval						= SpotStatusData.radarData.timedomain_dif;
		for (int i = 0; i < 16; i++) {
			CoapLongStructure.Radarval[i] = radar_targetinfo.pMagNow[i+2]>255?255:radar_targetinfo.pMagNow[i+2];
			CoapLongStructure.Radarback[i] = radar_targetinfo.pMagBG[i+2]>255?255:radar_targetinfo.pMagBG[i+2];
		}
#endif
		NET_Coap_Message_SendDataEnqueue((unsigned char *)&CoapLongStructure, sizeof(CoapLongStructure));
		NETCoapNeedSendCode.LongStatus = 0;
		Inspect_Message_SpotStatusOffSet();
		NET_NBIOT_CoapSentDataAfterExexution();
#endif
	}
	/* COAP WORK INFO DATA ENQUEUE */
	else if (NETCoapNeedSendCode.WorkInfo) {
#if NBCOAP_SENDCODE_WORK_INFO
		if (TCFG_Utility_Get_Nbiot_Registered() != true) {
			return;
		}
		NET_NBIOT_CoapInfoStructureInit();
		CoapInfoStructure.HeadPacket.PacketType				= 0x05;
		CoapInfoStructure.MsgPacket.Type					= COAP_MSGTYPE_TYPE_WORK_INFO;
		len = NET_COAP_Message_Operate_Creat_Json_Work_Info((char *)&CoapInfoStructure.InfoData);
		NET_Coap_Message_SendDataEnqueue((unsigned char *)&CoapInfoStructure, sizeof(CoapInfoStructure) - sizeof(CoapInfoStructure.InfoData) + len);
		NETCoapNeedSendCode.WorkInfo = 0;
		NET_NBIOT_CoapSentDataAfterExexution();
#endif
	}
	/* COAP BASIC INFO DATA ENQUEUE */
	else if (NETCoapNeedSendCode.BasicInfo) {
#if NBCOAP_SENDCODE_BASIC_INFO
		if (TCFG_Utility_Get_Nbiot_Registered() != true) {
			return;
		}
		NET_NBIOT_CoapInfoStructureInit();
		CoapInfoStructure.HeadPacket.PacketType				= 0x05;
		CoapInfoStructure.MsgPacket.Type					= COAP_MSGTYPE_TYPE_BASIC_INFO;
		len = NET_COAP_Message_Operate_Creat_Json_Basic_Info((char *)&CoapInfoStructure.InfoData);
		NET_Coap_Message_SendDataEnqueue((unsigned char *)&CoapInfoStructure, sizeof(CoapInfoStructure) - sizeof(CoapInfoStructure.InfoData) + len);
		NETCoapNeedSendCode.BasicInfo = 0;
		NET_NBIOT_CoapSentDataAfterExexution();
#endif
	}
	/* COAP DYNAMIC INFO DATA ENQUEUE */
	else if (NETCoapNeedSendCode.DynamicInfo) {
#if NBCOAP_SENDCODE_DYNAMIC_INFO
		if (TCFG_Utility_Get_Nbiot_Registered() != true) {
			return;
		}
		NET_NBIOT_CoapInfoStructureInit();
		CoapInfoStructure.HeadPacket.PacketType				= 0x05;
		CoapInfoStructure.MsgPacket.Type					= COAP_MSGTYPE_TYPE_DYNAMIC_INFO;
		len = NET_COAP_Message_Operate_Creat_Json_Dynamic_Info((char *)&CoapInfoStructure.InfoData);
		NET_Coap_Message_SendDataEnqueue((unsigned char *)&CoapInfoStructure, sizeof(CoapInfoStructure) - sizeof(CoapInfoStructure.InfoData) + len);
		NETCoapNeedSendCode.DynamicInfo = 0;
		NET_NBIOT_CoapSentDataAfterExexution();
#endif
	}
	/* COAP RESPONSE INFO DATA ENQUEUE */
	else if (NETCoapNeedSendCode.ResponseInfo) {
#if NBCOAP_SENDCODE_RESPONSE_INFO
		NET_NBIOT_CoapInfoStructureInit();
		CoapInfoStructure.HeadPacket.PacketType				= 0x05;
		CoapInfoStructure.MsgPacket.Type					= COAP_MSGTYPE_TYPE_INFO;
		len = NET_COAP_Message_Operate_Creat_Json_Response_Info((char *)&CoapInfoStructure.InfoData, NETCoapNeedSendCode.ResponseInfoErrcode, NETCoapNeedSendCode.ResponseInfoMsgId);
		NET_Coap_Message_SendDataEnqueue((unsigned char *)&CoapInfoStructure, sizeof(CoapInfoStructure) - sizeof(CoapInfoStructure.InfoData) + len);
		NETCoapNeedSendCode.ResponseInfo = 0;
		NET_NBIOT_CoapSentDataAfterExexution();
#endif
	}
	
#elif NETPROTOCAL == NETMQTTSN
	
	u32 len = 0;
	SpotStatusTypedef SpotStatusData;
	
	/* 检查是否有数据需要发送 */
	if (Inspect_Message_SpotStatusisEmpty() == false) {
	#if NBMQTTSN_SENDCODE_STATUS_EXTEND
		NETMqttSNNeedSendCode.StatusExtend = 1;
	#endif
	}
	
	/* MQTTSN SHORT STATUS DATA ENQUEUE */
	if (NETMqttSNNeedSendCode.StatusBasic) {
#if NBMQTTSN_SENDCODE_STATUS_BASIC
		Inspect_Message_SpotStatusDequeue(&SpotStatusData);
		NET_NBIOT_MqttSNShortStructureInit();
		MqttSNShortStructure.MsgPacket.Type				= MQTTSN_MSGTYPE_TYPE_SHORT_STATUS;
		MqttSNShortStructure.DateTime						= SpotStatusData.unixTime;
		MqttSNShortStructure.SpotStatus					= SpotStatusData.spot_status;
		MqttSNShortStructure.SpotCount					= SpotStatusData.spot_count;
		NET_MqttSN_Message_SendDataEnqueue((unsigned char *)&MqttSNShortStructure, sizeof(MqttSNShortStructure));
		NETMqttSNNeedSendCode.StatusBasic = 0;
		Inspect_Message_SpotStatusOffSet();
		NET_NBIOT_MqttSNSentDataAfterExexution();
#endif
	}
	/* MQTTSN LONG STATUS DATA ENQUEUE */
	else if (NETMqttSNNeedSendCode.StatusExtend) {
#if NBMQTTSN_SENDCODE_STATUS_EXTEND
		Inspect_Message_SpotStatusDequeue(&SpotStatusData);
		NET_NBIOT_MqttSNLongStructureInit();
		MqttSNLongStructure.MsgPacket.Type					= MQTTSN_MSGTYPE_TYPE_LONG_STATUS;
		MqttSNLongStructure.DateTime						= SpotStatusData.unixTime;
		MqttSNLongStructure.SpotStatus					= SpotStatusData.spot_status;
		MqttSNLongStructure.SpotCount						= SpotStatusData.spot_count;
		MqttSNLongStructure.MagneticX						= SpotStatusData.qmc5883lData.X_Now;
		MqttSNLongStructure.MagneticY						= SpotStatusData.qmc5883lData.Y_Now;
		MqttSNLongStructure.MagneticZ						= SpotStatusData.qmc5883lData.Z_Now;
		MqttSNLongStructure.MagneticDiff					= SpotStatusData.qmc5883lDiff.BackVal_Diff > 65535 ? 65535 : SpotStatusData.qmc5883lDiff.BackVal_Diff;
		MqttSNLongStructure.RadarDistance					= SpotStatusData.radarData.DisVal;
		MqttSNLongStructure.RadarStrength					= SpotStatusData.radarData.MagVal;
		MqttSNLongStructure.RadarCoverCount				= SpotStatusData.radarData.Diff_v2;
		MqttSNLongStructure.RadarDiff						= SpotStatusData.radarData.Diff;
#if MQTTSN_STATUS_MSG_VERSION_TYPE == MQTTSN_STATUS_MSG_VERSION_V2
		MqttSNLongStructure.NBRssi						= TCFG_Utility_Get_Nbiot_Rssi_IntVal();
		MqttSNLongStructure.NBSnr						= TCFG_Utility_Get_Nbiot_RadioSNR();
		MqttSNLongStructure.MCUTemp						= TCFG_Utility_Get_Device_Temperature();
		MqttSNLongStructure.QMCTemp						= Qmc5883lData.temp_now;
		MqttSNLongStructure.MagneticBackX					= Qmc5883lData.X_Back;
		MqttSNLongStructure.MagneticBackY					= Qmc5883lData.Y_Back;
		MqttSNLongStructure.MagneticBackZ					= Qmc5883lData.Z_Back;
		MqttSNLongStructure.Debugval						= SpotStatusData.radarData.timedomain_dif;
		for (int i = 0; i < 16; i++) {
			MqttSNLongStructure.Radarval[i] = radar_targetinfo.pMagNow[i+2]>255?255:radar_targetinfo.pMagNow[i+2];
			MqttSNLongStructure.Radarback[i] = radar_targetinfo.pMagBG[i+2]>255?255:radar_targetinfo.pMagBG[i+2];
		}
#endif
		NET_MqttSN_Message_SendDataEnqueue((unsigned char *)&MqttSNLongStructure, sizeof(MqttSNLongStructure));
		NETMqttSNNeedSendCode.StatusExtend = 0;
		Inspect_Message_SpotStatusOffSet();
		NET_NBIOT_MqttSNSentDataAfterExexution();
#endif
	}
	/* MQTTSN INFO WORK DATA ENQUEUE */
	else if (NETMqttSNNeedSendCode.InfoWork) {
#if NBMQTTSN_SENDCODE_WORK_INFO
		if (TCFG_Utility_Get_Nbiot_Registered() != true) {
			return;
		}
		NET_NBIOT_MqttSNInfoStructureInit();
		MqttSNInfoStructure.HeadPacket.PacketType			= 0x05;
		MqttSNInfoStructure.MsgPacket.Type					= MQTTSN_MSGTYPE_TYPE_WORK_INFO;
		len = NET_MQTTSN_Message_Operate_Creat_Json_Work_Info((char *)&MqttSNInfoStructure.InfoData);
		NET_MqttSN_Message_SendDataEnqueue((unsigned char *)&MqttSNInfoStructure, sizeof(MqttSNInfoStructure) - sizeof(MqttSNInfoStructure.InfoData) + len);
		NETMqttSNNeedSendCode.InfoWork = 0;
		NET_NBIOT_MqttSNSentDataAfterExexution();
#endif
	}
	/* MQTTSN INFO BASIC DATA ENQUEUE */
	else if (NETMqttSNNeedSendCode.InfoBasic) {
#if NBMQTTSN_SENDCODE_BASIC_INFO
		if (TCFG_Utility_Get_Nbiot_Registered() != true) {
			return;
		}
		NET_NBIOT_MqttSNInfoStructureInit();
		MqttSNInfoStructure.HeadPacket.PacketType			= 0x05;
		MqttSNInfoStructure.MsgPacket.Type					= MQTTSN_MSGTYPE_TYPE_BASIC_INFO;
		len = NET_MQTTSN_Message_Operate_Creat_Json_Basic_Info((char *)&MqttSNInfoStructure.InfoData);
		NET_MqttSN_Message_SendDataEnqueue((unsigned char *)&MqttSNInfoStructure, sizeof(MqttSNInfoStructure) - sizeof(MqttSNInfoStructure.InfoData) + len);
		NETMqttSNNeedSendCode.InfoBasic = 0;
		NET_NBIOT_MqttSNSentDataAfterExexution();
#endif
	}
	/* MQTTSN INFO DYNAMIC DATA ENQUEUE */
	else if (NETMqttSNNeedSendCode.InfoDynamic) {
#if NBMQTTSN_SENDCODE_DYNAMIC_INFO
		if (TCFG_Utility_Get_Nbiot_Registered() != true) {
			return;
		}
		NET_NBIOT_MqttSNInfoStructureInit();
		MqttSNInfoStructure.HeadPacket.PacketType			= 0x05;
		MqttSNInfoStructure.MsgPacket.Type					= MQTTSN_MSGTYPE_TYPE_DYNAMIC_INFO;
		len = NET_MQTTSN_Message_Operate_Creat_Json_Dynamic_Info((char *)&MqttSNInfoStructure.InfoData);
		NET_MqttSN_Message_SendDataEnqueue((unsigned char *)&MqttSNInfoStructure, sizeof(MqttSNInfoStructure) - sizeof(MqttSNInfoStructure.InfoData) + len);
		NETMqttSNNeedSendCode.InfoDynamic = 0;
		NET_NBIOT_MqttSNSentDataAfterExexution();
#endif
	}
	/* MQTTSN INFO RESPONSE DATA ENQUEUE */
	else if (NETMqttSNNeedSendCode.InfoResponse) {
#if NBMQTTSN_SENDCODE_RESPONSE_INFO
		NET_NBIOT_MqttSNInfoStructureInit();
		MqttSNInfoStructure.HeadPacket.PacketType			= 0x05;
		MqttSNInfoStructure.MsgPacket.Type					= MQTTSN_MSGTYPE_TYPE_INFO;
		len = NET_MQTTSN_Message_Operate_Creat_Json_Response_Info((char *)&MqttSNInfoStructure.InfoData, NETMqttSNNeedSendCode.InfoResponseErrcode, NETMqttSNNeedSendCode.InfoResponseMsgId);
		NET_MqttSN_Message_SendDataEnqueue((unsigned char *)&MqttSNInfoStructure, sizeof(MqttSNInfoStructure) - sizeof(MqttSNInfoStructure.InfoData) + len);
		NETMqttSNNeedSendCode.InfoResponse = 0;
		NET_NBIOT_MqttSNSentDataAfterExexution();
#endif
	}
	
#elif NETPROTOCAL == NETONENET
	
	u32 len = 0;
	SpotStatusTypedef SpotStatusData;
	
	/* 检查是否有数据需要发送 */
	if (Inspect_Message_SpotStatusisEmpty() == false) {
	#if NBONENET_SENDCODE_LONG_STATUS
		NETOneNETNeedSendCode.LongStatus = 1;
	#endif
	}
	
	/* ONENET SHORT STATUS DATA ENQUEUE */
	if (NETOneNETNeedSendCode.ShortStatus) {
#if NBONENET_SENDCODE_SHORT_STATUS
		Inspect_Message_SpotStatusDequeue(&SpotStatusData);
		NET_NBIOT_OneNETShortStructureInit();
		OneNETShortStructure.MsgPacket.Type				= ONENET_MSGTYPE_TYPE_SHORT_STATUS;
		OneNETShortStructure.DateTime						= SpotStatusData.unixTime;
		OneNETShortStructure.SpotStatus					= SpotStatusData.spot_status;
		OneNETShortStructure.SpotCount					= SpotStatusData.spot_count;
		NET_OneNET_Message_SendDataEnqueue((unsigned char *)&OneNETShortStructure, sizeof(OneNETShortStructure));
		NETOneNETNeedSendCode.ShortStatus = 0;
		Inspect_Message_SpotStatusOffSet();
		NET_NBIOT_OneNETSentDataAfterExexution();
#endif
	}
	/* ONENET LONG STATUS DATA ENQUEUE */
	else if (NETOneNETNeedSendCode.LongStatus) {
#if NBONENET_SENDCODE_LONG_STATUS
		Inspect_Message_SpotStatusDequeue(&SpotStatusData);
		NET_NBIOT_OneNETLongStructureInit();
		OneNETLongStructure.MsgPacket.Type					= ONENET_MSGTYPE_TYPE_LONG_STATUS;
		OneNETLongStructure.DateTime						= SpotStatusData.unixTime;
		OneNETLongStructure.SpotStatus					= SpotStatusData.spot_status;
		OneNETLongStructure.SpotCount						= SpotStatusData.spot_count;
		OneNETLongStructure.MagneticX						= SpotStatusData.qmc5883lData.X_Now;
		OneNETLongStructure.MagneticY						= SpotStatusData.qmc5883lData.Y_Now;
		OneNETLongStructure.MagneticZ						= SpotStatusData.qmc5883lData.Z_Now;
		OneNETLongStructure.MagneticDiff					= SpotStatusData.qmc5883lDiff.BackVal_Diff > 65535 ? 65535 : SpotStatusData.qmc5883lDiff.BackVal_Diff;
		OneNETLongStructure.RadarDistance					= SpotStatusData.radarData.DisVal;
		OneNETLongStructure.RadarStrength					= SpotStatusData.radarData.MagVal;
		OneNETLongStructure.RadarCoverCount				= SpotStatusData.radarData.Diff_v2;
		OneNETLongStructure.RadarDiff						= SpotStatusData.radarData.Diff;
#if ONENET_STATUS_MSG_VERSION_TYPE == ONENET_STATUS_MSG_VERSION_77BYTE_V2
		OneNETLongStructure.NBRssi						= TCFG_Utility_Get_Nbiot_Rssi_IntVal();
		OneNETLongStructure.NBSnr						= TCFG_Utility_Get_Nbiot_RadioSNR();
		OneNETLongStructure.MCUTemp						= TCFG_Utility_Get_Device_Temperature();
		OneNETLongStructure.QMCTemp						= Qmc5883lData.temp_now;
		OneNETLongStructure.MagneticBackX					= Qmc5883lData.X_Back;
		OneNETLongStructure.MagneticBackY					= Qmc5883lData.Y_Back;
		OneNETLongStructure.MagneticBackZ					= Qmc5883lData.Z_Back;
		OneNETLongStructure.Debugval						= SpotStatusData.radarData.timedomain_dif;
		for (int i = 0; i < 16; i++) {
			OneNETLongStructure.Radarval[i] = radar_targetinfo.pMagNow[i+2]>255?255:radar_targetinfo.pMagNow[i+2];
			OneNETLongStructure.Radarback[i] = radar_targetinfo.pMagBG[i+2]>255?255:radar_targetinfo.pMagBG[i+2];
		}
#endif
		NET_OneNET_Message_SendDataEnqueue((unsigned char *)&OneNETLongStructure, sizeof(OneNETLongStructure));
		NETOneNETNeedSendCode.LongStatus = 0;
		Inspect_Message_SpotStatusOffSet();
		NET_NBIOT_OneNETSentDataAfterExexution();
#endif
	}
	/* ONENET WORK INFO DATA ENQUEUE */
	else if (NETOneNETNeedSendCode.WorkInfo) {
#if NBONENET_SENDCODE_WORK_INFO
		if (TCFG_Utility_Get_Nbiot_Registered() != true) {
			return;
		}
		NET_NBIOT_OneNETInfoStructureInit();
		OneNETInfoStructure.HeadPacket.PacketType			= 0x05;
		OneNETInfoStructure.MsgPacket.Type					= ONENET_MSGTYPE_TYPE_WORK_INFO;
		len = NET_ONENET_Message_Operate_Creat_Json_Work_Info((char *)&OneNETInfoStructure.InfoData);
		NET_OneNET_Message_SendDataEnqueue((unsigned char *)&OneNETInfoStructure, sizeof(OneNETInfoStructure) - sizeof(OneNETInfoStructure.InfoData) + len);
		NETOneNETNeedSendCode.WorkInfo = 0;
		NET_NBIOT_OneNETSentDataAfterExexution();
#endif
	}
	/* ONENET BASIC INFO DATA ENQUEUE */
	else if (NETOneNETNeedSendCode.BasicInfo) {
#if NBONENET_SENDCODE_BASIC_INFO
		if (TCFG_Utility_Get_Nbiot_Registered() != true) {
			return;
		}
		NET_NBIOT_OneNETInfoStructureInit();
		OneNETInfoStructure.HeadPacket.PacketType			= 0x05;
		OneNETInfoStructure.MsgPacket.Type					= ONENET_MSGTYPE_TYPE_BASIC_INFO;
		len = NET_ONENET_Message_Operate_Creat_Json_Basic_Info((char *)&OneNETInfoStructure.InfoData);
		NET_OneNET_Message_SendDataEnqueue((unsigned char *)&OneNETInfoStructure, sizeof(OneNETInfoStructure) - sizeof(OneNETInfoStructure.InfoData) + len);
		NETOneNETNeedSendCode.BasicInfo = 0;
		NET_NBIOT_OneNETSentDataAfterExexution();
#endif
	}
	/* ONENET DYNAMIC INFO DATA ENQUEUE */
	else if (NETOneNETNeedSendCode.DynamicInfo) {
#if NBONENET_SENDCODE_DYNAMIC_INFO
		if (TCFG_Utility_Get_Nbiot_Registered() != true) {
			return;
		}
		NET_NBIOT_OneNETInfoStructureInit();
		OneNETInfoStructure.HeadPacket.PacketType			= 0x05;
		OneNETInfoStructure.MsgPacket.Type					= ONENET_MSGTYPE_TYPE_DYNAMIC_INFO;
		len = NET_ONENET_Message_Operate_Creat_Json_Dynamic_Info((char *)&OneNETInfoStructure.InfoData);
		NET_OneNET_Message_SendDataEnqueue((unsigned char *)&OneNETInfoStructure, sizeof(OneNETInfoStructure) - sizeof(OneNETInfoStructure.InfoData) + len);
		NETOneNETNeedSendCode.DynamicInfo = 0;
		NET_NBIOT_OneNETSentDataAfterExexution();
#endif
	}
	/* ONENET RESPONSE INFO DATA ENQUEUE */
	else if (NETOneNETNeedSendCode.ResponseInfo) {
#if NBONENET_SENDCODE_RESPONSE_INFO
		NET_NBIOT_OneNETInfoStructureInit();
		OneNETInfoStructure.HeadPacket.PacketType			= 0x05;
		OneNETInfoStructure.MsgPacket.Type					= ONENET_MSGTYPE_TYPE_INFO;
		len = NET_ONENET_Message_Operate_Creat_Json_Response_Info((char *)&OneNETInfoStructure.InfoData, NETOneNETNeedSendCode.ResponseInfoErrcode, NETOneNETNeedSendCode.ResponseInfoMsgId);
		NET_OneNET_Message_SendDataEnqueue((unsigned char *)&OneNETInfoStructure, sizeof(OneNETInfoStructure) - sizeof(OneNETInfoStructure.InfoData) + len);
		NETOneNETNeedSendCode.ResponseInfo = 0;
		NET_NBIOT_OneNETSentDataAfterExexution();
#endif
	}
	
#endif
}

/**********************************************************************************************************
 @Function			void NET_NBIOT_TaskProcessing(NET_NBIOT_ClientsTypeDef* pClient)
 @Description			NET_NBIOT_TaskProcessing						: NET工作处理
 @Input				void
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_TaskProcessing(NET_NBIOT_ClientsTypeDef* pClient)
{
	/* NBIOT PollExecution */
#if NETPROTOCAL == NETCOAP
	
	switch (pClient->PollExecution)
	{
	case NET_POLL_EXECUTION_COAP:
		NET_COAP_APP_PollExecution(&NbiotClientHandler);
		break;
	
	case NET_POLL_EXECUTION_DNS:
		pClient->PollExecution = NET_POLL_EXECUTION_COAP;
		break;
	
	case NET_POLL_EXECUTION_MQTTSN:
		pClient->PollExecution = NET_POLL_EXECUTION_COAP;
		break;
	
	case NET_POLL_EXECUTION_PCP:
		NET_PCP_APP_PollExecution(&PCPClientHandler);
		break;
	
	case NET_POLL_EXECUTION_ONENET:
		pClient->PollExecution = NET_POLL_EXECUTION_COAP;
		break;
	}
	
#elif NETPROTOCAL == NETMQTTSN
	
	switch (pClient->PollExecution)
	{
	case NET_POLL_EXECUTION_COAP:
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_DISABLE
		pClient->PollExecution = NET_POLL_EXECUTION_MQTTSN;
#endif
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_ENABLE
		pClient->PollExecution = NET_POLL_EXECUTION_DNS;
#endif
		break;
	
	case NET_POLL_EXECUTION_DNS:
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_DISABLE
		pClient->PollExecution = NET_POLL_EXECUTION_MQTTSN;
#endif
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_ENABLE
		NET_DNS_APP_PollExecution(&DNSClientHandler);
#endif
		break;
	
	case NET_POLL_EXECUTION_MQTTSN:
		NET_MQTTSN_APP_PollExecution(&MqttSNClientHandler);
		break;
	
	case NET_POLL_EXECUTION_PCP:
		NET_MqttSN_PCP_APP_PollExecution(&MqttSNPCPClientHandler);
		break;
	
	case NET_POLL_EXECUTION_ONENET:
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_DISABLE
		pClient->PollExecution = NET_POLL_EXECUTION_MQTTSN;
#endif
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_ENABLE
		pClient->PollExecution = NET_POLL_EXECUTION_DNS;
#endif
		break;
	}
	
#elif NETPROTOCAL == NETONENET
	
	switch (pClient->PollExecution)
	{
	case NET_POLL_EXECUTION_COAP:
		pClient->PollExecution = NET_POLL_EXECUTION_ONENET;
		break;
	
	case NET_POLL_EXECUTION_DNS:
		pClient->PollExecution = NET_POLL_EXECUTION_ONENET;
		break;
	
	case NET_POLL_EXECUTION_MQTTSN:
		pClient->PollExecution = NET_POLL_EXECUTION_ONENET;
		break;
	
	case NET_POLL_EXECUTION_PCP:
		pClient->PollExecution = NET_POLL_EXECUTION_ONENET;
		break;
	
	case NET_POLL_EXECUTION_ONENET:
		NET_ONENET_APP_PollExecution(&OneNETClientHandler);
		break;
	}
	
#endif
}

/**********************************************************************************************************
 @Function			void NET_NBIOT_App_Task(void)
 @Description			NET_NBIOT_App_Task							: NET处理
 @Input				void
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_App_Task(void)
{
	NET_NBIOT_DataProcessing(&NetNbiotClientHandler);							//数据处理
	
	NET_NBIOT_TaskProcessing(&NetNbiotClientHandler);							//工作处理
}

/**********************************************************************************************************
 @Function			void NET_NBIOT_BackupCurrentApp_Task(void)
 @Description			NET_NBIOT_BackupCurrentApp_Task				: NET备份固件
 @Input				void
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_BackupCurrentApp_Task(void)
{
#ifndef NETPROTOCAL
	#error No Define NETPROTOCAL!
#else
#if (NETPROTOCAL == NETCOAP)
	
	PCP_Upgrade_BackupCurrentAPP(&PCPClientHandler);
	
#elif (NETPROTOCAL == NETMQTTSN)
	
	MqttPCP_Upgrade_BackupCurrentAPP(&MqttSNPCPClientHandler);
	
#elif (NETPROTOCAL == NETONENET)
	
	//Todo
	
#else
	#error NETPROTOCAL Define Error
#endif
#endif
}

/********************************************** END OF FLEE **********************************************/
