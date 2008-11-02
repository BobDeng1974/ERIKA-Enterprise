/* ###*B*###
 * Copyright (C) Roberto Bucher (SUPSI- Lugano)
 *               Simone Mannori (Scilab / INRIA / DIGITEO)
 *
 * Copyright (C) METALAU Project (INRIA)
 * ###*E*### */
 
 
#include "scicos_block.h"
#include <math.h>

void sinh_blk(scicos_block *block,int flag)
{
  int j;
  if(flag==1){
    for (j=0;j<block->insz[0];j++) {
      block->outptr[0][j]=sinh(block->inptr[0][j]);
    }
  }
}
