/**
  *********************************************************************************************************
  * @file    hal_bm1422agmv.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2019-08-30
  * @brief   HAL BM1422AGMV Device
  *********************************************************************************************************
  * @attention
  *			
  *
  *
  *********************************************************************************************************
  */

#include "hal_bm1422agmv.h"
#include "platform_config.h"
#include "platform_map.h"
#include "stm32l1xx_config.h"
#include "tmesh_algorithm.h"
#include "hal_iic.h"
#include "hal_beep.h"
#include "hal_iwdg.h"
#include "delay.h"
#include <stdlib.h>

/**********************************************************************************************************
 @Function			static u8 BM1422AGMV_WriteByte(u8 ucRegAddr, u8 ucRegData)
 @Description			BM1422AGMV写入1Byte数据到寄存器( 内部调用 )
 @Input				ucRegAddr  : 写入地址
					ucRegData  : 写入数据
 @Return				0
**********************************************************************************************************/
static u8 BM1422AGMV_WriteByte(u8 ucRegAddr, u8 ucRegData)
{
	IIC_Start();																		//发送起始信号
	
	IIC_Send_Byte(BM1422AGMV_SLAVE_ADDRESS_W);												//发送I2C从机地址写
	IIC_Wait_Ack();																	//等待应答
	
	IIC_Send_Byte(ucRegAddr);															//发送待写入数据地址
	IIC_Wait_Ack();																	//等待应答
	
	IIC_Send_Byte(ucRegData);															//发送需写入数据
	IIC_Wait_Ack();																	//等待应答
	
	IIC_Stop();																		//发送停止信号
	
	return 0;
}

/**********************************************************************************************************
 @Function			static u8 BM1422AGMV_ReadByte(u8 ucRegAddr)
 @Description			BM1422AGMV读取1Byte数据( 内部调用 )
 @Input				ucRegAddr  : 读取数据地址
 @Return				ucRegAddr  : 读取数据
**********************************************************************************************************/
static u8 BM1422AGMV_ReadByte(u8 ucRegAddr)
{
	u8 ucData;
	
	IIC_Start();																		//发送起始信号
	
	IIC_Send_Byte(BM1422AGMV_SLAVE_ADDRESS_W);												//发送I2C从机地址写
	IIC_Wait_Ack();																	//等待应答
	
	IIC_Send_Byte(ucRegAddr);															//发送待读取数据地址
	IIC_Wait_Ack();																	//等待应答
	
	IIC_Start();																		//发送起始信号
	
	IIC_Send_Byte(BM1422AGMV_SLAVE_ADDRESS_R);												//发送I2C从机地址读
	IIC_Wait_Ack();																	//等待应答
	
	ucData = IIC_Read_Byte(0);															//读取数据
	
	IIC_Stop();																		//发送停止信号
	
	return ucData;
}

#if 1
/**********************************************************************************************************
 @Function			static u8 BM1422AGMV_WriteBuffer(u8 ucRegAddr, u8 *pucRegData, u8 ucRegCount)
 @Description			BM1422AGMV写入nByte数据到寄存器( 内部调用 )
 @Input				ucRegAddr  : 写入地址
					pucRegData : 写入数据地址
					ucRegCount : 写入数据个数
 @Return				0
**********************************************************************************************************/
u8 BM1422AGMV_WriteBuffer(u8 ucRegAddr, u8 *pucRegData, u8 ucRegCount)
{
	IIC_Start();																		//发送起始信号
	
	IIC_Send_Byte(BM1422AGMV_SLAVE_ADDRESS_W);												//发送I2C从机地址写
	IIC_Wait_Ack();																	//等待应答
	
	IIC_Send_Byte(ucRegAddr);															//发送待写入数据地址
	IIC_Wait_Ack();																	//等待应答
	
	for (int i = 0; i < ucRegCount; i++) {
		IIC_Send_Byte(pucRegData[i]);														//发送需写入数据
		IIC_Wait_Ack();																//等待应答
	}
	
	IIC_Stop();																		//发送停止信号
	
	return 0;
}

/**********************************************************************************************************
 @Function			static u8 BM1422AGMV_ReadBuffer(u8 ucRegAddr, u8 *pucRegData, u8 ucRegCount)
 @Description			BM1422AGMV读取nByte数据( 内部调用 )
 @Input				ucRegAddr  : 读取地址
					ucRegAddr  : 读取数据地址
					ucRegCount : 读取数据个数
 @Return				0
**********************************************************************************************************/
u8 BM1422AGMV_ReadBuffer(u8 ucRegAddr, u8 *pucRegData, u8 ucRegCount)
{
	IIC_Start();																		//发送起始信号
	
	IIC_Send_Byte(BM1422AGMV_SLAVE_ADDRESS_W);												//发送I2C从机地址写
	IIC_Wait_Ack();																	//等待应答
	
	IIC_Send_Byte(ucRegAddr);															//发送待写入数据地址
	IIC_Wait_Ack();																	//等待应答
	
	IIC_Start();																		//发送起始信号
	
	IIC_Send_Byte(BM1422AGMV_SLAVE_ADDRESS_R);												//发送I2C从机地址读
	IIC_Wait_Ack();																	//等待应答
	
	for (int i = 0; i < ucRegCount; i++) {
		if ((i + 1) == ucRegCount) {
			pucRegData[i] = IIC_Read_Byte(0);
		}
		else {
			pucRegData[i] = IIC_Read_Byte(1);
		}
	}
	
	IIC_Stop();																		//发送停止信号
	
	return 0;
}
#endif

/**********************************************************************************************************
 @Function			void BM1422AGMV_Init(void)
 @Description			BM1422AGMV初始化
 @Input				void
 @Return				void
**********************************************************************************************************/
void BM1422AGMV_Init(void)
{
	IIC_Init();																		//IIC初始化
	
	BM1422AGMV_WriteByte(BM1422_CNTL1_RW, BM1422_CNTL1_RST_SOFT_REST_ENABLE);						//复位BM1422AGMV
	
	/* 工作, 14位, 复位释放, 1000Hz输出, 单次测量模式 */
	BM1422AGMV_WriteByte(BM1422_CNTL1_RW, BM1422_CNTL1_PC1_ACTIVE | BM1422_CNTL1_OUT_14BIT | BM1422_CNTL1_RST_SOFT_REST_RELEASE | BM1422_CNTL1_ODR_RATES_1000HZ | BM1422_CNTL1_FS1_MODE_SINGLE);
	
	/* 控制寄存器4清0 */
	BM1422AGMV_WriteByte(BM1422_CNTL4_WO_L, 0x00);
	BM1422AGMV_WriteByte(BM1422_CNTL4_WO_H, 0x00);
	
	/* 使能DRDY引脚高电平有效 */
	BM1422AGMV_WriteByte(BM1422_CNTL2_RW, BM1422_CNTL2_DRDY_HIGH_ACTIVE | BM1422_CNTL2_DRDY_ENABLE);
	
#if 0
	/* 休眠 */
	BM1422AGMV_PowerCtrl_Selection(BM1422_CNTL1_PC1_POWERDOWN);
#endif
	
#if BM1422_DRDY_EXIT
	BM1422AGMV_Drdy_Exti_Init();
#else
	BM1422AGMV_Drdy_Init();
#endif
}

/**********************************************************************************************************
 @Function			void BM1422AGMV_Drdy_Init(void)
 @Description			BM1422AGMV引脚配置DRDY高电平读取
 @Input				void
 @Return				void
**********************************************************************************************************/
void BM1422AGMV_Drdy_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	BM1422_DRDY_RCC_GPIO_CLK_ENABLE();
	
	GPIO_Initure.Pin = BM1422_DRDY_PIN;
	GPIO_Initure.Mode = GPIO_MODE_INPUT;
	GPIO_Initure.Pull = GPIO_PULLDOWN;
	GPIO_Initure.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(BM1422_DRDY_GPIOx, &GPIO_Initure);
}

/**********************************************************************************************************
 @Function			void BM1422AGMV_Drdy_DeInit(void)
 @Description			BM1422AGMV引脚配置DRDY反初始化
 @Input				void
 @Return				void
**********************************************************************************************************/
void BM1422AGMV_Drdy_DeInit(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	BM1422_DRDY_RCC_GPIO_CLK_ENABLE();
	
	GPIO_Initure.Pin = BM1422_DRDY_PIN;
	GPIO_Initure.Mode = GPIO_MODE_INPUT;
	GPIO_Initure.Pull = GPIO_NOPULL;
	GPIO_Initure.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(BM1422_DRDY_GPIOx, &GPIO_Initure);
}

/**********************************************************************************************************
 @Function			void QMC5883L_Drdy_Exti_Init(void)
 @Description			BM1422AGMV引脚配置DRDY高电平读取(中断)
 @Input				void
 @Return				void
**********************************************************************************************************/
void BM1422AGMV_Drdy_Exti_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	BM1422_DRDY_RCC_GPIO_CLK_ENABLE();
	
	GPIO_Initure.Pin = BM1422_DRDY_PIN;
	GPIO_Initure.Mode = GPIO_MODE_IT_RISING;
	GPIO_Initure.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(BM1422_DRDY_GPIOx, &GPIO_Initure);
	
	HAL_NVIC_SetPriority(BM1422_DRDY_IRQn, 2, 1);
	HAL_NVIC_EnableIRQ(BM1422_DRDY_IRQn);
}

/**********************************************************************************************************
 @Function			void BM1422AGMV_Discharge(void)
 @Description			BM1422AGMV硬件复位
 @Input				void
 @Return				void
**********************************************************************************************************/
void BM1422AGMV_Discharge(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	IIC_SCL_RCC_GPIO_CLK_ENABLE();
	IIC_SDA_RCC_GPIO_CLK_ENABLE();
	
	GPIO_Initure.Pin = IIC_SCL_PIN;
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Initure.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(IIC_SCL_GPIOx, &GPIO_Initure);
	
	GPIO_Initure.Pin = IIC_SDA_PIN;
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Initure.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(IIC_SDA_GPIOx, &GPIO_Initure);
	
	HAL_GPIO_WritePin(IIC_SCL_GPIOx, IIC_SCL_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IIC_SDA_GPIOx, IIC_SDA_PIN, GPIO_PIN_RESET);
	Delay_MS(3000);
	HAL_GPIO_WritePin(IIC_SCL_GPIOx, IIC_SCL_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(IIC_SDA_GPIOx, IIC_SDA_PIN, GPIO_PIN_SET);
}

/**********************************************************************************************************
 @Function			void BM1422AGMV_Mode_Selection(u8 mode)
 @Description			BM1422AGMV测量模式选择
 @Input				mode : BM1422_CNTL1_FS1_MODE_CONTINUOUS / BM1422_CNTL1_FS1_MODE_SINGLE
 @Return				void
**********************************************************************************************************/
void BM1422AGMV_Mode_Selection(u8 mode)
{
	u8 reg;
	
	reg = BM1422AGMV_ReadByte(BM1422_CNTL1_RW);
	
	if (mode == BM1422_CNTL1_FS1_MODE_CONTINUOUS) {
		BM1422AGMV_WriteByte(BM1422_CNTL1_RW, ((reg & 0xFD) | BM1422_CNTL1_FS1_MODE_CONTINUOUS));
	}
	else if (mode == BM1422_CNTL1_FS1_MODE_SINGLE) {
		BM1422AGMV_WriteByte(BM1422_CNTL1_RW, ((reg & 0xFD) | BM1422_CNTL1_FS1_MODE_SINGLE));
	}
	else {
		BM1422AGMV_WriteByte(BM1422_CNTL1_RW, ((reg & 0xFD) | BM1422_CNTL1_FS1_MODE_SINGLE));
	}
}

/**********************************************************************************************************
 @Function			void BM1422AGMV_Rates_Selection(u8 rates)
 @Description			BM1422AGMV数据输出速率选择
 @Input				rates : BM1422_CNTL1_ODR_RATES_10HZ / BM1422_CNTL1_ODR_RATES_20HZ
						   BM1422_CNTL1_ODR_RATES_100HZ / BM1422_CNTL1_ODR_RATES_1000HZ
 @Return				void
**********************************************************************************************************/
void BM1422AGMV_Rates_Selection(u8 rates)
{
	u8 reg;
	
	reg = BM1422AGMV_ReadByte(BM1422_CNTL1_RW);
	
	if (rates == BM1422_CNTL1_ODR_RATES_10HZ) {
		BM1422AGMV_WriteByte(BM1422_CNTL1_RW, ((reg & 0xE7) | BM1422_CNTL1_ODR_RATES_10HZ));
	}
	else if (rates == BM1422_CNTL1_ODR_RATES_20HZ) {
		BM1422AGMV_WriteByte(BM1422_CNTL1_RW, ((reg & 0xE7) | BM1422_CNTL1_ODR_RATES_20HZ));
	}
	else if (rates == BM1422_CNTL1_ODR_RATES_100HZ) {
		BM1422AGMV_WriteByte(BM1422_CNTL1_RW, ((reg & 0xE7) | BM1422_CNTL1_ODR_RATES_100HZ));
	}
	else if (rates == BM1422_CNTL1_ODR_RATES_1000HZ) {
		BM1422AGMV_WriteByte(BM1422_CNTL1_RW, ((reg & 0xE7) | BM1422_CNTL1_ODR_RATES_1000HZ));
	}
	else {
		BM1422AGMV_WriteByte(BM1422_CNTL1_RW, ((reg & 0xE7) | BM1422_CNTL1_ODR_RATES_1000HZ));
	}
}

/**********************************************************************************************************
 @Function			void BM1422AGMV_DataBit_Selection(u8 databit)
 @Description			BM1422AGMV输出数据位选择
 @Input				databit : BM1422_CNTL1_OUT_12BIT / BM1422_CNTL1_OUT_14BIT
 @Return				void
**********************************************************************************************************/
void BM1422AGMV_DataBit_Selection(u8 databit)
{
	u8 reg;
	
	reg = BM1422AGMV_ReadByte(BM1422_CNTL1_RW);
	
	if (databit == BM1422_CNTL1_OUT_12BIT) {
		BM1422AGMV_WriteByte(BM1422_CNTL1_RW, ((reg & 0xBF) | BM1422_CNTL1_OUT_12BIT));
	}
	else if (databit == BM1422_CNTL1_OUT_14BIT) {
		BM1422AGMV_WriteByte(BM1422_CNTL1_RW, ((reg & 0xBF) | BM1422_CNTL1_OUT_14BIT));
	}
	else {
		BM1422AGMV_WriteByte(BM1422_CNTL1_RW, ((reg & 0xBF) | BM1422_CNTL1_OUT_14BIT));
	}
}

/**********************************************************************************************************
 @Function			void BM1422AGMV_PowerCtrl_Selection(u8 powerctrl)
 @Description			BM1422AGMV电源控制选择
 @Input				databit : BM1422_CNTL1_PC1_POWERDOWN / BM1422_CNTL1_PC1_ACTIVE
 @Return				void
**********************************************************************************************************/
void BM1422AGMV_PowerCtrl_Selection(u8 powerctrl)
{
	u8 reg;
	
	reg = BM1422AGMV_ReadByte(BM1422_CNTL1_RW);
	
	if (powerctrl == BM1422_CNTL1_PC1_POWERDOWN) {
		BM1422AGMV_WriteByte(BM1422_CNTL1_RW, ((reg & 0x7F) | BM1422_CNTL1_PC1_POWERDOWN));
	}
	else if (powerctrl == BM1422_CNTL1_PC1_ACTIVE) {
		BM1422AGMV_WriteByte(BM1422_CNTL1_RW, ((reg & 0x7F) | BM1422_CNTL1_PC1_ACTIVE));
		BM1422AGMV_WriteByte(BM1422_CNTL4_WO_L, 0x00);
		BM1422AGMV_WriteByte(BM1422_CNTL4_WO_H, 0x00);
		BM1422AGMV_WriteByte(BM1422_CNTL2_RW, BM1422_CNTL2_DRDY_HIGH_ACTIVE | BM1422_CNTL2_DRDY_ENABLE);
	}
	else {
		BM1422AGMV_WriteByte(BM1422_CNTL1_RW, ((reg & 0x7F) | BM1422_CNTL1_PC1_ACTIVE));
		BM1422AGMV_WriteByte(BM1422_CNTL4_WO_L, 0x00);
		BM1422AGMV_WriteByte(BM1422_CNTL4_WO_H, 0x00);
		BM1422AGMV_WriteByte(BM1422_CNTL2_RW, BM1422_CNTL2_DRDY_HIGH_ACTIVE | BM1422_CNTL2_DRDY_ENABLE);
	}
}

/**********************************************************************************************************
 @Function			void BM1422AGMV_Start_Measurement(void)
 @Description			BM1422AGMV启动测量
 @Input				void
 @Return				void
**********************************************************************************************************/
void BM1422AGMV_Start_Measurement(void)
{
	BM1422AGMV_WriteByte(BM1422_CNTL3_RW, BM1422_CNTL3_FORCE_START);
}

/**********************************************************************************************************
 @Function			void BM1422AGMV_ReadData_Single(short* x, short* y, short* z)
 @Description			BM1422AGMV读取数据
 @Input				x, y, z
 @Return				void
**********************************************************************************************************/
void BM1422AGMV_ReadData_Single(short* x, short* y, short* z)
{
	uint8_t ucReadBuf[BM1422_SAMPLE_TIMES][BM1422_REG_MAG];
	int16_t magdata_x[BM1422_SAMPLE_TIMES], magdata_y[BM1422_SAMPLE_TIMES], magdata_z[BM1422_SAMPLE_TIMES];
	u8  index = 0;
	u8  sample_times = 0;
	u32 readtimeover = 0;
	static short bm_x_old, bm_y_old, bm_z_old;
	static char bm_same_cnt = 0;
	
	BM1422AGMV_Start_Measurement();
	
	while ((readtimeover++) < 10000) {
		if (BM1422_DRDY_READ() == 1) {
			BM1422AGMV_ReadBuffer(BM1422_DATA_OUT_X_RO_L, ucReadBuf[sample_times], BM1422_REG_MAG);
			sample_times++;
			if (sample_times < BM1422_SAMPLE_TIMES) BM1422AGMV_Start_Measurement();
		}
		
		if (sample_times >= BM1422_SAMPLE_TIMES) {
			for (index = 0; index < BM1422_SAMPLE_TIMES; index++) {
				magdata_x[index] = (int16_t)(ucReadBuf[index][1] << 8) | ucReadBuf[index][0];
				magdata_y[index] = (int16_t)(ucReadBuf[index][3] << 8) | ucReadBuf[index][2];
				magdata_z[index] = (int16_t)(ucReadBuf[index][5] << 8) | ucReadBuf[index][4];
			}
			for (index = 1; index < BM1422_SAMPLE_TIMES; index++) {
				if (abs(magdata_x[0] - magdata_x[index]) > BM1422_DEVIATION_MAX) {
					sample_times = 0;
					break;
				}
				else if (abs(magdata_y[0] - magdata_y[index]) > BM1422_DEVIATION_MAX) {
					sample_times = 0;
					break;
				}
				else if (abs(magdata_z[0] - magdata_z[index]) > BM1422_DEVIATION_MAX) {
					sample_times = 0;
					break;
				}
			}
			if (sample_times != 0) break;
		}
		
		Delay_US(10);
	}
	
	if (sample_times >= BM1422_SAMPLE_TIMES) {
		*x = 0; *y = 0; *z = 0;
		
		for (index = 0; index < BM1422_SAMPLE_TIMES; index++) {
			*x += (((int16_t)(ucReadBuf[index][1] << 8)|ucReadBuf[index][0])+BM1422_SAMPLE_TIMES/2)/BM1422_SAMPLE_TIMES;
			*y += (((int16_t)(ucReadBuf[index][3] << 8)|ucReadBuf[index][2])+BM1422_SAMPLE_TIMES/2)/BM1422_SAMPLE_TIMES;
			*z += (((int16_t)(ucReadBuf[index][5] << 8)|ucReadBuf[index][4])+BM1422_SAMPLE_TIMES/2)/BM1422_SAMPLE_TIMES;
		}
		
		if ((*x == bm_x_old) && (*y == bm_y_old) && (*z == bm_z_old)) {
			bm_same_cnt++;
		}
		
		bm_x_old = *x; bm_y_old = *y; bm_z_old = *z;
		
		if (bm_same_cnt > 60) {
			bm_same_cnt = 0;
			ReInitModule();
		}
	}
	else {
		ReInitModule();
	}
}

/**********************************************************************************************************
 @Function			void BM1422AGMV_ClearInsideData(void)
 @Description			BM1422AGMV清除内部待读取数据缓存
 @Input				void
 @Return				void
**********************************************************************************************************/
void BM1422AGMV_ClearInsideData(void)
{
	u8 ucReadBuf[BM1422_REG_MAG];
	
	if (BM1422_DRDY_READ() == 1) {
		BM1422AGMV_ReadBuffer(BM1422_DATA_OUT_X_RO_L, ucReadBuf, BM1422_REG_MAG);
	}
}

/**********************************************************************************************************
 @Function			short BM1422AGMV_Temperature_Read(void)
 @Description			BM1422AGMV温度读取
 @Input				void
 @Return				void
**********************************************************************************************************/
short BM1422AGMV_Temperature_Read(void)
{
	uint8_t ucReadBuf[2];
	short tempval;
	u32 readtimeover = 0;
	
	BM1422AGMV_Start_Measurement();
	
	while ((readtimeover++) < 1000) {
		if (BM1422_DRDY_READ() == 1) {
			BM1422AGMV_ReadBuffer(BM1422_TEMP_RO_L, ucReadBuf, 2);
			break;
		}
		Delay_US(10);
	}
	
	tempval = ((int16_t)(ucReadBuf[1] << 8) | ucReadBuf[0]);
	
	Qmc5883lData.temp_now = tempval / 100;
	
	return tempval;
}

/********************************************** END OF FLEE **********************************************/
