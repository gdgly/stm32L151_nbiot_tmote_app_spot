#ifndef __PCP_CONFIG_H
#define   __PCP_CONFIG_H

#include "sys.h"
#include "nbiotconfig.h"

typedef struct PCP_CoAPNetTransportTypeDef	PCP_CoAPNetTransportTypeDef;

/* PCP Status */
typedef enum
{
	PCP_OK       						= 0x00,
	PCP_ERROR    						= 0x01,
	PCP_CMD_TIMEOUT					= 0x02,
	
	/* -General Errors- */
	PCP_Operation_not_allowed			= 3,
	PCP_Operation_not_supported			= 4,
	PCP_Need_to_enter_PIN				= 5,
	PCP_Memory_failure					= 23,
	PCP_No_Network_Service				= 30,
	PCP_Incorrect_parameters				= 50,
	PCP_Command_implemented_but			= 51,
	PCP_Command_aborted_by_user			= 52,
	PCP_Uplink_Busy_Flow_Control			= 159,
	PCP_ME_failure						= 300,
	PCP_SMS_service_of_ME_reserved		= 301,
	PCP_operation_not_allowed			= 302,
	PCP_operation_not_supported			= 303,
	PCP_invalid_PDU_mode_parameter		= 304,
	PCP_invalid_text_mode_parameter		= 305,
	PCP_SIM_not_inserted				= 310,
	PCP_SIM_PIN_required				= 311,
	PCP_PH_SIM_PIN_required				= 312,
	PCP_SIM_failure					= 313,
	PCP_SIM_busy						= 314,
	PCP_SIM_wrong						= 315,
	PCP_SIM_PUK_required				= 316,
	PCP_SIM_PIN2_required				= 317,
	PCP_SIM_PUK2_required				= 318,
	PCP_memory_failure					= 320,
	PCP_invalid_memory_index				= 321,
	PCP_memory_full					= 322,
	PCP_SMSC_address_unknown				= 330,
	PCP_no_network_service				= 331,
	PCP_network_timeout					= 332,
	PCP_acknowledgement_expected			= 340,
	PCP_unknown_error					= 500,
	
	/* -HISI specific error codes- */
	PCP_Required_parameter_not_configured	= 512,
	PCP_TUP_not_registered				= 513,
	PCP_AT_Internal_Error				= 514,
	PCP_CID_is_active					= 515,
	PCP_Incorrect_State_for_Command		= 516,
	PCP_Cid_is_invalid					= 517,
	PCP_Deactive_last_active_cid			= 520,
	PCP_Cid_is_not_defined				= 521
}PCP_StatusTypeDef;

/* PCP Message Data Structure */
typedef __packed struct
{
	unsigned short						StartX;											//起始标识	0xFFFE
	unsigned char						ProtocolType;										//版本号		0x01
	unsigned char						MessageType;										//消息码		0xXX
	unsigned short						CRCCheckCode;										//检验码		CRC16
	unsigned short						PacketDataLength;									//数据区长度
	unsigned char						pPacketData[1];									//数据区
}PCP_MessageDataTypeDef;

/* PCP CoAP Transport */
struct PCP_CoAPNetTransportTypeDef
{
	NBIOT_ClientsTypeDef*				NBIotStack;
	PCP_StatusTypeDef					(*Write)(PCP_CoAPNetTransportTypeDef*, const char*, u16);
	PCP_StatusTypeDef					(*Read)(PCP_CoAPNetTransportTypeDef*, char*, u16*);
};


























#endif /* __PCP_CONFIG_H */
