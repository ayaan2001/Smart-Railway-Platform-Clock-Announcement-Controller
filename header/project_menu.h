#ifndef __MENU_H__
#define __MENU_H__

#include "project_types.h"

void PrintMenu(void);
void EditRTCTime(void);
void EditTrainTime(void);
void EditArrivalTime(void);
void EditDepartureTime(void);
void EditDelayTime(void);
void EditPlatform(void);

extern volatile u8 EditMode;

#endif

