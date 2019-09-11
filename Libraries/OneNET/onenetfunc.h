#ifndef __ONENET_FUNC_H
#define   __ONENET_FUNC_H

#include "sys.h"
#include "onenetconfig.h"

unsigned short NBIOT_OneNET_Related_GetNextMsgId(ONENET_ClientsTypeDef* pClient);																				//获取下一个消息ID

ONENET_StatusTypeDef NBIOT_OneNET_Related_AccessConfig_LwM2MServer(ONENET_ClientsTypeDef* pClient, u8 mode, sc8* host, u16 port);											//设置LwM2M服务器
ONENET_StatusTypeDef NBIOT_OneNET_Related_AccessConfig_AckTimeout(ONENET_ClientsTypeDef* pClient, u8 rspTimeout);														//设置应答超时时间
ONENET_StatusTypeDef NBIOT_OneNET_Related_AccessConfig_ObserveAutoack(ONENET_ClientsTypeDef* pClient, u8 obsAutoack);													//设置模块自动应答

ONENET_StatusTypeDef NBIOT_OneNET_Related_Create_SuiteInstance(ONENET_ClientsTypeDef* pClient, s32* refer);															//创建通信套件实例
ONENET_StatusTypeDef NBIOT_OneNET_Related_Delete_SuiteInstance(ONENET_ClientsTypeDef* pClient, s32 refer);															//删除通信套件实例
ONENET_StatusTypeDef NBIOT_OneNET_Related_CheckRead_SuiteVersion(ONENET_ClientsTypeDef* pClient);																	//检出通信套件版本

ONENET_StatusTypeDef NBIOT_OneNET_Related_Add_LwM2MObject(ONENET_ClientsTypeDef* pClient, s32 refer, u32 objId, u16 insCount, sc8* insBitmap, u16 attrCount, u16 actCount);		//创建LwM2M对象
ONENET_StatusTypeDef NBIOT_OneNET_Related_Del_LwM2MObject(ONENET_ClientsTypeDef* pClient, s32 refer, u32 objId);														//删除LwM2M对象

ONENET_StatusTypeDef NBIOT_OneNET_Related_Send_RegisterRequest(ONENET_ClientsTypeDef* pClient, s32 refer, u32 lifetime, u32 timeout);										//发送注册请求
ONENET_StatusTypeDef NBIOT_OneNET_Related_Send_DeregisterRequest(ONENET_ClientsTypeDef* pClient, s32 refer);															//发送注销请求

ONENET_StatusTypeDef NBIOT_OneNET_Related_Respond_DiscoverRequest(ONENET_ClientsTypeDef* pClient, s32 refer, u32 msgId, u8 result, u16 length, sc8* valuestring, sc8* raiMode);	//响应Discover请求
ONENET_StatusTypeDef NBIOT_OneNET_Related_Respond_ObserveRequest(ONENET_ClientsTypeDef* pClient, s32 refer, u32 msgId, u8 result, sc8* raiMode);								//响应Observe请求
ONENET_StatusTypeDef NBIOT_OneNET_Related_Respond_ReadRequest(ONENET_ClientsTypeDef* pClient, s32 refer, u32 msgId, u8 result, ONENET_MessageParaTypeDef* msg, sc8* raiMode);		//响应Read请求
ONENET_StatusTypeDef NBIOT_OneNET_Related_Respond_WriteRequest(ONENET_ClientsTypeDef* pClient, s32 refer, u32 msgId, u8 result, sc8* raiMode);								//响应Write请求
ONENET_StatusTypeDef NBIOT_OneNET_Related_Respond_ExecuteRequest(ONENET_ClientsTypeDef* pClient, s32 refer, u32 msgId, u8 result, sc8* raiMode);								//响应Execute请求
ONENET_StatusTypeDef NBIOT_OneNET_Related_Respond_WriteAttributesRequest(ONENET_ClientsTypeDef* pClient, s32 refer, u32 msgId, u8 result, sc8* raiMode);						//响应WriteAttributes请求

ONENET_StatusTypeDef NBIOT_OneNET_Related_Notify_ApplicationServer(ONENET_ClientsTypeDef* pClient, s32 refer, u32 msgId, ONENET_MessageParaTypeDef* msg, u16 ackid, sc8* raiMode);	//通知数据到平台

ONENET_StatusTypeDef NBIOT_OneNET_Related_Send_UpdateRequest(ONENET_ClientsTypeDef* pClient, s32 refer, u32 lifetime, u8 withObjectFlag, sc8* raiMode);						//发送更新生命周期

ONENET_StatusTypeDef ONENET_ReadPacket(ONENET_ClientsTypeDef* pClient, int *relatedType);																			//接收数据包
ONENET_StatusTypeDef ONENET_RecvPacketProcess(ONENET_ClientsTypeDef* pClient, int *relatedType);																	//接收数据包处理
ONENET_StatusTypeDef ONENET_WaitforRecvAck(ONENET_ClientsTypeDef* pClient, ONENET_RelatedURCsCodeTypeDef relatedType, Stm32_CalculagraphTypeDef* timerS);						//等待接收相应应答消息类型

#endif /* __ONENET_FUNC_H */
