/**
  *********************************************************************************************************
  * @file    radar_api.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2017-10-31
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  * 
  *
  *********************************************************************************************************
  */

#include "radar_api.h"
#include "radar_adc.h"
#include "radar_dac.h"
#include "radar_dma.h"
#include "radar_timer.h"
#include "stm32_dsp.h"
#include "platform_config.h"
#include "platform_map.h"
#include "stm32l1xx_config.h"
#include "hal_eeprom.h"
#include "hal_beep.h"
#include "hal_vptat.h"
#include "hal_temperature.h"
#include "radio_rf_app.h"
#include "delay.h"
#include "usart.h"
#include "string.h"
#include <math.h>

#define SAMPLE_NUM			TRADAR_SAMPLE_NUM
#define SAMPLE_MAX_TIMES		6

enum TRADAR_MODEL_TYPE
{
	TRADAR_IMSEMI			= 0x00,
	TRADAR_INFINEON		= 0x01
};

#define	XIAMEN
#define	VDDA2V8														//VDDA = 2.8V 0~2633 -> 0~1.8V
//#define	VDDA3V3														//VDDA = 3.3V 0~2234 -> 0~1.8V
//#define	RADAR_VPTAT

#ifdef VDDA3V3
	char		RADER_RANGE	= 6;
	short	RADER_LOW		= 250;
#else
#ifdef XIAMEN
	char		RADER_RANGE	= 15;										// 6x304 +650 = 2486
	short	RADER_LOW		= 100;
#else
	char		RADER_RANGE	= 8;											// 6x304 +650 = 2486
	short	RADER_LOW		= 250;
#endif
#endif

char radar_model = TRADAR_INFINEON;

short val_vptat, val_vptat_adjust;

u16 fre_magBG[TRADAR_BACKGROUND_NUM] = {10};
s16 time_magBG[TRADAR_BACKGROUND_NUM] = {10};

u32 sample_array0[SAMPLE_NUM] = {0};

u16 sample_buffer[SAMPLE_MAX_TIMES][SAMPLE_NUM] = {0};

int n_array = 0;
int flag_main_go = 0;
u8  bgTimes = 1;
u8  radar_trigged_again = 0;

/**********************************************************************************************************
 @Function			void Radar_Init(void)
 @Description			雷达初始化
 @Input				void
 @Return				void
**********************************************************************************************************/
void Radar_Init(void)
{
#if RADAR_MODEL_TYPE == RADAR_MODEL_V1
	tradar_set_model(TRADAR_V1);
#elif RADAR_MODEL_TYPE == RADAR_MODEL_V2
	tradar_set_model(TRADAR_V2);
#else
	#error RADAR_MODEL_TYPE Define Error
#endif
	
	Radar_TIM2_Init(10-1, 32-1);																	//10us中断一次
	__HAL_TIM_DISABLE(&RADAR_TIM2_Handler);															//先关闭TIM2,等待使用雷达时开启
	
	FLASH_EEPROM_ReadBuffer(EEPROM_BASE_ADDR1, (u8 *)fre_magBG, sizeof(fre_magBG));							//读取EEPROM背景值
	FLASH_EEPROM_ReadBuffer(EEPROM_BASE_ADDR1+256, (u8 *)time_magBG, sizeof(time_magBG));					//读取EEPROM背景值
	
	tradar_timedomain_background_set(time_magBG, (sizeof(time_magBG))/2);
	tradar_base_background_set(fre_magBG, (sizeof(fre_magBG))/2);
	tradar_background_set(fre_magBG, (sizeof(fre_magBG))/2);
}

/**********************************************************************************************************
 @Function			u8 Radar_CheckData(u32 datain_t[], u16 in_num)
 @Description			check whether the model is in use
 @Input				datain_t			: sample data
					in_num			: the data number
 @Return				TRADAR_ERROR		: 异常
					TRADAR_OK			: 正确
**********************************************************************************************************/
u8 Radar_CheckData(u32 datain_t[], u16 in_num)
{
	uint16_t i, count = 0;
	
	for (i = 0; i < in_num; i++) {
#ifdef VDDA2V8
		if ((datain_t[i] < 2390) || (datain_t[i] > 2480)) {
			count++;
		}
#else
		if ((datain_t[i] < 2390) || (datain_t[i] > 2470)) {
			count++;
		}
#endif		
	}
	
	if (count < 10) {
		return TRADAR_ERROR;
	}
	else {
		return TRADAR_OK;
	}
}

/**********************************************************************************************************
 @Function			void Radar_InitBG_Cmd(u32 v23456, u32 v7890a, u32 vbcdef, u32 vg)
 @Description			Radar_InitBG_Cmd
 @Input				
 @Return				void
**********************************************************************************************************/
void Radar_InitBG_Cmd(u32 v23456, u32 v7890a, u32 vbcdef, u32 vg)
{
	fre_magBG[2] = (v23456/100000000)%100;
	fre_magBG[3] = (v23456/1000000)%100;
	fre_magBG[4] = (v23456/10000)%100;
	fre_magBG[5] = (v23456/100)%100;
	fre_magBG[6] = v23456%100;
	
	fre_magBG[7] = (v7890a/100000000)%100;
	fre_magBG[8] = (v7890a/1000000)%100;
	fre_magBG[9] = (v7890a/10000)%100;
	fre_magBG[10] = (v7890a/100)%100;
	fre_magBG[11] = v7890a%100;
	
	fre_magBG[12] = (vbcdef/100000000)%100;
	fre_magBG[13] = (vbcdef/1000000)%100;
	fre_magBG[14] = (vbcdef/10000)%100;
	fre_magBG[15] = (vbcdef/100)%100;
	fre_magBG[16] = vbcdef%100;
	
	fre_magBG[17] = vg%100;
	
	FLASH_EEPROM_WriteBuffer(EEPROM_BASE_ADDR1, (u8 *)fre_magBG, sizeof(fre_magBG));
	FLASH_EEPROM_WriteBuffer(EEPROM_BASE_ADDR1+256, (u8 *)time_magBG, sizeof(time_magBG));
	
	tradar_base_background_set(fre_magBG, (sizeof(fre_magBG))/2);
	tradar_background_set(fre_magBG, (sizeof(fre_magBG))/2);
}

/**********************************************************************************************************
 @Function			void Radar_UpdateBG_Cmd(u8 oldgain, u8 newgain)
 @Description			Radar_UpdateBG_Cmd
 @Input				
 @Return				void
**********************************************************************************************************/
void Radar_UpdateBG_Cmd(u8 oldgain, u8 newgain)
{
	FLASH_EEPROM_ReadBuffer(EEPROM_BASE_ADDR1, (u8 *)fre_magBG, sizeof(fre_magBG));							//读取EEPROM背景值
	
	fre_magBG[2] = (fre_magBG[2]*newgain+oldgain/2)/oldgain;
	fre_magBG[3] = (fre_magBG[3]*newgain+oldgain/2)/oldgain;
	fre_magBG[4] = (fre_magBG[4]*newgain+oldgain/2)/oldgain;
	
	FLASH_EEPROM_WriteBuffer(EEPROM_BASE_ADDR1, (u8 *)fre_magBG, sizeof(fre_magBG));
	tradar_background_set(fre_magBG, (sizeof(fre_magBG))/2);
}

/**********************************************************************************************************
 @Function			u8 Radar_InitBackground(char mode)
 @Description			雷达初始化背景值
 @Input				mode
 @Return				void
**********************************************************************************************************/
u8 Radar_InitBackground(char mode)
{
	RADARPOWER(ON);																			//开启雷达电源
	Delay_MS(500);
	RADAR_ENTER_CRITICAL_SECTION();
	
	n_array = 0;
	flag_main_go = 0;
	
	__HAL_TIM_ENABLE(&RADAR_TIM2_Handler);															//启动雷达定时器
	
	memset((void*)fre_magBG, 0, sizeof(fre_magBG));
	memset((void*)time_magBG, 0, sizeof(time_magBG));
	
	bgTimes = 31;																				//标记需初始化雷达背景
	
	while (1) {
		while (flag_main_go != 0) {
			if (Radar_CheckData(sample_array0, SAMPLE_NUM) == TRADAR_OK) {								//雷达数据校验
				if (bgTimes > 1) {
					tradar_background_transform(sample_array0, SAMPLE_NUM, fre_magBG, time_magBG, (sizeof(fre_magBG))/2);
					
					bgTimes--;
					if (bgTimes == 1) {															//初始化背景完成
						if (mode == TO_SAVE_RADAR_BACKGROUND) {										//保存背景
							FLASH_EEPROM_WriteBuffer(EEPROM_BASE_ADDR1, (u8 *)fre_magBG, sizeof(fre_magBG));
							FLASH_EEPROM_WriteBuffer(EEPROM_BASE_ADDR1+256, (u8 *)time_magBG, sizeof(time_magBG));
							tradar_timedomain_background_set(time_magBG,(sizeof(time_magBG))/2);
							tradar_base_background_set(fre_magBG,(sizeof(fre_magBG))/2);
							tradar_background_set(fre_magBG, (sizeof(fre_magBG))/2);
							val_vptat_adjust = val_vptat;
						}
						else if (mode == NOT_SAVE_RADAR_BACKGROUND) {								//不保存背景
							//微调雷达背景时,雷达背景值不能和之前的背景相差太多.
						}
						
						flag_main_go = 0;
						
						__HAL_TIM_DISABLE(&RADAR_TIM2_Handler);										//雷达工作结束关闭定时器
						RADARPOWER(OFF);														//关闭雷达电源
						
						RADAR_EXIT_CRITICAL_SECTION();
						
						return TRADAR_OK;
					}
				}
			}
			else {
				__HAL_TIM_DISABLE(&RADAR_TIM2_Handler);												//雷达工作结束关闭定时器
				RADARPOWER(OFF);																//关闭雷达电源
				
				RADAR_EXIT_CRITICAL_SECTION();
				
				return TRADAR_ERROR;
			}
			
#if 0
#if RADAR_MODEL_TYPE == RADAR_MODEL_V1
			
#elif RADAR_MODEL_TYPE == RADAR_MODEL_V2
			RADARPOWER(OFF);																	//关闭雷达电源
#else
	#error RADAR_MODEL_TYPE Define Error
#endif
#endif
			BEEP_CtrlRepeat_Extend(1, 30, 0);
			
#if 0
#if RADAR_MODEL_TYPE == RADAR_MODEL_V1
			
#elif RADAR_MODEL_TYPE == RADAR_MODEL_V2
			RADARPOWER(ON);																	//开启雷达电源
#else
	#error RADAR_MODEL_TYPE Define Error
#endif
#endif
			Delay_MS(30);
			
			flag_main_go = 0;
		}
	}
}

extern s16 tradar_time_mag_buff[TRADAR_BACKGROUND_NUM];
extern s16 tradar_time_magBG[TRADAR_BACKGROUND_NUM];
tradar_targetinfo_s radar_targetinfo;

/**********************************************************************************************************
 @Function			u8 Radar_GetData(tradar_targetinfo_s* pTargetinfo[], u8 dataNum)
 @Description			Radar_GetData	: 获取雷达数据
 @Input				dataNum 		: 雷达数据条数
 @Return				1 			: 异常
					0 			: 正确
**********************************************************************************************************/
u8 Radar_GetData(tradar_targetinfo_s* pTargetinfo[], u8 dataNum)
{
	uint16_t i;
	uint8_t j;
	
	if (dataNum > SAMPLE_MAX_TIMES) return 1;
	
	TCFG_AddRadarCount();
	RADARPOWER(ON);																			//开启雷达电源
	RADAR_ENTER_CRITICAL_SECTION();
	Delay_MS(5);
	
	n_array = 0;
	flag_main_go = 0;
	
	__HAL_TIM_ENABLE(&RADAR_TIM2_Handler);															//启动雷达定时器
	
	radar_trigged_again = 1;
	
	j = dataNum;
	while (j) {																				// sample and buffer
		if (flag_main_go != 0) {
			j--;
			for (i = 0; i < SAMPLE_NUM; i++) {
				sample_buffer[j][i] = sample_array0[i];
			}
			if (j) flag_main_go = 0;
		}
	}
	
	__HAL_TIM_DISABLE(&RADAR_TIM2_Handler);															//雷达工作结束关闭定时器
	RADARPOWER(OFF);																			//关闭雷达电源
	
	j = dataNum;
	while (j) {																				// calculate the data in buffer
		j--;
		for (i = 0; i < SAMPLE_NUM; i++) {
			sample_array0[i] = sample_buffer[j][i];
		}
		tradar_target_detect(sample_array0, SAMPLE_NUM, pTargetinfo[j]);
		if (pTargetinfo[j]->strenth_total_diff > 255) {
			pTargetinfo[j]->strenth_total_diff = 255;
		}
		if (pTargetinfo[j]->strenth_total_diff_v2 > 255) {
			pTargetinfo[j]->strenth_total_diff_v2 = 255;
		}
		if ((pTargetinfo[j]->pMagNow[2] <= 3) && (pTargetinfo[j]->pMagNow[3] <= 3) && (pTargetinfo[j]->pMagNow[4] <= 3)) {
			__NOP();
		}
		else {
			__NOP();
		}
	}
	
	memcpy(&radar_targetinfo, pTargetinfo[0], sizeof(radar_targetinfo));
	RADAR_EXIT_CRITICAL_SECTION();
	
#ifdef RADAR_DEBUG_LOG_RF_PRINT
//	if (DEBUG_WORK == Radio_Trf_Get_Workmode()) {													//调试信息
	{
		u8 i, error = 1;
		u16 fre_mag_average;
		
		/* if all the strength is smaller than 4 ,error */
		/* if one strength is larger than 10 ,error */
		for (i = 2; i < 10; i++) {
			if (pTargetinfo[0]->pMagBG[i] > 10) {
				error = 2;
				break;
			}
			if (pTargetinfo[0]->pMagNow[i] > 3) {
				error = 0;
			}
		}
		if (error == 1) {
			Radio_Trf_Printf("radar:no signal");
		}
		else if (error == 2) {
			fre_mag_average = (pTargetinfo[0]->pMagBG[2]+pTargetinfo[0]->pMagBG[3]+pTargetinfo[0]->pMagBG[4]+pTargetinfo[0]->pMagBG[5]+pTargetinfo[0]->pMagBG[6])/5;
			Radio_Trf_Printf("radar warn %hu:bad backgroud.%02d %02d %02d %02d %02d;",fre_mag_average,pTargetinfo[0]->pMagBG[2],
			pTargetinfo[0]->pMagBG[3],pTargetinfo[0]->pMagBG[4],pTargetinfo[0]->pMagBG[5],
			pTargetinfo[0]->pMagBG[6]);
		}
		
		Radio_Trf_Debug_Printf_Level3("%dlow%d.%dst%dds%ddf%d&%d;tdf%d.%d;%d,%d,%d,%d",
				dataNum,RADER_LOW,val_vptat,(uint32_t)radar_targetinfo.status,
				(uint32_t)radar_targetinfo.distance_cm,
				radar_targetinfo.strenth_total_diff,radar_targetinfo.strenth_total_diff_v2,
				radar_targetinfo.time_total_square_diff,radar_targetinfo.time_total_diff,
				radar_targetinfo.overnum[0],radar_targetinfo.overnum[1],radar_targetinfo.overnum_bg[0],radar_targetinfo.overnum_bg[1]);
		
		Radio_Trf_Debug_Printf_Level3("n:%02d%02d%02d%02d%02d;%02d%02d%02d%02d%02d;%02d%02d%02d%02d%02d%02d.%d.%d",
				pTargetinfo[0]->pMagNow[2],
				pTargetinfo[0]->pMagNow[3],pTargetinfo[0]->pMagNow[4],pTargetinfo[0]->pMagNow[5],
				pTargetinfo[0]->pMagNow[6],pTargetinfo[0]->pMagNow[7],pTargetinfo[0]->pMagNow[8],
				pTargetinfo[0]->pMagNow[9],pTargetinfo[0]->pMagNow[10],pTargetinfo[0]->pMagNow[11],
				pTargetinfo[0]->pMagNow[12],pTargetinfo[0]->pMagNow[13],pTargetinfo[0]->pMagNow[14],
				pTargetinfo[0]->pMagNow[15],pTargetinfo[0]->pMagNow[16],pTargetinfo[0]->pMagNow[17],
				talgo_get_fredomain_least(),Qmc5883lDiff.BackVal_Diff);
		
		Radio_Trf_Debug_Printf_Level3("b:%02d%02d%02d%02d%02d;%02d%02d%02d%02d%02d;%02d%02d%02d%02d%02d%02d.%d",
				pTargetinfo[0]->pMagBG[2],
				pTargetinfo[0]->pMagBG[3],pTargetinfo[0]->pMagBG[4],pTargetinfo[0]->pMagBG[5],
				pTargetinfo[0]->pMagBG[6],pTargetinfo[0]->pMagBG[7],pTargetinfo[0]->pMagBG[8],
				pTargetinfo[0]->pMagBG[9],pTargetinfo[0]->pMagBG[10],pTargetinfo[0]->pMagBG[11],
				pTargetinfo[0]->pMagBG[12],pTargetinfo[0]->pMagBG[13],pTargetinfo[0]->pMagBG[14],
				pTargetinfo[0]->pMagBG[15],pTargetinfo[0]->pMagBG[16],pTargetinfo[0]->pMagBG[17],
				talgo_get_timedomain_least());
	}
#endif
	
	return 0;
}

/**********************************************************************************************************
 @Function			void Radar_GetSample(void)
 @Description			雷达采样
 @Input				void
 @Return				void
**********************************************************************************************************/
void Radar_GetSample(void)
{
	if (flag_main_go != 0) {
		return;	
	}
	
	if (n_array >= SAMPLE_NUM) {
		n_array = 0;
		flag_main_go = 1;
	}
	
	sample_array0[n_array] = RADAR_ADC_ConvertedValue;
		
	/* 设置DAC通道值 */
	HAL_DAC_SetValue(&RADAR_DAC_Handler, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (SAMPLE_NUM - n_array) * RADER_RANGE + RADER_LOW);
	
	/* 启动DAC */
	HAL_DAC_Start(&RADAR_DAC_Handler, DAC_CHANNEL_1);
	
	n_array++;
}

/**********************************************************************************************************
 @Function			void Radar_GetSample_Time(void)
 @Description			雷达采样节拍
 @Input				void
 @Return				void
**********************************************************************************************************/
void Radar_GetSample_Time(void)
{
	static int xx = 0;
	
	if (xx < 1) {
#ifdef XIAMEN
		xx = 13;
#else
		xx = 13;
#endif
		Radar_GetSample();
	}
	else {
		xx--;
	}
}

/**********************************************************************************************************
 @Function			u16 talgo_get_radartunebase_vptat(void)
 @Description			talgo_get_radartunebase_vptat
 @Input				void
 @Return				void
**********************************************************************************************************/
u16 talgo_get_radartunebase_vptat(void)
{
	val_vptat = VPTAT_ADC_Read(100);
	return 0;
}

/**********************************************************************************************************
 @Function			void Radar_EnterCriticalSection(void)
 @Description			雷达启动初始化配置系统时钟为32MHz HSI
 @Input				void
 @Return				void
**********************************************************************************************************/
void Radar_EnterCriticalSection(void)
{
#if RADAR_MODEL_TYPE == RADAR_MODEL_V1
	//talgo_get_radartunebase_test();
#elif RADAR_MODEL_TYPE == RADAR_MODEL_V2
	talgo_get_radartunebase_vptat();
	/* -<0.1v 那么是国产雷达- */
	if (val_vptat < 10) {
		RADER_RANGE = TCFG_SystemData.RadarGain;							// 6x304 +650 = 2486
		RADER_LOW = 100;
		radar_model = TRADAR_IMSEMI;
	}
	else {
		RADER_RANGE = 8;												// 6x304 +650 = 2486
		RADER_LOW = 250;
		radar_model = TRADAR_INFINEON;
	}
	__NOP();
#else
	#error RADAR_MODEL_TYPE Define Error
#endif
	
#ifndef SYSTEMCLOCK
	#error No Define SYSTEMCLOCK!
#else
#if (SYSTEMCLOCK == SYSTEMCLOCKMSI)
	Stm32_Clock_Init(RCC_PLLMUL_6, RCC_PLLDIV_3);
	Delay_Init(32000);
#endif
#endif
	
	Radar_DMA_Init();
	Radar_ADC_Init();
	Radar_DAC_Init();
}

/**********************************************************************************************************
 @Function			void Radar_EnterCriticalSection(void)
 @Description			雷达退出反初始化配置为原系统时钟MSI
 @Input				void
 @Return				void
**********************************************************************************************************/
void Radar_ExitCriticalSection(void)
{
#ifndef SYSTEMCLOCK
	#error No Define SYSTEMCLOCK!
#else
#if (SYSTEMCLOCK == SYSTEMCLOCKMSI)
	Stm32_MSIClock_Init(RCC_MSIRANGE_6);
	Delay_Init(4194);
#endif
#endif
	
	Radar_ADC_DeInit();
	Radar_DMA_DeInit();
	Radar_DAC_DeInit();
}

/**********************************************************************************************************
 @Function			char Radar_GetModel(void)
 @Description			获取雷达类型
 @Input				void
 @Return				Model
**********************************************************************************************************/
char Radar_GetModel(void)
{
	return radar_model;
}

/********************************************** END OF FLEE **********************************************/
