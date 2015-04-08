#include <stdio.h>


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

//for (i = 0 ; i < 100000 ; i++) {
//for (i = 0 ; i < 1000000 ; i++) {
for (i = 0 ; i < 2 ; i++) {
  stream = open_memstream (&bp, &size);
  fprintf (stream, "hello");
  fflush (stream);
  printf ("buf = `%s', size = %d\n", bp, size);
  fprintf (stream, ", world (rep %d)",rep);
  fclose (stream);
  printf ("buf = `%s', size = %d\n", bp, size);

  printf("Playing back te text\n");

  //pbstream = fmemopen(message, 13, "r");
  pbstream = fmemopen(bp, size, "r");
  while ((ch = fgetc(pbstream)) != EOF) 
    printf("Got %c %x\n",ch,ch);
  fclose(pbstream);

  rep++;

}

  //playing back:

  return 0;
}
