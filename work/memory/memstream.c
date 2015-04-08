#include <stdio.h>

char message[] = "Hello World!";

int main() {
	int ch;
	FILE *stream;
	printf("This is a memory stream test\n");

	stream = fmemopen(message, 13, "r");
	while ((ch = fgetc(stream)) != EOF) 
		printf("Got %c %x\n",ch,ch);
	fclose(stream);

	return 0;
}

