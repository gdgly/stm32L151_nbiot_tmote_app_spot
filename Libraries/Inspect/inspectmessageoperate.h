#ifndef __INSPECT_MESSAGE_OPERATE_H
#define   __INSPECT_MESSAGE_OPERATE_H

#include "sys.h"
#include "tmesh_algorithm.h"

#define INSPECT_SPOT_STATUS_PACK_NUM		3
#define INSPECT_QMC_STATUS_PACK_NUM		20

typedef __packed struct
{
	int16_t							X;
	int16_t							Y;
	int16_t							Z;
	int8_t							Temperature;
	int8_t							Status;
}Qmc5883LStatusDataTypeDef;

/* Inspect Qmc5883L Status */
typedef struct
{
	unsigned char						Front;
	unsigned char						Rear;
	Qmc5883LStatusDataTypeDef			QmcStatus[INSPECT_QMC_STATUS_PACK_NUM];
}Inspect_SwapQmcStatusTypeDef;

/* Inspect Spot Status */
typedef struct
{
	unsigned char						Front;
	unsigned char						Rear;
	SpotStatusTypedef					SpotStatus[INSPECT_SPOT_STATUS_PACK_NUM];
}Inspect_SwapSpotStatusTypeDef;

extern Inspect_SwapSpotStatusTypeDef		InspectSpotStatus;
extern Inspect_SwapQmcStatusTypeDef		InspectQmcStatus;

bool Inspect_Message_SpotStatusisFull(void);
bool Inspect_Message_QmcStatusisFull(void);

bool Inspect_Message_SpotStatusisEmpty(void);
bool Inspect_Message_QmcStatusisEmpty(void);

void Inspect_Message_SpotStatusEnqueue(SpotStatusTypedef dataBuf);
void Inspect_Message_QmcStatusEnqueue(Qmc5883LStatusDataTypeDef dataBuf);

bool Inspect_Message_SpotStatusDequeue(SpotStatusTypedef* dataBuf);
bool Inspect_Message_QmcStatusDequeue(Qmc5883LStatusDataTypeDef* dataBuf);

void Inspect_Message_SpotStatusDequeueEx(SpotStatusTypedef* dataBuf);
void Inspect_Message_QmcStatusDequeueEx(Qmc5883LStatusDataTypeDef* dataBuf);

bool Inspect_Message_SpotStatusOffSet(void);
bool Inspect_Message_QmcStatusOffSet(void);

unsigned char Inspect_Message_SpotStatusRear(void);
unsigned char Inspect_Message_QmcStatusRear(void);

#endif
