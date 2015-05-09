#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "rpi-dma.h"
#include "rpi-pwm.h"
#include "rpi-gpio.h"
#include "rpi-systimer.h"

#define BCM2708_PERI_BASE       0x20000000
//#define GPIO_BASE               (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */
//#define PWM_BASE                (BCM2708_PERI_BASE + 0x20C000) /* PWM controller */
#define CLOCK_BASE              (BCM2708_PERI_BASE + 0x101000)
#define PWMCLK_CNTL 40
#define PWMCLK_DIV  41


/** GPIO Register set */
volatile unsigned int* gpio = (unsigned int*)GPIO_BASE;
volatile unsigned int* pwm = (unsigned int*)PWM_BASE;
volatile unsigned int* clk = (unsigned int*)CLOCK_BASE;

volatile unsigned int* dma0 = (unsigned int*)DMA0_BASE;
//volatile unsigned int dma0[20];

/** Main function - we'll never return from here */
//void kernel_main( unsigned int r0, unsigned int r1, unsigned int atags )

char str[] = "This is the test message that is going "
	     "to be copied by a DMA channel.";



void main () 
{
	int i;

char dest[256];
char str2[] = "Second text copied by DMA";
	dma_cb_t cb[2];
	dma_cb_t *cb_ptr_un;
	dma_cb_t *cb_ptr;
	dma_cb_t *cb_ptr_linked;
	void *mem;
	uint32_t align;

	printf ("DMA program\n");
	printf("Test string : %s\n",str);
	printf("length of test array : %dB\n",sizeof(str));
	printf("length of test string : %d\n",strlen(str));
	//256 aligned: 100000000
	//32 aligned: 100000
	// Note:
	// good: 000000
	// next good:  100000
	// worst case: 000001
	// diff:       011111 31 align-1
	// With malloc giving 8 byte alignment (double)
	// good: 000000
	// next good:  100000
	// worst case: 001000
	// diff:       011000  24
	printf("Size of DMA control block %d\n",sizeof(dma_cb_t));
	for (i = 0 ; i < 2 ; i++) {
		printf("Addr of cb[%d] = %x\n", i , (uintptr_t)&cb[i]);
	}
	mem = malloc(15);
	printf("Addr of mem = %x\n", (uintptr_t)mem);
	free(mem);
	cb_ptr_un = (dma_cb_t*)malloc(sizeof(dma_cb_t));
	printf("Addr of cb_ptr_un = %x\n", (uintptr_t)cb_ptr_un);
	free(cb_ptr_un);
	align = sizeof(dma_cb_t);
	printf("Alignment to be used %d\n",align);
	mem = malloc(2*sizeof(dma_cb_t) + (align-1));
	printf("Addr of mem = %x\n", (uintptr_t)mem);
	cb_ptr = (dma_cb_t*)( ((uintptr_t)mem+(align-1)) & ~(uintptr_t)(align-1) );
	printf("Addr of cb_ptr = %x\n", (uintptr_t)cb_ptr);
	cb_ptr[0].ti = DMA_S_INC | DMA_D_INC;
	// By recomendation of the BCM doc using this alias
	// region in the bus addresses for ram (0xC0000000):
	//cb_ptr->source_ad = (uint32_t)str | 0xC0000000;
	//cb_ptr->dest_ad = (uint32_t)dest | 0xC0000000;
	// Not all the address work in the 0xC0000000 region
	cb_ptr[0].source_ad = (uint32_t)str ;
	cb_ptr[0].dest_ad = (uint32_t)dest ;
	cb_ptr[0].txfr_len = sizeof(str) - 1;
	cb_ptr[0].stride = 0;
	cb_ptr[0].nextconbk = (uint32_t)&cb_ptr[1];

	cb_ptr[1].ti = DMA_S_INC | DMA_D_INC;
	cb_ptr[1].source_ad = (uint32_t)str2 ;
	cb_ptr[1].dest_ad = cb_ptr[0].dest_ad + cb_ptr[0].txfr_len ;
	cb_ptr[1].txfr_len = sizeof(str2);
	cb_ptr[1].stride = 0;
	cb_ptr[1].nextconbk = 0;

	printf("Copying %d bytes from %x to %x\n",cb_ptr[0].txfr_len,cb_ptr[0].source_ad,cb_ptr[0].dest_ad);
	dma0[DMA_CS] = DMA_RESET;
	//sleep(1);
	RPI_WaitMicroSeconds(1000000);
	printf("Reset done\n");
	dma0[DMA_DEBUG] = DMA_DEBUG_READ_ERROR | DMA_DEBUG_FIFO_ERROR | DMA_DEBUG_READ_LAST_NOT_SET_ERROR; 
	//dma0[DMA_CONBLK_AD] = (uint32_t)cb_ptr | 0xC0000000;
	dma0[DMA_CONBLK_AD] = (uint32_t)cb_ptr ;
	dma0[DMA_CS] = DMA_ACTIVE;
	//sleep(1);
	RPI_WaitMicroSeconds(1000000);
	printf("Src %x and dest %x from registers\n",dma0[DMA_SOURCE_AD],dma0[DMA_DEST_AD]);
	printf("Destination buffer has: %s\n",dest);
	free(mem);

	while(1);
}
