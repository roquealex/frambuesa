#include <stdint.h>
#include "rpi-systimer.h"

static rpi_sys_timer_t* rpiSystemTimer = (rpi_sys_timer_t*)RPI_SYSTIMER_BASE;

rpi_sys_timer_t* RPI_GetSystemTimer(void)
{
    return rpiSystemTimer;
}

void RPI_WaitMicroSeconds( uint32_t us )
{
    volatile uint32_t ts = rpiSystemTimer->counter_lo;

    while( ( rpiSystemTimer->counter_lo - ts ) < us )
    {
        /* BLANK */
    }
}

uint32_t RPI_GetTimeStamp( void )
{
	//static uint32_t ts = 0;
	//int r =rand()%400;
	//int r =23;
    //ts += (0x20000000 + r);
    
    volatile uint32_t ts = rpiSystemTimer->counter_lo;
    return ts;
}
