/**
  *********************************************************************************************************
  * @file    pcpupgrade.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-08-07
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "pcpupgrade.h"
#include "platform_config.h"
#include "platform_map.h"
#include "stm32l1xx_config.h"
#include "delay.h"
#include "usart.h"
#include "hal_spiflash.h"
#include "hal_rtc.h"
#include "hal_beep.h"
#include "radio_rf_app.h"
#include "radio_hal_rf.h"

static void STMFLASH_ReadBuffer(uint32_t addr, uint8_t *buf, uint32_t length)
{
	while (length--) {
		*buf++ = *(__IO uint8_t *)addr++;
	}
}

/**********************************************************************************************************
 @Function			PCP_ResultCodeTypeDef PCP_Upgrade_BackupCurrentAPP(PCP_ClientsTypeDef* pClient)
 @Description			PCP_Upgrade_BackupCurrentAPP			: PCP升级备份当前APP
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
PCP_ResultCodeTypeDef PCP_Upgrade_BackupCurrentAPP(PCP_ClientsTypeDef* pClient)
{
#ifdef GD25Q_80CSIG
	PCP_ResultCodeTypeDef PCPResultCodeStatus = PCP_ExecuteSuccess;
	
	if (GD25Q80CSIG_OK != GD25Q_SPIFLASH_Get_Status()) {
		PCPResultCodeStatus = PCP_InternalAnomaly;
		goto exit;
	}
	
	Radio_Trf_Printf("Backup APP ...");
	Radio_Rf_Interrupt_Deinit();
	GD25Q_SPIFLASH_WakeUp();
	GD25Q_SPIFLASH_Init();
	
#if UPGRADE_BACKUP_APP_WRITE_TYPE == UPGRADE_BACKUP_APP_WRITE_DISABLE
	if (GD25Q_SPIFLASH_GetByte(APP2_INFO_UPGRADE_STATUS_OFFSET) == 0x55) {
		if (GD25Q_SPIFLASH_GetWord(APP2_INFO_UPGRADE_SOFTVER_OFFSET) == ((SOFTWAREMAJOR<<16)|(SOFTWARESUB<<0))) {
			/* 备份区已有当前版本APP */
			GD25Q_SPIFLASH_PowerDown();
			Radio_Trf_Printf("APP has been backed up");
			PCPResultCodeStatus = PCP_LatestVersion;
			goto exit;
		}
	}
#endif
	
	/* 备份区没有当前APP或APP版本不同 */
	/* 擦除APP2区 */
	GD25Q_SPIFLASH_EraseBlock(APP2_BASE_ADDR + 0 * GD25Q80_BLOCK_BYTE_SIZE);
	GD25Q_SPIFLASH_EraseBlock(APP2_BASE_ADDR + 1 * GD25Q80_BLOCK_BYTE_SIZE);
	GD25Q_SPIFLASH_EraseBlock(APP2_BASE_ADDR + 2 * GD25Q80_BLOCK_BYTE_SIZE);
	GD25Q_SPIFLASH_EraseBlock(APP2_BASE_ADDR + 3 * GD25Q80_BLOCK_BYTE_SIZE);
	/* 写入APP2DATA */
	for (int packIndex = 0; packIndex < 228; packIndex++) {
		STMFLASH_ReadBuffer(APP_LOWEST_ADDRESS + packIndex * 512, pClient->DataProcessStack, 512);
		GD25Q_SPIFLASH_WriteBuffer(pClient->DataProcessStack, APP2_DATA_ADDR + packIndex * 512, 512);
	}
	/* 写入APP2INFO */
	GD25Q_SPIFLASH_SetByte(APP2_INFO_UPGRADE_STATUS_OFFSET, 0x55);										//标识升级包
	GD25Q_SPIFLASH_SetWord(APP2_INFO_UPGRADE_BASEADDR_OFFSET, APP2_DATA_ADDR);								//升级包基地址
	GD25Q_SPIFLASH_SetHalfWord(APP2_INFO_UPGRADE_BLOCKNUM_OFFSET, 228);									//升级包块数
	GD25Q_SPIFLASH_SetHalfWord(APP2_INFO_UPGRADE_BLOCKLEN_OFFSET, 512);									//升级包块长度
	GD25Q_SPIFLASH_SetHalfWord(APP2_INFO_UPGRADE_DATALEN_OFFSET, 512);									//升级包块有效数据长度
	
	GD25Q_SPIFLASH_SetWord(APP2_INFO_UPGRADE_INDEX_OFFSET, 0);											//升级APP序号
	GD25Q_SPIFLASH_SetWord(APP2_INFO_UPGRADE_SOFTVER_OFFSET, ((SOFTWAREMAJOR<<16)|(SOFTWARESUB<<0)));			//升级包版本号
	
	GD25Q_SPIFLASH_PowerDown();
	Radio_Trf_Printf("APP backup over");
	
exit:
	return PCPResultCodeStatus;
#endif
}

/**********************************************************************************************************
 @Function			PCP_ResultCodeTypeDef PCP_Upgrade_NewVersionNotice(PCP_ClientsTypeDef* pClient)
 @Description			PCP_Upgrade_NewVersionNotice			: PCP升级新版本通知处理
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
PCP_ResultCodeTypeDef PCP_Upgrade_NewVersionNotice(PCP_ClientsTypeDef* pClient)
{
#ifdef GD25Q_80CSIG
	PCP_ResultCodeTypeDef PCPResultCodeStatus = PCP_ExecuteSuccess;
	u32 MajorVer = 0, SubVer = 0;
	
	if (GD25Q80CSIG_OK != GD25Q_SPIFLASH_Get_Status()) {
		PCPResultCodeStatus = PCP_InternalAnomaly;
		goto exit;
	}
	
	Radio_Trf_Printf("NewVersion APP ...");
	
	/* 信号质量与信噪比低中断升级 */
	if ((TCFG_Utility_Get_Nbiot_Rssi_IntVal() < UPGRADE_LOW_LIMIT_RSSI) || (TCFG_Utility_Get_Nbiot_RadioSNR() < UPGRADE_LOW_LIMIT_SNR)) {
		Radio_Trf_Printf("Signal Quality Difference");
		PCPResultCodeStatus = PCP_SignalqualityDifference;
		goto exit;
	}
	
	Radio_Rf_Interrupt_Deinit();
	GD25Q_SPIFLASH_WakeUp();
	GD25Q_SPIFLASH_Init();
	
	if (sscanf((const char*)pClient->Parameter.PlatformSoftVersion, "V%d.%d", &MajorVer, &SubVer) <= 0) {
		PCPResultCodeStatus = PCP_InternalAnomaly;
		goto exit;
	}
	if (GD25Q_SPIFLASH_GetWord(APP1_INFO_UPGRADE_SOFTVER_OFFSET) == ((MajorVer<<16)|(SubVer<<0))) {
		if (GD25Q_SPIFLASH_GetByte(APP1_INFO_UPGRADE_STATUS_OFFSET) == 0x55) {
			/* 备份区已有当前升级版本APP */
			GD25Q_SPIFLASH_PowerDown();
			Radio_Trf_Printf("APP has been latestVersion");
			PCPResultCodeStatus = PCP_LatestVersion;
			goto exit;
		}
		else {
			/* 备份区已有该APP正在升级 */
			/* 分片序号调至未下载数据包,实现断点续传 */
			pClient->UpgradeExecution.PackSliceIndex += GD25Q_SPIFLASH_GetNumofByte(APP1_PACKSLICE_STATUS_OFFSET, APP_PACKSLICE_NUM, 0x00);
			if (pClient->Parameter.UpgradePackSliceNum <= pClient->UpgradeExecution.PackSliceIndex) {
				pClient->UpgradeExecution.PackSliceIndex = pClient->Parameter.UpgradePackSliceNum - 1;
			}
			GD25Q_SPIFLASH_PowerDown();
			Radio_Trf_Printf("APP Upgradeing");
			PCPResultCodeStatus = PCP_ExecuteSuccess;
			goto exit;
		}
	}
	
	/* 备份区没有当前升级APP或APP版本不同 */
	/* 擦除APP1区 */
	GD25Q_SPIFLASH_EraseBlock(APP1_BASE_ADDR + 0 * GD25Q80_BLOCK_BYTE_SIZE);
	GD25Q_SPIFLASH_EraseBlock(APP1_BASE_ADDR + 1 * GD25Q80_BLOCK_BYTE_SIZE);
	GD25Q_SPIFLASH_EraseBlock(APP1_BASE_ADDR + 2 * GD25Q80_BLOCK_BYTE_SIZE);
	GD25Q_SPIFLASH_EraseBlock(APP1_BASE_ADDR + 3 * GD25Q80_BLOCK_BYTE_SIZE);
	/* 写入APP1INFO */
	GD25Q_SPIFLASH_SetWord(APP1_INFO_UPGRADE_BASEADDR_OFFSET, APP1_DATA_ADDR);								//升级包基地址
	GD25Q_SPIFLASH_SetHalfWord(APP1_INFO_UPGRADE_BLOCKNUM_OFFSET, pClient->Parameter.UpgradePackSliceNum);		//升级包块数
	GD25Q_SPIFLASH_SetHalfWord(APP1_INFO_UPGRADE_BLOCKLEN_OFFSET, UPGRADE_PACK_SLICE_BLOCK_SIZE);				//升级包块长度
	GD25Q_SPIFLASH_SetHalfWord(APP1_INFO_UPGRADE_DATALEN_OFFSET, pClient->Parameter.UpgradePackSliceSize);		//升级包块有效数据长度
	GD25Q_SPIFLASH_SetWord(APP1_INFO_UPGRADE_SOFTVER_OFFSET, ((MajorVer<<16)|(SubVer<<0)));					//升级包版本号
	GD25Q_SPIFLASH_SetWord(APP1_INFO_DOWNLOAD_TIME_OFFSET, RTC_GetUnixTimeToStamp());						//升级包下载时间
	GD25Q_SPIFLASH_SetWord(APP1_DATA_CHECK_CODE_OFFSET, pClient->Parameter.UpgradePackCheckCode);				//升级包校验码
	
	GD25Q_SPIFLASH_PowerDown();
	Radio_Trf_Printf("APP will start to upgrade");
	
exit:
	return PCPResultCodeStatus;
#endif
}

/**********************************************************************************************************
 @Function			PCP_ResultCodeTypeDef PCP_Upgrade_DataDownload(PCP_ClientsTypeDef* pClient, u16 SliceIndex, u8* UpgradeData, u16 UpgradeDataLength)
 @Description			PCP_Upgrade_DataDownload				: PCP升级包下载处理
 @Input				pClient							: PCP客户端实例
					SliceIndex						: PCP分片序号
					UpgradeData						: 升级包数据
					UpgradeDataLength					: 升级包长度
 @Return				void
**********************************************************************************************************/
PCP_ResultCodeTypeDef PCP_Upgrade_DataDownload(PCP_ClientsTypeDef* pClient, u16 SliceIndex, u8* UpgradeData, u16 UpgradeDataLength)
{
#ifdef GD25Q_80CSIG
	PCP_ResultCodeTypeDef PCPResultCodeStatus = PCP_ExecuteSuccess;
	
	if (GD25Q80CSIG_OK != GD25Q_SPIFLASH_Get_Status()) {
		PCPResultCodeStatus = PCP_InternalAnomaly;
		goto exit;
	}
	
	Radio_Rf_Interrupt_Deinit();
	GD25Q_SPIFLASH_WakeUp();
	GD25Q_SPIFLASH_Init();
	
	if (GD25Q_SPIFLASH_GetByte(APP1_PACKSLICE_STATUS_OFFSET + SliceIndex) != 0xFF) {
		/* 该分片数据已写入 */
		pClient->UpgradeExecution.PackSliceIndex = SliceIndex + 1;
		GD25Q_SPIFLASH_PowerDown();
		Radio_Trf_Printf("PackSlice has been write");
		goto exit;
	}
	
	if (UpgradeDataLength > pClient->UpgradeExecution.PackSliceSize) {
		/* 该分片数据长度异常 */
		GD25Q_SPIFLASH_PowerDown();
		Radio_Trf_Printf("PackSlice Size Fail");
		goto exit;
	}
	
	/* 分片数据写入分片地址 */
	/* 写入APP1DATA */
	GD25Q_SPIFLASH_WriteBuffer(UpgradeData, APP1_DATA_ADDR + SliceIndex * UPGRADE_PACK_SLICE_BLOCK_SIZE, UpgradeDataLength);
	GD25Q_SPIFLASH_SetByte(APP1_PACKSLICE_STATUS_OFFSET + SliceIndex, 0x00);
	
	pClient->UpgradeExecution.PackSliceIndex = SliceIndex + 1;
	GD25Q_SPIFLASH_PowerDown();
	Radio_Trf_Printf("PackSlice write to flash");
	
exit:
	return PCPResultCodeStatus;
#endif
}

/**********************************************************************************************************
 @Function			PCP_ResultCodeTypeDef PCP_Upgrade_DataAssemble(PCP_ClientsTypeDef* pClient)
 @Description			PCP_Upgrade_DataAssemble				: PCP升级包组装校验处理
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
PCP_ResultCodeTypeDef PCP_Upgrade_DataAssemble(PCP_ClientsTypeDef* pClient)
{
#ifdef GD25Q_80CSIG
	PCP_ResultCodeTypeDef PCPResultCodeStatus = PCP_ExecuteSuccess;
	
	if (GD25Q80CSIG_OK != GD25Q_SPIFLASH_Get_Status()) {
		PCPResultCodeStatus = PCP_InternalAnomaly;
		goto exit;
	}
	
	Radio_Rf_Interrupt_Deinit();
	GD25Q_SPIFLASH_WakeUp();
	GD25Q_SPIFLASH_Init();
	
	if (pClient->UpgradeExecution.PackSliceNum != GD25Q_SPIFLASH_GetNumofByte(APP1_PACKSLICE_STATUS_OFFSET, pClient->UpgradeExecution.PackSliceNum, 0x00)) {
		/* 数据包没有下载完全 */
		GD25Q_SPIFLASH_EraseSector(APP1_INFORMATION_ADDR);
		GD25Q_SPIFLASH_PowerDown();
		Radio_Trf_Printf("Upgrade Check Fail");
		PCPResultCodeStatus = PCP_UpgradePacketCheckFail;
		goto exit;
	}
	
	GD25Q_SPIFLASH_PowerDown();
	Radio_Trf_Printf("Upgrade Check Ok");
	
exit:
	return PCPResultCodeStatus;
#endif
}

/**********************************************************************************************************
 @Function			PCP_ResultCodeTypeDef PCP_Upgrade_AfterUpdata(PCP_ClientsTypeDef* pClient)
 @Description			PCP_Upgrade_AfterUpdata				: PCP升级包开始升级处理
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
PCP_ResultCodeTypeDef PCP_Upgrade_AfterUpdata(PCP_ClientsTypeDef* pClient)
{
#ifdef GD25Q_80CSIG
	PCP_ResultCodeTypeDef PCPResultCodeStatus = PCP_ExecuteSuccess;
	
	if (GD25Q80CSIG_OK != GD25Q_SPIFLASH_Get_Status()) {
		PCPResultCodeStatus = PCP_InternalAnomaly;
		goto exit;
	}
	
	Radio_Trf_Printf("Start Boot Upgrade APP ...");
	Radio_Rf_Interrupt_Deinit();
	GD25Q_SPIFLASH_WakeUp();
	GD25Q_SPIFLASH_Init();
	
	if (GD25Q_SPIFLASH_GetByte(APP1_INFO_UPGRADE_STATUS_OFFSET) == 0xFF) {
		GD25Q_SPIFLASH_SetByte(APP1_INFO_UPGRADE_STATUS_OFFSET, 0x55);									//标识有升级包且可升级
	}
	
	TCFG_EEPROM_SetBootMode(TCFG_ENV_BOOTMODE_SPIFLASH_UPGRADE);										//设置升级模式
	BEEP_CtrlRepeat_Extend(5, 50, 50);
	Stm32_System_Software_Reboot();
	
exit:
	return PCPResultCodeStatus;
#endif
}

/**********************************************************************************************************
 @Function			void PCP_Upgrade_PrintUpgradeInfo(void)
 @Description			PCP_Upgrade_PrintUpgradeInfo			: 打印升级包信息
 @Input				void
 @Return				void
**********************************************************************************************************/
void PCP_Upgrade_PrintUpgradeInfo(void)
{
#ifdef GD25Q_80CSIG
	PCP_APPInfoTypeDef APP1Info;
	PCP_APPInfoTypeDef APP2Info;
	
	if (GD25Q80CSIG_OK != GD25Q_SPIFLASH_Get_Status()) {
		Radio_Trf_Printf("SPI FLASH Fail");
		return;
	}
	
	Radio_Rf_Interrupt_Deinit();
	GD25Q_SPIFLASH_WakeUp();
	GD25Q_SPIFLASH_Init();
	
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
