#ifndef __MQTT_PCP_UPGRADE_H
#define   __MQTT_PCP_UPGRADE_H

#include "sys.h"
#include "mqttpcpconfig.h"

typedef __packed struct
{
	u8			Status;
	u32			BaseAddr;
	u16			BlockNum;
	u16			BlockLen;
	u16			DataLen;
	u32			SoftVer;
	u32			CheckCode;
}MqttSNPCP_APPInfoTypeDef;

#define	MQTTSN_UPGRADE_BACKUP_APP_WRITE_DISABLE		0												//控制备份
#define	MQTTSN_UPGRADE_BACKUP_APP_WRITE_ENABLE		1												//强制备份
#define	MQTTSN_UPGRADE_BACKUP_APP_WRITE_TYPE		MQTTSN_UPGRADE_BACKUP_APP_WRITE_DISABLE					//备份区备份模式

#if 0
#define	MQTTSN_UPGRADE_LOW_LIMIT_RSSI				-100												//信号值限制下限
#define	MQTTSN_UPGRADE_LOW_LIMIT_SNR				-100												//信号质量限制下限
#else
#define	MQTTSN_UPGRADE_LOW_LIMIT_RSSI				TCFG_EEPROM_GetUpgradeLimitRssi()						//信号值限制下限
#define	MQTTSN_UPGRADE_LOW_LIMIT_SNR				TCFG_EEPROM_GetUpgradeLimitSnr()						//信号质量限制下限
#endif

#define	MQTTSN_UPGRADE_PACK_SLICE_BLOCK_SIZE		512												//升级包最大分片大小

MqttSNPCP_ResultCodeTypeDef MqttPCP_Upgrade_BackupCurrentAPP(MqttSNPCP_ClientsTypeDef* pClient);											//升级备份当前APP
MqttSNPCP_ResultCodeTypeDef MqttPCP_Upgrade_NewVersionNotice(MqttSNPCP_ClientsTypeDef* pClient);											//升级新版本通知
MqttSNPCP_ResultCodeTypeDef MqttPCP_Upgrade_DataDownload(MqttSNPCP_ClientsTypeDef* pClient, u16 SliceIndex, u8* UpgradeData, u16 UpgradeDataLength);	//升级包下载
MqttSNPCP_ResultCodeTypeDef MqttPCP_Upgrade_DataAssemble(MqttSNPCP_ClientsTypeDef* pClient);												//升级包组装校验
MqttSNPCP_ResultCodeTypeDef MqttPCP_Upgrade_AfterUpdata(MqttSNPCP_ClientsTypeDef* pClient);												//升级包开始升级

u8 MqttSNCalculateSumCheckCode(u32 regCode, uint8_t* buf, uint32_t length);								//加和校验
u8 MqttSNCalculateStringToHex(u8 HighChar, u8 LowChar);												//字符转换

#endif /* __MQTT_PCP_UPGRADE_H */
