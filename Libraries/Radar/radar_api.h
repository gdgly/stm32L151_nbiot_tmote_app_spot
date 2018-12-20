#ifndef __RADAR_API_H
#define   __RADAR_API_H

#include "sys.h"
#include "tmesh_radar_algorithm.h"
#include "tmesh_algorithm.h"

#define RADAR_DEBUG_LOG_RF_PRINT											//定义开启RF输出DEBUG信息

#define RADAR_ENTER_CRITICAL_SECTION()		Radar_EnterCriticalSection();
#define RADAR_EXIT_CRITICAL_SECTION()		Radar_ExitCriticalSection();

#define TO_SAVE_RADAR_BACKGROUND			1
#define NOT_SAVE_RADAR_BACKGROUND			0

#define TRADAR_GAIN_HIGHEST				15
#define TRADAR_GAIN_LOWEST				12

extern char TRADAR_GAIN_DEFAULT;
extern char TRADAR_HIGHPASS_DEFAULT;
extern char RADAR_COVERGAIN_DEFAULT;

extern tradar_targetinfo_s radar_targetinfo;
extern int inteval;

void Radar_Init(void);													//雷达初始化
u8   Radar_CheckData(u32 datain_t[], u16 in_num);								//雷达数据检查
void Radar_InitBG_Cmd(u32 v23456, u32 v7890a, u32 vbcdef, u32 vg);				//雷达初始化背景命令
u8   Radar_InitBackground(char mode);										//雷达初始化背景值
void Radar_UpdateBG_Cmd(u8 oldgain, u8 newgain);								//雷达背景设置
u8   Radar_GetData(tradar_targetinfo_s* targetinfo[], u8 dataNum);				//获取雷达数据
char	Radar_GetModel(void);												//获取雷达类型

void Radar_GetSample(void);												//雷达采样
void Radar_GetSample_Time(void);											//雷达采样节拍

void Radar_Set_SampleInterval(char val);									//set the sample interval
char Radar_Get_SampleInterval(void);										//get the sample interval

void Radar_EnterCriticalSection(void);										//雷达启动初始化配置系统时钟HSI
void Radar_ExitCriticalSection(void);										//雷达退出反初始化配置为原系统时钟MSI

u8 Radar_MeasureRadarVCC(void);											// measure the vcc of radar ic
u8 Radar_Get_RadarVcc(void);												// get the radar vcc that have been measureed

#endif
