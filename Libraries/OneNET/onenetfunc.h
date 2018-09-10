#ifndef __ONENET_FUNC_H
#define   __ONENET_FUNC_H

#include "sys.h"
#include "onenetconfig.h"

ONENET_StatusTypeDef NBIOT_OneNET_Related_AccessConfig_LwM2MServer(ONENET_ClientsTypeDef* pClient, u8 mode, sc8* host, u16 port);	//设置LwM2M服务器
ONENET_StatusTypeDef NBIOT_OneNET_Related_AccessConfig_AckTimeout(ONENET_ClientsTypeDef* pClient, u8 rspTimeout);				//设置应答超时时间








#endif /* __ONENET_FUNC_H */
