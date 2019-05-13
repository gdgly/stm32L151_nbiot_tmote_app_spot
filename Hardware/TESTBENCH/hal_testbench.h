#ifndef __HAL_TEST_BENCH_H
#define   __HAL_TEST_BENCH_H

#include "sys.h"

#define	TESTBENCH_ERASER_FLASHSN_DISABLE	0
#define	TESTBENCH_ERASER_FLASHSN_ENABLE	1
#define	TESTBENCH_ERASER_FLASHSN_TYPE		TESTBENCH_ERASER_FLASHSN_ENABLE

typedef __packed struct
{
	u16				StartCode;
	u32				DeviceSN;
	int16_t			mag_X;
	int16_t			mag_Y;
	int16_t			mag_Z;
	short			radarval1;
	short			radarval2;
	short			radarval3;
	char				imei[20];
	char				iccid[25];
	u16				RF4438ID;
	u8				CheckNum;
	u8				CR;
	u8				LF;
}TESTBenchDataBuf;

typedef struct
{
	u8				TestState;										// [0] --> All, [1] --> RF4438, [2] --> Magnetism, [3] --> radarval, [4] --> NBIoTval, [7] --> Compel
	u8				SendTimes;
	TESTBenchDataBuf	Data;
}TESTBenchClientTypeDef;

extern TESTBenchClientTypeDef TestBenchClient;

bool TestBench_FLASH_CheckSubSN(void);
void TestBench_FLASH_WriteSubSN(void);

void TestBench_StateCtrl_ALL_ENBALE(void);
void TestBench_StateCtrl_ALL_DISABLE(void);
bool TestBench_StateCtrl_ALL_STATE(void);

void TestBench_StateCtrl_RF4438_ENBALE(void);
void TestBench_StateCtrl_RF4438_DISABLE(void);
void TestBench_Tack_RF4438(u16 rfid);

void TestBench_StateCtrl_Magnetism_ENBALE(void);
void TestBench_StateCtrl_Magnetism_DISABLE(void);
void TestBench_Tack_Magnetism(int16_t mag_x, int16_t mag_y, int16_t mag_z);

void TestBench_StateCtrl_Radarval_ENBALE(void);
void TestBench_StateCtrl_Radarval_DISABLE(void);
void TestBench_Tack_Radarval(short radarval1, short radarval2, short radarval3);

void TestBench_StateCtrl_NBIoTval_ENBALE(void);
void TestBench_StateCtrl_NBIoTval_DISABLE(void);
void TestBench_Tack_NBIoTval(const char* imei, const char* iccid);

void TestBench_Report_Compel(void);
void TestBench_Report_DeviceData(void);

#endif /* __HAL_TEST_BENCH_H */
