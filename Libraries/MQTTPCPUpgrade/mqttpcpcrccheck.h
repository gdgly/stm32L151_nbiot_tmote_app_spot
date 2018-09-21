#ifndef __MQTT_PCP_CRC_CHECK_H
#define   __MQTT_PCP_CRC_CHECK_H

#include "sys.h"
#include "mqttpcpconfig.h"

u16 MqttPCPCrcCheck_doCrcCheckCode(u16 regCode, u8* message, u16 length);			//计算PCPCrc校验值
u16 MqttPCPCrcCheck_getCrcCheckCode(u8* crcCheckData, u16 crcCheckLength);			//获取计算PCPCrc校验值

#endif /* __MQTT_PCP_CRC_CHECK_H */
