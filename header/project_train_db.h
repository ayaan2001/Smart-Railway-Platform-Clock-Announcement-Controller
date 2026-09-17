//train_db.h

#ifndef __TRAIN_DB_H__
#define __TRAIN_DB_H__

#include "project_types.h"

#define MAX 3

typedef struct
{
    u32 trainNo;
    char trainName[40];
    char destination[40];
    u8 platform;
    u8 arrHour;
    u8 arrMin;
    u8 depHour;
    u8 depMin;
    u16 delayMin;
    u8 status;
}TB;

extern TB traindb[MAX];

TB *FindTrain(u32 trainNo);

TB *GetCurrentTrain(u8 hour,u8 min);

TB *GetNextTrain(TB *current);

extern TB *CurrentTrain;

extern u8 ForceDisplay;

extern u8 ShowDelayMessage;

extern u16 DelayDisplayMinute;

extern volatile u8 PlatformChanged;

extern u8 TrainInfoChanged;

#endif

