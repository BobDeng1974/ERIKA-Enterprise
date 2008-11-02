/* ###*B*###
 * Copyright (C) Roberto Bucher (SUPSI- Lugano)
 *               Simone Mannori (Scilab / INRIA / DIGITEO)
 *
 * Copyright (C) METALAU Project (INRIA)
 * ###*E*### */
 
 
#include <math.h>
#include "scicos_block4.h"
#include <stdio.h>

void summation_i16s(scicos_block *block,int flag)
{
 if((flag==1)|(flag==6)) {
    int j,k;
    int nu,mu,nin;
    short *y;
    int *ipar;
    double v,l;
    double *rpar;
    short *u;

    y=Getint16OutPortPtrs(block,1);
    nu=GetInPortRows(block,1);
    mu=GetInPortCols(block,1);
    ipar=GetIparPtrs(block);
    rpar=GetRparPtrs(block);
    nin=GetNin(block);
    l=pow(2,16)/2;
    if (nin==1){
      v=0;
      u=Getint16InPortPtrs(block,1);
      for (j=0;j<nu*mu;j++) {
	v=v+(double)u[j];
      }
      if (v>=l)  v=l-1;
      else if (v<-l) v=-l;
      y[0]=(short)v; 
    }
    else {
      for (j=0;j<nu*mu;j++) {
        v=0;
	for (k=0;k<nin;k++) {
	  u=Getint16InPortPtrs(block,k+1);
          if(ipar[k]>0){
	     v=v+(double)u[j];
	    }
	  else{
	     v=v-(double)u[j];}
      }
      if (v>=l)  v=l-1;
      else if (v<-l) v=-l;
      y[j]=(short)v;
    }
   }
  }
}
