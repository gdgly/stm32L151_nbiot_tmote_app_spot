/**
  *********************************************************************************************************
  * @file    ctwingtransport.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2019-10-14
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "ctwingtransport.h"
#include "platform_config.h"
#include "platform_map.h"
#include "stm32l1xx_config.h"
#include "nbiotfunc.h"
#include "string.h"

/**********************************************************************************************************
 @Function			CTWING_StatusTypeDef CTWING_Transport_Init(CTWING_LWM2MTransportTypeDef* NetLWM2M, NBIOT_ClientsTypeDef* NBIotStack)
 @Description			CTWING_StatusTypeDef	: Initiative初始化CTWING数据传输接口
 @Input				NetLWM2M				: LWM2M Struct*
					NBIotStack			: NBIot协议栈
 @Return				CTWING_StatusTypeDef	: CTWING处理状态
**********************************************************************************************************/
CTWING_StatusTypeDef CTWING_Transport_Init(CTWING_LWM2MTransportTypeDef* NetLWM2M, NBIOT_ClientsTypeDef* NBIotStack)
{
	CTWING_StatusTypeDef CTWingStatus = CTWING_OK;
	
	NetLWM2M->NBIotStack = NBIotStack;
	
	return CTWingStatus;
}

/********************************************** END OF FLEE **********************************************/
