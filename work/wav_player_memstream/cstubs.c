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
#include <sys/stat.h>

extern caddr_t _get_stack_pointer(void);


caddr_t _sbrk( int incr ) {
	extern char _end;
	static char* heap_end;
	char* prev_heap_end;

	if (heap_end == 0 )
		heap_end = &_end;

	prev_heap_end = heap_end;

	if ( ( heap_end + incr ) > _get_stack_pointer() ) {
		while(1);
	}

	heap_end += incr;
	return (caddr_t)prev_heap_end;
}

int _isatty(int file)
{
    return 1;
}

/* Set position in a file. Minimal implementation: */
int _lseek(int file, int ptr, int dir)
{
    return 0;
}

int _read( int file, char *ptr, int len )
{
    return 0;
}

int _close( int file )
{
    return -1;
}

int _fstat( int file, struct stat *st )
{
    st->st_mode = S_IFCHR;
    return 0;
}


int _write(int file, char *ptr, int len) {
	int todo;
	char curr_char;
		  
	for (todo = 0; todo < len; todo++) {
		curr_char = (*ptr++);
		_outbyte(curr_char);
		if (curr_char == '\n')
			_outbyte('\r');
	}
	return len;
}
