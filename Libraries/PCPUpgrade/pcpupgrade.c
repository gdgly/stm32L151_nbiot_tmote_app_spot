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
#include "radio_rf_app.h"
#include "radio_hal_rf.h"

static void STMFLASH_ReadBuffer(uint32_t addr, uint8_t *buf, uint32_t length)
{
	while (length--) {
		*buf++ = *(__IO uint8_t *)addr++;
	}
}

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef PCP_Upgrade_BackupCurrentAPP(PCP_ClientsTypeDef* pClient)
 @Description			PCP_Upgrade_BackupCurrentAPP			: PCP升级备份当前APP
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
PCP_StatusTypeDef PCP_Upgrade_BackupCurrentAPP(PCP_ClientsTypeDef* pClient)
{
#ifdef GD25Q_80CSIG
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	
	if (GD25Q80CSIG_OK != GD25Q_SPIFLASH_Get_Status()) {
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
	return PCPStatus;
#endif
}



























/********************************************** END OF FLEE **********************************************/
