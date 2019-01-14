/**
  *********************************************************************************************************
  * @file    tmesh_rf_app.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-03-29
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "radio_rf_app.h"
#include "radio_hal_rf.h"
#include "radio_msg_config.h"
#include "radio.h"
#include "tmesh_xtea.h"
#include "platform_map.h"
#include "platform_config.h"
#include "stm32l1xx_config.h"
#include "hal_beep.h"
#include "hal_qmc5883l.h"
#include "radar_api.h"
#include "inspectconfig.h"
#include "net_nbiot_app.h"
#include "pcpupgrade.h"
#include "delay.h"
#include "string.h"
#include <stdarg.h>

#include "tmesh_algorithm.h"

static int32_t cmdtime_pre = -180;
char gateway_nearby = 10;

static frameInfo_t sInFrameQ[SIZE_INFRAME_Q];

unsigned char TRF_SendBuf[RF_BUFFER_SIZE];
unsigned char TRF_RecvBuf[RF_BUFFER_SIZE];
unsigned char TRF_PrintfBuf[RF_PRINTF_BUFFER_SIZE];

/**********************************************************************************************************
 @Function			void Radio_Rf_QInit(void)
 @Description			Radio_Rf_QInit
 @Input				void
 @Return				void
**********************************************************************************************************/
void Radio_Rf_QInit(void)
{
	memset(sInFrameQ, 0, sizeof(sInFrameQ));
}

/**********************************************************************************************************
 @Function			void Radio_Rf_QPush(mrfiPacket_t* rfpacket)
 @Description			Radio_Rf_QPush
 @Input				rfpacket
 @Return				void
**********************************************************************************************************/
void Radio_Rf_QPush(mrfiPacket_t* rfpacket)
{
	int8_t i, oldest = 0, avail = -1;
	
	for (i = 0; i < SIZE_INFRAME_Q; i++) {
		if (sInFrameQ[i].fi_usage == FI_AVAILABLE) {
			avail = i;
		}
		if (sInFrameQ[i].orderStamp < sInFrameQ[oldest].orderStamp) {
			oldest = i;
		}
	}
	
	if (avail < 0) {
		avail = oldest;
	}
	
	memcpy(&sInFrameQ[avail].mrfiPkt, rfpacket, sizeof(mrfiPacket_t));
	sInFrameQ[avail].fi_usage = FI_INUSE_UNTIL_DEL;
	sInFrameQ[avail].orderStamp = sInFrameQ[oldest].orderStamp++;
}

/**********************************************************************************************************
 @Function			mrfiPacket_t* Radio_Rf_QPop(void)
 @Description			Radio_Rf_QPop
 @Input				void
 @Return				rfpacket
**********************************************************************************************************/
mrfiPacket_t* Radio_Rf_QPop(void)
{
	int8_t i;
	
	for (i = 0; i < SIZE_INFRAME_Q; i++) {
		if (sInFrameQ[i].fi_usage == FI_INUSE_UNTIL_DEL) {
			sInFrameQ[i].fi_usage = FI_AVAILABLE;
			return &sInFrameQ[i].mrfiPkt;
		}
	}
	
	return NULL;
}

/**********************************************************************************************************
 @Function			char Radio_Rf_BeforeSleep(void)
 @Description			Radio_Rf_BeforeSleep		: set the radio to sleep mode
 @Input				void
 @Return				void
**********************************************************************************************************/
void Radio_Rf_BeforeSleep(void)
{
#ifdef	RADIO_SI4438
	if (Radio_Rf_is_Sleep() == 0) {
		Radio_Rf_Sleep();
		Radio_Rf_Interrupt_Deinit();
		Radio_Rf_Interface_Deinit();
	}
#endif
}

/**********************************************************************************************************
 @Function			void Radio_Rf_Data_Handle_Isr(mrfiPacket_t* rfpacket)
 @Description			Radio_Rf_Data_Handle_Isr
 @Input				rfpacket
 @Return				void
**********************************************************************************************************/
void Radio_Rf_Data_Handle_Isr(mrfiPacket_t* rfpacket)
{
	Radio_Rf_QPush(rfpacket);
}

/**********************************************************************************************************
 @Function			char Radio_Rf_Receive(uint8_t *outmsg, uint8_t *len)
 @Description			Radio_Rf_Receive
 @Input				outmsg
					len
 @Return				void
**********************************************************************************************************/
char Radio_Rf_Receive(uint8_t *outmsg, uint8_t *len)
{
	mrfiPacket_t* rfpacket;
	rfpacket = Radio_Rf_QPop();
	
	if (rfpacket != NULL) {
		*len = 1 + (CFG_GET_FROM_FRAME(CFG_P_FRAME_LEN(rfpacket->frame), CFG_LEN_OS));
		memcpy(outmsg, rfpacket->frame, *len);
		
		return TRF_SUCCESS;
	}
	
	return TRF_NO_PAYLOAD;
}

/**********************************************************************************************************
 @Function			char Radio_Rf_Send(uint8_t *inmsg, uint8_t len)
 @Description			Radio_Rf_Send
 @Input				inmsg
					len
 @Return				void
**********************************************************************************************************/
char Radio_Rf_Send(uint8_t *inmsg, uint8_t len)
{
	static char error_cnt = 0;
	char rc;
	
	rc = Radio_Rf_PrepareToTx(inmsg, len);
	if (rc == TRF_ERROR) {
		error_cnt++;
		if (error_cnt > 30) {
			trf_status = TRF_ERROR;
		}
		else if ((error_cnt % 3) == 0) {
			/* 初始化地磁和无线 */
			ReInitModule();
		}
	}
	else {
		error_cnt = 0;
	}
	
	return rc;
}

/**********************************************************************************************************
 @Function			char Radio_Rf_Operate_Recvmsg(uint8_t *inmsg, uint8_t len)
 @Description			Radio_Rf_Operate_Recvmsg
 @Input				inmsg
					len
 @Return				void
**********************************************************************************************************/
char Radio_Rf_Operate_Recvmsg(uint8_t *inmsg, uint8_t len)
{
	char rc = TRF_SUCCESS;
	trf_msg_s* pPayload;
	
	unsigned int mac_sn = 0;
	unsigned int uval32 = 0;
	unsigned short int uval16 = 0;
	short magTempCoefX, magTempCoefY, magTempCoefZ;
#if RADIO_DOWNLOAD_CMD_UPLIMIT
	short limitRssi, limitSnr;
#endif
	
	mac_sn = TCFG_EEPROM_Get_MAC_SN();
	
	if (CFG_GET_FROM_FRAME(CFG_P_FRAME_PRE(inmsg), CFG_PRE_TYPE_OS) == XMESHCFG_PRT)
	{
		pPayload = (trf_msg_s*)CFG_P_FRAME_PAYLOAD(inmsg);
		if (CFG_GET_FROM_FRAME(CFG_P_FRAME_HEAD(inmsg), CFG_HEAD_TYPE_OS) == TMOTE_PLAIN_SET)
		{
			uint32_t cnt;
			uint8_t payload_len;
			
			cnt = CFG_GET_FROM_FRAME(CFG_P_FRAME_HEAD(inmsg), CFG_PKTNUM_OS);
			payload_len = CFG_GET_PAYLOAD_LEN(inmsg) + CFG_FRAME_LEN_SIZE;
			
			tmesh_securityMix(mac_sn);
			tmesh_decipher((uint8_t*)pPayload, payload_len , &cnt);
			
			if (pPayload->head.destSN != mac_sn) {
				rc = TRF_NOT_FORME;
				return rc;
			}
			
			BEEP_CtrlRepeat_Extend(1, 300, 0);
			
			/* 升级指令 */
			if (pPayload->head.type == TRF_MSG_UPGRADE) {
				for (int i = 0; i < 5; i++) {
					TCFG_EEPROM_SetBootMode(TCFG_ENV_BOOTMODE_TOUPDATE);
					if (TCFG_ENV_BOOTMODE_TOUPDATE == TCFG_EEPROM_GetBootMode()) {
						Radio_Trf_Default_Resp(100, TRF_MSG_UPGRADE);
						BEEP_CtrlRepeat_Extend(5, 25, 25);
						Stm32_System_Software_Reboot(RBTMODE_RADIO_UPGRADE);
					}
				}
			}
			/* 传感器灵敏度配置指令 */
			else if (pPayload->head.type == TRF_MSG_SENSITIVITY) {
				TCFG_SystemData.Sensitivity = ((tmote_sensitivity_s*)CFG_P_FRAME_PAYLOAD(inmsg))->sensitivity;
				if ((TCFG_SystemData.Sensitivity > SENSE_LOWEST) || (TCFG_SystemData.Sensitivity < SENSE_HIGHEST)) {
					TCFG_SystemData.Sensitivity = SENSE_MIDDLE;
					TCFG_EEPROM_SetSavedSensitivity(TCFG_SystemData.Sensitivity);
				}
				else {
					TCFG_EEPROM_SetSavedSensitivity(TCFG_SystemData.Sensitivity);
				}
				#if RADIO_CMD_ECHO_TYPE
				Radio_Trf_Printf("Inspect Spot Sensitivity : %hu", TCFG_EEPROM_GetSavedSensitivity());
				#endif
			}
			/* 工作模式配置指令 */
			else if (pPayload->head.type == TRF_MSG_WORKMODE) {
				TCFG_SystemData.WorkMode = ((tmote_work_mode_s*)CFG_P_FRAME_PAYLOAD(inmsg))->mode;
				if ((TCFG_SystemData.WorkMode != DEBUG_WORK) && (TCFG_SystemData.WorkMode != NORMAL_WORK)) {
					TCFG_SystemData.WorkMode = NORMAL_WORK;
					TCFG_EEPROM_SetWorkMode(TCFG_SystemData.WorkMode);
				}
				else {
					TCFG_EEPROM_SetWorkMode(TCFG_SystemData.WorkMode);
				}
				#if RADIO_CMD_ECHO_TYPE
				Radio_Trf_Printf("Work Mode : %hu", TCFG_EEPROM_GetWorkMode());
				#endif
			}
			/* 无线心跳间隔时间配置指令 */
			else if (pPayload->head.type == TRF_MSG_RFHEART_INTERVAL) {
				TCFG_SystemData.Heartinterval = ((tmote_beat_interval_s*)CFG_P_FRAME_PAYLOAD(inmsg))->seconds;
				if ((TCFG_SystemData.Heartinterval > 120) || (TCFG_SystemData.Heartinterval < 1)) {
					TCFG_SystemData.Heartinterval = HEART_INTERVAL;
					TCFG_EEPROM_SetHeartinterval(TCFG_SystemData.Heartinterval);
				}
				else {
					TCFG_EEPROM_SetHeartinterval(TCFG_SystemData.Heartinterval);
				}
				#if RADIO_CMD_ECHO_TYPE
				Radio_Trf_Printf("RF Heartinter val : %hu", TCFG_EEPROM_GetHeartinterval());
				#endif
			}
			/* 初始化传感器指令 */
			else if (pPayload->head.type == TRF_MSG_INITBACKGROUND) {
				Radar_InitBackground(TO_SAVE_RADAR_BACKGROUND);
				QMC5883L_InitBackgroud();
				#if RADIO_CMD_ECHO_TYPE
				Radio_Trf_Printf("Init Sensor Background");
				#endif
			}
			/* 其他下行指令 */
			else if (pPayload->head.type == TRF_MSG_GENERAL_CMD) {
				((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf[15] = 0;
				/* Reboot */
			#if RADIO_DOWNLOAD_CMD_REBOOT
				if (strstr(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "reboot")) {
					BEEP_CtrlRepeat_Extend(2, 500, 250);
				#if RADIO_CMD_ECHO_TYPE
					Radio_Trf_Printf("Reboot : OK");
				#endif
					Stm32_System_Software_Reboot(RBTMODE_RADIO_COMMAND);
				}
			#endif
				/* NewSn */
			#if RADIO_DOWNLOAD_CMD_NEWSN
				else if (strstr(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "newsn")) {
					sscanf(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "newsn:%08x", &uval32);
					TCFG_EEPROM_Set_MAC_SN(uval32);
				#if RADIO_CMD_ECHO_TYPE
					Radio_Trf_Printf("New SN : %08x", TCFG_EEPROM_Get_MAC_SN());
				#endif
				}
			#endif
				/* CDPIP */
			#if RADIO_DOWNLOAD_CMD_CDPIP
				else if (strstr(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "ip")) {
					sscanf(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "ip%08x:%hu", &uval32, &uval16);
					TCFG_EEPROM_SetServerIP(uval32);
					TCFG_EEPROM_SetServerPort(uval16);
					TCFG_SystemData.NBCoapCDPServer.ip.ip32 = TCFG_EEPROM_GetServerIP();
					TCFG_SystemData.NBCoapCDPServer.port = TCFG_EEPROM_GetServerPort();
					#if NBCOAP_SENDCODE_DYNAMIC_INFO
					NETCoapNeedSendCode.DynamicInfo = 1;
					#endif
					NET_NBIOT_Initialization();
				#if RADIO_CMD_ECHO_TYPE
					Radio_Trf_Printf("CDP IP %d.%d.%d.%d:%d", 
					TCFG_SystemData.NBCoapCDPServer.ip.ip8[3], TCFG_SystemData.NBCoapCDPServer.ip.ip8[2], 
					TCFG_SystemData.NBCoapCDPServer.ip.ip8[1], TCFG_SystemData.NBCoapCDPServer.ip.ip8[0], 
					TCFG_SystemData.NBCoapCDPServer.port);
				#endif
				}
			#endif
				/* Active */
			#if RADIO_DOWNLOAD_CMD_ACTIVE
				else if (strstr(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "active")) {
					sscanf(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "active:%hu", &uval16);
					TCFG_EEPROM_SetActiveDevice(uval16);
				#if RADIO_CMD_ECHO_TYPE
					Radio_Trf_Printf("Active : %hu", TCFG_EEPROM_GetActiveDevice());
				#endif
					if (uval16) {
						DeviceActivedMode = true;
						BEEP_CtrlRepeat_Extend(5, 30, 70);
				#if NETPROTOCAL == NETCOAP
					#if NBCOAP_SENDCODE_WORK_INFO
						NETCoapNeedSendCode.WorkInfo = 1;
					#endif
				#elif NETPROTOCAL == NETMQTTSN
					#if NBMQTTSN_SENDCODE_WORK_INFO
						NETMqttSNNeedSendCode.InfoWork = 1;
					#endif
				#elif NETPROTOCAL == NETONENET
					#if NBONENET_SENDCODE_WORK_INFO
						NETOneNETNeedSendCode.WorkInfo = 1;
					#endif
				#endif
					}
					else {
						DeviceActivedMode = false;
						BEEP_CtrlRepeat_Extend(1, 500, 0);
					}
				}
			#endif
				/* Magmod */
			#if RADIO_DOWNLOAD_CMD_MAGMOD
				else if (strstr(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "magmod")) {
					sscanf(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "magmod:%hu", &uval16);
					TCFG_EEPROM_SetMagMode(uval16);
					talgo_set_magmod(uval16);
				#if RADIO_CMD_ECHO_TYPE
					Radio_Trf_Printf("MagMod : %hu", TCFG_EEPROM_GetMagMode());
				#endif
				}
			#endif
				/* NbHeart */
			#if RADIO_DOWNLOAD_CMD_NBHEART
				else if (strstr(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "nbheart")) {
					sscanf(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "nbheart:%hu", &uval16);
					TCFG_EEPROM_SetNbiotHeart(uval16);
					TCFG_SystemData.NBIotHeart = TCFG_EEPROM_GetNbiotHeart();
				#if RADIO_CMD_ECHO_TYPE
					Radio_Trf_Printf("NbHeart : %hu", TCFG_SystemData.NBIotHeart);
				#endif
				}
			#endif
				/* DisRange */
			#if RADIO_DOWNLOAD_CMD_DISRANGE
				else if (strstr(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "disrange")) {
					sscanf(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "disrange:%hu", &uval16);
					tradar_set_distance_range(uval16 + 4);
					TCFG_EEPROM_SetRadarRange(uval16);
					TCFG_SystemData.RadarRange = TCFG_EEPROM_GetRadarRange();
				#if RADIO_CMD_ECHO_TYPE
					Radio_Trf_Printf("RadarRange : %hu", TCFG_SystemData.RadarRange);
				#endif
				}
			#endif
				/* MagInit */
			#if RADIO_DOWNLOAD_CMD_MAGINIT
				else if (strstr(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "maginit")) {
					QMC5883L_InitBackgroud();
					TCFG_EEPROM_SetMagManualBack(0, Qmc5883lData.X_Back, Qmc5883lData.Y_Back, Qmc5883lData.Z_Back);
					BEEP_CtrlRepeat_Extend(5, 30, 70);
				#if RADIO_CMD_ECHO_TYPE
					Radio_Trf_Printf("MagInit : OK");
				#endif
				}
			#endif
				/* CarInDelay */
			#if RADIO_DOWNLOAD_CMD_CARINDELAY
				else if (strstr(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "indelay")) {
					sscanf(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "indelay:%hu", &uval16);
					TCFG_EEPROM_SetCarInDelay(uval16);
					TCFG_SystemData.CarInDelay = TCFG_EEPROM_GetCarInDelay();
				#if RADIO_CMD_ECHO_TYPE
					Radio_Trf_Printf("CarInDelay : %hu", TCFG_SystemData.CarInDelay);
				#endif
				}
			#endif
				/* RFDprintLv */
			#if RADIO_DOWNLOAD_CMD_RFDPRINTLV
				else if (strstr(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "rfdplv")) {
					sscanf(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "rfdplv:%hu", &uval16);
					TCFG_EEPROM_SetRFDprintLv(uval16);
					TCFG_SystemData.RFDprintLv = TCFG_EEPROM_GetRFDprintLv();
				#if RADIO_CMD_ECHO_TYPE
					Radio_Trf_Printf("RFDprintLv : %hu", TCFG_SystemData.RFDprintLv);
				#endif
				}
			#endif
				/* RATime */
			#if RADIO_DOWNLOAD_CMD_RATIME
				else if (strstr(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "ratime")) {
					sscanf(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "ratime:%hu", &uval16);
					TCFG_EEPROM_SetCoapRATimeHour(uval16);
					TCFG_SystemData.CoapRATimeHour = TCFG_EEPROM_GetCoapRATimeHour();
				#if RADIO_CMD_ECHO_TYPE
					Radio_Trf_Printf("RATime : %hu", TCFG_SystemData.CoapRATimeHour);
				#endif
				}
			#endif
				/* MagTempCoef */
			#if RADIO_DOWNLOAD_CMD_MAGTEMPCOEF
				else if (strstr(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "coef")) {
					sscanf(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "coef:%hd,%hd,%hd", &magTempCoefX, &magTempCoefY, &magTempCoefZ);
					TCFG_SystemData.MagCoefX = magTempCoefX;
					TCFG_SystemData.MagCoefY = magTempCoefY;
					TCFG_SystemData.MagCoefZ = magTempCoefZ;
					TCFG_EEPROM_SetMagTempCoef(TCFG_SystemData.MagCoefX, TCFG_SystemData.MagCoefY, TCFG_SystemData.MagCoefZ);
				#if RADIO_CMD_ECHO_TYPE
					Radio_Trf_Printf("Coef:%hd,%hd,%hd", TCFG_SystemData.MagCoefX, TCFG_SystemData.MagCoefY, TCFG_SystemData.MagCoefZ);
				#endif
				}
			#endif
				/* SetQmcCoef */
			#if RADIO_DOWNLOAD_CMD_SETQMCCOEF
				else if (strstr(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "SetQmcCoef")) {
					QMC5883L_measure_qmc_coef((signed char*)&magTempCoefX, (signed char*)&magTempCoefY, (signed char*)&magTempCoefZ);
					TCFG_SystemData.MagCoefX = magTempCoefX;
					TCFG_SystemData.MagCoefY = magTempCoefY;
					TCFG_SystemData.MagCoefZ = magTempCoefZ;
					TCFG_EEPROM_SetMagTempCoef(TCFG_SystemData.MagCoefX, TCFG_SystemData.MagCoefY, TCFG_SystemData.MagCoefZ);
				#if RADIO_CMD_ECHO_TYPE
					Radio_Trf_Printf("Coef:%hd,%hd,%hd", TCFG_SystemData.MagCoefX, TCFG_SystemData.MagCoefY, TCFG_SystemData.MagCoefZ);
				#endif
				}
			#endif
				/* BeepOff */
			#if RADIO_DOWNLOAD_CMD_BEEPOFF
				else if (strstr(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "beepoff")) {
					sscanf(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "beepoff:%hu", &uval16);
					TCFG_SystemData.BeepCtrlOff = uval16;
					TCFG_EEPROM_SetBeepOff(TCFG_SystemData.BeepCtrlOff);
				#if RADIO_CMD_ECHO_TYPE
					Radio_Trf_Printf("Beepoff:%hu", TCFG_EEPROM_GetBeepOff());
				#endif
				}
			#endif
				/* Rollinit */
			#if RADIO_DOWNLOAD_CMD_ROLLINIT
				else if (strstr(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "rollinit")) {
					sscanf(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "rollinit:%hu", &uval16);
					TCFG_SystemData.RollingOverInitSensor = uval16;
					TCFG_EEPROM_SetRollingOverInitSensor(TCFG_SystemData.RollingOverInitSensor);
				#if RADIO_CMD_ECHO_TYPE
					Radio_Trf_Printf("Rollinit:%hu", TCFG_EEPROM_GetBeepOff());
				#endif
				}
			#endif
				/* UpLimit */
			#if RADIO_DOWNLOAD_CMD_UPLIMIT
				else if (strstr(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "uplimit")) {
					sscanf(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "uplimit:%hd,%hd", &limitRssi, &limitSnr);
					TCFG_SystemData.UpgradeLimitRssi = limitRssi;
					TCFG_SystemData.UpgradeLimitSnr = limitSnr;
					TCFG_EEPROM_SetUpgradeLimitRssi(TCFG_SystemData.UpgradeLimitRssi);
					TCFG_EEPROM_SetUpgradeLimitSnr(TCFG_SystemData.UpgradeLimitSnr);
				#if RADIO_CMD_ECHO_TYPE
					Radio_Trf_Printf("Uplimit:%hd,%hd", TCFG_EEPROM_GetUpgradeLimitRssi(), TCFG_EEPROM_GetUpgradeLimitSnr());
				#endif
				}
			#endif
				/* NBLimit */
			#if RADIO_DOWNLOAD_CMD_NBLIMIT
				else if (strstr(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "nblimit")) {
					sscanf(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "nblimit:%hd", &uval16);
					TCFG_EEPROM_SetNBIotSentCountLimit(uval16);
				#if RADIO_CMD_ECHO_TYPE
					Radio_Trf_Printf("NBlimit:%hd", TCFG_EEPROM_GetNBIotSentCountLimit());
				#endif
				}
			#endif
				/* CoverGain */
			#if RADIO_DOWNLOAD_CMD_COVERGAIN
				else if (strstr(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "covergain")) {
					sscanf(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "covergain:%hu", &uval16);
					if ((uval16 < RADAR_COVERGAIN_LOW) || (uval16 > RADAR_COVERGAIN_HIGH)) {
						uval16 = RADAR_COVERGAIN_DEFAULT;
					}
					if (TCFG_SystemData.CoverGain != uval16) {
						Radar_UpdateBG_Cmd(TCFG_SystemData.CoverGain, uval16);
						TCFG_SystemData.CoverGain = uval16;
						TCFG_EEPROM_SetCoverGain(TCFG_SystemData.CoverGain);
					}
				#if RADIO_CMD_ECHO_TYPE
					Radio_Trf_Printf("CoverGain:%hd", TCFG_EEPROM_GetCoverGain());
				#endif
				}
			#endif
				/* RadarGain */
			#if RADIO_DOWNLOAD_CMD_RADARGAIN
				else if (strstr(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "radargain")) {
					sscanf(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "radargain:%hu", &uval16);
					if ((uval16 < TRADAR_GAIN_LOWEST) || (uval16 > TRADAR_GAIN_HIGHEST)) {
						uval16 = TRADAR_GAIN_DEFAULT;
					}
					if (TCFG_SystemData.RadarGain != uval16) {
						TCFG_SystemData.RadarGain = uval16;
						TCFG_EEPROM_SetRadarGain(TCFG_SystemData.RadarGain);
					}
				#if RADIO_CMD_ECHO_TYPE
					Radio_Trf_Printf("RadarGain:%hd", TCFG_EEPROM_GetRadarGain());
				#endif
				}
			#endif
				/* SensorMode */
			#if RADIO_DOWNLOAD_CMD_SENSORMODE
				else if (strstr(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "sensormode")) {
					sscanf(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "sensormode:%hu", &uval16);
					if (TCFG_SystemData.SenseMode != uval16) {
						TCFG_SystemData.SenseMode = uval16;
						TCFG_EEPROM_SetSenseMode(TCFG_SystemData.SenseMode);
					}
				#if RADIO_CMD_ECHO_TYPE
					Radio_Trf_Printf("SensorMode:%hd", TCFG_EEPROM_GetSenseMode());
				#endif
				}
			#endif
				/* radar sample interval */
			#if RADIO_DOWNLOAD_CMD_INTERVAL
				else if (strstr(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "interval")) {
					sscanf(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "interval:%hu", &uval16);
					if (TCFG_SystemData.RadarSampleInterval != uval16) {
						TCFG_SystemData.RadarSampleInterval = uval16;
						Radar_Set_SampleInterval(TCFG_SystemData.RadarSampleInterval);
						TCFG_EEPROM_SetSampleInterval(TCFG_SystemData.RadarSampleInterval);
					}
				#if RADIO_CMD_ECHO_TYPE
					Radio_Trf_Printf("RadarSampleInterval:%hd", TCFG_EEPROM_GetSampleInterval());
				#endif
				}
			#endif
				/* cut-off frequency of high pass */
			#if RADIO_DOWNLOAD_CMD_HIGHPASS
				else if (strstr(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "highpass")) {
					sscanf(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "highpass:%hu", &uval16);
					if (TCFG_SystemData.RadarHighPass != uval16) {
						TCFG_SystemData.RadarHighPass = uval16;
						TCFG_EEPROM_SetHighPass(TCFG_SystemData.RadarHighPass);
					}
				#if RADIO_CMD_ECHO_TYPE
					Radio_Trf_Printf("RadarHighPass:%hd", TCFG_EEPROM_GetHighPass());
				#endif
				}
			#endif
				/* Restore */
			#if RADIO_DOWNLOAD_CMD_RESTORE
				else if (strstr(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "restore")) {
					TCFG_EEPROM_WriteConfigData();
					BEEP_CtrlRepeat_Extend(5, 30, 70);
				#if RADIO_CMD_ECHO_TYPE
					Radio_Trf_Printf("Restore Config Data");
				#endif
				}
			#endif
				/* WorkInfo */
			#if RADIO_CMD_UPLOAD_WORKINFO
				else if (strstr(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "workinfo")) {
				#if NETPROTOCAL == NETCOAP
					#if NBCOAP_SENDCODE_WORK_INFO
					NETCoapNeedSendCode.WorkInfo = 1;
					#endif
				#elif NETPROTOCAL == NETMQTTSN
					#if NBMQTTSN_SENDCODE_WORK_INFO
					NETMqttSNNeedSendCode.InfoWork = 1;
					#endif
				#elif NETPROTOCAL == NETONENET
					#if NBONENET_SENDCODE_WORK_INFO
					NETOneNETNeedSendCode.WorkInfo = 1;
					#endif
				#endif
				#if RADIO_PRINT_WORKINFO
					RadioPrintWorkinfo();
				#endif
				}
			#endif
				/* NetInfo */
			#if RADIO_CMD_UPLOAD_NETINFO
				else if (strstr(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "netinfo")) {
				#if NETPROTOCAL == NETCOAP
					#if NBCOAP_SENDCODE_BASIC_INFO
					NETCoapNeedSendCode.BasicInfo = 1;
					#endif
					#if NBCOAP_SENDCODE_DYNAMIC_INFO
					NETCoapNeedSendCode.DynamicInfo = 1;
					#endif
				#elif NETPROTOCAL == NETMQTTSN
					#if NBMQTTSN_SENDCODE_BASIC_INFO
					NETMqttSNNeedSendCode.InfoBasic = 1;
					#endif
					#if NBMQTTSN_SENDCODE_DYNAMIC_INFO
					NETMqttSNNeedSendCode.InfoDynamic = 1;
					#endif
				#elif NETPROTOCAL == NETONENET
					#if NBONENET_SENDCODE_BASIC_INFO
					NETOneNETNeedSendCode.BasicInfo = 1;
					#endif
					#if NBONENET_SENDCODE_DYNAMIC_INFO
					NETOneNETNeedSendCode.DynamicInfo = 1;
					#endif
				#endif
				#if RADIO_PRINT_NETINFO
					RadioPrintNetinfo();
				#endif
				}
			#endif
				/* DeviceInfo */
			#if RADIO_CMD_UPLOAD_DEVINFO
				else if (strstr(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "devinfo")) {
				#if RADIO_PRINT_DEVINFO
					RadioPrintDeviceinfo();
				#endif
				}
			#endif
				/* UpgradeInfo */
			#if RADIO_CMD_UPLOAD_UPGRADEINFO
				else if (strstr(((tmote_general_cmd_s*)CFG_P_FRAME_PAYLOAD(inmsg))->buf, "upgradeinfo")) {
				#if RADIO_PRINT_UPGRADEINFO
					RadioPrintUpgradeinfo();
				#endif
				}
			#endif
				/* ...... */
			}
		}
		else {
			if (pPayload->head.destSN != mac_sn) {
				rc = TRF_NOT_FORME;
				return rc;
			}
			
			if (CFG_GET_FROM_FRAME(CFG_P_FRAME_HEAD(inmsg), CFG_HEAD_TYPE_OS) == TMOTE_PLAIN_GET)
			{
				BEEP_CtrlRepeat_Extend(2, 50, 50);
				if (pPayload->head.type == TRF_MSG_BASICINFO) {
					__NOP();
				}
				else if (pPayload->head.type == TRF_MSG_SENSITIVITY) {
					__NOP();
				}
				else if (pPayload->head.type == TRF_MSG_WORKMODE) {
					__NOP();
				}
				else if (pPayload->head.type == TRF_MSG_INITBACKGROUND) {
					__NOP();
				}
			}
			else if (CFG_GET_FROM_FRAME(CFG_P_FRAME_HEAD(inmsg), CFG_HEAD_TYPE_OS) == TMOTE_PLAIN_ACK)
			{
				gateway_nearby = 15;
				rc = TRF_IS_ACK;
				__NOP();
			}
			else {
				rc = TRF_BAD_MSGTYPE;
			}
		}
	}
	else {
		rc = TRF_BAD_PROTOCAL;
	}
	
	if (rc == TRF_SUCCESS) {
		Radio_Trf_Default_Resp(100, pPayload->head.type);
	}
	
	return rc;
}

/**********************************************************************************************************
 @Function			void Radio_Trf_App_Task(void)
 @Description			Radio_Trf_App_Task
 @Input				void
 @Return				void
**********************************************************************************************************/
void Radio_Trf_App_Task(void)
{
	static uint32_t hearttime_pre = 0;										//心跳包计时器
	uint8_t len = 0;
	
	if (TRF_OK != Radio_Rf_get_Status()) {
		return;
	}
	
	/* 到达心跳时间 */
	if ((hearttime_pre + TCFG_SystemData.Heartinterval) < Stm32_GetSecondTick()) {
		hearttime_pre = Stm32_GetSecondTick();
		/* 发送心跳包 */
		Radio_Trf_Do_Heartbeat();
	}
	/* 接收到下行指令持续180秒连续间隔3秒发送心跳包 */
	else if ((cmdtime_pre + 180 > Stm32_GetSecondTick()) && ((hearttime_pre + RADIO_GATCMD_NEARBY_HEART_SEC) < Stm32_GetSecondTick())) {
		hearttime_pre = Stm32_GetSecondTick();
		/* 发送心跳包 */
		Radio_Trf_Do_Heartbeat();
		Radio_Trf_Printf_SensorBackground();
	}
	/* 接收到配置器Ping包连续间隔5秒发送心跳包 */
	else if ((gateway_nearby > 0) && ((hearttime_pre + RADIO_GATWAY_NEARBY_HEART_SEC) < Stm32_GetSecondTick())) {
		hearttime_pre = Stm32_GetSecondTick();
		/* 发送心跳包 */
		Radio_Trf_Do_Heartbeat();
		Radio_Trf_Printf_SensorBackground();
	}
	
	/* 接收无线下行数据 */
	if (TRF_SUCCESS == Radio_Rf_Receive(TRF_RecvBuf, &len)) {
		if (TRF_SUCCESS == Radio_Rf_Operate_Recvmsg(TRF_RecvBuf, len)) {
			cmdtime_pre = Stm32_GetSecondTick();
			TCFG_EEPROM_SetRFCmdCnt(1 + TCFG_EEPROM_GetRFCmdCnt());
		}
	}
}

/**********************************************************************************************************
 @Function			void Radio_Trf_Cfg_Buildframe(uint8_t *inmsg, uint8_t pkttype, uint8_t pktnum, uint32_t sn, uint8_t *outsend, uint8_t len)
 @Description			Radio_Trf_Cfg_Buildframe
 @Input				...
 @Return				void
**********************************************************************************************************/
void Radio_Trf_Cfg_Buildframe(uint8_t *inmsg, uint8_t pkttype, uint8_t pktnum, uint32_t sn, uint8_t *outsend, uint8_t len)
{
	uint8_t i;
	uint16_t check_sum = 0;
	
	CFG_PUT_INTO_FRAME(CFG_P_FRAME_LEN(outsend), CFG_LEN_OS, CFG_FRAME_PAYLOAD_OFS + CFG_FRAME_CRC_SIZE + len - CFG_FRAME_LEN_SIZE);
	CFG_PUT_INTO_FRAME(CFG_P_FRAME_PRE(outsend), CFG_PRE_TYPE_OS, XMESHCFG_PRT);
	CFG_PUT_INTO_FRAME(CFG_P_FRAME_PRE(outsend), CFG_PRE_VS_OS, CONFIG_VERSION);
	CFG_PUT_INTO_FRAME(CFG_P_FRAME_HEAD(outsend), CFG_HEAD_TYPE_OS, pkttype);
	
	if (pktnum) {
		CFG_PUT_INTO_FRAME(CFG_P_FRAME_HEAD(outsend), CFG_PKTNUM_OS, pktnum);
	}
	
	if (len) {
		memcpy(CFG_P_FRAME_PAYLOAD(outsend), inmsg, len);
	}
	
	memcpy(CFG_P_FRAME_SN(outsend), &sn, 4);
	
	for (i = 1; i < CFG_FRAME_PAYLOAD_OFS + len; i++) {
		check_sum += outsend[i];
	}
	
	outsend[CFG_FRAME_PAYLOAD_OFS + len] = check_sum / 0x100;
	outsend[CFG_FRAME_PAYLOAD_OFS + len + 1] = check_sum % 0x100;
	
	return;
}

/**********************************************************************************************************
 @Function			uint8_t Radio_Trf_Xmit_Get_Pktnum(void)
 @Description			Radio_Trf_Xmit_Get_Pktnum
 @Input				void
 @Return				pktnum
**********************************************************************************************************/
uint8_t Radio_Trf_Xmit_Get_Pktnum(void)
{
	static uint8_t pktnum = 0;
	return pktnum++;
}

/**********************************************************************************************************
 @Function			void Radio_Trf_Default_Resp(uint8_t ret, uint8_t type)
 @Description			Radio_Trf_Default_Resp
 @Input				void
 @Return				void
**********************************************************************************************************/
void Radio_Trf_Default_Resp(uint8_t ret, uint8_t type)
{
	trf_defaultrsp_s *pDefaultRsp = (trf_defaultrsp_s*)(TRF_SendBuf + 32);
	pDefaultRsp->head.destSN		= 0xFFFFFFFF;
	pDefaultRsp->head.version	= TRF_MSG_VERSION;
	pDefaultRsp->head.type		= type;
	pDefaultRsp->ret			= ret;
	
	Radio_Trf_Cfg_Buildframe((uint8_t *)pDefaultRsp, TMOTE_PLAIN_RSP, Radio_Trf_Xmit_Get_Pktnum(), TCFG_EEPROM_Get_MAC_SN(), TRF_SendBuf, sizeof(trf_defaultrsp_s));
	Radio_Rf_Send(TRF_SendBuf, TRF_SendBuf[0]);
}

/**********************************************************************************************************
 @Function			uint8_t Radio_Trf_Get_Workmode(void)
 @Description			Radio_Trf_Get_Workmode
 @Input				void
 @Return				workmode
**********************************************************************************************************/
uint8_t Radio_Trf_Get_Workmode(void)
{
	if (DeviceIdleMode == true) {
		return IDLE_WORK;
	}
	else if (DeviceActivedMode != true) {
		return NOTACTIVE_WORK;
	}
	else if (gateway_nearby == 0) {
		return NORMAL_WORK;
	}
	else {
		return TCFG_SystemData.WorkMode;
	}
}

/**********************************************************************************************************
 @Function			void Radio_Trf_Set_Workmode(uint8_t val)
 @Description			Radio_Trf_Set_Workmode
 @Input				workmode
 @Return				void
**********************************************************************************************************/
void Radio_Trf_Set_Workmode(uint8_t val)
{
	TCFG_SystemData.WorkMode = val;
}

/**********************************************************************************************************
 @Function			void Radio_Trf_Xmit_Heartbeat(void)
 @Description			Radio_Trf_Xmit_Heartbeat
 @Input				void
 @Return				void
**********************************************************************************************************/
void Radio_Trf_Xmit_Heartbeat(void)
{
	trf_heartbeat_s *pHeartBeat = (trf_heartbeat_s*)(TRF_SendBuf + 32);
	
	if (TRF_OK != Radio_Rf_get_Status()) {
		return;
	}
	
	if (gateway_nearby) {
		gateway_nearby--;
	}
	
	memset((void*)pHeartBeat, 0, sizeof(trf_heartbeat_s));
	pHeartBeat->head.destSN		= 0xFFFFFFFF;
	pHeartBeat->head.version		= TRF_MSG_VERSION;
	pHeartBeat->head.type		= TRF_MSG_HEART;
	pHeartBeat->major_softver	= TCFG_Utility_Get_Major_Softnumber();
	pHeartBeat->sub_softver		= TCFG_Utility_Get_Sub_Softnumber();
	pHeartBeat->DevType			= MVB_MODEL_TYPE;
	pHeartBeat->sub_hardver		= TCFG_Utility_Get_Major_Hardnumber();
	if (DeviceActivedMode == true) {
		pHeartBeat->workmode	= TCFG_SystemData.WorkMode;
	}
	else {
		pHeartBeat->workmode	= NOTACTIVE_WORK;
	}
	/* 0=free, 1=occupy */
	pHeartBeat->status			= talgo_get_spotstatus();
	/* 0=sending,1=sent */
	pHeartBeat->nbstate			= TCFG_Utility_Get_Nbiot_NetStateIdentification();
	
	Radio_Trf_Cfg_Buildframe((uint8_t *)pHeartBeat, TMOTE_PLAIN_PUB, Radio_Trf_Xmit_Get_Pktnum(), TCFG_EEPROM_Get_MAC_SN(), TRF_SendBuf, sizeof(trf_heartbeat_s));
	Radio_Rf_Send(TRF_SendBuf, TRF_SendBuf[0]);
	Delay_MS(7);
}

/**********************************************************************************************************
 @Function			void Radio_Trf_Do_Heartbeat(void)
 @Description			Radio_Trf_Do_Heartbeat
 @Input				void
 @Return				void
**********************************************************************************************************/
void Radio_Trf_Do_Heartbeat(void)
{
#ifdef	RADIO_SI4438
	Radio_Rf_Interface_Init();
	Radio_Rf_Interrupt_Init();
	
	Radio_Trf_Xmit_Heartbeat();
#endif
}

/**********************************************************************************************************
 @Function			void Radio_Trf_Do_Rf_Pintf(char* info)
 @Description			Radio_Trf_Do_Rf_Pintf
 @Input				info
 @Return				void
**********************************************************************************************************/
void Radio_Trf_Do_Rf_Pintf(char* info)
{
	uint8_t infolen;
	trf_msg_s *pMsg = (trf_msg_s*)(TRF_SendBuf + 32);
	
	infolen = strlen(info);
	if (infolen > 44) {
		infolen = 44;
	}
	
	Radio_Rf_Interface_Init();
	Radio_Rf_Interrupt_Init();
	
	if (TRF_OK != Radio_Rf_get_Status()) {
		return;
	}
	
	pMsg->head.destSN			= 0xFFFFFFFF;
	pMsg->head.version			= TRF_MSG_VERSION;
	pMsg->head.type			= TRF_MSG_DEBUG_INFO;
	memcpy(pMsg->pData, info, infolen);
	
	Radio_Trf_Cfg_Buildframe((uint8_t *)pMsg, TMOTE_PLAIN_PUB, Radio_Trf_Xmit_Get_Pktnum(), TCFG_EEPROM_Get_MAC_SN(), TRF_SendBuf, sizeof(trf_msghead_s) + infolen);
	Radio_Rf_Send(TRF_SendBuf, TRF_SendBuf[0]);
	if (TRF_SendBuf[0] >=  RADIO_TX_ALMOST_EMPTY_THRESHOLD) {
		Radio_StartTx_dummy(RF_CHANNEL1);
	}
}

/**********************************************************************************************************
 @Function			void Radio_Trf_Debug_Printf_Level3(const char *fmt, ...)
 @Description			Radio_Trf_Debug_Printf_Level3	: Radio Debug Printf
 @Input				...
 @Return				void
**********************************************************************************************************/
void Radio_Trf_Debug_Printf_Level3(const char *fmt, ...)
{
	if ((DEBUG_WORK == Radio_Trf_Get_Workmode()) && (TCFG_EEPROM_GetRFDprintLv() >= 3)) {
#ifdef	RADIO_SI4438
		__va_list args;
		va_start (args, fmt);
		
		memset(TRF_PrintfBuf, 0, sizeof(TRF_PrintfBuf));
		vsprintf((char*)TRF_PrintfBuf, fmt, args);
		va_end (args);
		Radio_Trf_Do_Rf_Pintf((char*)TRF_PrintfBuf);
		Delay_US(300);
#endif
	}
}

/**********************************************************************************************************
 @Function			void Radio_Trf_Debug_Printf_Level2(const char *fmt, ...)
 @Description			Radio_Trf_Debug_Printf_Level2	: Radio Debug Printf
 @Input				...
 @Return				void
**********************************************************************************************************/
void Radio_Trf_Debug_Printf_Level2(const char *fmt, ...)
{
	if ((DEBUG_WORK == Radio_Trf_Get_Workmode()) && (TCFG_EEPROM_GetRFDprintLv() >= 2)) {
#ifdef	RADIO_SI4438
		__va_list args;
		va_start (args, fmt);
		
		memset(TRF_PrintfBuf, 0, sizeof(TRF_PrintfBuf));
		vsprintf((char*)TRF_PrintfBuf, fmt, args);
		va_end (args);
		Radio_Trf_Do_Rf_Pintf((char*)TRF_PrintfBuf);
		Delay_US(300);
#endif
	}
}

/**********************************************************************************************************
 @Function			void Radio_Trf_Debug_Printf_Level1(const char *fmt, ...)
 @Description			Radio_Trf_Debug_Printf_Level1	: Radio Debug Printf
 @Input				...
 @Return				void
**********************************************************************************************************/
void Radio_Trf_Debug_Printf_Level1(const char *fmt, ...)
{
	if ((DEBUG_WORK == Radio_Trf_Get_Workmode()) && (TCFG_EEPROM_GetRFDprintLv() >= 1)) {
#ifdef	RADIO_SI4438
		__va_list args;
		va_start (args, fmt);
		
		memset(TRF_PrintfBuf, 0, sizeof(TRF_PrintfBuf));
		vsprintf((char*)TRF_PrintfBuf, fmt, args);
		va_end (args);
		Radio_Trf_Do_Rf_Pintf((char*)TRF_PrintfBuf);
		Delay_US(300);
#endif
	}
}

/**********************************************************************************************************
 @Function			void Radio_Trf_Debug_Printf_Level0(const char *fmt, ...)
 @Description			Radio_Trf_Debug_Printf_Level0	: Radio Debug Printf
 @Input				...
 @Return				void
**********************************************************************************************************/
void Radio_Trf_Debug_Printf_Level0(const char *fmt, ...)
{
	if (DEBUG_WORK == Radio_Trf_Get_Workmode()) {
#ifdef	RADIO_SI4438
		__va_list args;
		va_start (args, fmt);
		
		memset(TRF_PrintfBuf, 0, sizeof(TRF_PrintfBuf));
		vsprintf((char*)TRF_PrintfBuf, fmt, args);
		va_end (args);
		Radio_Trf_Do_Rf_Pintf((char*)TRF_PrintfBuf);
		Delay_US(300);
#endif
	}
}

/**********************************************************************************************************
 @Function			void Radio_Trf_Printf(const char *fmt, ...)
 @Description			Radio_Trf_Printf			: Radio Printf
 @Input				...
 @Return				void
**********************************************************************************************************/
void Radio_Trf_Printf(const char *fmt, ...)
{
#ifdef	RADIO_SI4438
	__va_list args;
	va_start (args, fmt);
	
	memset(TRF_PrintfBuf, 0, sizeof(TRF_PrintfBuf));
	vsprintf ((char*)TRF_PrintfBuf, fmt, args);
	va_end (args);
	Radio_Trf_Do_Rf_Pintf((char*)TRF_PrintfBuf);
	Delay_US(300);
#endif
}

/**********************************************************************************************************
 @Function			void Radio_Trf_Printf_SensorBackground(void)
 @Description			Radio_Trf_Printf_SensorBackground	: 打印传感器背景值
 @Input				void
 @Return				void
**********************************************************************************************************/
void Radio_Trf_Printf_SensorBackground(void)
{
#ifdef	RADIO_SI4438
	Radio_Trf_Printf("bbbbbxyz=%d,%d,%d,%d,%d,%d,%d,%d,", \
	radar_targetinfo.pMagBG[2], radar_targetinfo.pMagBG[3], radar_targetinfo.pMagBG[4], radar_targetinfo.pMagBG[5], radar_targetinfo.pMagBG[6], \
	Qmc5883lData.X_Now, Qmc5883lData.Y_Now, Qmc5883lData.Z_Now);
#endif
}

/********************************************** END OF FLEE **********************************************/
