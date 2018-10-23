#ifndef __ONENET_CONFIG_H
#define   __ONENET_CONFIG_H

#include "sys.h"
#include "nbiotconfig.h"
#include "net_nbiot_app.h"

#define ONENET_COMMAND_TIMEOUT_SEC			30
#define ONENET_COMMAND_FAILURE_CNT			3

/* ONENET 协议栈开辟缓存大小 */
#define ONENET_BUFFER_SIZE				512
#define ONENET_DATASTACK_SIZE				1100

/* ONENET 协议标识 */
#define ONENET_PROCOTOL_HEAD				"+MIPL"
#define ONENET_PROCOTOL_DISCOVER			"+MIPLDISCOVER"
#define ONENET_PROCOTOL_OBSERVE			"+MIPLOBSERVE"
#define ONENET_PROCOTOL_READ				"+MIPLREAD"
#define ONENET_PROCOTOL_WRITE				"+MIPLWRITE"
#define ONENET_PROCOTOL_EXECUTE			"+MIPLEXECUTE"
#define ONENET_PROCOTOL_PARAMETER			"+MIPLPARAMETER"
#define ONENET_PROCOTOL_EVENT				"+MIPLEVENT"

/* ONENET LWM2M 服务地址 */
#define ONENET_LWM2MSERVER_MODE			1
#define ONENET_LWM2MSERVER_ADDR			"183.230.40.39"
#define ONENET_LWM2MSERVER_PORT			5683

/* ONENET LWM2M 应答时间 */
#define ONENET_LWM2MSERVER_ACKTIMEOUT		20

/* ONENET OBServer 自动应答选择 */
#define ONENET_OBSAUTOACK_DISABLE			0
#define ONENET_OBSAUTOACK_ENABLE			1
#define ONENET_OBSAUTOACK_TYPE			ONENET_OBSAUTOACK_ENABLE

/* ONENET 对象设置 */
#define ONENET_OBJECT_OBJID				6203												// 对象ID
#define ONENET_OBJECT_INSCOUNT			2												// 实例个数
#define ONENET_OBJECT_INSBITMAP			"11"												// 实例位使能
#define ONENET_OBJECT_ATTRCOUNT			1												// 可读写属性个数
#define ONENET_OBJECT_ACTCOUNT			0												// 可执行属性个数

/* ONENET 注册设置 */
#define ONENET_REGISTER_LIFETIME			10 * 3600											// 生命周期
#define ONENET_REGISTER_TIMEOUT			40												// 注册时间

/* ONENET 资源数据 */
#define ONENET_DISCOVER_LENGTH			4												// 资源长度
#define ONENET_DISCOVER_VALSTR			"1000"											// 资源内容
#define ONENET_DISCOVER_VALUE				 1000											// 资源内容

typedef struct ONENET_ParameterTypeDef		ONENET_ParameterTypeDef;
typedef struct ONENET_LWM2MTransportTypeDef	ONENET_LWM2MTransportTypeDef;
typedef struct ONENET_ClientsTypeDef		ONENET_ClientsTypeDef;

/* ONENET Status */
typedef enum
{
	ONENET_OK       					= 0x00,
	ONENET_ERROR    					= 0x01,
	ONENET_CMD_TIMEOUT					= 0x02,
	
	/* -General Errors- */
	ONENET_Operation_not_allowed			= 3,
	ONENET_Operation_not_supported		= 4,
	ONENET_Need_to_enter_PIN				= 5,
	ONENET_Memory_failure				= 23,
	ONENET_No_Network_Service			= 30,
	ONENET_Incorrect_parameters			= 50,
	ONENET_Command_implemented_but		= 51,
	ONENET_Command_aborted_by_user		= 52,
	ONENET_Uplink_Busy_Flow_Control		= 159,
	ONENET_ME_failure					= 300,
	ONENET_SMS_service_of_ME_reserved		= 301,
	ONENET_operation_not_allowed			= 302,
	ONENET_operation_not_supported		= 303,
	ONENET_invalid_PDU_mode_parameter		= 304,
	ONENET_invalid_text_mode_parameter		= 305,
	ONENET_SIM_not_inserted				= 310,
	ONENET_SIM_PIN_required				= 311,
	ONENET_PH_SIM_PIN_required			= 312,
	ONENET_SIM_failure					= 313,
	ONENET_SIM_busy					= 314,
	ONENET_SIM_wrong					= 315,
	ONENET_SIM_PUK_required				= 316,
	ONENET_SIM_PIN2_required				= 317,
	ONENET_SIM_PUK2_required				= 318,
	ONENET_memory_failure				= 320,
	ONENET_invalid_memory_index			= 321,
	ONENET_memory_full					= 322,
	ONENET_SMSC_address_unknown			= 330,
	ONENET_no_network_service			= 331,
	ONENET_network_timeout				= 332,
	ONENET_acknowledgement_expected		= 340,
	ONENET_unknown_error				= 500,
	
	/* -HISI specific error codes- */
	ONENET_Required_parameter_not_configured= 512,
	ONENET_TUP_not_registered			= 513,
	ONENET_AT_Internal_Error				= 514,
	ONENET_CID_is_active				= 515,
	ONENET_Incorrect_State_for_Command		= 516,
	ONENET_Cid_is_invalid				= 517,
	ONENET_CID_is_not_active				= 518,
	ONENET_Deactive_last_active_cid		= 520,
	ONENET_Cid_is_not_defined			= 521,
	ONENET_Uart_parity_error				= 522,
	ONENET_Uart_frame_error				= 523,
	ONENET_Mt_not_power_on				= 524,
	ONENET_sent_data_sequence_repeat_error	= 525,
	ONENET_at_command_abort_error			= 526,
	ONENET_command_interrupted			= 527,
	ONENET_Configuration_conflicts		= 528,
	ONENET_FOTA_is_updating				= 529,
	ONENET_Not_the_at_allocated_socket		= 530
}ONENET_StatusTypeDef;

/* ONENET Result Code */
typedef enum
{
	ONENET_Content						= 1,
	ONENET_Changed						= 2,
	ONENET_BadRequest					= 11,
	ONENET_Unauthorized					= 12,
	ONENET_NotFound					= 13,
	ONENET_MethodNotAllowed				= 14,
	ONENET_NotAcceptable				= 15
}ONENET_ResultcodeTypeDef;

/* ONENET Eventid Code */
typedef enum
{
	EVENT_BOOTSTRAP_START				= 1,
	EVENT_BOOTSTRAP_SUCCESS				= 2,
	EVENT_BOOTSTRAP_FAILED				= 3,
	EVENT_CONNECT_SUCCESS				= 4,
	EVENT_CONNECT_FAILED				= 5,
	EVENT_REG_SUCCESS					= 6,
	EVENT_REG_FAILED					= 7,
	EVENT_REG_TIMEOUT					= 8,
	EVENT_LIFETIME_TIMEOUT				= 9,
	EVENT_STATUS_HALT					= 10,
	EVENT_UPDATE_SUCCESS				= 11,
	EVENT_UPDATE_FAILED					= 12,
	EVENT_UPDATE_TIMEOUT				= 13,
	EVENT_UPDATE_NEED					= 14,
	EVENT_DEREG_DONE					= 15,
	EVENT_RESPONSE_FAILED				= 20,
	EVENT_RESPONSE_SUCCESS				= 21,
	EVENT_NOTIFY_FAILED					= 25,
	EVENT_NOTIFY_SUCCESS				= 26,
	EVENT_FIRMWARE_DOWNLOADING			= 40,
	EVENT_FIRMWARE_DOWNLOAD_FAILED		= 41,
	EVENT_FIRMWARE_DOWNLOADED			= 42,
	EVENT_FIRMWARE_UPDATING				= 43,
	EVENT_FIRMWARE_UPDATE_SUCCESS			= 44,
	EVENT_FIRMWARE_UPDATE_FAILED			= 45,
	EVENT_FIRMWARE_UPDATE_OVER			= 46,
	EVENT_FIRMWARE_DOWNLOAD_DISCONNECT		= 47,
	EVENT_FIRMWARE_ERASE_SUCCESS			= 48,
	EVENT_FIRMWARE_ERASE_FAIL			= 49
}ONENET_EventidcodeTypeDef;

/* ONENET ValueType Code */
typedef enum
{
	ONENET_String						= 1,
	ONENET_Opaque						= 2,
	ONENET_Integer						= 3,
	ONENET_Float						= 4,
	ONENET_Boolean						= 5
}ONENET_ValueTypecodeTypeDef;

/* ONENET Related URCs Code */
typedef enum
{
	ONENET_MIPLDISCOVER					= 0x01,
	ONENET_MIPLOBSERVE					= 0x02,
	ONENET_MIPLREAD					= 0x03,
	ONENET_MIPLWRITE					= 0x04,
	ONENET_MIPLEXECUTE					= 0x05,
	ONENET_MIPLPARAMETER				= 0x06,
	ONENET_MIPLEVENT					= 0x07
}ONENET_RelatedURCsCodeTypeDef;

/* ONENET Is ProcessState */
typedef enum
{
	ONENET_PROCESSSTATE_INIT				= 0x00,
	ONENET_PROCESSSTATE_SUITE			= 0x01,
	ONENET_PROCESSSTATE_OBJECT			= 0x02,
	ONENET_PROCESSSTATE_REGISTER			= 0x03,
	ONENET_PROCESSSTATE_ACTIVE			= 0x04,
	ONENET_PROCESSSTATE_SLEEP			= 0x05,
	ONENET_PROCESSSTATE_AWEAK			= 0x06,
	ONENET_PROCESSSTATE_LOST				= 0x07
}ONENET_ProcessStateTypeDef;

/* ONENET Message Parameter */
typedef struct
{
	u32								objId;											/*< Object identifier >*/
	s32								insId;											/*< The instance identifier >*/
	s32								resId;											/*< The resource identifier >*/
	u8								valueType;										/*< The value type : 1 String, 2 Opaque, 3 Integer, 4 Float, 5 Boolean >*/
	u32								len;												/*< The value length >*/
	sc8*								value;											/*< The value >*/
	s32								index;											/*< The index number of the data >*/
	s8								flag;											/*< The message indication >*/
}ONENET_MessageParaTypeDef;

/* ONENET Object Parameter */
typedef struct
{
	u32								objId;											/*< Object identifier >*/
	u16								insCount;											/*< The instance count >*/
	sc8*								insBitmap;										/*< The instance bitmap >*/
	u16								attrCount;										/*< The attribute count >*/
	u16								actCount;											/*< The action count >*/
}ONENET_ObjectParaTypeDef;

/* ONENET Observe Parameter */
typedef struct
{
	int								ref;												/*< Instance ID of OneNET communication suite >*/
	int								msgId;											/*< The message identifier of packet >*/
	char								flag;											/*< Indicates whether or not to observe >*/
	int								objId;											/*< The object identifier >*/
	int								insId;											/*< The instance identifier >*/
	int								resId;											/*< The resource identifier >*/
}ONENET_ObserveParaTypeDef;

/* ONENET Discover Parameter */
typedef struct
{
	int								ref;												/*< Instance ID of OneNET communication suite >*/
	int								msgId;											/*< The message identifier of packet >*/
	int								objId;											/*< The object identifier >*/
}ONENET_DiscoverParaTypeDef;

/* ONENET Event Parameter */
typedef struct
{
	int								ref;												/*< Instance ID of OneNET communication suite >*/
	int								evtid;											/*< Event identifier >*/
	u16								ackid;											/*< Acknowledgement identifier >*/
}ONENET_EventParaTypeDef;

/* ONENET Parameter */
struct ONENET_ParameterTypeDef
{
	char								suiteVersion[20];									// 通信套件版本
	int								suiteRefer;										// 通信套件
	ONENET_ObjectParaTypeDef				objectInfo;										// 对象信息
	ONENET_ObserveParaTypeDef			observeInfo[ONENET_OBJECT_INSCOUNT];					// 实例信息
	ONENET_DiscoverParaTypeDef			discoverInfo;										// 资源信息
	ONENET_EventParaTypeDef				eventInfo;										// 事件信息
	
};




/* ONENET Transport */
struct ONENET_LWM2MTransportTypeDef
{
	NBIOT_ClientsTypeDef*				NBIotStack;
	
	ONENET_StatusTypeDef				(*Write)(ONENET_ClientsTypeDef*, ONENET_ObserveParaTypeDef, const char*, u16, u16);
//	ONENET_StatusTypeDef				(*Read)(ONENET_LWM2MTransportTypeDef*, char*, u16*);
};

/* ONENET Clients */
struct ONENET_ClientsTypeDef
{
	unsigned char*						Sendbuf;
	unsigned char*						Recvbuf;
	size_t							Sendbuf_size;
	size_t							Recvbuf_size;
	short							Sendlen;
	short							Recvlen;
	unsigned char*						DataProcessStack;
	size_t							DataProcessStack_size;
	unsigned short						Command_Timeout_Sec;
	unsigned short						Command_Failure_Cnt;
	
	struct ONENETDictateRuningCtlTypeDef
	{
		bool							dictateEnable;
		unsigned int					dictateTimeoutSec;
		unsigned char					dictateInitFailureCnt;
		unsigned char					dictateSuiteFailureCnt;
		unsigned char					dictateObjectFailureCnt;
		unsigned char					dictateRegisterFailureCnt;
		unsigned char					dictateActiveFailureCnt;
		unsigned char					dictateSleepFailureCnt;
		unsigned char					dictateAweakFailureCnt;
		unsigned char					dictateLostFailureCnt;
		Stm32_CalculagraphTypeDef		dictateRunTime;
	}DictateRunCtl;
	
	ONENET_ParameterTypeDef				Parameter;
	ONENET_ProcessStateTypeDef			ProcessState;
	ONENET_LWM2MTransportTypeDef*			LWM2MStack;
	NET_NBIOT_ClientsTypeDef*			NetNbiotStack;
	
	Stm32_CalculagraphTypeDef			UpdateTimer;
	Stm32_CalculagraphTypeDef			ActiveTimer;
};

/* Application Programming Interface */
void OneNET_WaitforCallback(ONENET_ClientsTypeDef* pClient);																//ONENET数据等待接收回调函数
void OneNET_Client_Init(ONENET_ClientsTypeDef* pClient, ONENET_LWM2MTransportTypeDef* NetSock, NET_NBIOT_ClientsTypeDef* NetNbiotStack);	//ONENET客户端初始化

#endif /* __ONENET_CONFIG_H */
