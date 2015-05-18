/*	
 * Copyright 2001-2010 Georges Menie (www.menie.org)
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University of California, Berkeley nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* this code needs standard functions memcpy() and memset()
   and input/output functions _inbyte() and _outbyte().

   the prototypes of the input/output functions are:
     int _inbyte(unsigned short timeout); // msec timeout
     void _outbyte(int c);

 */
//comment

#include "crc16.h"
#include "wav_player.h"

//#include "sine_wav_capture.h"

//#include "sin_16k_mono.h"
#include "sin_16k_stereo.h"
//#include "sin_44k_mono.h"
//#include "sin_44k_stereo.h"


#include <stdio.h>

/*
#define DEBUG_XMODEM
*/

#define SOH  0x01
#define STX  0x02
#define EOT  0x04
#define ACK  0x06
#define NAK  0x15
#define CAN  0x18
#define CTRLZ 0x1A


#define MAXRETRANS 25

#ifdef DEBUG_XMODEM        
char* dbuf;
#endif

//#define SPYBUFF
#ifdef SPYBUFF
//4096
//int spybuff[4096];
int spybuff[10240];
int spyidx = 0;
int spybuffro = 0;

void init_spybuff() {
	int i;
	//printf("Expr %d\n", sizeof(spybuff)/sizeof(spybuff[0]) );
	for (i = 0 ; i < sizeof(spybuff)/sizeof(spybuff[0]) ; i++) {
		spybuff[i] = 0;
	}
	spyidx = 0;
	spybuffro = 0;
}

void dump_spybuff() {
	int fullrows;
	int lastrow;
	int i,j;
	int idx;
	int columns = 8;
	int buff_size;

	if (spybuffro == 0) {
	// File dump start:
		//buff_size = sizeof(spybuff);
		buff_size = spyidx;
		fullrows = buff_size / columns;
		lastrow = buff_size % columns;
		printf("buff size %d\n", buff_size);
		printf("full rows %d\n", fullrows);
		printf("last rows %d\n", lastrow);
		idx = 0;
		for (i = 0 ; i < fullrows ; i++) {
			for (j = 0 ; j < columns ; j++) {
				if(spybuff[idx] < 0) {
					printf("%d,",spybuff[idx++]);
				} else {
					printf("0x%02x,",spybuff[idx++]);
				}
			}
			printf("\n");
		}
		for (i = 0 ; i < lastrow ; i++) {
			if(spybuff[idx] < 0) {
				printf("%d%c",spybuff[idx++],(i==(lastrow-1))?'\n':',' );
			} else {
				printf("0x%02x%c",spybuff[idx++],(i==(lastrow-1))?'\n':',' );
			}
		}
	// File dump end
	} else {
		printf("(spybuffro != 0) not supported : %d, idx %d\n",spybuffro,spyidx);
	}


}

#endif

#ifdef SPYBUFF
// Save version
int xm_inbyte(unsigned int dly) {
	int ret;
	ret = _inbyte(dly);
	//ret = restore_inbyte(dly);
	//spybuff[spyidx++] = (char)ret;
	spybuff[spyidx++] = ret;
	if (spyidx > sizeof(spybuff)/sizeof(spybuff[0]) ) {
		spyidx = 0;
		spybuffro++;
	}
	return ret;
}

#else
// regular version
int xm_inbyte(unsigned int dly) {
	return _inbyte(dly);
	//return -1;
}
/*
*/
//100ac
//106dc


// restore version
/*
int pbidx = 0;
volatile
int xm_inbyte(unsigned int dly) {
	int data;
	if (pbidx >= sizeof(sine_wav_capture)/sizeof(sine_wav_capture[0])) while(1);
	data = sine_wav_capture[pbidx++];
	//data &= 0x00FF;
	//printf("%02x\n",data);
	//printf("%d\n",data);
	return data;
	//return SOH;
}
*/
#endif

static int check(int crc, const unsigned char *buf, int sz)
{
	if (crc) {
		unsigned short crc = crc16_ccitt(buf, sz);
		unsigned short tcrc = (buf[sz]<<8)+buf[sz+1];
		if (crc == tcrc) {
                        #ifdef DEBUG_XMODEM        
                        dbuf += sprintf(dbuf, "cok1\n");
                        #endif
			return 1;
                        }
	}
	else {
		int i;
		unsigned char cks = 0;
		for (i = 0; i < sz; ++i) {
			cks += buf[i];
		}
		if (cks == buf[sz]) {
                    #ifdef DEBUG_XMODEM        
                    dbuf += sprintf(dbuf, "cok2\n");
                    #endif
		    return 1;
                    }
	}

        #ifdef DEBUG_XMODEM        
        dbuf += sprintf(dbuf, "cer\n");
        #endif
	return 0;
}


static void flushinput(void)
{
	#ifdef SPYBUFF
	// to record the last -1 in the capture
	while (xm_inbyte(DLY_300MS) >= 0) ;
	#else
	while (_inbyte(DLY_300MS) >= 0) ;
	#endif
}


int xmodemReceive(char **dest, int destsz)
{
	unsigned char xbuff[1030]; /* 1024 for XModem 1k + 3 head chars + 2 crc + nul */
	unsigned char *p;
	int bufsz, crc = 0;
	unsigned char trychar = 'C';
	unsigned char packetno = 1;
	int i, c, len = 0;
	int retry, retrans = MAXRETRANS;
        unsigned int wait;


	#ifdef SPYBUFF
	init_spybuff();
	#endif
	FILE *stream;
	//char *bp;
	size_t size;

        #ifdef DEBUG_XMODEM        
        dbuf = (char *) DEBUG_BUF;
        dbuf += sprintf(dbuf, "XR\n");
        #endif
        
	// opening the memstream
	//stream = open_memstream (&bp, &size);
	stream = open_memstream (dest, &size);
	fseek( stream, 0, SEEK_SET ); //_lseek stub not used
	// Exposing the bug (Index 3 should be zero):
	//for (i = 0 ; i < 6; i++) {
	//	printf("memstream_ptr[%d] = %08x\n",i,((unsigned int *)stream->_cookie)[i]);
	//}
/*
*/

	for(;;) {
		for( retry = 0; retry < 80; ++retry) {
			if (trychar) _outbyte(trychar);
			if ((c = xm_inbyte(DLY_1S)) >= 0) {		
				switch (c) {
				case SOH:
					bufsz = 128;
                                        #ifdef DEBUG_XMODEM
                                        dbuf += sprintf(dbuf, "128\n");
                                        #endif
					goto start_recv;
				case STX:
					bufsz = 1024;
                                        #ifdef DEBUG_XMODEM
                                        dbuf += sprintf(dbuf, "1k\n");
                                        #endif
					goto start_recv;
				case EOT:
					_outbyte(ACK);
					flushinput();
                                        #ifdef DEBUG_XMODEM
                                        dbuf += sprintf(dbuf, "EOT\n");
                                        #endif

					#ifdef SPYBUFF
					dump_spybuff();
					#endif
					// When this close is called it looks
					// like r0 doesnt have the correct
					// value. file points to 0x109b8
					// and r0 points to 27ec4. After calling
					// mem open the 0x27ec4 is the address
					// given to stream.
					//
					fclose (stream);
					//printf("\nERROR in size %d != %d\n",
					//	size,len);
					//for (i = 0 ; i < len ; i++)
					//	printf("%02x ",(*dest)[i]);
					//pbidx = 0;
					return len; /* normal end */
				case CAN:
                                        #ifdef DEBUG_XMODEM
                                        dbuf += sprintf(dbuf, "CAN\n");
                                        #endif
					if ((c = xm_inbyte(DLY_1S)) == CAN) {
						flushinput();
						_outbyte(ACK);
						fclose (stream);
						return -1; /* canceled by remote */
					}
					break;
				default:
					break;
				}
			}
		}
		if (trychar == 'C') { trychar = NAK; continue; }
		flushinput();
		_outbyte(CAN);
		_outbyte(CAN);
		_outbyte(CAN);
		fclose (stream);
		return -2; /* sync error */

	start_recv:
                #ifdef DEBUG_XMODEM
                dbuf += sprintf(dbuf, "sr\n");
                #endif
		if (trychar == 'C') crc = 1;
		trychar = 0;
		p = xbuff;
		*p++ = c;
		for (i = 0;  i < (bufsz+(crc?1:0)+3); ++i) {
			if ((c = xm_inbyte(DLY_1S)) < 0) goto reject;
			*p++ = c;
		}
                
                
		if (xbuff[1] == (unsigned char)(~xbuff[2]) && 
			(xbuff[1] == packetno || xbuff[1] == (unsigned char)packetno-1) &&
			check(crc, &xbuff[3], bufsz)) {
			if (xbuff[1] == packetno)	{
				register int count = destsz - len;
				if (count > bufsz) count = bufsz;
				if (count > 0) {
					//memcpy (&dest[len], &xbuff[3], count);
					for (i = 0;  i < (count); ++i) {
						//dest[len+i] = xbuff[3+i];
						fputc(xbuff[3+i],stream);
						//fprintf(stream,"X");
					}
					len += count;
				}
				++packetno;
				retrans = MAXRETRANS+1;
			}
			if (--retrans <= 0) {
				flushinput();
				_outbyte(CAN);
				_outbyte(CAN);
				_outbyte(CAN);
                                #ifdef DEBUG_XMODEM
                                dbuf += sprintf(dbuf, "mx e\n");                
                                #endif
				fclose (stream);
				return -3; /* too many retry error */
			}
			_outbyte(ACK);
			continue;
                        

		}
                else {
                        #ifdef DEBUG_XMODEM
                        dbuf += sprintf(dbuf, "bad\n");                
                        #endif
                        }
                        
	reject:
                #ifdef DEBUG_XMODEM
                dbuf += sprintf(dbuf, "rj %d\n", c);
                #endif
                
		flushinput();
		_outbyte(NAK);
	}
}

/*
Debug notes, on a good case where the file is closed properly this is the content of stream:

(gdb) print *(FILE *)0x27ec4
$4 = {_p = 0x284d8 "", _r = 0, _w = 0, _flags = 10376, 
  _file = -1, _bf = {
    _base = 0x280d8 "0\276\067Â\310\001δӒ٘\337\277\345\377\353S\362\267\370\036\377\214\005\360\vH\022\220\030\275\036\315$\265*y0", _size = 1024}, _lbfsize = 0, 
  _cookie = 0x28068, _read = 0x0, 
  _write = 0x125e8 <memwriter>, 
  _seek = 0x122e0 <memseeker>, 
  _close = 0x12594 <memcloser>, _ub = {_base = 0x0, 
    _size = 0}, _up = 0x0, _ur = 0, _ubuf = "\000\000", 
  _nbuf = "", _lb = {_base = 0x0, _size = 0}, _blksize = 0, 
  _offset = 0, _data = 0x0, _lock = 0, _mbstate = {
    __count = 0, __value = {__wch = 0, 
      __wchb = "\000\000\000"}}, _flags2 = 0}

On the bad case this is the contents of the stream before closing:

(gdb) print *(FILE *)0x27ec4
$2 = {_p = 0x284d7 "\271cE\350\026)\v", _r = 0, _w = 1, 
  _flags = 10440, _file = -1, _bf = {
    _base = 0x280d8 "\276\067Â\310\001δӒ٘\337\277\345\377\353S\362\267\370\036\377\214\005\360\vH\022\220\030\275\036\315$\265*y0", _size = 1024}, _lbfsize = 0, 
  _cookie = 0x28068, _read = 0x0, 
  _write = 0x125e8 <memwriter>, 
  _seek = 0x122e0 <memseeker>, 
  _close = 0x12594 <memcloser>, _ub = {_base = 0x0, 
    _size = 0}, _up = 0x0, _ur = 0, _ubuf = "\000\000", 
  _nbuf = "", _lb = {_base = 0x0, _size = 0}, _blksize = 0, 
  _offset = 0, _data = 0x0, _lock = 0, _mbstate = {
    __count = 0, __value = {__wch = 0, 
      __wchb = "\000\000\000"}}, _flags2 = 0}


At the begining this is the good one :

(gdb) print stream
$2 = (FILE *) 0x27ec4
(gdb) print *stream
$3 = {_p = 0x0, _r = 0, _w = 0, _flags = 8200, _file = -1, 
  _bf = {_base = 0x0, _size = 0}, _lbfsize = 0, 
  _cookie = 0x28068, _read = 0x0, 
  _write = 0x125e8 <memwriter>, 
  _seek = 0x122e0 <memseeker>, 
  _close = 0x12594 <memcloser>, _ub = {_base = 0x0, 
    _size = 0}, _up = 0x0, _ur = 0, _ubuf = "\000\000", 
  _nbuf = "", _lb = {_base = 0x0, _size = 0}, _blksize = 0, 
  _offset = 0, _data = 0x0, _lock = 0, _mbstate = {
    __count = 0, __value = {__wch = 0, 
      __wchb = "\000\000\000"}}, _flags2 = 0}

In the bad run:

(gdb) print stream
$2 = (FILE *) 0x27ec4
(gdb) print *stream
$3 = {_p = 0x0, _r = 0, _w = 0, _flags = 8200, _file = -1, 
  _bf = {_base = 0x0, _size = 0}, _lbfsize = 0, 
  _cookie = 0x28068, _read = 0x0, 
  _write = 0x125e8 <memwriter>, 
  _seek = 0x122e0 <memseeker>, 
  _close = 0x12594 <memcloser>, _ub = {_base = 0x0, 
    _size = 0}, _up = 0x0, _ur = 0, _ubuf = "\000\000", 
  _nbuf = "", _lb = {_base = 0x0, _size = 0}, _blksize = 0, 
  _offset = 0, _data = 0x0, _lock = 0, _mbstate = {
    __count = 0, __value = {__wch = 0, 
      __wchb = "\000\000\000"}}, _flags2 = 0}



*/

