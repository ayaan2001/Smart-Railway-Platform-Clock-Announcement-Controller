//buzzer.c

#include <LPC21xx.h>
#include "project_buzzer.h"
#include "project_delay.h"
#include "project_defines.h"

void Buzzer_Init(void)
{
    IODIR0 |= (1 << BUZZER);
    IOCLR0 = (1 << BUZZER);
}

void Buzzer_On(void)
{
    IOSET0 = (1 << BUZZER);
}

void Buzzer_Off(void)
{
    IOCLR0 = (1 << BUZZER);
}

void Buzzer_Beep(void)
{
    Buzzer_On();
	
    #ifdef HARDWARE
				delay_ms(500);
		#else
				delay_ms(100);
		#endif
	
    Buzzer_Off();
	
    #ifdef HARDWARE
				delay_ms(500);
		#else
				delay_ms(100);
		#endif
	
}

void Buzzer_Alert(void)
{
    Buzzer_Beep();
    Buzzer_Beep();
    Buzzer_Beep();
}

