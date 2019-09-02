#ifndef __BSP_BM1422AGMV_H
#define   __BSP_BM1422AGMV_H

#include "sys.h"

#define BM1422_DRDY_GPIOx						GPIOA
#define BM1422_DRDY_PIN							GPIO_PIN_11
#define BM1422_DRDY_RCC_GPIO_CLK_ENABLE()			__HAL_RCC_GPIOA_CLK_ENABLE()
#define BM1422_DRDY_RCC_GPIO_CLK_DISABLE()			__HAL_RCC_GPIOA_CLK_DISABLE()
#define BM1422_DRDY_READ()						HAL_GPIO_ReadPin(QMC_DRDY_GPIOx, QMC_DRDY_PIN)

#define BM1422_DRDY_IRQn							EXTI15_10_IRQn								//DRDY中断线
#define BM1422_DRDY_EXIT							0										//DRDY中断使能

#define BM1422AGMV_SLAVE_ADDRESS_W					0x1C										//I2C从机地址写
#define BM1422AGMV_SLAVE_ADDRESS_R					0x1D										//I2C从机地址读

/****** BM1422AGMV Register Map ******/
#define BM1422_INFORMATION_RO_L					0x0D										//BM1422AGMV Info 信息低8位 0x01
#define BM1422_INFORMATION_RO_H					0x0E										//BM1422AGMV Info 信息高8位 0x01

#define BM1422_WIA_RO							0x0F										//BM1422AGMV Who I am 设备ID 0x41

#define BM1422_DATA_OUT_X_RO_L					0x10										//BM1422AGMV X轴数据低8位
#define BM1422_DATA_OUT_X_RO_H					0x11										//BM1422AGMV X轴数据高8位
#define BM1422_DATA_OUT_Y_RO_L					0x12										//BM1422AGMV Y轴数据低8位
#define BM1422_DATA_OUT_Y_RO_H					0x13										//BM1422AGMV Y轴数据高8位
#define BM1422_DATA_OUT_Z_RO_L					0x14										//BM1422AGMV Z轴数据低8位
#define BM1422_DATA_OUT_Z_RO_H					0x15										//BM1422AGMV Z轴数据高8位























/*----- BM1422AGMV Register Map -----*/
















#endif /* __BSP_BM1422AGMV_H */
