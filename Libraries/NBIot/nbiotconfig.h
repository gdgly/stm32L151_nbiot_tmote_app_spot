#ifndef __NBIOT_CONFIG_H
#define   __NBIOT_CONFIG_H

#include "sys.h"
#include "net_nbiot_app.h"

#define NBIOT_RESET_GPIOx				GPIOB
#define NBIOT_RESET_PIN					GPIO_PIN_2
#define NBIOT_RESET_RCC_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()
#define NBIOT_RESET_RCC_GPIO_CLK_DISABLE()	__HAL_RCC_GPIOB_CLK_DISABLE()

#define NBIOT_COMMAND_TIMEOUT_MSEC			8000
#define NBIOT_COMMAND_FAILURE_CNT			3

/* NBIOT 协议栈开辟缓存大小 */
#define NBIOT_BUFFER_SIZE				512
#define NBIOT_ATBUFFER_SIZE				1024
#define NBIOT_DATASTACK_SIZE				1024

/* NCONFIG配置参数 */
#define AutoConnect						"AUTOCONNECT"
#define CrScrambling					"CR_0354_0338_SCRAMBLING"
#define CrSiAvoid						"CR_0859_SI_AVOID"
#define CombineAttach					"COMBINE_ATTACH"
#define CellReselection					"CELL_RESELECTION"
#define EnableBip						"ENABLE_BIP"

/* NBIOT 串口波特率计算中间值 */
#define BAUDRATE_CAL_MIDDLE_NUM			16

/* 下行数据协议值偏移地址 */
#define TCLOD_MSGTYPE_OFFSET				4
#define TCLOD_MSGVERSION_OFFSET			5
#define TCLOD_MSGENCRYPT_OFFSET			6
#define TCLOD_MSGID_OFFSET				7
#define TCLOD_DATA_OFFSET				8

/* 下行数据协议值 */
#define TCLOD_MSGTYPE					'1'
#define TCLOD_MSGVERSION					'1'
#define TCLOD_MSGENCRYPT					'0'

#define TCLOD_MAGIC_NUM					9

typedef struct NBIOT_CDPServerTypeDef		NBIOT_CDPServerTypeDef;
typedef struct NBIOT_ParameterTypeDef		NBIOT_ParameterTypeDef;
typedef struct NBIOT_ATCmdTypeDef			NBIOT_ATCmdTypeDef;
typedef struct NBIOT_ClientsTypeDef		NBIOT_ClientsTypeDef;

/* NBIOT NET MSGID */
typedef enum
{
	TCLOD_LOGIN_REQ					= '0',
	TCLOD_LOGIN_RSP					= '1',
	TCLOD_LOGOUT_REQ					= '2',
	TCLOD_LOGOUT_RSP					= '3',
	
	TCLOD_KEEPALIVE_REQ					= '4',
	TCLOD_KEEPALIVE_RSP					= '5',
	TCLOD_SYSINFO_REQ					= '6',
	TCLOD_SYSINFO_RSP					= '7',
	TCLOD_CONFIG_SET					= 'S',
	TCLOD_CONFIG_SET_RSP				= 's',
	TCLOD_CONFIG_GET					= 'G',
	TCLOD_CONFIG_GET_RSP				= 'g',
	
	TCLOD_MODIFYUSER_REQ				= 'c',
	TCLOD_MODIFYUSER_RSP				= 'd',
	TCLOD_MODIFYPASSWORD_REQ				= 'e',
	TCLOD_MODIFYPASSWORD_RSP				= 'f',
	
	TCLOD_CLOUD_VERSION_REQ				= 'g',
	TCLOD_CLOUD_VERSION_RSP				= 'i',
	TCLOD_CLOUD_UPGRADE_START_REQ			= 'j',
	TCLOD_CLOUD_UPGRADE_START_RSP			= 'k',
	TCLOD_CLOUD_UPGRADE_STOP_REQ			= 'l',
	TCLOD_CLOUD_UPGRADE_STOP_RSP			= 'm',
	TCLOD_CLOUD_UPGRADE_PROGRESS			= 'n',
	TCLOD_CLOUD_UPGRADE_DATA				= 'U',
	TCLOD_CLOUD_UPGRADE_DATA_RSP			= 'u',
}NBIOT_MsgIDTypeDef;

/* NBIOT NET RET CODE */
typedef enum
{
	NETIP_OK							= 100,											//正常
	NETIP_UNKNOWNERROR					= 101,											//未知错误
	NETIP_NOTSUPPORT					= 102,											//版本不支持
	NETIP_NOTVALID						= 103,											//非法请求
	NETIP_LOGINED						= 104,											//该用户已经登录
	NETIP_UNLOGINED					= 105,											//该用户未登录
	NETIP_USERORPWDERROR				= 106,											//用户名密码错误
	NETIP_NOPOWER						= 107,											//无权限
	NETIP_TIMEOUT						= 108,											//超时
	NETIP_NOTFOUND						= 109,											//查找失败,没有找到对应文件
	NETIP_FOUND						= 110,											//查找成功,返回全部文件
	NETIP_FOUNDPART					= 111,											//查找成功,返回部分文件
	NETIP_USEREXIST					= 112,											//该用户已经存在
	NETIP_USENOTEXIST					= 113,											//该用户不存在
	NETIP_GROUPEXIST					= 114,											//该用户组已经存在
	NETIP_GROUPNOTEXIST					= 115,											//该用户组不存在
	NETIP_NOTLINKEDIN					= 116,											//device has not linked in the gateway
	NETIP_OFFLINE						= 117,											//device has linked in the gateway,but is offline
	NETIP_NOTMACHED					= 118,											//device has linked in the gateway,but isn't offline
	NETIP_CACHEDSUCCESS					= 119,											//
	NETIP_CACHEDFAIL					= 120,											//
	NETIP_NEEDUPDATE					= 121,											//
	NETIP_ERRORPARAM					= 122,											//错误参数
	NETIP_NEEDREBOOT					= 150,											//成功,设备需要重启
	NET_OPERATION_UPGRADE_ALAREADY_START	= 511,											//重发开启
	NET_OPERATION_UPGRADE_NOT_START		= 512,											//未开始升级
	NET_OPERATION_UPGRADE_FAILED			= 514,											//操作失败
	NET_OPERATION_UPGRADE_FAILED_BUSY		= 516,											//设备忙或服务器忙
	NET_OPERATION_UPGRADE_NO_POWER		= 517											//该升级由其他连接开启,无法停止
}NBIOT_RetCodeTypeDef;

/* NBIOT Status */
typedef enum
{
	NBIOT_OK       					= 0x00,
	NBIOT_ERROR    					= 0x01,
	NBIOT_CMD_TIMEOUT					= 0x02
}NBIOT_StatusTypeDef;

/* NBIOT BAND */
typedef enum
{
	ChinaTelecom						= 5,
	ChinaMobile						= 8,
	ChinaUnicom						= 20
}NBIOT_BandTypeDef;

/* NBIOT Detach or Attach Netstate */
typedef enum
{
	Detach							= 0x00,
	Attach							= 0x01
}NBIOT_NetstateTypeDef;

/* NBIOT MinFunc of FullFunc Functionality */
typedef enum
{
	MinFunc							= 0x00,
	FullFunc							= 0x01
}NBIOT_FunctionalityTypeDef;

/* NBIOT Function is Open? */
typedef enum
{
	CloseFunc							= 0x00,
	OpenFunc							= 0x01
}NBIOT_OpenOrCloseFuncTypeDef;

/* NBIOT NConfig is True? */
typedef enum
{
	NConfigFalse						= 0x00,
	NConfigTrue						= 0x01
}NBIOT_NConfigTypeDef;

/* NBIOT PSM Status */
typedef enum
{
	NormalMode						= 0x00,
	PowerSavingMode					= 0x01
}NBIOT_PSMStatusTypeDef;

/* NBIOT Dictate Event */
typedef enum
{
	STOP_MODE							= 0x00,											//停止模式
	HARDWARE_REBOOT					= 0x01,											//硬件重启
	MODULE_CHECK						= 0x02,											//模块检查
	PARAMETER_CONFIG					= 0x03,											//参数配置
	ICCID_CHECK						= 0x04,											//检测SIM卡
	MISC_EQUIP_CONFIG					= 0x05,											//其他配置
	ATTACH_CHECK						= 0x06,											//注网检查
	ATTACH_EXECUTE						= 0x07,											//注网进行
	ATTACH_INQUIRE						= 0x08,											//注网查询
	PARAMETER_CHECKOUT					= 0x09,											//参数检出
	MINIMUM_FUNCTIONALITY				= 0x0A,											//最小功能
	FULL_FUNCTIONALITY					= 0x0B,											//完整功能
	CDP_SERVER_CHECK					= 0x0C,											//CDP服务器查询
	CDP_SERVER_CONFIG					= 0x0D,											//CDP服务器配置
	SEND_DATA							= 0x0E,											//发送数据
	RECV_DATA							= 0x0F,											//接收数据
	EXECUT_DOWNLINK_DATA				= 0x10,											//执行下行数据
	MQTTSN_PROCESS_STACK				= 0x11,											//MQTTSN执行栈
	DNS_PROCESS_STACK					= 0x12											//DNS执行栈
}NBIOT_DictateEventTypeDef;

/* NBIOT CDP Server Address */
struct NBIOT_CDPServerTypeDef
{
	unsigned char						CDPServerHost[16];
	unsigned short						CDPServerPort;
};

/* NBIOT Parameter */
struct NBIOT_ParameterTypeDef
{
	char								manufacturer[10];									//厂商名
	char								manufacturermode[20];								//厂商型号
	char								modelversion[30];									//模块软件版本
	char								imei[20];											//IMEI
	char								imeisv[20];										//IMEISV
	char								iccid[25];										//运营商卡号
	char								imsi[20];											//IMSI
	char								cgpaddr[20];										//核心网地址
	char								cgdcont[30];										//核心网名称
	int								rssi;											//信号质量
	
	struct NetworkRegistrationStatusTypeDef
	{
		unsigned short int				tac;												//跟踪区域代码
		unsigned int					cellID;											//全球唯一基站标识ID
	}networkRegStatus;
	
	struct StatisticsRADIOTypeDef
	{
		int							Signalpower;										//信号功率
		int							Totalpower;										//总功率
		int							TXpower;											//发送功率
		unsigned int					TXtime;											//自上次重启后的总发送时间
		unsigned int					RXtime;											//自上次重启后的总接收时间
		unsigned int					CellID;											//全球唯一基站标识ID
		int							ECL;												//覆盖等级
		int							SNR;												//信噪比
		int							EARFCN;											//基站频点
		int							PCI;												//PCI
		int							RSRQ;											//参考信号接收质量
	}statisticsRADIO;
	
	struct StatisticsCELLTypeDef
	{
		int							earfcn;											//基站频点
		int							physicalcellID;									//小区物理ID
		int							primarycell;										//1标识当前服务小区
		int							rsrp;											//参考信号接收功率
		int							rsrq;											//参考信号接收质量
		int							rssi;											//信号质量
		int							snr;												//信噪比
	}statisticsCELL;
	
	struct DataTimeTypeDef
	{
		int							year;
		int							month;
		int							day;
		int							hour;
		int							min;
		int							sec;
		unsigned int					localstamp;
	}dataTime;
	
	struct COAPSendMessage
	{
		int							pending;
		int							sent;
		int							error;
	}coapSendMessage;
	
	struct COAPReadMessage
	{
		int							buffered;
		int							received;
		int							dropped;
	}coapReadMessage;
	
	struct NConfigTypeDef
	{
		NBIOT_NConfigTypeDef			autoConnect;
		NBIOT_NConfigTypeDef			crScrambling;
		NBIOT_NConfigTypeDef			crSiAvoid;
		NBIOT_NConfigTypeDef			combineAttach;
		NBIOT_NConfigTypeDef			cellReselection;
		NBIOT_NConfigTypeDef			enableBip;
	}nconfig;
	
	NBIOT_PSMStatusTypeDef				psmstate;
	NBIOT_NetstateTypeDef				netstate;
	NBIOT_FunctionalityTypeDef			functionality;
	NBIOT_OpenOrCloseFuncTypeDef			nnmistate;
	NBIOT_OpenOrCloseFuncTypeDef			nsmistate;
	NBIOT_BandTypeDef					band;
	NBIOT_CDPServerTypeDef				cdpserver;
};

/* NBIOT BaudrateCal */
typedef __packed struct
{
	unsigned int						StartMs;
	unsigned int						StartClock;
	unsigned int						EndMs;
	unsigned int						EndClock;
	unsigned int						OverFlow;
	unsigned int						MiddleMs;
	unsigned int						MiddleClock;
	unsigned char						MiddleNum;
	unsigned char						TotalNum;
}NBIOT_BaudrateCalTypeDef;

/* NBIOT Baudrate */
typedef struct
{
	bool								EnBaudRateState;
	unsigned int						Baud;
	NBIOT_BaudrateCalTypeDef				NBIOTBaudRateNow;
	NBIOT_BaudrateCalTypeDef				NBIOTBaudRateCal;
}NBIOT_BaudrateTypeDef;

/* NBIOT ATCmd */
struct NBIOT_ATCmdTypeDef
{
	unsigned char*						ATSendbuf;
	unsigned char*						ATRecvbuf;
	size_t							ATSendbuf_size;
	size_t							ATRecvbuf_size;
	short							ATSendlen;
	short							ATRecvlen;
	char*							ATack;
	char*							ATNack;
	Stm32_CalculagraphTypeDef			CmdWaitTime;
	NBIOT_StatusTypeDef					(*Write)(NBIOT_ATCmdTypeDef*);
	NBIOT_StatusTypeDef 				(*Read)(NBIOT_ATCmdTypeDef*);
};

/* NBIOT Clients */
struct NBIOT_ClientsTypeDef
{
	unsigned char*						Sendbuf;
	unsigned char*						Recvbuf;
	size_t							Sendbuf_size;
	size_t							Recvbuf_size;
	short							Sendlen;
	short							Recvlen;
	unsigned char*						DataProcessStack;
	size_t							DataProcessStack_size;
	unsigned int						Command_Timeout_Msec;
	unsigned int						Command_Failure_Cnt;
	
	struct NBIOTDictateRuningCtlTypeDef
	{
		bool							dictateEnable;
		unsigned int					dictateTimeoutSec;
		unsigned char					dictateRebootFailureCnt;
		unsigned char					dictateModuleCheckFailureCnt;
		unsigned char					dictateParameterConfigFailureCnt;
		unsigned char					dictateSimICCIDCheckFailureCnt;
		unsigned char					dictateMiscEquipConfigFailureCnt;
		unsigned char					dictateAttachCheckFailureCnt;
		unsigned char					dictateAttachExecuteFailureCnt;
		unsigned char					dictateAttachInquireFailureCnt;
		unsigned char					dictatePatameterCheckOutFailureCnt;
		unsigned char					dictateFullFunctionalityFailureCnt;
		unsigned char					dictateMinimumFunctionalityFailureCnt;
		unsigned char					dictateCDPServerCheckFailureCnt;
		unsigned char					dictateCDPServerConfigFailureCnt;
		unsigned char					dictateSendDataFailureCnt;
		unsigned char					dictateRecvDataFailureCnt;
		Stm32_CalculagraphTypeDef		dictateRunTime;
		NBIOT_DictateEventTypeDef		dictateEvent;
	}DictateRunCtl;
	
	NBIOT_ParameterTypeDef				Parameter;
	NBIOT_ATCmdTypeDef*					ATCmdStack;
	NET_NBIOT_ClientsTypeDef*			NetNbiotStack;
};

extern NBIOT_BaudrateTypeDef NBIOTBaudRate;;																		//NBIOT波特率计算

void NBIOT_Client_Init(NBIOT_ClientsTypeDef* pClient, NBIOT_ATCmdTypeDef* ATCmdStack, NET_NBIOT_ClientsTypeDef* NetNbiotStack);	//NBIOT客户端初始化

#endif
