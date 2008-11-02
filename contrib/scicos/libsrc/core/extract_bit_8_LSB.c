/* ###*B*###
 * Copyright (C) Roberto Bucher (SUPSI- Lugano)
 *               Simone Mannori (Scilab / INRIA / DIGITEO)
 *
 * Copyright (C) METALAU Project (INRIA)
 * ###*E*### */
 
 
#include "scicos_block4.h"
#include <math.h>

void extract_bit_8_LSB(scicos_block *block,int flag)
{
   int i,maxim;
   char *y,*u,ref,n;
   int *ipar;
   y=Getint8OutPortPtrs(block,1);
   u=Getint8InPortPtrs(block,1);
   ipar=GetIparPtrs(block);
   maxim=8;
   ref=0;
   for(i=0;i<*ipar;i++)
       {n=(char)pow(2,i);
        ref=ref+n;}
   *y=(*u)&(ref);
}
