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
#define AEP_SERVICE_ID_DATAREPORT			1
#define AEP_SERVICE_ENTIFIER_DATAREPORT		"DataReport"

#define AEP_SERVICE_ID_SIGNALREPORT		2
#define AEP_SERVICE_ENTIFIER_SIGNALREPORT	"SignalReport"

#define AEP_SERVICE_ID_HEARTBEAT			3
#define AEP_SERVICE_ENTIFIER_HEARTBEAT		"HeartBeat"

#define AEP_SERVICE_ID_PARKINGCHANGE		1001
#define AEP_SERVICE_ENTIFIER_PARKINGCHANGE	"ParkingChange"

#define AEP_SERVICE_ID_ERRORCODE			1002
#define AEP_SERVICE_ENTIFIER_ERRORCODE		"ErrorCode"

#define AEP_SERVICE_ID_LOWVOLTAGE			1003
#define AEP_SERVICE_ENTIFIER_LOWVOLTAGE		"LowVoltage"

#define AEP_SERVICE_ID_MAGNETICDISTURB		1004
#define AEP_SERVICE_ENTIFIER_MAGNETICDISTURB	"MagneticDisturb"
#endif

#if CTWING_AEPMODULE_TYPE == CTWING_AEPMODULE_MVB_VD33D_P2_3
#define AEP_SERVICE_ID_DATAREPORT			1
#define AEP_SERVICE_ENTIFIER_DATAREPORT		"DataReport"

#define AEP_SERVICE_ID_SIGNALREPORT		2
#define AEP_SERVICE_ENTIFIER_SIGNALREPORT	"SignalReport"

#define AEP_SERVICE_ID_EXTRAREPORT			101
#define AEP_SERVICE_ENTIFIER_EXTRAREPORT	"ExtraReport"

#define AEP_SERVICE_ID_HEARTBEAT			102
#define AEP_SERVICE_ENTIFIER_HEARTBEAT		"HeartBeat"

#define AEP_SERVICE_ID_PARKINGCHANGE		1001
#define AEP_SERVICE_ENTIFIER_PARKINGCHANGE	"ParkingChange"

#define AEP_SERVICE_ID_ERRORCODE			1002
#define AEP_SERVICE_ENTIFIER_ERRORCODE		"ErrorCode"

#define AEP_SERVICE_ID_LOWVOLTAGE			1003
#define AEP_SERVICE_ENTIFIER_LOWVOLTAGE		"LowVoltage"

#define AEP_SERVICE_ID_MAGNETICDISTURB		1004
#define AEP_SERVICE_ENTIFIER_MAGNETICDISTURB	"MagneticDisturb"
#endif

#define AEP_MODULE_SAAS_VERSION			"JS_1.0"
#define AEP_MODULE_MANUFACTURER_ID			"91330104563048753R"
#define AEP_MODULE_NB_ICCID				TCFG_Utility_Get_Nbiot_Iccid_String()
#define AEP_MODULE_NB_IMSI				TCFG_Utility_Get_Nbiot_Imsi_String()
#define AEP_MODULE_NB_IMEI				TCFG_Utility_Get_Nbiot_Imei_String()
#define AEP_MODULE_NB_FIRMWARE			TCFG_Utility_Get_Nbiot_ModelVersion()
#define AEP_MODULE_NB_MODULE				TCFG_Utility_Get_Nbiot_Manufacturermode()
#define AEP_MODULE_MCU_FIRMWARE			TCFG_Utility_Get_Softwear_Version_String()
#define AEP_MODULE_TERMINAL_TYPE			TCFG_Utility_Get_Hardwear_Version_String()
#define AEP_MODULE_MANUFACTURER_NAME		TCFG_EEPROM_Get_Vender_String()

#define AEP_MODULE_NB_RSRP				TCFG_Utility_Get_Nbiot_CellRsrp()
#define AEP_MODULE_NB_SINR				TCFG_Utility_Get_Nbiot_CellSnr()
#define AEP_MODULE_NB_PCI				TCFG_Utility_Get_Nbiot_RadioPCI()
#define AEP_MODULE_NB_ECL				TCFG_Utility_Get_Nbiot_RadioECL()
#define AEP_MODULE_NB_CELLID				TCFG_Utility_Get_Nbiot_RadioCellID()

#define AEP_MODULE_NB_SIGNALPOWER			TCFG_Utility_Get_Nbiot_RadioSignalpower()
#define AEP_MODULE_NB_SNR				TCFG_Utility_Get_Nbiot_RadioSNR()
#define AEP_MODULE_NB_TXPOWER				TCFG_Utility_Get_Nbiot_RadioTXpower()

typedef struct AepStrStruct
{
	unsigned short		len;
	char*			str;
} AepString;

typedef AepString AepBytes;

#if CTWING_AEPMODULE_TYPE == CTWING_AEPMODULE_MVB_VD33D_P2_1
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
#endif

#if CTWING_AEPMODULE_TYPE == CTWING_AEPMODULE_MVB_VD33D_P2_2
typedef struct AepDataReportStruct
{
	float			battery_voltage;
	int				battery_value;
	long long			ptime;
	char				parking_state;
	char				error_code;
	char				parking_change;
	int				magnetic_value;
} AepDataReportData;

extern AepDataReportData			AepDataReportSrcdata;

AepString CTWing_DataReport_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepDataReportData srcStruct);

typedef struct AepSignalReportStruct
{
	int				rsrp;
	int				sinr;
	int				pci;
	int				ecl;
	int				cell_id;
} AepSignalReportData;

extern AepSignalReportData		AepSignalReportSrcdata;

AepString CTWing_SignalReport_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepSignalReportData srcStruct);

typedef struct AepHeartBeatStruct
{
	char				ICCID[20];
	float			heartbeat_time;
	float			protect_time;
	char				saas_version[6];
	char				manufacturer_id[18];
	char				IMSI[15];
	char				IMEI[15];
	AepString			nb_firmware;
	AepString			nb_module;
	AepString			mcu_firmware;
	AepString			terminal_type;
	AepString			manufacturer_name;
	int				rsrp;
	int				sinr;
	int				pci;
	int				ecl;
	int				cell_id;
	float			battery_voltage;
	int				battery_value;
	short			signal_power;
	short			snr;
	short			tx_power;
	char				parking_state;
	char				error_code;
	char				parking_change;
	int				magnetic_value;
} AepHeartBeatData;

extern AepHeartBeatData			AepHeartBeatSrcdata;

AepString CTWing_HeartBeat_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepHeartBeatData srcStruct);

typedef struct AepParkingChangeInfoStruct
{
	char				parking_change;
} AepParkingChangeInfoData;

extern AepParkingChangeInfoData	AepParkingChangeInfoSrcdata;

AepString CTWing_ParkingChangeInfo_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepParkingChangeInfoData srcStruct);

typedef struct AepErrorCodeReportStruct
{
	char				error_code;
} AepErrorCodeReportData;

extern AepErrorCodeReportData		AepErrorCodeReportSrcdata;

AepString CTWing_ErrorCodeReport_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepErrorCodeReportData srcStruct);

typedef struct AepLowVoltageAlarmStruct
{
	float			battery_voltage;
} AepLowVoltageAlarmData;

extern AepLowVoltageAlarmData		AepLowVoltageAlarmSrcdata;

AepString CTWing_LowVoltageAlarm_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepLowVoltageAlarmData srcStruct);

typedef struct AepMagneticDisturbStruct
{
	int				magnetic_value;
} AepMagneticDisturbData;

extern AepMagneticDisturbData		AepMagneticDisturbSrcdata;

AepString CTWing_MagneticDisturb_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepMagneticDisturbData srcStruct);
#endif

#if CTWING_AEPMODULE_TYPE == CTWING_AEPMODULE_MVB_VD33D_P2_3
typedef struct AepDataReportStruct
{
	char				parking_state;
} AepDataReportData;

extern AepDataReportData			AepDataReportSrcdata;

AepString CTWing_DataReport_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepDataReportData srcStruct);

typedef struct AepSignalReportStruct
{
	int				rsrp;
	int				sinr;
	int				pci;
	int				ecl;
	int				cell_id;
} AepSignalReportData;

extern AepSignalReportData		AepSignalReportSrcdata;

AepString CTWing_SignalReport_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepSignalReportData srcStruct);

typedef struct AepExtraReportStruct
{
	float			battery_voltage;
	int				battery_value;
	long long			ptime;
	char				parking_state;
	char				error_code;
	char				parking_change;
	int				magnetic_value;
} AepExtraReportData;

extern AepExtraReportData		AepExtraReportSrcdata;

AepString CTWing_ExtraReport_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepExtraReportData srcStruct);

typedef struct AepHeartBeatStruct
{
	char				ICCID[20];
	float			heartbeat_time;
	float			protect_time;
	char				saas_version[6];
	char				manufacturer_id[18];
	char				IMSI[15];
	char				IMEI[15];
	AepString			nb_firmware;
	AepString			nb_module;
	AepString			mcu_firmware;
	AepString			terminal_type;
	AepString			manufacturer_name;
	int				rsrp;
	int				sinr;
	int				pci;
	int				ecl;
	int				cell_id;
	float			battery_voltage;
	int				battery_value;
	short			signal_power;
	short			snr;
	short			tx_power;
	char				parking_state;
	char				error_code;
	char				parking_change;
	int				magnetic_value;
} AepHeartBeatData;

extern AepHeartBeatData			AepHeartBeatSrcdata;

AepString CTWing_HeartBeat_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepHeartBeatData srcStruct);

typedef struct AepParkingChangeInfoStruct
{
	char				parking_change;
} AepParkingChangeInfoData;

extern AepParkingChangeInfoData	AepParkingChangeInfoSrcdata;

AepString CTWing_ParkingChangeInfo_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepParkingChangeInfoData srcStruct);

typedef struct AepErrorCodeReportStruct
{
	char				error_code;
} AepErrorCodeReportData;

extern AepErrorCodeReportData		AepErrorCodeReportSrcdata;

AepString CTWing_ErrorCodeReport_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepErrorCodeReportData srcStruct);

typedef struct AepLowVoltageAlarmStruct
{
	float			battery_voltage;
} AepLowVoltageAlarmData;

extern AepLowVoltageAlarmData		AepLowVoltageAlarmSrcdata;

AepString CTWing_LowVoltageAlarm_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepLowVoltageAlarmData srcStruct);

typedef struct AepMagneticDisturbStruct
{
	int				magnetic_value;
} AepMagneticDisturbData;

extern AepMagneticDisturbData		AepMagneticDisturbSrcdata;

AepString CTWing_MagneticDisturb_CodeDataReport(CTWING_ClientsTypeDef* pClient, AepMagneticDisturbData srcStruct);
#endif

AepString CTWing_CodeDataReportByIdToStr(CTWING_ClientsTypeDef* pClient, int serviceId, void * srcStruct);

AepBytes CTWing_CodeDataReportByIdToBytes(CTWING_ClientsTypeDef* pClient, int serviceId, void * srcStruct);

AepString CTWing_CodeDataReportByIdentifierToStr(CTWING_ClientsTypeDef* pClient, char* serviceIdentifier, void * srcStruct);

AepBytes CTWing_CodeDataReportByIdentifierToBytes(CTWING_ClientsTypeDef* pClient, char* serviceIdentifier, void * srcStruct);

#if CTWING_AEPMODULE_TYPE == CTWING_AEPMODULE_MVB_VD33D_P2_1
void CTWing_Message_Operate_Creat_Work_Info(CTWING_ClientsTypeDef* pClient, AepWorkInfo * srcStruct);
void CTWing_Message_Operate_Creat_Basic_Info(CTWING_ClientsTypeDef* pClient, AepBasicInfo * srcStruct);
void CTWing_Message_Operate_Creat_Dynamic_Info(CTWING_ClientsTypeDef* pClient, AepDynamicInfo * srcStruct);
#endif

#if CTWING_AEPMODULE_TYPE == CTWING_AEPMODULE_MVB_VD33D_P2_2
void CTWing_Message_Operate_Creat_Data_Report(CTWING_ClientsTypeDef* pClient, AepDataReportData * srcStruct);
void CTWing_Message_Operate_Creat_Signal_Report(CTWING_ClientsTypeDef* pClient, AepSignalReportData * srcStruct);
void CTWing_Message_Operate_Creat_Heart_Beat(CTWING_ClientsTypeDef* pClient, AepHeartBeatData * srcStruct);
#endif

#if CTWING_AEPMODULE_TYPE == CTWING_AEPMODULE_MVB_VD33D_P2_3
void CTWing_Message_Operate_Creat_Extra_Report(CTWING_ClientsTypeDef* pClient, AepExtraReportData * srcStruct);
void CTWing_Message_Operate_Creat_Heart_Beat(CTWING_ClientsTypeDef* pClient, AepHeartBeatData * srcStruct);
#endif

#endif /* __CTWING_AEPCODES_H */
