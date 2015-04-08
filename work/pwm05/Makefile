# ----------------------------------------------------------------
#                                                               //
#   Makefile for the blinker01 application.                   //
#                                                               //
#   This file is part of the Amber project                      //
#   http://www.opencores.org/project,amber                      //
#                                                               //
#   Description                                                 //
#   Simple makefile that defines the sources and target.        //
#   Uses the common.mk common makefile script.                  //
#                                                               //
#   Author(s):                                                  //
#       - Conor Santifort, csantifort.amber@gmail.com           //
#                                                               //
#/ ///////////////////////////////////////////////////////////////
#                                                               //
#  Copyright (C) 2010 Authors and OPENCORES.ORG                 //
#                                                               //
#  This source file may be used and distributed without         //
#  restriction provided that this copyright statement is not    //
#  removed from the file and that any derivative work contains  //
#  the original copyright notice and the associated disclaimer. //
#                                                               //
#  This source file is free software; you can redistribute it   //
#  and/or modify it under the terms of the GNU Lesser General   //
#  Public License as published by the Free Software Foundation; //
#  either version 2.1 of the License, or (at your option) any   //
#  later version.                                               //
#                                                               //
#  This source is distributed in the hope that it will be       //
#  useful, but WITHOUT ANY WARRANTY; without even the implied   //
#  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      //
#  PURPOSE.  See the GNU Lesser General Public License for more //
#  details.                                                     //
#                                                               //
#  You should have received a copy of the GNU Lesser General    //
#  Public License along with this source; if not, download it   //
#  from http://www.opencores.org/lgpl.shtml                     //
#                                                               //
# ----------------------------------------------------------------
# Assembly source files

#SRC = blinker01.c start.S
SRC = pwm05.c rpi-systimer.c cstartup.c cstubs.c start.S
TGT = pwm05.elf
#LDS = sections.lds

#USE_MINI_LIBC = 1
#CREATE_FLT_OUTPUT = 1

#include ../include/common.mk



LIBC_OBJ         = ../mini-libc/printf.o ../mini-libc/libc_asm.o ../mini-libc/memcpy.o
TOOLSPATH        = ../tools
#AMBER_CROSSTOOL ?= amber-crosstool-not-defined
AMBER_CROSSTOOL ?= arm-none-eabi

  AS    = $(AMBER_CROSSTOOL)-as
  CC    = $(AMBER_CROSSTOOL)-gcc
 CXX    = $(AMBER_CROSSTOOL)-g++
  AR    = $(AMBER_CROSSTOOL)-ar
  LD    = $(AMBER_CROSSTOOL)-ld
  #LD    = $(AMBER_CROSSTOOL)-gcc
  DS    = $(AMBER_CROSSTOOL)-objdump
  OC    = $(AMBER_CROSSTOOL)-objcopy
 ELF    = $(TOOLSPATH)/amber-elfsplitter
 BMF32  = ../tools/amber-memparams32.sh
 BMF128 = ../tools/amber-memparams128.sh

 MMP32  = $(addsuffix _memparams32.v, $(basename $(TGT)))
 MMP128 = $(addsuffix _memparams128.v, $(basename $(TGT)))
 MEM    = $(addsuffix .mem, $(basename $(TGT)))
 DIS    = $(addsuffix .dis, $(basename $(TGT)))
 FLT    = $(addsuffix .flt, $(basename $(TGT)))
 
ifdef USE_MINI_LIBC
 OBJ = $(addsuffix .o,   $(basename $(SRC))) $(LIBC_OBJ)
else
 OBJ = $(addsuffix .o,   $(basename $(SRC)))
endif

ifdef LDS
    TLDS = -T $(LDS)
else
    TLDS = 
endif

ifndef TGT
    TGT = aout.elf
endif

ifdef MIN_SIZE
    # optimize for size
    OPTIMIZE = -Os
else
    # optimize for speed
    #OPTIMIZE = -O3
    OPTIMIZE = -O2
endif

 MAP = $(addsuffix .map, $(basename $(TGT))) 
 
 ASFLAGS = -I../include 
  #CFLAGS = -c $(OPTIMIZE) -march=armv2a -mno-thumb-interwork -ffreestanding -I../include
  # if -nostartfiles is omited then an extra .init section is added with _init
  #AMBER:CFLAGS = -c $(OPTIMIZE) -march=armv2a -mno-thumb-interwork -nostartfiles
  CFLAGS = -c $(OPTIMIZE) -mfpu=vfp -mfloat-abi=softfp -march=armv6zk -mtune=arm1176jzf-s -nostartfiles
  #CFLAGS = -c $(OPTIMIZE) -march=armv2a -mno-thumb-interwork
 DSFLAGS = -C -S -EL
 #LDFLAGS = -Bstatic -Map $(MAP) --strip-debug --fix-v4bx 
# Doesnt work, -lc has to be at the end
 #LDFLAGS = -Bstatic -Map $(MAP) --strip-debug --fix-v4bx -lc
 #CLDFLAGS = -Wl,-Map,$(MAP) -Wl,-verbose
 CLDFLAGS = -Wl,-Bstatic -Wl,-Map,$(MAP) -Wl,--strip-debug -Wl,--fix-v4bx 

ifdef USE_MINI_LIBC
debug:  mini-libc $(ELF) $(MMP32) $(MMP128) $(DIS)
else
debug:  $(MMP) $(DIS)
endif

$(MMP32): $(MEM)
	$(BMF32) $(MEM) $(MMP32)

$(MMP128): $(MEM)
	$(BMF128) $(MEM) $(MMP128)

$(MEM): $(TGT)
	$(ELF) $(TGT) > $(MEM)

$(TGT): $(OBJ)
ifdef CREATE_FLT_OUTPUT
	$(LD) $(LDFLAGS) -elf2flt=-v -elf2flt=-k -o $(FLT) $(TLDS) $(OBJ)
endif
	#$(LD) $(LDFLAGS) -o $(TGT) $(TLDS) $(OBJ)
	$(CC) -nostartfiles  $(CLDFLAGS) -o $(TGT) $(TLDS) $(OBJ)
	#$(LD) $(LDFLAGS) -o $(TGT) $(TLDS) $(OBJ) -lc -lm -lgcc
	$(OC) -R .comment -R .note $(TGT)

$(OBJ): $(DEP)

mini-libc:
	$(MAKE) -s -C ../mini-libc MIN_SIZE=1

$(ELF):
	$(MAKE) -s -C $(TOOLSPATH)
        
$(DIS): $(TGT)
	$(DS) $(DSFLAGS) $^ > $@

clean:
	@rm -rfv *.o *.elf *.flt *.gdb *.dis *.map *.mem *.v $(MMP32) $(MMP128)

