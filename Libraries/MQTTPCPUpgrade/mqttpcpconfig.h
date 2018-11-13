#ifndef __MQTT_PCP_CONFIG_H
#define   __MQTT_PCP_CONFIG_H

#include "sys.h"
#include "nbiotconfig.h"
#include "mqttsnconfig.h"
#include "mqttsnfunc.h"
#include "mqttsntransport.h"

#define MQTTSN_PCP_DEBUG_LOG_RF_PRINT														//定义开启RF输出DEBUG信息

#define MQTTSN_PCP_COMMAND_TIMEOUT_SEC			30
#define MQTTSN_PCP_COMMAND_FAILURE_CNT			3

/* MQTTSN_PCP 协议栈开辟缓存大小 */
#define MQTTSN_PCP_BUFFER_SIZE				512
#define MQTTSN_PCP_DATASTACK_SIZE				512

#define MQTTSN_PCP_START_X					0xFFFE
#define MQTTSN_PCP_PROTOCOL_TYPE				0x01

typedef struct MqttSNPCP_ParameterTypeDef		MqttSNPCP_ParameterTypeDef;
typedef struct MqttSNPCP_MqttNetTransportTypeDef	MqttSNPCP_MqttNetTransportTypeDef;
typedef struct MqttSNPCP_ClientsTypeDef			MqttSNPCP_ClientsTypeDef;

/* MQTTSN_PCP Status */
typedef enum
{
	MQTTSN_PCP_OK       					= 0x00,
	MQTTSN_PCP_ERROR    					= 0x01,
	MQTTSN_PCP_CMD_TIMEOUT					= 0x02,
	
	/* -General Errors- */
	MQTTSN_PCP_Operation_not_allowed			= 3,
	MQTTSN_PCP_Operation_not_supported			= 4,
	MQTTSN_PCP_Need_to_enter_PIN				= 5,
	MQTTSN_PCP_Memory_failure				= 23,
	MQTTSN_PCP_No_Network_Service				= 30,
	MQTTSN_PCP_Incorrect_parameters			= 50,
	MQTTSN_PCP_Command_implemented_but			= 51,
	MQTTSN_PCP_Command_aborted_by_user			= 52,
	MQTTSN_PCP_Uplink_Busy_Flow_Control		= 159,
	MQTTSN_PCP_ME_failure					= 300,
	MQTTSN_PCP_SMS_service_of_ME_reserved		= 301,
	MQTTSN_PCP_operation_not_allowed			= 302,
	MQTTSN_PCP_operation_not_supported			= 303,
	MQTTSN_PCP_invalid_PDU_mode_parameter		= 304,
	MQTTSN_PCP_invalid_text_mode_parameter		= 305,
	MQTTSN_PCP_SIM_not_inserted				= 310,
	MQTTSN_PCP_SIM_PIN_required				= 311,
	MQTTSN_PCP_PH_SIM_PIN_required			= 312,
	MQTTSN_PCP_SIM_failure					= 313,
	MQTTSN_PCP_SIM_busy						= 314,
	MQTTSN_PCP_SIM_wrong					= 315,
	MQTTSN_PCP_SIM_PUK_required				= 316,
	MQTTSN_PCP_SIM_PIN2_required				= 317,
	MQTTSN_PCP_SIM_PUK2_required				= 318,
	MQTTSN_PCP_memory_failure				= 320,
	MQTTSN_PCP_invalid_memory_index			= 321,
	MQTTSN_PCP_memory_full					= 322,
	MQTTSN_PCP_SMSC_address_unknown			= 330,
	MQTTSN_PCP_no_network_service				= 331,
	MQTTSN_PCP_network_timeout				= 332,
	MQTTSN_PCP_acknowledgement_expected		= 340,
	MQTTSN_PCP_unknown_error					= 500,
	
	/* -HISI specific error codes- */
	MQTTSN_PCP_Required_parameter_not_configured	= 512,
	MQTTSN_PCP_TUP_not_registered				= 513,
	MQTTSN_PCP_AT_Internal_Error				= 514,
	MQTTSN_PCP_CID_is_active					= 515,
	MQTTSN_PCP_Incorrect_State_for_Command		= 516,
	MQTTSN_PCP_Cid_is_invalid				= 517,
	MQTTSN_PCP_Deactive_last_active_cid		= 520,
	MQTTSN_PCP_Cid_is_not_defined				= 521,
	
	/* -PCP Private error codes- */
	MQTTSN_PCP_Frame_Format_Error				= 600,
	MQTTSN_PCP_Frame_CheckCode_Error			= 601,
	MQTTSN_PCP_Frame_None					= 602,
	MQTTSN_PCP_MessageType_Error				= 603,
	MQTTSN_PCP_UpgradePack_Error				= 604,
	MQTTSN_PCP_UpgradeDown_Error				= 605,
	MQTTSN_PCP_Upgrade_Error					= 606
}MqttSNPCP_StatusTypeDef;

/* MQTTSN_PCP Message Code */
typedef enum
{
	MQTTSN_PCP_QueryDeviceVersion				= 0x13,										//查询设备版本
	MQTTSN_PCP_NewVersionNotice				= 0x14,										//新版本通知
	MQTTSN_PCP_RequestUpgradePackage			= 0x15,										//请求升级包
	MQTTSN_PCP_ReportDownloadStatus			= 0x16,										//上报升级包下载状态
	MQTTSN_PCP_PerformUpgrade				= 0x17,										//执行升级
	MQTTSN_PCP_ReportUpgrades				= 0x18										//上报升级结果
}MqttSNPCP_MessageCodeTypeDef;

/* MQTTSN_PCP Result Code */
typedef enum
{
	MQTTSN_PCP_ExecuteSuccess				= 0x00,										//处理成功
	MQTTSN_PCP_EquipmentUse					= 0x01,										//设备使用中
	MQTTSN_PCP_SignalqualityDifference			= 0x02,										//信号质量差
	MQTTSN_PCP_LatestVersion					= 0x03,										//已经是最新版本
	MQTTSN_PCP_LackofElectricity				= 0x04,										//电量不足
	MQTTSN_PCP_LackofSpace					= 0x05,										//剩余空间不足
	MQTTSN_PCP_DownloadTimeout				= 0x06,										//下载超时
	MQTTSN_PCP_UpgradePacketCheckFail			= 0x07,										//升级包校验失败
	MQTTSN_PCP_UpgradePacketTypeNonsupport		= 0x08,										//升级包类型不支持
	MQTTSN_PCP_InsufficientMemory				= 0x09,										//内存不足
	MQTTSN_PCP_InstallationFailure			= 0x0A,										//安装升级包失败
	MQTTSN_PCP_InternalAnomaly				= 0x7F,										//内部异常
	MQTTSN_PCP_UpgradeTaskFail				= 0x80,										//升级任务不存在
	MQTTSN_PCP_SliceNotFound					= 0x81										//指定分片不存在
}MqttSNPCP_ResultCodeTypeDef;

/* MQTTSN_PCP Dictate Event */
typedef enum
{
	MQTTSN_PCP_EVENT_STOP					= 0x00,										//Stop
	MQTTSN_PCP_EVENT_INITIALIZED				= 0x01,										//Initialized
	MQTTSN_PCP_EVENT_READY					= 0x02,										//Ready
	MQTTSN_PCP_EVENT_FRAME_RECV				= 0x03,										//RECV
	MQTTSN_PCP_EVENT_FRAME_SEND				= 0x04,										//SEND
	MQTTSN_PCP_EVENT_EXECUTE					= 0x05,										//Execute
	MQTTSN_PCP_EVENT_ACTIVEUPLOAD				= 0x06										//ActiveUpload
}MqttSNPCP_DictateEventTypeDef;

/* MQTTSN_PCP Upgrade Status */
typedef enum
{
	MQTTSN_PCP_UPGRADE_STANDBY				= 0x00,										//升级准备
	MQTTSN_PCP_UPGRADE_QUERYVERSION			= 0x01,										//升级查询设备版本
	MQTTSN_PCP_UPGRADE_DOWNLOAD				= 0x02,										//升级下载数据包
	MQTTSN_PCP_UPGRADE_ASSEMBLE				= 0x03,										//升级组装升级包
	MQTTSN_PCP_UPGRADE_INSTALL				= 0x04,										//升级版本
	MQTTSN_PCP_UPGRADE_FAILED				= 0x05										//升级错误
}MqttSNPCP_UpgradeStatusTypeDef;

/* MQTTSN_PCP Message Data Structure */
typedef __packed struct
{
	unsigned short							StartX;										//起始标识	0xFFFE
	unsigned char							ProtocolType;									//版本号		0x01
	unsigned char							MessageType;									//消息码		0xXX
	unsigned short							CRCCheckCode;									//检验码		CRC16
	unsigned short							PacketDataLength;								//数据区长度
	unsigned char							pPacketData[1];								//数据区
}MqttSNPCP_MessageDataTypeDef;

/* MQTTSN_PCP Parameter */
struct MqttSNPCP_ParameterTypeDef
{
	unsigned char							ProtocolType;									//版本号
	unsigned char							MessageType;									//消息码
	unsigned char							PlatformSoftVersion[16];							//目的版本号
	unsigned short							UpgradePackSliceSize;							//升级包分片大小
	unsigned short							UpgradePackSliceNum;							//升级包分片总数
	unsigned short							UpgradePackCheckCode;							//升级包校验码
	unsigned short							UpgradePackSliceIndex;							//升级包分片序号
	unsigned short							UpgradeStartTimes;								//升级任务启动次数
};

/* MQTTSN_PCP MqttSN Transport */
struct MqttSNPCP_MqttNetTransportTypeDef
{
	MQTTSN_ClientsTypeDef*					MqttSNStack;
	MqttSNPCP_StatusTypeDef					(*Write)(MqttSNPCP_MqttNetTransportTypeDef*, const char*, u16);
	MqttSNPCP_StatusTypeDef					(*Read)(MqttSNPCP_MqttNetTransportTypeDef*, char*, u16*);
};

/* MQTTSN_PCP Clients */
struct MqttSNPCP_ClientsTypeDef
{
	unsigned char*							Sendbuf;
	unsigned char*							Recvbuf;
	size_t								Sendbuf_size;
	size_t								Recvbuf_size;
	short								Sendlen;
	short								Recvlen;
	unsigned char*							DataProcessStack;
	size_t								DataProcessStack_size;
	unsigned short							Command_Timeout_Sec;
	unsigned short							Command_Failure_Cnt;
	
	struct MqttSNPCPDictateRuningCtlTypeDef
	{
		bool								dictateEnable;
		unsigned int						dictateTimeoutSec;
		unsigned char						dictateInitializedFailureCnt;
		unsigned char						dictateReadyFailureCnt;
		unsigned char						dictateRecvFailureCnt;
		unsigned char						dictateSendFailureCnt;
		unsigned char						dictateExecuteFailureCnt;
		unsigned char						dictateActiveUploadFailureCnt;
		unsigned char						dictateUpgradeQueryVersionCnt;
		unsigned char						dictateUpgradeDownloadCnt;
		unsigned char						dictateUpgradeAssembleCnt;
		unsigned char						dictateUpgradeInstallCnt;
		Stm32_CalculagraphTypeDef			dictateRunTime;
		MqttSNPCP_DictateEventTypeDef			dictateEvent;
	}DictateRunCtl;
	
	struct MqttSNPCPUpgradeExecutionTypeDef
	{
		MqttSNPCP_UpgradeStatusTypeDef		upgradeStatus;
		unsigned char						DeviceSoftVersion[16];
		unsigned char						PlatformSoftVersion[16];
		unsigned short						PackSliceIndex;
		unsigned short						PackSliceSize;
		unsigned short						PackLastSliceSize;
		unsigned short						PackSliceNum;
		unsigned short						PackCheckCode;
	}UpgradeExecution;
	
	MqttSNPCP_ParameterTypeDef				Parameter;
	MqttSNPCP_MqttNetTransportTypeDef*			MqttSNStack;
	NET_NBIOT_ClientsTypeDef*				NetNbiotStack;
};

/* Application Programming Interface */
void MqttPCP_Client_Init(MqttSNPCP_ClientsTypeDef* pClient, MqttSNPCP_MqttNetTransportTypeDef* NetSock, NET_NBIOT_ClientsTypeDef* NetNbiotStack);			//PCP客户端初始化
MqttSNPCP_ResultCodeTypeDef MqttPCP_UpgradeDataNewVersionNotice_Callback(MqttSNPCP_ClientsTypeDef* pClient);											//PCP新版本通知处理回调
MqttSNPCP_ResultCodeTypeDef MqttPCP_UpgradeDataDownload_Callback(MqttSNPCP_ClientsTypeDef* pClient, u16 SliceIndex, u8* UpgradeData, u16 UpgradeDataLength);	//PCP升级包下载回调
MqttSNPCP_ResultCodeTypeDef MqttPCP_UpgradeDataAssemble_Callback(MqttSNPCP_ClientsTypeDef* pClient);												//PCP升级包组装回调
MqttSNPCP_ResultCodeTypeDef MqttPCP_UpgradeDataReportUpgrades_Callback(MqttSNPCP_ClientsTypeDef* pClient);											//PCP上报升级结果回调

#endif /* __MQTT_PCP_CONFIG_H */
