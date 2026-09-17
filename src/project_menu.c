
// menu.c

#include "project_menu.h"
#include "project_lcd.h"
#include "project_types.h"
#include "project_lcd_defines.h"
#include "project_kpm.h"
#include "project_rtc.h"
#include "project_delay.h"
#include "project_train_info.h"
#include "project_train_db.h"
#include "project_buzzer.h"

u8 key;
volatile u8 EditMode = 0;
u8 trainMenuExit;


/*------------------------------------------------
  Check Arrival Time < Departure Time
------------------------------------------------*/
u8 IsValidTrainTime(u8 arrHour, u8 arrMin,
                    u8 depHour, u8 depMin)
{
    u16 arrTime;
    u16 depTime;

    arrTime = arrHour * 60 + arrMin;
    depTime = depHour * 60 + depMin;

    if(arrTime < depTime)
        return 1;

    return 0;
}


/*------------------------------------------------
  Small message delay
------------------------------------------------*/
void MenuDelay(void)
{
    delay_ms(1000);
}


/*------------------------------------------------
  ADMIN MENU
------------------------------------------------*/
void PrintMenu(void)
{
    u8 key;

    EditMode = 1;

    CmdLCD(CLEAR_LCD);
    StrLCD("ADMIN MODE");
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("1.RTC 2.TRAIN");

    delay_ms(1000);

    while(1)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("1:RTC  2:TRAIN");
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("3:EXIT");

        key = keyscan();

        switch(key)
        {
            case '1':
                EditRTCTime();
                break;

            case '2':
                EditTrainTime();
                break;

            case '3':
                CmdLCD(CLEAR_LCD);
                StrLCD("Exiting...");
                MenuDelay();

                ResetTrainDisplay();
                CmdLCD(CLEAR_LCD);

                EditMode = 0;
                return;

            default:
                CmdLCD(CLEAR_LCD);
                StrLCD("Invalid Option");
                MenuDelay();
                break;
        }
    }
}


/*------------------------------------------------
  RTC EDIT MENU
------------------------------------------------*/
void EditRTCTime(void)
{
    u32 hr, min, sec;
    u32 date, month, year, day;
    u32 value;
    u8 key;

    GetRTCTimeInfo(&hr, &min, &sec);
    GetRTCDateInfo(&date, &month, &year);
    GetRTCDay(&day);

    while(1)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("1:H 2:M 3:S");
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("4:D 5:MO 6:Y");

        delay_ms(700);

        CmdLCD(CLEAR_LCD);
        StrLCD("7:DAY 8:EXIT");

        key = keyscan();

        switch(key)
        {
            /* -------- HOUR -------- */
            case '1':

                CmdLCD(CLEAR_LCD);
                StrLCD("Enter Hour:");
                CmdLCD(GOTO_LINE2_POS0);

                value = ReadNumLCD(0);

                if(value > 23)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Hour");
                    MenuDelay();
                    break;
                }

                hr = value;
                SetRTCTimeInfo(hr, min, sec);

                CmdLCD(CLEAR_LCD);
                StrLCD("Hour Updated");
                MenuDelay();
                break;


            /* -------- MINUTE -------- */
            case '2':

                CmdLCD(CLEAR_LCD);
                StrLCD("Enter Minute:");
                CmdLCD(GOTO_LINE2_POS0);

                value = ReadNumLCD(0);

                if(value > 59)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Minute");
                    MenuDelay();
                    break;
                }

                min = value;
                SetRTCTimeInfo(hr, min, sec);

                CmdLCD(CLEAR_LCD);
                StrLCD("Minute Updated");
                MenuDelay();
                break;


            /* -------- SECOND -------- */
            case '3':

                CmdLCD(CLEAR_LCD);
                StrLCD("Enter Second:");
                CmdLCD(GOTO_LINE2_POS0);

                value = ReadNumLCD(0);

                if(value > 59)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Second");
                    MenuDelay();
                    break;
                }

                sec = value;
                SetRTCTimeInfo(hr, min, sec);

                CmdLCD(CLEAR_LCD);
                StrLCD("Second Updated");
                MenuDelay();
                break;


            /* -------- DATE -------- */
            case '4':

                CmdLCD(CLEAR_LCD);
                StrLCD("Enter Date:");
                CmdLCD(GOTO_LINE2_POS0);

                value = ReadNumLCD(0);

                if(value < 1 || value > 31)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Date");
                    MenuDelay();
                    break;
                }

                date = value;
                SetRTCDateInfo(date, month, year);

                CmdLCD(CLEAR_LCD);
                StrLCD("Date Updated");
                MenuDelay();
                break;


            /* -------- MONTH -------- */
            case '5':

                CmdLCD(CLEAR_LCD);
                StrLCD("Enter Month:");
                CmdLCD(GOTO_LINE2_POS0);

                value = ReadNumLCD(0);

                if(value < 1 || value > 12)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Month");
                    MenuDelay();
                    break;
                }

                month = value;
                SetRTCDateInfo(date, month, year);

                CmdLCD(CLEAR_LCD);
                StrLCD("Month Updated");
                MenuDelay();
                break;


            /* -------- YEAR -------- */
            case '6':

                CmdLCD(CLEAR_LCD);
                StrLCD("Enter Year:");
                CmdLCD(GOTO_LINE2_POS0);

                value = ReadNumLCD(0);

                if(value < 2000 || value > 2099)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Year");
                    MenuDelay();
                    break;
                }

                year = value;
                SetRTCDateInfo(date, month, year);

                CmdLCD(CLEAR_LCD);
                StrLCD("Year Updated");
                MenuDelay();
                break;


            /* -------- DAY -------- */
            case '7':

                CmdLCD(CLEAR_LCD);
                StrLCD("Day 0-6:");
                CmdLCD(GOTO_LINE2_POS0);

                value = ReadNumLCD(0);

                if(value > 6)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Day");
                    MenuDelay();
                    break;
                }

                day = value;
                SetRTCDay(day);

                CmdLCD(CLEAR_LCD);
                StrLCD("Day Updated");
                MenuDelay();
                break;


            /* -------- EXIT -------- */
            case '8':

                CmdLCD(CLEAR_LCD);
                StrLCD("RTC Menu Exit");
                MenuDelay();

                return;


            default:

                CmdLCD(CLEAR_LCD);
                StrLCD("Invalid Option");
                MenuDelay();
                break;
        }

        /* Update local RTC variables */
        GetRTCTimeInfo(&hr, &min, &sec);
        GetRTCDateInfo(&date, &month, &year);
        GetRTCDay(&day);
    }
}


/*------------------------------------------------
  TRAIN EDIT MENU
------------------------------------------------*/
void EditTrainTime(void)
{
    u32 trainNo;
    TB *train;
    u8 key;

    CmdLCD(CLEAR_LCD);
    StrLCD("Train Number:");
    CmdLCD(GOTO_LINE2_POS0);

    trainNo = ReadNumLCD(9);

    train = FindTrain(trainNo);

    if(train == 0)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Train Not Found");
        MenuDelay();
        return;
    }

    while(1)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("1:ARRIVAL");
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("2:DEPARTURE");

        delay_ms(700);

        CmdLCD(CLEAR_LCD);
        StrLCD("3:DELAY 4:PLAT");
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("5:EXIT");

        key = keyscan();

        switch(key)
        {
            case '1':
                EditArrivalTime();
                break;

            case '2':
                EditDepartureTime();
                break;

            case '3':
                EditDelayTime();
                break;

            case '4':
                EditPlatform();
                break;

            case '5':
                CmdLCD(CLEAR_LCD);
                StrLCD("Train Menu Exit");
                MenuDelay();

                ResetTrainDisplay();
                CmdLCD(CLEAR_LCD);

                return;

            default:
                CmdLCD(CLEAR_LCD);
                StrLCD("Invalid Option");
                MenuDelay();
                break;
        }
    }
}


/*------------------------------------------------
  EDIT ARRIVAL TIME
------------------------------------------------*/
void EditArrivalTime(void)
{
    u32 trainNo;
    u32 hour;
    u32 min;
    TB *train;

    CmdLCD(CLEAR_LCD);
    StrLCD("Train No:");
    CmdLCD(GOTO_LINE2_POS0);

    trainNo = ReadNumLCD(9);

    train = FindTrain(trainNo);

    if(train == 0)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Train Not Found");
        MenuDelay();
        return;
    }

    /* Hour */
    CmdLCD(CLEAR_LCD);
    StrLCD("Arrival Hour:");
    CmdLCD(GOTO_LINE2_POS0);

    hour = ReadNumLCD(0);

    if(hour > 23)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Invalid Hour");
        MenuDelay();
        return;
    }

    /* Minute */
    CmdLCD(CLEAR_LCD);
    StrLCD("Arrival Min:");
    CmdLCD(GOTO_LINE2_POS0);

    min = ReadNumLCD(0);

    if(min > 59)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Invalid Minute");
        MenuDelay();
        return;
    }

    /* Check arrival < departure */
    if(!IsValidTrainTime(hour, min,
                         train->depHour,
                         train->depMin))
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Invalid Arrival");
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("Before Departure");
        MenuDelay();
        return;
    }

    train->arrHour = hour;
    train->arrMin = min;

    TrainInfoChanged = 1;
    CurrentTrain = train;
    ForceDisplay = 1;

    CmdLCD(CLEAR_LCD);
    StrLCD("Arrival Updated");

    Buzzer_Alert();
    MenuDelay();
}


/*------------------------------------------------
  EDIT DEPARTURE TIME
------------------------------------------------*/
void EditDepartureTime(void)
{
    u32 trainNo;
    u32 hour;
    u32 min;
    TB *train;

    CmdLCD(CLEAR_LCD);
    StrLCD("Train No:");
    CmdLCD(GOTO_LINE2_POS0);

    trainNo = ReadNumLCD(9);

    train = FindTrain(trainNo);

    if(train == 0)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Train Not Found");
        MenuDelay();
        return;
    }

    /* Hour */
    CmdLCD(CLEAR_LCD);
    StrLCD("Departure Hour:");
    CmdLCD(GOTO_LINE2_POS0);

    hour = ReadNumLCD(0);

    if(hour > 23)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Invalid Hour");
        MenuDelay();
        return;
    }

    /* Minute */
    CmdLCD(CLEAR_LCD);
    StrLCD("Departure Min:");
    CmdLCD(GOTO_LINE2_POS0);

    min = ReadNumLCD(0);

    if(min > 59)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Invalid Minute");
        MenuDelay();
        return;
    }

    /* Check departure > arrival */
    if(!IsValidTrainTime(train->arrHour,
                         train->arrMin,
                         hour,
                         min))
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Invalid Departure");
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("After Arrival");
        MenuDelay();
        return;
    }

    train->depHour = hour;
    train->depMin = min;

    TrainInfoChanged = 1;
    CurrentTrain = train;
    ForceDisplay = 1;

    CmdLCD(CLEAR_LCD);
    StrLCD("Departure Updated");

    Buzzer_Alert();
    MenuDelay();
}


/*------------------------------------------------
  EDIT DELAY
------------------------------------------------*/
void EditDelayTime(void)
{
    u32 trainNo;
    u32 delay;
    TB *train;

    CmdLCD(CLEAR_LCD);
    StrLCD("Train No:");
    CmdLCD(GOTO_LINE2_POS0);

    trainNo = ReadNumLCD(9);

    train = FindTrain(trainNo);

    if(train == 0)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Train Not Found");
        MenuDelay();
        return;
    }

    CmdLCD(CLEAR_LCD);
    StrLCD("Delay Minutes:");
    CmdLCD(GOTO_LINE2_POS0);

    delay = ReadNumLCD(0);

    if(delay > 999)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Delay > 999");
        MenuDelay();
        return;
    }

    train->delayMin = delay;

    if(delay == 0)
        train->status = 0;
    else
        train->status = 1;

    TrainInfoChanged = 1;

    CurrentTrain = train;
    ForceDisplay = 1;

    ShowDelayMessage = 1;
    DelayDisplayMinute = delay;

    CmdLCD(CLEAR_LCD);
    StrLCD("Delay Updated");

    Buzzer_Alert();
    MenuDelay();

    ForceTrainRefresh();
}


/*------------------------------------------------
  EDIT PLATFORM
------------------------------------------------*/
void EditPlatform(void)
{
    u32 trainNo;
    u32 platform;
    TB *train;

    CmdLCD(CLEAR_LCD);
    StrLCD("Train No:");
    CmdLCD(GOTO_LINE2_POS0);

    trainNo = ReadNumLCD(9);

    train = FindTrain(trainNo);

    if(train == 0)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Train Not Found");
        MenuDelay();
        return;
    }

    CmdLCD(CLEAR_LCD);
    StrLCD("Platform 1-9:");
    CmdLCD(GOTO_LINE2_POS0);

    platform = ReadNumLCD(0);

    if(platform < 1 || platform > 9)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Invalid Platform");
        MenuDelay();
        return;
    }

    train->platform = platform;

    PlatformChanged = 1;
    TrainInfoChanged = 1;

    CurrentTrain = train;
    ForceDisplay = 1;

    CmdLCD(CLEAR_LCD);
    StrLCD("Platform Updated");

    Buzzer_Alert();
    MenuDelay();

    ResetTrainDisplay();
}

