#ifndef __ONENET_FUNC_H
#define   __ONENET_FUNC_H

#include "sys.h"
#include "onenetconfig.h"

ONENET_StatusTypeDef NBIOT_OneNET_Related_AccessConfig_LwM2MServer(ONENET_ClientsTypeDef* pClient, u8 mode, sc8* host, u16 port);	//设置LwM2M服务器
ONENET_StatusTypeDef NBIOT_OneNET_Related_AccessConfig_AckTimeout(ONENET_ClientsTypeDef* pClient, u8 rspTimeout);				//设置应答超时时间
ONENET_StatusTypeDef NBIOT_OneNET_Related_AccessConfig_ObserveAutoack(ONENET_ClientsTypeDef* pClient, u8 obsAutoack);			//设置模块自动应答
ONENET_StatusTypeDef NBIOT_OneNET_Related_Create_SuiteInstance(ONENET_ClientsTypeDef* pClient, s32* refer);					//创建通信套件实例






#endif /* __ONENET_FUNC_H */
