/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2008  Evidence Srl
 *
 * This file is part of ERIKA Enterprise.
 *
 * ERIKA Enterprise is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation, 
 * (with a special exception described below).
 *
 * Linking this code statically or dynamically with other modules is
 * making a combined work based on this code.  Thus, the terms and
 * conditions of the GNU General Public License cover the whole
 * combination.
 *
 * As a special exception, the copyright holders of this library give you
 * permission to link this code with independent modules to produce an
 * executable, regardless of the license terms of these independent
 * modules, and to copy and distribute the resulting executable under
 * terms of your choice, provided that you also meet, for each linked
 * independent module, the terms and conditions of the license of that
 * module.  An independent module is a module which is not derived from
 * or based on this library.  If you modify this code, you may extend
 * this exception to your version of the code, but you are not
 * obligated to do so.  If you do not wish to do so, delete this
 * exception statement from your version.
 *
 * ERIKA Enterprise is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License version 2 for more details.
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with ERIKA Enterprise; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 * ###*E*### */

#include "ee.h"
#include "cpu/pic30/inc/ee_irqstub.h"

#include "eeuart.h"

// *****************************************************
// IMPORTANT: please check which board you are using
// and select the proper define below
// the Demo board uses UART2
// the Multibus board may use both, please check what is connected
// the Motion board / Demo2 board uses UART1
// *****************************************************

// motion board, multibus board
#define USE_UART1
// demo board, multibus board
//#define USE_UART2



// Primary (XT, HS, EC) Oscillator without PLL
_FOSCSEL(FNOSC_PRIPLL);
// OSC2 Pin Function: OSC2 is Clock Output - Primary Oscillator Mode: XT Crystanl
_FOSC(OSCIOFNC_ON & POSCMD_XT);
// Watchdog Timer Enabled/disabled by user software
_FWDT(FWDTEN_OFF);
// Disable Code Protection
_FGS(GCP_OFF);            							

/* Program the Timer1 peripheral to raise interrupts */
void T1_program(void)
{
	T1CON = 0;		/* Stops the Timer1 and reset control reg	*/
	TMR1  = 0;		/* Clear contents of the timer register	*/
	PR1   = 0x9c40;		/* Load the Period register wit the value 0xffff	*/
	IPC0bits.T1IP = 5;	/* Set Timer1 priority to 1		*/
	IFS0bits.T1IF = 0;	/* Clear the Timer1 interrupt status flag	*/
	IEC0bits.T1IE = 1;	/* Enable Timer1 interrupts		*/
	T1CONbits.TON = 1;	/* Start Timer1 with prescaler settings at 1:1
				* and clock source set to the internal 
				* instruction cycle			*/
}

/* Clear the Timer1 interrupt status flag */
void T1_clear(void)
{
	IFS0bits.T1IF = 0;
}

/* This is an ISR Type 2 which is attached to the Timer 1 peripheral IRQ pin
 * The ISR simply calls CounterTick to implement the timing reference
 */
ISR2(_T1Interrupt)
{
	/* clear the interrupt source */
	T1_clear();

	/* count the interrupts, waking up expired alarms */
	CounterTick(myCounter);
}

unsigned char RXBuff = 0;


/* Send a character using the UART port
 */
TASK(TaskSend)
{
	//LATBbits.LATB14 ^= 1;

#ifdef USE_UART1
	EE_UART1_Send('<');
	EE_UART1_Send('H');
	EE_UART1_Send('>');
#endif

#ifdef USE_UART2
	EE_UART2_Send('<');
	EE_UART2_Send('H');
	EE_UART2_Send('>');
#endif
}

/* Get a character from the UART buffer
 */
TASK(TaskReceive)
{
	LATBbits.LATB14 ^= 1;
	
#ifdef USE_UART1
	if (EE_UART1_Receive(&RXBuff) == 0) {
		EE_UART1_Send('<');
		EE_UART1_Send(RXBuff);
		EE_UART1_Send('>');
	}	
#endif

#ifdef USE_UART2
	if (EE_UART2_Receive(&RXBuff) == 0) {
		EE_UART2_Send('<');
		EE_UART2_Send(RXBuff);
		EE_UART2_Send('>');
	}	
#endif
}

int main(void)
{
	/* Clock setup for 40MIPS */
	CLKDIVbits.DOZEN   = 0;
	CLKDIVbits.PLLPRE  = 0;
	CLKDIVbits.PLLPOST = 0;
	PLLFBDbits.PLLDIV  = 78;
	
	/* Wait for PLL to lock */
	while(OSCCONbits.LOCK!=1);
	
	/* Program Timer 1 to raise interrupts */
	T1_program();
  
	/* Init led */
	EE_leds_init();

	/* UART Init */
#ifdef USE_UART1
	EE_UART1_Init(115200, BIT8_NO | BIT_STOP_1, CTRL_SIMPLE);
#endif

#ifdef USE_UART2
	EE_UART2_Init(115200, BIT8_NO | BIT_STOP_1, CTRL_SIMPLE);
#endif

	/* Program a cyclic alarm which will fire after an offset of 10 counter 
	* ticks, and after that periodically every 500 ticks */
	SetRelAlarm(AlarmSend, 500, 500);
	SetRelAlarm(AlarmReceive, 500, 500);
	
	/* Forever loop: background activities (if any) should go here */
	for (;;);
	
	return 0;
}
