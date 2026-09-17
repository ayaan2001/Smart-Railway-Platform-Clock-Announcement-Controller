//train_info_display.h

#ifndef __TRAIN_INFO_DISPLAY_H__
#define __TRAIN_INFO_DISPLAY_H__

#include "project_types.h"

void DisplayTrain(u32 trainNo,
                  char *trainName,
                  char *dest,
                  u8 platform,
                  u8 arrHour,
                  u8 arrMin,
                  u8 depHour,
                  u8 depMin);

void AddDelay(u8 hour,
              u8 min,
              u16 delay,
              u8 *newHour,
              u8 *newMin);

void ResetTrainDisplay(void);

void ForceTrainRefresh(void);

int my_strlen(char *str);

void WelcomeScreen(void);

void CheckTrainApproaching(u8 currentHour,
                           u8 currentMin,
                           u8 arrHour,
                           u8 arrMin);

void DisplayTrainDetails(u8 platform,
                         u8 arrHour,
                         u8 arrMin,
                         u8 depHour,
                         u8 depMin);

void DisplayTrainRTC(u8 hour,u8 min,u8 sec,u16 delayMin);

#endif

