/* ###*B*###
 * Copyright (C) Roberto Bucher (SUPSI- Lugano)
 *               Simone Mannori (Scilab / INRIA / DIGITEO)
 *
 * Copyright (C) METALAU Project (INRIA)
 * ###*E*### */
 
 
//** 9 Feb 2008 : Revision notes by Simone Mannori 
//**
#include <machine.h>
#include <scicos_block4.h>

#include <ee.h>

/* Trimmer bit allocation: AN15-RB15 */

static void init(scicos_block *block)
{
	EE_trimmer_init();
}

static void inout(scicos_block *block)
{
	float adcdata;
	float * y = block->outptr[0];
	
	adcdata = EE_trimmer_get_volt();

	y[0] = adcdata;
}

static void end(scicos_block *block)
{
}

void flex_dmb_potin(scicos_block *block,int flag)
{
	switch (flag) {
		case 1:	/* set output */
			inout(block);
			break;

		case 2:	/* get input */
			inout(block);
			break;
		
		case 4:	/* initialisation */
			init(block);
			break;
		
		case 5:	/* ending */
			end(block);
			break;
	}
}
