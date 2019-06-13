/**
  *********************************************************************************************************
  * @file    hal_testbench.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-07-24
  * @brief   
  *********************************************************************************************************
  * @attention
  *		  
  *
  *********************************************************************************************************
  */

#include "hal_testbench.h"
#include "platform_config.h"
#include "platform_map.h"

/**********************************************************************************************************
 @Function			bool TestBench_FLASH_CheckSubSN(void)
 @Description			TestBench_FLASH_CheckSubSN
 @Input				void
 @Return				bool
**********************************************************************************************************/
bool TestBench_FLASH_CheckSubSN(void)
{
	if ( (0x00 == *(__IO uint8_t *)(FLASH_END - 0)) && \
		(0x00 == *(__IO uint8_t *)(FLASH_END - 1)) && \
		(0x00 == *(__IO uint8_t *)(FLASH_END - 2)) && \
		(0x00 == *(__IO uint8_t *)(FLASH_END - 3)) ) {
		return false;
	}
	else {
		return true;
	}
}

/**********************************************************************************************************
 @Function			void TestBench_FLASH_WriteSubSN(void)
 @Description			TestBench_FLASH_WriteSubSN
 @Input				void
 @Return				void
**********************************************************************************************************/
void TestBench_FLASH_WriteSubSN(void)
{
	unsigned int subsn = 0x00000000;
	
	subsn |= *(__IO uint8_t *)(FLASH_END - 0) << 0;
	subsn |= *(__IO uint8_t *)(FLASH_END - 1) << 8;
	subsn |= *(__IO uint8_t *)(FLASH_END - 2) << 16;
	subsn |= *(__IO uint8_t *)(FLASH_END - 3) << 24;
	
	TCFG_EEPROM_Set_MAC_SN(subsn);
	TCFG_EEPROM_SetVender("mvb");
}

/********************************************** END OF FLEE **********************************************/
