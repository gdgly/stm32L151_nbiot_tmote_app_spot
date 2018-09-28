#ifndef __MQTT_PCP_FUNC_H
#define   __MQTT_PCP_FUNC_H

#include "sys.h"
#include "mqttpcpconfig.h"

/* Ack Query Device Version */
typedef __packed struct
{
	unsigned char						ResultCode;										//结果码
	unsigned char						DeviceSoftVersion[16];								//当前版本号
}MqttSNPCP_AckQueryDeviceVersionTypeDef;

/* Rck New Version Notice */
typedef __packed struct
{
	unsigned char						PlatformSoftVersion[16];								//目的版本号
	unsigned short						UpgradePackSliceSize;								//升级包分片大小
	unsigned short						UpgradePackSliceNum;								//升级包分片总数
	unsigned short						UpgradePackCheckCode;								//升级包校验码
}MqttSNPCP_RckNewVersionNoticeTypeDef;

/* Request Upgrade Package */
typedef __packed struct
{
	unsigned char						PlatformSoftVersion[16];								//目的版本号
	unsigned short						UpgradePackSliceIndex;								//升级包分片序号
}MqttSNPCP_RequestUpgradePackageTypeDef;

/* Ack Request Upgrade Package */
typedef __packed struct
{
	unsigned char						ResultCode;										//结果码
	unsigned short						UpgradePackSliceIndex;								//升级包分片序号
	unsigned char						pUpgradeData[1];									//分片数据
}MqttSNPCP_AckRequestUpgradePackageTypeDef;

/* Report Upgrades */
typedef __packed struct
{
	unsigned char						ResultCode;										//结果码
	unsigned char						DeviceSoftVersion[16];								//当前版本号
}MqttSNPCP_ReportUpgradesTypeDef;

MqttSNPCP_StatusTypeDef MqttPCP_Func_FrameCheck(unsigned char* DataFrame, short DataLength);			//PCP数据帧检查

MqttSNPCP_StatusTypeDef MqttPCP_Func_SelectMessageExecuteCmd(MqttSNPCP_ClientsTypeDef* pClient);		//PCP判断不同消息码处理不同命令
MqttSNPCP_StatusTypeDef MqttPCP_Func_AckQueryDeviceVersion(MqttSNPCP_ClientsTypeDef* pClient);			//PCP查询设备版本
MqttSNPCP_StatusTypeDef MqttPCP_Func_AckNewVersionNotice(MqttSNPCP_ClientsTypeDef* pClient);			//PCP新版本通知
MqttSNPCP_StatusTypeDef MqttPCP_Func_AckRequestUpgradePackage(MqttSNPCP_ClientsTypeDef* pClient);		//PCP请求升级包应答
MqttSNPCP_StatusTypeDef MqttPCP_Func_AckReportDownloadStatus(MqttSNPCP_ClientsTypeDef* pClient);		//PCP上报升级包下载状态应答
MqttSNPCP_StatusTypeDef MqttPCP_Func_AckPerformUpgrade(MqttSNPCP_ClientsTypeDef* pClient);			//PCP执行升级
MqttSNPCP_StatusTypeDef MqttPCP_Func_AckReportUpgrades(MqttSNPCP_ClientsTypeDef* pClient);			//PCP上报升级结果应答

MqttSNPCP_StatusTypeDef MqttPCP_Func_SelectUpgradeStatusExecuteCmd(MqttSNPCP_ClientsTypeDef* pClient);	//PCP判断不同主动上传数据码处理不同命令
MqttSNPCP_StatusTypeDef MqttPCP_Func_QueryDeviceVersion(MqttSNPCP_ClientsTypeDef* pClient);			//PCP查询设备版本应答包
MqttSNPCP_StatusTypeDef MqttPCP_Func_RequestUpgradePackage(MqttSNPCP_ClientsTypeDef* pClient);			//PCP请求升级包
MqttSNPCP_StatusTypeDef MqttPCP_Func_ReportDownloadStatus(MqttSNPCP_ClientsTypeDef* pClient);			//PCP上报升级包下载状态
MqttSNPCP_StatusTypeDef MqttPCP_Func_ReportUpgrades(MqttSNPCP_ClientsTypeDef* pClient);				//PCP上报升级结果

#endif /* __MQTT_PCP_FUNC_H */
