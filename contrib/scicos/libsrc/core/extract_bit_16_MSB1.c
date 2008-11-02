/* ###*B*###
 * Copyright (C) Roberto Bucher (SUPSI- Lugano)
 *               Simone Mannori (Scilab / INRIA / DIGITEO)
 *
 * Copyright (C) METALAU Project (INRIA)
 * ###*E*### */
 
 
#include "scicos_block4.h"
#include <math.h>

void extract_bit_16_MSB1(scicos_block *block,int flag)
{
   int i,maxim;
   short *y,*u,ref,n;
   int *ipar;
   y=Getint16OutPortPtrs(block,1);
   u=Getint16InPortPtrs(block,1);
   ipar=GetIparPtrs(block);
   maxim=16;
   ref=0;
   for(i=0;i<*ipar;i++)
       {n=(short)pow(2,maxim-1-i);
        ref=ref+n;}
   *y=(*u)&(ref);
   *y=*y>>(maxim-*ipar);
}
