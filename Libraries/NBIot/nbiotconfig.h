#ifndef __NBIOT_CONFIG_H
#define   __NBIOT_CONFIG_H

#include "sys.h"
#include "net_nbiot_app.h"

#define NBIOT_RESET_GPIOx				GPIOB
#define NBIOT_RESET_PIN					GPIO_PIN_2
#define NBIOT_RESET_RCC_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()
#define NBIOT_RESET_RCC_GPIO_CLK_DISABLE()	__HAL_RCC_GPIOB_CLK_DISABLE()

#define NBIOT_SERIAL_TIMEOUT_MSEC			100												//串口连续接收数据包超时时间MS
#define NBIOT_COMMAND_TIMEOUT_MSEC			8000												//指令发送等待响应包超时时间MS
#define NBIOT_COMMAND_FAILURE_CNT			3												//指令执行错误尝试次数

#define NBIOT_COMMAND_TIMEOUT_AUTO			0
#define NBIOT_COMMAND_TIMEOUT_MANUAL		1
#define NBIOT_COMMAND_TIMEOUT_TYPE			NBIOT_COMMAND_TIMEOUT_AUTO

/* NBIOT 指令配置表 1编译, 0未编译 */
#define NBIOT_ATCMD_SET_HARDWAREPOWEROFF	1
#define NBIOT_ATCMD_SET_HARDWAREREBOOT		1
#define NBIOT_ATCMD_SET_SOFTWAREREBOOT		1
#define NBIOT_ATCMD_SET_CLEARSTOREDEARFCN	1
#define NBIOT_ATCMD_GET_MANUFACTURER		1
#define NBIOT_ATCMD_GET_MANUFACTURERMODEL	1
#define NBIOT_ATCMD_GET_MODULEVERSION		1
#define NBIOT_ATCMD_GET_IMEI				1
#define NBIOT_ATCMD_GET_IMEISV			1
#define NBIOT_ATCMD_GET_RSSI				1
#define NBIOT_ATCMD_GET_STATISTICSRADIO		1
#define NBIOT_ATCMD_GET_STATISTICSCELL		1
#define NBIOT_ATCMD_GET_AREACODE			1
#define NBIOT_ATCMD_GET_ICCID				1
#define NBIOT_ATCMD_GET_IMSI				1
#define NBIOT_ATCMD_GET_CGPADDR			1
#define NBIOT_ATCMD_GET_CGDCONT			1
#define NBIOT_ATCMD_GET_PDPCONTEXT			1
#define NBIOT_ATCMD_GET_DATATIME			1
#define NBIOT_ATCMD_GET_CHIPINFO			0
#define NBIOT_ATCMD_GET_PSMSTATUS			1
#define NBIOT_ATCMD_GET_CSCON				1
#define NBIOT_ATCMD_GET_NMSTATUS			1
#define NBIOT_ATCMD_SET_CGATT				1
#define NBIOT_ATCMD_GET_CGATT				1
#define NBIOT_ATCMD_SET_CFUN				1
#define NBIOT_ATCMD_GET_CFUN				1
#define NBIOT_ATCMD_SET_NNMI				1
#define NBIOT_ATCMD_GET_NNMI				1
#define NBIOT_ATCMD_SET_NSMI				1
#define NBIOT_ATCMD_GET_NSMI				1
#define NBIOT_ATCMD_SET_NBAND				1
#define NBIOT_ATCMD_GET_NBAND				1
#define NBIOT_ATCMD_TST_NBAND				1
#define NBIOT_ATCMD_SET_CMEE				1
#define NBIOT_ATCMD_GET_CMEE				1
#define NBIOT_ATCMD_SET_NCDP				1
#define NBIOT_ATCMD_GET_NCDP				1
#define NBIOT_ATCMD_SET_DNSSERVERADDRESS	0
#define NBIOT_ATCMD_GET_DNSSERVERADDRESS	0
#define NBIOT_ATCMD_SET_LWM2MLIFETIME		0
#define NBIOT_ATCMD_GET_LWM2MLIFETIME		0
#define NBIOT_ATCMD_SET_NCONFIG			1
#define NBIOT_ATCMD_GET_NCONFIG			1
#define NBIOT_ATCMD_GET_NQMGS				1
#define NBIOT_ATCMD_GET_NQMGR				1
#define NBIOT_ATCMD_SET_COAPPAYLOAD		1
#define NBIOT_ATCMD_GET_COAPPAYLOAD		1
#define NBIOT_ATCMD_GET_CONDATA			1
#define NBIOT_ATCMD_SET_CONDATA			1
#define NBIOT_ATCMD_CRT_UDPSOCKET			1
#define NBIOT_ATCMD_CLS_UDPSOCKET			1
#define NBIOT_ATCMD_SED_UDPPAYLOAD			1
#define NBIOT_ATCMD_SED_UDPPAYLOADFLAG		1
#define NBIOT_ATCMD_RED_UDPPAYLOAD			1

/* NBIOT 指令发送等待响应时间 */
#define NBIOT_COMMAND_NCSEARFCN_MSEC		600
#define NBIOT_COMMAND_CGMI_MSEC			600
#define NBIOT_COMMAND_CGMM_MSEC			600
#define NBIOT_COMMAND_CGMR_MSEC			600
#define NBIOT_COMMAND_CGSN_MSEC			600
#define NBIOT_COMMAND_CSQ_MSEC			600
#define NBIOT_COMMAND_NUESTATS_MSEC		600
#define NBIOT_COMMAND_CEREG_MSEC			1800
#define NBIOT_COMMAND_NCCID_MSEC			600
#define NBIOT_COMMAND_CIMI_MSEC			600
#define NBIOT_COMMAND_CGPADDR_MSEC			600
#define NBIOT_COMMAND_CGDCONT_MSEC			600
#define NBIOT_COMMAND_CGCONTRDP_MSEC		600
#define NBIOT_COMMAND_CCLK_MSEC			600
#define NBIOT_COMMAND_CHIPINFO_MSEC		600
#define NBIOT_COMMAND_NPSMR_MSEC			1800
#define NBIOT_COMMAND_CSCON_MSEC			600
#define NBIOT_COMMAND_NMSTATUS_MSEC		600
#define NBIOT_COMMAND_CGATT_MSEC			1500
#define NBIOT_COMMAND_CFUN_MSEC			8000
#define NBIOT_COMMAND_NNMI_MSEC			600
#define NBIOT_COMMAND_NSMI_MSEC			600
#define NBIOT_COMMAND_NBAND_MSEC			600
#define NBIOT_COMMAND_CMEE_MSEC			600
#define NBIOT_COMMAND_NCDP_MSEC			600
#define NBIOT_COMMAND_QIDNSCFG_MSEC		600
#define NBIOT_COMMAND_LWM2MLIFE_MSEC		600
#define NBIOT_COMMAND_NCONFIG_MSEC			1000
#define NBIOT_COMMAND_NQMGS_MSEC			600
#define NBIOT_COMMAND_NQMGR_MSEC			600
#define NBIOT_COMMAND_NMGS_MSEC			1000
#define NBIOT_COMMAND_NMGR_MSEC			1000
#define NBIOT_COMMAND_DATASTATUS_MSEC		800
#define NBIOT_COMMAND_DATAEX_MSEC			1000
#define NBIOT_COMMAND_NSOCR_MSEC			600
#define NBIOT_COMMAND_NSOCL_MSEC			600
#define NBIOT_COMMAND_NSOST_MSEC			1000
#define NBIOT_COMMAND_NSOSTF_MSEC			1000
#define NBIOT_COMMAND_NSORF_MSEC			1000

/* NBIOT 协议栈开辟缓存大小 */
#define NBIOT_BUFFER_SIZE				512
#define NBIOT_ATBUFFER_SIZE				1100
#define NBIOT_DATASTACK_SIZE				1100

/* NBIOT 厂商 */
#define NBIOT_MANUFACTURER_LIERDA			"Lierda"
#define NBIOT_MANUFACTURER_QUECTEL			"Quectel"

/* NCONFIG配置参数 */
#define AutoConnect						"AUTOCONNECT"										//自动注网模式
#define CrScrambling					"CR_0354_0338_SCRAMBLING"							//扰码功能
#define CrSiAvoid						"CR_0859_SI_AVOID"									//扰码功能
#define CombineAttach					"COMBINE_ATTACH"									//联合附着
#define CellReselection					"CELL_RESELECTION"									//小区重选
#define EnableBip						"ENABLE_BIP"										//短信

#define AutoConnectVal					NConfigTrue										//True
#define CrScramblingVal					NConfigTrue										//True
#define CrSiAvoidVal					NConfigTrue										//True
#define CombineAttachVal					NConfigFalse										//False
#define CellReselectionVal				NConfigTrue										//True
#define EnableBipVal					NConfigFalse										//False

/* NBIOT 事件监听器配置 */
#define NBCOAP_LISTEN_MODE_ENTER_NONE		COAP_ENTER_NONE
#define NBCOAP_LISTEN_MODE_ENTER_PARAMETER	COAP_ENTER_PARAMETER_CHECKOUT
#define NBCOAP_LISTEN_DEFAULT_BOOTMODE		NBCOAP_LISTEN_MODE_ENTER_PARAMETER						//Coap监听NB默认起始模式

#define NBCOAP_LISTEN_ENTER_PARAMETER_SEC	15												//Coap监听NB进入参数检查等待时间

#define NBCOAP_LISTEN_PARAMETER_DISABLE		0
#define NBCOAP_LISTEN_PARAMETER_ENABLE		1
#define NBCOAP_LISTEN_PARAMETER_TYPE		NBCOAP_LISTEN_PARAMETER_ENABLE						//Coap监听NB进入参数检查模式

/* NBIOT 串口波特率计算中间值 */
#define BAUDRATE_CAL_MIDDLE_NUM			16

/* NBIOT 频段配置个数 */
#define NBIOT_NBAND_NUM					3

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
typedef struct NBIOT_NBandTypeDef			NBIOT_NBandTypeDef;
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
	NETIP_CACHEDSUCCESS					= 119,											//Cached Success
	NETIP_CACHEDFAIL					= 120,											//Cached Fall
	NETIP_NEEDUPDATE					= 121,											//Need Up Date
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
	/* -Normal Errors- */
	NBIOT_OK       					= 0x00,
	NBIOT_ERROR    					= 0x01,
	NBIOT_CMD_TIMEOUT					= 0x02,
	
	/* -General Errors- */
	Operation_not_allowed				= 3,
	Operation_not_supported				= 4,
	Need_to_enter_PIN					= 5,
	Memory_failure						= 23,
	No_Network_Service					= 30,
	Incorrect_parameters				= 50,
	Command_implemented_but				= 51,
	Command_aborted_by_user				= 52,
	Uplink_Busy_Flow_Control				= 159,
	ME_failure						= 300,
	SMS_service_of_ME_reserved			= 301,
	operation_not_allowed				= 302,
	operation_not_supported				= 303,
	invalid_PDU_mode_parameter			= 304,
	invalid_text_mode_parameter			= 305,
	SIM_not_inserted					= 310,
	SIM_PIN_required					= 311,
	PH_SIM_PIN_required					= 312,
	SIM_failure						= 313,
	SIM_busy							= 314,
	SIM_wrong							= 315,
	SIM_PUK_required					= 316,
	SIM_PIN2_required					= 317,
	SIM_PUK2_required					= 318,
	memory_failure						= 320,
	invalid_memory_index				= 321,
	memory_full						= 322,
	SMSC_address_unknown				= 330,
	no_network_service					= 331,
	network_timeout					= 332,
	acknowledgement_expected				= 340,
	unknown_error						= 500,
	
	/* -HISI specific error codes- */
	Required_parameter_not_configured		= 512,
	TUP_not_registered					= 513,
	AT_Internal_Error					= 514,
	CID_is_active						= 515,
	Incorrect_State_for_Command			= 516,
	Cid_is_invalid						= 517,
	CID_is_not_active					= 518,
	Deactive_last_active_cid				= 520,
	Cid_is_not_defined					= 521,
	Uart_parity_error					= 522,
	Uart_frame_error					= 523,
	Mt_not_power_on					= 524,
	sent_data_sequence_repeat_error		= 525,
	at_command_abort_error				= 526,
	command_interrupted					= 527,
	Configuration_conflicts				= 528,
	FOTA_is_updating					= 529,
	Not_the_at_allocated_socket			= 530
}NBIOT_StatusTypeDef;

/* NBIOT BAND */
typedef enum
{
	NBand_2100MHz						= 1,
	NBand_1800MHz						= 3,
	NBand_850MHz						= 5,
	NBand_900MHz						= 8,
	NBand_800MHz						= 20,
	NBand_700MHz						= 28
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

/* NBIOT ReportError is Status */
typedef enum
{
	CMEDisable						= 0x00,
	CMEEnable							= 0x01
}NBIOT_ReportErrorTypeDef;

/* NBIOT Message Registration Status */
typedef enum
{
	UNINITIALISED						= 0x00,
	MISSING_CONFIG						= 0x01,
	INIT_FAILED						= 0x02,
	INIITIALISED						= 0x03,
	REGISTERING						= 0x04,
	REREGISTERING						= 0x05,
	REGISTERED						= 0x06,
	REREGISTERED						= 0x07,
	MO_DATA_ENABLED					= 0x08,
	NO_UE_IP							= 0x09,
	MEMORY_ERROR						= 0x0A,
	COAP_ERROR						= 0x0B,
	MSG_SEND_FAILED					= 0x0C,
	REJECTED_BY_SERVER					= 0x0D,
	TIMEOUT_AND_RETRYING				= 0x0E,
	TIMEOUT_AND_FAILED					= 0x0F
}NBIOT_MessageRegistrationStatusTypeDef;

/* NBIOT PSM Status */
typedef enum
{
	NormalMode						= 0x00,
	PowerSavingMode					= 0x01
}NBIOT_PSMStatusTypeDef;

/* NBIOT Connect Status */
typedef enum
{
	IdleMode							= 0x00,
	ConnectedMode						= 0x01
}NBIOT_ConnectStatusTypeDef;

/* NBIOT CON Status */
typedef enum
{
	Unsent							= 0x00,
	WaitResponse						= 0x01,
	SendFail							= 0x02,
	SendTimeout						= 0x03,
	SendSussess						= 0x04,
	RecvRST							= 0x05
}NBIOT_CONDataStatusTypeDef;

/* NBIOT Dictate Event */
typedef enum
{
	STOP_MODE							= 0x00,											//停止模式
	HARDWARE_REBOOT					= 0x01,											//硬件重启
	REPORT_ERROE						= 0x02,											//错误输出
	MODULE_CHECK						= 0x03,											//模块检查
	PARAMETER_CONFIG					= 0x04,											//参数配置
	ICCID_CHECK						= 0x05,											//检测SIM卡
	MISC_EQUIP_CONFIG					= 0x06,											//其他配置
	ATTACH_CHECK						= 0x07,											//注网检查
	ATTACH_EXECUTE						= 0x08,											//注网进行
	ATTACH_INQUIRE						= 0x09,											//注网查询
	PARAMETER_CHECKOUT					= 0x0A,											//参数检出
	MINIMUM_FUNCTIONALITY				= 0x0B,											//最小功能
	FULL_FUNCTIONALITY					= 0x0C,											//完整功能
	CLEAR_STORED_EARFCN					= 0x0D,											//清除小区频点
	CDP_SERVER_CHECK					= 0x0E,											//CDP服务器查询
	CDP_SERVER_CONFIG					= 0x0F,											//CDP服务器配置
	NBAND_MODE_CHECK					= 0x10,											//NBAND模式查询
	NBAND_MODE_CONFIG					= 0x11,											//NBAND模式配置
	SEND_DATA							= 0x12,											//发送数据
	RECV_DATA							= 0x13,											//接收数据
	SEND_DATA_RA_NORMAL					= 0x14,											//发送数据RA_NORMAL
	RECV_DATA_RA_NORMAL					= 0x15,											//接收数据RA_NORMAL
	EXECUT_DOWNLINK_DATA				= 0x16,											//执行下行数据
	MQTTSN_PROCESS_STACK				= 0x17,											//MQTTSN执行栈
	DNS_PROCESS_STACK					= 0x18,											//DNS执行栈
	ONENET_PROCESS_STACK				= 0x19,											//ONENET执行栈
	LISTEN_RUN_CTL						= 0x1A											//监听运行控制器
}NBIOT_DictateEventTypeDef;

/* NBIOT Listen Event */
typedef enum
{
	COAP_ENTER_NONE					= 0x00,											//无监听
	COAP_ENTER_PARAMETER_CHECKOUT			= 0x01											//进入NBIOT运行信息监听
}NBIOT_ListenEventTypeDef;

/* NBIOT CDP Server */
typedef __packed struct NBIOT_ServerAddrTypeDef
{
	__packed union
	{
		unsigned char					ip8[4];
		unsigned int					ip32;
	}ip;
	unsigned short						port;
}NBIOT_ServerAddr;

/* NBIOT CDP Server Address */
struct NBIOT_CDPServerTypeDef
{
	unsigned char						CDPServerHost[16];
	unsigned short						CDPServerPort;
};

/* NBIOT NBAND */
struct NBIOT_NBandTypeDef
{
	unsigned char						NBandNum;
	NBIOT_BandTypeDef					NBandVal[NBIOT_NBAND_NUM];
};

/* NBIOT Parameter */
struct NBIOT_ParameterTypeDef
{
#if NBIOT_ATCMD_GET_MANUFACTURER
	char								manufacturer[10];									//厂商名
#endif
#if NBIOT_ATCMD_GET_MANUFACTURERMODEL
	char								manufacturermode[20];								//厂商型号
#endif
#if NBIOT_ATCMD_GET_MODULEVERSION
	char								modelversion[30];									//模块软件版本
#endif
#if NBIOT_ATCMD_GET_IMEI
	char								imei[20];											//IMEI
#endif
#if NBIOT_ATCMD_GET_IMEISV
	char								imeisv[20];										//IMEISV
#endif
#if NBIOT_ATCMD_GET_ICCID
	char								iccid[25];										//运营商卡号
#endif
#if NBIOT_ATCMD_GET_IMSI
	char								imsi[20];											//IMSI
#endif
	char								cgpaddr[20];										//核心网地址
	char								cgdcontPDPType[10];									//核心网类型
	char								cgdcontAPN[15];									//核心网名称
	char								cgcontrdpAPN[50];									//核心网名称
	bool								bandsupport;										//band支持
	int								rssi;											//信号质量
#if NBIOT_ATCMD_GET_LWM2MLIFETIME
	unsigned int						lwm2mlifetime;										//LWM2MLifetime
#endif
	
#if NBIOT_ATCMD_GET_AREACODE
	struct NetworkRegistrationStatusTypeDef {
		unsigned short int				tac;												//跟踪区域代码
		unsigned int					cellID;											//全球唯一基站标识ID
	}networkRegStatus;
#endif
	
#if NBIOT_ATCMD_GET_STATISTICSRADIO
	struct StatisticsRADIOTypeDef {
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
#endif
	
#if NBIOT_ATCMD_GET_STATISTICSCELL
	struct StatisticsCELLTypeDef {
		int							earfcn;											//基站频点
		int							physicalcellID;									//小区物理ID
		int							primarycell;										//1标识当前服务小区
		int							rsrp;											//参考信号接收功率
		int							rsrq;											//参考信号接收质量
		int							rssi;											//信号质量
		int							snr;												//信噪比
	}statisticsCELL;
#endif
	
#if NBIOT_ATCMD_GET_DATATIME
	struct DataTimeTypeDef {
		int							year;
		int							month;
		int							day;
		int							hour;
		int							min;
		int							sec;
		unsigned int					localstamp;
		unsigned char					errcount;
	}dataTime;
#endif
	
#if NBIOT_ATCMD_GET_NQMGS
	struct COAPSendMessage {
		int							pending;
		int							sent;
		int							error;
	}coapSendMessage;
#endif
	
#if NBIOT_ATCMD_GET_NQMGR
	struct COAPReadMessage {
		int							buffered;
		int							received;
		int							dropped;
	}coapReadMessage;
#endif
	
#if NBIOT_ATCMD_GET_NCONFIG
	struct NConfigTypeDef {
		NBIOT_NConfigTypeDef			autoConnect;
		NBIOT_NConfigTypeDef			crScrambling;
		NBIOT_NConfigTypeDef			crSiAvoid;
		NBIOT_NConfigTypeDef			combineAttach;
		NBIOT_NConfigTypeDef			cellReselection;
		NBIOT_NConfigTypeDef			enableBip;
	}nconfig;
#endif
	
#if NBIOT_ATCMD_GET_DNSSERVERADDRESS
	struct QIDNSCfgTypeDef {
		unsigned char					PrimaryDns[16];
		unsigned char					SecondaryDns[16];
	}qidnscfg;
#endif
	
#if NBIOT_ATCMD_GET_CHIPINFO
	struct ChipInfoTypeDef {
		unsigned int					temp;
		unsigned int					vbat;
	}qchipinfo;
#endif
	
	NBIOT_CONDataStatusTypeDef			condatastate;
	NBIOT_PSMStatusTypeDef				psmstate;
	NBIOT_ConnectStatusTypeDef			connectedstate;
	NBIOT_NetstateTypeDef				netstate;
	NBIOT_FunctionalityTypeDef			functionality;
	NBIOT_OpenOrCloseFuncTypeDef			nnmistate;
	NBIOT_OpenOrCloseFuncTypeDef			nsmistate;
	NBIOT_NBandTypeDef					band;
	NBIOT_ReportErrorTypeDef				cmeestate;
	NBIOT_MessageRegistrationStatusTypeDef	messageregistration;
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
	Stm32_CalculagraphTypeDef			SerialWaitTime;
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
	
	unsigned int						CoapConnectTimeSec;
	unsigned int						CoapIdleTimeSec;
	Stm32_EventRunningTimeTypeDef			ConnectTimeMS;
	Stm32_EventRunningTimeTypeDef			IdleTimeMS;
	
	/* 事件运行控制器 */
	struct NBIOTDictateRuningCtlTypeDef
	{
		bool							dictateCoapRANormalEnable;
		bool							dictateEnable;
		unsigned int					dictateTimeoutSec;
		unsigned char					dictateRebootFailureCnt;
		unsigned char					dictateReportErrorFailureCnt;
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
		unsigned char					dictateClearStoredEARFCNFailureCnt;
		unsigned char					dictateCDPServerCheckFailureCnt;
		unsigned char					dictateCDPServerConfigFailureCnt;
		unsigned char					dictateNbandModeCheckFailureCnt;
		unsigned char					dictateNbandModeConfigFailureCnt;
		unsigned char					dictateSendDataFailureCnt;
		unsigned char					dictateRecvDataFailureCnt;
		unsigned char					dictateSendDataRANormalFailureCnt;
		unsigned char					dictateRecvDataRANormalFailureCnt;
		unsigned char					dictateListenRunCtlFailureCnt;
		Stm32_CalculagraphTypeDef		dictateRunTime;
		NBIOT_DictateEventTypeDef		dictateEvent;
	}DictateRunCtl;
	
	/* 事件运行监听器 */
	struct COAPListenRuningCtlTypeDef
	{
#if NBCOAP_LISTEN_PARAMETER_TYPE == NBCOAP_LISTEN_PARAMETER_ENABLE
		struct COAPListenEnterParameterTypeDef
		{
			bool						listenEnable;
			bool						listenStatus;
			unsigned int				listenTimereachSec;
			Stm32_CalculagraphTypeDef	listenRunTime;
			
			struct EventCtlParameterTypedef
			{
				bool						eventEnable;
				unsigned int				eventTimeoutSec;
				unsigned char				eventFailureCnt;
				Stm32_CalculagraphTypeDef	eventRunTime;
			}EventCtl;
		}ListenEnterParameter;
#endif
		NBIOT_ListenEventTypeDef			listenEvent;
	}ListenRunCtl;
	
	bool								Registered;
	bool								NetStateIdentification;
	bool								ClearStoredEARFCN;
	NBIOT_ParameterTypeDef				Parameter;
	NBIOT_ATCmdTypeDef*					ATCmdStack;
	NET_NBIOT_ClientsTypeDef*			NetNbiotStack;
};

extern NBIOT_BaudrateTypeDef NBIOTBaudRate;;																		//NBIOT波特率计算

void NBIOT_Client_Init(NBIOT_ClientsTypeDef* pClient, NBIOT_ATCmdTypeDef* ATCmdStack, NET_NBIOT_ClientsTypeDef* NetNbiotStack);	//NBIOT客户端初始化

void NBIOT_COAP_RA_NORMAL_SET_STATE(NBIOT_ClientsTypeDef* pClient, bool enable);										//设置COAP发送RA包或Normal包状态
bool NBIOT_COAP_RA_NORMAL_GET_STATE(NBIOT_ClientsTypeDef* pClient);													//读取COAP发送RA包或Normal包状态

#endif /* __NBIOT_CONFIG_H */
