#ifndef __CTWING_AEPCODES_H
#define   __CTWING_AEPCODES_H

#include "sys.h"
#include "ctwingconfig.h"

#define AEP_BIG_ENDIAN					'b'
#define AEP_LIT_ENDIAN					'l'

static union { char c[4]; unsigned long mylong; } endian_test = {{ 'l', '?', '?', 'b' } };

#define AEP_ENDIANNESS					((char)endian_test.mylong)

#define AEP_SERVICE_ID_SPOTSTATUSDATA		2
#define AEP_SERVICE_ENTIFIER_SPOTSTATUSDATA	"SpotStatusData"

#define AEP_SERVICE_ID_WORKINFO			3
#define AEP_SERVICE_ENTIFIER_WORKINFO		"WorkInfo"

#define AEP_SERVICE_ID_BASICINFO			4
#define AEP_SERVICE_ENTIFIER_BASICINFO		"BasicInfo"

#define AEP_SERVICE_ID_DYNAMICINFO			5
#define AEP_SERVICE_ENTIFIER_DYNAMICINFO	"DynamicInfo"

typedef struct AepStrStruct
{
	unsigned short		len;
	char*			str;
} AepString;

typedef AepString AepBytes;






















typedef struct AepSpotStatusDataStruct
{
	unsigned int		DateTime;
	unsigned short		SpotCount;
	unsigned char		SpotStatus;
	short			MagneticX;
	short			MagneticY;
	short			MagneticZ;
	unsigned short		MagneticDiff;
	unsigned char		RadarDistance;
	unsigned char		RadarStrength;
	unsigned char		RadarCoverCount;
	unsigned char		RadarDiff;
	unsigned char		NBRssi;
	char				NBSnr;
	char				MCUTemp;
	char				QMCTemp;
	short			MagneticBackX;
	short			MagneticBackY;
	short			MagneticBackZ;
	short			Debugval;
	AepString			Radarval;
	AepString			Radarback;
} AepSpotStatusData;

AepString CTWing_SpotStatusData_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepSpotStatusData srcStruct);

typedef struct AepWorkInfoStruct
{
	char				SN[8];
	unsigned char		Sense;
	AepString			WorkMode;
	unsigned char		RfChannel;
	unsigned short		Range;
	unsigned int		Earfcn;
	unsigned int		Tac;
	unsigned int		Ci;
	AepString			Cmdcnt;
	AepString			Nbruntime;
	AepString			APN;
	AepString			Coef;
	unsigned char		Rollinit;
	unsigned char		RadioRv;
} AepWorkInfo;

AepString CTWing_WorkInfo_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepWorkInfo srcStruct);

typedef struct AepBasicInfoStruct
{
	char				SN[8];
	AepString			ModelType;
	AepString			Vender;
	AepString			Hard;
	AepString			Soft;
	AepString			Sim;
	AepString			Imei;
	AepString			Imsi;
	AepString			Nbvender;
	AepString			Nbmode;
	AepString			Boot;
	AepString			Ver;
	unsigned char		Rmold;
	unsigned char		Rvcc;
} AepBasicInfo;

AepString CTWing_BasicInfo_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepBasicInfo srcStruct);

typedef struct AepDynamicInfoStruct
{
	char				SN[8];
	unsigned int		RT;
	unsigned short		Batt;
	AepString			RAlib;
	unsigned int		Rcnt;
	short			Temp;
	unsigned short		Qmcrbt;
	AepString			Nbrun;
	unsigned char		Indelay;
	AepString			Gain;
	unsigned char		Smode;
	char				Sinter;
	char				hpass;
	AepString			x;
	AepString			y;
	AepString			z;
} AepDynamicInfo;

AepString CTWing_DynamicInfo_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepDynamicInfo srcStruct);



































#endif /* __CTWING_AEPCODES_H */
