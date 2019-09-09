#ifndef __PLATFORM_CONFIG_H
#define   __PLATFORM_CONFIG_H

#include "sys.h"
#include "net_coap_app.h"
#include "net_pcp_app.h"
#include "net_dns_app.h"
#include "net_mqttsn_app.h"
#include "net_mqttsn_pcp_app.h"
#include "net_onenet_app.h"

//#define	MVB_SUBSN						0x81011000						//设备号
//#define	MVB_BRAND						"mvb"							//厂牌

#define	CPUVERSION					"STM32L151xBA"						//处理器型号

#define	MVB_STATICMAGNETIC_MODEL			51
#define	MVB_FLOWMAGNETIC_MODEL			52
#define	MVB_SPOT_LOCKER_MODEL			53
#define	MVB_BARRIER_MODEL				54
#define	MVB_MODEL_TYPE					MVB_STATICMAGNETIC_MODEL				//设备类型

#define	SYSTEMCLOCKMSI					4194000
#define	SYSTEMCLOCKHSI					32000000
#define	SYSTEMCLOCKHSE					64000000
#define	SYSTEMCLOCK					SYSTEMCLOCKMSI						//系统时钟

#define	SYSTEMRTCCLOCKLSI				37000
#define	SYSTEMRTCCLOCKLSE				32768
#define	SYSTEMRTCCLOCK					SYSTEMRTCCLOCKLSE					//RTC时钟

#define	LOWPOWERDISABLE				0
#define	LOWPOWERENABLE					1
#define	LOWPOWERMODE					LOWPOWERENABLE						//低功耗模式

#define	DEBUGLOWPOWERDISABLE			0
#define	DEBUGLOWPOWERENABLE				1
#define	DEBUGLOWPOWERMODE				DEBUGLOWPOWERDISABLE				//调试低功耗模式

#define	NETCOAP						0								//COAP
#define	NETMQTTSN						1								//MQTTSN
#define	NETONENET						2								//ONENET
#define	NETPROTOCAL					NETCOAP							//网络模式

#define	NETCARRIERCHINATELECOM			00								//中国电信
#define	NETCARRIERCHINAMOBILE			10								//中国移动
#define	NETCARRIERCHINAUNICOM			20								//中国联通
#define	NETCARRIEROTHERUNICOM			30								//其他网段
#define	NETCARRIERTYPE					NETCARRIERCHINATELECOM				//运营商

#define	NETFIFOMESSAGEDISABLE			0
#define	NETFIFOMESSAGEENABLE			1
#define	NETFIFOMESSAGETYPE				NETFIFOMESSAGEENABLE				//Net使能FifoMessage

#define	NETFIFO_COAPSENDPARKNUM_MAX		40								//NetCoAP发送缓存最大包数
#define	NETFIFO_COAPRECVPARKNUM_MAX		20								//NetCoAP接收缓存最大包数
#define	NETFIFO_COAPSENDPARKSIZE_MAX		3072								//NetCoAP发送缓存大小
#define	NETFIFO_COAPRECVPARKSIZE_MAX		2048								//NetCoAP接收缓存大小

#define	NETFIFO_COAPPCPSENDPARKNUM_MAX	20								//NetCoAPPCP发送缓存最大包数
#define	NETFIFO_COAPPCPRECVPARKNUM_MAX	20								//NetCoAPPCP接收缓存最大包数
#define	NETFIFO_COAPPCPSENDPARKSIZE_MAX	512								//NetCoAPPCP发送缓存大小
#define	NETFIFO_COAPPCPRECVPARKSIZE_MAX	2048								//NetCoAPPCP接收缓存大小

#define	NETFIFO_MQTTSNSENDPARKNUM_MAX		30								//NetMqttSN发送缓存最大包数
#define	NETFIFO_MQTTSNSENDPARKSIZE_MAX	2120								//NetMqttSN发送缓存大小

#define	NETFIFO_MQTTSNPCPSENDPARKNUM_MAX	20								//NetMqttSNPCP发送缓存最大包数
#define	NETFIFO_MQTTSNPCPRECVPARKNUM_MAX	20								//NetMqttSNPCP接收缓存最大包数
#define	NETFIFO_MQTTSNPCPSENDPARKSIZE_MAX	512								//NetMqttSNPCP发送缓存大小
#define	NETFIFO_MQTTSNPCPRECVPARKSIZE_MAX	2048								//NetMqttSNPCP接收缓存大小

#define	NETFIFO_ONENETSENDPARKNUM_MAX		30								//NetOneNET发送缓存最大包数
#define	NETFIFO_ONENETRECVPARKNUM_MAX		10								//NetOneNET接收缓存最大包数
#define	NETFIFO_ONENETSENDPARKSIZE_MAX	2048								//NetOneNET发送缓存大小
#define	NETFIFO_ONENETRECVPARKSIZE_MAX	1536								//NetOneNET接收缓存大小

#define	NBCOAP_SENDMODE_NORMAL			SEND_DATA
#define	NBCOAP_SENDMODE_RAIDLE			SEND_DATA_RA_NORMAL
#define	NBCOAP_SENDMODE_TYPE			NBCOAP_SENDMODE_RAIDLE				//COAP发送模式

#define	NBCOAP_SENDMODE_NORMAL_MODE		0
#define	NBCOAP_SENDMODE_RAIDLE_MODE		1
#define	NBCOAP_SENDMODE_CONFIG_TYPE		NBCOAP_SENDMODE_RAIDLE_MODE			//COAP发送模式

#define	NBCOAP_RASENDMODE_NORMAL			0
#define	NBCOAP_RASENDMODE_IDLE			1
#define	NBCOAP_RASENDMODE_TYPE			NBCOAP_RASENDMODE_IDLE				//COAP发送RA模式

#define	NBCOAP_SENDDATA_NQMGSCHECK_DISABLE	0
#define	NBCOAP_SENDDATA_NQMGSCHECK_ENABLE	1
#define	NBCOAP_SENDDATA_NQMGSCHECK_TYPE	NBCOAP_SENDDATA_NQMGSCHECK_DISABLE		//COAP发送数据NQMGS检查

#define	NBCOAP_SEND_BEFORE_ATTACH_DISABLE	0
#define	NBCOAP_SEND_BEFORE_ATTACH_ENABLE	1
#define	NBCOAP_SEND_BEFORE_ATTACH_TYPE	NBCOAP_SEND_BEFORE_ATTACH_DISABLE		//COAP发送数据之前检查注网

#define	NBCOAP_RATIME_NORMAL_2HOUR		2
#define	NBCOAP_RATIME_NORMAL_4HOUR		4
#define	NBCOAP_RATIME_NORMAL_TYPE		NBCOAP_RATIME_NORMAL_2HOUR			//Coap间隔时间发送普通数据包

#define	NBCOAP_PCP_UPGRADE_LIMIT_RSSI		10								//PCP升级限制信号值
#define	NBCOAP_PCP_UPGRADE_LIMIT_SNR		-80								//PCP升级限制信噪比

#define	NBIOT_CONTINUE_LIFETIME			2								//NBIOT休眠正放继续活跃20秒
#define	NBIOT_MAX_LIFETIME				12								//NBIOT休眠正放最大活跃120秒

#define	NBIOT_CLEAR_STORED_EARFCN_DISABLE	0
#define	NBIOT_CLEAR_STORED_EARFCN_ENABLE	1
#if NETPROTOCAL == NETCOAP
#define	NBIOT_CLEAR_STORED_EARFCN_STAT	NBIOT_CLEAR_STORED_EARFCN_ENABLE		//NBIOT清除频点功能
#endif
#if NETPROTOCAL == NETMQTTSN
#define	NBIOT_CLEAR_STORED_EARFCN_STAT	NBIOT_CLEAR_STORED_EARFCN_DISABLE		//NBIOT清除频点功能
#endif
#if NETPROTOCAL == NETONENET
#define	NBIOT_CLEAR_STORED_EARFCN_STAT	NBIOT_CLEAR_STORED_EARFCN_ENABLE		//NBIOT清除频点功能
#endif

#define	NBIOT_CLEAR_STORED_EARFCN_TRUE	true
#define	NBIOT_CLEAR_STORED_EARFCN_FALSE	false
#define	NBIOT_CLEAR_STORED_EARFCN_TYPE	NBIOT_CLEAR_STORED_EARFCN_FALSE		//NBIOT默认初始化后清除频点

#define	NBIOT_STATION_TIMESTAMP_DISABLE	0
#define	NBIOT_STATION_TIMESTAMP_ENABLE	1
#define	NBIOT_STATION_TIMESTAMP_TYPE		NBIOT_STATION_TIMESTAMP_ENABLE		//NBIOT基站时间检查

#define	NBIOT_STATION_TIMESTAMP_DIFFTIME	20 * 60							//NBIOT基站时间差20Min
#define	NBIOT_STATION_TIMESTAMP_ERRCOUNT	1								//NBIOT基站时间异常次数

#define	NBIOT_MODULE_MODE_V120			120
#define	NBIOT_MODULE_MODE_V150			150
#define	NBIOT_MODULE_MODE_TYPE			NBIOT_MODULE_MODE_V150				//NBIOT模组类型

#define	RADIO_CMD_ECHO_DISABLE			0
#define	RADIO_CMD_ECHO_ENABLE			1
#define	RADIO_CMD_ECHO_TYPE				RADIO_CMD_ECHO_DISABLE				//无线命令回显

#define	DEVICE_BOOT_START_MAGINIT_DISABLE	0
#define	DEVICE_BOOT_START_MAGINIT_ENABLE	1
#define	DEVICE_BOOT_START_MAGINIT_TYPE	DEVICE_BOOT_START_MAGINIT_DISABLE		//上电初始化背景

#define	NBIOT_MODEL_LIERDA				0
#define	NBIOT_MODEL_QUECTEL				1
#define	NBIOT_MODEL_TYPE				NBIOT_MODEL_LIERDA					//NB模组厂家

#define	NBIOT_AUTO_MODEL_DISABLE			0
#define	NBIOT_AUTO_MODEL_ENABLE			1
#define	NBIOT_AUTO_MODEL_TYPE			NBIOT_AUTO_MODEL_ENABLE				//NB模组厂家自动选择

#define	PRODUCTTEST_READ_DISABLE			0
#define	PRODUCTTEST_READ_ENABLE			1
#define	PRODUCTTEST_READ_TYPE			PRODUCTTEST_READ_ENABLE				//测试点测试模式

#define	PRODUCTTEST_TIMEOUT_DISABLE		0
#define	PRODUCTTEST_TIMEOUT_ENABLE		1
#define	PRODUCTTEST_TIMEOUT_TYPE			PRODUCTTEST_TIMEOUT_ENABLE			//测试点超时退出测试

#define	PRODUCTTEST_OVER_TIME			300								//测试点超时时间

#define	PRODUCTTEST_OVER_BOOTCNT			30								//测试点超过启动次数

#define	TESTBENCH_DISABLE				0
#define	TESTBENCH_ENABLE				1
#define	TESTBENCH_TYPE					TESTBENCH_ENABLE					//测试架测试

#define	ROLLINGOVER_INITSENSOR_CLOSE		0
#define	ROLLINGOVER_INITSENSOR_OPEN		1
#define	ROLLINGOVER_INITSENSOR_DEFAULT	ROLLINGOVER_INITSENSOR_CLOSE			//翻转初始化默认

#define	ROLLINGOVER_INITSENSOR_DISABLE	0
#define	ROLLINGOVER_INITSENSOR_ENABLE		1
#define	ROLLINGOVER_INITSENSOR_TYPE		ROLLINGOVER_INITSENSOR_DISABLE		//翻转初始化背景

#define	NBIOT_SNEDCOUNTDAY_LIMIT_DISABLE	0
#define	NBIOT_SNEDCOUNTDAY_LIMIT_ENABLE	1
#define	NBIOT_SNEDCOUNTDAY_LIMIT_TYPE		NBIOT_SNEDCOUNTDAY_LIMIT_ENABLE		//限定一天NB发送包数使能

#define	NBIOT_SNEDCOUNTDAY_LIMIT_NUM		200								//限定一天NB发送包数

#define	RADAR_MODEL_V1					1
#define	RADAR_MODEL_V2					2
#define	RADAR_MODEL_TYPE				RADAR_MODEL_V2						//雷达类型

#define	BEEP_ACTIVE					0
#define	BEEP_PASSIVE					1
#define	BEEP_MODEL_TYPE				BEEP_PASSIVE						//蜂鸣器类型

#define	BEEP_ON_US					230								//无源蜂鸣器开时间
#define	BEEP_OFF_US					230								//无源蜂鸣器关时间

#define	SOFTWAREMAJOR					20								//主固件版本
#define	SOFTWARESUB					149								//从固件版本
#define	HARDWAREMAJOR_V1				2								//主硬件版本
#define	HARDWAREMAJOR_V2				12								//主硬件版本

#if NETPROTOCAL == NETCOAP
#define	NBIOT_PRINT_ERRORCODE_DISABLE		0
#define	NBIOT_PRINT_ERRORCODE_ENABLE		1
#define	NBIOT_PRINT_ERROR_CODE_TYPE		NBIOT_PRINT_ERRORCODE_ENABLE			//NBIOT输出错误码模式

#define	RADIO_CMD_UPLOAD_WORKINFO		1								//无线命令主动上报WorkInfo
#define	RADIO_CMD_UPLOAD_NETINFO			1								//无线命令主动上报NetInfo
#define	RADIO_CMD_UPLOAD_DEVINFO			1								//无线命令主动上报DeviceInfo
#define	RADIO_CMD_UPLOAD_UPGRADEINFO		1								//无线命令主动上报UpgradeInfo

#define	RADIO_PRINT_WORKINFO			1								//无线输出Workinfo
#define	RADIO_PRINT_NETINFO				1								//无线输出NetInfo
#define	RADIO_PRINT_DEVINFO				1								//无线输出DeviceInfo
#define	RADIO_PRINT_UPGRADEINFO			1								//无线输出UpgradeInfo

#define	RADIO_PRINT_WORKINFO_SOFT		1								//无线输出Workinfo SOFT
#define	RADIO_PRINT_WORKINFO_SENSE		0								//无线输出Workinfo SENSE
#define	RADIO_PRINT_WORKINFO_MODE		0								//无线输出Workinfo MODE
#define	RADIO_PRINT_WORKINFO_CHANNEL		0								//无线输出Workinfo CHANNEL
#define	RADIO_PRINT_WORKINFO_RANGE		0								//无线输出Workinfo RANGE
#define	RADIO_PRINT_WORKINFO_EARFCN		1								//无线输出Workinfo EARFCN
#define	RADIO_PRINT_WORKINFO_TAC			1								//无线输出Workinfo TAC
#define	RADIO_PRINT_WORKINFO_CI			1								//无线输出Workinfo CI
#define	RADIO_PRINT_WORKINFO_CMDCNT		0								//无线输出Workinfo CMDCNT
#define	RADIO_PRINT_WORKINFO_NBRUNTIME	0								//无线输出Workinfo NBRUNTIME
#define	RADIO_PRINT_WORKINFO_APN			0								//无线输出Workinfo APN
#define	RADIO_PRINT_WORKINFO_COEF		1								//无线输出Workinfo COEF

#define	RADIO_PRINT_NETINFO_MUFTUR		1								//无线输出NetInfo MUFTUR
#define	RADIO_PRINT_NETINFO_MUFTURMD		0								//无线输出NetInfo MUFTURMD
#define	RADIO_PRINT_NETINFO_MDUVER		1								//无线输出NetInfo MDUVER
#define	RADIO_PRINT_NETINFO_IMEI			1								//无线输出NetInfo IMEI
#define	RADIO_PRINT_NETINFO_ICCID		1								//无线输出NetInfo ICCID
#define	RADIO_PRINT_NETINFO_IMSI			1								//无线输出NetInfo IMSI
#define	RADIO_PRINT_NETINFO_CGP			1								//无线输出NetInfo CGP
#define	RADIO_PRINT_NETINFO_PDPTYPE		0								//无线输出NetInfo PDPTYPE
#define	RADIO_PRINT_NETINFO_APN			0								//无线输出NetInfo APN
#define	RADIO_PRINT_NETINFO_RSSI			1								//无线输出NetInfo RSSI
#define	RADIO_PRINT_NETINFO_SNR			1								//无线输出NetInfo SNR
#define	RADIO_PRINT_NETINFO_OTHER		1								//无线输出NetInfo OTHER

#define	RADIO_PRINT_DEVINFO_RUNTIME		1								//无线输出DeviceInfo RUNTIME
#define	RADIO_PRINT_DEVINFO_BATT			1								//无线输出DeviceInfo BATT
#define	RADIO_PRINT_DEVINFO_RDLIB		0								//无线输出DeviceInfo RDLIB
#define	RADIO_PRINT_DEVINFO_RDCNT		0								//无线输出DeviceInfo RDCNT
#define	RADIO_PRINT_DEVINFO_MCUTEMP		0								//无线输出DeviceInfo MCUTEMP
#define	RADIO_PRINT_DEVINFO_ALGOLIB		0								//无线输出DeviceInfo ALGOLIB
#define	RADIO_PRINT_DEVINFO_QMCREBOOT		1								//无线输出DeviceInfo QMCREBOOT
#define	RADIO_PRINT_DEVINFO_NBBOOT		1								//无线输出DeviceInfo NBBOOT
#define	RADIO_PRINT_DEVINFO_NBSENT		0								//无线输出DeviceInfo NBSENT
#define	RADIO_PRINT_DEVINFO_NBRECV		0								//无线输出DeviceInfo NBRECV
#define	RADIO_PRINT_DEVINFO_INDELAY		0								//无线输出DeviceInfo INDELAY
#define	RADIO_PRINT_DEVINFO_NBHEART		0								//无线输出DeviceInfo NBHEART
#define	RADIO_PRINT_DEVINFO_CGAIN		1								//无线输出DeviceInfo CGAIN
#define	RADIO_PRINT_DEVINFO_RGAIN		1								//无线输出DeviceInfo RGAIN
#define	RADIO_PRINT_DEVINFO_SMODE		1								//无线输出DeviceInfo SMODE
#endif
#if NETPROTOCAL == NETMQTTSN
#define	NBIOT_PRINT_ERRORCODE_DISABLE		0
#define	NBIOT_PRINT_ERRORCODE_ENABLE		1
#define	NBIOT_PRINT_ERROR_CODE_TYPE		NBIOT_PRINT_ERRORCODE_DISABLE			//NBIOT输出错误码模式

#define	RADIO_CMD_UPLOAD_WORKINFO		1								//无线命令主动上报WorkInfo
#define	RADIO_CMD_UPLOAD_NETINFO			1								//无线命令主动上报NetInfo
#define	RADIO_CMD_UPLOAD_DEVINFO			0								//无线命令主动上报DeviceInfo
#define	RADIO_CMD_UPLOAD_UPGRADEINFO		0								//无线命令主动上报UpgradeInfo

#define	RADIO_PRINT_WORKINFO			0								//无线输出Workinfo
#define	RADIO_PRINT_NETINFO				1								//无线输出NetInfo
#define	RADIO_PRINT_DEVINFO				0								//无线输出DeviceInfo
#define	RADIO_PRINT_UPGRADEINFO			0								//无线输出UpgradeInfo

#define	RADIO_PRINT_WORKINFO_SOFT		1								//无线输出Workinfo SOFT
#define	RADIO_PRINT_WORKINFO_SENSE		0								//无线输出Workinfo SENSE
#define	RADIO_PRINT_WORKINFO_MODE		0								//无线输出Workinfo MODE
#define	RADIO_PRINT_WORKINFO_CHANNEL		0								//无线输出Workinfo CHANNEL
#define	RADIO_PRINT_WORKINFO_RANGE		0								//无线输出Workinfo RANGE
#define	RADIO_PRINT_WORKINFO_EARFCN		1								//无线输出Workinfo EARFCN
#define	RADIO_PRINT_WORKINFO_TAC			1								//无线输出Workinfo TAC
#define	RADIO_PRINT_WORKINFO_CI			1								//无线输出Workinfo CI
#define	RADIO_PRINT_WORKINFO_CMDCNT		0								//无线输出Workinfo CMDCNT
#define	RADIO_PRINT_WORKINFO_NBRUNTIME	0								//无线输出Workinfo NBRUNTIME
#define	RADIO_PRINT_WORKINFO_APN			0								//无线输出Workinfo APN
#define	RADIO_PRINT_WORKINFO_COEF		1								//无线输出Workinfo COEF

#define	RADIO_PRINT_NETINFO_MUFTUR		0								//无线输出NetInfo MUFTUR
#define	RADIO_PRINT_NETINFO_MUFTURMD		0								//无线输出NetInfo MUFTURMD
#define	RADIO_PRINT_NETINFO_MDUVER		0								//无线输出NetInfo MDUVER
#define	RADIO_PRINT_NETINFO_IMEI			1								//无线输出NetInfo IMEI
#define	RADIO_PRINT_NETINFO_ICCID		1								//无线输出NetInfo ICCID
#define	RADIO_PRINT_NETINFO_IMSI			1								//无线输出NetInfo IMSI
#define	RADIO_PRINT_NETINFO_CGP			0								//无线输出NetInfo CGP
#define	RADIO_PRINT_NETINFO_PDPTYPE		0								//无线输出NetInfo PDPTYPE
#define	RADIO_PRINT_NETINFO_APN			0								//无线输出NetInfo APN
#define	RADIO_PRINT_NETINFO_RSSI			0								//无线输出NetInfo RSSI
#define	RADIO_PRINT_NETINFO_SNR			0								//无线输出NetInfo SNR
#define	RADIO_PRINT_NETINFO_OTHER		0								//无线输出NetInfo OTHER

#define	RADIO_PRINT_DEVINFO_RUNTIME		1								//无线输出DeviceInfo RUNTIME
#define	RADIO_PRINT_DEVINFO_BATT			1								//无线输出DeviceInfo BATT
#define	RADIO_PRINT_DEVINFO_RDLIB		0								//无线输出DeviceInfo RDLIB
#define	RADIO_PRINT_DEVINFO_RDCNT		0								//无线输出DeviceInfo RDCNT
#define	RADIO_PRINT_DEVINFO_MCUTEMP		0								//无线输出DeviceInfo MCUTEMP
#define	RADIO_PRINT_DEVINFO_ALGOLIB		0								//无线输出DeviceInfo ALGOLIB
#define	RADIO_PRINT_DEVINFO_QMCREBOOT		1								//无线输出DeviceInfo QMCREBOOT
#define	RADIO_PRINT_DEVINFO_NBBOOT		1								//无线输出DeviceInfo NBBOOT
#define	RADIO_PRINT_DEVINFO_NBSENT		0								//无线输出DeviceInfo NBSENT
#define	RADIO_PRINT_DEVINFO_NBRECV		0								//无线输出DeviceInfo NBRECV
#define	RADIO_PRINT_DEVINFO_INDELAY		0								//无线输出DeviceInfo INDELAY
#define	RADIO_PRINT_DEVINFO_NBHEART		0								//无线输出DeviceInfo NBHEART
#define	RADIO_PRINT_DEVINFO_CGAIN		1								//无线输出DeviceInfo CGAIN
#define	RADIO_PRINT_DEVINFO_RGAIN		1								//无线输出DeviceInfo RGAIN
#define	RADIO_PRINT_DEVINFO_SMODE		1								//无线输出DeviceInfo SMODE
#endif
#if NETPROTOCAL == NETONENET
#define	NBIOT_PRINT_ERRORCODE_DISABLE		0
#define	NBIOT_PRINT_ERRORCODE_ENABLE		1
#define	NBIOT_PRINT_ERROR_CODE_TYPE		NBIOT_PRINT_ERRORCODE_ENABLE			//NBIOT输出错误码模式

#define	RADIO_CMD_UPLOAD_WORKINFO		1								//无线命令主动上报WorkInfo
#define	RADIO_CMD_UPLOAD_NETINFO			1								//无线命令主动上报NetInfo
#define	RADIO_CMD_UPLOAD_DEVINFO			1								//无线命令主动上报DeviceInfo
#define	RADIO_CMD_UPLOAD_UPGRADEINFO		1								//无线命令主动上报UpgradeInfo

#define	RADIO_PRINT_WORKINFO			1								//无线输出Workinfo
#define	RADIO_PRINT_NETINFO				1								//无线输出NetInfo
#define	RADIO_PRINT_DEVINFO				1								//无线输出DeviceInfo
#define	RADIO_PRINT_UPGRADEINFO			1								//无线输出UpgradeInfo

#define	RADIO_PRINT_WORKINFO_SOFT		1								//无线输出Workinfo SOFT
#define	RADIO_PRINT_WORKINFO_SENSE		0								//无线输出Workinfo SENSE
#define	RADIO_PRINT_WORKINFO_MODE		0								//无线输出Workinfo MODE
#define	RADIO_PRINT_WORKINFO_CHANNEL		0								//无线输出Workinfo CHANNEL
#define	RADIO_PRINT_WORKINFO_RANGE		0								//无线输出Workinfo RANGE
#define	RADIO_PRINT_WORKINFO_EARFCN		1								//无线输出Workinfo EARFCN
#define	RADIO_PRINT_WORKINFO_TAC			1								//无线输出Workinfo TAC
#define	RADIO_PRINT_WORKINFO_CI			1								//无线输出Workinfo CI
#define	RADIO_PRINT_WORKINFO_CMDCNT		0								//无线输出Workinfo CMDCNT
#define	RADIO_PRINT_WORKINFO_NBRUNTIME	0								//无线输出Workinfo NBRUNTIME
#define	RADIO_PRINT_WORKINFO_APN			0								//无线输出Workinfo APN
#define	RADIO_PRINT_WORKINFO_COEF		1								//无线输出Workinfo COEF

#define	RADIO_PRINT_NETINFO_MUFTUR		1								//无线输出NetInfo MUFTUR
#define	RADIO_PRINT_NETINFO_MUFTURMD		0								//无线输出NetInfo MUFTURMD
#define	RADIO_PRINT_NETINFO_MDUVER		1								//无线输出NetInfo MDUVER
#define	RADIO_PRINT_NETINFO_IMEI			1								//无线输出NetInfo IMEI
#define	RADIO_PRINT_NETINFO_ICCID		1								//无线输出NetInfo ICCID
#define	RADIO_PRINT_NETINFO_IMSI			1								//无线输出NetInfo IMSI
#define	RADIO_PRINT_NETINFO_CGP			1								//无线输出NetInfo CGP
#define	RADIO_PRINT_NETINFO_PDPTYPE		0								//无线输出NetInfo PDPTYPE
#define	RADIO_PRINT_NETINFO_APN			0								//无线输出NetInfo APN
#define	RADIO_PRINT_NETINFO_RSSI			1								//无线输出NetInfo RSSI
#define	RADIO_PRINT_NETINFO_SNR			1								//无线输出NetInfo SNR
#define	RADIO_PRINT_NETINFO_OTHER		1								//无线输出NetInfo OTHER

#define	RADIO_PRINT_DEVINFO_RUNTIME		1								//无线输出DeviceInfo RUNTIME
#define	RADIO_PRINT_DEVINFO_BATT			1								//无线输出DeviceInfo BATT
#define	RADIO_PRINT_DEVINFO_RDLIB		0								//无线输出DeviceInfo RDLIB
#define	RADIO_PRINT_DEVINFO_RDCNT		0								//无线输出DeviceInfo RDCNT
#define	RADIO_PRINT_DEVINFO_MCUTEMP		0								//无线输出DeviceInfo MCUTEMP
#define	RADIO_PRINT_DEVINFO_ALGOLIB		0								//无线输出DeviceInfo ALGOLIB
#define	RADIO_PRINT_DEVINFO_QMCREBOOT		1								//无线输出DeviceInfo QMCREBOOT
#define	RADIO_PRINT_DEVINFO_NBBOOT		1								//无线输出DeviceInfo NBBOOT
#define	RADIO_PRINT_DEVINFO_NBSENT		0								//无线输出DeviceInfo NBSENT
#define	RADIO_PRINT_DEVINFO_NBRECV		0								//无线输出DeviceInfo NBRECV
#define	RADIO_PRINT_DEVINFO_INDELAY		0								//无线输出DeviceInfo INDELAY
#define	RADIO_PRINT_DEVINFO_NBHEART		0								//无线输出DeviceInfo NBHEART
#define	RADIO_PRINT_DEVINFO_CGAIN		1								//无线输出DeviceInfo CGAIN
#define	RADIO_PRINT_DEVINFO_RGAIN		1								//无线输出DeviceInfo RGAIN
#define	RADIO_PRINT_DEVINFO_SMODE		1								//无线输出DeviceInfo SMODE
#endif

#define	NBCOAP_SENDCODE_SHORT_STATUS		0								//NB上报信息使能CoapShortStatus
#define	NBCOAP_SENDCODE_LONG_STATUS		1								//NB上报信息使能CoapLongStatus
#define	NBCOAP_SENDCODE_WORK_INFO		1								//NB上报信息使能CoapWorkInfo
#define	NBCOAP_SENDCODE_BASIC_INFO		1								//NB上报信息使能CoapBasicInfo
#define	NBCOAP_SENDCODE_DYNAMIC_INFO		1								//NB上报信息使能CoapDynamicInfo
#define	NBCOAP_SENDCODE_RESPONSE_INFO		1								//NB上报信息使能CoapResponseInfo
#define	NBCOAP_SENDCODE_QMC_DATA			1								//NB上报信息使能CoapQmcData

#define	NBMQTTSN_SENDCODE_STATUS_BASIC	0								//NB上报信息使能MqttSNStatusBasic
#define	NBMQTTSN_SENDCODE_STATUS_EXTEND	1								//NB上报信息使能MqttSNStatusExtend
#define	NBMQTTSN_SENDCODE_WORK_INFO		1								//NB上报信息使能MqttSNWorkInfo
#define	NBMQTTSN_SENDCODE_BASIC_INFO		1								//NB上报信息使能MqttSNBasicInfo
#define	NBMQTTSN_SENDCODE_DYNAMIC_INFO	1								//NB上报信息使能MqttSNDynamicInfo
#define	NBMQTTSN_SENDCODE_RESPONSE_INFO	1								//NB上报信息使能MqttSNResponseInfo
#define	NBMQTTSN_SENDCODE_QMC_DATA		1								//NB上报信息使能MqttSNQmcData

#define	NBONENET_SENDCODE_SHORT_STATUS	0								//NB上报信息使能OneNETShortStatus
#define	NBONENET_SENDCODE_LONG_STATUS		1								//NB上报信息使能OneNETLongStatus
#define	NBONENET_SENDCODE_WORK_INFO		1								//NB上报信息使能OneNETWorkInfo
#define	NBONENET_SENDCODE_BASIC_INFO		1								//NB上报信息使能OneNETBasicInfo
#define	NBONENET_SENDCODE_DYNAMIC_INFO	1								//NB上报信息使能OneNETDynamicInfo
#define	NBONENET_SENDCODE_RESPONSE_INFO	1								//NB上报信息使能OneNETResponseInfo
#define	NBONENET_SENDCODE_QMC_DATA		1								//NB上报信息使能OneNETQmcData

#if NETPROTOCAL == NETCOAP
#define	RADIO_DOWNLOAD_CMD_REBOOT		1								//RADIO下行指令Reboot
#define	RADIO_DOWNLOAD_CMD_NEWSN			1								//RADIO下行指令NewSn
#define	RADIO_DOWNLOAD_CMD_CDPIP			1								//RADIO下行指令CDPIP
#define	RADIO_DOWNLOAD_CMD_ACTIVE		1								//RADIO下行指令Active
#define	RADIO_DOWNLOAD_CMD_MAGMOD		1								//RADIO下行指令MagMod
#define	RADIO_DOWNLOAD_CMD_NBHEART		1								//RADIO下行指令NbHeart
#define	RADIO_DOWNLOAD_CMD_MAGINIT		1								//RADIO下行指令MagInit
#define	RADIO_DOWNLOAD_CMD_DISRANGE		1								//RADIO下行指令DisRange
#define	RADIO_DOWNLOAD_CMD_CARINDELAY		1								//RADIO下行指令CarInDelay
#define	RADIO_DOWNLOAD_CMD_RATIME		1								//RADIO下行指令RATime
#define	RADIO_DOWNLOAD_CMD_MAGTEMPCOEF	1								//RADIO下行指令MagTempCoef
#define	RADIO_DOWNLOAD_CMD_SETQMCCOEF		1								//RADIO下行指令SetQmcCoef
#define	RADIO_DOWNLOAD_CMD_BEEPOFF		1								//RADIO下行指令BeepOff
#define	RADIO_DOWNLOAD_CMD_ROLLINIT		1								//RADIO下行指令RollInit
#define	RADIO_DOWNLOAD_CMD_UPLIMIT		1								//RADIO下行指令UpLimit
#define	RADIO_DOWNLOAD_CMD_NBLIMIT		1								//RADIO下行指令NBLimit
#define	RADIO_DOWNLOAD_CMD_COVERGAIN		1								//RADIO下行指令CoverGain
#define	RADIO_DOWNLOAD_CMD_RADARGAIN		1								//RADIO下行指令RadarGain
#define	RADIO_DOWNLOAD_CMD_SENSORMODE		1								//RADIO下行指令SensorMode
#define	RADIO_DOWNLOAD_CMD_INTERVAL		1								//RADIO下行指令RadarSampleInterval
#define	RADIO_DOWNLOAD_CMD_HIGHPASS		1								//RADIO下行指令RadarHighPass
#define	RADIO_DOWNLOAD_CMD_RFDPRINTLV		1								//RADIO下行指令RFDprintLv
#define	RADIO_DOWNLOAD_CMD_RESTORE		1								//RADIO下行指令Restore
#endif
#if NETPROTOCAL == NETMQTTSN
#define	RADIO_DOWNLOAD_CMD_REBOOT		1								//RADIO下行指令Reboot
#define	RADIO_DOWNLOAD_CMD_NEWSN			1								//RADIO下行指令NewSn
#define	RADIO_DOWNLOAD_CMD_CDPIP			0								//RADIO下行指令CDPIP
#define	RADIO_DOWNLOAD_CMD_ACTIVE		1								//RADIO下行指令Active
#define	RADIO_DOWNLOAD_CMD_MAGMOD		1								//RADIO下行指令MagMod
#define	RADIO_DOWNLOAD_CMD_NBHEART		1								//RADIO下行指令NbHeart
#define	RADIO_DOWNLOAD_CMD_MAGINIT		1								//RADIO下行指令MagInit
#define	RADIO_DOWNLOAD_CMD_DISRANGE		1								//RADIO下行指令DisRange
#define	RADIO_DOWNLOAD_CMD_CARINDELAY		0								//RADIO下行指令CarInDelay
#define	RADIO_DOWNLOAD_CMD_RATIME		0								//RADIO下行指令RATime
#define	RADIO_DOWNLOAD_CMD_MAGTEMPCOEF	0								//RADIO下行指令MagTempCoef
#define	RADIO_DOWNLOAD_CMD_SETQMCCOEF		0								//RADIO下行指令SetQmcCoef
#define	RADIO_DOWNLOAD_CMD_BEEPOFF		0								//RADIO下行指令BeepOff
#define	RADIO_DOWNLOAD_CMD_ROLLINIT		0								//RADIO下行指令RollInit
#define	RADIO_DOWNLOAD_CMD_UPLIMIT		0								//RADIO下行指令UpLimit
#define	RADIO_DOWNLOAD_CMD_NBLIMIT		0								//RADIO下行指令NBLimit
#define	RADIO_DOWNLOAD_CMD_COVERGAIN		0								//RADIO下行指令CoverGain
#define	RADIO_DOWNLOAD_CMD_RADARGAIN		0								//RADIO下行指令RadarGain
#define	RADIO_DOWNLOAD_CMD_SENSORMODE		1								//RADIO下行指令SensorMode
#define	RADIO_DOWNLOAD_CMD_INTERVAL		1								//RADIO下行指令RadarSampleInterval
#define	RADIO_DOWNLOAD_CMD_HIGHPASS		1								//RADIO下行指令RadarHighPass
#define	RADIO_DOWNLOAD_CMD_RFDPRINTLV		1								//RADIO下行指令RFDprintLv
#define	RADIO_DOWNLOAD_CMD_RESTORE		0								//RADIO下行指令Restore
#endif
#if NETPROTOCAL == NETONENET
#define	RADIO_DOWNLOAD_CMD_REBOOT		1								//RADIO下行指令Reboot
#define	RADIO_DOWNLOAD_CMD_NEWSN			1								//RADIO下行指令NewSn
#define	RADIO_DOWNLOAD_CMD_CDPIP			1								//RADIO下行指令CDPIP
#define	RADIO_DOWNLOAD_CMD_ACTIVE		1								//RADIO下行指令Active
#define	RADIO_DOWNLOAD_CMD_MAGMOD		1								//RADIO下行指令MagMod
#define	RADIO_DOWNLOAD_CMD_NBHEART		1								//RADIO下行指令NbHeart
#define	RADIO_DOWNLOAD_CMD_MAGINIT		1								//RADIO下行指令MagInit
#define	RADIO_DOWNLOAD_CMD_DISRANGE		1								//RADIO下行指令DisRange
#define	RADIO_DOWNLOAD_CMD_CARINDELAY		1								//RADIO下行指令CarInDelay
#define	RADIO_DOWNLOAD_CMD_RATIME		1								//RADIO下行指令RATime
#define	RADIO_DOWNLOAD_CMD_MAGTEMPCOEF	1								//RADIO下行指令MagTempCoef
#define	RADIO_DOWNLOAD_CMD_SETQMCCOEF		1								//RADIO下行指令SetQmcCoef
#define	RADIO_DOWNLOAD_CMD_BEEPOFF		1								//RADIO下行指令BeepOff
#define	RADIO_DOWNLOAD_CMD_ROLLINIT		1								//RADIO下行指令RollInit
#define	RADIO_DOWNLOAD_CMD_UPLIMIT		1								//RADIO下行指令UpLimit
#define	RADIO_DOWNLOAD_CMD_NBLIMIT		1								//RADIO下行指令NBLimit
#define	RADIO_DOWNLOAD_CMD_COVERGAIN		1								//RADIO下行指令CoverGain
#define	RADIO_DOWNLOAD_CMD_RADARGAIN		1								//RADIO下行指令RadarGain
#define	RADIO_DOWNLOAD_CMD_SENSORMODE		1								//RADIO下行指令SensorMode
#define	RADIO_DOWNLOAD_CMD_INTERVAL		1								//RADIO下行指令RadarSampleInterval
#define	RADIO_DOWNLOAD_CMD_HIGHPASS		1								//RADIO下行指令RadarHighPass
#define	RADIO_DOWNLOAD_CMD_RFDPRINTLV		1								//RADIO下行指令RFDprintLv
#define	RADIO_DOWNLOAD_CMD_RESTORE		1								//RADIO下行指令Restore
#endif

#define	NBCOAP_DOWNLOAD_CMD_WORKMODE		1								//NBCOAP下行指令Workmode
#define	NBCOAP_DOWNLOAD_CMD_SENSE		1								//NBCOAP下行指令Sense
#define	NBCOAP_DOWNLOAD_CMD_RFHEART		1								//NBCOAP下行指令RFHeart
#define	NBCOAP_DOWNLOAD_CMD_BACKGROUND	1								//NBCOAP下行指令Background
#define	NBCOAP_DOWNLOAD_CMD_REBOOT		1								//NBCOAP下行指令Reboot
#define	NBCOAP_DOWNLOAD_CMD_NEWSN		1								//NBCOAP下行指令NewSn
#define	NBCOAP_DOWNLOAD_CMD_CDPIP		1								//NBCOAP下行指令CDPIP
#define	NBCOAP_DOWNLOAD_CMD_ACTIVE		1								//NBCOAP下行指令Active
#define	NBCOAP_DOWNLOAD_CMD_MAGMOD		1								//NBCOAP下行指令MagMod
#define	NBCOAP_DOWNLOAD_CMD_NBHEART		1								//NBCOAP下行指令NbHeart
#define	NBCOAP_DOWNLOAD_CMD_INITRADAR		1								//NBCOAP下行指令InitRadar
#define	NBCOAP_DOWNLOAD_CMD_INITMAG		1								//NBCOAP下行指令InitMag
#define	NBCOAP_DOWNLOAD_CMD_DISRANGE		1								//NBCOAP下行指令DisRange
#define	NBCOAP_DOWNLOAD_CMD_CARINDELAY	1								//NBCOAP下行指令CarInDelay
#define	NBCOAP_DOWNLOAD_CMD_RATIME		1								//NBCOAP下行指令RATime
#define	NBCOAP_DOWNLOAD_CMD_MAGTEMPCOEF	1								//NBCOAP下行指令MagTempCoef
#define	NBCOAP_DOWNLOAD_CMD_SETQMCCOEF	1								//NBCOAP下行指令SetQmcCoef
#define	NBCOAP_DOWNLOAD_CMD_BEEPOFF		1								//NBCOAP下行指令BeepOff
#define	NBCOAP_DOWNLOAD_CMD_ROLLINIT		1								//NBCOAP下行指令RollInit
#define	NBCOAP_DOWNLOAD_CMD_UPLIMIT		1								//NBCOAP下行指令UpLimit
#define	NBCOAP_DOWNLOAD_CMD_NBLIMIT		1								//NBCOAP下行指令NBLimit
#define	NBCOAP_DOWNLOAD_CMD_COVERGAIN		1								//NBCOAP下行指令CoverGain
#define	NBCOAP_DOWNLOAD_CMD_RADARGAIN		1								//NBCOAP下行指令RadarGain
#define	NBCOAP_DOWNLOAD_CMD_SENSORMODE	1								//NBCOAP下行指令SensorMode
#define	NBCOAP_DOWNLOAD_CMD_SETMAG		1								//NBCOAP下行指令SetMag
#define	NBCOAP_DOWNLOAD_CMD_CFGRADAR		1								//NBCOAP下行指令ConfigRadar

#define	MQTTSN_DOWNLOAD_CMD_WORKMODE		1								//MQTTSN下行指令Workmode
#define	MQTTSN_DOWNLOAD_CMD_SENSE		0								//MQTTSN下行指令Sense
#define	MQTTSN_DOWNLOAD_CMD_RFHEART		0								//MQTTSN下行指令RFHeart
#define	MQTTSN_DOWNLOAD_CMD_BACKGROUND	1								//MQTTSN下行指令Background
#define	MQTTSN_DOWNLOAD_CMD_REBOOT		1								//MQTTSN下行指令Reboot
#define	MQTTSN_DOWNLOAD_CMD_NEWSN		1								//MQTTSN下行指令NewSn
#define	MQTTSN_DOWNLOAD_CMD_ACTIVE		1								//MQTTSN下行指令Active
#define	MQTTSN_DOWNLOAD_CMD_MAGMOD		1								//MQTTSN下行指令MagMod
#define	MQTTSN_DOWNLOAD_CMD_NBHEART		1								//MQTTSN下行指令NbHeart
#define	MQTTSN_DOWNLOAD_CMD_INITRADAR		1								//MQTTSN下行指令InitRadar
#define	MQTTSN_DOWNLOAD_CMD_INITMAG		1								//MQTTSN下行指令InitMag
#define	MQTTSN_DOWNLOAD_CMD_DISRANGE		1								//MQTTSN下行指令DisRange
#define	MQTTSN_DOWNLOAD_CMD_CARINDELAY	1								//MQTTSN下行指令CarInDelay
#define	MQTTSN_DOWNLOAD_CMD_MAGTEMPCOEF	1								//MQTTSN下行指令MagTempCoef
#define	MQTTSN_DOWNLOAD_CMD_SETQMCCOEF	1								//MQTTSN下行指令SetQmcCoef
#define	MQTTSN_DOWNLOAD_CMD_BEEPOFF		0								//MQTTSN下行指令BeepOff
#define	MQTTSN_DOWNLOAD_CMD_ROLLINIT		0								//MQTTSN下行指令RollInit
#define	MQTTSN_DOWNLOAD_CMD_UPLIMIT		0								//MQTTSN下行指令UpLimit
#define	MQTTSN_DOWNLOAD_CMD_NBLIMIT		0								//MQTTSN下行指令NBLimit
#define	MQTTSN_DOWNLOAD_CMD_COVERGAIN		1								//MQTTSN下行指令CoverGain
#define	MQTTSN_DOWNLOAD_CMD_RADARGAIN		1								//MQTTSN下行指令RadarGain
#define	MQTTSN_DOWNLOAD_CMD_SENSORMODE	1								//MQTTSN下行指令SensorMode
#define	MQTTSN_DOWNLOAD_CMD_SETMAG		1								//MQTTSN下行指令SetMag
#define	MQTTSN_DOWNLOAD_CMD_CFGRADAR		1								//MQTTSN下行指令ConfigRadar

#define	RF_DPRINT_LV_0					0								//调试信息不打印
#define	RF_DPRINT_LV_1					1								//基本信息打印
#define	RF_DPRINT_LV_2					2								//NB信息打印
#define	RF_DPRINT_LV_3					3								//所有信息打印
#define	RF_DPRINT_LV_TYPE				RF_DPRINT_LV_2						//无线调试信息输出等级

#define	RADIO_SI4438													//无线开启
#define	GD25Q_80CSIG													//SPIFLASH开启

#define	RADIO_RF_CHANNEL1				36								//无线默认通道

#define	USART1_DISABLE					0
#define	USART1_ENABLE					1
#define	USART1_TYPE					USART1_ENABLE						//串口1状态

#define	USART2_DISABLE					0
#define	USART2_ENABLE					1
#define	USART2_TYPE					USART2_DISABLE						//串口2状态

#define	USART3_DISABLE					0
#define	USART3_ENABLE					1
#define	USART3_TYPE					USART3_DISABLE						//串口3状态

#define	USART_IRQ_CTRL_DISABLE			0
#define	USART_IRQ_CTRL_ENABLE			1
#define	USART_IRQ_CTRL_TYPE				USART_IRQ_CTRL_DISABLE				//串口控制中断

#define	IDLE_WORK						4								//休眠
#define	NOTACTIVE_WORK					3								//不工作

#define	DEBUG_WORK					1								//调试工作模式
#define	NORMAL_WORK					0								//正常工作模式

#define	RADIO_GATWAY_NEARBY_HEART_SEC		5								//无线接收网关数据心跳包间隔时间
#define	RADIO_GATCMD_NEARBY_HEART_SEC		3								//无线接收网关数据命令包间隔时间

#define	NBIOT_MODULE_REBOOT_ERROR_COUNT	5								//NB重启错误限定次数
#define	NBIOT_MODULE_REBOOT_ERROR_INIT	2								//NB重启错误串口次数

#define	TIME_TO_MODULE_INIT				3600*24*3							//间隔时间初始化模块

#define	NBIOT_HEART_DATA_HOURS			4								//NB心跳数据包时间( 4 * 60 * 60)S
#define	NBIOT_HEART_DATA_TIMER			16								//NB心跳数据包时间(16 * 15 * 60)S

#define	UPLOAD_QMCDATA_MAXPACK			16								//QMC一次上传最大包数

#define	MQTTSN_DNS_SERVER_DISABLE		0
#define	MQTTSN_DNS_SERVER_ENABLE			1
#define	MQTTSN_DNS_SERVER_TYPE			MQTTSN_DNS_SERVER_ENABLE				//DNS服务状态

#define	MQTTSN_DNS_USE_EEPROM_DISABLE		0
#define	MQTTSN_DNS_USE_EEPROM_ENABLE		1
#define	MQTTSN_DNS_USE_EEPROM_TYPE		MQTTSN_DNS_USE_EEPROM_ENABLE			//DNS服务器使用EERPOM存储MqttSN地址

#define	DNS_SERVER_HOST_IP				"114.114.114.114"					//DNS服务器默认配置
#define	DNS_SERVER_LOCAL_PORT			5000
#define	DNS_SERVER_TELE_PORT			53

#define	MQTTSN_SERVER_HOST_NAME			"mqtt.movebroad.cn"					//MQTTSN服务器默认配置
#define	MQTTSN_SERVER_HOST_IP			"47.98.140.145"
#define	MQTTSN_SERVER_LOCAL_PORT			4000
#define	MQTTSN_SERVER_TELE_PORT			1884

#define	COAPCDPADDR_CHINA_TELECOM_FORMAL	"117.60.157.137"					//电信生产 : "117.60.157.137"		ip753c9d89:5683
#define	COAPCDPADDR_CHINA_TELECOM_TEST	"180.101.147.115"					//电信测试 : "180.101.147.115"	ipb4659373:5683
#define	COAPCDPPORT_CHINA_TELECOM		5683
#define	COAPCDPADDR_CHINA_MOBILE_FORMAL	"117.60.157.137"					//移动生产 : "117.60.157.137"		ip753c9d89:5683
#define	COAPCDPADDR_CHINA_MOBILE_TEST		"180.101.147.115"					//移动测试 : "180.101.147.115"	ipb4659373:5683
#define	COAPCDPPORT_CHINA_MOBILE			5683
#define	COAPCDPADDR_CHINA_UNICOM_FORMAL	"58.240.96.46"						//联通生产 : "58.240.96.46"		ip3af0602e:5683
#define	COAPCDPADDR_CHINA_UNICOM_TEST		"58.240.96.46"						//联通测试 : "58.240.96.46"		ip3af0602e:5683
#define	COAPCDPPORT_CHINA_UNICOM			5683
#define	COAPCDPADDR_OTHER_UNICOM_FORMAL	"117.60.157.137"					//其他生产 : "117.60.157.137"		ip753c9d89:5683
#define	COAPCDPADDR_OTHER_UNICOM_TEST		"180.101.147.115"					//其他测试 : "180.101.147.115"	ipb4659373:5683
#define	COAPCDPPORT_OTHER_UNICOM			5683

#define	NBNET_NBIOT_BAND1				NBand_2100MHz
#define	NBNET_NBIOT_BAND3				NBand_1800MHz
#define	NBNET_NBIOT_BAND5				NBand_850MHz
#define	NBNET_NBIOT_BAND8				NBand_900MHz
#define	NBNET_NBIOT_BAND20				NBand_800MHz
#define	NBNET_NBIOT_BAND28				NBand_700MHz

#if NETCARRIERTYPE == NETCARRIERCHINATELECOM
#define	COAPCDPADDR					COAPCDPADDR_CHINA_TELECOM_FORMAL
#define	COAPCDPPORT					COAPCDPPORT_CHINA_TELECOM

#define	COAP_NBIOT_BAND_NUM				1
#define	COAP_NBIOT_BAND_VAL1			NBNET_NBIOT_BAND5
#define	COAP_NBIOT_BAND_VAL2			NBNET_NBIOT_BAND5
#define	COAP_NBIOT_BAND_VAL3			NBNET_NBIOT_BAND5
#define	COAP_NBIOT_BAND_TYPE			CoAPNBIoTBandType
#define	COAP_NBIOT_CGATT_TIME_S			2 * 60 + 60 * COAP_NBIOT_BAND_NUM

#define	ONENET_NBIOT_BAND_NUM			1
#define	ONENET_NBIOT_BAND_VAL1			NBNET_NBIOT_BAND5
#define	ONENET_NBIOT_BAND_VAL2			NBNET_NBIOT_BAND5
#define	ONENET_NBIOT_BAND_VAL3			NBNET_NBIOT_BAND5
#define	ONENET_NBIOT_BAND_TYPE			OneNETNBIoTBandType
#define	ONENET_NBIOT_CGATT_TIME_S		2 * 60 + 60 * ONENET_NBIOT_BAND_NUM

#define	MQTTSN_NBIOT_BAND_NUM			1
#define	MQTTSN_NBIOT_BAND_VAL1			NBNET_NBIOT_BAND5
#define	MQTTSN_NBIOT_BAND_VAL2			NBNET_NBIOT_BAND5
#define	MQTTSN_NBIOT_BAND_VAL3			NBNET_NBIOT_BAND5
#define	MQTTSN_NBIOT_BAND_TYPE			MqttSNNBIoTBandType
#define	MQTTSN_NBIOT_CGATT_TIME_S		2 * 60 + 60 * MQTTSN_NBIOT_BAND_NUM

#define	DNS_NBIOT_BAND_NUM				1
#define	DNS_NBIOT_BAND_VAL1				NBNET_NBIOT_BAND5
#define	DNS_NBIOT_BAND_VAL2				NBNET_NBIOT_BAND5
#define	DNS_NBIOT_BAND_VAL3				NBNET_NBIOT_BAND5
#define	DNS_NBIOT_BAND_TYPE				DNSNBIoTBandType
#define	DNS_NBIOT_CGATT_TIME_S			2 * 60 + 60 * DNS_NBIOT_BAND_NUM
#endif
#if NETCARRIERTYPE == NETCARRIERCHINAMOBILE
#define	COAPCDPADDR					COAPCDPADDR_CHINA_MOBILE_FORMAL
#define	COAPCDPPORT					COAPCDPPORT_CHINA_MOBILE

#define	COAP_NBIOT_BAND_NUM				1
#define	COAP_NBIOT_BAND_VAL1			NBNET_NBIOT_BAND8
#define	COAP_NBIOT_BAND_VAL2			NBNET_NBIOT_BAND8
#define	COAP_NBIOT_BAND_VAL3			NBNET_NBIOT_BAND8
#define	COAP_NBIOT_BAND_TYPE			CoAPNBIoTBandType
#define	COAP_NBIOT_CGATT_TIME_S			2 * 60 + 60 * COAP_NBIOT_BAND_NUM

#define	ONENET_NBIOT_BAND_NUM			1
#define	ONENET_NBIOT_BAND_VAL1			NBNET_NBIOT_BAND8
#define	ONENET_NBIOT_BAND_VAL2			NBNET_NBIOT_BAND8
#define	ONENET_NBIOT_BAND_VAL3			NBNET_NBIOT_BAND8
#define	ONENET_NBIOT_BAND_TYPE			OneNETNBIoTBandType
#define	ONENET_NBIOT_CGATT_TIME_S		2 * 60 + 60 * ONENET_NBIOT_BAND_NUM

#define	MQTTSN_NBIOT_BAND_NUM			1
#define	MQTTSN_NBIOT_BAND_VAL1			NBNET_NBIOT_BAND8
#define	MQTTSN_NBIOT_BAND_VAL2			NBNET_NBIOT_BAND8
#define	MQTTSN_NBIOT_BAND_VAL3			NBNET_NBIOT_BAND8
#define	MQTTSN_NBIOT_BAND_TYPE			MqttSNNBIoTBandType
#define	MQTTSN_NBIOT_CGATT_TIME_S		2 * 60 + 60 * MQTTSN_NBIOT_BAND_NUM

#define	DNS_NBIOT_BAND_NUM				1
#define	DNS_NBIOT_BAND_VAL1				NBNET_NBIOT_BAND8
#define	DNS_NBIOT_BAND_VAL2				NBNET_NBIOT_BAND8
#define	DNS_NBIOT_BAND_VAL3				NBNET_NBIOT_BAND8
#define	DNS_NBIOT_BAND_TYPE				DNSNBIoTBandType
#define	DNS_NBIOT_CGATT_TIME_S			2 * 60 + 60 * DNS_NBIOT_BAND_NUM
#endif
#if NETCARRIERTYPE == NETCARRIERCHINAUNICOM
#define	COAPCDPADDR					COAPCDPADDR_CHINA_UNICOM_FORMAL
#define	COAPCDPPORT					COAPCDPPORT_CHINA_UNICOM

#define	COAP_NBIOT_BAND_NUM				2
#define	COAP_NBIOT_BAND_VAL1			NBNET_NBIOT_BAND8
#define	COAP_NBIOT_BAND_VAL2			NBNET_NBIOT_BAND3
#define	COAP_NBIOT_BAND_VAL3			NBNET_NBIOT_BAND5
#define	COAP_NBIOT_BAND_TYPE			CoAPNBIoTBandType
#define	COAP_NBIOT_CGATT_TIME_S			3 * 60 + 60 * COAP_NBIOT_BAND_NUM

#define	ONENET_NBIOT_BAND_NUM			2
#define	ONENET_NBIOT_BAND_VAL1			NBNET_NBIOT_BAND8
#define	ONENET_NBIOT_BAND_VAL2			NBNET_NBIOT_BAND3
#define	ONENET_NBIOT_BAND_VAL3			NBNET_NBIOT_BAND5
#define	ONENET_NBIOT_BAND_TYPE			OneNETNBIoTBandType
#define	ONENET_NBIOT_CGATT_TIME_S		3 * 60 + 60 * ONENET_NBIOT_BAND_NUM

#define	MQTTSN_NBIOT_BAND_NUM			2
#define	MQTTSN_NBIOT_BAND_VAL1			NBNET_NBIOT_BAND8
#define	MQTTSN_NBIOT_BAND_VAL2			NBNET_NBIOT_BAND3
#define	MQTTSN_NBIOT_BAND_VAL3			NBNET_NBIOT_BAND5
#define	MQTTSN_NBIOT_BAND_TYPE			MqttSNNBIoTBandType
#define	MQTTSN_NBIOT_CGATT_TIME_S		3 * 60 + 60 * MQTTSN_NBIOT_BAND_NUM

#define	DNS_NBIOT_BAND_NUM				2
#define	DNS_NBIOT_BAND_VAL1				NBNET_NBIOT_BAND8
#define	DNS_NBIOT_BAND_VAL2				NBNET_NBIOT_BAND3
#define	DNS_NBIOT_BAND_VAL3				NBNET_NBIOT_BAND5
#define	DNS_NBIOT_BAND_TYPE				DNSNBIoTBandType
#define	DNS_NBIOT_CGATT_TIME_S			3 * 60 + 60 * DNS_NBIOT_BAND_NUM
#endif
#if NETCARRIERTYPE == NETCARRIEROTHERUNICOM
#define	COAPCDPADDR					COAPCDPADDR_OTHER_UNICOM_FORMAL
#define	COAPCDPPORT					COAPCDPPORT_OTHER_UNICOM

#define	COAP_NBIOT_BAND_NUM				3
#define	COAP_NBIOT_BAND_VAL1			NBNET_NBIOT_BAND3
#define	COAP_NBIOT_BAND_VAL2			NBNET_NBIOT_BAND5
#define	COAP_NBIOT_BAND_VAL3			NBNET_NBIOT_BAND20
#define	COAP_NBIOT_BAND_TYPE			CoAPNBIoTBandType
#define	COAP_NBIOT_CGATT_TIME_S			2 * 60 + 60 * COAP_NBIOT_BAND_NUM

#define	ONENET_NBIOT_BAND_NUM			3
#define	ONENET_NBIOT_BAND_VAL1			NBNET_NBIOT_BAND3
#define	ONENET_NBIOT_BAND_VAL2			NBNET_NBIOT_BAND5
#define	ONENET_NBIOT_BAND_VAL3			NBNET_NBIOT_BAND20
#define	ONENET_NBIOT_BAND_TYPE			OneNETNBIoTBandType
#define	ONENET_NBIOT_CGATT_TIME_S		2 * 60 + 60 * ONENET_NBIOT_BAND_NUM

#define	MQTTSN_NBIOT_BAND_NUM			3
#define	MQTTSN_NBIOT_BAND_VAL1			NBNET_NBIOT_BAND3
#define	MQTTSN_NBIOT_BAND_VAL2			NBNET_NBIOT_BAND5
#define	MQTTSN_NBIOT_BAND_VAL3			NBNET_NBIOT_BAND20
#define	MQTTSN_NBIOT_BAND_TYPE			MqttSNNBIoTBandType
#define	MQTTSN_NBIOT_CGATT_TIME_S		2 * 60 + 60 * MQTTSN_NBIOT_BAND_NUM

#define	DNS_NBIOT_BAND_NUM				3
#define	DNS_NBIOT_BAND_VAL1				NBNET_NBIOT_BAND3
#define	DNS_NBIOT_BAND_VAL2				NBNET_NBIOT_BAND5
#define	DNS_NBIOT_BAND_VAL3				NBNET_NBIOT_BAND20
#define	DNS_NBIOT_BAND_TYPE				DNSNBIoTBandType
#define	DNS_NBIOT_CGATT_TIME_S			2 * 60 + 60 * DNS_NBIOT_BAND_NUM
#endif

#if NETPROTOCAL == NETCOAP
#define	NBIOT_MODULE_BAND_SUPPORT		COAP_NBIOT_BAND_TYPE
#endif
#if NETPROTOCAL == NETMQTTSN
#define	NBIOT_MODULE_BAND_SUPPORT		MQTTSN_NBIOT_BAND_TYPE
#endif
#if NETPROTOCAL == NETONENET
#define	NBIOT_MODULE_BAND_SUPPORT		ONENET_NBIOT_BAND_TYPE
#endif

extern bool BootUp;														//BootUp
extern bool BackUp;														//BackUp
extern bool DeviceIdleMode;												//IDLE MODE
extern bool DeviceActivedMode;											//ACTIVED MODE

#if NETPROTOCAL == NETCOAP
extern COAP_PacketShortTypeDef			CoapShortStructure;					//Coap Short Packet
extern COAP_PacketLongTypeDef				CoapLongStructure;					//Coap Long Packet
extern COAP_PacketInfoTypeDef				CoapInfoStructure;					//Coap Info Packet
extern COAP_PacketPrivateTypeDef			CoapPrivateStructure;				//Coap Private Packet
#endif

#if NETPROTOCAL == NETMQTTSN
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
extern MQTTSN_StatusBasicTypeDef			MqttSNStatusBasicStructure;			//MqttSN Basic Status Packet
extern MQTTSN_StatusExtendTypeDef			MqttSNStatusExtendStructure;			//MqttSN Extend Status Packet
extern MQTTSN_InfoWorkTypeDef				MqttSNInfoWorkStructure;				//MqttSN Work Info Packet
extern MQTTSN_InfoBasicTypeDef			MqttSNInfoBasicStructure;			//MqttSN Basic Info Packet
extern MQTTSN_InfoDynamicTypeDef			MqttSNInfoDynamicStructure;			//MqttSN Dynamic Info Packet
extern MQTTSN_InfoResponseTypeDef			MqttSNInfoResponseStructure;			//MqttSN Response Info Packet
#endif
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM
extern MQTTSN_PacketShortTypeDef			MqttSNShortStructure;				//MqttSN Short Packet
extern MQTTSN_PacketLongTypeDef			MqttSNLongStructure;				//MqttSN Long Packet
extern MQTTSN_PacketInfoTypeDef			MqttSNInfoStructure;				//MqttSN Info Packet
extern MQTTSN_PacketPrivateTypeDef			MqttSNPrivateStructure;				//MqttSN Private Packet
#endif
#endif

#if NETPROTOCAL == NETONENET
extern ONENET_PacketShortTypeDef			OneNETShortStructure;				//ONENET Short Packet
extern ONENET_PacketLongTypeDef			OneNETLongStructure;				//ONENET Long Packet
extern ONENET_PacketInfoTypeDef			OneNETInfoStructure;				//ONENET Info Packet
extern ONENET_PacketPrivateTypeDef			OneNETPrivateStructure;				//ONENET Private Packet
#endif

extern NET_NBIOT_ClientsTypeDef			NetNbiotClientHandler;				//NET NBIOT Clinet Handler
extern NBIOT_ATCmdTypeDef				NbiotATCmdHandler;					//NBIOT AT Cmd Handler
extern NBIOT_ClientsTypeDef				NbiotClientHandler;					//Coap Clinet Handler

#if NETPROTOCAL == NETCOAP
extern PCP_CoAPNetTransportTypeDef			PCPCoAPNetHandler;					//PCP Net Handler
extern PCP_ClientsTypeDef				PCPClientHandler;					//PCP Clinet Handler
#endif

#if NETPROTOCAL == NETMQTTSN
#if MQTTSN_DNS_SERVER_TYPE == MQTTSN_DNS_SERVER_ENABLE
extern DNS_SocketNetTypeDef				DNSSocketNetHandler;				//DNS Net Handler
extern DNS_ClientsTypeDef				DNSClientHandler;					//DNS Clinet Handler
#endif
extern MQTTSN_SocketNetTypeDef			MqttSNSocketNetHandler;				//MqttSN Net Handler
extern MQTTSN_ClientsTypeDef				MqttSNClientHandler;				//MqttSN Clinet Handler
extern MqttSNPCP_MqttNetTransportTypeDef	MqttSNPCPMqttNetHandler;				//MqttSN PCP Net Handler
extern MqttSNPCP_ClientsTypeDef			MqttSNPCPClientHandler;				//MqttSN PCP Clinet Handler
#endif

#if NETPROTOCAL == NETONENET
extern ONENET_LWM2MTransportTypeDef		OneNETLWM2MNetHandler;				//ONENET Net Handler
extern ONENET_ClientsTypeDef				OneNETClientHandler;				//ONENET Clinet Handler
#endif

extern NBIOT_NBandTypeDef				CoAPNBIoTBandType;					//CoAP NBIoT Band
extern NBIOT_NBandTypeDef				OneNETNBIoTBandType;				//OneNET NBIoT Band
extern NBIOT_NBandTypeDef				MqttSNNBIoTBandType;				//MqttSN NBIoT Band
extern NBIOT_NBandTypeDef				DNSNBIoTBandType;					//DNS NBIoT Band

void RadioPrintWorkinfo(void);											//Radio Print WorkInfo
void RadioPrintNetinfo(void);												//Radio Print NetInfo
void RadioPrintDeviceinfo(void);											//Radio Print DeviceInfo
void RadioPrintUpgradeinfo(void);											//Radio Print UpgradeInfo

#endif
