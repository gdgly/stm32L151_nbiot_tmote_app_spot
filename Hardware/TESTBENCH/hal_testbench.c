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
#include "delay.h"
#include "usart.h"

TESTBenchClientTypeDef TestBenchClient = {
	.TestState		= 0x00,
	.SendTimes		= 0x03,
	.Data			= {
		.StartCode	= 0xFAFA,
		.DeviceSN		= 0x00000000,
		.mag_X		= 0x0000,
		.mag_Y		= 0x0000,
		.mag_Z		= 0x0000,
		.radarval1	= 0x0000,
		.radarval2	= 0x0000,
		.radarval3	= 0x0000,
		.RF4438ID		= 0x0000,
		.CheckNum		= 0x00,
		.CR			= '\r',
		.LF			= '\n',
	},
};

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

/**********************************************************************************************************
 @Function			void TestBench_StateCtrl_ALL_ENBALE(void)
 @Description			TestBench_StateCtrl_ALL_ENBALE
 @Input				void
 @Return				void
**********************************************************************************************************/
void TestBench_StateCtrl_ALL_ENBALE(void)
{
	TestBenchClient.TestState |=  (1<<0);
	TestBenchClient.SendTimes = 0x03;
}

/**********************************************************************************************************
 @Function			void TestBench_StateCtrl_ALL_DISABLE(void)
 @Description			TestBench_StateCtrl_ALL_DISABLE
 @Input				void
 @Return				void
**********************************************************************************************************/
void TestBench_StateCtrl_ALL_DISABLE(void)
{
	TestBenchClient.TestState &= ~(1<<0);
	TestBenchClient.SendTimes = 0x00;
}

/**********************************************************************************************************
 @Function			bool TestBench_StateCtrl_ALL_STATE(void)
 @Description			TestBench_StateCtrl_ALL_STATE
 @Input				void
 @Return				bool
**********************************************************************************************************/
bool TestBench_StateCtrl_ALL_STATE(void)
{
	return (TestBenchClient.TestState & (1<<0)) ? true : false;
}

/**********************************************************************************************************
 @Function			void TestBench_StateCtrl_RF4438_ENBALE(void)
 @Description			TestBench_StateCtrl_RF4438_ENBALE
 @Input				void
 @Return				void
**********************************************************************************************************/
void TestBench_StateCtrl_RF4438_ENBALE(void)
{
	TestBenchClient.TestState |=  (1<<1);
}

/**********************************************************************************************************
 @Function			void TestBench_StateCtrl_RF4438_DISABLE(void)
 @Description			TestBench_StateCtrl_RF4438_DISABLE
 @Input				void
 @Return				void
**********************************************************************************************************/
void TestBench_StateCtrl_RF4438_DISABLE(void)
{
	TestBenchClient.TestState &= ~(1<<1);
}

/**********************************************************************************************************
 @Function			void TestBench_Tack_RF4438(u16 rfid)
 @Description			TestBench_Tack_RF4438
 @Input				rfid
 @Return				void
**********************************************************************************************************/
void TestBench_Tack_RF4438(u16 rfid)
{
	if (TestBench_StateCtrl_ALL_STATE() != false) {
		TestBenchClient.Data.RF4438ID = rfid;
		TestBench_StateCtrl_RF4438_ENBALE();
	}
}

/**********************************************************************************************************
 @Function			void TestBench_StateCtrl_Magnetism_ENBALE(void)
 @Description			TestBench_StateCtrl_Magnetism_ENBALE
 @Input				void
 @Return				void
**********************************************************************************************************/
void TestBench_StateCtrl_Magnetism_ENBALE(void)
{
	TestBenchClient.TestState |=  (1<<2);
}

/**********************************************************************************************************
 @Function			void TestBench_StateCtrl_Magnetism_DISABLE(void)
 @Description			TestBench_StateCtrl_Magnetism_DISABLE
 @Input				void
 @Return				void
**********************************************************************************************************/
void TestBench_StateCtrl_Magnetism_DISABLE(void)
{
	TestBenchClient.TestState &= ~(1<<2);
}

/**********************************************************************************************************
 @Function			void TestBench_Tack_Magnetism(int16_t mag_x, int16_t mag_y, int16_t mag_z)
 @Description			TestBench_Tack_Magnetism
 @Input				mag_x
					mag_y
					mag_z
 @Return				void
**********************************************************************************************************/
void TestBench_Tack_Magnetism(int16_t mag_x, int16_t mag_y, int16_t mag_z)
{
	if (TestBench_StateCtrl_ALL_STATE() != false) {
		TestBenchClient.Data.mag_X = mag_x;
		TestBenchClient.Data.mag_Y = mag_y;
		TestBenchClient.Data.mag_Z = mag_z;
		TestBench_StateCtrl_Magnetism_ENBALE();
	}
}

/**********************************************************************************************************
 @Function			void TestBench_StateCtrl_Radarval_ENBALE(void)
 @Description			TestBench_StateCtrl_Radarval_ENBALE
 @Input				void
 @Return				void
**********************************************************************************************************/
void TestBench_StateCtrl_Radarval_ENBALE(void)
{
	TestBenchClient.TestState |=  (1<<3);
}

/**********************************************************************************************************
 @Function			void TestBench_StateCtrl_Radarval_DISABLE(void)
 @Description			TestBench_StateCtrl_Radarval_DISABLE
 @Input				void
 @Return				void
**********************************************************************************************************/
void TestBench_StateCtrl_Radarval_DISABLE(void)
{
	TestBenchClient.TestState &= ~(1<<3);
}

/**********************************************************************************************************
 @Function			void TestBench_Tack_Radarval(short radarval1, short radarval2, short radarval3)
 @Description			TestBench_Tack_Radarval
 @Input				radarval1
					radarval2
					radarval3
 @Return				void
**********************************************************************************************************/
void TestBench_Tack_Radarval(short radarval1, short radarval2, short radarval3)
{
	if (TestBench_StateCtrl_ALL_STATE() != false) {
		TestBenchClient.Data.radarval1 = radarval1;
		TestBenchClient.Data.radarval2 = radarval2;
		TestBenchClient.Data.radarval3 = radarval3;
		TestBench_StateCtrl_Radarval_ENBALE();
	}
}

/**********************************************************************************************************
 @Function			void TestBench_StateCtrl_NBIoTval_ENBALE(void)
 @Description			TestBench_StateCtrl_NBIoTval_ENBALE
 @Input				void
 @Return				void
**********************************************************************************************************/
void TestBench_StateCtrl_NBIoTval_ENBALE(void)
{
	TestBenchClient.TestState |=  (1<<4);
}

/**********************************************************************************************************
 @Function			void TestBench_StateCtrl_NBIoTval_DISABLE(void)
 @Description			TestBench_StateCtrl_NBIoTval_DISABLE
 @Input				void
 @Return				void
**********************************************************************************************************/
void TestBench_StateCtrl_NBIoTval_DISABLE(void)
{
	TestBenchClient.TestState &= ~(1<<4);
}

/**********************************************************************************************************
 @Function			void TestBench_Tack_NBIoTval(const char* imei, const char* iccid)
 @Description			TestBench_Tack_NBIoTval
 @Input				imei
					iccid
 @Return				void
**********************************************************************************************************/
void TestBench_Tack_NBIoTval(const char* imei, const char* iccid)
{
	if (TestBench_StateCtrl_ALL_STATE() != false) {
		memcpy((void*)TestBenchClient.Data.imei,  imei,  sizeof(TestBenchClient.Data.imei));
		memcpy((void*)TestBenchClient.Data.iccid, iccid, sizeof(TestBenchClient.Data.iccid));
		TestBench_StateCtrl_NBIoTval_ENBALE();
	}
}

/**********************************************************************************************************
 @Function			void TestBench_Report_Compel(void)
 @Description			TestBench_Report_Compel
 @Input				void
 @Return				void
**********************************************************************************************************/
void TestBench_Report_Compel(void)
{
	if (TestBench_StateCtrl_ALL_STATE() != false) {
		TestBenchClient.TestState |=  (1<<7);
	}
}

/**********************************************************************************************************
 @Function			void TestBench_Report_DeviceData(void)
 @Description			TestBench_Report_DeviceData
 @Input				void
 @Return				void
**********************************************************************************************************/
void TestBench_Report_DeviceData(void)
{
	u8* pcheckval = (u8*)&TestBenchClient.Data;
#if TESTBENCH_ERASER_FLASHSN_TYPE
	FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PAGEError = 0;
#endif
	
	if (TestBenchClient.TestState >= 0x1F) {
		if (TestBenchClient.SendTimes > 0) {
			/* Need Send */
			TestBenchClient.Data.DeviceSN = TCFG_EEPROM_Get_MAC_SN();
			TestBenchClient.Data.CheckNum = TestBenchClient.Data.StartCode >> 8;
			for (int i = 0; i < sizeof(TestBenchClient.Data) - 4; i++) {
				TestBenchClient.Data.CheckNum = TestBenchClient.Data.CheckNum ^ *(pcheckval + i + 1);
			}
			HAL_UART_Transmit(&UART2_Handler, (u8*)&TestBenchClient.Data, sizeof(TestBenchClient.Data), 0x07FF);
			TestBenchClient.SendTimes--;
		}
		else {
			/* Send Over */
			TestBench_StateCtrl_ALL_DISABLE();
#if TESTBENCH_ERASER_FLASHSN_TYPE
			HAL_FLASH_Unlock();
			__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGAERR | FLASH_FLAG_WRPERR);
			EraseInitStruct.TypeErase	= FLASH_TYPEERASE_PAGES;
			EraseInitStruct.PageAddress	= FLASH_END - PAGESIZE;
			EraseInitStruct.NbPages		= 1;
			HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError);
			HAL_FLASH_Lock();
#endif
		}
	}
}









/********************************************** END OF FLEE **********************************************/
