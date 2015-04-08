

/*
    Part of the Raspberry-Pi Bare Metal Tutorials
    Copyright (c) 2013, Brian Sidebotham
    All rights reserved.
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright notice,
        this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
        this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.
*/

/*
    C-Library stubs introduced for newlib
*/

#include "rpi-pwm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "rpi-gpio.h"
#include "rpi-systimer.h"

#define BCM2708_PERI_BASE       0x20000000
//#define GPIO_BASE               (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */
//#define PWM_BASE                (BCM2708_PERI_BASE + 0x20C000) /* PWM controller */
#define CLOCK_BASE              (BCM2708_PERI_BASE + 0x101000)
#define PWMCLK_CNTL 40
#define PWMCLK_DIV  41


/** GPIO Register set */
volatile unsigned int* gpio = (unsigned int*)GPIO_BASE;
volatile unsigned int* pwm = (unsigned int*)PWM_BASE;
volatile unsigned int* clk = (unsigned int*)CLOCK_BASE;

/** Main function - we'll never return from here */
//void kernel_main( unsigned int r0, unsigned int r1, unsigned int atags )
void main () 
{
    int loop;
    uint32_t period;
    uint32_t hi_time;
    uint32_t lo_time;
    float duty_cycle;

    uint32_t gpio_num;
    uint32_t gpio_fsel_reg;
    uint32_t gpio_fsel_lsb;
    uint32_t gpio_fsel_regval;

	uint32_t range;

    uint32_t pwm_clk_freq;
    uint32_t bits_per_sample;
    uint32_t sampling_rate;

    // The floating point unit doesn't need to be enable for the following
    // calculations because the compiler solves the operations at compile
    // time using only constants

    // Program GPIO18 alternate function 5

    // period in microseconds:
    //period = 1000000;
    //period = 2272; //440hz
    //period = 4545;
    //period = 2272; //440hz
    period = 23; // As close as I could get to 44 Khz probably totally inacurate
    //duty_cycle = 0.50;
    duty_cycle = 0.25;
    //gpio_num = 16; // Led
    gpio_num = 18; // PWM in different location
    //gpio_num = 40; // PWM Audio left
    //gpio_num = 45; // PWM Audio right


    // Calculations:
    hi_time = period * duty_cycle;
    lo_time = period - hi_time;
	

    gpio_fsel_reg = (gpio_num/10) + GPIO_GPFSEL0 ;
    gpio_fsel_lsb = (gpio_num%10) * 3 ;


    /* Write 1 to the GPIO16 init nibble in the Function Select 1 GPIO
       peripheral register to enable GPIO16 as an output */
    //gpio[GPIO_GPFSEL1] |= (1 << 18);

    // Read modify write routine:
    gpio_fsel_regval = gpio[gpio_fsel_reg];
    // programming 3'b001 to the right field
    gpio_fsel_regval &= ~(0x7 << gpio_fsel_lsb);
    gpio_fsel_regval |= (0x2 << gpio_fsel_lsb);
    gpio[gpio_fsel_reg] = gpio_fsel_regval;

/*
uint32_t MSEN1 : 1; // Channel 1 M/S Enable g
uint32_t CLRF1 : 1; // Clear Fifo 1: Clears FIFO 0: Has no effect g
uint32_t USEF1 : 1; // Channel 1 Use Fifo g
uint32_t POLA1 : 1; // Channel 1 Polarity g
uint32_t SBIT1 : 1; // Channel 1 Silence Bit g
uint32_t RPTL1 : 1; // Channel 1 Repeat Last Data g
uint32_t MODE1 : 1; // Channel 1 Mode 0: PWM mode 1: Serialiser mode g
uint32_t PWEN1 : 1; // Channel 1 Enable g
*/
//1110_0101
//0110_0101 // reg pwm
//0010_0101 // reg pwm

    printf ("PWM program\n");
    printf ("GPIO offset %d\n",gpio_fsel_reg);
    printf ("GPIO FSEL LSB %d\n",gpio_fsel_lsb);
    printf ("Value to program %08x\n",gpio_fsel_regval);
    //printf ("Old test GPIO offset %d\n", GPIO_GPFSEL1);
    //printf ("Old test Value %08x\n", gpio[GPIO_GPFSEL1]);


       // stop clock and waiting for busy flag doesn't work, so kill clock
        *(clk + PWMCLK_CNTL) = 0x5A000000 | (1 << 5);
        //usleep(10);
        RPI_WaitMicroSeconds( 100 );

        // set frequency
        // DIVI is the integer part of the divisor
        // the fractional part (DIVF) drops clock cycles to get the output frequency, bad for servo motors
        // 320 bits for one cycle of 20 milliseconds = 62.5 us per bit = 16 kHz
	// 65536 bits for 1 cycle of 22.72 us
        // 4 bits for one cycle of 0.25 milliseconds = 62.5 us per bit = 16 kHz
        //int idiv = (int) (19200000.0f / 16000.0f);
        int idiv = (int) (19200000.0f / 32000.0f); // 8 range
        //int idiv = (int) (19200000.0f / 64000.0f); // 16 crange
        //int idiv = (int) (19200000.0f / 5120000.0f);
        //int idiv = 2;
        if (idiv < 1 || idiv > 0x1000) {
                printf("idiv out of range: %x\n", idiv);
		while(1);
        }
        *(clk + PWMCLK_DIV)  = 0x5A000000 | (idiv<<12);

        // source=osc and enable clock
        //*(clk + PWMCLK_CNTL) = 0x5A000011; // 19.2 Mhz
        *(clk + PWMCLK_CNTL) = 0x5A000015; // 1000Mhz
        //*(clk + PWMCLK_CNTL) = 0x5A000014; // none
        //*(clk + PWMCLK_CNTL) = 0x5A000016; // 500Mhz


        // disable PWM
        *(pwm + PWM_CTL) = 0;

        RPI_WaitMicroSeconds( 100 );

    //pwm[PWM_RNG1] = 0x0000FFFF;
    //pwm[PWM_RNG1] = 3200;
    //pwm[PWM_DAT1] = 1600;
	range = 8000;
    pwm[PWM_RNG1] = range;
    pwm[PWM_RNG2] = range;
    pwm[PWM_DAT1] = 1;
    //pwm[PWM_RNG1] = 0x0000FFFF;
    //pwm[PWM_DAT1] = 0x7fff;

    //pwm[PWM_CTL] = 0x00000085; // no fifo m/s
    //pwm[PWM_CTL] = 0x00000005; // no fifo regular pwm
    //pwm[PWM_CTL] = 0x000000e5; // fifo m/s
    //pwm[PWM_CTL] = 0x000000e1; // fifo m/s dont repeat 
    //pwm[PWM_CTL] = 0x00000065; // fifo reg pwm
    //pwm[PWM_CTL] = 0x00000061; // fifo reg pwm no repeat
    //pwm[PWM_CTL] = 0x00002565; // dual channel fifo m/s (repeat twice bug)
    //pwm[PWM_CTL] = 0x00002161; // dual channel fifo m/s
    pwm[PWM_CTL] = 0x000021e1; // dual channel fifo m/s

    //pwm[PWM_CTL] = 0x00000085;
    //pwm[PWM_RNG1] = 0x0000FFFF;
    //pwm[PWM_CTL] = 0x000000e5;
    //pwm[PWM_DAT1] = 0x00007FFF;
    //pwm[PWM_FIF1] = 0x00007FFF;

    printf ("Period %d us\n",period);
    printf ("Duty Cycle %d %\n",(int)(duty_cycle*100));
    printf ("Hi time %d us\n",hi_time);
    printf ("Low time %d us\n",lo_time);

    printf ("Range %d \n",range);
    /* Never exit as there is no OS to exit to! */
    //loop = range/2;
    loop = 1;
    while(1)
    {

	    //pwm[PWM_DAT1] = loop;
	while((pwm[PWM_STA] & 0x01) == 1);
    //pwm[PWM_FIF1] = loop;
    pwm[PWM_FIF1] = range>>1;
    //pwm[PWM_FIF1] = 1;
        //RPI_WaitMicroSeconds( 2000000 );
	loop++;
	//if (loop > range) loop = 0;
	if (loop == range) loop = 1;

        //RPI_WaitMicroSeconds( 500000 );
        //RPI_WaitMicroSeconds( 500000 );
        //RPI_WaitMicroSeconds( lo_time );
    }
}
