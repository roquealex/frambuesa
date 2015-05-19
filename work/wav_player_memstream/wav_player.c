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

#include "fir.h"

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


// This structure will save the current wav information required for playback
struct {
	// Number of samples per channel
	unsigned int num_samples;

	// Sample frequency for PWM 32k, 44.1k and 48k only
	// the original wav sample freq is fs / 2^inter
	uint32_t fs;

	// Interpolation ration.
	uint32_t inter;

	// Pointer to the file in memory
	char *wav_data;

	// Direct pointer to the samples in memory,
	// only 16 bit samples supported
	int16_t *samples;

	// 1 if 2 channel , else 0
	uint32_t is_stereo;
	// Equalizer is enabled
	uint32_t is_valid;
	// Equalizer is enabled (if supported by fs)
	uint32_t eq_en;
	// Equalizer is enabled by user
	uint32_t user_eq_en;
} audio_info;

//#define SAMPLE_BUFF_SIZE (1<<9) // FAIL
// Works:
//#define SAMPLE_BUFF_SIZE (1<<10)
// 64K
#define SAMPLE_BUFF_SIZE (1<<14)
// 256K
//#define SAMPLE_BUFF_SIZE (1<<16)
//2^20/2^2:
// 1 MB:
//#define SAMPLE_BUFF_SIZE (1<<18)
// 2 MB:
//#define SAMPLE_BUFF_SIZE (1<<19)
//#define SAMPLE_BUFF_SIZE (1<<6)
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


	audio_info.is_valid = 0;
	audio_info.user_eq_en = 1;

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
				load_audio();
				break;
			case 'e':
				eq_enable();
				break;

		}
		
	} while (c != 'x');
	printf ("END\n");
    /* Flush out UART FIFO */
    printf ("                ");
    //_testpass();
}


void play_audio ( void ){

	// Memory to hold the control blocks for DMA
	void *mem;
	dma_cb_t *cb_ptr;
	uint32_t align;

	printf("Playing\n");
	if (!audio_info.is_valid) {
		printf("No audio is loaded in memory, load some audio first\n");
		return;
	}
	printf("Number of samples: %d\n",audio_info.num_samples);
	printf("Channels: %d\n",audio_info.is_stereo?2:1);
	if (audio_info.num_samples > 0 ) {
		int i = 0;
		int j;
		int16_t x[2][FIR_ORDER];
		int xidx[2] = {0,0};
		unsigned int samples_per_buffer;
		unsigned int left_samples = audio_info.num_samples;
		int buffer_num = 0;
		uint32_t min_delta_time = 0xffffffff;

		// Initializing the samples:
		for (i = 0; i < 2 ; i++) {
			for (j = 0; j < FIR_ORDER ; j++) {
				x[i][j] = 0;
			}
		}

		// Allocating space for 2 control blocks
		align = sizeof(dma_cb_t);
		printf("Alignment to be used %d\n",align);
		mem = malloc(2*sizeof(dma_cb_t) + (align-1));
		printf("Addr of mem = %x\n", (uintptr_t)mem);
		cb_ptr = (dma_cb_t*)( ((uintptr_t)mem+(align-1)) & ~(uintptr_t)(align-1) );
		printf("Addr of cb_ptr = %x\n", (uintptr_t)cb_ptr);

		//pwm[PWM_CTL] = 0x00002161; // dual channel fifo m/s
		// This is for stereo:
		pwm[PWM_CTL] = PWM_CTL_USEF2 | PWM_CTL_PWEN2 |
			PWM_CTL_CLRF1 | PWM_CTL_USEF1 | PWM_CTL_PWEN1 ;
		printf("PWM_CTL = %08x\n", pwm[PWM_CTL]);
		//pwm[PWM_DMAC] = PWMDMAC_ENAB | PWMDMAC_THRSHLD;
		pwm[PWM_DMAC] = 0x80000707;

		dma[0][DMA_CS] = DMA_CS_RESET;
		dma[1][DMA_CS] = DMA_CS_RESET;
#ifndef AVOID_HW_WRITES
		RPI_WaitMicroSeconds(1000);
		printf("Reset done\n");
#endif
		// To start with the number of samples that a buffer can fit is
		// half of the size since the buffer interleaves a left and a
		// right sample.
		samples_per_buffer = (SAMPLE_BUFF_SIZE/2);
		samples_per_buffer /= audio_info.inter;

		// This is temporary until we get the buffers for FIR
			int16_t last_sample[2] = {0,0};
		while (left_samples>0) {
			int size_of_loop = (samples_per_buffer > left_samples) ? left_samples : samples_per_buffer ;
			int initial_idx = audio_info.num_samples-left_samples ;
			for (i = 0 ; i < size_of_loop ; i ++) {
				// Maximun interpolation supported is 4
				//uint32_t interarr[4];
				//int idx = (audio_info.is_stereo)?i*2:i;
				int idx = (initial_idx+i);
				if(audio_info.is_stereo) idx *= 2;
				//uint16_t sample = samples[initial_idx+idx] ^ 0x8000;
				// Array of left and right current samples
				int16_t sample[2];
				sample[0] = audio_info.samples[idx];
				sample[1] = (audio_info.is_stereo)?audio_info.samples[idx+1]:sample[0];
				//sample[0] = ((i&3)==0)?0x7fff:((i&3)==2)?0x8000:0;
				//sample[1] = (i&1)?0x7fff:0x8000;
				//sample[0] = 0x7fff;
				//sample[1] = 0x8000;
				//sample[0] = (0x1000 * (i&0xf));
				//sample[1] = 0x8000;
				// Do the interpolation here:
				//for (j = 0 ; j < audio_info.inter ; j++) {
				//	int32_t curr_sample;
				//}

				// Doing one side at the time
				for (j = 0 ; j < 2 ; j++) {
					int k;
					int16_t mid_sample;
					// for 2 and 4 interpolation we need the meed sample
					if (audio_info.inter > 1) mid_sample = midpoint16(last_sample[j],sample[j]);
					for (k = 0 ; k <audio_info.inter  ; k++) {
						int l;
						int16_t curr_sample;
						int32_t curr_sample32;
						curr_sample = sample[j];
		int16_t *xptr;
		int xidxptr;
						
						if ( k == (audio_info.inter-1) ){
							curr_sample = sample[j];
						} else if (k == 0) {
							curr_sample = (audio_info.inter==2)?mid_sample
								:midpoint16(last_sample[j],mid_sample);
						} else if(k==1) {
							curr_sample = mid_sample;
						} else { //if (k==2) 
							curr_sample = midpoint16(mid_sample,sample[j]);
						}
						/*
						*/


						if ( audio_info.user_eq_en && audio_info.eq_en ) {
							// From this point we don't want to do complex 
							// memory access in the loops so getting a
							// direct pointer to x[] and a copy of ptr
							xptr = x[j];
							xidxptr = xidx[j];
							//x[j][xidx[j]] = curr_sample;
							xptr[xidxptr] = curr_sample;

							curr_sample32 = 0;
							//printf("value of idx at the beginning: %d\n", xidx[j]);
							for(l = 0 ; l < FIR_ORDER ; l++) {
								//curr_sample32 += ((int32_t)b_high_pass_44k1[l] *
								/*
								curr_sample32 += ((int32_t)b_low_pass_44k1[l] *
										x[j][xidx[j]]);
								xidx[j]++;
								xidx[j]&=(FIR_ORDER-1);
								*/
								// Reoimplementing with copies:
								curr_sample32 += ((int32_t)b_low_pass_44k1[l] *
										xptr[xidxptr++]);
								//xidxptr++;
								xidxptr&=(FIR_ORDER-1);
							}
							//printf("value of idx at the end: %d\n", xidx[j]);
							// In theory we should return the value of xidxptr
							// to xidx[j] but in practice at the end of the loop
							// xidxptr is the same as the original xidx[j] because
							// it is a circular index on 64 elements.
							xidx[j]--;
							xidx[j]&=(FIR_ORDER-1);

							curr_sample32 >>= 15;
						} else {
							// Sign extend the sample to 32 bits:
							curr_sample32 = (int32_t)curr_sample;
						}

						// Moving the sample to all positive range
						curr_sample32 += 0x8000;
						curr_sample32 >>= 6;
						//sample_buffer[buffer_num][(i*2)+j] = (uint32_t) curr_sample ;
						//sample_buffer[buffer_num][((i*2+k)*audio_info.inter)+j] = (uint32_t) curr_sample ;
						sample_buffer[buffer_num][((i*audio_info.inter+k)*2)+j] = curr_sample32 ;
					}
					last_sample[j] = sample[j];
				}
				/*
				last_sample[0] = sample[0];
				last_sample[1] = sample[1];
				// Removing the symbol by adding offset
				sample[0] ^= 0x8000;
				sample[1] ^= 0x8000;
				// Reducing the bit size to match the range of the PWM
				sample[0]>>=6;
				sample[1]>>=6;
				sample_buffer[buffer_num][i*2] = (uint32_t) sample[0];
				sample_buffer[buffer_num][(i*2)+1] = (uint32_t) sample[1];
				*/
				//i++;
				//sample_buffer[buffer_num][i] = (uint32_t) sample;
			}
			left_samples -= size_of_loop;
			// Now play the buffer
			//cb_ptr[0].ti = DMA_S_INC | DMA_D_INC ;
			cb_ptr[buffer_num].ti = DMA_NO_WIDE_BURSTS | DMA_WAIT_RESP | DMA_S_INC | DMA_D_DREQ | (5<<16);
			//DMA_NO_WIDE_BURSTS | DMA_WAIT_RESP | DMA_D_DREQ | DMA_PER_MAP(5);
			//cb_ptr[0].ti = DMA_WAIT_RESP | DMA_S_INC | (5<<16);
			cb_ptr[buffer_num].source_ad = (uint32_t) &sample_buffer[buffer_num][0] ;
			cb_ptr[buffer_num].dest_ad = (((uint32_t) &pwm[PWM_FIF1])&0x00FFFFFF) | 0x7E000000;
			// This is for the old mono/stereo scheme
			//cb_ptr[buffer_num].txfr_len = size_of_loop*4*2; // size of the transffer is in bytes so x4
			// Size of loops is in number of samples, mutiplying it for two for left and right
			// and 4 because on the DMA buffer uses 32 bit samples (This is what PWM receives)
			cb_ptr[buffer_num].txfr_len = size_of_loop*4*2*audio_info.inter; // size of the transffer is in bytes so x4
			cb_ptr[buffer_num].stride = 0;
			cb_ptr[buffer_num].nextconbk = 0;
			printf("Copying %d bytes from %x to %x\n",cb_ptr[buffer_num].txfr_len,cb_ptr[buffer_num].source_ad,cb_ptr[buffer_num].dest_ad);
			/*
			// This code will be substituted by dma:
			dma[buffer_num][DMA_CS] = DMA_CS_RESET;
#ifndef AVOID_HW_WRITES
			RPI_WaitMicroSeconds(1000);
			printf("Reset done\n");
#endif
			*/

			dma[buffer_num][DMA_DEBUG] = DMA_DEBUG_READ_ERROR | DMA_DEBUG_FIFO_ERROR | DMA_DEBUG_READ_LAST_NOT_SET_ERROR; 
			//dma[buffer_num][DMA_CONBLK_AD] = (uint32_t)cb_ptr | 0xC0000000;
			dma[buffer_num][DMA_CONBLK_AD] = (uint32_t)&cb_ptr[buffer_num] ;
			// Starting the DMA engine:
			//>printf("DMA_CS = %08x\n", dma[buffer_num][DMA_CS]);
			// If by the time we reached this point the other DMA engine
			// already finished our processing is too slow.
#ifndef AVOID_HW_WRITES
			if (dma[buffer_num^1][DMA_CS] & DMA_CS_END){
				printf("FATAL_ERROR: The processing of data was too slow\n");
				printf("DMA_CS = %08x\n", dma[buffer_num][DMA_CS]);
				pwm[PWM_CTL] = 0;
				while(1);
			}
#endif
			// Don't do this wait on the first loop
			// otherwise wait for the other DMA to finish
			if (initial_idx != 0) {
				uint32_t ts0, delta_time;
				ts0 = RPI_GetTimeStamp();
				// Wait for the other DMA channel to finish to activate this one
				while(!(dma[buffer_num^1][DMA_CS] & DMA_CS_END));
				// Technically we need to write only the END bit so we
				// would need to apply an or mask but in reality we want
				// to write everything in zero for this particular case
				dma[buffer_num^1][DMA_CS] = DMA_CS_END;
				//delta_time = RPI_GetTimeStamp();
				//printf("time %u - %u\n",delta_time,ts0);
				//if (ts0 > delta_time) printf("ROLLOVER\n");
				//delta_time -= ts0;
				delta_time = RPI_GetTimeStamp() - ts0;
				//printf("Delta time %d\n",delta_time);
				if (min_delta_time > delta_time) min_delta_time = delta_time;
			}
			// Play the next buffer
			dma[buffer_num][DMA_CS] = DMA_CS_ACTIVE;
			//>printf("DMA_CS = %08x\n", dma[buffer_num][DMA_CS]);

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
					if((i%2)==0) printf("%s|", graph);
					else printf("%s|\n", graph);
					
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
			if (left_samples>0) buffer_num ^= 1;

		}// while (left_samples>0) 

		// Wait for the last DMA transffer to finish before stopping the PWM
		while(!(dma[buffer_num][DMA_CS] & DMA_CS_END));
		// Technically we need to write only the END bit so we
		// would need to apply an or mask but in reality we want
		// to write everything in zero for this particular case
		dma[buffer_num^1][DMA_CS] = DMA_CS_END;

		// Cool down the PWM to avoid that clicking sound:
		// the algorithm uses last_sample but thats not a
		// given since when filtering is used the last value
		// in the pwm might be different from these
		//printf("Last samples %d %d\n", last_sample[1], last_sample[0]);
		for (i = 0; i < 2 ; i++) {
			last_sample[i] >>= 6;
			last_sample[i] += 0x0200;
		}
		while (last_sample[1] != 0 || last_sample[0] != 0) {
			for (i = 0; i < 10 ; i++) {
				for (j = 0; j < 2 ; j++) {
					while((pwm[PWM_STA] & 0x01) == 1);
					pwm[PWM_FIF1] = last_sample[j];
					//printf("Last sample %d %d\n", j, last_sample[j]);
				}
				//printf("Last samples %d %d\n", last_sample[1], last_sample[0]);
			}
			for (i = 0; i < 2 ; i++)
				if (last_sample[i] != 0) last_sample[i]--;
		}
		for (i = 0; i < 2 ; i++) {
			while((pwm[PWM_STA] & 0x01) == 1);
			pwm[PWM_FIF1] = 0;
		}

		// Stop PWM:
		printf("At the end Left samples %d\n", left_samples);
		pwm[PWM_CTL] = 0;
		free(mem);
		printf("Done Playing\n");
		printf("Minimun slack after processing data and next DMA start %u us\n", min_delta_time);
	} // if (num_samples > 0 ) 
}
void stop_audio ( void ){
	printf("Stopping\n");
}

// Toggles the user eq en flag
void eq_enable ( void ){
	if (audio_info.user_eq_en) {
		printf("Disabling equalizer\n");
		audio_info.user_eq_en = 0;
	} else {
		printf("Enabling equalizer\n");
		audio_info.user_eq_en = 1;
	}
}

void load_audio (){
	struct riff_header *riff_header_ptr;
	struct fmt *fmt_ptr;
	struct data *data_ptr;
	unsigned int num_samples;
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

	if (audio_info.is_valid) {
		printf ("Previously loaded audio, releasing memory at address %x first...\n",(int)audio_info.wav_data);
		free(audio_info.wav_data);
	}

	audio_info.is_valid = 0;

	wav_data = 0;
	//file_size = xmodemReceive((char *) address, FILE_MAX_SIZE);   
	file_size = xmodemReceive(&wav_data, FILE_MAX_SIZE);   
	audio_info.wav_data = wav_data;
	if (file_size < 0 || file_size > FILE_MAX_SIZE) {
		printf ("Xmodem error file size 0x%x \n", file_size);
		free(wav_data);
		return;
	}
	printf ("\nFile loaded\n");

	//printf("Address given to wav_data %x\n",wav_data);
	//wav_data = (char *) address;


	if ( (((int)wav_data) & 0x03) != 0 ) {
		printf("FATAL_ERROR: Load address is not aligned to 32 bits\n");
		free(wav_data);
		while(1);
	}

	riff_header_ptr = (struct riff_header *)wav_data;
	if (riff_header_ptr->ChunkID != 0x46464952 || 
		riff_header_ptr->Format != 0x45564157  ||
		riff_header_ptr->ChunkSize < 20 ) {

		printf("ERROR: There is something wrong with the RIFF format, not a WAVE or too small\n");
		printf("ID %0x\n",riff_header_ptr->ChunkID);
		printf("Size %d\n",riff_header_ptr->ChunkSize);
		printf("Format %0x\n",riff_header_ptr->Format);
		free(wav_data);
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
		printf("ERROR: Invalid data chuck, it must start with 'data' %x\n", data_ptr->Subchunk2ID);
		free(wav_data);
		return ;
	}
	printf("Data info:\n");
	//printf("Audio Format: %x\n",data_ptr->Subchunk2ID);
	printf("Size: %d bytes\n",data_ptr->Subchunk2Size);
	//num_samples = data_ptr->Subchunk2Size / (fmt_ptr->NumChannels * ( fmt_ptr->BitsPerSample/8) );
	num_samples = data_ptr->Subchunk2Size;// / (fmt_ptr->NumChannels * ( fmt_ptr->BitsPerSample/8) );

	// To unify the case of stereo and mono plus different interpolations it is better
	// to use the sample size as the size of left or right channel
	if (fmt_ptr->NumChannels == 2) {
		if(num_samples&0x01) {
			printf("ERROR: Wrong wav format, size of subchunk is %d and 2 "
				"channels but size is not multiple of 2\n", data_ptr->Subchunk2Size);
			free(wav_data);
			while(1);
		}
		num_samples >>= 1;
		audio_info.is_stereo = 1;
	} else if (fmt_ptr->NumChannels == 1) {
		audio_info.is_stereo = 0;
	} else {
		printf("ERROR: Unsupported number of channels %d, only stereo "
			"and mono is supported\n", fmt_ptr->NumChannels);
		free(wav_data);
		while(1);
	}
	// Only 16 bit wavs are supported for now
	switch ( fmt_ptr->BitsPerSample) {
		//case 8:
		//	break;
		case 16:
			if(num_samples&0x01) {
				printf("ERROR: Wrong wav format, size of subchunk is %d and 16 "
					"bits per sample, but size not multiple or 2 or 4\n",
					data_ptr->Subchunk2Size);
				free(wav_data);
				while(1);
			}
			num_samples >>= 1;
			break;
		//case 32:
		//	num_samples >>= 2;
		//	break;
		default:
			printf("ERROR: Invalid bits per sample %x\n", fmt_ptr->BitsPerSample);
			free(wav_data);
			return ;
			break;
	}
	audio_info.num_samples = num_samples;
	printf("Number of samples: %d\n",audio_info.num_samples);
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
        //uint32_t bits_per_sample = 14;
	
	// No interpolation:
        uint32_t sampling_rate = fmt_ptr->SampleRate;
	if (sampling_rate > 48000 || sampling_rate < 8000) {
		printf("ERROR: Sampling rate is unsupported %d\n", sampling_rate);
		free(wav_data);
		while(1);
	}
	// Fastest interpolation
	//sampling_rate = 12000;
	//sampling_rate = 48000;
	switch (sampling_rate) {
		case 44100:
		case 48000:
		case 32000:
			// These are the primary sampling rates supported:
			audio_info.fs = sampling_rate;
			audio_info.inter = 1;
			audio_info.eq_en = 1;
			break;
		case 22050:
		case 24000:
		case 16000:
			audio_info.fs = sampling_rate*2;
			audio_info.inter = 2;
			audio_info.eq_en = 1;
			break;
		case 11025:
		case 12000:
		case  8000:
			audio_info.fs = sampling_rate*4;
			audio_info.inter = 4;
			audio_info.eq_en = 1;
			break;
		default:
			printf("WARNING: Sampling rate %d is unsupported for EQ, no interpolation\n", sampling_rate);
			audio_info.fs = sampling_rate;
			audio_info.inter = 1;
			audio_info.eq_en = 0;
			break;
	}
	
        //uint32_t sampling_rate = 48000;
        //int idiv = (pwm_clk_freq/0x3FF)/sampling_rate;
        //int idiv = (pwm_clk_freq/(1<<10))/sampling_rate;
        int idiv = (pwm_clk_freq>>bits_per_sample)/audio_info.fs;
        printf ("PWM Clk freq %d Hz\n",pwm_clk_freq);
        printf ("Bits per sample %d\n",bits_per_sample);
        printf ("Sampling rate %d Hz\n",sampling_rate);
        printf ("Fs rate %d Hz\n",audio_info.fs);
        printf ("Interpolation %d\n",audio_info.inter);
        printf ("Int div %d\n",idiv);

        if (idiv < 1 || idiv > 0x1000) {
                printf("FATAL_ERROR: idiv out of range: %x\n", idiv);
		free(wav_data);
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

	audio_info.samples = (uint16_t *) sample_start;
	audio_info.is_valid = 1;

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

    printf("e");                     
    print_spaces(29);
    printf(": Enable/Disable Equalizer (Currently %s)\n",audio_info.user_eq_en?"Enabled":"Disabled");


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
int16_t midpoint16(int16_t a,int16_t b) {
	return ((int32_t)a+(int32_t)b)>>1;
}

/* Print a number of spaces */
void print_spaces ( int num ) 
{
    while(num--) printf(" ");
}
