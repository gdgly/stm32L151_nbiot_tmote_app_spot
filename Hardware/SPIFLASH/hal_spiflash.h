#ifndef __HAL_SPI_FLASH_H
#define   __HAL_SPI_FLASH_H

#include "sys.h"

/* Definition for SPIx clock resources */
#define GD25Q_FLASH_SPIx							SPI1
#define GD25Q_FLASH_SPIx_RCC_CLK_ENABLE()			__HAL_RCC_SPI1_CLK_ENABLE()
#define GD25Q_FLASH_SPIx_RCC_CLK_DISABLE()			__HAL_RCC_SPI1_CLK_DISABLE()

#define GD25Q_FLASH_SPIx_FORCE_RESET()				__HAL_RCC_SPI1_FORCE_RESET()
#define GD25Q_FLASH_SPIx_RELEASE_RESET()			__HAL_RCC_SPI1_RELEASE_RESET()

/* Definition for SPIx GPIO resources */
#define GD25Q_FLASH_SPIx_NSS_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE()
#define GD25Q_FLASH_SPIx_NSS_GPIO_CLK_DISABLE()		__HAL_RCC_GPIOB_CLK_DISABLE()
#define GD25Q_FLASH_SPIx_SCK_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()
#define GD25Q_FLASH_SPIx_SCK_GPIO_CLK_DISABLE()		__HAL_RCC_GPIOA_CLK_DISABLE()
#define GD25Q_FLASH_SPIx_MOSI_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()
#define GD25Q_FLASH_SPIx_MOSI_GPIO_CLK_DISABLE()		__HAL_RCC_GPIOA_CLK_DISABLE()
#define GD25Q_FLASH_SPIx_MISO_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()
#define GD25Q_FLASH_SPIx_MISO_GPIO_CLK_DISABLE()		__HAL_RCC_GPIOA_CLK_DISABLE()

/* Definition for SPIx Pins */
#define GD25Q_FLASH_SPIx_NSS_GPIO_PORT				GPIOB
#define GD25Q_FLASH_SPIx_NSS_PIN					GPIO_PIN_12
#define GD25Q_FLASH_SPIx_SCK_GPIO_PORT				GPIOA
#define GD25Q_FLASH_SPIx_SCK_PIN					GPIO_PIN_5
#define GD25Q_FLASH_SPIx_SCK_AF					GPIO_AF5_SPI1
#define GD25Q_FLASH_SPIx_MOSI_GPIO_PORT				GPIOA
#define GD25Q_FLASH_SPIx_MOSI_PIN					GPIO_PIN_7
#define GD25Q_FLASH_SPIx_MOSI_AF					GPIO_AF5_SPI1
#define GD25Q_FLASH_SPIx_MISO_GPIO_PORT				GPIOA
#define GD25Q_FLASH_SPIx_MISO_PIN					GPIO_PIN_6
#define GD25Q_FLASH_SPIx_MISO_AF					GPIO_AF5_SPI1

#define GD25Q_FLASH_SPIx_NSS_ENABLE()				HAL_GPIO_WritePin(GD25Q_FLASH_SPIx_NSS_GPIO_PORT, GD25Q_FLASH_SPIx_NSS_PIN, GPIO_PIN_RESET)
#define GD25Q_FLASH_SPIx_NSS_DISABLE()				HAL_GPIO_WritePin(GD25Q_FLASH_SPIx_NSS_GPIO_PORT, GD25Q_FLASH_SPIx_NSS_PIN, GPIO_PIN_SET)

#define GD25Q_Dummy_Byte							0xFF

/* GD25Q80CSIG 指令表 */
#define GD25Q_WriteEnable						0x06
#define GD25Q_WriteDisable						0x04

#define GD25Q_ReadStatusReg						0x05
#define GD25Q_WriteStatusReg						0x01

#define GD25Q_ReadData							0x03
#define GD25Q_FastReadData						0x0B
#define GD25Q_FastReadDual						0x3B
#define GD25Q_PageProgram						0x02

#define GD25Q_SectorErase						0x20
#define GD25Q_BlockErase							0xD8
#define GD25Q_ChipErase							0xC7

#define GD25Q_PowerDown							0xB9
#define GD25Q_ReleasePowerDown					0xAB
#define GD25Q_DeviceID							0xAB
#define GD25Q_ManufactDeviceID					0x90
#define GD25Q_JedecDeviceID						0x9F
/* GD25Q80CSIG End Command */

extern SPI_HandleTypeDef GD25Q_SPIFLASH_Handler;

void GD25Q_SPIFLASH_Init(void);														//GD25Q SPIFLASH 初始化

u8 GD25Q_SPI_FLASH_ReadByte(void);														//SPI FLASH 读取一个字节数据
u8 GD25Q_SPI_FLASH_SendByte(u8 byte);													//SPI FLASH 发送一个字节数据

u32 GD25Q_SPIFLASH_ReadDeviceID(void);													//GD25Q SPIFLASH 读取设备ID













#endif /* __HAL_SPI_FLASH_H */
