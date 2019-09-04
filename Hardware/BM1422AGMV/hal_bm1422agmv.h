#ifndef __BSP_BM1422AGMV_H
#define   __BSP_BM1422AGMV_H

#include "sys.h"

#define BM1422_DRDY_GPIOx						GPIOA
#define BM1422_DRDY_PIN							GPIO_PIN_11
#define BM1422_DRDY_RCC_GPIO_CLK_ENABLE()			__HAL_RCC_GPIOA_CLK_ENABLE()
#define BM1422_DRDY_RCC_GPIO_CLK_DISABLE()			__HAL_RCC_GPIOA_CLK_DISABLE()
#define BM1422_DRDY_READ()						HAL_GPIO_ReadPin(BM1422_DRDY_GPIOx, BM1422_DRDY_PIN)

#define BM1422_DRDY_IRQn							EXTI15_10_IRQn								//DRDY中断线
#define BM1422_DRDY_EXIT							0										//DRDY中断使能

#define BM1422AGMV_SLAVE_ADDRESS_W					(0x1C)									//I2C从机地址写
#define BM1422AGMV_SLAVE_ADDRESS_R					(0x1D)									//I2C从机地址读

/****** BM1422AGMV Register Map ******/
#define BM1422_INFORMATION_RO_L					(0x0D)									//BM1422AGMV Info 信息低8位 0x01
#define BM1422_INFORMATION_RO_H					(0x0E)									//BM1422AGMV Info 信息高8位 0x01

#define BM1422_WIA_RO							(0x0F)									//BM1422AGMV Who I am 设备ID 0x41

#define BM1422_DATA_OUT_X_RO_L					(0x10)									//BM1422AGMV X轴数据低8位
#define BM1422_DATA_OUT_X_RO_H					(0x11)									//BM1422AGMV X轴数据高8位
#define BM1422_DATA_OUT_Y_RO_L					(0x12)									//BM1422AGMV Y轴数据低8位
#define BM1422_DATA_OUT_Y_RO_H					(0x13)									//BM1422AGMV Y轴数据高8位
#define BM1422_DATA_OUT_Z_RO_L					(0x14)									//BM1422AGMV Z轴数据低8位
#define BM1422_DATA_OUT_Z_RO_H					(0x15)									//BM1422AGMV Z轴数据高8位

#define BM1422_STA1_RO							(0x18)									//BM1422AGMV 状态寄存器1
#define BM1422_STA1_RD_DRDY						(1<<6)									//BM1422AGMV 0: Not ready NG 1: Ready OK

#define BM1422_CNTL1_RW							(0x1B)									//BM1422AGMV 控制寄存器1
#define BM1422_CNTL1_FS1_MODE_CONTINUOUS			(0<<1)									//BM1422AGMV 连续测量模式
#define BM1422_CNTL1_FS1_MODE_SINGLE				(1<<1)									//BM1422AGMV 单次测量模式
#define BM1422_CNTL1_ODR_RATES_10HZ				(0<<3)									//BM1422AGMV 数据输出速率10Hz
#define BM1422_CNTL1_ODR_RATES_20HZ				(2<<3)									//BM1422AGMV 数据输出速率20Hz
#define BM1422_CNTL1_ODR_RATES_100HZ				(1<<3)									//BM1422AGMV 数据输出速率100Hz
#define BM1422_CNTL1_ODR_RATES_1000HZ				(3<<3)									//BM1422AGMV 数据输出速率1000Hz
#define BM1422_CNTL1_RST_SOFT_REST_RELEASE			(0<<5)									//BM1422AGMV 复位释放
#define BM1422_CNTL1_RST_SOFT_REST_ENABLE			(1<<5)									//BM1422AGMV 复位使能
#define BM1422_CNTL1_OUT_12BIT					(0<<6)									//BM1422AGMV 输出数据12Bit
#define BM1422_CNTL1_OUT_14BIT					(1<<6)									//BM1422AGMV 输出数据14Bit
#define BM1422_CNTL1_PC1_POWERDOWN					(0<<7)									//BM1422AGMV 断电
#define BM1422_CNTL1_PC1_ACTIVE					(1<<7)									//BM1422AGMV 工作

#define BM1422_CNTL2_RW							(0x1C)									//BM1422AGMV 控制寄存器2
#define BM1422_CNTL2_DRDY_LOW_ACTIVE				(0<<2)									//BM1422AGMV DRDY低电平有效
#define BM1422_CNTL2_DRDY_HIGH_ACTIVE				(1<<2)									//BM1422AGMV DRDY高电平有效
#define BM1422_CNTL2_DRDY_DISABLE					(0<<3)									//BM1422AGMV DRDY引脚失能
#define BM1422_CNTL2_DRDY_ENABLE					(1<<3)									//BM1422AGMV DRDY引脚使能

#define BM1422_CNTL3_RW							(0x1D)									//BM1422AGMV 控制寄存器3
#define BM1422_CNTL3_FORCE_START					(1<<6)									//BM1422AGMV 启动测量

#define BM1422_CNTL4_WO_L						(0x5C)									//BM1422AGMV 控制寄存器4低8位
#define BM1422_CNTL4_WO_H						(0x5D)									//BM1422AGMV 控制寄存器4高8位

#define BM1422_TEMP_RO_L							(0x60)									//BM1422AGMV 温度数据低8位
#define BM1422_TEMP_RO_H							(0x61)									//BM1422AGMV 温度数据高8位
/*----- BM1422AGMV Register Map -----*/

#define BM1422_REG_MAG							6										//地磁数据
#define BM1422_SAMPLE_TIMES						5										//采样次数
#define BM1422_DEVIATION_MAX						900										//最大跳变值

void BM1422AGMV_Init(void);																//BM1422AGMV初始化

void BM1422AGMV_Drdy_Init(void);															//BM1422AGMV引脚配置DRDY高电平读取
void BM1422AGMV_Drdy_DeInit(void);															//BM1422AGMV引脚配置DRDY反初始化
void BM1422AGMV_Drdy_Exti_Init(void);														//BM1422AGMV引脚配置DRDY高电平读取(中断)

void BM1422AGMV_Discharge(void);															//BM1422AGMV硬件复位

void BM1422AGMV_Mode_Selection(u8 mode);													//BM1422AGMV测量模式选择
void BM1422AGMV_Rates_Selection(u8 rates);													//BM1422AGMV数据输出速率选择
void BM1422AGMV_DataBit_Selection(u8 databit);												//BM1422AGMV输出数据位选择
void BM1422AGMV_PowerCtrl_Selection(u8 powerctrl);											//BM1422AGMV电源控制选择

void BM1422AGMV_Start_Measurement(void);													//BM1422AGMV启动测量

void BM1422AGMV_ReadData_Single(short* x, short* y, short* z);									//BM1422AGMV读取数据

void BM1422AGMV_ClearInsideData(void);														//BM1422AGMV清除内部待读取数据缓存

#endif /* __BSP_BM1422AGMV_H */
