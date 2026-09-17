//led.h

#include "project_types.h"
#include "project_train_db.h"

#ifndef __LED_H__
#define __LED_H__

void InitLED(void);

void UpdateTrainStatusLED(u8 hour,u8 min,TB *train);

#endif

