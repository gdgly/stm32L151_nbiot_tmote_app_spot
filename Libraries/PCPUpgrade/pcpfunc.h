#ifndef __PCP_FUNC_H
#define   __PCP_FUNC_H

#include "sys.h"
#include "pcpconfig.h"

/* Ack Query Device Version */
typedef __packed struct
{
	unsigned char						ResultCode;									//结果码
	unsigned char						DeviceSoftVersion[16];							//当前版本号
}PCP_AckQueryDeviceVersionTypeDef;

/* Rck New Version Notice */
typedef __packed struct
{
	unsigned char						PlatformSoftVersion[16];							//目的版本号
	unsigned short						UpgradePackSliceSize;							//升级包分片大小
	unsigned short						UpgradePackSliceNum;							//升级包分片总数
	unsigned short						UpgradePackCheckCode;							//升级包校验码
}PCP_RckNewVersionNoticeTypeDef;



PCP_StatusTypeDef PCP_Func_FrameCheck(unsigned char* DataFrame, short DataLength);				//PCP数据帧检查

PCP_StatusTypeDef PCP_Func_SelectMessageExecuteCmd(PCP_ClientsTypeDef* pClient);				//PCP判断不同消息码处理不同命令
PCP_StatusTypeDef PCP_Func_AckQueryDeviceVersion(PCP_ClientsTypeDef* pClient);					//PCP查询设备版本应答
PCP_StatusTypeDef PCP_Func_AckNewVersionNotice(PCP_ClientsTypeDef* pClient);					//PCP新版本通知应答

PCP_StatusTypeDef PCP_Func_SelectUpgradeStatusExecuteCmd(PCP_ClientsTypeDef* pClient);			//PCP判断不同主动上传数据码处理不同命令






#endif /* __PCP_FUNC_H */
