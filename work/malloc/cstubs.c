#include <sys/stat.h>

#include <errno.h>

//#undef errno
extern int  errno;

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
	int i;

	for(i = 0; i < len; i++)
	{
		int inchar;
		do {
			inchar = _inbyte(100000);
		} while (inchar == -1);
		//ptr[i] = UART_getc();
		ptr[i] = (char)inchar;
		// echo:
		_outbyte(ptr[i]);

		if (ptr[i] == 13)
		{
			_outbyte('\n');
			return i+1;
		}
	}
	return len;
//	while (1);
    //return 0;
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
		//_outbyte (*ptr++);
		//uart_putc (*ptr++);
	}
	return len;
}

void _exit(int rc) {
	char msg[] = "_exit called. Infinite loop\n";
	_write(0,msg,28);
	while(1) ;
}

int _kill(int pid, int sig){
	errno = EINVAL;
	return -1;
}

int _getpid() {
	return 1;
}



