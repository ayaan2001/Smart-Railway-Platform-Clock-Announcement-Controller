//rtc.h
#include "project_types.h"

void RTC_Init(void);
void GetRTCTimeInfo(u32 *,u32 *,u32 *);
void DisplayRTCTime(u32,u32,u32);
void GetRTCDateInfo(u32 *,u32 *,u32 *);
void DisplayRTCDate(u32,u32,u32);

void SetRTCTimeInfo(u32,u32,u32);
void SetRTCDateInfo(u32,u32,u32);

void GetRTCDay(u32 *);
void DisplayRTCDay(u32);
void SetRTCDay(u32);

