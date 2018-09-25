/**
  *********************************************************************************************************
  * @file    mqttpcpupgrade.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-09-25
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "mqttpcpupgrade.h"
#include "platform_config.h"
#include "platform_map.h"
#include "stm32l1xx_config.h"
#include "delay.h"
#include "usart.h"
#include "hal_spiflash.h"
#include "hal_rtc.h"
#include "hal_beep.h"
#include "hal_iwdg.h"
#include "radio_rf_app.h"
#include "radio_hal_rf.h"








u8 MqttSNCalculateSumCheckCode(u32 regCode, uint8_t* buf, uint32_t length)
{
	u32 sumCheckCode = regCode;
	
	while (length--) {
		sumCheckCode += *buf++;
	}
	
	sumCheckCode %= 0xFF;
	
	return sumCheckCode;
}

u8 MqttSNCalculateStringToHex(u8 HighChar, u8 LowChar)
{
	u8 val = 0;
	
	if ((HighChar >= '0') && (HighChar <= '9')) {
		val |= ((HighChar - '0') << 4);
	}
	else if ((HighChar >= 'A') && (HighChar <= 'F')) {
		val |= ((HighChar - 'A' + 10) << 4);
	}
	else if ((HighChar >= 'a') && (HighChar <= 'f')) {
		val |= ((HighChar - 'a' + 10) << 4);
	}
	
	if ((LowChar >= '0') && (LowChar <= '9')) {
		val |= (LowChar - '0');
	}
	else if ((LowChar >= 'A') && (LowChar <= 'F')) {
		val |= (LowChar - 'A' + 10);
	}
	else if ((LowChar >= 'a') && (LowChar <= 'f')) {
		val |= (LowChar - 'a' + 10);
	}
	
	return val;
}



























/********************************************** END OF FLEE **********************************************/
