/**
  *********************************************************************************************************
  * @file    ctwingaepcodes.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2019-11-04
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "ctwingaepcodes.h"
#include "platform_config.h"
#include "platform_map.h"
#include "stm32l1xx_config.h"
#include "radar_api.h"
#include "tmesh_algorithm.h"
#include "delay.h"
#include "usart.h"
#include "string.h"
#include "stdlib.h"
#include "ctype.h"

#if CTWING_AEPMODULE_TYPE == CTWING_AEPMODULE_MVB_VD33D_P2_1
AepSpotStatusData				AepSpotStatusSrcdata;
AepWorkInfo					AepWorkInfoSrcdata;
AepBasicInfo					AepBasicInfoSrcdata;
AepDynamicInfo					AepDynamicInfoSrcdata;

AepSpotStatusDataString			AepSpotStatusString;
AepWorkInfoDataString			AepWorkInfoString;
AepBasicInfoDataString			AepBasicInfoString;
AepDynamicInfoDataString			AepDynamicInfoString;
#endif

#if CTWING_AEPMODULE_TYPE == CTWING_AEPMODULE_MVB_VD33D_P2_2
AepDataReportData				AepDataReportSrcdata;
AepSignalReportData				AepSignalReportSrcdata;
AepHeartBeatData				AepHeartBeatSrcdata;
AepParkingChangeInfoData			AepParkingChangeInfoSrcdata;
AepErrorCodeReportData			AepErrorCodeReportSrcdata;
AepLowVoltageAlarmData			AepLowVoltageAlarmSrcdata;
AepMagneticDisturbData			AepMagneticDisturbSrcdata;
#endif

#if CTWING_AEPMODULE_TYPE == CTWING_AEPMODULE_MVB_VD33D_P2_3
AepDataReportData				AepDataReportSrcdata;
AepSignalReportData				AepSignalReportSrcdata;
AepExtraReportData				AepExtraReportSrcdata;
AepHeartBeatData				AepHeartBeatSrcdata;
AepParkingChangeInfoData			AepParkingChangeInfoSrcdata;
AepErrorCodeReportData			AepErrorCodeReportSrcdata;
AepLowVoltageAlarmData			AepLowVoltageAlarmSrcdata;
AepMagneticDisturbData			AepMagneticDisturbSrcdata;
#endif

uint16_t aep_htons(uint16_t source)
{
	if (AEP_ENDIANNESS == AEP_BIG_ENDIAN)
		return source;
	else
		return (uint16_t)( 0
		| ((source & 0x00ff) << 8)
		| ((source & 0xff00) >> 8) );
}

uint32_t aep_htoni(uint32_t source)
{
	if (AEP_ENDIANNESS == AEP_BIG_ENDIAN)
		return source;
	else
		return (uint32_t)( 0
		| ((source & 0x000000ff) << 24)
		| ((source & 0x0000ff00) << 8)
		| ((source & 0x00ff0000) >> 8)
		| ((source & 0xff000000) >> 24) );
}

uint64_t aep_htonl(uint64_t source)
{
	if (AEP_ENDIANNESS == AEP_BIG_ENDIAN)
		return source;
	else
		return (uint64_t)( 0
		| ((source & (uint64_t)(0x00000000000000ff)) << 56)
		| ((source & (uint64_t)(0x000000000000ff00)) << 40)
		| ((source & (uint64_t)(0x0000000000ff0000)) << 24)
		| ((source & (uint64_t)(0x00000000ff000000)) << 8)
		| ((source & (uint64_t)(0x000000ff00000000)) >> 8)
		| ((source & (uint64_t)(0x0000ff0000000000)) >> 24)
		| ((source & (uint64_t)(0x00ff000000000000)) >> 40)
		| ((source & (uint64_t)(0xff00000000000000)) >> 56) );
}

float aep_htonf(float source)
{
	if (AEP_ENDIANNESS == AEP_BIG_ENDIAN)
		return source;
	else {
		uint32_t t = 0
		| ((*(uint32_t*)&source & 0x000000ff) << 24)
		| ((*(uint32_t*)&source & 0x0000ff00) << 8)
		| ((*(uint32_t*)&source & 0x00ff0000) >> 8)
		| ((*(uint32_t*)&source & 0xff000000) >> 24);
		return *(float*)&t;
	}
}

double aep_htond(double source)
{
	if (AEP_ENDIANNESS == AEP_BIG_ENDIAN)
		return source;
	else {
		uint64_t t = 0
		| ((*(uint64_t*)&source & (uint64_t)(0x00000000000000ff)) << 56)
		| ((*(uint64_t*)&source & (uint64_t)(0x000000000000ff00)) << 40)
		| ((*(uint64_t*)&source & (uint64_t)(0x0000000000ff0000)) << 24)
		| ((*(uint64_t*)&source & (uint64_t)(0x00000000ff000000)) << 8)
		| ((*(uint64_t*)&source & (uint64_t)(0x000000ff00000000)) >> 8)
		| ((*(uint64_t*)&source & (uint64_t)(0x0000ff0000000000)) >> 24)
		| ((*(uint64_t*)&source & (uint64_t)(0x00ff000000000000)) >> 40)
		| ((*(uint64_t*)&source & (uint64_t)(0xff00000000000000)) >> 56);
		return *(double*)&t;
	}
}

void HexToStr(char *pbDest, char *pbSrc, int nLen)
{
	unsigned char ddl, ddh;
	
	for (int i = 0; i < nLen; i++) {
		ddh = 48 + (unsigned char)pbSrc[i] / 16;
		ddl = 48 + (unsigned char)pbSrc[i] % 16;
		if (ddh > 57) ddh = ddh + 7;
		if (ddl > 57) ddl = ddl + 7;
		pbDest[i*2+0] = ddh;
		pbDest[i*2+1] = ddl;
	}
}

void StrToHex(char *pbDest, char *pbSrc, int nLen)
{
	unsigned char h1, h2;
	unsigned char s1, s2;
	
	for (int i = 0; i < nLen; i++) {
		h1 = pbSrc[2*i+0];
		h2 = pbSrc[2*i+1];
		
		s1 = toupper(h1) - 0x30;
		if (s1 > 9) s1 -= 7;
		
		s2 = toupper(h2) - 0x30;
		if (s2 > 9) s2 -= 7;
		
		pbDest[i] = s1*16 + s2;
	}
}

#if CTWING_AEPMODULE_TYPE == CTWING_AEPMODULE_MVB_VD33D_P2_1
/**********************************************************************************************************
 @Function			void CTWing_Message_Operate_Creat_Work_Info(CTWING_ClientsTypeDef* pClient, AepWorkInfo * srcStruct)
 @Description			CTWing_Message_Operate_Creat_Work_Info		: 填写WorkInfo数据
 @Input				pClient								: CTWing客户端实例
					srcStruct
 @Return				void
**********************************************************************************************************/
void CTWing_Message_Operate_Creat_Work_Info(CTWING_ClientsTypeDef* pClient, AepWorkInfo * srcStruct)
{
	sprintf(srcStruct->SN, "%08x", TCFG_EEPROM_Get_MAC_SN());
	srcStruct->Sense			= TCFG_EEPROM_GetSavedSensitivity();
	srcStruct->WorkMode.str		= TCFG_EEPROM_Get_WorkMode_String();
	srcStruct->WorkMode.len		= strlen((const char*)srcStruct->WorkMode.str);
	srcStruct->RfChannel		= TCFG_EEPROM_GetRfChannel();
	srcStruct->Range			= TCFG_Utility_Get_DistanceRange();
	srcStruct->Earfcn			= TCFG_Utility_Get_Nbiot_RadioEARFCN();
	srcStruct->Tac				= TCFG_Utility_Get_Nbiot_NetworkRegStatusTac();
	srcStruct->Ci				= TCFG_Utility_Get_Nbiot_NetworkRegStatusCellID();
	memset((void *)AepWorkInfoString.Cmdcnt, 0x0, sizeof(AepWorkInfoString.Cmdcnt));
	sprintf(AepWorkInfoString.Cmdcnt, "%d.%d", TCFG_EEPROM_GetRFCmdCnt(), TCFG_EEPROM_GetNBCmdCnt());
	srcStruct->Cmdcnt.str		= AepWorkInfoString.Cmdcnt;
	srcStruct->Cmdcnt.len		= strlen((const char*)srcStruct->Cmdcnt.str);
	memset((void *)AepWorkInfoString.Nbruntime, 0x0, sizeof(AepWorkInfoString.Nbruntime));
	sprintf(AepWorkInfoString.Nbruntime, "%d.%d", TCFG_Utility_GetCoapConnectTime(), TCFG_Utility_GetCoapIdleTime());
	srcStruct->Nbruntime.str		= AepWorkInfoString.Nbruntime;
	srcStruct->Nbruntime.len		= strlen((const char*)srcStruct->Nbruntime.str);
	srcStruct->APN.str			= TCFG_Utility_Get_Nbiot_PDPContext_APN();
	srcStruct->APN.len			= strlen((const char*)srcStruct->APN.str);
	memset((void *)AepWorkInfoString.Coef, 0x0, sizeof(AepWorkInfoString.Coef));
	sprintf(AepWorkInfoString.Coef, "%d.%d.%d", TCFG_SystemData.MagCoefX, TCFG_SystemData.MagCoefY, TCFG_SystemData.MagCoefZ);
	srcStruct->Coef.str			= AepWorkInfoString.Coef;
	srcStruct->Coef.len			= strlen((const char*)srcStruct->Coef.str);
	srcStruct->Rollinit			= TCFG_EEPROM_GetRollingOverInitSensor();
	srcStruct->RadioRv			= TCFG_Utility_Get_RadioGatewayNearby();
}

/**********************************************************************************************************
 @Function			void CTWing_Message_Operate_Creat_Basic_Info(CTWING_ClientsTypeDef* pClient, AepBasicInfo * srcStruct)
 @Description			CTWing_Message_Operate_Creat_Basic_Info		: 填写BasicInfo数据
 @Input				pClient								: CTWing客户端实例
					srcStruct
 @Return				void
**********************************************************************************************************/
void CTWing_Message_Operate_Creat_Basic_Info(CTWING_ClientsTypeDef* pClient, AepBasicInfo * srcStruct)
{
	sprintf(srcStruct->SN, "%08x", TCFG_EEPROM_Get_MAC_SN());
	memset((void *)AepBasicInfoString.ModelType, 0x0, sizeof(AepBasicInfoString.ModelType));
	sprintf(AepBasicInfoString.ModelType, "%d.1", TCFG_Utility_Get_Mvb_ModelType());
	srcStruct->ModelType.str		= AepBasicInfoString.ModelType;
	srcStruct->ModelType.len		= strlen((const char*)srcStruct->ModelType.str);
	srcStruct->Vender.str		= TCFG_EEPROM_Get_Vender_String();
	srcStruct->Vender.len		= strlen((const char*)srcStruct->Vender.str);
	srcStruct->Hard.str			= TCFG_Utility_Get_Hardwear_Version_String();
	srcStruct->Hard.len			= strlen((const char*)srcStruct->Hard.str);
	memset((void *)AepBasicInfoString.Soft, 0x0, sizeof(AepBasicInfoString.Soft));
	sprintf(AepBasicInfoString.Soft, "%d:%d.%d", TCFG_EEPROM_GetBootVersion(), TCFG_Utility_Get_Major_Softnumber(), TCFG_Utility_Get_Sub_Softnumber());
	srcStruct->Soft.str			= AepBasicInfoString.Soft;
	srcStruct->Soft.len			= strlen((const char*)srcStruct->Soft.str);
	srcStruct->Sim.str			= TCFG_Utility_Get_Nbiot_Iccid_String();
	srcStruct->Sim.len			= strlen((const char*)srcStruct->Sim.str);
	srcStruct->Imei.str			= TCFG_Utility_Get_Nbiot_Imei_String();
	srcStruct->Imei.len			= strlen((const char*)srcStruct->Imei.str);
	srcStruct->Imsi.str			= TCFG_Utility_Get_Nbiot_Imsi_String();
	srcStruct->Imsi.len			= strlen((const char*)srcStruct->Imsi.str);
	srcStruct->Nbvender.str		= TCFG_Utility_Get_Nbiot_Manufacturer();
	srcStruct->Nbvender.len		= strlen((const char*)srcStruct->Nbvender.str);
	srcStruct->Nbmode.str		= TCFG_Utility_Get_Nbiot_Manufacturermode();
	srcStruct->Nbmode.len		= strlen((const char*)srcStruct->Nbmode.str);
	memset((void *)AepBasicInfoString.Boot, 0x0, sizeof(AepBasicInfoString.Boot));
	sprintf(AepBasicInfoString.Boot, "%d.%d.%d", TCFG_Utility_Get_SoftResetFlag(), TCFG_Utility_Get_Device_BootCount(), TCFG_EEPROM_GetDeviceRbtMode());
	srcStruct->Boot.str			= AepBasicInfoString.Boot;
	srcStruct->Boot.len			= strlen((const char*)srcStruct->Boot.str);
	srcStruct->Ver.str			= TCFG_Utility_Get_Nbiot_ModelVersion();
	srcStruct->Ver.len			= strlen((const char*)srcStruct->Ver.str);
	srcStruct->Rmold			= Radar_GetModel();
	srcStruct->Rvcc			= Radar_Get_RadarVcc();
}

/**********************************************************************************************************
 @Function			void CTWing_Message_Operate_Creat_Dynamic_Info(CTWING_ClientsTypeDef* pClient, AepDynamicInfo * srcStruct)
 @Description			CTWing_Message_Operate_Creat_Dynamic_Info	: 填写DynamicInfo数据
 @Input				pClient								: CTWing客户端实例
					srcStruct
 @Return				void
**********************************************************************************************************/
void CTWing_Message_Operate_Creat_Dynamic_Info(CTWING_ClientsTypeDef* pClient, AepDynamicInfo * srcStruct)
{
	sprintf(srcStruct->SN, "%08x", TCFG_EEPROM_Get_MAC_SN());
	srcStruct->RT				= TCFG_Utility_Get_Run_Time() / 60 / 60;
	srcStruct->Batt			= TCFG_Utility_Get_Device_Batt_ShortVal();
	memset((void *)AepDynamicInfoString.RAlib, 0x0, sizeof(AepDynamicInfoString.RAlib));
	sprintf(AepDynamicInfoString.RAlib, "[%d,%d]", TCFG_Utility_Get_RadarLibNum(), TCFG_Utility_Get_AlgoLibNum());
	srcStruct->RAlib.str		= AepDynamicInfoString.RAlib;
	srcStruct->RAlib.len		= strlen((const char*)srcStruct->RAlib.str);
	srcStruct->Rcnt			= TCFG_GetRadarCount();
	srcStruct->Temp			= TCFG_Utility_Get_Device_Temperature();
	srcStruct->Qmcrbt			= TCFG_Utility_Get_ReInitModuleCount();
	memset((void *)AepDynamicInfoString.Nbrun, 0x0, sizeof(AepDynamicInfoString.Nbrun));
	sprintf(AepDynamicInfoString.Nbrun, "[%d,%d,%d,%d,%d]", TCFG_Utility_Get_Nbiot_BootCount(), TCFG_Utility_Get_Nbiot_SentCount(), TCFG_Utility_Get_Nbiot_RecvCount(), TCFG_Utility_Get_NBIot_SentCountLimit(), (TCFG_EEPROM_GetNbiotHeart() * 15));
	srcStruct->Nbrun.str		= AepDynamicInfoString.Nbrun;
	srcStruct->Nbrun.len		= strlen((const char*)srcStruct->Nbrun.str);
	srcStruct->Indelay			= TCFG_EEPROM_GetCarInDelay();
	memset((void *)AepDynamicInfoString.Gain, 0x0, sizeof(AepDynamicInfoString.Gain));
	sprintf(AepDynamicInfoString.Gain, "[%d,%d]", TCFG_Utility_Get_GainCover(), TCFG_EEPROM_GetRadarGain());
	srcStruct->Gain.str			= AepDynamicInfoString.Gain;
	srcStruct->Gain.len			= strlen((const char*)srcStruct->Gain.str);
	srcStruct->Smode			= TCFG_EEPROM_GetSenseMode();
	srcStruct->Sinter			= Radar_Get_SampleInterval();
	srcStruct->hpass			= tradar_get_highpass();
	memset((void *)AepDynamicInfoString.x, 0x0, sizeof(AepDynamicInfoString.x));
	sprintf(AepDynamicInfoString.x, "[%d,%d,%d,%d,%d]", TCFG_EEPROM_GetMagManualBack(0, TCFG_X_AXIS),
											  TCFG_EEPROM_GetMagManualBack(1, TCFG_X_AXIS),
											  TCFG_EEPROM_GetMagManualBack(2, TCFG_X_AXIS),
											  TCFG_EEPROM_GetMagManualBack(3, TCFG_X_AXIS),
											  TCFG_EEPROM_GetMagManualBack(4, TCFG_X_AXIS));
	srcStruct->x.str			= AepDynamicInfoString.x;
	srcStruct->x.len			= strlen((const char*)srcStruct->x.str);
	memset((void *)AepDynamicInfoString.y, 0x0, sizeof(AepDynamicInfoString.y));
	sprintf(AepDynamicInfoString.y, "[%d,%d,%d,%d,%d]", TCFG_EEPROM_GetMagManualBack(0, TCFG_Y_AXIS),
											  TCFG_EEPROM_GetMagManualBack(1, TCFG_Y_AXIS),
											  TCFG_EEPROM_GetMagManualBack(2, TCFG_Y_AXIS),
											  TCFG_EEPROM_GetMagManualBack(3, TCFG_Y_AXIS),
											  TCFG_EEPROM_GetMagManualBack(4, TCFG_Y_AXIS));
	srcStruct->y.str			= AepDynamicInfoString.y;
	srcStruct->y.len			= strlen((const char*)srcStruct->y.str);
	
	memset((void *)AepDynamicInfoString.z, 0x0, sizeof(AepDynamicInfoString.z));
	sprintf(AepDynamicInfoString.z, "[%d,%d,%d,%d,%d]", Qmc5883lHistoryBack.Z_Back[0],
											  Qmc5883lHistoryBack.Z_Back[1],
											  Qmc5883lHistoryBack.Z_Back[2],
											  Qmc5883lHistoryBack.Z_Back[3],
											  Qmc5883lHistoryBack.Z_Back[4]);
	srcStruct->z.str			= AepDynamicInfoString.z;
	srcStruct->z.len			= strlen((const char*)srcStruct->z.str);
}
#endif

#if CTWING_AEPMODULE_TYPE == CTWING_AEPMODULE_MVB_VD33D_P2_1
/**********************************************************************************************************
 @Function			AepString CTWing_SpotStatusData_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepSpotStatusData srcStruct)
 @Description			CTWing_SpotStatusData_CodeDataReport		: 序列化SpotStatusData
 @Input				pClient								: CTWing客户端实例
					srcStruct
 @Return				void
**********************************************************************************************************/
AepString CTWing_SpotStatusData_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepSpotStatusData srcStruct)
{
	char* index;
	AepString resultStruct;
	
	unsigned short tempLen;
	unsigned short payloadLen = 39 + (srcStruct.Radarval.len + 2) + (srcStruct.Radarback.len + 2);
	
	memset((void *)pClient->AepMallocProcessStack, 0x0, sizeof(pClient->AepMallocProcessStack));
	
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)pClient->AepMallocProcessStack;
	
	srcStruct.DateTime					= aep_htoni(srcStruct.DateTime);
	srcStruct.SpotCount					= aep_htons(srcStruct.SpotCount);
	srcStruct.MagneticX					= aep_htons(srcStruct.MagneticX);
	srcStruct.MagneticY					= aep_htons(srcStruct.MagneticY);
	srcStruct.MagneticZ					= aep_htons(srcStruct.MagneticZ);
	srcStruct.MagneticDiff				= aep_htons(srcStruct.MagneticDiff);
	srcStruct.MagneticBackX				= aep_htons(srcStruct.MagneticBackX);
	srcStruct.MagneticBackY				= aep_htons(srcStruct.MagneticBackY);
	srcStruct.MagneticBackZ				= aep_htons(srcStruct.MagneticBackZ);
	srcStruct.Debugval					= aep_htons(srcStruct.Debugval);
	
	index = resultStruct.str;
	
	memcpy(index, "02", 2);
	index += 1 * 2;
	
	tempLen = aep_htons(AEP_SERVICE_ID_SPOTSTATUSDATA);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)srcStruct.SN, 8);
	index += 8 * 2;
	
	HexToStr(index, (char *)&srcStruct.DateTime, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.SpotCount, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.SpotStatus, 1);
	index += 1 * 2;
	
	HexToStr(index, (char *)&srcStruct.MagneticX, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.MagneticY, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.MagneticZ, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.MagneticDiff, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.RadarDistance, 1);
	index += 1 * 2;
	
	HexToStr(index, (char *)&srcStruct.RadarStrength, 1);
	index += 1 * 2;
	
	HexToStr(index, (char *)&srcStruct.RadarCoverCount, 1);
	index += 1 * 2;
	
	HexToStr(index, (char *)&srcStruct.RadarDiff, 1);
	index += 1 * 2;
	
	HexToStr(index, (char *)&srcStruct.NBRssi, 1);
	index += 1 * 2;
	
	HexToStr(index, (char *)&srcStruct.NBSnr, 1);
	index += 1 * 2;
	
	HexToStr(index, (char *)&srcStruct.MCUTemp, 1);
	index += 1 * 2;
	
	HexToStr(index, (char *)&srcStruct.QMCTemp, 1);
	index += 1 * 2;
	
	HexToStr(index, (char *)&srcStruct.MagneticBackX, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.MagneticBackY, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.MagneticBackZ, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.Debugval, 2);
	index += 2 * 2;
	
	tempLen = aep_htons(srcStruct.Radarval.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.Radarval.str, srcStruct.Radarval.len);
	index += srcStruct.Radarval.len * 2;
	
	tempLen = aep_htons(srcStruct.Radarback.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.Radarback.str, srcStruct.Radarback.len);
	index += srcStruct.Radarback.len * 2;
	
	return resultStruct;
}

/**********************************************************************************************************
 @Function			AepString CTWing_WorkInfo_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepWorkInfo srcStruct)
 @Description			CTWing_WorkInfo_CodeDataReport			: 序列化WorkInfo
 @Input				pClient								: CTWing客户端实例
					srcStruct
 @Return				void
**********************************************************************************************************/
AepString CTWing_WorkInfo_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepWorkInfo srcStruct)
{
	char* index;
	AepString resultStruct;
	
	unsigned short tempLen;
	unsigned short payloadLen = 26 + (srcStruct.WorkMode.len + 2) + (srcStruct.Cmdcnt.len + 2) + (srcStruct.Nbruntime.len + 2) + (srcStruct.APN.len + 2) + (srcStruct.Coef.len + 2);
	
	memset((void *)pClient->AepMallocProcessStack, 0x0, sizeof(pClient->AepMallocProcessStack));
	
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)pClient->AepMallocProcessStack;
	
	srcStruct.Range					= aep_htons(srcStruct.Range);
	srcStruct.Earfcn					= aep_htoni(srcStruct.Earfcn);
	srcStruct.Tac						= aep_htoni(srcStruct.Tac);
	srcStruct.Ci						= aep_htoni(srcStruct.Ci);
	
	index = resultStruct.str;
	
	memcpy(index, "02", 2);
	index += 1 * 2;
	
	tempLen = aep_htons(AEP_SERVICE_ID_WORKINFO);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)srcStruct.SN, 8);
	index += 8 * 2;
	
	HexToStr(index, (char *)&srcStruct.Sense, 1);
	index += 1 * 2;
	
	tempLen = aep_htons(srcStruct.WorkMode.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.WorkMode.str, srcStruct.WorkMode.len);
	index += srcStruct.WorkMode.len * 2;
	
	HexToStr(index, (char *)&srcStruct.RfChannel, 1);
	index += 1 * 2;
	
	HexToStr(index, (char *)&srcStruct.Range, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.Earfcn, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.Tac, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.Ci, 4);
	index += 4 * 2;
	
	tempLen = aep_htons(srcStruct.Cmdcnt.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.Cmdcnt.str, srcStruct.Cmdcnt.len);
	index += srcStruct.Cmdcnt.len * 2;
	
	tempLen = aep_htons(srcStruct.Nbruntime.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.Nbruntime.str, srcStruct.Nbruntime.len);
	index += srcStruct.Nbruntime.len * 2;
	
	tempLen = aep_htons(srcStruct.APN.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.APN.str, srcStruct.APN.len);
	index += srcStruct.APN.len * 2;
	
	tempLen = aep_htons(srcStruct.Coef.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.Coef.str, srcStruct.Coef.len);
	index += srcStruct.Coef.len * 2;
	
	HexToStr(index, (char *)&srcStruct.Rollinit, 1);
	index += 1 * 2;
	
	HexToStr(index, (char *)&srcStruct.RadioRv, 1);
	index += 1 * 2;
	
	return resultStruct;
}

/**********************************************************************************************************
 @Function			AepString CTWing_BasicInfo_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepBasicInfo srcStruct)
 @Description			CTWing_BasicInfo_CodeDataReport			: 序列化BasicInfo
 @Input				pClient								: CTWing客户端实例
					srcStruct
 @Return				void
**********************************************************************************************************/
AepString CTWing_BasicInfo_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepBasicInfo srcStruct)
{
	char* index;
	AepString resultStruct;
	
	unsigned short tempLen;
	unsigned short payloadLen = 10 + (srcStruct.ModelType.len + 2) 
							 + (srcStruct.Vender.len + 2) 
							 + (srcStruct.Hard.len + 2) 
							 + (srcStruct.Soft.len + 2) 
							 + (srcStruct.Sim.len + 2) 
							 + (srcStruct.Imei.len + 2) 
							 + (srcStruct.Imsi.len + 2) 
							 + (srcStruct.Nbvender.len + 2) 
							 + (srcStruct.Nbmode.len + 2) 
							 + (srcStruct.Boot.len + 2) 
							 + (srcStruct.Ver.len + 2);
	
	memset((void *)pClient->AepMallocProcessStack, 0x0, sizeof(pClient->AepMallocProcessStack));
	
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)pClient->AepMallocProcessStack;
	
	index = resultStruct.str;
	
	memcpy(index, "02", 2);
	index += 1 * 2;
	
	tempLen = aep_htons(AEP_SERVICE_ID_BASICINFO);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)srcStruct.SN, 8);
	index += 8 * 2;
	
	tempLen = aep_htons(srcStruct.ModelType.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.ModelType.str, srcStruct.ModelType.len);
	index += srcStruct.ModelType.len * 2;
	
	tempLen = aep_htons(srcStruct.Vender.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.Vender.str, srcStruct.Vender.len);
	index += srcStruct.Vender.len * 2;
	
	tempLen = aep_htons(srcStruct.Hard.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.Hard.str, srcStruct.Hard.len);
	index += srcStruct.Hard.len * 2;
	
	tempLen = aep_htons(srcStruct.Soft.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.Soft.str, srcStruct.Soft.len);
	index += srcStruct.Soft.len * 2;
	
	tempLen = aep_htons(srcStruct.Sim.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.Sim.str, srcStruct.Sim.len);
	index += srcStruct.Sim.len * 2;
	
	tempLen = aep_htons(srcStruct.Imei.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.Imei.str, srcStruct.Imei.len);
	index += srcStruct.Imei.len * 2;
	
	tempLen = aep_htons(srcStruct.Imsi.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.Imsi.str, srcStruct.Imsi.len);
	index += srcStruct.Imsi.len * 2;
	
	tempLen = aep_htons(srcStruct.Nbvender.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.Nbvender.str, srcStruct.Nbvender.len);
	index += srcStruct.Nbvender.len * 2;
	
	tempLen = aep_htons(srcStruct.Nbmode.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.Nbmode.str, srcStruct.Nbmode.len);
	index += srcStruct.Nbmode.len * 2;
	
	tempLen = aep_htons(srcStruct.Boot.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.Boot.str, srcStruct.Boot.len);
	index += srcStruct.Boot.len * 2;
	
	tempLen = aep_htons(srcStruct.Ver.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.Ver.str, srcStruct.Ver.len);
	index += srcStruct.Ver.len * 2;
	
	HexToStr(index, (char *)&srcStruct.Rmold, 1);
	index += 1 * 2;
	
	HexToStr(index, (char *)&srcStruct.Rvcc, 1);
	index += 1 * 2;
	
	return resultStruct;
}

/**********************************************************************************************************
 @Function			AepString CTWing_DynamicInfo_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepDynamicInfo srcStruct)
 @Description			CTWing_DynamicInfo_CodeDataReport			: 序列化DynamicInfo
 @Input				pClient								: CTWing客户端实例
					srcStruct
 @Return				void
**********************************************************************************************************/
AepString CTWing_DynamicInfo_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepDynamicInfo srcStruct)
{
	char* index;
	AepString resultStruct;
	
	unsigned short tempLen;
	unsigned short payloadLen = 26 + (srcStruct.RAlib.len + 2) + (srcStruct.Nbrun.len + 2) + (srcStruct.Gain.len + 2) + (srcStruct.x.len + 2) + (srcStruct.y.len + 2) + (srcStruct.z.len + 2);
	
	memset((void *)pClient->AepMallocProcessStack, 0x0, sizeof(pClient->AepMallocProcessStack));
	
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)pClient->AepMallocProcessStack;
	
	srcStruct.RT						= aep_htoni(srcStruct.RT);
	srcStruct.Batt						= aep_htons(srcStruct.Batt);
	srcStruct.Rcnt						= aep_htoni(srcStruct.Rcnt);
	srcStruct.Temp						= aep_htons(srcStruct.Temp);
	srcStruct.Qmcrbt					= aep_htons(srcStruct.Qmcrbt);
	
	index = resultStruct.str;
	
	memcpy(index, "02", 2);
	index += 1 * 2;
	
	tempLen = aep_htons(AEP_SERVICE_ID_DYNAMICINFO);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)srcStruct.SN, 8);
	index += 8 * 2;
	
	HexToStr(index, (char *)&srcStruct.RT, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.Batt, 2);
	index += 2 * 2;
	
	tempLen = aep_htons(srcStruct.RAlib.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.RAlib.str, srcStruct.RAlib.len);
	index += srcStruct.RAlib.len * 2;
	
	HexToStr(index, (char *)&srcStruct.Rcnt, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.Temp, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.Qmcrbt, 2);
	index += 2 * 2;
	
	tempLen = aep_htons(srcStruct.Nbrun.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.Nbrun.str, srcStruct.Nbrun.len);
	index += srcStruct.Nbrun.len * 2;
	
	HexToStr(index, (char *)&srcStruct.Indelay, 1);
	index += 1 * 2;
	
	tempLen = aep_htons(srcStruct.Gain.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.Gain.str, srcStruct.Gain.len);
	index += srcStruct.Gain.len * 2;
	
	HexToStr(index, (char *)&srcStruct.Smode, 1);
	index += 1 * 2;
	
	HexToStr(index, (char *)&srcStruct.Sinter, 1);
	index += 1 * 2;
	
	HexToStr(index, (char *)&srcStruct.hpass, 1);
	index += 1 * 2;
	
	tempLen = aep_htons(srcStruct.x.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.x.str, srcStruct.x.len);
	index += srcStruct.x.len * 2;
	
	tempLen = aep_htons(srcStruct.y.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.y.str, srcStruct.y.len);
	index += srcStruct.y.len * 2;
	
	tempLen = aep_htons(srcStruct.z.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.z.str, srcStruct.z.len);
	index += srcStruct.z.len * 2;
	
	return resultStruct;
}
#endif

#if CTWING_AEPMODULE_TYPE == CTWING_AEPMODULE_MVB_VD33D_P2_2
/**********************************************************************************************************
 @Function			void CTWing_Message_Operate_Creat_Data_Report(CTWING_ClientsTypeDef* pClient, AepDataReportData * srcStruct)
 @Description			CTWing_Message_Operate_Creat_Data_Report	: 填写DataReport数据
 @Input				pClient								: CTWing客户端实例
					srcStruct
 @Return				void
**********************************************************************************************************/
void CTWing_Message_Operate_Creat_Data_Report(CTWING_ClientsTypeDef* pClient, AepDataReportData * srcStruct)
{
	srcStruct->battery_voltage	= ((float)TCFG_Utility_Get_Device_Batt_ShortVal() / 100.0);
	srcStruct->battery_value		= (100 - (int)(TCFG_Utility_Get_Run_Time() / 946080)) < 0 ? 0 : (100 - (int)(TCFG_Utility_Get_Run_Time() / 946080));
	srcStruct->error_code		= 0;
}

/**********************************************************************************************************
 @Function			void CTWing_Message_Operate_Creat_Signal_Report(CTWING_ClientsTypeDef* pClient, AepSignalReportData * srcStruct)
 @Description			CTWing_Message_Operate_Creat_Signal_Report	: 填写SignalReport数据
 @Input				pClient								: CTWing客户端实例
					srcStruct
 @Return				void
**********************************************************************************************************/
void CTWing_Message_Operate_Creat_Signal_Report(CTWING_ClientsTypeDef* pClient, AepSignalReportData * srcStruct)
{
	srcStruct->rsrp			= TCFG_Utility_Get_Nbiot_CellRsrp();
	srcStruct->sinr			= TCFG_Utility_Get_Nbiot_CellSnr();
	srcStruct->pci				= TCFG_Utility_Get_Nbiot_RadioPCI();
	srcStruct->ecl				= TCFG_Utility_Get_Nbiot_RadioECL();
	srcStruct->cell_id			= TCFG_Utility_Get_Nbiot_RadioCellID();
}

/**********************************************************************************************************
 @Function			void CTWing_Message_Operate_Creat_Heart_Beat(CTWING_ClientsTypeDef* pClient, AepHeartBeatData * srcStruct)
 @Description			CTWing_Message_Operate_Creat_Heart_Beat		: 填写HeartBeat数据
 @Input				pClient								: CTWing客户端实例
					srcStruct
 @Return				void
**********************************************************************************************************/
void CTWing_Message_Operate_Creat_Heart_Beat(CTWING_ClientsTypeDef* pClient, AepHeartBeatData * srcStruct)
{
	memset((void *)srcStruct->ICCID, 0x0, sizeof(srcStruct->ICCID));
	memcpy((void *)srcStruct->ICCID, (void *)TCFG_Utility_Get_Nbiot_Iccid_String(), sizeof(srcStruct->ICCID));
	srcStruct->heartbeat_time	= (float)(TCFG_EEPROM_GetNbiotHeart() * 900) / (float)(60 * 60);
	srcStruct->protect_time		= (float)(CTWING_LWM2M_LIFETIME) / (float)(60 * 60);
	memset((void *)srcStruct->saas_version, 0x0, sizeof(srcStruct->saas_version));
	memcpy((void *)srcStruct->saas_version, (void *)"0.0.01", sizeof(srcStruct->saas_version));
	memset((void *)srcStruct->manufacturer_id, 0x0, sizeof(srcStruct->manufacturer_id));
	memcpy((void *)srcStruct->manufacturer_id, (void *)"91330104563048753R", sizeof(srcStruct->manufacturer_id));
	memset((void *)srcStruct->IMSI, 0x0, sizeof(srcStruct->IMSI));
	memcpy((void *)srcStruct->IMSI, (void *)TCFG_Utility_Get_Nbiot_Imsi_String(), sizeof(srcStruct->IMSI));
	memset((void *)srcStruct->IMEI, 0x0, sizeof(srcStruct->IMEI));
	memcpy((void *)srcStruct->IMEI, (void *)TCFG_Utility_Get_Nbiot_Imei_String(), sizeof(srcStruct->IMEI));
	
	srcStruct->nb_firmware.str		= TCFG_Utility_Get_Nbiot_ModelVersion();
	srcStruct->nb_firmware.len		= strlen((const char*)srcStruct->nb_firmware.str);
	srcStruct->nb_module.str			= TCFG_Utility_Get_Nbiot_Manufacturermode();
	srcStruct->nb_module.len			= strlen((const char*)srcStruct->nb_module.str);
	srcStruct->mcu_firmware.str		= TCFG_Utility_Get_Softwear_Version_String();
	srcStruct->mcu_firmware.len		= strlen((const char*)srcStruct->mcu_firmware.str);
	srcStruct->terminal_type.str		= TCFG_Utility_Get_Hardwear_Version_String();
	srcStruct->terminal_type.len		= strlen((const char*)srcStruct->terminal_type.str);
	srcStruct->manufacturer_name.str	= TCFG_EEPROM_Get_Vender_String();
	srcStruct->manufacturer_name.len	= strlen((const char*)srcStruct->manufacturer_name.str);
	
	srcStruct->rsrp			= TCFG_Utility_Get_Nbiot_CellRsrp();
	srcStruct->sinr			= TCFG_Utility_Get_Nbiot_CellSnr();
	srcStruct->pci				= TCFG_Utility_Get_Nbiot_RadioPCI();
	srcStruct->ecl				= TCFG_Utility_Get_Nbiot_RadioECL();
	srcStruct->cell_id			= TCFG_Utility_Get_Nbiot_RadioCellID();
	
	srcStruct->battery_voltage	= ((float)TCFG_Utility_Get_Device_Batt_ShortVal() / 100.0);
	srcStruct->battery_value		= (100 - (int)(TCFG_Utility_Get_Run_Time() / 946080)) < 0 ? 0 : (100 - (int)(TCFG_Utility_Get_Run_Time() / 946080));
	srcStruct->error_code		= 0;
	
	srcStruct->signal_power		= TCFG_Utility_Get_Nbiot_RadioSignalpower();
	srcStruct->snr				= TCFG_Utility_Get_Nbiot_RadioSNR();
	srcStruct->tx_power			= TCFG_Utility_Get_Nbiot_RadioTXpower();
}
#endif

#if CTWING_AEPMODULE_TYPE == CTWING_AEPMODULE_MVB_VD33D_P2_2
/**********************************************************************************************************
 @Function			AepString CTWing_DataReport_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepDataReportData srcStruct)
 @Description			CTWing_DataReport_CodeDataReport			: 序列化DataReport
 @Input				pClient								: CTWing客户端实例
					srcStruct
 @Return				void
**********************************************************************************************************/
AepString CTWing_DataReport_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepDataReportData srcStruct)
{
	char* index;
	AepString resultStruct;
	
	unsigned short tempLen;
	unsigned short payloadLen = 23;
	
	memset((void *)pClient->AepMallocProcessStack, 0x0, sizeof(pClient->AepMallocProcessStack));
	
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)pClient->AepMallocProcessStack;
	
	srcStruct.battery_voltage			= aep_htonf(srcStruct.battery_voltage);
	srcStruct.battery_value				= aep_htoni(srcStruct.battery_value);
	srcStruct.ptime					= aep_htonl(srcStruct.ptime);
	srcStruct.magnetic_value				= aep_htoni(srcStruct.magnetic_value);
	
	index = resultStruct.str;
	
	memcpy(index, "02", 2);
	index += 1 * 2;
	
	tempLen = aep_htons(AEP_SERVICE_ID_DATAREPORT);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.battery_voltage, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.battery_value, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.ptime, 8);
	index += 8 * 2;
	
	HexToStr(index, (char *)&srcStruct.parking_state, 1);
	index += 1 * 2;
	
	HexToStr(index, (char *)&srcStruct.error_code, 1);
	index += 1 * 2;
	
	HexToStr(index, (char *)&srcStruct.parking_change, 1);
	index += 1 * 2;
	
	HexToStr(index, (char *)&srcStruct.magnetic_value, 4);
	index += 4 * 2;
	
	return resultStruct;
}

/**********************************************************************************************************
 @Function			AepString CTWing_SignalReport_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepSignalReportData srcStruct)
 @Description			CTWing_SignalReport_CodeDataReport			: 序列化SignalReport
 @Input				pClient								: CTWing客户端实例
					srcStruct
 @Return				void
**********************************************************************************************************/
AepString CTWing_SignalReport_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepSignalReportData srcStruct)
{
	char* index;
	AepString resultStruct;
	
	unsigned short tempLen;
	unsigned short payloadLen = 20;
	
	memset((void *)pClient->AepMallocProcessStack, 0x0, sizeof(pClient->AepMallocProcessStack));
	
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)pClient->AepMallocProcessStack;
	
	srcStruct.rsrp						= aep_htoni(srcStruct.rsrp);
	srcStruct.sinr						= aep_htoni(srcStruct.sinr);
	srcStruct.pci						= aep_htoni(srcStruct.pci);
	srcStruct.ecl						= aep_htoni(srcStruct.ecl);
	srcStruct.cell_id					= aep_htoni(srcStruct.cell_id);
	
	index = resultStruct.str;
	
	memcpy(index, "02", 2);
	index += 1 * 2;
	
	tempLen = aep_htons(AEP_SERVICE_ID_SIGNALREPORT);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.rsrp, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.sinr, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.pci, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.ecl, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.cell_id, 4);
	index += 4 * 2;
	
	return resultStruct;
}

/**********************************************************************************************************
 @Function			AepString CTWing_HeartBeat_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepHeartBeatData srcStruct)
 @Description			CTWing_HeartBeat_CodeDataReport			: 序列化HeartBeat
 @Input				pClient								: CTWing客户端实例
					srcStruct
 @Return				void
**********************************************************************************************************/
AepString CTWing_HeartBeat_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepHeartBeatData srcStruct)
{
	char* index;
	AepString resultStruct;
	
	unsigned short tempLen;
	unsigned short payloadLen = 123 + (srcStruct.nb_firmware.len + 2) + (srcStruct.nb_module.len + 2) + (srcStruct.mcu_firmware.len + 2) + (srcStruct.terminal_type.len + 2) + (srcStruct.manufacturer_name.len + 2);
	
	memset((void *)pClient->AepMallocProcessStack, 0x0, sizeof(pClient->AepMallocProcessStack));
	
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)pClient->AepMallocProcessStack;
	
	srcStruct.heartbeat_time				= aep_htonf(srcStruct.heartbeat_time);
	srcStruct.protect_time				= aep_htonf(srcStruct.protect_time);
	srcStruct.rsrp						= aep_htoni(srcStruct.rsrp);
	srcStruct.sinr						= aep_htoni(srcStruct.sinr);
	srcStruct.pci						= aep_htoni(srcStruct.pci);
	srcStruct.ecl						= aep_htoni(srcStruct.ecl);
	srcStruct.cell_id					= aep_htoni(srcStruct.cell_id);
	srcStruct.battery_voltage			= aep_htonf(srcStruct.battery_voltage);
	srcStruct.battery_value				= aep_htoni(srcStruct.battery_value);
	srcStruct.signal_power				= aep_htons(srcStruct.signal_power);
	srcStruct.snr						= aep_htons(srcStruct.snr);
	srcStruct.tx_power					= aep_htons(srcStruct.tx_power);
	srcStruct.magnetic_value				= aep_htoni(srcStruct.magnetic_value);
	
	index = resultStruct.str;
	
	memcpy(index, "02", 2);
	index += 1 * 2;
	
	tempLen = aep_htons(AEP_SERVICE_ID_HEARTBEAT);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)srcStruct.ICCID, 20);
	index += 20 * 2;
	
	HexToStr(index, (char *)&srcStruct.heartbeat_time, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.protect_time, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)srcStruct.saas_version, 6);
	index += 6 * 2;
	
	HexToStr(index, (char *)srcStruct.manufacturer_id, 18);
	index += 18 * 2;
	
	HexToStr(index, (char *)srcStruct.IMSI, 15);
	index += 15 * 2;
	
	HexToStr(index, (char *)srcStruct.IMEI, 15);
	index += 15 * 2;
	
	tempLen = aep_htons(srcStruct.nb_firmware.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.nb_firmware.str, srcStruct.nb_firmware.len);
	index += srcStruct.nb_firmware.len * 2;
	
	tempLen = aep_htons(srcStruct.nb_module.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.nb_module.str, srcStruct.nb_module.len);
	index += srcStruct.nb_module.len * 2;
	
	tempLen = aep_htons(srcStruct.mcu_firmware.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.mcu_firmware.str, srcStruct.mcu_firmware.len);
	index += srcStruct.mcu_firmware.len * 2;
	
	tempLen = aep_htons(srcStruct.terminal_type.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.terminal_type.str, srcStruct.terminal_type.len);
	index += srcStruct.terminal_type.len * 2;
	
	tempLen = aep_htons(srcStruct.manufacturer_name.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.manufacturer_name.str, srcStruct.manufacturer_name.len);
	index += srcStruct.manufacturer_name.len * 2;
	
	HexToStr(index, (char *)&srcStruct.rsrp, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.sinr, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.pci, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.ecl, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.cell_id, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.battery_voltage, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.battery_value, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.signal_power, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.snr, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.tx_power, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.parking_state, 1);
	index += 1 * 2;
	
	HexToStr(index, (char *)&srcStruct.error_code, 1);
	index += 1 * 2;
	
	HexToStr(index, (char *)&srcStruct.parking_change, 1);
	index += 1 * 2;
	
	HexToStr(index, (char *)&srcStruct.magnetic_value, 4);
	index += 4 * 2;
	
	return resultStruct;
}

/**********************************************************************************************************
 @Function			AepString CTWing_ParkingChangeInfo_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepParkingChangeInfoData srcStruct)
 @Description			CTWing_ParkingChangeInfo_CodeDataReport		: 序列化ParkingChangeInfo
 @Input				pClient								: CTWing客户端实例
					srcStruct
 @Return				void
**********************************************************************************************************/
AepString CTWing_ParkingChangeInfo_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepParkingChangeInfoData srcStruct)
{
	char* index;
	AepString resultStruct;
	
	unsigned short tempLen;
	unsigned short payloadLen = 1;
	
	memset((void *)pClient->AepMallocProcessStack, 0x0, sizeof(pClient->AepMallocProcessStack));
	
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)pClient->AepMallocProcessStack;
	
	index = resultStruct.str;
	
	memcpy(index, "07", 2);
	index += 1 * 2;
	
	tempLen = aep_htons(AEP_SERVICE_ID_PARKINGCHANGE);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.parking_change, 1);
	index += 1 * 2;
	
	return resultStruct;
}

/**********************************************************************************************************
 @Function			AepString CTWing_ErrorCodeReport_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepErrorCodeReportData srcStruct)
 @Description			CTWing_ErrorCodeReport_CodeDataReport		: 序列化ErrorCodeReport
 @Input				pClient								: CTWing客户端实例
					srcStruct
 @Return				void
**********************************************************************************************************/
AepString CTWing_ErrorCodeReport_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepErrorCodeReportData srcStruct)
{
	char* index;
	AepString resultStruct;
	
	unsigned short tempLen;
	unsigned short payloadLen = 1;
	
	memset((void *)pClient->AepMallocProcessStack, 0x0, sizeof(pClient->AepMallocProcessStack));
	
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)pClient->AepMallocProcessStack;
	
	index = resultStruct.str;
	
	memcpy(index, "07", 2);
	index += 1 * 2;
	
	tempLen = aep_htons(AEP_SERVICE_ID_ERRORCODE);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.error_code, 1);
	index += 1 * 2;
	
	return resultStruct;
}

/**********************************************************************************************************
 @Function			AepString CTWing_LowVoltageAlarm_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepLowVoltageAlarmData srcStruct)
 @Description			CTWing_LowVoltageAlarm_CodeDataReport		: 序列化LowVoltageAlarm
 @Input				pClient								: CTWing客户端实例
					srcStruct
 @Return				void
**********************************************************************************************************/
AepString CTWing_LowVoltageAlarm_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepLowVoltageAlarmData srcStruct)
{
	char* index;
	AepString resultStruct;
	
	unsigned short tempLen;
	unsigned short payloadLen = 4;
	
	memset((void *)pClient->AepMallocProcessStack, 0x0, sizeof(pClient->AepMallocProcessStack));
	
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)pClient->AepMallocProcessStack;
	
	index = resultStruct.str;
	
	memcpy(index, "07", 2);
	index += 1 * 2;
	
	tempLen = aep_htons(AEP_SERVICE_ID_LOWVOLTAGE);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.battery_voltage, 4);
	index += 4 * 2;
	
	return resultStruct;
}

/**********************************************************************************************************
 @Function			AepString CTWing_MagneticDisturb_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepMagneticDisturbData srcStruct)
 @Description			CTWing_MagneticDisturb_CodeDataReport		: 序列化MagneticDisturb
 @Input				pClient								: CTWing客户端实例
					srcStruct
 @Return				void
**********************************************************************************************************/
AepString CTWing_MagneticDisturb_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepMagneticDisturbData srcStruct)
{
	char* index;
	AepString resultStruct;
	
	unsigned short tempLen;
	unsigned short payloadLen = 4;
	
	memset((void *)pClient->AepMallocProcessStack, 0x0, sizeof(pClient->AepMallocProcessStack));
	
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)pClient->AepMallocProcessStack;
	
	index = resultStruct.str;
	
	memcpy(index, "07", 2);
	index += 1 * 2;
	
	tempLen = aep_htons(AEP_SERVICE_ID_MAGNETICDISTURB);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.magnetic_value, 4);
	index += 4 * 2;
	
	return resultStruct;
}
#endif

#if CTWING_AEPMODULE_TYPE == CTWING_AEPMODULE_MVB_VD33D_P2_3
/**********************************************************************************************************
 @Function			void CTWing_Message_Operate_Creat_Extra_Report(CTWING_ClientsTypeDef* pClient, AepExtraReportData * srcStruct)
 @Description			CTWing_Message_Operate_Creat_Extra_Report	: 填写ExtraReport数据
 @Input				pClient								: CTWing客户端实例
					srcStruct
 @Return				void
**********************************************************************************************************/
void CTWing_Message_Operate_Creat_Extra_Report(CTWING_ClientsTypeDef* pClient, AepExtraReportData * srcStruct)
{
	srcStruct->battery_voltage				= 3.6;
	srcStruct->battery_value					= 100;
	srcStruct->error_code					= 0;
}

/**********************************************************************************************************
 @Function			void CTWing_Message_Operate_Creat_Heart_Beat(CTWING_ClientsTypeDef* pClient, AepHeartBeatData * srcStruct)
 @Description			CTWing_Message_Operate_Creat_Heart_Beat		: 填写HeartBeat数据
 @Input				pClient								: CTWing客户端实例
					srcStruct
 @Return				void
**********************************************************************************************************/
void CTWing_Message_Operate_Creat_Heart_Beat(CTWING_ClientsTypeDef* pClient, AepHeartBeatData * srcStruct)
{
	srcStruct->battery_voltage				= 3.6;
	srcStruct->battery_value					= 100;
	srcStruct->error_code					= 0;
	
	memset((void *)srcStruct->ICCID,			0x0,								sizeof(srcStruct->ICCID));
	memcpy((void *)srcStruct->ICCID,			(void *)AEP_MODULE_NB_ICCID,			sizeof(srcStruct->ICCID));
	memset((void *)srcStruct->saas_version,		0x0,								sizeof(srcStruct->saas_version));
	memcpy((void *)srcStruct->saas_version, 	(void *)AEP_MODULE_SAAS_VERSION,		sizeof(srcStruct->saas_version));
	memset((void *)srcStruct->manufacturer_id,	0x0,								sizeof(srcStruct->manufacturer_id));
	memcpy((void *)srcStruct->manufacturer_id,	(void *)AEP_MODULE_MANUFACTURER_ID,	sizeof(srcStruct->manufacturer_id));
	memset((void *)srcStruct->IMSI,			0x0,								sizeof(srcStruct->IMSI));
	memcpy((void *)srcStruct->IMSI,			(void *)AEP_MODULE_NB_IMSI,			sizeof(srcStruct->IMSI));
	memset((void *)srcStruct->IMEI,			0x0,								sizeof(srcStruct->IMEI));
	memcpy((void *)srcStruct->IMEI,			(void *)AEP_MODULE_NB_IMEI,			sizeof(srcStruct->IMEI));
	
	srcStruct->nb_firmware.str				= AEP_MODULE_NB_FIRMWARE;
	srcStruct->nb_firmware.len				= strlen((const char*)srcStruct->nb_firmware.str);
	srcStruct->nb_module.str					= AEP_MODULE_NB_MODULE;
	srcStruct->nb_module.len					= strlen((const char*)srcStruct->nb_module.str);
	srcStruct->mcu_firmware.str				= AEP_MODULE_MCU_FIRMWARE;
	srcStruct->mcu_firmware.len				= strlen((const char*)srcStruct->mcu_firmware.str);
	srcStruct->terminal_type.str				= AEP_MODULE_TERMINAL_TYPE;
	srcStruct->terminal_type.len				= strlen((const char*)srcStruct->terminal_type.str);
	srcStruct->manufacturer_name.str			= AEP_MODULE_MANUFACTURER_NAME;
	srcStruct->manufacturer_name.len			= strlen((const char*)srcStruct->manufacturer_name.str);
	
	srcStruct->rsrp						= AEP_MODULE_NB_RSRP;
	srcStruct->sinr						= AEP_MODULE_NB_SINR;
	srcStruct->pci							= AEP_MODULE_NB_PCI;
	srcStruct->ecl							= AEP_MODULE_NB_ECL;
	srcStruct->cell_id						= AEP_MODULE_NB_CELLID;
	
	srcStruct->signal_power					= AEP_MODULE_NB_SIGNALPOWER;
	srcStruct->snr							= AEP_MODULE_NB_SNR;
	srcStruct->tx_power						= AEP_MODULE_NB_TXPOWER;
}

#endif

#if CTWING_AEPMODULE_TYPE == CTWING_AEPMODULE_MVB_VD33D_P2_3
/**********************************************************************************************************
 @Function			AepString CTWing_DataReport_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepDataReportData srcStruct)
 @Description			CTWing_DataReport_CodeDataReport			: 序列化DataReport
 @Input				pClient								: CTWing客户端实例
					srcStruct
 @Return				void
**********************************************************************************************************/
AepString CTWing_DataReport_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepDataReportData srcStruct)
{
	char* index;
	AepString resultStruct;
	
	unsigned short tempLen;
	unsigned short payloadLen = 1;
	
	memset((void *)pClient->AepMallocProcessStack, 0x0, sizeof(pClient->AepMallocProcessStack));
	
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)pClient->AepMallocProcessStack;
	
	index = resultStruct.str;
	
	memcpy(index, "02", 2);
	index += 1 * 2;
	
	tempLen = aep_htons(AEP_SERVICE_ID_DATAREPORT);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.parking_state, 1);
	index += 1 * 2;
	
	return resultStruct;
}

/**********************************************************************************************************
 @Function			AepString CTWing_SignalReport_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepSignalReportData srcStruct)
 @Description			CTWing_SignalReport_CodeDataReport			: 序列化SignalReport
 @Input				pClient								: CTWing客户端实例
					srcStruct
 @Return				void
**********************************************************************************************************/
AepString CTWing_SignalReport_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepSignalReportData srcStruct)
{
	char* index;
	AepString resultStruct;
	
	unsigned short tempLen;
	unsigned short payloadLen = 20;
	
	memset((void *)pClient->AepMallocProcessStack, 0x0, sizeof(pClient->AepMallocProcessStack));
	
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)pClient->AepMallocProcessStack;
	
	srcStruct.rsrp						= aep_htoni(srcStruct.rsrp);
	srcStruct.sinr						= aep_htoni(srcStruct.sinr);
	srcStruct.pci						= aep_htoni(srcStruct.pci);
	srcStruct.ecl						= aep_htoni(srcStruct.ecl);
	srcStruct.cell_id					= aep_htoni(srcStruct.cell_id);
	
	index = resultStruct.str;
	
	memcpy(index, "02", 2);
	index += 1 * 2;
	
	tempLen = aep_htons(AEP_SERVICE_ID_SIGNALREPORT);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.rsrp, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.sinr, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.pci, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.ecl, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.cell_id, 4);
	index += 4 * 2;
	
	return resultStruct;
}

/**********************************************************************************************************
 @Function			AepString CTWing_ExtraReport_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepExtraReportData srcStruct)
 @Description			CTWing_ExtraReport_CodeDataReport			: 序列化ExtraReport
 @Input				pClient								: CTWing客户端实例
					srcStruct
 @Return				void
**********************************************************************************************************/
AepString CTWing_ExtraReport_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepExtraReportData srcStruct)
{
	char* index;
	AepString resultStruct;
	
	unsigned short tempLen;
	unsigned short payloadLen = 23;
	
	memset((void *)pClient->AepMallocProcessStack, 0x0, sizeof(pClient->AepMallocProcessStack));
	
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)pClient->AepMallocProcessStack;
	
	srcStruct.battery_voltage			= aep_htonf(srcStruct.battery_voltage);
	srcStruct.battery_value				= aep_htoni(srcStruct.battery_value);
	srcStruct.ptime					= aep_htonl(srcStruct.ptime);
	srcStruct.magnetic_value				= aep_htoni(srcStruct.magnetic_value);
	
	index = resultStruct.str;
	
	memcpy(index, "02", 2);
	index += 1 * 2;
	
	tempLen = aep_htons(AEP_SERVICE_ID_EXTRAREPORT);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.battery_voltage, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.battery_value, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.ptime, 8);
	index += 8 * 2;
	
	HexToStr(index, (char *)&srcStruct.parking_state, 1);
	index += 1 * 2;
	
	HexToStr(index, (char *)&srcStruct.error_code, 1);
	index += 1 * 2;
	
	HexToStr(index, (char *)&srcStruct.parking_change, 1);
	index += 1 * 2;
	
	HexToStr(index, (char *)&srcStruct.magnetic_value, 4);
	index += 4 * 2;
	
	return resultStruct;
}

/**********************************************************************************************************
 @Function			AepString CTWing_HeartBeat_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepHeartBeatData srcStruct)
 @Description			CTWing_HeartBeat_CodeDataReport			: 序列化HeartBeat
 @Input				pClient								: CTWing客户端实例
					srcStruct
 @Return				void
**********************************************************************************************************/
AepString CTWing_HeartBeat_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepHeartBeatData srcStruct)
{
	char* index;
	AepString resultStruct;
	
	unsigned short tempLen;
	unsigned short payloadLen = 123 + (srcStruct.nb_firmware.len + 2) + (srcStruct.nb_module.len + 2) + (srcStruct.mcu_firmware.len + 2) + (srcStruct.terminal_type.len + 2) + (srcStruct.manufacturer_name.len + 2);
	
	memset((void *)pClient->AepMallocProcessStack, 0x0, sizeof(pClient->AepMallocProcessStack));
	
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)pClient->AepMallocProcessStack;
	
	srcStruct.heartbeat_time				= aep_htonf(srcStruct.heartbeat_time);
	srcStruct.protect_time				= aep_htonf(srcStruct.protect_time);
	srcStruct.rsrp						= aep_htoni(srcStruct.rsrp);
	srcStruct.sinr						= aep_htoni(srcStruct.sinr);
	srcStruct.pci						= aep_htoni(srcStruct.pci);
	srcStruct.ecl						= aep_htoni(srcStruct.ecl);
	srcStruct.cell_id					= aep_htoni(srcStruct.cell_id);
	srcStruct.battery_voltage			= aep_htonf(srcStruct.battery_voltage);
	srcStruct.battery_value				= aep_htoni(srcStruct.battery_value);
	srcStruct.signal_power				= aep_htons(srcStruct.signal_power);
	srcStruct.snr						= aep_htons(srcStruct.snr);
	srcStruct.tx_power					= aep_htons(srcStruct.tx_power);
	srcStruct.magnetic_value				= aep_htoni(srcStruct.magnetic_value);
	
	index = resultStruct.str;
	
	memcpy(index, "02", 2);
	index += 1 * 2;
	
	tempLen = aep_htons(AEP_SERVICE_ID_HEARTBEAT);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)srcStruct.ICCID, 20);
	index += 20 * 2;
	
	HexToStr(index, (char *)&srcStruct.heartbeat_time, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.protect_time, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)srcStruct.saas_version, 6);
	index += 6 * 2;
	
	HexToStr(index, (char *)srcStruct.manufacturer_id, 18);
	index += 18 * 2;
	
	HexToStr(index, (char *)srcStruct.IMSI, 15);
	index += 15 * 2;
	
	HexToStr(index, (char *)srcStruct.IMEI, 15);
	index += 15 * 2;
	
	tempLen = aep_htons(srcStruct.nb_firmware.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.nb_firmware.str, srcStruct.nb_firmware.len);
	index += srcStruct.nb_firmware.len * 2;
	
	tempLen = aep_htons(srcStruct.nb_module.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.nb_module.str, srcStruct.nb_module.len);
	index += srcStruct.nb_module.len * 2;
	
	tempLen = aep_htons(srcStruct.mcu_firmware.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.mcu_firmware.str, srcStruct.mcu_firmware.len);
	index += srcStruct.mcu_firmware.len * 2;
	
	tempLen = aep_htons(srcStruct.terminal_type.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.terminal_type.str, srcStruct.terminal_type.len);
	index += srcStruct.terminal_type.len * 2;
	
	tempLen = aep_htons(srcStruct.manufacturer_name.len);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	HexToStr(index, (char *)srcStruct.manufacturer_name.str, srcStruct.manufacturer_name.len);
	index += srcStruct.manufacturer_name.len * 2;
	
	HexToStr(index, (char *)&srcStruct.rsrp, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.sinr, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.pci, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.ecl, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.cell_id, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.battery_voltage, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.battery_value, 4);
	index += 4 * 2;
	
	HexToStr(index, (char *)&srcStruct.signal_power, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.snr, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.tx_power, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.parking_state, 1);
	index += 1 * 2;
	
	HexToStr(index, (char *)&srcStruct.error_code, 1);
	index += 1 * 2;
	
	HexToStr(index, (char *)&srcStruct.parking_change, 1);
	index += 1 * 2;
	
	HexToStr(index, (char *)&srcStruct.magnetic_value, 4);
	index += 4 * 2;
	
	return resultStruct;
}

/**********************************************************************************************************
 @Function			AepString CTWing_ParkingChangeInfo_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepParkingChangeInfoData srcStruct)
 @Description			CTWing_ParkingChangeInfo_CodeDataReport		: 序列化ParkingChangeInfo
 @Input				pClient								: CTWing客户端实例
					srcStruct
 @Return				void
**********************************************************************************************************/
AepString CTWing_ParkingChangeInfo_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepParkingChangeInfoData srcStruct)
{
	char* index;
	AepString resultStruct;
	
	unsigned short tempLen;
	unsigned short payloadLen = 1;
	
	memset((void *)pClient->AepMallocProcessStack, 0x0, sizeof(pClient->AepMallocProcessStack));
	
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)pClient->AepMallocProcessStack;
	
	index = resultStruct.str;
	
	memcpy(index, "07", 2);
	index += 1 * 2;
	
	tempLen = aep_htons(AEP_SERVICE_ID_PARKINGCHANGE);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.parking_change, 1);
	index += 1 * 2;
	
	return resultStruct;
}

/**********************************************************************************************************
 @Function			AepString CTWing_ErrorCodeReport_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepErrorCodeReportData srcStruct)
 @Description			CTWing_ErrorCodeReport_CodeDataReport		: 序列化ErrorCodeReport
 @Input				pClient								: CTWing客户端实例
					srcStruct
 @Return				void
**********************************************************************************************************/
AepString CTWing_ErrorCodeReport_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepErrorCodeReportData srcStruct)
{
	char* index;
	AepString resultStruct;
	
	unsigned short tempLen;
	unsigned short payloadLen = 1;
	
	memset((void *)pClient->AepMallocProcessStack, 0x0, sizeof(pClient->AepMallocProcessStack));
	
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)pClient->AepMallocProcessStack;
	
	index = resultStruct.str;
	
	memcpy(index, "07", 2);
	index += 1 * 2;
	
	tempLen = aep_htons(AEP_SERVICE_ID_ERRORCODE);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.error_code, 1);
	index += 1 * 2;
	
	return resultStruct;
}

/**********************************************************************************************************
 @Function			AepString CTWing_LowVoltageAlarm_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepLowVoltageAlarmData srcStruct)
 @Description			CTWing_LowVoltageAlarm_CodeDataReport		: 序列化LowVoltageAlarm
 @Input				pClient								: CTWing客户端实例
					srcStruct
 @Return				void
**********************************************************************************************************/
AepString CTWing_LowVoltageAlarm_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepLowVoltageAlarmData srcStruct)
{
	char* index;
	AepString resultStruct;
	
	unsigned short tempLen;
	unsigned short payloadLen = 4;
	
	memset((void *)pClient->AepMallocProcessStack, 0x0, sizeof(pClient->AepMallocProcessStack));
	
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)pClient->AepMallocProcessStack;
	
	index = resultStruct.str;
	
	memcpy(index, "07", 2);
	index += 1 * 2;
	
	tempLen = aep_htons(AEP_SERVICE_ID_LOWVOLTAGE);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.battery_voltage, 4);
	index += 4 * 2;
	
	return resultStruct;
}

/**********************************************************************************************************
 @Function			AepString CTWing_MagneticDisturb_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepMagneticDisturbData srcStruct)
 @Description			CTWing_MagneticDisturb_CodeDataReport		: 序列化MagneticDisturb
 @Input				pClient								: CTWing客户端实例
					srcStruct
 @Return				void
**********************************************************************************************************/
AepString CTWing_MagneticDisturb_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepMagneticDisturbData srcStruct)
{
	char* index;
	AepString resultStruct;
	
	unsigned short tempLen;
	unsigned short payloadLen = 4;
	
	memset((void *)pClient->AepMallocProcessStack, 0x0, sizeof(pClient->AepMallocProcessStack));
	
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)pClient->AepMallocProcessStack;
	
	index = resultStruct.str;
	
	memcpy(index, "07", 2);
	index += 1 * 2;
	
	tempLen = aep_htons(AEP_SERVICE_ID_MAGNETICDISTURB);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;
	
	HexToStr(index, (char *)&srcStruct.magnetic_value, 4);
	index += 4 * 2;
	
	return resultStruct;
}
#endif

/**********************************************************************************************************
 @Function			AepString CTWing_CodeDataReportByIdToStr(CTWING_ClientsTypeDef* pClient, int serviceId, void * srcStruct)
 @Description			CTWing_CodeDataReportByIdToStr			: 序列化数据包
 @Input				pClient								: CTWing客户端实例
					serviceId
					srcStruct
 @Return				void
**********************************************************************************************************/
AepString CTWing_CodeDataReportByIdToStr(CTWING_ClientsTypeDef* pClient, int serviceId, void * srcStruct)
{
#if CTWING_AEPMODULE_TYPE == CTWING_AEPMODULE_MVB_VD33D_P2_1
	if (serviceId == AEP_SERVICE_ID_SPOTSTATUSDATA) {
		return CTWing_SpotStatusData_CodeDataReport(pClient, *(AepSpotStatusData*)srcStruct);
	}
	else if (serviceId == AEP_SERVICE_ID_WORKINFO) {
		return CTWing_WorkInfo_CodeDataReport(pClient, *(AepWorkInfo*)srcStruct);
	}
	else if (serviceId == AEP_SERVICE_ID_BASICINFO) {
		return CTWing_BasicInfo_CodeDataReport(pClient, *(AepBasicInfo*)srcStruct);
	}
	else if (serviceId == AEP_SERVICE_ID_DYNAMICINFO) {
		return CTWing_DynamicInfo_CodeDataReport(pClient, *(AepDynamicInfo*)srcStruct);
	}
	else {
		AepString result = {0};
		return result;
	}
#endif
	
#if CTWING_AEPMODULE_TYPE == CTWING_AEPMODULE_MVB_VD33D_P2_2
	if (serviceId == AEP_SERVICE_ID_DATAREPORT) {
		return CTWing_DataReport_CodeDataReport(pClient, *(AepDataReportData*)srcStruct);
	}
	else if (serviceId == AEP_SERVICE_ID_SIGNALREPORT) {
		return CTWing_SignalReport_CodeDataReport(pClient, *(AepSignalReportData*)srcStruct);
	}
	else if (serviceId == AEP_SERVICE_ID_HEARTBEAT) {
		return CTWing_HeartBeat_CodeDataReport(pClient, *(AepHeartBeatData*)srcStruct);
	}
	else if (serviceId == AEP_SERVICE_ID_PARKINGCHANGE) {
		return CTWing_ParkingChangeInfo_CodeDataReport(pClient, *(AepParkingChangeInfoData*)srcStruct);
	}
	else if (serviceId == AEP_SERVICE_ID_ERRORCODE) {
		return CTWing_ErrorCodeReport_CodeDataReport(pClient, *(AepErrorCodeReportData*)srcStruct);
	}
	else if (serviceId == AEP_SERVICE_ID_LOWVOLTAGE) {
		return CTWing_LowVoltageAlarm_CodeDataReport(pClient, *(AepLowVoltageAlarmData*)srcStruct);
	}
	else if (serviceId == AEP_SERVICE_ID_MAGNETICDISTURB) {
		return CTWing_MagneticDisturb_CodeDataReport(pClient, *(AepMagneticDisturbData*)srcStruct);
	}
	else {
		AepString result = {0};
		return result;
	}
#endif
	
#if CTWING_AEPMODULE_TYPE == CTWING_AEPMODULE_MVB_VD33D_P2_3
	if (serviceId == AEP_SERVICE_ID_DATAREPORT) {
		return CTWing_DataReport_CodeDataReport(pClient, *(AepDataReportData*)srcStruct);
	}
	else if (serviceId == AEP_SERVICE_ID_SIGNALREPORT) {
		return CTWing_SignalReport_CodeDataReport(pClient, *(AepSignalReportData*)srcStruct);
	}
	else if (serviceId == AEP_SERVICE_ID_EXTRAREPORT) {
		return CTWing_ExtraReport_CodeDataReport(pClient, *(AepExtraReportData*)srcStruct);
	}
	else if (serviceId == AEP_SERVICE_ID_HEARTBEAT) {
		return CTWing_HeartBeat_CodeDataReport(pClient, *(AepHeartBeatData*)srcStruct);
	}
	else if (serviceId == AEP_SERVICE_ID_PARKINGCHANGE) {
		return CTWing_ParkingChangeInfo_CodeDataReport(pClient, *(AepParkingChangeInfoData*)srcStruct);
	}
	else if (serviceId == AEP_SERVICE_ID_ERRORCODE) {
		return CTWing_ErrorCodeReport_CodeDataReport(pClient, *(AepErrorCodeReportData*)srcStruct);
	}
	else if (serviceId == AEP_SERVICE_ID_LOWVOLTAGE) {
		return CTWing_LowVoltageAlarm_CodeDataReport(pClient, *(AepLowVoltageAlarmData*)srcStruct);
	}
	else if (serviceId == AEP_SERVICE_ID_MAGNETICDISTURB) {
		return CTWing_MagneticDisturb_CodeDataReport(pClient, *(AepMagneticDisturbData*)srcStruct);
	}
	else {
		AepString result = {0};
		return result;
	}
#endif
}

/**********************************************************************************************************
 @Function			AepBytes CTWing_CodeDataReportByIdToBytes(CTWING_ClientsTypeDef* pClient, int serviceId, void * srcStruct)
 @Description			CTWing_CodeDataReportByIdToBytes			: 序列化数据包
 @Input				pClient								: CTWing客户端实例
					serviceId
					srcStruct
 @Return				void
**********************************************************************************************************/
AepBytes CTWing_CodeDataReportByIdToBytes(CTWING_ClientsTypeDef* pClient, int serviceId, void * srcStruct)
{
	AepString tempStr = CTWing_CodeDataReportByIdToStr(pClient, serviceId, srcStruct);
	AepBytes result = {0};
	result.len = tempStr.len / 2;
	if (result.len > 0) {
		memset((void *)pClient->DataProcessStack, 0x0, sizeof(pClient->DataProcessStack));
		result.str = (char *)pClient->DataProcessStack;
		StrToHex(result.str, tempStr.str, result.len);
	}
	
	return result;
}

/**********************************************************************************************************
 @Function			AepString CTWing_CodeDataReportByIdentifierToStr(CTWING_ClientsTypeDef* pClient, char* serviceIdentifier, void * srcStruct)
 @Description			CTWing_CodeDataReportByIdentifierToStr		: 序列化数据包
 @Input				pClient								: CTWing客户端实例
					serviceIdentifier
					srcStruct
 @Return				void
**********************************************************************************************************/
AepString CTWing_CodeDataReportByIdentifierToStr(CTWING_ClientsTypeDef* pClient, char* serviceIdentifier, void * srcStruct)
{
#if CTWING_AEPMODULE_TYPE == CTWING_AEPMODULE_MVB_VD33D_P2_1
	if (strcmp(serviceIdentifier, AEP_SERVICE_ENTIFIER_SPOTSTATUSDATA) == 0) {
		return CTWing_SpotStatusData_CodeDataReport(pClient, *(AepSpotStatusData*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, AEP_SERVICE_ENTIFIER_WORKINFO) == 0) {
		return CTWing_WorkInfo_CodeDataReport(pClient, *(AepWorkInfo*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, AEP_SERVICE_ENTIFIER_BASICINFO) == 0) {
		return CTWing_BasicInfo_CodeDataReport(pClient, *(AepBasicInfo*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, AEP_SERVICE_ENTIFIER_DYNAMICINFO) == 0) {
		return CTWing_DynamicInfo_CodeDataReport(pClient, *(AepDynamicInfo*)srcStruct);
	}
	else {
		AepString result = {0};
		return result;
	}
#endif
	
#if CTWING_AEPMODULE_TYPE == CTWING_AEPMODULE_MVB_VD33D_P2_2
	if (strcmp(serviceIdentifier, AEP_SERVICE_ENTIFIER_DATAREPORT) == 0) {
		return CTWing_DataReport_CodeDataReport(pClient, *(AepDataReportData*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, AEP_SERVICE_ENTIFIER_SIGNALREPORT) == 0) {
		return CTWing_SignalReport_CodeDataReport(pClient, *(AepSignalReportData*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, AEP_SERVICE_ENTIFIER_HEARTBEAT) == 0) {
		return CTWing_HeartBeat_CodeDataReport(pClient, *(AepHeartBeatData*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, AEP_SERVICE_ENTIFIER_PARKINGCHANGE) == 0) {
		return CTWing_ParkingChangeInfo_CodeDataReport(pClient, *(AepParkingChangeInfoData*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, AEP_SERVICE_ENTIFIER_ERRORCODE) == 0) {
		return CTWing_ErrorCodeReport_CodeDataReport(pClient, *(AepErrorCodeReportData*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, AEP_SERVICE_ENTIFIER_LOWVOLTAGE) == 0) {
		return CTWing_LowVoltageAlarm_CodeDataReport(pClient, *(AepLowVoltageAlarmData*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, AEP_SERVICE_ENTIFIER_MAGNETICDISTURB) == 0) {
		return CTWing_MagneticDisturb_CodeDataReport(pClient, *(AepMagneticDisturbData*)srcStruct);
	}
	else {
		AepString result = {0};
		return result;
	}
#endif
	
#if CTWING_AEPMODULE_TYPE == CTWING_AEPMODULE_MVB_VD33D_P2_3
	if (strcmp(serviceIdentifier, AEP_SERVICE_ENTIFIER_DATAREPORT) == 0) {
		return CTWing_DataReport_CodeDataReport(pClient, *(AepDataReportData*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, AEP_SERVICE_ENTIFIER_SIGNALREPORT) == 0) {
		return CTWing_SignalReport_CodeDataReport(pClient, *(AepSignalReportData*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, AEP_SERVICE_ENTIFIER_EXTRAREPORT) == 0) {
		return CTWing_ExtraReport_CodeDataReport(pClient, *(AepExtraReportData*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, AEP_SERVICE_ENTIFIER_HEARTBEAT) == 0) {
		return CTWing_HeartBeat_CodeDataReport(pClient, *(AepHeartBeatData*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, AEP_SERVICE_ENTIFIER_PARKINGCHANGE) == 0) {
		return CTWing_ParkingChangeInfo_CodeDataReport(pClient, *(AepParkingChangeInfoData*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, AEP_SERVICE_ENTIFIER_ERRORCODE) == 0) {
		return CTWing_ErrorCodeReport_CodeDataReport(pClient, *(AepErrorCodeReportData*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, AEP_SERVICE_ENTIFIER_LOWVOLTAGE) == 0) {
		return CTWing_LowVoltageAlarm_CodeDataReport(pClient, *(AepLowVoltageAlarmData*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, AEP_SERVICE_ENTIFIER_MAGNETICDISTURB) == 0) {
		return CTWing_MagneticDisturb_CodeDataReport(pClient, *(AepMagneticDisturbData*)srcStruct);
	}
	else {
		AepString result = {0};
		return result;
	}
#endif
}

/**********************************************************************************************************
 @Function			AepBytes CTWing_CodeDataReportByIdentifierToBytes(CTWING_ClientsTypeDef* pClient, char* serviceIdentifier, void * srcStruct)
 @Description			CTWing_CodeDataReportByIdentifierToBytes	: 序列化数据包
 @Input				pClient								: CTWing客户端实例
					serviceIdentifier
					srcStruct
 @Return				void
**********************************************************************************************************/
AepBytes CTWing_CodeDataReportByIdentifierToBytes(CTWING_ClientsTypeDef* pClient, char* serviceIdentifier, void * srcStruct)
{
	AepString tempStr = CTWing_CodeDataReportByIdentifierToStr(pClient, serviceIdentifier, srcStruct);
	AepBytes result = {0};
	result.len = tempStr.len / 2;
	if (result.len > 0) {
		memset((void *)pClient->DataProcessStack, 0x0, sizeof(pClient->DataProcessStack));
		result.str = (char *)pClient->DataProcessStack;
		StrToHex(result.str, tempStr.str, result.len);
	}
	
	return result;
}

/********************************************** END OF FLEE **********************************************/
