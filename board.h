#pragma once

#include <pins.h>

////
//
//      6502DUE board pins mapped on SAM3X8E
//
////

typedef inout_t<pin_t<PC, 5> > D0;
typedef inout_t<pin_t<PC, 4> > D1;
typedef inout_t<pin_t<PC, 3> > D2;
typedef inout_t<pin_t<PC, 2> > D3;
typedef inout_t<pin_t<PC, 1> > D4;
typedef inout_t<pin_t<PD, 10> > D5;
typedef inout_t<pin_t<PA, 7> > D6;
typedef inout_t<pin_t<PD, 9> > D7;

typedef input_t<pin_t<PA, 16> > A0;
typedef input_t<pin_t<PA, 24> > A1;
typedef input_t<pin_t<PA, 23> > A2;
typedef input_t<pin_t<PA, 22> > A3;
typedef input_t<pin_t<PA, 6> > A4;
typedef input_t<pin_t<PA, 4> > A5;
typedef input_t<pin_t<PA, 3> > A6;
typedef input_t<pin_t<PA, 2> > A7;
typedef input_t<pin_t<PB, 17> > A8;
typedef input_t<pin_t<PB, 18> > A9;
typedef input_t<pin_t<PB, 19> > A10;
typedef input_t<pin_t<PB, 20> > A11;
typedef input_t<pin_t<PB, 15> > A12;
typedef input_t<pin_t<PB, 16> > A13;
typedef input_t<pin_t<PA, 1> > A14;
typedef input_t<pin_t<PA, 0> > A15;

typedef output_t<pin_t<PB, 26> > BE;
typedef input_t<pin_t<PA, 14> > MLB;
typedef input_t<pin_t<PA, 15> > RWB;
typedef input_t<pin_t<PD, 0> > RDY;    // use pullup
typedef input_t<pin_t<PD, 1> > SYNC;
typedef output_t<pin_t<PD, 2> > SOB;
typedef output_t<pin_t<PC, 29> > PHI2;
typedef input_t<pin_t<PD, 7> > PHI1O;
typedef input_t<pin_t<PD, 8> > PHI2O;
typedef output_t<pin_t<PC, 14> > RESB;
typedef output_t<pin_t<PC, 15> > IRQB;
typedef output_t<pin_t<PC, 16> > NMIB;
typedef input_t<pin_t<PC, 17> > VBP;

typedef output_t<pin_t<PB, 27> > LED0;
typedef output_t<pin_t<PA, 21> > LED1;
typedef output_t<pin_t<PC, 30> > LED2;

