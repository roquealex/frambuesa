#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


//char message[] = "Hello World!";

int
main (void)
{
  char *bp;
  size_t size;
  FILE *stream;

  int ch;
  FILE *pbstream;

  int rep;
  int i, j;

  int freq_size = 256;
  int freq[freq_size];
  int seed;

  rep = 0;

// 10140
//for (i = 0 ; i < 100000 ; i++) {
//for (i = 0 ; i < 1000000 ; i++) {
//for (i = 0 ; i < 38; i++) {
  seed = 1;
  printf ("Memstream test seed %d\n",seed);
  srand(seed);
  for (i = 0 ; i < freq_size; i++) freq[i] = 0;

for (i = 0 ; i < 10000000 ; i++) {
  int32_t *memstream_ptr;
  int diff = 'z' - 'A' + 1;
  //size = 0;
  //bp = 0;
  // Need to make sure that size is 0 before calling otherwise
  // malloc will be called for size numeber of elements
//size = 0x800000; 
  size = 0;
  printf("Size before calling open %d\n",size);
  stream = open_memstream (&bp, &size);
/*
  memstream_ptr = (int32_t *)stream->_cookie;
  for (j = 0 ; j < 6; j++) {
    printf("memstream_ptr[%d] = %08x\n",j,memstream_ptr[j]);
  }
*/
  // This is the workaround for the bug in the library:
  fseek( stream, 0, SEEK_SET ); //_lseek stub not used
  // Exposing the bug:
/*
  memstream_ptr = (int32_t *)stream->_cookie;
  for (j = 0 ; j < 6; j++) {
    printf("memstream_ptr[%d] = %08x\n",j,memstream_ptr[j]);
  }
*/

/*
  for (j = 0 ; j < 10000000 ; j++) {
    unsigned char randc;
    //randc = 'A' + rand()%diff;
    randc = rand();
    fputc(randc,stream);
  }
*/
  fprintf (stream, "hello");
  fflush (stream);
  printf ("buf = `%s', size = %d\n", bp, size);
  fprintf (stream, ", world (rep %d)",rep);

  fclose (stream);
  printf ("buf = `%s', size = %d\n", bp, size);
  // 32 - 126 is printable
/*
  printf("Calculating freq on %d elements\n",size);
  for(j = 0 ; j < size ; j++) {
    freq[(unsigned)bp[j]]++;
  }
  for (j = 0 ; j < freq_size; j++) {
    //printf ("freq[%d] =  %d , char %c\n", j, freq[j], (j>=32 && j<=126)?j:0);
    printf ("(%c) %d", (j>=32 && j<=126)?j:' ',freq[j]);
    if ((j&0x3)==0x03) printf("\n");
    else printf("\t");
  }
*/
  //for(j = 0 ; j < size ; j++) {
  //  printf ("0x%02x ", bp[j]);
  //}
  printf ("buf = `large', size = %d\n", size);

  printf("Playing back the text\n");

/*
  //The play back works even worse
  //pbstream = fmemopen(message, 13, "r");
  pbstream = fmemopen(bp, size, "r");
  //while ((ch = fgetc(pbstream)) != EOF) 
  //  printf("Got %c %x\n",ch,ch);
  while ((ch = fgetc(pbstream)) != EOF) {
    //printf("%c",ch)
  }
  printf("\n");
  fclose(pbstream);
*/


// if size is not reset to 0 then when buffer size is 767 it starts working again
//size = 0;
// forcing the size to 767 doesnt work as workaround but works as if it wasn't 0
//size = 767;
  //printf ("buf = `%s', size = %d\n", bp, size);
free(bp);
//size = 1;
//size = 768;
  rep++;
//10214

}

  //playing back:
while(1);

  return 0;
}

/* 
 Debug notes

With a single element. Just before closing

good run :

(gdb) print stream 
$5 = (FILE *) 0x23ca4
(gdb) print *stream
$6 = {_p = 0x23eb9 "", _r = 0, _w = 1023, _flags = 10376, 
  _file = -1, _bf = {_base = 0x23eb8 "-", _size = 1024}, 
  _lbfsize = 0, _cookie = 0x23e48, _read = 0x0, 
  _write = 0x11b40 <memwriter>, 
  _seek = 0x11838 <memseeker>, 
  _close = 0x11aec <memcloser>, _ub = {_base = 0x0, 
    _size = 0}, _up = 0x0, _ur = 0, _ubuf = "\000\000", 
  _nbuf = "", _lb = {_base = 0x0, _size = 0}, _blksize = 0, 
  _offset = 0, _data = 0x0, _lock = 0, _mbstate = {
    __count = 0, __value = {__wch = 0, 
      __wchb = "\000\000\000"}}, _flags2 = 0}

On the failing one:

(gdb) print *stream
$2 = {
  _p = 0x23eb9 "\222\211f\300LxE]\034\367}\343\374'ρ\250\t[\312)\203i\f\205\037\344c\256\035\034t\326Y\023<C\261\372\351[\363\v\227F:\362l\217\ngFB$\202k*$\245\353\250*\242\365s\247\062\246д\036l}\210W2\016\373\203\343\005K\265\037\372\030NjA\263\301\217:\260R\357\355{N3\222/;\336e}\220\374\305K\330z\274\300\315\024s\301\364\373\061\212Y\025\221\321\033\063\362\203\252\036\311|\177%\260\036\\\205\060\237\265B\323\356\305a\021\227\325\070\255\017\251\326\332G\367\215_!a\255\022", 
  _r = 0, _w = 1023, _flags = 10376, _file = -1, _bf = {
    _base = 0x23eb8 "-\222\211f\300LxE]\034\367}\343\374'ρ\250\t[\312)\203i\f\205\037\344c\256\035\034t\326Y\023<C\261\372\351[\363\v\227F:\362l\217\ngFB$\202k*$\245\353\250*\242\365s\247\062\246д\036l}\210W2\016\373\203\343\005K\265\037\372\030NjA\263\301\217:\260R\357\355{N3\222/;\336e}\220\374\305K\330z\274\300\315\024s\301\364\373\061\212Y\025\221\321\033\063\362\203\252\036\311|\177%\260\036\\\205\060\237\265B\323\356\305a\021\227\325\070\255\017\251\326\332G\367\215_!a\255\022", _size = 1024}, _lbfsize = 0, _cookie = 0x23e48, 
---Type <return> to continue, or q <return> to quit---
  _read = 0x0, _write = 0x11b40 <memwriter>, 
  _seek = 0x11838 <memseeker>, 
  _close = 0x11aec <memcloser>, _ub = {_base = 0x0, 
    _size = 0}, _up = 0x0, _ur = 0, _ubuf = "\000\000", 
  _nbuf = "", _lb = {_base = 0x0, _size = 0}, _blksize = 0, 
  _offset = 0, _data = 0x0, _lock = 0, _mbstate = {
    __count = 0, __value = {__wch = 0, 
      __wchb = "\000\000\000"}}, _flags2 = 0}

When the program is stuck and I ctrl c I get:

Program received signal SIGINT, Interrupt.
0x00010278 in _sbrk (incr=681340928) at cstubs.c:21
21		if ( ( heap_end + incr ) > _get_stack_pointer() ) {

(gdb) print heap_end
$3 = 0x25000 "-l\200i\035\374Tx\370\341\335\371\207\t\342\317\062\370KF\217\366\206\204cz\203\t̺\237*3\261U@I"

(gdb) backtrace
#0  0x00010278 in _sbrk (incr=681340928) at cstubs.c:21
#1  0x0001255c in _sbrk_r ()
#2  0x000112f4 in _malloc_r ()
#3  0x000122bc in _realloc_r ()
#4  0x00011c28 in memwriter ()
#5  0x00010618 in __sflush_r ()
#6  0x00010424 in _fclose_r ()
#7  0x00010104 in main () at malloc_test.c:52

(gdb) print $sp
$5 = (void *) 0x6fffb1c
               289EC000
               289C7000
The if evaluates true, the addition of the incr + heap_end passes the sp: 289EC000

(gdb) x /i $pc
=> 0x10278 <_sbrk+44>:	b	0x10278 <_sbrk+44>

Notes on the failing one:

_sbrk is called from fputc asking for 4096 elements and finally from close where it asks for many elements

On the good one however the fputc requests the 4096 elements but there is no request in the close

Experiment:

reference source file in newlib:

./newlib/libc/stdio/open_memstream.c 

Break on memwriter and dump the pointers on both
Contents of parameters r1-r3

(gdb) x /x $r1
0x23e48:	0x00023e48
(gdb) x /x $r2
0x23eb8:	0x0000002d
(gdb) x /x $r3
0x1:	0x04e3a000

It looks like the r2 parameter is the buffer where our single "-" character is written.

r3 could be the desired size which is 1 in this case

From a bit more dump on r1:

(gdb) x /6xw $r1
0x23e48:	0x00023e48	0x06ffffe4	0x06ffffe8	0x00000000
0x23e58:	0x00000000	0x00000040

We can assume:
pos = eof = 0
max = 0x040 (64)

On the bad one:

(gdb) x /x $r2
0x23eb8:	0x6689922d
(gdb) x /x $r3
0x1:	0x04e3a000

The r2 parameter is a bit good knowing that only a single 0x2d has been written so far.

The r3 is ok since it is 1 (it is not a pointer so 0x04e3a000 has nothing to do)

(gdb) x /6x $r1
0x23e48:	0x00023e48	0x06ffffe4	0x06ffffe8	0x289c68df
0x23e58:	0x00000000	0x00000040

r1 looks equaly bad where pos = 0x289c68df , eof = 0 and max still 0x40

Following this code:

  if (c->pos + n >= c->max)
    {
      size_t newsize = c->max * 3 / 2;
      if (newsize < c->pos + n + 1)
	newsize = c->pos + n + 1;
      cbuf = _realloc_r (ptr, cbuf, newsize);

newsize should be 0x289c68df + 1 + 1 = 0x289C68E1 which is close to the new size requested by the _sbrk call: 0x289C7000 -> incr=681340928
#0  0x00010278 in _sbrk (incr=681340928) at cstubs.c:21

Find where the pos fields gets assigned and how it gets corrupted
This is the field _cookie in the FILE.

For the good one after memopen:

(gdb) print stream->_cookie            
$6 = (void *) 0x23e48
(gdb) x /6x stream->_cookie
0x23e48:	0x00023e48	0x06ffffe4	0x06ffffe8	0x00000000
0x23e58:	0x00000000	0x00000040

For the bad one:

(gdb) print stream->_cookie
$3 = (void *) 0x23e48
(gdb) x /6x stream->_cookie
0x23e48:	0x00023e48	0x06ffffe4	0x06ffffe8	0x289c68df
0x23e58:	0x00000000	0x00000040

At the creation time the pos field is already dirty

Potential problem:

The assembly for internal_open_memstream_r shows an error. Knowing that r4 is the _cookie structure of type memstream, the following asm doesnt match c:

   119f0:	e8840210 	stm	r4, {r4, r9}
   119f4:	e5847008 	str	r7, [r4, #8]
   119f8:	e5846010 	str	r6, [r4, #16]
   119fc:	e5846018 	str	r6, [r4, #24]
   11a00:	e5c4801c 	strb	r8, [r4, #28]

Doesn't match the c equivalent, r7 is the size ptr, then r6 has a zero, #12 is the expected index for pos which is not initialized

  c->storage = c;
  c->pbuf = buf;
  c->psize = size;
  c->pos = 0;
  c->eof = 0;
  c->saved.w = L'\0';
  c->wide = (int8_t) wide;

Exp indexes
typedef struct memstream {
  void *storage; // 0
  char **pbuf; // 4
  size_t *psize; // 8
  size_t pos; // 12
  size_t eof; // 16
  size_t max; // 20
  union { //24
    char c;
    wchar_t w;
  } saved;
  int8_t wide; // 28?
} memstream;




*/

