//eint_interrupt.c

#include <LPC21xx.h>
#include "project_types.h"

#define EINT0_CH 14

void eint0_isr(void) __irq;

extern volatile u8 EditMode;

volatile u8 edit_request = 0;

void Init_EINT(void)
{
    PINSEL0 &= ~(3 << 2);
    PINSEL0 |= (3 << 2);

    EXTINT = (1 << 0);

    EXTMODE |= (1 << 0);

    EXTPOLAR &= ~(1 << 0);

    VICIntSelect &= ~(1 << EINT0_CH);

    VICVectAddr0 = (unsigned int)eint0_isr;

    VICVectCntl0 = (1 << 5) | EINT0_CH;

    VICIntEnable = (1 << EINT0_CH);
}

void eint0_isr(void) __irq
{
    edit_request = 1;

    EXTINT = 1 << 0;

    VICVectAddr = 0;
}

