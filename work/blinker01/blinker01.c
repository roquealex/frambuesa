/*----------------------------------------------------------------
//                                                              //
//  hello-world.c                                               //
//                                                              //
//  This file is part of the Amber project                      //
//  http://www.opencores.org/project,amber                      //
//                                                              //
//  Description                                                 //
//  Simple stand-alone example application.                     //
//                                                              //
//  Author(s):                                                  //
//      - Conor Santifort, csantifort.amber@gmail.com           //
//                                                              //
//////////////////////////////////////////////////////////////////
//                                                              //
// Copyright (C) 2010 Authors and OPENCORES.ORG                 //
//                                                              //
// This source file may be used and distributed without         //
// restriction provided that this copyright statement is not    //
// removed from the file and that any derivative work contains  //
// the original copyright notice and the associated disclaimer. //
//                                                              //
// This source file is free software; you can redistribute it   //
// and/or modify it under the terms of the GNU Lesser General   //
// Public License as published by the Free Software Foundation; //
// either version 2.1 of the License, or (at your option) any   //
// later version.                                               //
//                                                              //
// This source is distributed in the hope that it will be       //
// useful, but WITHOUT ANY WARRANTY; without even the implied   //
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      //
// PURPOSE.  See the GNU Lesser General Public License for more //
// details.                                                     //
//                                                              //
// You should have received a copy of the GNU Lesser General    //
// Public License along with this source; if not, download it   //
// from http://www.opencores.org/lgpl.shtml                     //
//                                                              //
----------------------------------------------------------------*/



/* Note that the stdio.h referred to here is the one in
   mini-libc. This applications compiles in mini-libc
   so it can run stand-alone.
*/   
//#include "stdio.h"
//#include <stdio.h>
//#include <stdlib.h>


/*
#ifdef MINIMIPS
	#define USER_END '\r'
#else
	#define USER_END '\n'
#endif

#define USER_END '\n'


char str_buffer[12];

char str_hello[] = "Hi Hi!\n";

void main () 
{
    printf ("Clinker program\n");
    printf ("Compiled from dell\n");
	printf ("               \n");
	_outbyte('e');
	_outbyte('n');
	_outbyte('d');
	//return 0;

}
*/

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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "rpi-gpio.h"
#include "rpi-systimer.h"

/** GPIO Register set */
volatile unsigned int* gpio = (unsigned int*)GPIO_BASE;

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
    uint32_t gpio_setclr_num;
    uint32_t gpio_set_reg;
    uint32_t gpio_clr_reg;
    uint32_t gpio_setclr_bit;

    // The floating point unit doesn't need to be enable for the following
    // calculations because the compiler solves the operations at compile
    // time using only constants

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
    gpio_setclr_num = (gpio_num/32);
    gpio_set_reg = gpio_setclr_num + GPIO_GPSET0;
    gpio_clr_reg = gpio_setclr_num + GPIO_GPCLR0;
    gpio_setclr_bit = (gpio_num%32);
    /* Write 1 to the GPIO16 init nibble in the Function Select 1 GPIO
       peripheral register to enable GPIO16 as an output */
    //gpio[GPIO_GPFSEL1] |= (1 << 18);

    // Read modify write routine:
    gpio_fsel_regval = gpio[gpio_fsel_reg];
    // programming 3'b001 to the right field
    gpio_fsel_regval &= ~(0x7 << gpio_fsel_lsb);
    gpio_fsel_regval |= (1 << gpio_fsel_lsb);
    gpio[gpio_fsel_reg] = gpio_fsel_regval;

    printf ("Blinker program\n");
    printf ("GPIO offset %d\n",gpio_fsel_reg);
    printf ("GPIO FSEL LSB %d\n",gpio_fsel_lsb);
    printf ("Value to program %08x\n",gpio_fsel_regval);
    //printf ("Old test GPIO offset %d\n", GPIO_GPFSEL1);
    //printf ("Old test Value %08x\n", gpio[GPIO_GPFSEL1]);
    printf ("GPIO set/clr num %d\n", gpio_setclr_num);
    printf ("GPIO set reg %d\n", gpio_set_reg);
    printf ("GPIO clr reg %d\n", gpio_clr_reg);
    printf ("GPIO set/clr bit %d\n", gpio_setclr_bit);

    printf ("Period %d us\n",period);
    printf ("Duty Cycle %d %\n",(int)(duty_cycle*100));
    printf ("Hi time %d us\n",hi_time);
    printf ("Low time %d us\n",lo_time);

    /* Never exit as there is no OS to exit to! */
    while(1)
    {

        /* Set the GPIO16 output high ( Turn OK LED off )*/
        //gpio[GPIO_GPSET0] = (1 << 16);
        gpio[gpio_set_reg] = (1 << gpio_setclr_bit);

        /* Wait half a second */
        //RPI_WaitMicroSeconds( 500000 );
        RPI_WaitMicroSeconds( hi_time );

        /* Set the GPIO16 output low ( Turn OK LED on )*/
        //gpio[GPIO_GPCLR0] = (1 << 16);
        gpio[gpio_clr_reg] = (1 << gpio_setclr_bit);

        /* Wait half a second */
        //RPI_WaitMicroSeconds( 500000 );
        RPI_WaitMicroSeconds( lo_time );
    }
}
