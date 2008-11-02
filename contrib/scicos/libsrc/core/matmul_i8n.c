/* ###*B*###
 * Copyright (C) Roberto Bucher (SUPSI- Lugano)
 *               Simone Mannori (Scilab / INRIA / DIGITEO)
 *
 * Copyright (C) METALAU Project (INRIA)
 * ###*E*### */
 
 
# include "scicos_block4.h"
# include "machine.h"
# include <math.h>

void matmul_i8n(scicos_block *block,int flag)
{
 if ((flag==1)|(flag==6)) {
  char *u1,*u2,*y; 
  double C,D;
  int mu1,nu1,nu2,i,j,l,ji,jl=0,il;  // =0 to avoid compiler warning
  int *ipar;

  mu1=GetInPortRows(block,1);
  nu1=GetInPortCols(block,1);
  nu2=GetInPortCols(block,2);
  u1=Getint8InPortPtrs(block,1);
  u2=Getint8InPortPtrs(block,2);
  y=Getint8OutPortPtrs(block,1);
  ipar=GetIparPtrs(block);

        for (l=0;l<nu2;l++)
	    {for(j=0;j<mu1;j++)
	        {D=0;
	        for(i=0;i<nu1;i++)
		   {ji=j+i*mu1;
		    jl=j+l*mu1;
		    il=i+l*nu1;
		    C=(double)(u1[ji])*(double)(u2[il]);
		    D=D + C;}
		    y[jl]=(char)D;
		  }
	     }
     }
}
