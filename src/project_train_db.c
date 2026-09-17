//train_db.c

#include "project_train_db.h"

TB traindb[MAX] =
{
    {12745,"manugur sf express","manugur",1,9,30,9,40,0,0},
    {12723,"telangana express","hazrat nizamuddin",2,10,30,10,40,0,0},
    {12704,"falaknuma express","Vishakapatnam",3,11,00,11,10,0,0}
};

u8 ShowDelayMessage = 0;
u16 DelayDisplayMinute = 0;
TB *CurrentTrain = 0;
u8 ForceDisplay = 0;
volatile u8 PlatformChanged = 0;
u8 TrainInfoChanged = 0;

TB *FindTrain(u32 trainNo)
{
    s32 i;

    for(i=0;i<MAX;i++)
    {
        if(traindb[i].trainNo == trainNo)
        {
            return &traindb[i];
        }
    }

    return 0;
}

TB *GetCurrentTrain(u8 hour,u8 min)
{
    u16 now;
    u16 arr;
    u16 dep;
    s32 i;

    now = hour * 60 + min;

    for(i=0;i<MAX;i++)
    {
        arr = traindb[i].arrHour * 60
            + traindb[i].arrMin
            + traindb[i].delayMin;

        dep = traindb[i].depHour * 60
            + traindb[i].depMin
            + traindb[i].delayMin;

        if(traindb[i].delayMin > 0)
        {
            if(now < dep)
            {
                return &traindb[i];
            }
        }
        else
        {
            if((now >= (arr - 5)) && (now < dep))
            {
                return &traindb[i];
            }
        }
    }

    return 0;
}

TB *GetNextTrain(TB *current)
{
    s32 i;

    if(current == 0)
        return 0;

    for(i=0;i<MAX;i++)
    {
        if(&traindb[i] == current)
        {
            if(i + 1 < MAX)
            {
                return &traindb[i + 1];
            }

            return 0;
        }
    }

    return 0;
}

