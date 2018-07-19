#ifndef __PCP_FUNC_H
#define   __PCP_FUNC_H

#include "sys.h"
#include "pcpconfig.h"

PCP_StatusTypeDef PCP_Func_FrameCheck(unsigned char* DataFrame, short DataLength);				//PCP数据帧检查
PCP_StatusTypeDef PCP_Func_SelectMessageExecuteCmd(PCP_ClientsTypeDef* pClient);				//PCP判断不同消息码处理不同命令

PCP_StatusTypeDef PCP_Func_AckQueryDeviceVersion(PCP_ClientsTypeDef* pClient);					//PCP查询设备版本应答







#endif /* __PCP_FUNC_H */
