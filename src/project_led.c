#include <LPC21xx.h>
#include "project_types.h"
#include "project_train_db.h"

extern u8 TrainInfoActive;

#define GREEN_LED   16
#define YELLOW_LED  17
#define RED_LED     18


void InitLED(void)
{
    /* P0.23 = GREEN */
    IODIR0 |= (1 << GREEN_LED);

    /* P0.24 = YELLOW */
    IODIR0 |= (1 << YELLOW_LED);

    /* P0.25 = RED */
    IODIR0 |= (1 << RED_LED);

    /* Initially all LEDs OFF */
    IOCLR0 = (1 << GREEN_LED) |
             (1 << YELLOW_LED) |
             (1 << RED_LED);
}


void UpdateTrainStatusLED(u8 hour, u8 min, TB *train)
{
    u16 now;
    u16 arr;
    u16 dep;
    u16 yellowStart;


    /*----------------------------------
      STEP 1: Turn OFF all LEDs
    ----------------------------------*/

    IOCLR0 = (1 << GREEN_LED) |
             (1 << YELLOW_LED) |
             (1 << RED_LED);


    /*----------------------------------
      STEP 2: No train
    ----------------------------------*/

    if(train == 0)
    {
        return;
    }


    /*----------------------------------
      STEP 3: Convert current time
      into total minutes
    ----------------------------------*/

    now = hour * 60 + min;


    /*----------------------------------
      STEP 4: Calculate arrival
      including delay
    ----------------------------------*/

    arr = train->arrHour * 60
        + train->arrMin
        + train->delayMin;


    /*----------------------------------
      STEP 5: Calculate departure
      including delay
    ----------------------------------*/

    dep = train->depHour * 60
        + train->depMin
        + train->delayMin;


    /*----------------------------------
      STEP 6: RED
      Train information changed
    ----------------------------------*/

    if(TrainInfoActive)
    {
        IOSET0 = (1 << RED_LED);
        return;
    }


    /*----------------------------------
      STEP 7: RED
      Train is delayed
    ----------------------------------*/

    if(train->delayMin > 0)
    {
        IOSET0 = (1 << RED_LED);
        return;
    }


    /*----------------------------------
      STEP 8: After departure
      All LEDs OFF
    ----------------------------------*/

    if(now >= dep)
    {
        PlatformChanged = 0;
        return;
    }


    /*----------------------------------
      STEP 9: GREEN
      Train is on platform
    ----------------------------------*/

    if(now >= arr && now < dep)
    {
        IOSET0 = (1 << GREEN_LED);
        return;
    }


    /*----------------------------------
      STEP 10: Calculate yellow start
      5 minutes before arrival
    ----------------------------------*/

    if(arr >= 5)
    {
        yellowStart = arr - 5;
    }
    else
    {
        yellowStart = 0;
    }


    /*----------------------------------
      STEP 11: YELLOW
      Train approaching
    ----------------------------------*/

    if(now >= yellowStart && now < arr)
    {
        IOSET0 = (1 << YELLOW_LED);
        return;
    }
}

