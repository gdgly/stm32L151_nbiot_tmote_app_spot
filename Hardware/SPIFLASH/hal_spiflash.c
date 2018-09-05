/**
  *********************************************************************************************************
  * @file    hal_spiflash.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-07-24
  * @brief   SPI FLASH GD25Q80CSIG Device
  *		   SPI FLASH GD25Q40BSIG Device
  *********************************************************************************************************
  * @attention
  *		   NSS  : PB12
  *		   SCK  : PA5
  *		   MOSI : PA7
  *		   MISO : PA6
  *********************************************************************************************************
  */

#include "hal_spiflash.h"
#include "platform_config.h"
#include "platform_map.h"
#include "delay.h"
#include "usart.h"

SPI_HandleTypeDef GD25Q_SPIFLASH_Handler;
GD25Q_StatusTypeDef GD25QStatus = GD25Q80CSIG_OK;

/**********************************************************************************************************
 @Function			void GD25Q_SPIFLASH_Init(void)
 @Description			GD25Q_SPIFLASH_Init			: GD25Q SPIFLASH 初始化
 @Input				void
 @Return				void
**********************************************************************************************************/
void GD25Q_SPIFLASH_Init(void)
{
	u32 ReadIdentificationID;
	
	GD25Q_SPIFLASH_Handler.Instance				= GD25Q_FLASH_SPIx;						//SPI1
	GD25Q_SPIFLASH_Handler.Init.Mode				= SPI_MODE_MASTER;						//设置SPI工作模式为主模式
	GD25Q_SPIFLASH_Handler.Init.Direction			= SPI_DIRECTION_2LINES;					//设置SPI数据模式为双线模式
	GD25Q_SPIFLASH_Handler.Init.DataSize			= SPI_DATASIZE_8BIT;					//设置SPI数据大小为8位帧结构
	GD25Q_SPIFLASH_Handler.Init.CLKPolarity			= SPI_POLARITY_HIGH;					//串行同步时钟的空闲状态为高电平
	GD25Q_SPIFLASH_Handler.Init.CLKPhase			= SPI_PHASE_2EDGE;						//串行同步时钟的第二个跳变沿数据被采样
	GD25Q_SPIFLASH_Handler.Init.NSS				= SPI_NSS_SOFT;						//NSS信号由软件控制
	
#ifndef SYSTEMCLOCK
	#error No Define SYSTEMCLOCK!
#else
#if (SYSTEMCLOCK == SYSTEMCLOCKMSI)
	GD25Q_SPIFLASH_Handler.Init.BaudRatePrescaler	= SPI_BAUDRATEPRESCALER_2;				//定义波特率与分频值
#elif (SYSTEMCLOCK == SYSTEMCLOCKHSI)
	GD25Q_SPIFLASH_Handler.Init.BaudRatePrescaler	= SPI_BAUDRATEPRESCALER_8;				//定义波特率与分频值
#else
	#error SYSTEMCLOCK Define Error
#endif
#endif
	
	GD25Q_SPIFLASH_Handler.Init.FirstBit			= SPI_FIRSTBIT_MSB;						//数据传输从MSB位开始
	GD25Q_SPIFLASH_Handler.Init.TIMode				= SPI_TIMODE_DISABLE;					//关闭TI模式
	GD25Q_SPIFLASH_Handler.Init.CRCCalculation		= SPI_CRCCALCULATION_DISABLE;				//关闭硬件CRC校验
	GD25Q_SPIFLASH_Handler.Init.CRCPolynomial		= 10;								//CRC值计算多项式
	
	if (HAL_SPI_Init(&GD25Q_SPIFLASH_Handler) != HAL_OK) {
		/* Initialization Error */
		GD25QStatus = GD25Q80CSIG_ERROR;
	}
	
	GD25Q_SPIFLASH_WakeUp();
	ReadIdentificationID = GD25Q_SPIFLASH_ReadIdentificationID();
	if ((ReadIdentificationID != GD25Q80CSIGIdentificationID) && (ReadIdentificationID != GD25Q40BSIGIdentificationID)) {
		GD25QStatus = GD25Q80CSIG_ERROR;
	}
}

/**********************************************************************************************************
 @Function			GD25Q_StatusTypeDef GD25Q_SPIFLASH_Get_Status(void)
 @Description			GD25Q_SPIFLASH_Get_Status	: GD25Q SPIFLASH 状态
 @Input				void
 @Return				GD25Q_StatusTypeDef
**********************************************************************************************************/
GD25Q_StatusTypeDef GD25Q_SPIFLASH_Get_Status(void)
{
#ifdef	GD25Q_80CSIG
	return GD25QStatus;
#else
	return GD25Q80CSIG_ERROR;
#endif
}

/**********************************************************************************************************
 @Function			u8 GD25Q_SPI_FLASH_ReadByte(void)
 @Description			GD25Q_SPI_FLASH_ReadByte		: SPI FLASH 读取一个字节数据
 @Input				void
 @Return				readByte					: 读取到的数据
**********************************************************************************************************/
u8 GD25Q_SPI_FLASH_ReadByte(void)
{
	u8 readByte, sendByte = GD25Q_Dummy_Byte;
	
	if (HAL_SPI_TransmitReceive(&GD25Q_SPIFLASH_Handler, &sendByte, &readByte, 1, 500) != HAL_OK) {
		readByte = GD25Q_Dummy_Byte;
	}
	return readByte;
}

/**********************************************************************************************************
 @Function			u8 GD25Q_SPI_FLASH_SendByte(u8 byte)
 @Description			GD25Q_SPI_FLASH_SendByte		: SPI FLASH 发送一个字节数据
 @Input				byte						: 发送的数据
 @Return				void
**********************************************************************************************************/
u8 GD25Q_SPI_FLASH_SendByte(u8 byte)
{
	u8 readByte = 0x00, sendByte = byte;
	
	if (HAL_SPI_TransmitReceive(&GD25Q_SPIFLASH_Handler, &sendByte, &readByte, 1, 500) != HAL_OK) {
		readByte = GD25Q_Dummy_Byte;
	}
	return readByte;
}

/**********************************************************************************************************
 @Function			u32 GD25Q_SPIFLASH_ReadDeviceID(void)
 @Description			GD25Q_SPIFLASH_ReadDeviceID			: GD25Q SPIFLASH 读取设备ID
 @Input				void
 @Return				设备ID
**********************************************************************************************************/
u32 GD25Q_SPIFLASH_ReadDeviceID(void)
{
	u32 deviceID = 0;
	
	/* 选择SPI FLASH, NSS使能 */
	GD25Q_FLASH_SPIx_NSS_ENABLE();
	
	GD25Q_SPI_FLASH_SendByte(GD25Q_DeviceID);
	GD25Q_SPI_FLASH_SendByte(GD25Q_Dummy_Byte);
	GD25Q_SPI_FLASH_SendByte(GD25Q_Dummy_Byte);
	GD25Q_SPI_FLASH_SendByte(GD25Q_Dummy_Byte);
	
	deviceID = GD25Q_SPI_FLASH_SendByte(GD25Q_Dummy_Byte);
	
	/* 禁用SPI FLASH, NSS失能 */
	GD25Q_FLASH_SPIx_NSS_DISABLE();
	
	return deviceID;
}

/**********************************************************************************************************
 @Function			u32 GD25Q_SPIFLASH_ReadManufactureID(void)
 @Description			GD25Q_SPIFLASH_ReadManufactureID		: GD25Q SPIFLASH 读取厂商ID
 @Input				void
 @Return				厂商ID
**********************************************************************************************************/
u32 GD25Q_SPIFLASH_ReadManufactureID(void)
{
	u32 manufactureID = 0;
	
	/* 选择SPI FLASH, NSS使能 */
	GD25Q_FLASH_SPIx_NSS_ENABLE();
	
	GD25Q_SPI_FLASH_SendByte(GD25Q_ManufactDeviceID);
	GD25Q_SPI_FLASH_SendByte(0x00);
	GD25Q_SPI_FLASH_SendByte(0x00);
	GD25Q_SPI_FLASH_SendByte(0x00);
	manufactureID |= GD25Q_SPI_FLASH_SendByte(GD25Q_Dummy_Byte) << 8;
	manufactureID |= GD25Q_SPI_FLASH_SendByte(GD25Q_Dummy_Byte);
	
	/* 禁用SPI FLASH, NSS失能 */
	GD25Q_FLASH_SPIx_NSS_DISABLE();
	
	return manufactureID;
}

/**********************************************************************************************************
 @Function			u32 GD25Q_SPIFLASH_ReadIdentificationID(void)
 @Description			GD25Q_SPIFLASH_ReadIdentificationID	: GD25Q SPIFLASH 读取型号ID
 @Input				void
 @Return				型号ID
**********************************************************************************************************/
u32 GD25Q_SPIFLASH_ReadIdentificationID(void)
{
	u32 identificationID = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;
	
	/* 选择SPI FLASH, NSS使能 */
	GD25Q_FLASH_SPIx_NSS_ENABLE();
	
	GD25Q_SPI_FLASH_SendByte(GD25Q_JedecDeviceID);
	Temp0 = GD25Q_SPI_FLASH_SendByte(GD25Q_Dummy_Byte);
	Temp1 = GD25Q_SPI_FLASH_SendByte(GD25Q_Dummy_Byte);
	Temp2 = GD25Q_SPI_FLASH_SendByte(GD25Q_Dummy_Byte);
	
	/* 禁用SPI FLASH, NSS失能 */
	GD25Q_FLASH_SPIx_NSS_DISABLE();
	
	identificationID = (Temp0 << 16) | (Temp1 << 8) | Temp2;
	
	return identificationID;
}

/**********************************************************************************************************
 @Function			void GD25Q_SPIFLASH_WriteEnable(void)
 @Description			GD25Q_SPIFLASH_WriteEnable	: GD25Q SPIFLASH 写使能
 @Input				void
 @Return				void
 @attention			WEL置位
**********************************************************************************************************/
void GD25Q_SPIFLASH_WriteEnable(void)
{
	/* 选择SPI FLASH, NSS使能 */
	GD25Q_FLASH_SPIx_NSS_ENABLE();
	
	GD25Q_SPI_FLASH_SendByte(GD25Q_WriteEnable);
	
	/* 禁用SPI FLASH, NSS失能 */
	GD25Q_FLASH_SPIx_NSS_DISABLE();
}

/**********************************************************************************************************
 @Function			void GD25Q_SPIFLASH_WriteDisable(void)
 @Description			GD25Q_SPIFLASH_WriteDisable	: GD25Q SPIFLASH 写禁止
 @Input				void
 @Return				void
 @attention			WEL清零
**********************************************************************************************************/
void GD25Q_SPIFLASH_WriteDisable(void)
{
	/* 选择SPI FLASH, NSS使能 */
	GD25Q_FLASH_SPIx_NSS_ENABLE();
	
	GD25Q_SPI_FLASH_SendByte(GD25Q_WriteDisable);
	
	/* 禁用SPI FLASH, NSS失能 */
	GD25Q_FLASH_SPIx_NSS_DISABLE();
}

/**********************************************************************************************************
 @Function			u8 GD25Q_SPIFLASH_ReadStatusRegister(u8 statusRegister)
 @Description			GD25Q_SPIFLASH_ReadStatusRegister	: GD25Q SPIFLASH 读状态寄存器
 @Input				状态寄存器地址
 @Return				状态寄存器值
**********************************************************************************************************/
u8 GD25Q_SPIFLASH_ReadStatusRegister(u8 statusRegister)
{
	u8 readByte = 0;
	
	/* 选择SPI FLASH, NSS使能 */
	GD25Q_FLASH_SPIx_NSS_ENABLE();
	
	GD25Q_SPI_FLASH_SendByte(statusRegister);
	readByte = GD25Q_SPI_FLASH_SendByte(GD25Q_Dummy_Byte);
	
	/* 禁用SPI FLASH, NSS失能 */
	GD25Q_FLASH_SPIx_NSS_DISABLE();
	
	return readByte;
}

/**********************************************************************************************************
 @Function			void GD25Q_SPIFLASH_WriteStatusRegister(u8 srLow, u8 srHigh)
 @Description			GD25Q_SPIFLASH_WriteStatusRegister	: GD25Q SPIFLASH 写状态寄存器
 @Input				srLow						: 状态寄存器低字节
					srHigh						: 状态寄存器高字节
 @Return				void
**********************************************************************************************************/
void GD25Q_SPIFLASH_WriteStatusRegister(u8 srLow, u8 srHigh)
{
	/* 选择SPI FLASH, NSS使能 */
	GD25Q_FLASH_SPIx_NSS_ENABLE();
	
	GD25Q_SPI_FLASH_SendByte(GD25Q_WriteStatusReg);
	GD25Q_SPI_FLASH_SendByte(srLow);
	GD25Q_SPI_FLASH_SendByte(srHigh);
	
	/* 禁用SPI FLASH, NSS失能 */
	GD25Q_FLASH_SPIx_NSS_DISABLE();
}

/**********************************************************************************************************
 @Function			void GD25Q_SPIFLASH_WaitForBusy(void)
 @Description			GD25Q_SPIFLASH_WaitForBusy		: GD25Q SPIFLASH 等待设备空闲
 @Input				void
 @Return				void
**********************************************************************************************************/
void GD25Q_SPIFLASH_WaitForBusy(void)
{
	u8 flashStatus = 0;
	
	/* 选择SPI FLASH, NSS使能 */
	GD25Q_FLASH_SPIx_NSS_ENABLE();
	
	/* Send "Read Status Register" instruction */
	GD25Q_SPI_FLASH_SendByte(GD25Q_ReadStatusReg1);
	
	/* Loop as long as the memory is busy with a write cycle */
	do {
		flashStatus = GD25Q_SPI_FLASH_SendByte(GD25Q_Dummy_Byte);
	}
	while ((flashStatus & 0x01) == 0x01);
	
	/* 禁用SPI FLASH, NSS失能 */
	GD25Q_FLASH_SPIx_NSS_DISABLE();
}

/**********************************************************************************************************
 @Function			void GD25Q_SPIFLASH_PowerDown(void)
 @Description			GD25Q_SPIFLASH_PowerDown			: GD25Q SPIFLASH 进入掉电模式
 @Input				void
 @Return				void
**********************************************************************************************************/
void GD25Q_SPIFLASH_PowerDown(void)
{
#ifdef GD25Q_80CSIG
	if (GD25Q80CSIG_OK != GD25Q_SPIFLASH_Get_Status()) {
		return;
	}
	
	/* Select the FLASH: Chip Select low */
	GD25Q_FLASH_SPIx_NSS_ENABLE();
	
	/* Send "Power Down" instruction */
	GD25Q_SPI_FLASH_SendByte(GD25Q_PowerDown);
	
	/* Deselect the FLASH: Chip Select high */
	GD25Q_FLASH_SPIx_NSS_DISABLE();
	
	/* Wait for TDP */
	Delay_US(3);
#endif
}

/**********************************************************************************************************
 @Function			void GD25Q_SPIFLASH_WakeUp(void)
 @Description			GD25Q_SPIFLASH_WakeUp			: GD25Q SPIFLASH 唤醒
 @Input				void
 @Return				void
**********************************************************************************************************/
void GD25Q_SPIFLASH_WakeUp(void)
{
#ifdef GD25Q_80CSIG
	if (GD25Q80CSIG_OK != GD25Q_SPIFLASH_Get_Status()) {
		return;
	}
	
	/* Select the FLASH: Chip Select low */
	GD25Q_FLASH_SPIx_NSS_ENABLE();
	
	/* Send "Release Power Down" instruction */
	GD25Q_SPI_FLASH_SendByte(GD25Q_ReleasePowerDown);
	
	/* Deselect the FLASH: Chip Select high */
	GD25Q_FLASH_SPIx_NSS_DISABLE();
	
	/* Wait for TRES1 */
	Delay_US(5);
#endif
}

/**********************************************************************************************************
 @Function			void GD25Q_SPIFLASH_EraseChip(void)
 @Description			GD25Q_SPIFLASH_EraseChip			: GD25Q SPIFLASH 擦除Chip
 @Input				void
 @Return				void
 @attention			800MS
**********************************************************************************************************/
void GD25Q_SPIFLASH_EraseChip(void)
{
#ifdef GD25Q_80CSIG
	if (GD25Q80CSIG_OK != GD25Q_SPIFLASH_Get_Status()) {
		return;
	}
	
	/* 发送FLASH写使能命令 */
	GD25Q_SPIFLASH_WriteEnable();
	
	/* 等待FLASH空闲 */
	GD25Q_SPIFLASH_WaitForBusy();
	
	/* Select the FLASH: Chip Select low */
	GD25Q_FLASH_SPIx_NSS_ENABLE();
	
	/* 发送片擦除命令 */
	GD25Q_SPI_FLASH_SendByte(GD25Q_ChipErase);
	
	/* Deselect the FLASH: Chip Select high */
	GD25Q_FLASH_SPIx_NSS_DISABLE();
	
	/* 等待擦除完毕 */
	GD25Q_SPIFLASH_WaitForBusy();
#endif
}

/**********************************************************************************************************
 @Function			void GD25Q_SPIFLASH_EraseBlock(u32 BlockAddr)
 @Description			GD25Q_SPIFLASH_EraseBlock		: GD25Q SPIFLASH 擦除Block
 @Input				BlockAddr						: 块地址(0x10000倍数)
 @Return				void
 @attention			GD25Q80CSIG SPI FLASH 擦除块大小为64KB(65536Byte),即一个块区大小,在写入数据之前要求先擦除空间
					50MS
**********************************************************************************************************/
void GD25Q_SPIFLASH_EraseBlock(u32 BlockAddr)
{
#ifdef GD25Q_80CSIG
	if (GD25Q80CSIG_OK != GD25Q_SPIFLASH_Get_Status()) {
		return;
	}
	
	/* 发送FLASH写使能命令 */
	GD25Q_SPIFLASH_WriteEnable();
	
	/* 等待FLASH空闲 */
	GD25Q_SPIFLASH_WaitForBusy();
	
	/* Select the FLASH: Chip Select low */
	GD25Q_FLASH_SPIx_NSS_ENABLE();
	
	/* 发送块区擦除命令 */
	GD25Q_SPI_FLASH_SendByte(GD25Q_BlockErase);
	
	/* 发送块区擦除地址高位 */
	GD25Q_SPI_FLASH_SendByte((BlockAddr & 0xFF0000) >> 16);
	
	/* 发送块区擦除地址中位 */
	GD25Q_SPI_FLASH_SendByte((BlockAddr & 0xFF00) >> 8);
	
	/* 发送块区擦除地址低位 */
	GD25Q_SPI_FLASH_SendByte(BlockAddr & 0xFF);
	
	/* Deselect the FLASH: Chip Select high */
	GD25Q_FLASH_SPIx_NSS_DISABLE();
	
	/* 等待擦除完毕 */
	GD25Q_SPIFLASH_WaitForBusy();
#endif
}

/**********************************************************************************************************
 @Function			void GD25Q_SPIFLASH_EraseSector(u32 SectorAddr)
 @Description			GD25Q_SPIFLASH_EraseSector		: GD25Q SPIFLASH 擦除Sector
 @Input				SectorAddr					: 扇区地址(0x1000倍数)
 @Return				void
 @attention			GD25Q80CSIG SPI FLASH 最小擦除块大小为4KB(4096Byte),即一个扇区大小,在写入数据之前要求先擦除空间
					20MS
**********************************************************************************************************/
void GD25Q_SPIFLASH_EraseSector(u32 SectorAddr)
{
#ifdef GD25Q_80CSIG
	if (GD25Q80CSIG_OK != GD25Q_SPIFLASH_Get_Status()) {
		return;
	}
	
	/* 发送FLASH写使能命令 */
	GD25Q_SPIFLASH_WriteEnable();
	
	/* 等待FLASH空闲 */
	GD25Q_SPIFLASH_WaitForBusy();
	
	/* Select the FLASH: Chip Select low */
	GD25Q_FLASH_SPIx_NSS_ENABLE();
	
	/* 发送扇区擦除命令 */
	GD25Q_SPI_FLASH_SendByte(GD25Q_SectorErase);
	
	/* 发送扇区擦除地址高位 */
	GD25Q_SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
	
	/* 发送扇区擦除地址中位 */
	GD25Q_SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
	
	/* 发送扇区擦除地址低位 */
	GD25Q_SPI_FLASH_SendByte(SectorAddr & 0xFF);
	
	/* Deselect the FLASH: Chip Select high */
	GD25Q_FLASH_SPIx_NSS_DISABLE();
	
	/* 等待擦除完毕 */
	GD25Q_SPIFLASH_WaitForBusy();
#endif
}

/**********************************************************************************************************
 @Function			void GD25Q_SPIFLASH_ReadBuffer(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
 @Description			GD25Q_SPIFLASH_ReadBuffer		: GD25Q SPIFLASH 读取SPI FLASH数据
 @Input				pBuffer						: 数据存储区
					ReadAddr						: 数据读取地址
					NumByteToRead					: 读取数据长度
 @Return				void
**********************************************************************************************************/
void GD25Q_SPIFLASH_ReadBuffer(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
#ifdef GD25Q_80CSIG
	if (GD25Q80CSIG_OK != GD25Q_SPIFLASH_Get_Status()) {
		return;
	}
	
	/* Select the FLASH: Chip Select low */
	GD25Q_FLASH_SPIx_NSS_ENABLE();
	
	/* 发送读指令 */
	GD25Q_SPI_FLASH_SendByte(GD25Q_ReadData);
	
	/* 发送读地址高位 */
	GD25Q_SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
	
	/* 发送读地址中位 */
	GD25Q_SPI_FLASH_SendByte((ReadAddr & 0xFF00) >> 8);
	
	/* 发送读地址低位 */
	GD25Q_SPI_FLASH_SendByte(ReadAddr & 0xFF);
	
	/* 读取数据 */
	while (NumByteToRead--) {
		*pBuffer = GD25Q_SPI_FLASH_SendByte(GD25Q_Dummy_Byte);
		pBuffer++;
	}
	
	/* Deselect the FLASH: Chip Select high */
	GD25Q_FLASH_SPIx_NSS_DISABLE();
#endif
}

/**********************************************************************************************************
 @Function			void GD25Q_SPIFLASH_WritePage(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
 @Description			GD25Q_SPIFLASH_WritePage			: GD25Q SPIFLASH 写入SPI FLASH数据Page
 @Input				pBuffer						: 数据存储区
					WriteAddr						: 数据写入地址
					NumByteToWrite					: 写入数据长度
 @Return				void
 @attention			写入字节数不得超过该页写入剩余字节数
**********************************************************************************************************/
void GD25Q_SPIFLASH_WritePage(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
#ifdef GD25Q_80CSIG
	if (GD25Q80CSIG_OK != GD25Q_SPIFLASH_Get_Status()) {
		return;
	}
	
	/* 发送FLASH写使能命令 */
	GD25Q_SPIFLASH_WriteEnable();
	
	/* 等待FLASH空闲 */
	GD25Q_SPIFLASH_WaitForBusy();
	
	/* Select the FLASH: Chip Select low */
	GD25Q_FLASH_SPIx_NSS_ENABLE();
	
	/* 发送页写命令 */
	GD25Q_SPI_FLASH_SendByte(GD25Q_PageProgram);
	
	/* 发送写地址高位 */
	GD25Q_SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
	
	/* 发送写地址中位 */
	GD25Q_SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
	
	/* 发送写地址低位 */
	GD25Q_SPI_FLASH_SendByte(WriteAddr & 0xFF);
	
	/* 写入数据 */
	while (NumByteToWrite--) {
		GD25Q_SPI_FLASH_SendByte(*pBuffer);
		pBuffer++;
	}
	
	/* Deselect the FLASH: Chip Select high */
	GD25Q_FLASH_SPIx_NSS_DISABLE();
	
	/* 等待写入完毕 */
	GD25Q_SPIFLASH_WaitForBusy();
#endif
}

/**********************************************************************************************************
 @Function			void GD25Q_SPIFLASH_WriteBuffer(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
 @Description			GD25Q_SPIFLASH_WriteBuffer		: GD25Q SPIFLASH 写入SPI FLASH数据
 @Input				pBuffer						: 数据存储区
					WriteAddr						: 数据写入地址
					NumByteToWrite					: 写入数据长度
 @Return				void
 @attention			具有自动换页功能
**********************************************************************************************************/
void GD25Q_SPIFLASH_WriteBuffer(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
#ifdef GD25Q_80CSIG
	u16 pagereMain;
	
	if (GD25Q80CSIG_OK != GD25Q_SPIFLASH_Get_Status()) {
		return;
	}
	
	pagereMain = GD25Q80_PAGE_BYTE_SIZE - WriteAddr % GD25Q80_PAGE_BYTE_SIZE;
	if (NumByteToWrite <= pagereMain) pagereMain = NumByteToWrite;
	
	while (true) {
		GD25Q_SPIFLASH_WritePage(pBuffer, WriteAddr, pagereMain);
		if (NumByteToWrite == pagereMain) break;
		else {
			pBuffer += pagereMain;
			WriteAddr += pagereMain;
			
			NumByteToWrite -= pagereMain;
			if (NumByteToWrite > GD25Q80_PAGE_BYTE_SIZE) pagereMain = GD25Q80_PAGE_BYTE_SIZE;
			else pagereMain = NumByteToWrite;
		}
	}
#endif
}

/**********************************************************************************************************
 @Function			unsigned char GD25Q_SPIFLASH_GetByte(u32 ReadAddr)
 @Description			GD25Q_SPIFLASH_GetByte						: GD25Q SPIFLASH 读取1个Byte
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned char GD25Q_SPIFLASH_GetByte(u32 ReadAddr)
{
#ifdef GD25Q_80CSIG
	unsigned char val = 0;
	
	GD25Q_SPIFLASH_ReadBuffer(&val, ReadAddr, 1);
	
	return val;
#endif
}

/**********************************************************************************************************
 @Function			unsigned short GD25Q_SPIFLASH_GetHalfWord(u32 ReadAddr)
 @Description			GD25Q_SPIFLASH_GetHalfWord					: GD25Q SPIFLASH 读取2个Byte
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned short GD25Q_SPIFLASH_GetHalfWord(u32 ReadAddr)
{
#ifdef GD25Q_80CSIG
	unsigned short val = 0;
	unsigned char tmpval[2];
	
	GD25Q_SPIFLASH_ReadBuffer(tmpval, ReadAddr, 2);
	
	val |= tmpval[0];
	val |= tmpval[1] << 8;
	
	return val;
#endif
}

/**********************************************************************************************************
 @Function			unsigned int GD25Q_SPIFLASH_GetWord(u32 ReadAddr)
 @Description			GD25Q_SPIFLASH_GetWord						: GD25Q SPIFLASH 读取4个Byte
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned int GD25Q_SPIFLASH_GetWord(u32 ReadAddr)
{
#ifdef GD25Q_80CSIG
	unsigned int val = 0;
	unsigned char tmpval[4];
	
	GD25Q_SPIFLASH_ReadBuffer(tmpval, ReadAddr, 4);
	
	val |= tmpval[0];
	val |= tmpval[1] << 8;
	val |= tmpval[2] << 16;
	val |= tmpval[3] << 24;
	
	return val;
#endif
}

/**********************************************************************************************************
 @Function			void GD25Q_SPIFLASH_SetByte(u32 WriteAddr, unsigned char val)
 @Description			GD25Q_SPIFLASH_SetByte						: GD25Q SPIFLASH 写入1个Byte
 @Input				val
 @Return				void
**********************************************************************************************************/
void GD25Q_SPIFLASH_SetByte(u32 WriteAddr, unsigned char val)
{
#ifdef GD25Q_80CSIG
	GD25Q_SPIFLASH_WriteBuffer(&val, WriteAddr, 1);
#endif
}

/**********************************************************************************************************
 @Function			void GD25Q_SPIFLASH_SetHalfWord(u32 WriteAddr, unsigned short val)
 @Description			GD25Q_SPIFLASH_SetHalfWord					: GD25Q SPIFLASH 写入2个Byte
 @Input				val
 @Return				void
**********************************************************************************************************/
void GD25Q_SPIFLASH_SetHalfWord(u32 WriteAddr, unsigned short val)
{
#ifdef GD25Q_80CSIG
	unsigned char tmpval[2];
	
	tmpval[0] = val & 0xFF;
	tmpval[1] = (val >> 8) & 0xFF;
	
	GD25Q_SPIFLASH_WriteBuffer(tmpval, WriteAddr, 2);
#endif
}

/**********************************************************************************************************
 @Function			void GD25Q_SPIFLASH_SetWord(u32 WriteAddr, unsigned int val)
 @Description			GD25Q_SPIFLASH_SetWord						: GD25Q SPIFLASH 写入4个Byte
 @Input				val
 @Return				void
**********************************************************************************************************/
void GD25Q_SPIFLASH_SetWord(u32 WriteAddr, unsigned int val)
{
#ifdef GD25Q_80CSIG
	unsigned char tmpval[4];
	
	tmpval[0] = val & 0xFF;
	tmpval[1] = (val >> 8) & 0xFF;
	tmpval[2] = (val >> 16) & 0xFF;
	tmpval[3] = (val >> 24) & 0xFF;
	
	GD25Q_SPIFLASH_WriteBuffer(tmpval, WriteAddr, 4);
#endif
}

/**********************************************************************************************************
 @Function			unsigned int GD25Q_SPIFLASH_GetNumofByte(u32 ReadAddr, u16 AllNum, u8 ByteVal)
 @Description			GD25Q_SPIFLASH_GetNumofByte					: GD25Q SPIFLASH 读取指定地址中指定字节中有该值的个数
 @Input				ReadAddr									: 读取地址
					AllNum									: 读取字节数
					ByteVal									: 匹配值
 @Return				val
**********************************************************************************************************/
unsigned int GD25Q_SPIFLASH_GetNumofByte(u32 ReadAddr, u16 AllNum, u8 ByteVal)
{
#ifdef GD25Q_80CSIG
	unsigned int ReadByteval = 0;
	
	if (GD25Q80CSIG_OK != GD25Q_SPIFLASH_Get_Status()) {
		return 0;
	}
	
	/* Select the FLASH: Chip Select low */
	GD25Q_FLASH_SPIx_NSS_ENABLE();
	
	/* 发送读指令 */
	GD25Q_SPI_FLASH_SendByte(GD25Q_ReadData);
	
	/* 发送读地址高位 */
	GD25Q_SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
	
	/* 发送读地址中位 */
	GD25Q_SPI_FLASH_SendByte((ReadAddr & 0xFF00) >> 8);
	
	/* 发送读地址低位 */
	GD25Q_SPI_FLASH_SendByte(ReadAddr & 0xFF);
	
	/* 读取数据 */
	while (AllNum--) {
		if (ByteVal == GD25Q_SPI_FLASH_SendByte(GD25Q_Dummy_Byte)) {
			ReadByteval++;
		}
	}
	
	/* Deselect the FLASH: Chip Select high */
	GD25Q_FLASH_SPIx_NSS_DISABLE();
	
	return ReadByteval;
#endif
}

/********************************************** END OF FLEE **********************************************/
