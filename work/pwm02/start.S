/*----------------------------------------------------------------
//                                                              //
//  start.S                                                     //
//                                                              //
//  This file is part of the Amber project                      //
//  http://www.opencores.org/project,amber                      //
//                                                              //
//  Description                                                 //
//  Assembly routines for hello-world.                          //
//  As hello-world is a stand-alone application, it needs a     //
//  simple start function written in assembly to call the       //
//  C code main() function.                                     //
//                                                              //
//  Author(s):                                                  //
//      - Conor Santifort, csantifort.amber@gmail.com           //
//                                                              //
//////////////////////////////////////////////////////////////////
//                                                              //
// Copyright (C) 2010 Authors and OPENCORES.ORG                 //
//                                                              //
// This source file may be used and distributed without         //
// restriction provided that this copyright statement is not    //
// removed from the file and that any derivative work contains  //
// the original copyright notice and the associated disclaimer. //
//                                                              //
// This source file is free software; you can redistribute it   //
// and/or modify it under the terms of the GNU Lesser General   //
// Public License as published by the Free Software Foundation; //
// either version 2.1 of the License, or (at your option) any   //
// later version.                                               //
//                                                              //
// This source is distributed in the hope that it will be       //
// useful, but WITHOUT ANY WARRANTY; without even the implied   //
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      //
// PURPOSE.  See the GNU Lesser General Public License for more //
// details.                                                     //
//                                                              //
// You should have received a copy of the GNU Lesser General    //
// Public License along with this source; if not, download it   //
// from http://www.opencores.org/lgpl.shtml                     //
//                                                              //
----------------------------------------------------------------*/
	//.section .text
	//.section ".text.startup"
	// funtion main seem to be the first thing if it is declared in the start. If we use init _start will be before main
	.section .init
	// also works with .text.exit, looks like main is declared as the first thing in .text.startup
	//.section ".text.exit"
	.globl	_start 
_start:               
        /* Switch to User Mode */
        /* and unset interrupt mask bits */
        #mov     r0, #0x00000000
        #teqp    pc, r0 
 
        @ Enable the cache
        #mov     r0,  #0xffffffff
	#mcr	15, 0, r0, cr3, cr0, 0   @ cacheable area
        #mov     r0,  #1
	#mcr	15, 0, r0, cr2, cr0, 0   @ cache enable
        
        @ initialize the stack pointer
        ldr     sp, AdrStack
	sub	sp, sp, #4

	# Move _trap function to 0x07100000
/*
	ldr	r0, =_trap
	mov	r1, #0x07100000
	mov	r2, #4
_trap_copy:
	ldr	r3, [r0], #4
	str	r3, [r1], #4
	subs	r2, #1
	bne	_trap_copy
*/
        @ jump to main
        #.extern main
        #bl      main
        .extern _cstartup
        bl      _cstartup
	
	mov	pc, #0
                
inf_loop:
        b       inf_loop
        
	.global _get_stack_pointer
_get_stack_pointer:
	// Ret stack pointer val
	str	sp, [sp]
	ldr	r0, [sp]
	mov	pc, lr

	.globl _trap      
_trap:
	mov	lr, #0
        ldr     pc, adr_core_status
adr_core_status:      .word  0x00000094
	mov	pc, #0x7100000

/* _outbyte: Output a single character through UART 0 */
        @ if the uart tx fifo is stuck full
        @ this routine will cycle forever
	.globl _outbyte      
_outbyte:        
        ldr     r1, AdrUARTDR
        ldr     r3, AdrUARTFR
        @ Check the tx_full flag
1:      ldr     r2, [r3]
        and     r2, r2, #0x20
        cmp     r2, #0
        streqb  r0, [r1]
	@ This is a bug in the code: Dont move to pc using s if it is not int
        #moveqs  pc, lr          @ return
        moveq  pc, lr          @ return
        bne     1b


/* _inbyte: Input a single character from UART 0 */
        @ r0 is the timeout in mS
	.globl _inbyte      
_inbyte:        
        ldr     r2, AdrUARTDR @ data
        ldr     r3, AdrUARTFR @ flags
        
        @ Multiple delay value by 2560
        @ as the delay loop takes about 12 clock cycles running cached
        @ so that factor gives 1:1mS @33MHz
        mov     r1, r0, lsl #11
        add     r1, r1, r0, lsl #9
        
        @ Check the r2 empty flag
2:      ldr     r0, [r3]
        ands    r0, r0, #0x10
        ldreqb  r0, [r2]
        moveq   pc, lr
        
        @ decrement timeout
        subs    r1, r1, #1
        bne     2b
        
        mov     r0, #-1
	@ bug:
        #movs    pc, lr
        mov     pc, lr


/* stack at top of ddr3 memory space */
//AdrStack: .word  0x8000000
AdrStack: .word  0x7000000
// This was amber:
//AdrUARTDR:      .word  0x16000000
//AdrUARTFR:      .word  0x16000018
AdrUARTDR:      .word 0x20201000
AdrUARTFR:      .word  0x20201018


        .section .bss
        .word  0x0    
/* ========================================================================= */
/* ========================================================================= */
        

