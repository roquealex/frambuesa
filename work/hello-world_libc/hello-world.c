/*----------------------------------------------------------------
//                                                              //
//  hello-world.c                                               //
//                                                              //
//  This file is part of the Amber project                      //
//  http://www.opencores.org/project,amber                      //
//                                                              //
//  Description                                                 //
//  Simple stand-alone example application.                     //
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



/* Note that the stdio.h referred to here is the one in
   mini-libc. This applications compiles in mini-libc
   so it can run stand-alone.
*/   
//#include "stdio.h"
#include <stdio.h>
//#include <stdlib.h>


/*
#ifdef MINIMIPS
	#define USER_END '\r'
#else
	#define USER_END '\n'
#endif

*/
#define USER_END '\n'


void quicksort(int *a, int inicio, int fin);
int particion(int *a, int inicio, int fin);
void getString(char *str, unsigned int size);

char str_buffer[12];
int _write(int file, char *ptr, int len);
//void kernel_main(unsigned int r0, unsigned int r1, unsigned int atags)

char str_hello[] = "Hi Hi!\n";

void main () 
{
	//int num;
	int i;
	int a[10];
	// Excelent line to observe uart in osciloscope:
	//while(1) _outbyte('U');
	_outbyte('H');
	_outbyte('o');
	_outbyte('l');
	_outbyte('a');
	_outbyte('!');
	_write(0,str_hello,6);
	//_trap();
	////trap: e3a0f671
	//// 8d30
	// takes 89d0
	// dies 89c0+
	// never goes to 8970
	// doesnt reach the branch in 8ae0
	// reaches 89c0 via a branch, previous instruction is not executed
	// Executes    8960:       eaffffd6        b       88c0 <__sfvwrite_r+0x60>
	// 8a3c not taken
	// Line 8954 is reached
	// 8b04 is reache
	// Copies correctly string to dest 1322c
	// r1 1321f
	// r10 1
	// r3 d
	// r3 13294
	// Most likely this is not implemented in the processor:
	//     8944:       e1d430bc        ldrh    r3, [r4, #12]
	//     it doesnt behave correctly, r3 receives 13294 which is the contents of r4

    printf ("Hello, World!\n");
    printf ("Compiled from dell\n");
    /*

	for(i=0;i<(sizeof(a))/4;i++){
		printf ("n%d?\n",i);
		getString(str_buffer,sizeof(str_buffer));
		a[i] = atoi(str_buffer);
		//putchar('\n');

	}

    //printf ("number is %d\n",num);
    //_testpass();
    //
    //
	quicksort(a,0,(sizeof(a)/4)-1);

	for(i=0;i<(sizeof(a))/4;i++){
		//printNumber(a[i]);
		printf ("s%d: %d\n",i,a[i]);
	}
	*/
	printf ("               \n");
	_outbyte('e');
	_outbyte('n');
	_outbyte('d');
	//return 0;

}


void getString(char *str, unsigned int size){
	unsigned int i;
	char d;
	i=0;
	    //printf ("Calling getString, size %d\n",size);
	while( ((d=getchar())!=USER_END) && (i<(size-1)) ){
		str[i] = d;
		//putchar(d);
	    //printf ("Char %c, i %d\n",d,i);
		i++;
	}
	str[i] = '\0';
}


void quicksort(int *a, int inicio, int fin){
	if(fin>inicio){
		int pivote;
		pivote = particion(a,inicio,fin);
		quicksort(a,inicio,pivote-1);
		quicksort(a,pivote+1,fin);
	}
}


/*
 * a es el arreglo a convertir
 * inicio es el indice inicial
 * fin es el indice final
 * por simplicidad el pivote es el primero
 */
int particion(int *a, int inicio, int fin){
	int izquierda, derecha;
	izquierda = inicio+1;
	derecha = fin;
	while(1){
	//while(derecha>izquierda){
		while(a[izquierda]<=a[inicio]){
			if(izquierda<fin){
				izquierda++;
			}else{
				int tm = a[fin];
				a[fin] = a[inicio];
				a[inicio] = tm;
				return fin;
			}
		}
		while(a[derecha]>a[inicio]){
			derecha--;
		}
		if(derecha<izquierda){
			//fin de algoritmo
			int tm = a[derecha];
			a[derecha] = a[inicio];
			a[inicio] = tm;
			return derecha;
		}else{
			int tm = a[derecha];
			a[derecha] = a[izquierda];
			a[izquierda] = tm;
		}
	}
}

