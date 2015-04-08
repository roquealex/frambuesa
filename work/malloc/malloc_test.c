#include <stdio.h>
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
  int i;

  rep = 0;

// 10140
//for (i = 0 ; i < 100000 ; i++) {
//for (i = 0 ; i < 1000000 ; i++) {
//for (i = 0 ; i < 38; i++) {
for (i = 0 ; i < 2; i++) {
  //size = 0;
  //bp = 0;
  stream = open_memstream (&bp, &size);
  fprintf (stream, "hello");
  fflush (stream);
  printf ("buf = `%s', size = %d\n", bp, size);
  fprintf (stream, ", world (rep %d)",rep);
  fclose (stream);
  printf ("buf = `%s', size = %d\n", bp, size);

  printf("Playing back the text\n");

  //pbstream = fmemopen(message, 13, "r");
  pbstream = fmemopen(bp, size, "r");
  //while ((ch = fgetc(pbstream)) != EOF) 
  //  printf("Got %c %x\n",ch,ch);
  while ((ch = fgetc(pbstream)) != EOF) 
    /*printf("%c",ch)*/;
  printf("\n",ch);
  fclose(pbstream);
// if size is not reset to 0 then when buffer size is 767 it starts working again
//size = 0;
// forcing the size to 767 doesnt work as workaround but works as if it wasn't 0
//size = 767;
  printf ("buf = `%s', size = %d\n", bp, size);
//free(bp);
//size = 1;
//size = 768;
  rep++;
//10214

}

  //playing back:
while(1);

  return 0;
}
