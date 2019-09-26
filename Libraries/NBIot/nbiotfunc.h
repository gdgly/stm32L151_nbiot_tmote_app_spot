#ifndef __NBIOT_FUNC_H
#define   __NBIOT_FUNC_H

#include "sys.h"
#include "nbiotconfig.h"

#if NBIOT_ATCMD_SET_HARDWAREPOWEROFF
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_HardwarePoweroff(NBIOT_ClientsTypeDef* pClient);																			//硬件断电
#endif
#if NBIOT_ATCMD_SET_HARDWAREREBOOT
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_HardwareReboot(NBIOT_ClientsTypeDef* pClient, u32 rebootTimeoutMS);															//硬件重启NB模块
#endif
#if NBIOT_ATCMD_SET_SOFTWAREREBOOT
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SoftwareReboot(NBIOT_ClientsTypeDef* pClient, u32 rebootTimeoutMS);															//软件重启NB模块
#endif
#if NBIOT_ATCMD_SET_CLEARSTOREDEARFCN
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_ClearStoredEarfcn(NBIOT_ClientsTypeDef* pClient);																		//清除小区频点
#endif
#if NBIOT_ATCMD_GET_MANUFACTURER
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadManufacturer(NBIOT_ClientsTypeDef* pClient);																		//检出制造商标识
#endif
#if NBIOT_ATCMD_GET_MANUFACTURERMODEL
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadManufacturerModel(NBIOT_ClientsTypeDef* pClient);																	//检出制造商型号
#endif
#if NBIOT_ATCMD_GET_MODULEVERSION
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadModuleVersion(NBIOT_ClientsTypeDef* pClient);																	//检出模组软件版本号
#endif
#if NBIOT_ATCMD_GET_IMEI
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadIMEI(NBIOT_ClientsTypeDef* pClient);																			//检出IMEI号
#endif
#if NBIOT_ATCMD_GET_IMEISV
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadIMEISV(NBIOT_ClientsTypeDef* pClient);																			//检出IMEISV号
#endif
#if NBIOT_ATCMD_GET_RSSI
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadRSSI(NBIOT_ClientsTypeDef* pClient);																			//检出RSSI号
#endif
#if NBIOT_ATCMD_GET_STATISTICSRADIO
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadStatisticsRADIO(NBIOT_ClientsTypeDef* pClient);																	//检出基站连接参数RADIO
#endif
#if NBIOT_ATCMD_GET_STATISTICSCELL
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadStatisticsCELL(NBIOT_ClientsTypeDef* pClient);																	//检出基站连接参数CELL
#endif
#if NBIOT_ATCMD_GET_AREACODE
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadAreaCode(NBIOT_ClientsTypeDef* pClient);																		//检出基站区域码
#endif
#if NBIOT_ATCMD_GET_ICCID
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadICCID(NBIOT_ClientsTypeDef* pClient);																			//检出ICCID卡号
#endif
#if NBIOT_ATCMD_GET_IMSI
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadIMSI(NBIOT_ClientsTypeDef* pClient);																			//检出IMSI号
#endif
#if NBIOT_ATCMD_GET_CGPADDR
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadCGPADDR(NBIOT_ClientsTypeDef* pClient);																			//检出CGPADDR地址
#endif
#if NBIOT_ATCMD_GET_CGDCONT
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadCGDCONT(NBIOT_ClientsTypeDef* pClient);																			//检出CGDCONT网址
#endif
#if NBIOT_ATCMD_GET_PDPCONTEXT
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadPDPContext(NBIOT_ClientsTypeDef* pClient);																		//检出模组APN
#endif
#if NBIOT_ATCMD_GET_DATATIME
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadDateTime(NBIOT_ClientsTypeDef* pClient);																		//检出基站时间
#endif
#if NBIOT_ATCMD_GET_CHIPINFO
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadSystemInformation(NBIOT_ClientsTypeDef* pClient);																	//检出模组系统信息
#endif
#if NBIOT_ATCMD_GET_PSMSTATUS
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadPowerSavingModeStatus(NBIOT_ClientsTypeDef* pClient);																//查询终端PSM状态
#endif
#if NBIOT_ATCMD_GET_CSCON
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadSignalConnectionStatus(NBIOT_ClientsTypeDef* pClient);																//查询终端Connected状态
#endif
#if NBIOT_ATCMD_GET_NMSTATUS
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadMessageRegistrationStatus(NBIOT_ClientsTypeDef* pClient);															//查询终端MessageRegistration状态
#endif
#if NBIOT_ATCMD_SET_CGATT
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetAttachOrDetach(NBIOT_ClientsTypeDef* pClient, NBIOT_NetstateTypeDef attdet);												//设置终端入网退网
#endif
#if NBIOT_ATCMD_GET_CGATT
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadAttachOrDetach(NBIOT_ClientsTypeDef* pClient);																	//查询终端入网退网
#endif
#if NBIOT_ATCMD_SET_CFUN
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetMinOrFullFunc(NBIOT_ClientsTypeDef* pClient, NBIOT_FunctionalityTypeDef minfull);											//设置设备功能
#endif
#if NBIOT_ATCMD_GET_CFUN
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadMinOrFullFunc(NBIOT_ClientsTypeDef* pClient);																	//查询设备功能
#endif
#if NBIOT_ATCMD_SET_NNMI
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetNewMessageIndications(NBIOT_ClientsTypeDef* pClient, NBIOT_OpenOrCloseFuncTypeDef state);										//设置NNMI是否开启
#endif
#if NBIOT_ATCMD_GET_NNMI
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadNewMessageIndications(NBIOT_ClientsTypeDef* pClient);																//查询NNMI是否开启
#endif
#if NBIOT_ATCMD_SET_NSMI
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetSentMessageIndications(NBIOT_ClientsTypeDef* pClient, NBIOT_OpenOrCloseFuncTypeDef state);										//设置NSMI是否开启
#endif
#if NBIOT_ATCMD_GET_NSMI
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadSentMessageIndications(NBIOT_ClientsTypeDef* pClient);																//查询NSMI是否开启
#endif
#if NBIOT_ATCMD_SET_NBAND
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetSupportedBands(NBIOT_ClientsTypeDef* pClient, NBIOT_NBandTypeDef bands);													//设置Band
#endif
#if NBIOT_ATCMD_GET_NBAND
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadSupportedBands(NBIOT_ClientsTypeDef* pClient);																	//查询Band
#endif
#if NBIOT_ATCMD_TST_NBAND
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_TestSupportedBands(NBIOT_ClientsTypeDef* pClient, NBIOT_NBandTypeDef bands);													//测试Band
#endif
#if NBIOT_ATCMD_SET_CMEE
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetReportTerminationError(NBIOT_ClientsTypeDef* pClient, NBIOT_ReportErrorTypeDef enable);										//设置CMEE
#endif
#if NBIOT_ATCMD_GET_CMEE
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadReportTerminationError(NBIOT_ClientsTypeDef* pClient);																//查询CMEE
#endif
#if NBIOT_ATCMD_SET_NCDP
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetCDPServer(NBIOT_ClientsTypeDef* pClient, const char *host, unsigned short port);												//设置CDP服务器
#endif
#if NBIOT_ATCMD_GET_NCDP
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadCDPServer(NBIOT_ClientsTypeDef* pClient);																		//查询CDP服务器
#endif
#if NBIOT_ATCMD_SET_DNSSERVERADDRESS
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetDNSServerAddress(NBIOT_ClientsTypeDef* pClient, const char *pri_dns, const char *sec_dns);										//设置DNS服务器地址
#endif
#if NBIOT_ATCMD_GET_DNSSERVERADDRESS
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadDNSServerAddress(NBIOT_ClientsTypeDef* pClient);																	//查询DNS服务器地址
#endif
#if NBIOT_ATCMD_SET_LWM2MLIFETIME
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetLWM2MLifetime(NBIOT_ClientsTypeDef* pClient, unsigned int lifetime);														//设置LWM2MLifetime
#endif
#if NBIOT_ATCMD_GET_LWM2MLIFETIME
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadLWM2MLifetime(NBIOT_ClientsTypeDef* pClient);																	//查询LWM2MLifetime
#endif
#if NBIOT_ATCMD_SET_NCONFIG
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetConfigUE(NBIOT_ClientsTypeDef* pClient, const char *ncmd, NBIOT_NConfigTypeDef state);										//配置NCONFIG模式
#endif
#if NBIOT_ATCMD_GET_NCONFIG
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadConfigUE(NBIOT_ClientsTypeDef* pClient);																		//查询NCONFIG模式
#endif
#if NBIOT_ATCMD_GET_NQMGS
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_QuerySendMessageCOAPPayload(NBIOT_ClientsTypeDef* pClient);																//COAP查询消息送达
#endif
#if NBIOT_ATCMD_GET_NQMGR
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_QueryReadMessageCOAPPayload(NBIOT_ClientsTypeDef* pClient);																//COAP查询消息下行
#endif
#if NBIOT_ATCMD_SET_COAPPAYLOAD
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SendCOAPPayload(NBIOT_ClientsTypeDef* pClient);																			//COAP发送负载数据
#endif
#if NBIOT_ATCMD_GET_COAPPAYLOAD
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_ReadCOAPPayload(NBIOT_ClientsTypeDef* pClient);																			//COAP读取负载数据
#endif
#if NBIOT_ATCMD_GET_CONDATA
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadCONDataStatus(NBIOT_ClientsTypeDef* pClient);																	//查询终端CON数据发送状态
#endif
#if NBIOT_ATCMD_SET_CONDATA
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SendCOAPPayloadFlag(NBIOT_ClientsTypeDef* pClient, const char *flag);														//COAP发送负载数据(FLAG)
#endif
#if NBIOT_ATCMD_CRT_UDPSOCKET
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CreateUDPSocket(NBIOT_ClientsTypeDef* pClient, u16 localport, u8 receivectl, int *socket);										//创建UDP链接
#endif
#if NBIOT_ATCMD_CLS_UDPSOCKET
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CloseUDPSocket(NBIOT_ClientsTypeDef* pClient, int socket);																	//关闭UDP连接
#endif
#if NBIOT_ATCMD_SED_UDPPAYLOAD
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SendUDPPayload(NBIOT_ClientsTypeDef* pClient, int socket, const char *addr, u16 port);											//UDP发送负载数据
#endif
#if NBIOT_ATCMD_SED_UDPPAYLOADFLAG
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SendUDPPayloadFlag(NBIOT_ClientsTypeDef* pClient, int , const char *, u16 , const char *);										//UDP发送负载数据(FLAG)
#endif
#if NBIOT_ATCMD_RED_UDPPAYLOAD
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_ReadUDPPayload(NBIOT_ClientsTypeDef* pClient, int , int , int *, char *, u16 *);												//UDP读取负载数据
#endif

#endif /* __NBIOT_FUNC_H */
