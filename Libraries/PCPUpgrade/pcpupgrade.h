#ifndef __PCP_UPGRADE_H
#define   __PCP_UPGRADE_H

#include "sys.h"
#include "pcpconfig.h"

#define	UPGRADE_BACKUP_APP_WRITE_DISABLE	0												//控制备份
#define	UPGRADE_BACKUP_APP_WRITE_ENABLE	1												//强制备份
#define	UPGRADE_BACKUP_APP_WRITE_TYPE		UPGRADE_BACKUP_APP_WRITE_DISABLE						//备份区备份模式

PCP_ResultCodeTypeDef PCP_Upgrade_BackupCurrentAPP(PCP_ClientsTypeDef* pClient);											//升级备份当前APP
PCP_ResultCodeTypeDef PCP_Upgrade_NewVersionNotice(PCP_ClientsTypeDef* pClient);											//升级新版本通知
PCP_ResultCodeTypeDef PCP_Upgrade_DataDownload(PCP_ClientsTypeDef* pClient, u16 SliceIndex, u8* UpgradeData, u16 UpgradeDataLength);	//升级包下载







#endif
