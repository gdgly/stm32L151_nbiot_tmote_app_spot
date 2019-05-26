/**
  *********************************************************************************************************
  * @file    platform_config.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2017-10-26
  * @brief   
  *********************************************************************************************************
  * @attention
  *			
  *
  *
  *********************************************************************************************************
  */

#include "platform_config.h"
#include "platform_map.h"
#include "hal_spiflash.h"
#include "radio_rf_app.h"
#include "radio_hal_rf.h"
#include "pcpupgrade.h"

bool BootUp					= true;									//BootUp
bool BackUp					= true;									//BackUp

/* Work Mode */
bool DeviceIdleMode				= false;									//IDLE MODE
bool DeviceActivedMode			= true;									//ACTIVED MODE

#if NETPROTOCAL == NETCOAP
/* Coap Packet */
COAP_PacketShortTypeDef			CoapShortStructure;							//Coap Short Packet
COAP_PacketLongTypeDef			CoapLongStructure;							//Coap Long Packet
COAP_PacketInfoTypeDef			CoapInfoStructure;							//Coap Info Packet
COAP_PacketPrivateTypeDef		CoapPrivateStructure;						//Coap Private Packet
#endif

#if NETPROTOCAL == NETMQTTSN
/* MqttSN Packet */
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
MQTTSN_StatusBasicTypeDef		MqttSNStatusBasicStructure;					//MqttSN Basic Status Packet
MQTTSN_StatusExtendTypeDef		MqttSNStatusExtendStructure;					//MqttSN Extend Status Packet
MQTTSN_InfoWorkTypeDef			MqttSNInfoWorkStructure;						//MqttSN Work Info Packet
MQTTSN_InfoBasicTypeDef			MqttSNInfoBasicStructure;					//MqttSN Basic Info Packet
MQTTSN_InfoDynamicTypeDef		MqttSNInfoDynamicStructure;					//MqttSN Dynamic Info Packet
MQTTSN_InfoResponseTypeDef		MqttSNInfoResponseStructure;					//MqttSN Response Info Packet
#endif
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM
MQTTSN_PacketShortTypeDef		MqttSNShortStructure;						//MqttSN Short Packet
MQTTSN_PacketLongTypeDef			MqttSNLongStructure;						//MqttSN Long Packet
MQTTSN_PacketInfoTypeDef			MqttSNInfoStructure;						//MqttSN Info Packet
MQTTSN_PacketPrivateTypeDef		MqttSNPrivateStructure;						//MqttSN Private Packet
#endif
#endif

#if NETPROTOCAL == NETONENET
/* OneNET Packet */
ONENET_PacketShortTypeDef		OneNETShortStructure;						//ONENET Short Packet
ONENET_PacketLongTypeDef			OneNETLongStructure;						//ONENET Long Packet
ONENET_PacketInfoTypeDef			OneNETInfoStructure;						//ONENET Info Packet
ONENET_PacketPrivateTypeDef		OneNETPrivateStructure;						//ONENET Private Packet
#endif

/* NET Handler */
NET_NBIOT_ClientsTypeDef			NetNbiotClientHandler;						//NET NBIOT Clinet Handler
NBIOT_ATCmdTypeDef				NbiotATCmdHandler;							//NBIOT AT Cmd Handler
NBIOT_ClientsTypeDef			NbiotClientHandler;							//Coap Clinet Handler

#if NETPROTOCAL == NETCOAP
PCP_CoAPNetTransportTypeDef		PCPCoAPNetHandler;							//PCP Net Handler
PCP_ClientsTypeDef				PCPClientHandler;							//PCP Clinet Handler
#endif

#if NETPROTOCAL == NETMQTTSN
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_ENABLE
DNS_SocketNetTypeDef			DNSSocketNetHandler;						//DNS Net Handler
DNS_ClientsTypeDef				DNSClientHandler;							//DNS Clinet Handler
#endif
MQTTSN_SocketNetTypeDef			MqttSNSocketNetHandler;						//MqttSN Net Handler
MQTTSN_ClientsTypeDef			MqttSNClientHandler;						//MqttSN Clinet Handler
MqttSNPCP_MqttNetTransportTypeDef	MqttSNPCPMqttNetHandler;						//MqttSN PCP Net Handler
MqttSNPCP_ClientsTypeDef			MqttSNPCPClientHandler;						//MqttSN PCP Clinet Handler
#endif

#if NETPROTOCAL == NETONENET
ONENET_LWM2MTransportTypeDef		OneNETLWM2MNetHandler;						//ONENET Net Handler
ONENET_ClientsTypeDef			OneNETClientHandler;						//ONENET Clinet Handler
#endif

NBIOT_NBandTypeDef				CoAPNBIoTBandType	= {COAP_NBIOT_BAND_NUM,   COAP_NBIOT_BAND_VAL1,   COAP_NBIOT_BAND_VAL2,   COAP_NBIOT_BAND_VAL3};
NBIOT_NBandTypeDef				OneNETNBIoTBandType	= {ONENET_NBIOT_BAND_NUM, ONENET_NBIOT_BAND_VAL1, ONENET_NBIOT_BAND_VAL2, ONENET_NBIOT_BAND_VAL3};
NBIOT_NBandTypeDef				MqttSNNBIoTBandType	= {MQTTSN_NBIOT_BAND_NUM, MQTTSN_NBIOT_BAND_VAL1, MQTTSN_NBIOT_BAND_VAL2, MQTTSN_NBIOT_BAND_VAL3};
NBIOT_NBandTypeDef				DNSNBIoTBandType	= {DNS_NBIOT_BAND_NUM,    DNS_NBIOT_BAND_VAL1,    DNS_NBIOT_BAND_VAL2,    DNS_NBIOT_BAND_VAL3};

/**********************************************************************************************************
 @Function			void RadioPrintWorkinfo(void)
 @Description			RadioPrintWorkinfo
 @Input				void
 @Return				void
**********************************************************************************************************/
void RadioPrintWorkinfo(void)
{
	TCFG_EEPROM_GetMagTempCoef(&TCFG_SystemData.MagCoefX, &TCFG_SystemData.MagCoefY, &TCFG_SystemData.MagCoefZ);
	
	Radio_Trf_Printf("WorkInfo:");
	
#if NETPROTOCAL == NETCOAP

#if RADIO_PRINT_WORKINFO_SOFT
	Radio_Trf_Printf("Soft:%d:%d.%d", TCFG_EEPROM_GetBootVersion(), TCFG_Utility_Get_Major_Softnumber(), TCFG_Utility_Get_Sub_Softnumber());
#endif
#if RADIO_PRINT_WORKINFO_SENSE
	Radio_Trf_Printf("Sense:%d", TCFG_EEPROM_GetSavedSensitivity());
#endif
#if RADIO_PRINT_WORKINFO_MODE
	Radio_Trf_Printf("Mode:%s", TCFG_EEPROM_Get_WorkMode_String());
#endif
#if RADIO_PRINT_WORKINFO_CHANNEL
	Radio_Trf_Printf("Channel:%d", TCFG_EEPROM_GetRfChannel());
#endif
#if RADIO_PRINT_WORKINFO_RANGE
	Radio_Trf_Printf("Range:%d", TCFG_Utility_Get_DistanceRange());
#endif
#if RADIO_PRINT_WORKINFO_EARFCN
	Radio_Trf_Printf("Earfcn:%d", TCFG_Utility_Get_Nbiot_RadioEARFCN());
#endif
#if RADIO_PRINT_WORKINFO_TAC
	Radio_Trf_Printf("Tac:%X", TCFG_Utility_Get_Nbiot_NetworkRegStatusTac());
#endif
#if RADIO_PRINT_WORKINFO_CI
	Radio_Trf_Printf("Ci:%X", TCFG_Utility_Get_Nbiot_NetworkRegStatusCellID());
#endif
#if RADIO_PRINT_WORKINFO_CMDCNT
	Radio_Trf_Printf("Cmdcnt:%d.%d", TCFG_EEPROM_GetRFCmdCnt(), TCFG_EEPROM_GetNBCmdCnt());
#endif
#if RADIO_PRINT_WORKINFO_NBRUNTIME
	Radio_Trf_Printf("Nbruntime:%d.%d", TCFG_Utility_GetCoapConnectTime(), TCFG_Utility_GetCoapIdleTime());
#endif
#if RADIO_PRINT_WORKINFO_APN
	Radio_Trf_Printf("APN:%s", TCFG_Utility_Get_Nbiot_PDPContext_APN());
#endif
#if RADIO_PRINT_WORKINFO_COEF
	Radio_Trf_Printf("Coef:%d.%d.%d", TCFG_SystemData.MagCoefX, TCFG_SystemData.MagCoefY, TCFG_SystemData.MagCoefZ);
#endif

#elif NETPROTOCAL == NETMQTTSN

#if RADIO_PRINT_WORKINFO_SOFT
	Radio_Trf_Printf("Soft:%d:%d.%d", TCFG_EEPROM_GetBootVersion(), TCFG_Utility_Get_Major_Softnumber(), TCFG_Utility_Get_Sub_Softnumber());
#endif
#if RADIO_PRINT_WORKINFO_SENSE
	Radio_Trf_Printf("Sense:%d", TCFG_EEPROM_GetSavedSensitivity());
#endif
#if RADIO_PRINT_WORKINFO_MODE
	Radio_Trf_Printf("Mode:%s", TCFG_EEPROM_Get_WorkMode_String());
#endif
#if RADIO_PRINT_WORKINFO_CHANNEL
	Radio_Trf_Printf("Channel:%d", TCFG_EEPROM_GetRfChannel());
#endif
#if RADIO_PRINT_WORKINFO_RANGE
	Radio_Trf_Printf("Range:%d", TCFG_Utility_Get_DistanceRange());
#endif
#if RADIO_PRINT_WORKINFO_EARFCN
	Radio_Trf_Printf("Earfcn:%d", TCFG_Utility_Get_Nbiot_RadioEARFCN());
#endif
#if RADIO_PRINT_WORKINFO_TAC
	Radio_Trf_Printf("Tac:%X", TCFG_Utility_Get_Nbiot_NetworkRegStatusTac());
#endif
#if RADIO_PRINT_WORKINFO_CI
	Radio_Trf_Printf("Ci:%X", TCFG_Utility_Get_Nbiot_NetworkRegStatusCellID());
#endif
#if RADIO_PRINT_WORKINFO_CMDCNT
	Radio_Trf_Printf("Cmdcnt:%d.%d", TCFG_EEPROM_GetRFCmdCnt(), TCFG_EEPROM_GetNBCmdCnt());
#endif
#if RADIO_PRINT_WORKINFO_NBRUNTIME
	Radio_Trf_Printf("Nbruntime:%d.%d", TCFG_Utility_GetCoapConnectTime(), TCFG_Utility_GetCoapIdleTime());
#endif
#if RADIO_PRINT_WORKINFO_APN
	Radio_Trf_Printf("APN:%s", TCFG_Utility_Get_Nbiot_PDPContext_APN());
#endif
#if RADIO_PRINT_WORKINFO_COEF
	Radio_Trf_Printf("Coef:%d.%d.%d", TCFG_SystemData.MagCoefX, TCFG_SystemData.MagCoefY, TCFG_SystemData.MagCoefZ);
#endif

#elif NETPROTOCAL == NETONENET

#if RADIO_PRINT_WORKINFO_SOFT
	Radio_Trf_Printf("Soft:%d:%d.%d", TCFG_EEPROM_GetBootVersion(), TCFG_Utility_Get_Major_Softnumber(), TCFG_Utility_Get_Sub_Softnumber());
#endif
#if RADIO_PRINT_WORKINFO_SENSE
	Radio_Trf_Printf("Sense:%d", TCFG_EEPROM_GetSavedSensitivity());
#endif
#if RADIO_PRINT_WORKINFO_MODE
	Radio_Trf_Printf("Mode:%s", TCFG_EEPROM_Get_WorkMode_String());
#endif
#if RADIO_PRINT_WORKINFO_CHANNEL
	Radio_Trf_Printf("Channel:%d", TCFG_EEPROM_GetRfChannel());
#endif
#if RADIO_PRINT_WORKINFO_RANGE
	Radio_Trf_Printf("Range:%d", TCFG_Utility_Get_DistanceRange());
#endif
#if RADIO_PRINT_WORKINFO_EARFCN
	Radio_Trf_Printf("Earfcn:%d", TCFG_Utility_Get_Nbiot_RadioEARFCN());
#endif
#if RADIO_PRINT_WORKINFO_TAC
	Radio_Trf_Printf("Tac:%X", TCFG_Utility_Get_Nbiot_NetworkRegStatusTac());
#endif
#if RADIO_PRINT_WORKINFO_CI
	Radio_Trf_Printf("Ci:%X", TCFG_Utility_Get_Nbiot_NetworkRegStatusCellID());
#endif
#if RADIO_PRINT_WORKINFO_CMDCNT
	Radio_Trf_Printf("Cmdcnt:%d.%d", TCFG_EEPROM_GetRFCmdCnt(), TCFG_EEPROM_GetNBCmdCnt());
#endif
#if RADIO_PRINT_WORKINFO_NBRUNTIME
	Radio_Trf_Printf("Nbruntime:%d.%d", TCFG_Utility_GetCoapConnectTime(), TCFG_Utility_GetCoapIdleTime());
#endif
#if RADIO_PRINT_WORKINFO_APN
	Radio_Trf_Printf("APN:%s", TCFG_Utility_Get_Nbiot_PDPContext_APN());
#endif
#if RADIO_PRINT_WORKINFO_COEF
	Radio_Trf_Printf("Coef:%d.%d.%d", TCFG_SystemData.MagCoefX, TCFG_SystemData.MagCoefY, TCFG_SystemData.MagCoefZ);
#endif

#endif
}

/**********************************************************************************************************
 @Function			void RadioPrintNetinfo(void)
 @Description			RadioPrintNetinfo
 @Input				void
 @Return				void
**********************************************************************************************************/
void RadioPrintNetinfo(void)
{
	Radio_Trf_Printf("NetInfo:");
	
#if NETPROTOCAL == NETCOAP

#if RADIO_PRINT_NETINFO_MUFTUR
	Radio_Trf_Printf("Muftur:%s", NbiotClientHandler.Parameter.manufacturer);
#endif
#if RADIO_PRINT_NETINFO_MUFTURMD
	Radio_Trf_Printf("MufturMd:%s", NbiotClientHandler.Parameter.manufacturermode);
#endif
#if RADIO_PRINT_NETINFO_MDUVER
	Radio_Trf_Printf("MduVer:%s", NbiotClientHandler.Parameter.modelversion);
#endif
#if RADIO_PRINT_NETINFO_IMEI
	Radio_Trf_Printf("IMEI:%s", NbiotClientHandler.Parameter.imei);
#endif
#if RADIO_PRINT_NETINFO_ICCID
	Radio_Trf_Printf("ICCID:%s", NbiotClientHandler.Parameter.iccid);
#endif
#if RADIO_PRINT_NETINFO_IMSI
	Radio_Trf_Printf("IMSI:%s", NbiotClientHandler.Parameter.imsi);
#endif
#if RADIO_PRINT_NETINFO_CGP
	Radio_Trf_Printf("CGP:%s", NbiotClientHandler.Parameter.cgpaddr);
#endif
#if RADIO_PRINT_NETINFO_PDPTYPE
	Radio_Trf_Printf("PDPType:%s", NbiotClientHandler.Parameter.cgdcontPDPType);
#endif
#if RADIO_PRINT_NETINFO_APN
	Radio_Trf_Printf("APN:%s", NbiotClientHandler.Parameter.cgdcontAPN);
#endif
#if RADIO_PRINT_NETINFO_RSSI
	Radio_Trf_Printf("RSSI:%d", NbiotClientHandler.Parameter.rssi);
#endif
#if RADIO_PRINT_NETINFO_SNR
	Radio_Trf_Printf("SNR:%d", NbiotClientHandler.Parameter.statisticsRADIO.SNR);
#endif
#if RADIO_PRINT_NETINFO_OTHER
	Radio_Trf_Printf("CDPHost:%s", NbiotClientHandler.Parameter.cdpserver.CDPServerHost);
	Radio_Trf_Printf("CDPPort:%d", NbiotClientHandler.Parameter.cdpserver.CDPServerPort);
#endif

#elif NETPROTOCAL == NETMQTTSN

#if RADIO_PRINT_NETINFO_MUFTUR
	Radio_Trf_Printf("Muftur:%s", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.manufacturer);
#endif
#if RADIO_PRINT_NETINFO_MUFTURMD
	Radio_Trf_Printf("MufturMd:%s", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.manufacturermode);
#endif
#if RADIO_PRINT_NETINFO_MDUVER
	Radio_Trf_Printf("MduVer:%s", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.modelversion);
#endif
#if RADIO_PRINT_NETINFO_IMEI
	Radio_Trf_Printf("IMEI:%s", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.imei);
#endif
#if RADIO_PRINT_NETINFO_ICCID
	Radio_Trf_Printf("ICCID:%s", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.iccid);
#endif
#if RADIO_PRINT_NETINFO_IMSI
	Radio_Trf_Printf("IMSI:%s", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.imsi);
#endif
#if RADIO_PRINT_NETINFO_CGP
	Radio_Trf_Printf("CGP:%s", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.cgpaddr);
#endif
#if RADIO_PRINT_NETINFO_PDPTYPE
	Radio_Trf_Printf("PDPType:%s", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.cgdcontPDPType);
#endif
#if RADIO_PRINT_NETINFO_APN
	Radio_Trf_Printf("APN:%s", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.cgdcontAPN);
#endif
#if RADIO_PRINT_NETINFO_RSSI
	Radio_Trf_Printf("RSSI:%d", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.rssi);
#endif
#if RADIO_PRINT_NETINFO_SNR
	Radio_Trf_Printf("SNR:%d", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.statisticsRADIO.SNR);
#endif
#if RADIO_PRINT_NETINFO_OTHER
	Radio_Trf_Printf("MqttSN :%s", MQTTSN_SERVER_HOST_NAME);
	Radio_Trf_Printf("MqttSN :%s:%d", MqttSNClientHandler.SocketStack->ServerHost, MqttSNClientHandler.SocketStack->ServerPort);
#endif

#elif NETPROTOCAL == NETONENET

#if RADIO_PRINT_NETINFO_MUFTUR
	Radio_Trf_Printf("Muftur:%s", OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.manufacturer);
#endif
#if RADIO_PRINT_NETINFO_MUFTURMD
	Radio_Trf_Printf("MufturMd:%s", OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.manufacturermode);
#endif
#if RADIO_PRINT_NETINFO_MDUVER
	Radio_Trf_Printf("MduVer:%s", OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.modelversion);
#endif
#if RADIO_PRINT_NETINFO_IMEI
	Radio_Trf_Printf("IMEI:%s", OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.imei);
#endif
#if RADIO_PRINT_NETINFO_ICCID
	Radio_Trf_Printf("ICCID:%s", OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.iccid);
#endif
#if RADIO_PRINT_NETINFO_IMSI
	Radio_Trf_Printf("IMSI:%s", OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.imsi);
#endif
#if RADIO_PRINT_NETINFO_CGP
	Radio_Trf_Printf("CGP:%s", OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.cgpaddr);
#endif
#if RADIO_PRINT_NETINFO_PDPTYPE
	Radio_Trf_Printf("PDPType:%s", OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.cgdcontPDPType);
#endif
#if RADIO_PRINT_NETINFO_APN
	Radio_Trf_Printf("APN:%s", OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.cgdcontAPN);
#endif
#if RADIO_PRINT_NETINFO_RSSI
	Radio_Trf_Printf("RSSI:%d", OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.rssi);
#endif
#if RADIO_PRINT_NETINFO_SNR
	Radio_Trf_Printf("SNR:%d", OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.statisticsRADIO.SNR);
#endif

#endif
}

/**********************************************************************************************************
 @Function			void RadioPrintDeviceinfo(void)
 @Description			RadioPrintDeviceinfo
 @Input				void
 @Return				void
**********************************************************************************************************/
void RadioPrintDeviceinfo(void)
{
	Radio_Trf_Printf("DevInfo:");
	
#if RADIO_PRINT_DEVINFO_RUNTIME
	Radio_Trf_Printf("Runtime:%d", TCFG_Utility_Get_Run_Time());
#endif
#if RADIO_PRINT_DEVINFO_BATT
	Radio_Trf_Printf("Batt:%d", TCFG_Utility_Get_Device_Batt_ShortVal());
#endif
#if RADIO_PRINT_DEVINFO_RDLIB
	Radio_Trf_Printf("RdLib:%d", TCFG_Utility_Get_RadarLibNum());
#endif
#if RADIO_PRINT_DEVINFO_RDCNT
	Radio_Trf_Printf("RdCnt:%d", TCFG_GetRadarCount());
#endif
#if RADIO_PRINT_DEVINFO_MCUTEMP
	Radio_Trf_Printf("MCUTemp:%d", TCFG_Utility_Get_Device_Temperature());
#endif
#if RADIO_PRINT_DEVINFO_ALGOLIB
	Radio_Trf_Printf("AlgoLib:%d", TCFG_Utility_Get_AlgoLibNum());
#endif
#if RADIO_PRINT_DEVINFO_QMCREBOOT
	Radio_Trf_Printf("QmcReboot:%d", TCFG_Utility_Get_ReInitModuleCount());
#endif
#if RADIO_PRINT_DEVINFO_NBBOOT
	Radio_Trf_Printf("Nbboot:%d", TCFG_Utility_Get_Nbiot_BootCount());
#endif
#if RADIO_PRINT_DEVINFO_NBSENT
	Radio_Trf_Printf("Nbsent:%d", TCFG_Utility_Get_Nbiot_SentCount());
#endif
#if RADIO_PRINT_DEVINFO_NBRECV
	Radio_Trf_Printf("Nbrecv:%d", TCFG_Utility_Get_Nbiot_RecvCount());
#endif
#if RADIO_PRINT_DEVINFO_INDELAY
	Radio_Trf_Printf("Indelay:%d", TCFG_EEPROM_GetCarInDelay());
#endif
#if RADIO_PRINT_DEVINFO_NBHEART
	Radio_Trf_Printf("Nbheart:%d", TCFG_EEPROM_GetNbiotHeart());
#endif
#if RADIO_PRINT_DEVINFO_CGAIN
	Radio_Trf_Printf("Cgain:%d", TCFG_Utility_Get_GainCover());
#endif
#if RADIO_PRINT_DEVINFO_RGAIN
	Radio_Trf_Printf("Rgain:%d", TCFG_EEPROM_GetRadarGain());
#endif
#if RADIO_PRINT_DEVINFO_SMODE
	Radio_Trf_Printf("Smode:%d", TCFG_EEPROM_GetSenseMode());
#endif
}

/**********************************************************************************************************
 @Function			void RadioPrintUpgradeinfo(void)
 @Description			RadioPrintUpgradeinfo
 @Input				void
 @Return				void
**********************************************************************************************************/
void RadioPrintUpgradeinfo(void)
{
	Radio_Trf_Printf("UpgradeInfo:");
	
#ifdef GD25Q_80CSIG
	PCP_APPInfoTypeDef APP1Info;
	PCP_APPInfoTypeDef APP2Info;
	
	Radio_Rf_Interrupt_Deinit();
	GD25Q_SPIFLASH_WakeUp();
	GD25Q_SPIFLASH_Init();
	
	if (GD25Q80CSIG_OK != GD25Q_SPIFLASH_Get_Status()) {
		GD25Q_SPIFLASH_PowerDown();
		Radio_Trf_Printf("SPI FLASH Fail %d", GD25Q_SPIFLASH_Get_Status());
		return;
	}
	
	APP1Info.Status	= GD25Q_SPIFLASH_GetByte(APP1_INFO_UPGRADE_STATUS_OFFSET);
	APP1Info.BaseAddr	= GD25Q_SPIFLASH_GetWord(APP1_INFO_UPGRADE_BASEADDR_OFFSET);
	APP1Info.BlockNum	= GD25Q_SPIFLASH_GetHalfWord(APP1_INFO_UPGRADE_BLOCKNUM_OFFSET);
	APP1Info.BlockLen	= GD25Q_SPIFLASH_GetHalfWord(APP1_INFO_UPGRADE_BLOCKLEN_OFFSET);
	APP1Info.DataLen	= GD25Q_SPIFLASH_GetHalfWord(APP1_INFO_UPGRADE_DATALEN_OFFSET);
	APP1Info.SoftVer	= GD25Q_SPIFLASH_GetWord(APP1_INFO_UPGRADE_SOFTVER_OFFSET);
	APP1Info.CheckCode	= GD25Q_SPIFLASH_GetWord(APP1_DATA_CHECK_CODE_OFFSET);
	
	APP2Info.Status	= GD25Q_SPIFLASH_GetByte(APP2_INFO_UPGRADE_STATUS_OFFSET);
	APP2Info.BaseAddr	= GD25Q_SPIFLASH_GetWord(APP2_INFO_UPGRADE_BASEADDR_OFFSET);
	APP2Info.BlockNum	= GD25Q_SPIFLASH_GetHalfWord(APP2_INFO_UPGRADE_BLOCKNUM_OFFSET);
	APP2Info.BlockLen	= GD25Q_SPIFLASH_GetHalfWord(APP2_INFO_UPGRADE_BLOCKLEN_OFFSET);
	APP2Info.DataLen	= GD25Q_SPIFLASH_GetHalfWord(APP2_INFO_UPGRADE_DATALEN_OFFSET);
	APP2Info.SoftVer	= GD25Q_SPIFLASH_GetWord(APP2_INFO_UPGRADE_SOFTVER_OFFSET);
	APP2Info.CheckCode	= GD25Q_SPIFLASH_GetWord(APP2_DATA_CHECK_CODE_OFFSET);
	
	GD25Q_SPIFLASH_PowerDown();
	
	Radio_Trf_Printf("APP1 Status: %02X", APP1Info.Status);
	Radio_Trf_Printf("APP1 BaseAddr: %08X", APP1Info.BaseAddr);
	Radio_Trf_Printf("APP1 BlockNum: %d", APP1Info.BlockNum);
	Radio_Trf_Printf("APP1 BlockLen: %d", APP1Info.BlockLen);
	Radio_Trf_Printf("APP1 DataLen: %d", APP1Info.DataLen);
	Radio_Trf_Printf("APP1 SoftVer: V%d.%d", APP1Info.SoftVer>>16, APP1Info.SoftVer&0xFFFF);
	Radio_Trf_Printf("APP1 CheckCode: %X", APP1Info.CheckCode);
	Radio_Trf_Printf("APP2 Status: %02X", APP2Info.Status);
	Radio_Trf_Printf("APP2 BaseAddr: %08X", APP2Info.BaseAddr);
	Radio_Trf_Printf("APP2 BlockNum: %d", APP2Info.BlockNum);
	Radio_Trf_Printf("APP2 BlockLen: %d", APP2Info.BlockLen);
	Radio_Trf_Printf("APP2 DataLen: %d", APP2Info.DataLen);
	Radio_Trf_Printf("APP2 SoftVer: V%d.%d", APP2Info.SoftVer>>16, APP2Info.SoftVer&0xFFFF);
	Radio_Trf_Printf("APP2 CheckCode: %X", APP2Info.CheckCode);
#else
	Radio_Trf_Printf("SPI FLASH Invalid");
#endif
}

/********************************************** END OF FLEE **********************************************/
