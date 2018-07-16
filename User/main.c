/**
  *********************************************************************************************************
  * @file    main.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2017-10-23
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "main.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stm32l1xx_config.h"
#include "platform_config.h"
#include "platform_map.h"
#include "hal_rtc.h"
#include "hal_eeprom.h"
#include "hal_iwdg.h"
#include "hal_beep.h"
#include "hal_switch.h"
#include "hal_vbat.h"
#include "hal_vptat.h"
#include "hal_temperature.h"
#include "hal_qmc5883l.h"
#include "net_dns_app.h"
#include "net_coap_app.h"
#include "net_mqttsn_app.h"
#include "net_nbiot_app.h"
#include "radar_api.h"
#include "radio_hal_rf.h"
#include "radio_rf_app.h"
#include "rollingover.h"
#include "tmesh_xtea.h"
#include "inspectconfig.h"
#include "inspectmessageoperate.h"

/****************************************** Select DEBUG *************************************************/
//#define	DEVICE_DEBUG													//定义开启设备调试
/********************************************* DEBUG *****************************************************/
#ifdef	DEVICE_DEBUG
#include "pcpconfig.h"
#include "pcpcrccheck.h"
#include "pcpsock.h"
void DeBugMain(void);
#endif
/****************************************** Debug Ending *************************************************/

/**********************************************************************************************************
 @Function			int main(void)
 @Description			Main
 @Input				void
 @Return				int
**********************************************************************************************************/
int main(void)
{
	HAL_Init();																		//HAL库初始化
	
#ifndef SYSTEMCLOCK
	#error No Define SYSTEMCLOCK!
#else
#if (SYSTEMCLOCK == SYSTEMCLOCKMSI)
	Stm32_MSIClock_Init(RCC_MSIRANGE_6);													//设置时钟MSI->4.194MHz
	Delay_Init(4194);																	//延时初始化4.194MHz系统时钟
#elif (SYSTEMCLOCK == SYSTEMCLOCKHSI)
	Stm32_Clock_Init(RCC_PLLMUL_6, RCC_PLLDIV_3);											//设置时钟HSI->32MHz
	Delay_Init(32000);																	//延时初始化32MHz系统时钟
#else
	#error SYSTEMCLOCK Define Error
#endif
#endif
	
	SoftResetFlag = RCC_ResetFlag_GetStatus();												//获取复位标志位
	
	IWDG_Init(IWDG_PRESCALER_256, 0x0FFF);													//看门狗初始化,溢出时间28s
	RTC_Init();																		//RTC初始化
	
#ifdef MVB_SUBSN
	TCFG_EEPROM_Set_MAC_SN(MVB_SUBSN);														//写入MACSN
	TCFG_EEPROM_SetVender(MVB_BRAND);														//写入Verder
	TCFG_EEPROM_WriteConfigData();														//写入系统配置信息
#endif
	if (TCFG_EEPROM_CheckNewSNorBrand() == true) {											//检测新设备号或厂牌
		TCFG_EEPROM_WriteConfigData();													//写入系统配置信息
	}
	TCFG_EEPROM_SystemInfo_Init();														//系统运行信息初始化
	
	BEEP_CtrlRepeat_Extend(5, 50, 25);														//蜂鸣器
	IWDG_Feed();																		//喂狗
	
	LowPowerCtrlIO_Init();																//低功耗控制IO初始化
	ModulePowerReset_Init();																//模块电源复位
	PowerCtrlIO_Init();																	//电源控制IO初始化
	
	Uart1_Init(9700);																	//串口1初始化
	Uart2_Init(9600);																	//串口2初始化
	
#ifdef RADIO_SI4438
	tmesh_securityInit();																//XTEA加密初始化
	Radio_Rf_Init();																	//SI4438初始化
	Radio_Trf_Xmit_Heartbeat();															//SI4438发送心跳包
#endif
	
	QMC5883L_Init();																	//地磁初始化
	Radar_Init();																		//雷达初始化
	
	Inspect_Spot_Init();																//车位检测算法初始化
	
	if (SoftResetFlag == RCC_RESET_FLAG_PORRST) {
		Radar_InitBackground(TO_SAVE_RADAR_BACKGROUND);										//雷达背景初始化
		QMC5883L_InitBackgroud();														//地磁背景初始化
	}
	
	NET_NBIOT_Initialization();															//NBIOT初始化
	
	BEEP_CtrlRepeat_Extend(10, 50, 25);													//蜂鸣器
	IWDG_Feed();																		//喂狗
	
	Radio_Trf_Printf(" Device Reboot: %d Cause: %d", TCFG_SystemData.DeviceBootCount, SoftResetFlag);	//启动信息
	Radio_Trf_Printf(" Copyright (C) 2018 Movebroad Version:%d.%d", TCFG_Utility_Get_Major_Softnumber(), TCFG_Utility_Get_Sub_Softnumber());
	
	while (true) {
		
#ifdef	DEVICE_DEBUG
		DeBugMain();
#endif
		
		/* 翻转检测处理 */
		RollingOverPollExecution();
		
		/* 喂狗 */
		IWDG_Feed();
		
#if LOWPOWERMODE == LOWPOWERENABLE
		LowPowerBeforeSleepInit();
		LowPowerEnterStop();
		LowPowerAfterSleepInit();
#elif LOWPOWERMODE == LOWPOWERDISABLE
		Delay_MS(1000);
#endif
		
		/* 喂狗 */
		IWDG_Feed();
		
		/* 软重启计数器清0 */
		SystemSoftResetTime = 0;
		
		/* 运行正常BootCount清0 */
		if ((BootUp == true) && (Stm32_GetSecondTick() > 90)) {
			TCFG_EEPROM_SetBootCount(0);
			BootUp = false;
		}
	}
}

/**********************************************************************************************************
 @Function			void MainMajorCycle(void)
 @Description			MainMajorCycle
 @Input				void
 @Return				void
 @attention			MqttSN等待数据接收中需处理程序
**********************************************************************************************************/
void MainMajorCycle(void)
{
	/* 小无线处理 */
	Radio_Trf_App_Task();
	
	/* 车辆检测 */
	Inspect_Spot_ExistenceDetect();
	
	/* 检测是否需要初始化传感器背景 */
	RollingOverInitSensorBackground();
	
	/* 日常处理 */
	MainHandleRoutine();
	
	/* NBIOT Data Processing */
	NET_NBIOT_DataProcessing(&NetNbiotClientHandler);
	
	/* 喂狗 */
	IWDG_Feed();
	
#if LOWPOWERMODE == LOWPOWERENABLE
	LowPowerBeforeSleepInit();
	LowPowerEnterStop();
	LowPowerAfterSleepInit();
#elif LOWPOWERMODE == LOWPOWERDISABLE
	Delay_MS(1000);
#endif
	
	/* 喂狗 */
	IWDG_Feed();
	
	/* 软重启计数器清0 */
	SystemSoftResetTime = 0;
	
	/* 运行正常BootCount清0 */
	if ((BootUp == true) && (Stm32_GetSecondTick() > 90)) {
		TCFG_EEPROM_SetBootCount(0);
		BootUp = false;
	}
}

/* ============================================ 正放处理 =============================================== */

/**********************************************************************************************************
 @Function			void MainRollingEnteredUpWork(void)
 @Description			MainRollingEnteredUpWork					: 已进入正放工作状态
 @Input				void
 @Return				void
**********************************************************************************************************/
void MainRollingEnteredUpWork(void)
{
	Radio_Trf_Debug_Printf_Level1("Entered Up Work");
	BEEP_CtrlRepeat_Extend(3, 30, 70);
	NETCoapNeedSendCode.WorkInfoWait = 3;
	NETMqttSNNeedSendCode.InfoWorkWait = 3;
}

/**********************************************************************************************************
 @Function			void MainRollingEnteringUpWork(void)
 @Description			MainRollingEnteringUpWork				: 将进入正放工作状态
 @Input				void
 @Return				void
**********************************************************************************************************/
void MainRollingEnteringUpWork(void)
{
	Radio_Trf_Debug_Printf_Level1("Entering Up Work");
	BEEP_CtrlRepeat_Extend(1, 500, 0);
}

/**********************************************************************************************************
 @Function			void MainRollingUpwardsActived(void)
 @Description			MainRollingUpwardsActived				: 正放工作中
 @Input				void
 @Return				void
**********************************************************************************************************/
void MainRollingUpwardsActived(void)
{
	/* 车辆检测 */
	Inspect_Spot_ExistenceDetect();
	
	/* 日常处理 */
	MainHandleRoutine();
	
	if (!((NETCoapNeedSendCode.WorkInfoWait > 0) && (NETMqttSNNeedSendCode.InfoWorkWait > 0))) {
		/* NBIOT APP Task */
		NET_NBIOT_App_Task();
	}
	
	/* 小无线处理 */
	Radio_Trf_App_Task();
}

/**********************************************************************************************************
 @Function			void MainRollingEnteringUpWork(void)
 @Description			MainRollingEnteringUpWork				: 正放休眠中
 @Input				void
 @Return				void
**********************************************************************************************************/
void MainRollingUpwardsSleep(void)
{
	/* NBIOT Power OFF */
	if (NBIOTPOWER_IO_READ()) {
		NET_NBIOT_Initialization();
		NBIOTPOWER(OFF);
	}
	
	/* 小无线处理 */
	Radio_Trf_App_Task();
}

/* ============================================ 倒放处理 =============================================== */

/**********************************************************************************************************
 @Function			void MainRollingEnteredDownSleep(void)
 @Description			MainRollingEnteredDownSleep				: 已进入倒放休眠状态
 @Input				void
 @Return				void
**********************************************************************************************************/
void MainRollingEnteredDownSleep(void)
{
	BEEP_CtrlRepeat_Extend(1, 500, 0);
}

/**********************************************************************************************************
 @Function			void MainRollingEnteredDownWork(void)
 @Description			MainRollingEnteredDownWork				: 已进入倒放工作状态
 @Input				void
 @Return				void
**********************************************************************************************************/
void MainRollingEnteredDownWork(void)
{
	NETCoapNeedSendCode.WorkInfoWait = 3;
	NETMqttSNNeedSendCode.InfoWorkWait = 3;
}

/**********************************************************************************************************
 @Function			void MainRollingEnteredDownSleepKeepActived(void)
 @Description			MainRollingEnteredDownSleepKeepActived		: 将进入倒放休眠状态前保持工作
 @Input				void
 @Return				void
**********************************************************************************************************/
void MainRollingEnteredDownSleepKeepActived(void)
{
	/* 日常处理 */
	MainHandleRoutine();
	
	if (!((NETCoapNeedSendCode.WorkInfoWait > 0) && (NETMqttSNNeedSendCode.InfoWorkWait > 0))) {
		/* NBIOT APP Task */
		NET_NBIOT_App_Task();
	}
}

/**********************************************************************************************************
 @Function			void MainRollingEnteringDownSleep(void)
 @Description			MainRollingEnteringDownSleep				: 将进入倒放休眠
 @Input				void
 @Return				void
**********************************************************************************************************/
void MainRollingEnteringDownSleep(void)
{
	/* NBIOT Power OFF */
	if (NBIOTPOWER_IO_READ()) {
		NET_NBIOT_Initialization();
		NBIOTPOWER(OFF);
	}
}

/**********************************************************************************************************
 @Function			void MainRollingDownSleep(void)
 @Description			MainRollingDownSleep					: 倒放休眠中
 @Input				void
 @Return				void
**********************************************************************************************************/
void MainRollingDownSleep(void)
{
	/* NBIOT Power OFF */
	if (NBIOTPOWER_IO_READ()) {
		NET_NBIOT_Initialization();
		NBIOTPOWER(OFF);
	}
}

/* ============================================ 日常处理 =============================================== */

/**********************************************************************************************************
 @Function			void MainHandleRoutine(void)
 @Description			MainHandleRoutine
 @Input				void
 @Return				void
**********************************************************************************************************/
void MainHandleRoutine(void)
{
	static uint32_t radarCountPre = 0;
	uint8_t val8;
	short val16;
	
	/* Every Second Running */
	if (Stm32_GetSecondTick() != SystemRunningTime.seconds) {
		SystemRunningTime.seconds = Stm32_GetSecondTick();
		
		
	}
	/* Every Ten Secound Running */
	if ((Stm32_GetSecondTick() / 10) != SystemRunningTime.tenseconds) {
		SystemRunningTime.tenseconds = Stm32_GetSecondTick() / 10;
		
		if (NETCoapNeedSendCode.WorkInfoWait > 0) {
			if (NETCoapNeedSendCode.WorkInfoWait > 1) {
				__NOP();
			}
			else if (NETCoapNeedSendCode.WorkInfoWait > 0) {
				NETCoapNeedSendCode.WorkInfo = 1;
			}
			NETCoapNeedSendCode.WorkInfoWait--;
		}
		
		if (NETMqttSNNeedSendCode.InfoWorkWait > 0) {
			if (NETMqttSNNeedSendCode.InfoWorkWait > 1) {
				__NOP();
			}
			else if (NETMqttSNNeedSendCode.InfoWorkWait > 0) {
				NETMqttSNNeedSendCode.InfoWork = 1;
			}
			NETMqttSNNeedSendCode.InfoWorkWait--;
		}
	}
	/* Every Minutes Running */
	if ((Stm32_GetSecondTick() / 60) != SystemRunningTime.minutes) {
		SystemRunningTime.minutes = Stm32_GetSecondTick() / 60;
		
		val16 = QMC5883L_Temperature_Read();
		Radio_Trf_Debug_Printf_Level3(" qmc_temp=%hd,x=%dy=%dz=%d ", val16, Qmc5883lData.X_Now, Qmc5883lData.Y_Now, Qmc5883lData.Z_Now);
	}
	/* Every FifteenMinutes Running */
	if ((Stm32_GetSecondTick() / 900) != SystemRunningTime.fifteenMinutes) {
		SystemRunningTime.fifteenMinutes = Stm32_GetSecondTick() / 900;
		
		val8 = TCFG_EEPROM_GetRadarDbgMode();
		if ((val8 != 0 ) && (SystemRunningTime.fifteenMinutes % val8 == 0)) {
			if (radarCountPre != TCFG_GetRadarCount()) {
				radarCountPre = TCFG_GetRadarCount();
				if (TCFG_Utility_GetCoapConnectDayTime() <= (TCFG_EEPROM_GetCoapQuotaTime() / 2)) {
					NETCoapNeedSendCode.RadarInfo = 1;
				}
				NETMqttSNNeedSendCode.InfoRadar = 1;
			}
		}
	}
	/* Every FortyMinutes Running */
	if ((Stm32_GetSecondTick() / 2400) != SystemRunningTime.fortyMinutes) {
		SystemRunningTime.fortyMinutes = Stm32_GetSecondTick() / 2400;
		
		if (RTC_Time_GetCurrentHour() == 0) {
			if (Inspect_Message_SpotStatusisEmpty() != false) {
				NET_NBIOT_Initialization();
			}
			
			/* Coap保持连接时间(一天) */
			TCFG_SystemData.CoapConnectDayTime = 0;
			NbiotClientHandler.CoapConnectDayTimeSec = TCFG_SystemData.CoapConnectDayTime;
			TCFG_EEPROM_SetCoapConnectDayTime(TCFG_SystemData.CoapConnectDayTime);
			
			/* Coap休眠时间(一天) */
			TCFG_SystemData.CoapIdleDayTime = 0;
			NbiotClientHandler.CoapIdleDayTimeSec = TCFG_SystemData.CoapIdleDayTime;
			TCFG_EEPROM_SetCoapIdleDayTime(TCFG_SystemData.CoapIdleDayTime);
			
			if (TCFG_EEPROM_GetNbiotHeart() > NBIOT_HEART_DATA_HOURS) {
				TCFG_SystemData.NBIotHeart = NBIOT_HEART_DATA_HOURS;
				TCFG_EEPROM_SetNbiotHeart(TCFG_SystemData.NBIotHeart);
			}
		}
	}
	/* Every Hours Running */
	if ((Stm32_GetSecondTick() / 3600) != SystemRunningTime.hours) {
		SystemRunningTime.hours = Stm32_GetSecondTick() / 3600;
		
		val8 = TCFG_EEPROM_GetCoapRATimeHour();
		if ((val8 != 0) && (SystemRunningTime.hours % val8 == 0)) {
			NBIOT_COAP_RA_NORMAL_SET_STATE(&NbiotClientHandler, true);
		}
	}
	/* Every TwoHours Running */
	if ((Stm32_GetSecondTick() / 7200) != SystemRunningTime.twoHours) {
		SystemRunningTime.twoHours = Stm32_GetSecondTick() / 7200;
		
		if (TCFG_GetRadarCount() != TCFG_EEPROM_GetRadarCount()) {
			TCFG_EEPROM_SetRadarCount(TCFG_GetRadarCount());
		}
		
		TCFG_SystemData.MagBackgroundX = Qmc5883lData.X_Back;
		TCFG_SystemData.MagBackgroundY = Qmc5883lData.Y_Back;
		TCFG_SystemData.MagBackgroundZ = Qmc5883lData.Z_Back;
		TCFG_EEPROM_SetMagBackgroud(TCFG_SystemData.MagBackgroundX, TCFG_SystemData.MagBackgroundY, TCFG_SystemData.MagBackgroundZ);
		
		TCFG_SystemData.MagBackgroudTemp = Qmc5883lData.temp_back;
		TCFG_EEPROM_SetMagBackgroudTemp(TCFG_SystemData.MagBackgroudTemp);
	}
	/* Every Day Running */
	if ((Stm32_GetSecondTick() / (24*3600)) != SystemRunningTime.days) {
		SystemRunningTime.days = Stm32_GetSecondTick() / (24*3600);
		
		NETCoapNeedSendCode.WorkInfo = 1;
		NETCoapNeedSendCode.DynamicInfo = 1;
		NETMqttSNNeedSendCode.InfoWork = 1;
		NETMqttSNNeedSendCode.InfoDynamic = 1;
		
		TCFG_SystemData.CoapConnectTime = TCFG_Utility_GetCoapConnectTime();
		TCFG_EEPROM_SetCoapConnectTime(TCFG_SystemData.CoapConnectTime);
		
		TCFG_SystemData.CoapIdleTime = TCFG_Utility_GetCoapIdleTime();
		TCFG_EEPROM_SetCoapIdleTime(TCFG_SystemData.CoapIdleTime);
		
		TCFG_SystemData.CoapConnectDayTime = TCFG_Utility_GetCoapConnectDayTime();
		TCFG_EEPROM_SetCoapConnectDayTime(TCFG_SystemData.CoapConnectDayTime);
		
		TCFG_SystemData.CoapIdleDayTime = TCFG_Utility_GetCoapIdleDayTime();
		TCFG_EEPROM_SetCoapIdleDayTime(TCFG_SystemData.CoapIdleDayTime);
	}
}

#ifdef	DEVICE_DEBUG
/********************************************* DEBUG *****************************************************/
NBIOT_StatusTypeDef NBIOT_RunStatus;
u8 TestData[] = {0xFF, 0xFE, 0x01, 0x13, 0x4C, 0x9A, 0x00, 0x00, 0x01, 0x00, 0x00};
u16 TestCheckCode = 0;
u16 TestReadCheckCode = 0;
PCP_MessageDataTypeDef* PCPMessageData;
/****************************************** Debug Ending *************************************************/
/**********************************************************************************************************
 @Function			void DeBugMain(void)
 @Description			DeBugMain
 @Input				void
 @Return				void
**********************************************************************************************************/
void DeBugMain(void)
{
	TCFG_EEPROM_SetBootCount(0);
#if 0
	NBIOT_Neul_NBxx_HardwareReboot(&NbiotClientHandler, 8000);

	NBIOT_RunStatus = NBIOT_Neul_NBxx_SetReportTerminationError(&NbiotClientHandler, CMEEnable);
	NBIOT_RunStatus = NBIOT_Neul_NBxx_CheckReadReportTerminationError(&NbiotClientHandler);
#endif
	
	PCPMessageData = (PCP_MessageDataTypeDef*)TestData;
	TestReadCheckCode = PCPSock_ntohs(PCPMessageData->CRCCheckCode);
	
	PCPMessageData->CRCCheckCode = 0;
	TestCheckCode = PCPCrcCheck_getCrcCheckCode(TestData, 8);
	PCPMessageData->CRCCheckCode = TestCheckCode;
	
	__NOP();
	__NOP();
	
	while (true) {
#if 0
		NBIOT_RunStatus = NBIOT_Neul_NBxx_CheckReadMessageRegistrationStatus(&NbiotClientHandler);
#endif
		__NOP();
		
		/* 小无线处理 */
		Radio_Trf_App_Task();
		
		Delay_MS(1000);
		
		/* 喂狗 */
		IWDG_Feed();
		
		/* 软重启计数器清0 */
		SystemSoftResetTime = 0;
		
		/* 运行正常BootCount清0 */
		if ((BootUp == true) && (Stm32_GetSecondTick() > 90)) {
			TCFG_EEPROM_SetBootCount(0);
			BootUp = false;
		}
	}
}
#endif

/********************************************** END OF FLEE **********************************************/
