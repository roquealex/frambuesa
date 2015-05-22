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

#ifndef RPI_PWM_H
#define RPI_PWM_H

#include <stdint.h>
/** The base address of the PWM peripheral (ARM Physical Address) */
#define PWM_BASE       0x2020C000UL
//7e20 0000

//PWM Control 32
#define PWM_CTL 0
//PWM Status 32
#define PWM_STA 1
//PWM DMA Configuration 32
#define PWM_DMAC 2
//PWM Channel 1 Range 32
#define PWM_RNG1 4
//PWM Channel 1 Data 32
#define PWM_DAT1 5
//PWM FIFO Input 32
#define PWM_FIF1 6
//PWM Channel 2 Range 32
#define PWM_RNG2 8
//PWM Channel 2 Data 32
#define PWM_DAT2 9

typedef struct {
uint32_t Reserved1 : 16;
uint32_t MSEN2 : 1; /* Channel 2 M/S Enable */
uint32_t Reserved0 : 1;
uint32_t USEF2 : 1; /* Channel 2 Use Fifo */
uint32_t POLA2 : 1; /* Channel 2 Polarity */
uint32_t SBIT2 : 1; /* Channel 2 Silence Bit */
uint32_t RPTL2 : 1; /* Channel 2 Repeat Last Data */
uint32_t MODE2 : 1; /* Channel 2 Mode 0: PWM mode 1: Serialiser */
uint32_t PWEN2 : 1; /* Channel 2 Enable */
uint32_t MSEN1 : 1; /* Channel 1 M/S Enable */
uint32_t CLRF1 : 1; /* Clear Fifo 1: Clears FIFO 0: Has no effect */
uint32_t USEF1 : 1; /* Channel 1 Use Fifo */
uint32_t POLA1 : 1; /* Channel 1 Polarity */
uint32_t SBIT1 : 1; /* Channel 1 Silence Bit */
uint32_t RPTL1 : 1; /* Channel 1 Repeat Last Data */
uint32_t MODE1 : 1; /* Channel 1 Mode 0: PWM mode 1: Serialiser mode */
uint32_t PWEN1 : 1; /* Channel 1 Enable */
} rpi_pwm_ctl_t;

#define PWM_CTL_MSEN2 (1<<15) /* Channel 2 M/S Enable */
#define PWM_CTL_USEF2 (1<<13) /* Channel 2 Use Fifo */
#define PWM_CTL_POLA2 (1<<12) /* Channel 2 Polarity */
#define PWM_CTL_SBIT2 (1<<11) /* Channel 2 Silence Bit */
#define PWM_CTL_RPTL2 (1<<10) /* Channel 2 Repeat Last Data */
#define PWM_CTL_MODE2 (1<<9) /* Channel 2 Mode 0: PWM mode 1: Serialiser */
#define PWM_CTL_PWEN2 (1<<8) /* Channel 2 Enable */
#define PWM_CTL_MSEN1 (1<<7) /* Channel 1 M/S Enable */
#define PWM_CTL_CLRF1 (1<<6) /* Clear Fifo 1: Clears FIFO 0: Has no effect */
#define PWM_CTL_USEF1 (1<<5) /* Channel 1 Use Fifo */
#define PWM_CTL_POLA1 (1<<4) /* Channel 1 Polarity */
#define PWM_CTL_SBIT1 (1<<3) /* Channel 1 Silence Bit */
#define PWM_CTL_RPTL1 (1<<2) /* Channel 1 Repeat Last Data */
#define PWM_CTL_MODE1 (1<<1) /* Channel 1 Mode 0: PWM mode 1: Serialiser mode */
#define PWM_CTL_PWEN1 (1<<0) /* Channel 1 Enable */

typedef union {
uint32_t val;
rpi_pwm_ctl_t fields;
} rpi_pwm_ctl_reg_t;


#define PWM_DMAC_ENAB (0x01UL<<31) /* DMA Enable */
//15:8 PANIC DMA Threshold for PANIC signal RW 0x7
//7:0 DREQ DMA Threshold for DREQ signal RW 0x7

#endif
