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


/* These uart input delay values are correct for a 33MHz system clock */
#define DLY_1S          1000
#define DLY_300MS       300
//#define FILE_MAX_SIZE   0x00800000 /* 8MB max Xmodem transfer file size        */
#define FILE_MAX_SIZE   0x01000000 /* 16MB max Xmodem transfer file size        */

#include <stdint.h>

void print_spaces ( int num );
void print_help ( void );
void play_audio ( void );
void stop_audio ( void );
void load_audio ( unsigned int );

int16_t midpoint16(int16_t, int16_t);


/*
localparam AMBER_AUDIO_SEQ_START_ADDRESS =  16'h0000;
localparam AMBER_AUDIO_SEQ_SAMPLE_COUNT  =  16'h0004;
localparam AMBER_AUDIO_SEQ_CMD           =  16'h0008;
localparam AMBER_AUDIO_SEQ_STATUS        =  16'h000c;
localparam AMBER_AUDIO_SEQ_CHL_CONFIG    =  16'h0010;
localparam AMBER_AUDIO_SEQ_SAMPLE_RATE   =  16'h0014;
*/
#define AMBER_AUDIO_SEQ_START_ADDRESS_OFF  0
#define AMBER_AUDIO_SEQ_SAMPLE_COUNT_OFF   1
#define AMBER_AUDIO_SEQ_CMD_OFF            2
#define AMBER_AUDIO_SEQ_STATUS_OFF         3
#define AMBER_AUDIO_SEQ_CHL_CONFIG_OFF     4
#define AMBER_AUDIO_SEQ_SAMPLE_RATE_OFF    5
#define AMBER_AUDIO_SEQ_BASE 0x20000000

/* Our structure */
struct riff_header
{
	//int x,y,z;
	unsigned long ChunkID;
	//Contains the letters "RIFF" in ASCII form
	//(0x52494646 big-endian form).
	unsigned long ChunkSize;
	//36 + SubChunk2Size, or more precisely:
	//4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
	//This is the size of the rest of the chunk 
	//following this number.  This is the size of the 
	//entire file in bytes minus 8 bytes for the
	//two fields not included in this count:
	//ChunkID and ChunkSize.
	unsigned long Format;
	//Contains the letters "WAVE"
	//(0x57415645 big-endian form).
};

struct fmt
{
	unsigned long Subchunk1ID;
	//Contains the letters "fmt "
	//(0x666d7420 big-endian form).
	unsigned int Subchunk1Size;
	//16 for PCM.  This is the size of the
	//rest of the Subchunk which follows this number.
	unsigned short AudioFormat;
	//PCM = 1 (i.e. Linear quantization)
	//Values other than 1 indicate some 
	//form of compression.
	unsigned short NumChannels;
	//Mono = 1, Stereo = 2, etc.
	unsigned long SampleRate;
	//8000, 44100, etc.
	unsigned long ByteRate;
	//== SampleRate * NumChannels * BitsPerSample/8
	unsigned short BlockAlign;
	//== NumChannels * BitsPerSample/8
	//The number of bytes for one sample including
	//all channels. I wonder what happens when
	//this number isn't an integer?
	unsigned short BitsPerSample;
	//8 bits = 8, 16 bits = 16, etc.

};

struct data
{
	unsigned long Subchunk2ID;
	//Contains the letters "data"
	//(0x64617461 big-endian form).
	unsigned long Subchunk2Size;
	//== NumSamples * NumChannels * BitsPerSample/8
	//This is the number of bytes in the data.
	//You can also think of this as the size
	//of the read of the subchunk following this 
	//number.

};

