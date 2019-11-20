#ifndef __CTWING_AEPCODES_H
#define   __CTWING_AEPCODES_H

#include "sys.h"
#include "ctwingconfig.h"

#define AEP_BIG_ENDIAN					'b'
#define AEP_LIT_ENDIAN					'l'

static union { char c[4]; unsigned long mylong; } endian_test = {{ 'l', '?', '?', 'b' } };

#define AEP_ENDIANNESS					((char)endian_test.mylong)

#if CTWING_AEPMODULE_TYPE == CTWING_AEPMODULE_MVB_VD33D_P2_1
#define AEP_SERVICE_ID_SPOTSTATUSDATA		2
#define AEP_SERVICE_ENTIFIER_SPOTSTATUSDATA	"SpotStatusData"

#define AEP_SERVICE_ID_WORKINFO			3
#define AEP_SERVICE_ENTIFIER_WORKINFO		"WorkInfo"

#define AEP_SERVICE_ID_BASICINFO			4
#define AEP_SERVICE_ENTIFIER_BASICINFO		"BasicInfo"

#define AEP_SERVICE_ID_DYNAMICINFO			5
#define AEP_SERVICE_ENTIFIER_DYNAMICINFO	"DynamicInfo"
#endif
#if CTWING_AEPMODULE_TYPE == CTWING_AEPMODULE_MVB_VD33D_P2_2
#define AEP_SERVICE_ID_DATA_REPORT			1
#define AEP_SERVICE_ENTIFIER_DATA_REPORT	"data_report"

#define AEP_SERVICE_ID_SIGNAL_REPORT		2
#define AEP_SERVICE_ENTIFIER_SIGNAL_REPORT	"signal_report"

#define AEP_SERVICE_ID_SPOTSTATUSDATA		4
#define AEP_SERVICE_ENTIFIER_SPOTSTATUSDATA	"SpotStatusData"

#define AEP_SERVICE_ID_WORKINFO			5
#define AEP_SERVICE_ENTIFIER_WORKINFO		"WorkInfo"

#define AEP_SERVICE_ID_BASICINFO			6
#define AEP_SERVICE_ENTIFIER_BASICINFO		"BasicInfo"

#define AEP_SERVICE_ID_DYNAMICINFO			7
#define AEP_SERVICE_ENTIFIER_DYNAMICINFO	"DynamicInfo"
#endif

typedef struct AepStrStruct
{
	unsigned short		len;
	char*			str;
} AepString;

typedef AepString AepBytes;

typedef struct AepSpotStatusDataStringStruct
{
	char				Radarval[64];
	char				Radarback[64];
} AepSpotStatusDataString;

extern AepSpotStatusDataString	AepSpotStatusString;

typedef struct AepSpotStatusDataStruct
{
	char				SN[8];
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

extern AepSpotStatusData			AepSpotStatusSrcdata;

AepString CTWing_SpotStatusData_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepSpotStatusData srcStruct);

typedef struct AepWorkInfoDataStringStruct
{
	char				Cmdcnt[12];
	char				Nbruntime[22];
	char				Coef[18];
} AepWorkInfoDataString;

extern AepWorkInfoDataString		AepWorkInfoString;

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

extern AepWorkInfo				AepWorkInfoSrcdata;

AepString CTWing_WorkInfo_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepWorkInfo srcStruct);

typedef struct AepBasicInfoDataStringStruct
{
	char				ModelType[8];
	char				Soft[12];
	char				Boot[18];
} AepBasicInfoDataString;

extern AepBasicInfoDataString		AepBasicInfoString;

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

extern AepBasicInfo				AepBasicInfoSrcdata;

AepString CTWing_BasicInfo_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepBasicInfo srcStruct);

typedef struct AepDynamicInfoDataStringStruct
{
	char				RAlib[14];
	char				Nbrun[32];
	char				Gain[14];
	char				x[37];
	char				y[37];
	char				z[37];
} AepDynamicInfoDataString;

extern AepDynamicInfoDataString	AepDynamicInfoString;

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

extern AepDynamicInfo			AepDynamicInfoSrcdata;

AepString CTWing_DynamicInfo_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepDynamicInfo srcStruct);

#if CTWING_AEPMODULE_TYPE == CTWING_AEPMODULE_MVB_VD33D_P2_2
typedef struct AepDataReportStruct
{
	char				parking_state;
} AepDataReportData;

extern AepDataReportData			AepDataReportSrcdata;

AepString CTWing_DataReportData_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepDataReportData srcStruct);

typedef struct AepSignalReportStruct
{
	int				rsrp;
	int				sinr;
	int				pci;
	int				ecl;
	int				cell_id;
} AepSignalReportData;

extern AepSignalReportData		AepSignalReportSrcdata;

AepString CTWing_SignalReportData_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepSignalReportData srcStruct);
#endif

AepString CTWing_CodeDataReportByIdToStr(CTWING_ClientsTypeDef* pClient, int serviceId, void * srcStruct);

AepBytes CTWing_CodeDataReportByIdToBytes(CTWING_ClientsTypeDef* pClient, int serviceId, void * srcStruct);

AepString CTWing_CodeDataReportByIdentifierToStr(CTWING_ClientsTypeDef* pClient, char* serviceIdentifier, void * srcStruct);

AepBytes CTWing_CodeDataReportByIdentifierToBytes(CTWING_ClientsTypeDef* pClient, char* serviceIdentifier, void * srcStruct);

void CTWing_Message_Operate_Creat_Work_Info(CTWING_ClientsTypeDef* pClient, AepWorkInfo * srcStruct);
void CTWing_Message_Operate_Creat_Basic_Info(CTWING_ClientsTypeDef* pClient, AepBasicInfo * srcStruct);
void CTWing_Message_Operate_Creat_Dynamic_Info(CTWING_ClientsTypeDef* pClient, AepDynamicInfo * srcStruct);

#if CTWING_AEPMODULE_TYPE == CTWING_AEPMODULE_MVB_VD33D_P2_2
void CTWing_Message_Operate_Creat_Signal_Report(CTWING_ClientsTypeDef* pClient, AepSignalReportData * srcStruct);
#endif

#endif /* __CTWING_AEPCODES_H */
