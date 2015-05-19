#ifndef FIR_H
#define FIR_H

#include <stdint.h>

//#define FIR_ORDER 64
#define FIR_ORDER 32
const int16_t b_low_pass_44k1[FIR_ORDER] = {
246 ,
291 ,
336 ,
383 ,
430 ,
475 ,
520 ,
562 ,
601 ,
637 ,
668 ,
694 ,
715 ,
731 ,
740 ,
743 ,
740 ,
731 ,
715 ,
694 ,
668 ,
637 ,
601 ,
562 ,
520 ,
475 ,
430 ,
383 ,
336 ,
291 ,
246 ,
0
};




/*
const int16_t b_low_pass_44k1[FIR_ORDER] = {
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  -0x3fff, //31
  0,//32
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,//47
  0,// 48
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};
*/

//const uint16_t b_band_pass_44k1[FIR_ORDER];
//const uint16_t b_high_pass_44k1[FIR_ORDER];

#endif
