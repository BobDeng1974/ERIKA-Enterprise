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

/* LDR bit allocation: AN13-RB13 */

static void init(scicos_block *block)
{
	EE_analogsensors_init();
}

static void inout(scicos_block *block)
{
	float adcdata;
	float * y = block->outptr[0];
   
	//** please specify the units of measure. Usually the best option
	//** is to give back a 0-100% relative value in the range of the
	//** sensor. 
	adcdata = EE_analog_get_light();

	y[0] = adcdata;
}

static void end(scicos_block *block)
{
}

void flex_dmb_ldr(scicos_block *block,int flag)
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
