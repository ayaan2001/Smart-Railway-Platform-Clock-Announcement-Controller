//train_info_display.c

#include "project_delay.h"
#include "project_lcd.h"
#include "project_lcd_defines.h"
#include "project_train_info.h"
#include "project_buzzer.h"

static int pos = 10;
static int first = 1;
static u32 lastTrainNo = 0xFFFFFFFF;
static u8 scrollMode = 0;

int my_strlen(char *str)
{
    int len = 0;

    while(*str)
    {
        len++;
        str++;
    }

    return len;
}

void ForceTrainRefresh(void)
{
    ResetTrainDisplay();
    lastTrainNo = 0;
}

void ResetTrainDisplay(void)
{
    pos = 10;
    first = 1;
		scrollMode=0;
}

void DisplayTrain(u32 trainNo,char *trainName,char *dest,u8 platform,
                  u8 arrHour,u8 arrMin,u8 depHour,u8 depMin)
{
    char disp[11] = "          ";
    char *scrollText;
    int len;
    int i;
    int index;

    if(lastTrainNo != trainNo)
    {
        lastTrainNo = trainNo;
        ResetTrainDisplay();
        CmdLCD(CLEAR_LCD);
    }

    if(scrollMode == 0)
    {
        scrollText = trainName;
    }
    else
    {
        scrollText = dest;
    }

    len = my_strlen(scrollText);

    if(first)
    {
        LCD_GotoXY(0,0);
        U32LCD(trainNo);
				LCD_CharXY(0,5,'<');

        LCD_CharXY(1,0,'P');
        LCD_GotoXY(1,1);
        U32LCD(platform);

        LCD_CharXY(1,3,'A');
        LCD_CharXY(1,4,(arrHour/10)+'0');
        LCD_CharXY(1,5,(arrHour%10)+'0');
        LCD_CharXY(1,6,':');
        LCD_CharXY(1,7,(arrMin/10)+'0');
        LCD_CharXY(1,8,(arrMin%10)+'0');

        LCD_CharXY(1,10,'D');
        LCD_CharXY(1,11,(depHour/10)+'0');
        LCD_CharXY(1,12,(depHour%10)+'0');
        LCD_CharXY(1,13,':');
        LCD_CharXY(1,14,(depMin/10)+'0');
        LCD_CharXY(1,15,(depMin%10)+'0');

        first = 0;
    }

    for(i=0; i<10; i++)
    {
        index = i - pos;

        if(index >= 0 && index < len)
        {
            disp[i] = scrollText[index];
        }
        else
        {
            disp[i] = ' ';
        }
    }

    disp[10] = '\0';

    LCD_StringXY(0,6,disp);

    #ifdef HARDWARE
				delay_ms(500);
		#else
				delay_ms(500);
		#endif

    pos--;

    if(pos < -len)
    {
        pos = 10;

        if(scrollMode == 0)
        {
            scrollMode = 1;
        }
        else
        {
            scrollMode = 0;
        }

        LCD_StringXY(0,6,"          ");
    }
}

void WelcomeScreen(void)
{
    char msg[] =
        "Smart Railway Platform Clock Announcement Controller";

    char disp[17] = "          ";

    int pos = 16;
    int len;
    int i;
    int index;
    int count = 0;

    len = my_strlen(msg);

    CmdLCD(CLEAR_LCD);

    LCD_GotoXY(0,0);
    StrLCD("   Welcome To   ");

    while(count < 70)
    {
        for(i=0; i<16; i++)
        {
            disp[i] = ' ';
        }

        disp[16] = '\0';

        for(i=0; i<16; i++)
        {
            index = i - pos;

            if(index >= 0 && index < len)
            {
                disp[i] = msg[index];
            }
            else
            {
                disp[i] = ' ';
            }
        }

        LCD_StringXY(1,0,disp);

        #ifdef HARDWARE
						delay_ms(400);
				#else
						delay_ms(500);
				#endif

        pos--;

        count++;
    }

    CmdLCD(CLEAR_LCD);
}

void CheckTrainApproaching(u8 currentHour,u8 currentMin,u8 arrHour,u8 arrMin)
{
    u32 currentTotal;
    u32 arrivalTotal;
    u32 difference;

    static u8 buzzerDone = 0;

    currentTotal = currentHour * 60 + currentMin;

    arrivalTotal = arrHour * 60 + arrMin;

    if(arrivalTotal < currentTotal)
    {
        arrivalTotal += 24 * 60;
    }

    difference = arrivalTotal - currentTotal;

    if(difference == 5)
    {
        if(buzzerDone == 0)
        {
            Buzzer_Alert();

            buzzerDone = 1;
        }
    }

    if(difference > 5)
    {
        buzzerDone = 0;
    }
}
void DisplayTrainDetails(u8 platform,
                         u8 arrHour,
                         u8 arrMin,
                         u8 depHour,
                         u8 depMin)
{
    LCD_CharXY(1,0,'P');

    LCD_GotoXY(1,1);
		U32LCD(platform);

    LCD_CharXY(1,3,'A');

    LCD_CharXY(1,4,(arrHour/10)+'0');
    LCD_CharXY(1,5,(arrHour%10)+'0');
    LCD_CharXY(1,6,':');
    LCD_CharXY(1,7,(arrMin/10)+'0');
    LCD_CharXY(1,8,(arrMin%10)+'0');

    LCD_CharXY(1,10,'D');

    LCD_CharXY(1,11,(depHour/10)+'0');
    LCD_CharXY(1,12,(depHour%10)+'0');
    LCD_CharXY(1,13,':');
    LCD_CharXY(1,14,(depMin/10)+'0');
    LCD_CharXY(1,15,(depMin%10)+'0');
}

void DisplayTrainRTC(u8 hour,u8 min,u8 sec,u16 delayMin)
{
    LCD_CharXY(1,0,(hour/10)+'0');
    LCD_CharXY(1,1,(hour%10)+'0');
    LCD_CharXY(1,2,':');

    LCD_CharXY(1,3,(min/10)+'0');
    LCD_CharXY(1,4,(min%10)+'0');
    LCD_CharXY(1,5,':');

    LCD_CharXY(1,6,(sec/10)+'0');
    LCD_CharXY(1,7,(sec%10)+'0');
	
		LCD_StringXY(1,9,"DY:");
	
		LCD_CharXY(1,12,(delayMin/100)+'0');
    LCD_CharXY(1,13,((delayMin/10)%10)+'0');
		LCD_CharXY(1,14,(delayMin%10)+'0');
		
		LCD_StringXY(1,15,"M");
}

