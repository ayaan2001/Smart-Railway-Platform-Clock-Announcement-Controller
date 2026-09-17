//main.c

#include "project_lcd.h"
#include "project_lcd_defines.h"
#include "project_kpm.h"
#include "project_rtc.h"
#include "project_eint0.h"
#include "project_menu.h"
#include "project_delay.h"
#include "project_train_info.h"
#include "project_train_db.h"
#include "project_led.h"
#include "project_buzzer.h"
#include <LPC21xx.h>

u32 hour,min,sec,date,month,year,day;

u8 displayMode = 0;
u8 trainSecondDisplay = 0;
u8 lastSec = 60;
u8 infoLastSec = 60;
u8 trainDisplayTime = 0;
u8 TrainInfoTimer = 0;
u8 TrainInfoActive = 0;

extern volatile u8 edit_request;

#define SUN 0
#define MON 1
#define TUE 2
#define WED 3
#define THU 4
#define FRI 5
#define SAT 6

int main()
{
    TB *train;
    u16 now;
    u16 dep;

    RTC_Init();
    InitLCD();

	Init_KPM();
    Init_EINT();
    InitLED();
    Buzzer_Init();

    WelcomeScreen();

    SetRTCTimeInfo(9,24,45);
    SetRTCDateInfo(15,9,2026);
    SetRTCDay(MON);

    while(1)
    {
        if(edit_request)
        {
            edit_request = 0;
            EditMode = 1;
            PrintMenu();
						
						if(TrainInfoChanged)
						{
							TrainInfoActive = 1;
							TrainInfoTimer = 0;
						}
						
						CmdLCD(CLEAR_LCD);

            displayMode = 0;
            trainSecondDisplay = 0;
            trainDisplayTime = 0;
            lastSec = 60;

            ResetTrainDisplay();
        }

        GetRTCTimeInfo(&hour,&min,&sec);
				
				if(TrainInfoActive)
				{
					if(sec != infoLastSec)
					{
						infoLastSec = sec;

						TrainInfoTimer++;

						if(TrainInfoTimer >= 2)
						{
							TrainInfoActive = 0;
							TrainInfoChanged = 0;
							TrainInfoTimer = 0;
						}
					}
				}

        now = hour * 60 + min;

        if(ForceDisplay)
        {
            TB *nextTrain;

            train = CurrentTrain;

            if(train != 0)
            {
                dep = train->depHour * 60
                    + train->depMin
                    + train->delayMin;

                if(now >= dep)
                {
                    nextTrain = GetNextTrain(CurrentTrain);

                    if(nextTrain != 0)
                    {
                        CurrentTrain = nextTrain;
                        ForceDisplay = 1;
                        train = nextTrain;
                        ResetTrainDisplay();
                        CmdLCD(CLEAR_LCD);
                        displayMode = 0;
						trainSecondDisplay = 0;
                        trainDisplayTime = 0;
                        lastSec = sec;
                    }
                    else
                    {
                        ForceDisplay = 0;
                        CurrentTrain = 0;
                        train = 0;
                        ResetTrainDisplay();
                        CmdLCD(CLEAR_LCD);
                        displayMode = 0;
						trainSecondDisplay = 0;
                        trainDisplayTime = 0;
                        lastSec = sec;
                    }
                }
            }
        }
        else
        {
            train = GetCurrentTrain(hour,min);

            if(train != 0)
            {
                CurrentTrain = train;
                ForceDisplay = 1;
            }
        }

        if(EditMode == 0)
        {
            UpdateTrainStatusLED(hour,min,train);
        }

        if(train != 0)
        {
            if(displayMode == 0)
            {
                CmdLCD(CLEAR_LCD);
                ResetTrainDisplay();
                displayMode = 1;
							
				trainSecondDisplay = 0;
				trainDisplayTime = 0;
				lastSec = sec;
            }

            if(ShowDelayMessage)
            {
                CmdLCD(CLEAR_LCD);

                LCD_GotoXY(0,0);
                U32LCD(train->trainNo);

                LCD_CharXY(0,5,':');

                LCD_StringXY(0,6,train->trainName);

                LCD_StringXY(1,0,"Delay:");

				U32LCD(DelayDisplayMinute);

                StrLCD(" Min");
								
				#ifdef HARDWARE
				delay_ms(1000);
			    #else
				delay_ms(500);
				#endif

                ShowDelayMessage = 0;

                ResetTrainDisplay();

                CmdLCD(CLEAR_LCD);
								
				trainSecondDisplay = 0;
                trainDisplayTime = 0;
                lastSec = sec;

                continue;
            }

            DisplayTrain(train->trainNo,
                         train->trainName,
                         train->destination,
                         train->platform,
                         train->arrHour,
                         train->arrMin,
                         train->depHour,
                         train->depMin);

            CheckTrainApproaching(hour,
                                  min,
                                  train->arrHour,
                                  train->arrMin);
						
            if(sec != lastSec)
				{
					lastSec = sec;

					trainDisplayTime++;

					if(trainDisplayTime >= 5)
				{
					trainDisplayTime = 0;

					if(trainSecondDisplay == 0)
					trainSecondDisplay = 1;
				     else
					trainSecondDisplay = 0;

					CmdLCD(GOTO_LINE2_POS0);
					StrLCD("                ");
				}
			}

				if(trainSecondDisplay == 0)
				{
					DisplayTrainDetails(train->platform,
					  train->arrHour,
					   train->arrMin,
						train->depHour,
						train->depMin);
						}
						else
						{
							DisplayTrainRTC(hour,min,sec,train->delayMin);
						}
				}
        else
        {
            if(displayMode == 1)
            {
                CmdLCD(CLEAR_LCD);
                ResetTrainDisplay();
                displayMode = 0;
								trainSecondDisplay = 0;
                trainDisplayTime = 0;
                lastSec = sec;
            }

            DisplayRTCTime(hour,min,sec);

            GetRTCDateInfo(&date,&month,&year);

            DisplayRTCDate(date,month,year);

            GetRTCDay(&day);

            DisplayRTCDay(day);
        }
    }
}

