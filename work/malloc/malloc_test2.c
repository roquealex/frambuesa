#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	char *cptr1;
	char *cptr2;
	char *cptr3;
	char *cptr4;

	size_t cptr1_size = 100;
	size_t cptr2_size = 120;
	size_t cptr3_size = 130;
	size_t cptr4_size = 140;

	printf("Malloc and delete test\n");
	printf("Creating 4 contigous ptrs\n");

	printf("cptr1 size %d\n",cptr1_size);
	printf("cptr2 size %d\n",cptr2_size);
	printf("cptr3 size %d\n",cptr3_size);
	printf("cptr4 size %d\n",cptr4_size);
	cptr1 = (char*)malloc(cptr1_size);
	cptr2 = (char*)malloc(cptr2_size);
	cptr3 = (char*)malloc(cptr3_size);
	cptr4 = (char*)malloc(cptr4_size);
	printf("cptr1 addr %x\n",(int)cptr1);
	printf("cptr2 addr %x\n",(int)cptr2);
	printf("cptr3 addr %x\n",(int)cptr3);
	printf("cptr4 addr %x\n",(int)cptr4);

	printf("Deleting cptr2 cptr4\n");
	free(cptr2);
	free(cptr4);
	printf("cptr1 addr %x\n",(int)cptr1);
	printf("cptr2 addr %x\n",(int)cptr2);
	printf("cptr3 addr %x\n",(int)cptr3);
	printf("cptr4 addr %x\n",(int)cptr4);

	printf("Creating a different cptr4\n");
	cptr4 = (char*)malloc(50);
	printf("cptr1 addr %x\n",(int)cptr1);
	printf("cptr2 addr %x\n",(int)cptr2);
	printf("cptr3 addr %x\n",(int)cptr3);
	printf("cptr4 addr %x\n",(int)cptr4);

	printf("Creating a different cptr2\n");
	//cptr2 = (char*)malloc(64); // fits on pc only
	//cptr2 = (char*)malloc(16); // fits on pc only
	cptr2 = (char*)malloc(69); // doesnt fit
	printf("cptr1 addr %x\n",(int)cptr1);
	printf("cptr2 addr %x\n",(int)cptr2);
	printf("cptr3 addr %x\n",(int)cptr3);
	printf("cptr4 addr %x\n",(int)cptr4);

	// Now with those 50 and 64 numbers it fits
	// exactly into the old space between cptr2
	// and cptr3:
	// 56 cptr4 
	// 72 cptr2
	// 128 cptr2 old

	free(cptr1);
	free(cptr2);
	free(cptr3);
	free(cptr4);
	while(1);
	return 0;
}
