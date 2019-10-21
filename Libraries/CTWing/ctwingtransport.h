#ifndef __CTWING_TRANSPORT_H
#define   __CTWING_TRANSPORT_H

#include "sys.h"
#include "ctwingconfig.h"

CTWING_StatusTypeDef CTWING_Transport_Init(CTWING_LWM2MTransportTypeDef* NetLWM2M, NBIOT_ClientsTypeDef* NBIotStack);												//初始化数据传输接口

#endif /* __CTWING_TRANSPORT_H */
