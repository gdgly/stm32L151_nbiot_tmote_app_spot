/**
  *********************************************************************************************************
  * @file    mqttpcpsock.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-09-21
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "mqttpcpsock.h"
#include "platform_config.h"
#include "platform_map.h"
#include "stm32l1xx_config.h"
#include "delay.h"
#include "usart.h"

/* -短整型大小端互换- */
#define MqttBigLittleSwap16(A)	((((uint16_t)(A) & 0xFF00) >> 8) | (((uint16_t)(A) & 0x00FF) << 8))
/* -长整型大小端互换- */
#define MqttBigLittleSwap32(A)	((((uint32_t)(A) & 0xFF000000) >> 24) | \
							 (((uint32_t)(A) & 0x00FF0000) >> 8 ) | \
							 (((uint32_t)(A) & 0x0000FF00) << 8 ) | \
							 (((uint32_t)(A) & 0x000000FF) << 24))

/**********************************************************************************************************
 @Function			static int MqttcheckCPUendian(void)
 @Description			MqttcheckCPUendian					: 检查CPU大小端模式
 @Input				void
 @Return				1								: 大端
					0								: 小端
**********************************************************************************************************/
static int MqttcheckCPUendian(void)
{
	union {
		unsigned long int i;
		unsigned char s[4];
	}c;
	
	c.i = 0x12345678;
	
	return (0x12 == c.s[0]);
}

/**********************************************************************************************************
 @Function			unsigned long int MqttPCPSock_htonl(unsigned long int h)
 @Description			MqttPCPSock_htonl					: 模拟htonl函数,本机字节序转网络字节序
 @Input				host	4byte
 @Return				net	4byte
**********************************************************************************************************/
unsigned long int MqttPCPSock_htonl(unsigned long int h)
{
	return MqttcheckCPUendian() ? h : MqttBigLittleSwap32(h);
}

/**********************************************************************************************************
 @Function			unsigned long int MqttPCPSock_ntohl(unsigned long int n)
 @Description			MqttPCPSock_ntohl					: 模拟ntohl函数,网络字节序转本机字节序
 @Input				net	4byte
 @Return				host	4byte
**********************************************************************************************************/
unsigned long int MqttPCPSock_ntohl(unsigned long int n)
{
	return MqttcheckCPUendian() ? n : MqttBigLittleSwap32(n);
}

/**********************************************************************************************************
 @Function			unsigned short int MqttPCPSock_htons(unsigned short int h)
 @Description			MqttPCPSock_htons					: 模拟htons函数,本机字节序转网络字节序
 @Input				host	2byte
 @Return				net	2byte
**********************************************************************************************************/
unsigned short int MqttPCPSock_htons(unsigned short int h)
{
	return MqttcheckCPUendian() ? h : MqttBigLittleSwap16(h);
}

/**********************************************************************************************************
 @Function			unsigned short int MqttPCPSock_ntohs(unsigned short int n)
 @Description			MqttPCPSock_ntohs					: 模拟ntohs函数,网络字节序转本机字节序
 @Input				net	2byte
 @Return				host	2byte
**********************************************************************************************************/
unsigned short int MqttPCPSock_ntohs(unsigned short int n)
{
	return MqttcheckCPUendian() ? n : MqttBigLittleSwap16(n);
}

/********************************************** END OF FLEE **********************************************/
