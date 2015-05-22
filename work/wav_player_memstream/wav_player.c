//                                                              
//  wav-player.c                                                
//                                                              
//  This file is part of wav player memstream                   
//                                                              
//  Description                                                 
//  Bare metal raspberrypi wav player using PWM audio output    
//                                                              
//  Author(s):                                                  
//      - Roque Arcudia, roquealex@gmail.com           
//      - Conor Santifort, csantifort.amber@gmail.com (file transfer based on his bootloader)
//                                                              
//                                                              
// Copyright (C) 2010 Authors and OPENCORES.ORG                 
//                                                              
// This source file may be used and distributed without         
// restriction provided that this copyright statement is not    
// removed from the file and that any derivative work contains  
// the original copyright notice and the associated disclaimer. 
//                                                              
// This source file is free software; you can redistribute it   
// and/or modify it under the terms of the GNU Lesser General   
// Public License as published by the Free Software Foundation; 
// either version 2.1 of the License, or (at your option) any   
// later version.                                               
//                                                              
// This source is distributed in the hope that it will be       
// useful, but WITHOUT ANY WARRANTY; without even the implied   
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      
// PURPOSE.  See the GNU Lesser General Public License for more 
// details.                                                     
//                                                              
// You should have received a copy of the GNU Lesser General    
// Public License along with this source; if not, download it   
// from http://www.opencores.org/lgpl.shtml                     
//                                                              

#include <stdio.h>
#include <stdlib.h>
#include "wav_player.h"
#include "rpi-pwm.h"

#include "rpi-dma.h"

#include "rpi-gpio.h"
#include "rpi-systimer.h"

#include "fir.h"

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

audio_info_t audio_info;

#define EQ_SETTINGS_NUM 3
const char *eq_setting_str[EQ_SETTINGS_NUM] = {"Low pass","High pass","Disabled"};
const int16_t *b_32k[EQ_SETTINGS_NUM-1] = {b_low_32k,b_high_32k};
const int16_t *b_44k1[EQ_SETTINGS_NUM-1] = {b_low_44k1,b_high_44k1};
const int16_t *b_48k[EQ_SETTINGS_NUM-1] = {b_low_48k,b_high_48k};

//#define SAMPLE_BUFF_SIZE (1<<9) // FAIL
// Works:
//#define SAMPLE_BUFF_SIZE (1<<10)
// 64K
#define SAMPLE_BUFF_SIZE (1<<14)
// 256K
//#define SAMPLE_BUFF_SIZE (1<<16)
// 1 MB:
//#define SAMPLE_BUFF_SIZE (1<<18)
// 2 MB:
//#define SAMPLE_BUFF_SIZE (1<<19)
// Good for visual testing:
//#define SAMPLE_BUFF_SIZE (1<<6)
uint32_t sample_buffer[2][SAMPLE_BUFF_SIZE];


main () 
{
	char c;

	audio_info.is_valid = 0;
	// Disabled by default (last position in the array)
	audio_info.eq_setting = (EQ_SETTINGS_NUM-1);

	printf ("#     #     #     #     #\n");
	printf ("#  #  #    # #    #     #\n");
	printf ("#  #  #   #   #   #     #\n");
	printf ("#  #  #  #     #  #     #\n");
	printf ("#  #  #  #######   #   #\n");
	printf ("#  #  #  #     #    # #\n");
	printf (" ## ##   #     #     #\n");
	printf ("\nWAV Player (mem stream v1.3)\n\n");

	print_help();
	printf("Ready>\n");

	do {
		//printf("\n? ");
		//c = getchar();
		while ((c = _inbyte (DLY_1S)) == 0xFF) ;
		//printf("c = %0x\n",c);
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
				eq_setting();
				break;

		}
		
	} while (1); // disabling going back to bootloader (c != 'x');
}


void play_audio ( void ){


	printf("Playing\n");
	if (!audio_info.is_valid) {
		printf("No audio is loaded in memory, load some audio first\n");
		return;
	}
	printf("Number of samples: %d\n",audio_info.num_samples);
	printf("Channels: %d\n",audio_info.is_stereo?2:1);
	if (audio_info.num_samples > 0 ) {
		// Loop variables:
		int i; // sample index (temporary sample buffer)
		int j; // left/right index
		unsigned int samples_per_buffer; // upper bound of i
		unsigned int left_samples; // samples in the file per channel
		int buffer_num = 0; // selects the temporary sample buffer
		// DMA Variables:	
		void *mem; // Memory to hold the control blocks for DMA
		volatile dma_cb_t *cb_ptr; // Pointers to control block (indexed as array)
		const uint32_t align = sizeof(dma_cb_t);
		// FIR variables:
		int user_eq_en;
		int16_t x[2][FIR_ORDER]; // prev samples L/R
		int xidx[2] = {0,0}; // current index L/R
		const int16_t *b_fir; // current coefs
		// Interpolation
		int16_t last_sample[2] = {0,0}; // stores the last sample redundant with x[j][t-1]
		// Profiling:
		uint32_t min_delta_time = 0xffffffff;

		// Select the current equalization coef
		if (audio_info.eq_setting == (EQ_SETTINGS_NUM-1)) {
			// Equaliztion is disabled if "disabled" setting is
			// chosen which is the last entry in the array of
			// EQ_SETTINGS_NUM size
			user_eq_en = 0;
		} else {
			// Select the right array based on current sample
			// frequency.
			user_eq_en = 1;
			switch(audio_info.fs) {
				case 32000:
					b_fir = b_32k[audio_info.eq_setting];
					break;
				case 44100:
					b_fir = b_44k1[audio_info.eq_setting];
					break;
				case 48000:
					b_fir = b_48k[audio_info.eq_setting];
					break;
				default:
					user_eq_en = 0;
					break;
			}
			if (user_eq_en) {
				printf("Playing audio with Equalizer setting : %s\n",eq_setting_str[audio_info.eq_setting]);
				//for (i = 0 ; i < FIR_ORDER ; i++) printf("%d\n",b_fir[i]);
			}
		}

		// Initializing the previous samples:
		for (i = 0; i < 2 ; i++) {
			for (j = 0; j < FIR_ORDER ; j++) {
				x[i][j] = 0;
			}
		}

		// Allocating space for 2 control blocks:
		// According to the doc the address address must
		// be 256 bit aligned, so the bottom 5 bits of
		// the address must be zero.
		mem = malloc(2*sizeof(dma_cb_t) + (align-1));
		//printf("Addr of mem = %x\n", (uintptr_t)mem);
		cb_ptr = (dma_cb_t*)( ((uintptr_t)mem+(align-1)) & ~(uintptr_t)(align-1) );
		//printf("Addr of cb_ptr = %x\n", (uintptr_t)cb_ptr);

		// Settings of the PWM
		//pwm[PWM_CTL] = 0x00002161; // Raw expected config
		pwm[PWM_CTL] = PWM_CTL_USEF2 | PWM_CTL_PWEN2 |
			PWM_CTL_CLRF1 | PWM_CTL_USEF1 | PWM_CTL_PWEN1 ;
		//printf("PWM_CTL = %08x\n", pwm[PWM_CTL]);
		//pwm[PWM_DMAC] = 0x80000707; // Raw expected config (using defaults)
		pwm[PWM_DMAC] |= PWM_DMAC_ENAB;
		//printf("PWM_DMAC = %08x\n", pwm[PWM_DMAC]);

		dma[0][DMA_CS] = DMA_CS_RESET;
		dma[1][DMA_CS] = DMA_CS_RESET;
#ifndef AVOID_HW_WRITES
		RPI_WaitMicroSeconds(1000);
		printf("Reset DMA done\n");
#endif
		// To start with the number of samples that a buffer can fit is
		// half of the size since the buffer interleaves a left and a
		// right sample.
		samples_per_buffer = (SAMPLE_BUFF_SIZE/2);
		samples_per_buffer /= audio_info.inter;
		left_samples = audio_info.num_samples;

		while (left_samples>0) {
			int size_of_loop = (samples_per_buffer > left_samples) ? left_samples : samples_per_buffer ;
			int initial_idx = audio_info.num_samples-left_samples ;
			for (i = 0 ; i < size_of_loop ; i ++) {
				// Maximun interpolation supported is 4
				int idx = (initial_idx+i);
				// Array of left and right current samples
				int16_t sample[2];
				if(audio_info.is_stereo) idx *= 2;
				// If it is mono repeat same sample on left and right
				// otherwise get both from the file
				sample[0] = audio_info.samples[idx];
				sample[1] = (audio_info.is_stereo)?audio_info.samples[idx+1]:sample[0];
				// Override with triangular waves good for visual testing:
				// sample[0] = ((i&3)==0)?0x7fff:((i&3)==2)?0x8000:0;
				// sample[1] = (i&1)?0x7fff:0x8000;

				// j select left or right sample
				for (j = 0 ; j < 2 ; j++) {
					int k;
					int16_t mid_sample;
					// for 2 and 4 interpolation we need the meed sample
					//                                           
					//         *sample[t]                        
					//        /   ^                              
					//       x   mid                             
					//      /     v                              
					//     x mid_sample=(sample[t-1]+sample[t])/2
					//    /       ^                              
					//   x       mid                             
					//  /         v                              
					// *sample[t-1]                              
					//                                           
					if (audio_info.inter > 1) mid_sample = midpoint16(last_sample[j],sample[j]);
					for (k = 0 ; k <audio_info.inter  ; k++) {
						int l;
						int16_t curr_sample;
						int32_t curr_sample32;
						// For faster speed get a pointer to data and a copy to index
						int16_t *xptr;
						int xidxptr;

						curr_sample = sample[j];
						
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

						if ( user_eq_en && audio_info.eq_en ) {
							// From this point we don't want to do complex 
							// memory access in the loops so getting a
							// direct pointer to x[] and a copy of ptr
							xptr = x[j];
							xidxptr = xidx[j];
							xptr[xidxptr] = curr_sample;
							curr_sample32 = 0;
							// The x buffer acts as a circular buffer for
							// each loop xidxptr will iterate on all
							// elements starting at the position of xidx[j]
							// it will rollover at the top and continue at
							// the bottom. At the end of the loop xidx[j]
							// will be decremented so the next loop will
							// use this sample as the previous
                                                        //     Sample N               Sample N + 1
							//               x[j]                    x[j] 
							//  Rollover ^  +----+               ^  +----+
							//           | 7|    |               | 7|    |
							//           | 6|    |               | 6|    |
							//           | 5|    |               | 5|    |
							// xidxptr++ | 4|    |     xidxptr++ | 4|    |
							//           | 3|    |               | 3|    |
							//           | 2|    |               | 2|    |
							// xidx[j]=> * 1|    |      v        | 1|    |
							//           ^ 0|    |  xidx[j]-- => * 0|    |
							//           |  +----+               ^  +----+
							for(l = 0 ; l < FIR_ORDER ; l++) {
								curr_sample32 += ((int32_t)b_fir[l] *
										xptr[xidxptr++]);
								xidxptr&=(FIR_ORDER-1);
							}
							xidx[j]--;
							xidx[j]&=(FIR_ORDER-1);

							// TODO : when using these triangulars some
							// samples are lost with the high pass, this
							// might be due to overflow in the range
							// check if this is the case, find out how
							// to saturate
							curr_sample32 >>= 15;
						} else {
							// Sign extend the sample to 32 bits:
							curr_sample32 = (int32_t)curr_sample;
						}

						// Moving the sample to all positive range
						// An offset and a scale is applied
						//
						//+N+    *             
						//  +   * *              Scale +M+    *          
						//  +                            +   * *         
						//  +  *   *                     +  *   *        
						//  +                            +               
						// 0+ *-----*-----*-  ==>  Offset+ *     *     * 
						//  +                         ^  +               
						//  +        *   *            |  +        *   *  
						//  +                         |  +         * *   
						//  +         * *             | 0+----------*----
						//-N+          *           
						//
						curr_sample32 += 0x8000;
						curr_sample32 >>= 6;
						sample_buffer[buffer_num][((i*audio_info.inter+k)*2)+j] = curr_sample32 ;
					}
					last_sample[j] = sample[j];
				}
			}
			left_samples -= size_of_loop;
			// TODO : Most of this config never changes and can
			// be setup once outside of this loop. This is risky
			// due to the bad BCM documentation so leaving it
			// for later
			//
			// Now play the buffer
			cb_ptr[buffer_num].ti = DMA_TI_NO_WIDE_BURSTS | DMA_TI_WAIT_RESP | DMA_TI_S_INC
				| DMA_TI_D_DREQ | (5<<16);
			cb_ptr[buffer_num].source_ad = (uint32_t) &sample_buffer[buffer_num][0] ;
			cb_ptr[buffer_num].dest_ad = (((uint32_t) &pwm[PWM_FIF1])&0x00FFFFFF) | 0x7E000000;
			// Size of loops is in number of samples, mutiplying it for two for left and right
			// and 4 because on the DMA buffer uses 32 bit samples (This is what PWM receives)
			cb_ptr[buffer_num].txfr_len = size_of_loop*4*2*audio_info.inter;
			cb_ptr[buffer_num].stride = 0;
			cb_ptr[buffer_num].nextconbk = 0;
			// Unecessary print but helps for debug
			printf("Copying %d bytes from %x to %x\n",cb_ptr[buffer_num].txfr_len,cb_ptr[buffer_num].source_ad,cb_ptr[buffer_num].dest_ad);

			dma[buffer_num][DMA_DEBUG] = DMA_DEBUG_READ_ERROR | DMA_DEBUG_FIFO_ERROR | DMA_DEBUG_READ_LAST_NOT_SET_ERROR; 
			// This line was here because the BCM doc suggested using
			// the mirror starting with 0xC but that didn't work for
			// all the addresses
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
				delta_time = RPI_GetTimeStamp() - ts0;
				//printf("Delta time %d\n",delta_time);
				if (min_delta_time > delta_time) min_delta_time = delta_time;
			}
			// Play the next buffer
			dma[buffer_num][DMA_CS] = DMA_CS_ACTIVE;

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
		// TODO : Move this inside the main loop
		// and get rid of the click at the start of
		// the playback
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
		pwm[PWM_CTL] = 0;
		free(mem);
		printf("Done Playing\n");
		printf("Minimun slack after processing data and next DMA start %u us\n", min_delta_time);
	} // if (num_samples > 0 ) 
}
void stop_audio ( void ){
	// Not implemented yet because the code
	// does not play the audio in the background
	printf("Stopping\n");
}

// Toggles the user eq en flag
void eq_setting ( void ){
	// round robin
	audio_info.eq_setting = (audio_info.eq_setting == (EQ_SETTINGS_NUM-1))?0:audio_info.eq_setting+1;
	printf("Changing equalizer setting to %s\n", eq_setting_str[audio_info.eq_setting]);
}

void load_audio (){
	struct riff_header *riff_header_ptr;
	struct fmt *fmt_ptr;
	struct data *data_ptr;
	unsigned int num_samples;
	void *sample_start;
	void *wav_start;
	unsigned int offset;
	uint32_t range;
	int i;

	FILE *stream;

	char *wav_data;
	int file_size;

	if (audio_info.is_valid) {
		printf ("Previously loaded audio, releasing memory at address %x first...\n",(int)audio_info.wav_data);
		free(audio_info.wav_data);
	}

	audio_info.is_valid = 0;

	wav_data = 0;
	file_size = xmodemReceive(&wav_data, FILE_MAX_SIZE);   
	audio_info.wav_data = wav_data;
	if (file_size < 0 || file_size > FILE_MAX_SIZE) {
		printf ("Xmodem error file size 0x%x \n", file_size);
		free(wav_data);
		return;
	}
	printf ("\nWAV File loaded!\n");

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
	printf("# Wav info:\n");
	printf("Audio Format       : %x\n",fmt_ptr->AudioFormat);
	printf("Number of channels : %d\n",fmt_ptr->NumChannels);
	printf("Sample Rate        : %d Khz\n",fmt_ptr->SampleRate);
	//8000, 44100, etc.
	printf("Byte Rate          : %d bps\n",fmt_ptr->ByteRate);
	printf("Block Align        : %d\n",fmt_ptr->BlockAlign);
	printf("Bits per sample    : %d bits\n",fmt_ptr->BitsPerSample);

	// 20 is the offset of AudioFormat
	data_ptr = (struct data *)&wav_data[20 + fmt_ptr->Subchunk1Size];
	// Comparing agains "data"
	if (data_ptr->Subchunk2ID != 0x61746164) {
		printf("ERROR: Invalid data chuck, it must start with 'data' %x\n", data_ptr->Subchunk2ID);
		free(wav_data);
		return ;
	}
	printf("# Data info:\n");
	printf("Size               : %d bytes\n",data_ptr->Subchunk2Size);
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
	wav_start = (void *)&wav_data[0];
	offset = 20 + fmt_ptr->Subchunk1Size + 8;
	//sample_start = (void *)&wav_data[20 + fmt_ptr->Subchunk1Size + 8];
	sample_start = (void *)&wav_data[offset];
	printf("# Calculated info:\n");
	printf("Number of samples  : %d\n",audio_info.num_samples);
	printf("Sample start       : %x\n",(unsigned int)sample_start);
	printf("Wav start          : %x\n",(unsigned int)wav_start);
	printf("Offset calculation : %d\n",((unsigned int)sample_start - (unsigned int)wav_start));
	printf("Offset             : %d\n",offset);
		
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
        printf("PWM Clk freq       : %d Hz\n",pwm_clk_freq);
        printf("Bits per sample    : %d\n",bits_per_sample);
        printf("Sampling rate      : %d Hz\n",sampling_rate);
        printf("Fs rate            : %d Hz\n",audio_info.fs);
        printf("Interpolation      : %d\n",audio_info.inter);
        printf("Int div            : %d\n",idiv);

        if (idiv < 1 || idiv > 0x1000) {
                printf("FATAL_ERROR: idiv out of range: %x\n", idiv);
		free(wav_data);
		while(1);
        }
#ifndef AVOID_HW_WRITES
        clk[PWMCLK_DIV]  = 0x5A000000 | (idiv<<12);

        // source=osc and enable clock
        //*(clk + PWMCLK_CNTL) = 0x5A000011; // 19.2 Mhz
        //*(clk + PWMCLK_CNTL) = 0x5A000015; // 1000Mhz
        //*(clk + PWMCLK_CNTL) = 0x5A000014; // none
        //*(clk + PWMCLK_CNTL) = 0x5A000016; // 500Mhz
        clk[PWMCLK_CNTL] = 0x5A000016; // 500Mhz


        // disable PWM
        pwm[PWM_CTL] = 0;

        RPI_WaitMicroSeconds( 100 );

#endif
	range = 1<<bits_per_sample;
#ifndef AVOID_HW_WRITES
	pwm[PWM_RNG1] = range;
	pwm[PWM_RNG2] = range;
#endif

	//printf ("Range %d \n",range);

	audio_info.samples = (uint16_t *) sample_start;
	audio_info.is_valid = 1;

}

void print_help ( void ) {
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
	printf(": Change EQ Setting (Currently %s)\n",eq_setting_str[audio_info.eq_setting]);

	printf("h");                     
	print_spaces(29);
	printf(": help\n");

}
int16_t midpoint16(int16_t a,int16_t b) {
	return ((int32_t)a+(int32_t)b)>>1;
}

/* Print a number of spaces */
void print_spaces ( int num ) {
	while(num--) printf(" ");
}

// This function is good to set one GPIO port to mirror
// the PWM to be observed by a LA
void mirror_pwm() {
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
}
