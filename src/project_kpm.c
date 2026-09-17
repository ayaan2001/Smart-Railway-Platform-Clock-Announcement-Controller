//kpm.c

#include <LPC21xx.h>
#include "project_types.h"
#include "project_kpm_defines.h"
#include "project_defines.h"
#include "project_kpm.h"
#include "project_lcd_defines.h"
#include "project_lcd.h"

u8 kpmLUT[4][4] =
{
    {'7','8','9','B'},
    {'4','5','6','/'},
    {'1','2','3','-'},
    {'C','0','E','+'}
};

void Init_KPM(void)
{
    WRITENIBBLE(IODIR1,ROW0,15);
}

u32 colscan(void)
{
    if(READNIBBLE(IOPIN1,COL0) < 15)
        return 0;
    else
        return 1;
}

u32 rowcheck(void)
{
    u32 rno;

    for(rno=0;rno<4;rno++)
    {
        WRITENIBBLE(IOPIN1,ROW0,(~(1<<rno)));

        if(colscan() == 0)
            break;
    }

    IOCLR1 = 15 << ROW0;

    return rno;
}

u32 colcheck(void)
{
    u32 cno;

    for(cno=0;cno<4;cno++)
    {
        if(READBIT(IOPIN1,(cno+COL0)) == 0)
            break;
    }

    return cno;
}

u32 keyscan(void)
{
    u32 row;
    u32 col;
    u32 key;

    while(colscan());

    row = rowcheck();
    col = colcheck();

    key = kpmLUT[row][col];

    while(!colscan());

    return key;
}

u32 readnum(void)
{
    u32 num = 0;
    u8 key;

    while(1)
    {
        key = keyscan();

        if(key >= '0' && key <= '9')
        {
            num = (num * 10) + (key - 48);
        }
        else
        {
            break;
        }
    }

    return num;
}

u32 ReadNumLCD(u8 p)
{
    u32 num = 0;
    u8 key;
    u8 digits = 0;
    u8 pos = p;

    CmdLCD(GOTO_LINE2_POS0);

    while(1)
    {
        key = keyscan();

        if(key >= '0' && key <= '9')
        {
            num = (num * 10) + (key - '0');

            LCD_CharXY(1,pos,key);

            pos++;
            digits++;
        }

        else if(key == KEY_BACKSPACE)
        {
            if(digits > 0)
            {
                num = num / 10;

                digits--;

                pos--;

                LCD_CharXY(1,pos,' ');

                LCD_GotoXY(1,pos);
            }
        }

        else if(key == KEY_CLEAR)
        {
            num = 0;
            digits = 0;
            pos = 0;

            CmdLCD(GOTO_LINE2_POS0);
            StrLCD("                ");
        }

        else if(key == KEY_ENTER)
        {
            break;
        }
    }

    return num;
}

