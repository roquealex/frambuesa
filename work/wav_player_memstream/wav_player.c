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
#include <stdio.h>
#include <stdlib.h>
#include "wav_player.h"
#include "rpi-pwm.h"

#include "rpi-dma.h"

#include "rpi-gpio.h"
#include "rpi-systimer.h"

#define BCM2708_PERI_BASE       0x20000000
//#define GPIO_BASE               (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */
//#define PWM_BASE                (BCM2708_PERI_BASE + 0x20C000) /* PWM controller */
#define CLOCK_BASE              (BCM2708_PERI_BASE + 0x101000)
#define PWMCLK_CNTL 40
#define PWMCLK_DIV  41

//#define AVOID_HW_WRITES

/** GPIO Register set */
volatile unsigned int* gpio = (unsigned int*)GPIO_BASE;
volatile unsigned int* clk = (unsigned int*)CLOCK_BASE;

#ifndef AVOID_HW_WRITES
volatile unsigned int* dma0 = (unsigned int*)DMA0_BASE;
volatile unsigned int* dma1 = (unsigned int*)DMA1_BASE;
volatile unsigned int *dma[2] = {(unsigned int*)DMA0_BASE, (unsigned int*)DMA1_BASE};
volatile unsigned int* pwm = (unsigned int*)PWM_BASE;
#else
volatile unsigned int dma0[20];
volatile unsigned int dma1[20];
volatile unsigned int *dma[2] = {dma0,dma1};
volatile unsigned int pwm[10];
#endif

/*
void print_spaces ( int num );
void print_help ( void );
void play_audio ( void );
void stop_audio ( void );
void load_audio ( void );


#define AMBER_AUDIO_SEQ_START_ADDRESS_OFF  0
#define AMBER_AUDIO_SEQ_SAMPLE_COUNT_OFF   1
#define AMBER_AUDIO_SEQ_CMD_OFF            2
#define AMBER_AUDIO_SEQ_STATUS_OFF         3
#define AMBER_AUDIO_SEQ_CHL_CONFIG_OFF     4
#define AMBER_AUDIO_SEQ_SAMPLE_RATE_OFF    5
*/


//volatile unsigned int* audio_seq = (unsigned int*)AMBER_AUDIO_SEQ_BASE;
//unsigned long audio_seq[8];
//20000000 

uint16_t *samples;
unsigned int num_samples = 0;
#define SAMPLE_BUFF_SIZE (1<<10)
// 64K
#define SAMPLE_BUFF_SIZE (1<<14)
// 256K
//#define SAMPLE_BUFF_SIZE (1<<16)
//2^20/2^2:
// 1 MB:
//#define SAMPLE_BUFF_SIZE (1<<18)
// 2 MB:
//#define SAMPLE_BUFF_SIZE (1<<19)
//#define SAMPLE_BUFF_SIZE (1<<5)
uint32_t sample_buffer[2][SAMPLE_BUFF_SIZE];


extern unsigned int AdrStack;
main () 
{
	char c;
	// Set up to mirror one of the PWM in GPIO:
	uint32_t gpio_num;
	uint32_t gpio_fsel_reg;
	uint32_t gpio_fsel_lsb;
	uint32_t gpio_fsel_regval;


	gpio_num = 18; // PWM in different location

	gpio_fsel_reg = (gpio_num/10) + GPIO_GPFSEL0 ;
	gpio_fsel_lsb = (gpio_num%10) * 3 ;


	// Read modify write routine:
	gpio_fsel_regval = gpio[gpio_fsel_reg];
	// programming 3'b001 to the right field
	gpio_fsel_regval &= ~(0x7 << gpio_fsel_lsb);
	gpio_fsel_regval |= (0x2 << gpio_fsel_lsb);
	gpio[gpio_fsel_reg] = gpio_fsel_regval;



    //printf ("Hello, World!\n");
	printf ("#     #     #     #     #\n");
	printf ("#  #  #    # #    #     #\n");
	printf ("#  #  #   #   #   #     #\n");
	printf ("#  #  #  #     #  #     #\n");
	printf ("#  #  #  #######   #   #\n");
	printf ("#  #  #  #     #    # #\n");
	printf (" ## ##   #     #     #\n");
	printf ("\nWAV Player (mem stream v1.0)\n\n");

	print_help();
	printf("Ready\n> ");

	do {
		printf("\n? ");
		//c = getchar();
		while ((c = _inbyte (DLY_1S)) == 0xFF) ;
		printf("c = %0x\n",c);
		switch (c) {
			case 'h':
				print_help();
				break;
			case 'p':
				play_audio();
				break;
			case 's':
				stop_audio();
				break;
			case 'l':
				//load_audio(AdrStack);
				load_audio(0x20000);
				break;

		}
		
	} while (c != 'x');
	printf ("END\n");
    /* Flush out UART FIFO */
    printf ("                ");
    //_testpass();
}


void play_audio ( void ){
	int i = 0;

	// Memory to hold the control blocks for DMA
	void *mem;
	dma_cb_t *cb_ptr;
	uint32_t align;

	printf("Playing\n");
	printf("Number of samples %d\n", num_samples);
	//num_samples = 20;
	/*
	pwm[PWM_CTL] = 0x00002161; // dual channel fifo m/s
	if (num_samples > 0 ) {
		for (i = 0 ; i < num_samples ; i ++) {
			uint16_t sample = samples[i] ^ 0x8000;
			while((pwm[PWM_STA] & 0x01) == 1);
			//pwm[PWM_FIF1] = samples[i]>>6;
			//pwm[PWM_FIF1] = sample>>6;
			sample>>=6;
			pwm[PWM_FIF1] = sample;
			//printf("sample %d = %x addr %0x\n",i,sample, &samples[i]);
		}
	}
	pwm[PWM_CTL] = 0;
	*/
	if (num_samples > 0 ) {
		unsigned int left_samples = num_samples;
		int buffer_num = 0;

		// Allocating space for 2 control blocks
		align = sizeof(dma_cb_t);
		printf("Alignment to be used %d\n",align);
		mem = malloc(2*sizeof(dma_cb_t) + (align-1));
		printf("Addr of mem = %x\n", (uintptr_t)mem);
		cb_ptr = (dma_cb_t*)( ((uintptr_t)mem+(align-1)) & ~(uintptr_t)(align-1) );
		printf("Addr of cb_ptr = %x\n", (uintptr_t)cb_ptr);

		//pwm[PWM_CTL] = 0x00002161; // dual channel fifo m/s
		pwm[PWM_CTL] = PWM_CTL_USEF2 | PWM_CTL_PWEN2 |
			PWM_CTL_CLRF1 | PWM_CTL_USEF1 | PWM_CTL_PWEN1 ;
		printf("PWM_CTL = %08x\n", pwm[PWM_CTL]);
		//pwm[PWM_DMAC] = PWMDMAC_ENAB | PWMDMAC_THRSHLD;
		pwm[PWM_DMAC] = 0x80000707;

		while (left_samples>0) {
			int size_of_loop = (SAMPLE_BUFF_SIZE > left_samples) ? left_samples : SAMPLE_BUFF_SIZE;
			int initial_idx = num_samples-left_samples ;
			//for (i = 0 ; i < num_samples ; i ++) {}
			// First create a buffer with enough samples:
			for (i = 0 ; i < size_of_loop ; i ++) {
				//uint16_t sample = samples[i] ^ 0x8000;
				uint16_t sample = samples[initial_idx+i] ^ 0x8000;
				sample>>=6;
				sample_buffer[buffer_num][i] = (uint32_t) sample;
			}
			left_samples -= size_of_loop;
			// Now play the buffer
			//cb_ptr[0].ti = DMA_S_INC | DMA_D_INC ;
			cb_ptr[buffer_num].ti = DMA_NO_WIDE_BURSTS | DMA_WAIT_RESP | DMA_S_INC | DMA_D_DREQ | (5<<16);
			//DMA_NO_WIDE_BURSTS | DMA_WAIT_RESP | DMA_D_DREQ | DMA_PER_MAP(5);
			//cb_ptr[0].ti = DMA_WAIT_RESP | DMA_S_INC | (5<<16);
			cb_ptr[buffer_num].source_ad = (uint32_t) &sample_buffer[buffer_num][0] ;
			cb_ptr[buffer_num].dest_ad = (((uint32_t) &pwm[PWM_FIF1])&0x00FFFFFF) | 0x7E000000;
			cb_ptr[buffer_num].txfr_len = size_of_loop*4; // size of the transffer is in bytes so x4
			cb_ptr[buffer_num].stride = 0;
			cb_ptr[buffer_num].nextconbk = 0;
			printf("Copying %d bytes from %x to %x\n",cb_ptr[buffer_num].txfr_len,cb_ptr[buffer_num].source_ad,cb_ptr[buffer_num].dest_ad);
			// This code will be substituted by dma:
			dma[buffer_num][DMA_CS] = DMA_CS_RESET;
#ifndef AVOID_HW_WRITES
			//sleep(1);
			RPI_WaitMicroSeconds(1000);
			printf("Reset done\n");
#endif

			dma[buffer_num][DMA_DEBUG] = DMA_DEBUG_READ_ERROR | DMA_DEBUG_FIFO_ERROR | DMA_DEBUG_READ_LAST_NOT_SET_ERROR; 
			//dma[buffer_num][DMA_CONBLK_AD] = (uint32_t)cb_ptr | 0xC0000000;
			dma[buffer_num][DMA_CONBLK_AD] = (uint32_t)&cb_ptr[buffer_num] ;
			// Starting the DMA engine:
			printf("DMA_CS = %08x\n", dma[buffer_num][DMA_CS]);
			// Don't do this wait on the first loop
			if (initial_idx != 0) {
				// Wait for the other DMA channel to finish to activate this one
				while(!(dma[buffer_num^1][DMA_CS] & DMA_CS_END));
				// Technically we need to write only the END bit so we
				// would need to apply an or mask but in reality we want
				// to write everything in zero for this particular case
				dma[buffer_num^1][DMA_CS] = DMA_CS_END;
			}
			// Play the next buffer
			dma[buffer_num][DMA_CS] = DMA_CS_ACTIVE;
			printf("DMA_CS = %08x\n", dma[buffer_num][DMA_CS]);

#ifdef AVOID_HW_WRITES
			// This code simulates the DMA transfer on emulator
			if ( 
				(dma[buffer_num][DMA_CS] & DMA_CS_ACTIVE) && 
				(pwm[PWM_CTL] & PWM_CTL_PWEN1) &&
				(pwm[PWM_CTL] & PWM_CTL_PWEN2) &&
				(pwm[PWM_DMAC] & PWM_DMAC_ENAB)
			) {
				dma_cb_t *cb_ptr_test;
				// Recover the pointer from DMA descriptor
				cb_ptr_test = (dma_cb_t*)dma[buffer_num][DMA_CONBLK_AD];
				for (i = 0 ; i < cb_ptr_test->txfr_len/4 ; i ++) {
					const int graph_width = 33;
					char graph[graph_width];
					int j;
					uint32_t sample = ((uint32_t*)cb_ptr_test->source_ad)[i];
					sample>>=5;
					for ( j = 0 ; j < (graph_width-1) ; j++) {
						if (j == sample) {
							graph[j] = '*';
						} else {
							graph[j] = ' ';
						}
					}
					graph[(graph_width-1)] = 0;
					//printf("%d\n", sample);
					//printf("%d\n", graph_width);
					if((i%2)==0) printf("%s", graph);
					else printf("%s\n", graph);
					
				}
				// Set the DMA done flag
				dma[buffer_num][DMA_CS] = DMA_CS_END;
			}
#endif

			//for (i = 0 ; i < 9 ; i++) {
			//RPI_WaitMicroSeconds(1000000);
			//printf("DMA_CS = %08x\n", dma[buffer_num][DMA_CS]);
			//}
			
			// Dont wait for it to finish anymore
			//while(!(dma[buffer_num][DMA_CS] & DMA_CS_END));

			//>printf("Finished DMA\n");
			//>printf("DMA_CS = %08x\n", dma[buffer_num][DMA_CS]);

			// Technically we need to write only the END bit so we
			// would need to apply an or mask but in reality we want
			// to write everything in zero for this particular case
			//dma[buffer_num][DMA_CS] = DMA_CS_END;
			
			//>printf("After clearing flag\n");
			//>printf("DMA_CS = %08x\n", dma[buffer_num][DMA_CS]);
			//while(1);

			//printf("Src %x and dest %x from registers\n",dma[buffer_num][DMA_SOURCE_AD],dma[buffer_num][DMA_DEST_AD]);
			// Toggle buffer number 0->1 or 1->0
			buffer_num ^= 1;

		}// while (left_samples>0) 
		// Stop PWM:
		printf("At the end Left samples %d\n", left_samples);
		pwm[PWM_CTL] = 0;
		free(mem);
		printf("Done Playing\n");
	} // if (num_samples > 0 ) 
}
void stop_audio ( void ){
	printf("Stopping\n");
}

//#define FAST_TEST
#ifdef FAST_TEST
char raw_data[] =
{
0x52, 0x49, 0x46, 0x46, 0xf8, 0xad, 0x36, 0x00, 0x57, 0x41, 0x56, 0x45, 0x66, 0x6d, 0x74, 0x20,
0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x44, 0xac, 0x00, 0x00, 0x10, 0xb1, 0x02, 0x00,
0x04, 0x00, 0x10, 0x00, 0x64, 0x61, 0x74, 0x61, 0xd4, 0xad, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00
//, 0xFF, 0xFF, 0xFF
};
#endif

void load_audio ( unsigned int address ){
	struct riff_header *riff_header_ptr;
	struct fmt *fmt_ptr;
	struct data *data_ptr;
	//unsigned int num_samples;
	void *sample_start;
	void *wav_start;
	unsigned int offset;
	int i;

	FILE *stream;

	char *wav_data;
	size_t wav_data_size;
	int file_size;

	int fullrows;
	int lastrow;
	int columns = 8;
	int j;
	int idx;

#ifdef FAST_TEST
	
	printf("Size of raw data: %d\n",sizeof(raw_data));
	//stream = open_memstream (&wav_data, &wav_data_size);
	//fprintf (stream, "hello");
	//fprintf (stream, ", world");
	for (i = 0; i < sizeof(raw_data) ; i++) {
		printf("raw data[%d]: %x\n",i,raw_data[i]);
		//if (i == 40)
		//	fputc(0xff,stream);
		//fputc(raw_data[i],stream);
		wav_data[i] = raw_data[i];
	}
	//fclose (stream);
	if (sizeof(raw_data) != wav_data_size) {
		printf("Size doesn't match : %d\n",wav_data_size);
	}
	for (i = 0; i < sizeof(raw_data) ; i++) {
		if (wav_data[i] != raw_data[i]) {
			printf("Difference in element %d: %x != %x\n",i,
				raw_data[i], wav_data[i]);
		}
	}
	//wav_data = (char *) 0x0FFF ;
	printf("Address given to wav_data %x\n",wav_data);
	//wav_data = raw_data;
	file_size = sizeof(raw_data);

#else

	wav_data = 0;
	printf("Loading Audio at address %0x\n", address);
	//file_size = xmodemReceive((char *) address, FILE_MAX_SIZE);   
	file_size = xmodemReceive(&wav_data, FILE_MAX_SIZE);   
	if (file_size < 0 || file_size > FILE_MAX_SIZE) {
		printf ("Xmodem error file size 0x%x \n", file_size);
		free(wav_data);
		return;
	}
	printf ("\nFile loaded\n");

	//printf("Address given to wav_data %x\n",wav_data);
	//wav_data = (char *) address;
#endif

/*
	// File dump start:
	fullrows = file_size / columns;
	lastrow = file_size % columns;
	printf("file size %d\n", file_size);
	printf("full rows %d\n", fullrows);
	printf("last rows %d\n", lastrow);
	idx = 0;
	for (i = 0 ; i < fullrows ; i++) {
		for (j = 0 ; j < columns ; j++) {
			printf("0x%02x,",wav_data[idx++]);
		}
		printf("\n");
	}
	for (i = 0 ; i < lastrow ; i++) {
		printf("0x%02x%c",wav_data[idx++],(i==(lastrow-1))?'\n':',' );
	}
	// File dump end
*/



	if ( (((int)wav_data) & 0x03) != 0 ) {
		printf("Load address is not aligned to 32 bits\n");
		while(1);
	}

	riff_header_ptr = (struct riff_header *)wav_data;
	if (riff_header_ptr->ChunkID != 0x46464952 || 
		riff_header_ptr->Format != 0x45564157  ||
		riff_header_ptr->ChunkSize < 20 ) {

		printf("There is something wrong with the RIFF format, not a WAVE or too small\n");
		printf("ID %0x\n",riff_header_ptr->ChunkID);
		printf("Size %d\n",riff_header_ptr->ChunkSize);
		printf("Format %0x\n",riff_header_ptr->Format);
		return ;
	}
	fmt_ptr = (struct fmt *)&wav_data[12];
	printf("Wav info:\n");
	printf("Audio Format: %x\n",fmt_ptr->AudioFormat);
	printf("Number of channels: %d\n",fmt_ptr->NumChannels);
	printf("Sample Rate: %d Khz\n",fmt_ptr->SampleRate);
	//8000, 44100, etc.
	printf("Sample Rate: %d bps\n",fmt_ptr->ByteRate);
	printf("Block Align: %d\n",fmt_ptr->BlockAlign);
	printf("Bits per sample: %d bits\n",fmt_ptr->BitsPerSample);

	// 20 is the offset of AudioFormat
	data_ptr = (struct data *)&wav_data[20 + fmt_ptr->Subchunk1Size];
	// Comparing agains "data"
	if (data_ptr->Subchunk2ID != 0x61746164) {
		printf("Invalid data chuck, it must start with 'data' %x\n", data_ptr->Subchunk2ID);
		return ;
	}
	printf("Data info:\n");
	//printf("Audio Format: %x\n",data_ptr->Subchunk2ID);
	printf("Size: %d bytes\n",data_ptr->Subchunk2Size);
	//num_samples = data_ptr->Subchunk2Size / (fmt_ptr->NumChannels * ( fmt_ptr->BitsPerSample/8) );
	num_samples = data_ptr->Subchunk2Size;// / (fmt_ptr->NumChannels * ( fmt_ptr->BitsPerSample/8) );

	// The following division is not required since
	// the algorithm works on the total number of samples
	// regardless of if they are left or right
	//if (fmt_ptr->NumChannels == 2) {
	//	num_samples >>= 1;
	//}
	switch ( fmt_ptr->BitsPerSample) {
		case 8:
			break;
		case 16:
			num_samples >>= 1;
			break;
		case 32:
			num_samples >>= 2;
			break;
		default:
			printf("Invalid bits per sample %x\n", fmt_ptr->BitsPerSample);
			return ;
			break;
	}
	printf("Number of samples: %d\n",num_samples);
	wav_start = (void *)&wav_data[0];
	offset = 20 + fmt_ptr->Subchunk1Size + 8;
	//sample_start = (void *)&wav_data[20 + fmt_ptr->Subchunk1Size + 8];
	sample_start = (void *)&wav_data[offset];
	printf("Sample start: %x\n",(unsigned int)sample_start);
	printf("Wav start: %x\n",(unsigned int)wav_start);
	printf("Offset calculation: %d\n",((unsigned int)sample_start - (unsigned int)wav_start));
	printf("Offset: %d\n",offset);
	//audio_seq[AMBER_AUDIO_SEQ_START_ADDRESS_OFF] = (unsigned int)sample_start;
	//audio_seq[AMBER_AUDIO_SEQ_SAMPLE_COUNT_OFF] = num_samples;
	//audio_seq[AMBER_AUDIO_SEQ_SAMPLE_COUNT_OFF] = data_ptr->Subchunk2Size;
		
	// Setting up the clock for pwm:

#ifndef AVOID_HW_WRITES
       // stop clock and waiting for busy flag doesn't work, so kill clock
        *(clk + PWMCLK_CNTL) = 0x5A000000 | (1 << 5);
        //usleep(10);
        RPI_WaitMicroSeconds( 100 );
#endif

        uint32_t pwm_clk_freq = 500000000;
        uint32_t bits_per_sample = 10;
        uint32_t sampling_rate = 44100;
        //int idiv = (pwm_clk_freq/0x3FF)/sampling_rate;
        //int idiv = (pwm_clk_freq/(1<<10))/sampling_rate;
        int idiv = (pwm_clk_freq>>bits_per_sample)/sampling_rate;
        printf ("PWM Clk freq %d Hz\n",pwm_clk_freq);
        printf ("Bits per sample %d\n",bits_per_sample);
        printf ("Sampling rate %d Hz\n",sampling_rate);
        printf ("Int div %d\n",idiv);

        if (idiv < 1 || idiv > 0x1000) {
                printf("idiv out of range: %x\n", idiv);
		while(1);
        }
#ifndef AVOID_HW_WRITES
        *(clk + PWMCLK_DIV)  = 0x5A000000 | (idiv<<12);

        // source=osc and enable clock
        //*(clk + PWMCLK_CNTL) = 0x5A000011; // 19.2 Mhz
        //*(clk + PWMCLK_CNTL) = 0x5A000015; // 1000Mhz
        //*(clk + PWMCLK_CNTL) = 0x5A000014; // none
        *(clk + PWMCLK_CNTL) = 0x5A000016; // 500Mhz


        // disable PWM
        *(pwm + PWM_CTL) = 0;

        RPI_WaitMicroSeconds( 100 );

    //pwm[PWM_RNG1] = 0x0000FFFF;
    //pwm[PWM_RNG1] = 3200;
    //pwm[PWM_DAT1] = 1600;
	//range = 8000;
#endif
	uint32_t range = 1<<bits_per_sample;
#ifndef AVOID_HW_WRITES
    pwm[PWM_RNG1] = range;
    pwm[PWM_RNG2] = range;
#endif

    printf ("Range %d \n",range);

	samples = (uint16_t *) sample_start;

}

void print_help ( void ) 

{
    printf("Commands:\n");
    printf("l");                     
    print_spaces(29);
    printf(": Load file wav\n");

    printf("p");                     
    print_spaces(29);
    printf(": play/pause wav\n");

    printf("s");                     
    print_spaces(29);
    printf(": stop wav\n");

    printf("x");                     
    print_spaces(29);
    printf(": exit (boot loader)\n");

    printf("h");                     
    print_spaces(29);
    printf(": help\n");

    /*
    printf("b <address>");                   
    print_spaces(19);
    printf(": Load binary file to <address>\n");

    printf("d <start address> <num bytes> : Dump mem\n");

    printf("h");                     
    print_spaces(29);
    printf(": Print help message\n");

    printf("j <address>");                     
    print_spaces(19);
    printf(": Execute loaded elf, jumping to <address>\n");

    printf("p <address>");                   
    print_spaces(19);
    printf(": Print ascii mem until first 0\n");
    
    printf("r <address>");                   
    print_spaces(19);
    printf(": Read mem\n");

    printf("s");                              
    print_spaces(29);
    printf(": Core status\n");
    
    printf("w <address> <value>");            
    print_spaces(11);
    printf(": Write mem\n");    
    */
}


/* Print a number of spaces */
void print_spaces ( int num ) 
{
    while(num--) printf(" ");
}
