
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	char *cptr1;
	char *cptr2;
	char *cptr3;
	char *it;
	uint32_t *uintptr1;
	uint32_t *uintptr2;
	size_t cptr1_size = 10;
	size_t cptr2_size = 10;
	size_t cptr3_size = 5;
	size_t uintptr1_size = 20;
	size_t uintptr2_size = 20;
	int i;
	printf("Malloc test\n");
	printf("Size of char %d \n",sizeof(char));
	printf("Size of uint32_t %d \n",sizeof(uint32_t));
	printf("Creating cptr1 of %d elemnts\n", cptr1_size);
	cptr1 = (char *)malloc(cptr1_size*sizeof(char));
	printf("cptr1 points to %x \n", (int)cptr1);
	// Notes on cptr1 on a PC:
	// if the size of the cptr1 is from 1 to 12
	// the difference in cptr1 and cptr2 adresses is 16
	// 800482c0 -> 800482d0
	// If I request 13 then now the difference increments to 24:
	// 800482c0 -> 800482d8
	// this is like this until 20 elements after 21 then it inreases
	// 800482c0 -> 800482e0.
	// The algorithm looks like it always gives 16 to start and then
	// it increases in 8
	// for 1024 it gave a difference of 1032
	// sounds like malloc needs some extra space to save some
	// information about the pointer just created, probably
	// used by free
	
	// Filling the array with some data:
	for (i = 0 ; i < cptr1_size ; i++) {
		cptr1[i] = (char)i;
	}

	printf("Creating cptr2 of %d elemnts\n", cptr2_size);
	cptr2 = (char *)malloc(cptr2_size*sizeof(char));
	printf("cptr2 points to %x \n", (int)cptr2);
	for (i = 0 ; i < cptr2_size ; i++) {
		cptr2[i] = (char)i;
	}

	printf("Creating cptr3 of %d elemnts\n", cptr3_size);
	cptr3 = (char *)malloc(cptr3_size*sizeof(char));
	printf("cptr3 points to %x \n", (int)cptr3);


	printf("Printing content from cptr1 to cptr2\n");
	for (it = cptr1 ; it != cptr2 ; it ++) {
		printf("Addr %x = %x\n",(int)it,*it);
	}
	// regardless of what I do the number before
	// the cptr2 array is 0x13 in 4 bytes
	
	printf("Printing content from cptr2 to cptr3\n");
	for (it = cptr2 ; it != cptr3 ; it ++) {
		printf("Addr %x = %x\n",(int)it,*it);
	}

	// This will supposedly break the alignment but seems
	// that we get in multiples of 8 so probably not:
	printf("Creating uintptr1 of %d elemnts\n", uintptr1_size);
	uintptr1 = (uint32_t *)malloc(uintptr1_size*sizeof(uint32_t));
	printf("uintptr1_size points to %x \n", (int)uintptr1);

	// 88 for a difference of 20 which is really 80
	printf("Creating uintptr2 of %d elemnts\n", uintptr2_size);
	uintptr2 = (uint32_t *)malloc(uintptr2_size*sizeof(uint32_t));
	printf("uintptr2_size points to %x \n", (int)uintptr2);


	free(cptr1);
	free(cptr2);
	free(cptr3);
	free(uintptr1);
	free(uintptr2); // Deleting a not malloc pointer gives error
	return 0;
}

