/**
  *********************************************************************************************************
  * @file    hal_spiflash.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-07-24
  * @brief   SPI FLASH GD25Q80CSIG Device
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

SPI_HandleTypeDef GD25Q_SPIFLASH_Handler;

/**********************************************************************************************************
 @Function			void GD25Q_SPIFLASH_Init(void)
 @Description			GD25Q_SPIFLASH_Init			: GD25Q SPIFLASH 初始化
 @Input				void
 @Return				void
**********************************************************************************************************/
void GD25Q_SPIFLASH_Init(void)
{
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
	}
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
 @Description			GD25Q_SPIFLASH_ReadDeviceID	: GD25Q SPIFLASH 读取设备ID
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































/********************************************** END OF FLEE **********************************************/
