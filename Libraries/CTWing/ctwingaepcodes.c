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
#include "delay.h"
#include "usart.h"
#include "string.h"
#include "stdlib.h"
#include "ctype.h"

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
	unsigned short payloadLen = 31 + (srcStruct.Radarval.len + 2) + (srcStruct.Radarback.len + 2);
	
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


























































/********************************************** END OF FLEE **********************************************/
