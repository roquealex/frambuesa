
#ifndef RPI_DMA_H
#define RPI_DMA_H

#include <stdint.h>

#define DMA_BASE       0x20007000UL

#define DMA0_BASE       0x20007000UL
#define DMA1_BASE       0x20007100UL
#define DMA2_BASE       0x20007200UL
#define DMA3_BASE       0x20007300UL
#define DMA4_BASE       0x20007400UL
#define DMA5_BASE       0x20007500UL
#define DMA6_BASE       0x20007600UL
#define DMA7_BASE       0x20007700UL
#define DMA8_BASE       0x20007800UL
#define DMA9_BASE       0x20007900UL
#define DMA10_BASE       0x20007a00UL
#define DMA11_BASE       0x20007b00UL
#define DMA12_BASE       0x20007c00UL
#define DMA13_BASE       0x20007d00UL
#define DMA14_BASE       0x20007e00UL

#define DMA15_BASE       0x20E05000UL

#define DMA_CS 0
#define DMA_CONBLK_AD 1
#define DMA_TI 2
#define DMA_SOURCE_AD 3
#define DMA_DEST_AD 4
#define DMA_TXFR_LEN 5
#define DMA_STRIDE 6
#define DMA_NEXTCONBK 7
#define DMA_DEBUG 8

typedef struct {
	uint32_t ti;
	uint32_t source_ad;
	uint32_t dest_ad;
	uint32_t txfr_len;
	uint32_t stride;
	uint32_t nextconbk;
	uint32_t reserved[2];
} dma_cb_t;


#define DMA_CS_ACTIVE      (1)
#define DMA_CS_END         (1<<(1))
#define DMA_CS_INT         (1<<(2))
#define DMA_CS_ISPAUSED    (1<<(4))
#define DMA_CS_ISHELD      (1<<(5))
#define DMA_CS_ERR         (1<<(8))
#define DMA_CS_ABORT       (1<<(30))
#define DMA_CS_RESET       (1<<(31))

#define DMA_TI_INT_EN      (1)
#define DMA_TI_WAIT_RESP   (1<<(3))
#define DMA_TI_D_INC       (1<<(4))
#define DMA_TI_D_DREQ      (1<<(6))
#define DMA_TI_S_INC       (1<<(8))
#define DMA_TI_S_DREQ      (1<<(10))
#define DMA_TI_NO_WIDE_BURSTS	(1<<26)
#define DMA_TI_PER_MAP(x)		((x)<<16)

#define DMA_DEBUG_READ_ERROR (1<<2)
#define DMA_DEBUG_FIFO_ERROR (1<<1)
#define DMA_DEBUG_READ_LAST_NOT_SET_ERROR (1<<0)

#endif
