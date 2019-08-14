/**
  *********************************************************************************************************
  * @file    inspectconfig.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-04-02
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "inspectconfig.h"
#include "inspectmessageoperate.h"
#include "stm32l1xx_config.h"
#include "platform_config.h"
#include "platform_map.h"
#include "hal_qmc5883l.h"
#include "hal_rtc.h"
#include "hal_beep.h"
#include "hal_temperature.h"
#include "radar_api.h"
#include "radio_rf_app.h"

SpotStatusTypedef				SpotStatusDataBackUp;

/**********************************************************************************************************
 @Function			char Inspect_Mag_Stable(void)
 @Description			Inspect_Mag_Stable			: 车位检测算法地磁稳定
 @Input				void
 @Return				char
**********************************************************************************************************/
char Inspect_Mag_Stable(void)
{
	char rt = MAG_NO_CHANGE;
	
	if (SpotStatusDataBackUp.spot_status == SPOT_CAR_OCCUPY) {
		rt = talgo_check_mag_motion_lately(TCFG_EEPROM_GetCarInDelay());
	}
	
	return rt;
}

/**********************************************************************************************************
 @Function			void Inspect_Spot_Init(void)
 @Description			Inspect_Spot_Init			: 车位检测算法初始化
 @Input				void
 @Return				void
**********************************************************************************************************/
void Inspect_Spot_Init(void)
{
	unsigned char i;
	
	talgo_register_mag_callback(QMC5883L_ReadData_Single);
	
	talgo_register_radar_callback(Radar_GetData);
	
	talgo_register_dprintf_callback(Radio_Trf_Debug_Printf_Level3);
	
	talgo_register_runningtime_callback(Stm32_GetSecondTick);
	
	talgo_register_unixtime_callback(RTC_GetUnixTimeToStamp);
	
	tradar_register_temperature_callback(TEMPERATURE_ADC_Read);
	
	talgo_register_rssi_callback(TCFG_Utility_Get_Nbiot_Rssi_UnCharVal);
	
#if RADAR_MODEL_TYPE == RADAR_MODEL_V1
	talgo_set_model(TRADAR_V1);
#elif RADAR_MODEL_TYPE == RADAR_MODEL_V2
	tradar_set_sensetivity(TCFG_SystemData.Sensitivity);
	talgo_set_model(TRADAR_V2);
#else
	#error RADAR_MODEL_TYPE Define Error
#endif
	
	talgo_set_magmod(TCFG_EEPROM_GetMagMode());
	
	i = 4 + TCFG_EEPROM_GetRadarRange();
	if ((i < TRADAR_RANGE_0M5) || (i > TRADAR_RANGE_1M6)) {
		i = TRADAR_RANGE_1M1;
	}
	tradar_set_distance_range(i);
}

/**********************************************************************************************************
 @Function			void Inspect_Spot_ExistenceDetect(void)
 @Description			Inspect_Spot_ExistenceDetect	: 车位检测算法处理
 @Input				void
 @Return				void
**********************************************************************************************************/
void Inspect_Spot_ExistenceDetect(void)
{
	TALGO_SPOTSTATUS SpotStatus;
	SpotStatusTypedef SpotStatusData;
	static char noStatusSent = 1;
	static uint32_t time2send_spot = 0;
	static uint8_t status_pre = SPOT_CAR_UNKNOWN;
	static int32_t lasttime2send_radar_at_free = -3600;
	static uint8_t prepare2send_radar = 0;
	
	talgo_set_sensitivity(TCFG_SystemData.Sensitivity - 1);
	tradar_set_gain_in_cover(TCFG_SystemData.CoverGain);
	talgo_set_sense_mode(TCFG_SystemData.SenseMode);
	tradar_set_highpass(TCFG_SystemData.RadarHighPass);
	
	SpotStatus = (TALGO_SPOTSTATUS)talgro_vechile_judge(&SpotStatusData);
	
	/* 车位状态改变(实时) */
	if ((SpotStatus == SPOT_OCCUPY2FREE) || (SpotStatus == SPOT_FREE2OCCUPY)) {
		/* -先记录状态数据, 等连续5~10秒磁场无波动, 那么进行数据发送- */
		SpotStatusDataBackUp = SpotStatusData;
		noStatusSent = 0;
		Radio_Trf_Do_Heartbeat();
		
		Radio_Trf_Debug_Printf_Level3("ispect:errcode=%d", SpotStatusData.radarData.timedomain_dif);
		
		if (SpotStatus == SPOT_OCCUPY2FREE) {
			BEEP_CtrlRepeat_Extend(1, 30, 70);
		}
		else if (SpotStatus == SPOT_FREE2OCCUPY) {
			BEEP_CtrlRepeat_Extend(2, 30, 70);
		}
	}
	/* 车位状态保持不变(等待) */
	else if (((SpotStatus == SPOT_LONGFREE) || (SpotStatus == SPOT_LONGOCCUPY)) && noStatusSent) {
		SpotStatusData.spot_count = 0;
		noStatusSent = 0;
		
		if (SpotStatus == SPOT_LONGFREE) {
			SpotStatusData.spot_status = SPOT_CAR_FREE;
		}
		else if (SpotStatus == SPOT_LONGOCCUPY) {
			SpotStatusData.spot_status = SPOT_CAR_OCCUPY;
		}
		
		SpotStatusData.qmc5883lData.X_Now					= Qmc5883lData.X_Now;
		SpotStatusData.qmc5883lData.Y_Now					= Qmc5883lData.Y_Now;
		SpotStatusData.qmc5883lData.Z_Now					= Qmc5883lData.Z_Now;
		SpotStatusData.qmc5883lData.X_Back					= Qmc5883lData.X_Back;
		SpotStatusData.qmc5883lData.Y_Back					= Qmc5883lData.Y_Back;
		SpotStatusData.qmc5883lData.Z_Back					= Qmc5883lData.Z_Back;
		
		SpotStatusData.qmc5883lDiff.BackVal_Diff			= Qmc5883lDiff.BackVal_Diff;
		SpotStatusData.qmc5883lDiff.GraduaVal_Diff			= Qmc5883lDiff.GraduaVal_Diff;
		
		SpotStatusData.radarData.NotargetNum				= sRadarData.NotargetNum;
		SpotStatusData.radarData.DismagNum					= sRadarData.DismagNum;
		SpotStatusData.radarData.CoverNum					= sRadarData.CoverNum;
		SpotStatusData.radarData.DisVal					= sRadarData.DisVal;
		SpotStatusData.radarData.MagVal					= sRadarData.MagVal;
		SpotStatusData.radarData.Diff						= sRadarData.Diff;
		SpotStatusData.radarData.Diff_v2					= sRadarData.Diff_v2;
		SpotStatusData.radarData.timedomain_square_dif		= sRadarData.timedomain_square_dif;
		
		SpotStatusData.unixTime							= TCFG_EEPROM_GetEventTime();
		SpotStatusData.timeCounter						= Stm32_GetSecondTick();
		
		time2send_spot = Stm32_GetSecondTick();
		Inspect_Message_SpotStatusEnqueue(SpotStatusData);
	}
	
	/* 心跳数据包 */
	if ((Stm32_GetSecondTick() > (time2send_spot + 900 * TCFG_EEPROM_GetNbiotHeart())) && ((Inspect_Detect_LastTime + 10) > Stm32_GetSecondTick())) {
		Inspect_Message_SpotStatusDequeueEx(&SpotStatusData);
		if (SpotStatusData.timeCounter != 0) {
			SpotStatusData.spot_count = 0;
			
			SpotStatusData.qmc5883lData.X_Now				= Qmc5883lData.X_Now;
			SpotStatusData.qmc5883lData.Y_Now				= Qmc5883lData.Y_Now;
			SpotStatusData.qmc5883lData.Z_Now				= Qmc5883lData.Z_Now;
			SpotStatusData.qmc5883lData.X_Back				= Qmc5883lData.X_Back;
			SpotStatusData.qmc5883lData.Y_Back				= Qmc5883lData.Y_Back;
			SpotStatusData.qmc5883lData.Z_Back				= Qmc5883lData.Z_Back;
			
			SpotStatusData.qmc5883lDiff.BackVal_Diff		= Qmc5883lDiff.BackVal_Diff;
			SpotStatusData.qmc5883lDiff.GraduaVal_Diff		= Qmc5883lDiff.GraduaVal_Diff;
			
			SpotStatusData.radarData.NotargetNum			= sRadarData.NotargetNum;
			SpotStatusData.radarData.DismagNum				= sRadarData.DismagNum;
			SpotStatusData.radarData.CoverNum				= sRadarData.CoverNum;
			SpotStatusData.radarData.DisVal				= sRadarData.DisVal;
			SpotStatusData.radarData.MagVal				= sRadarData.MagVal;
			SpotStatusData.radarData.Diff					= sRadarData.Diff;
			SpotStatusData.radarData.Diff_v2				= sRadarData.Diff_v2;
			SpotStatusData.radarData.timedomain_square_dif	= sRadarData.timedomain_square_dif;
			
			SpotStatusData.timeCounter					= Stm32_GetSecondTick();
			
			Inspect_Message_SpotStatusEnqueue(SpotStatusData);
		}
		time2send_spot = Stm32_GetSecondTick();
	}
	
	/* 数据存于缓存 */
	if (SpotStatusDataBackUp.timeCounter != 0) {
		
		if (((MAG_NO_CHANGE == Inspect_Mag_Stable()) && ((time2send_spot+8) < Stm32_GetSecondTick())) || ((SpotStatusDataBackUp.timeCounter+24) < Stm32_GetSecondTick())) {
			if (status_pre != SpotStatusDataBackUp.spot_status) {
				/* -把此刻的雷达数据报告到服务器- */
				SpotStatusDataBackUp.radarData.CoverNum		= sRadarData.Diff_v2;
				SpotStatusDataBackUp.radarData.DisVal		= sRadarData.DisVal;
				SpotStatusDataBackUp.radarData.MagVal		= sRadarData.MagVal;
				SpotStatusDataBackUp.radarData.Diff		= sRadarData.Diff;
				SpotStatusDataBackUp.radarData.Diff_v2		= sRadarData.Diff_v2;
				Inspect_Message_SpotStatusEnqueue(SpotStatusDataBackUp);
				status_pre = SpotStatusDataBackUp.spot_status;
				time2send_spot = Stm32_GetSecondTick();
				if (SpotStatusDataBackUp.spot_status == SPOT_CAR_FREE) {
					if ((lasttime2send_radar_at_free + 3600) < Stm32_GetSecondTick()) {
						lasttime2send_radar_at_free = Stm32_GetSecondTick();
					}
					prepare2send_radar = 60;
				}
				else {
					prepare2send_radar = 0;
				}
			}
			SpotStatusDataBackUp.timeCounter = 0;
		}
	}
	
	/* bug: when raining , the radar change,it will send radar debug info to net */
#if 0
	if (RADAR_NO_CHANGE != talgo_check_radar_motion()) {
		prepare2send_radar = 60;
	}
#endif
	
	if (prepare2send_radar > 0) {
		prepare2send_radar--;
		if ((prepare2send_radar == 0) && (SpotStatusDataBackUp.spot_status == 0)) {
			lasttime2send_radar_at_free = Stm32_GetSecondTick();
		}
	}
}

/********************************************** END OF FLEE **********************************************/
