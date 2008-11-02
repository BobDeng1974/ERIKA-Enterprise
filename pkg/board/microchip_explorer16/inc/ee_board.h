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

/* 
 * Author: 2006 Mauro Marinoni
 * CVS: $Id: ee_board.h,v 1.19 2007/03/16 10:17:36 nino Exp $
 */

#ifndef __INCLUDE_MICROCHIP_EXPLORER16_BOARD_H__
#define __INCLUDE_MICROCHIP_EXPLORER16_BOARD_H__

#include "mcu/microchip_dspic/inc/ee_mcu.h"

/* /\************************************************************************* */
/*  Buttons */
/*  *************************************************************************\/ */

#ifdef __USE_BUTTONS__

extern void (*EE_button_callback)(void);
extern EE_UINT8 EE_button_mask;

__INLINE__ void __ALWAYS_INLINE__ EE_buttons_init( void(*isr_callback)(void), EE_UINT8 mask ) {
	/* set BUTTON pins (S3/RD6-S4/RD13-S5/RA7-S6/RD7) as inputs */
	TRISDbits.TRISD6  = 1; 
	TRISDbits.TRISD13 = 1; 
	TRISAbits.TRISA7  = 1; 
	TRISDbits.TRISD7  = 1; 
	/* ser AN23 as digital output */
 	AD1PCFGHbits.PCFG23 = 1;

	/* Enable Interrupt */
	if (isr_callback != NULL) {
		if (mask & 0x01)
			CNEN1bits.CN15IE = 1;	// S3/RD6
		if (mask & 0x02)
			CNEN2bits.CN19IE = 1;	// S4/RD13
		if (mask & 0x04)
			CNEN2bits.CN23IE = 1;	// S5/RA7
		if (mask & 0x08)
			CNEN2bits.CN16IE = 1;	// S6/RD7
		
		IFS1bits.CNIF = 0;
		IEC1bits.CNIE = 1;
	}
	
	/* Save callback */
	EE_button_callback = isr_callback;	
} 

__INLINE__ EE_UINT8 __ALWAYS_INLINE__ EE_button_get_S3( void ) {
	if (PORTDbits.RD6)
		return 0;
	else
		return 1;
}
__INLINE__ EE_UINT8 __ALWAYS_INLINE__ EE_button_get_S4( void ) {
	if (PORTDbits.RD13)
		return 0;
	else
		return 1;
}
__INLINE__ EE_UINT8 __ALWAYS_INLINE__ EE_button_get_S5( void ) {
	if (PORTAbits.RA7)
		return 0;
	else
		return 1;
}
__INLINE__ EE_UINT8 __ALWAYS_INLINE__ EE_button_get_S6( void ) {
	if (PORTDbits.RD7)
		return 0;
	else
		return 1;
}

#endif


/* /\************************************************************************* */
/*  LEDs */
/*  *************************************************************************\/ */

#ifdef __USE_LEDS__

__INLINE__ void __ALWAYS_INLINE__ EE_leds_init(void) {
	/* set LEDs (D3-D10/RA0-RA7) drive state low */
	LATA &= 0xFF00; 
	/* set LED pins (D3-D10/RA0-RA7) as outputs */
	TRISA &= 0xFF00; 
}
__INLINE__ void __ALWAYS_INLINE__ EE_leds_on(void)   { LATA |= 0x00FF; }
__INLINE__ void __ALWAYS_INLINE__ EE_leds_off(void)  { LATA &= 0xFF00; }

__INLINE__ void __ALWAYS_INLINE__ EE_leds( EE_UINT8 data )  { LATA &= 0xFF00; LATA |= data; }

__INLINE__ void __ALWAYS_INLINE__ EE_led_on(void)   { LATAbits.LATA0 = 1; }
__INLINE__ void __ALWAYS_INLINE__ EE_led_off(void)  { LATAbits.LATA0 = 0; }

__INLINE__ void __ALWAYS_INLINE__ EE_led_3_on(void)   { LATAbits.LATA0 = 1; }
__INLINE__ void __ALWAYS_INLINE__ EE_led_3_off(void)  { LATAbits.LATA0 = 0; }
__INLINE__ void __ALWAYS_INLINE__ EE_led_4_on(void)   { LATAbits.LATA1 = 1; }
__INLINE__ void __ALWAYS_INLINE__ EE_led_4_off(void)  { LATAbits.LATA1 = 0; }
__INLINE__ void __ALWAYS_INLINE__ EE_led_5_on(void)   { LATAbits.LATA2 = 1; }
__INLINE__ void __ALWAYS_INLINE__ EE_led_5_off(void)  { LATAbits.LATA2 = 0; }
__INLINE__ void __ALWAYS_INLINE__ EE_led_6_on(void)   { LATAbits.LATA3 = 1; }
__INLINE__ void __ALWAYS_INLINE__ EE_led_6_off(void)  { LATAbits.LATA3 = 0; }
__INLINE__ void __ALWAYS_INLINE__ EE_led_7_on(void)   { LATAbits.LATA4 = 1; }
__INLINE__ void __ALWAYS_INLINE__ EE_led_7_off(void)  { LATAbits.LATA4 = 0; }
__INLINE__ void __ALWAYS_INLINE__ EE_led_8_on(void)   { LATAbits.LATA5 = 1; }
__INLINE__ void __ALWAYS_INLINE__ EE_led_8_off(void)  { LATAbits.LATA5 = 0; }
__INLINE__ void __ALWAYS_INLINE__ EE_led_9_on(void)   { LATAbits.LATA6 = 1; }
__INLINE__ void __ALWAYS_INLINE__ EE_led_9_off(void)  { LATAbits.LATA6 = 0; }
__INLINE__ void __ALWAYS_INLINE__ EE_led_10_on(void)  { LATAbits.LATA7 = 1; }
__INLINE__ void __ALWAYS_INLINE__ EE_led_10_off(void) { LATAbits.LATA7 = 0; }

#endif


/* /\************************************************************************* */
/*  LCD */
/*  *************************************************************************\/ */

#ifdef __USE_LCD__
/* 
   For Explorer 16 board, here are the data and control signal definitions
   RS -> RB15
   E  -> RD4
   RW -> RD5
   DATA -> RE0 - RE7   
*/

/* /\* Control signal data pins *\/ */
#define  EE_LCD_RW  LATDbits.LATD5       // LCD R/W signal
#define  EE_LCD_RS  LATBbits.LATB15      // LCD RS signal
#define  EE_LCD_E   LATDbits.LATD4       // LCD E signal 

/* /\* Control signal pin direction *\/ */
#define  EE_LCD_RW_TRIS		TRISDbits.TRISD5 
#define  EE_LCD_RS_TRIS		TRISBbits.TRISB15
#define  EE_LCD_E_TRIS		TRISDbits.TRISD4

/* /\* Data signals and pin direction *\/ */
#define  EE_LCD_DATA      LATE           // Port for LCD data
#define  EE_LCD_DATAPORT  PORTE
#define  EE_LCD_TRISDATA  TRISE          // I/O setup for data Port

/* /\* Send an impulse on the enable line.  *\/ */
__INLINE__ void __ALWAYS_INLINE__ EE_lcd_pulse_enable( void )
{
	EE_LCD_E = 1;
	Nop();
	Nop();
	Nop();
	EE_LCD_E = 0;
}

/* /\* Send a command to the lcd.  *\/ */
__INLINE__ void __ALWAYS_INLINE__ EE_lcd_command( EE_UINT8 cmd )
{
	EE_LCD_DATA &= 0xFF00;
	EE_LCD_DATA |= cmd;
	EE_LCD_RW = 0;
	EE_LCD_RS = 0;
	EE_lcd_pulse_enable();
	//Delay_Us( Delay200uS_count );
}

/* /\* Initialize the display.  *\/ */
__INLINE__ void __ALWAYS_INLINE__ EE_lcd_init(void) {
	// 15mS delay after Vdd reaches nnVdc before proceeding with LCD initialization
	// not always required and is based on system Vdd rise rate
	// Todo!!!
	//Delay(Delay_15mS_Cnt);
			
	/* Initial values */
	EE_LCD_DATA &= 0xFF00;	
	EE_LCD_RW   = 0;
	EE_LCD_RS   = 0;
	EE_LCD_E    = 0;

	/* Set pins direction */
	EE_LCD_TRISDATA &= 0xFF00;
 	EE_LCD_RW_TRIS  = 0;
	EE_LCD_RS_TRIS  = 0;
	EE_LCD_E_TRIS   = 0;

	/* Init - Step 1 */
	EE_LCD_DATA &= 0xFF00;
	EE_LCD_DATA |= 0x0038;
	EE_lcd_pulse_enable();
   	//Delay(Delay_5mS_Cnt);
      
	/* Init - Step 2 */
	EE_LCD_DATA &= 0xFF00;
	EE_LCD_DATA |= 0x0038;
	EE_lcd_pulse_enable();
	//Delay_Us( Delay200uS_count );

	/* Init - Step 2 */
	EE_LCD_DATA &= 0xFF00;
	EE_LCD_DATA |= 0x0038;
	EE_lcd_pulse_enable();
	//Delay_Us( Delay200uS_count );

	EE_lcd_command( 0x38 );	// Function set
	EE_lcd_command( 0x0C );	// Display on/off control, cursor blink off (0x0C)
	EE_lcd_command( 0x06 );	// Entry mode set (0x06)
}

/* /\* Send a data.  *\/ */
__INLINE__ void __ALWAYS_INLINE__ EE_lcd_putc( EE_INT8 data )
{
	EE_LCD_RW = 0;
	EE_LCD_RS = 1;
	EE_LCD_DATA &= 0xFF00;
	EE_LCD_DATA |= data;
	EE_lcd_pulse_enable();
	EE_LCD_RS = 0;
	//Delay_Us( Delay200uS_count );
}

/* /\* Send a data.  *\/ */
__INLINE__ EE_INT8 __ALWAYS_INLINE__ EE_lcd_getc( void )
{
	EE_INT8 buf;
	
	EE_LCD_TRISDATA |= 0x00FF;
	EE_LCD_RW = 1;
	EE_LCD_RS = 1;
	EE_lcd_pulse_enable();
	//Delay_Us( Delay200uS_count );
	buf = EE_LCD_DATAPORT & 0x00FF;
	EE_LCD_RS = 0;
	EE_LCD_TRISDATA &= 0xFF00;
	return ( buf );
}

/* /\* Send a string to the display.  *\/ */
__INLINE__ void __ALWAYS_INLINE__ EE_lcd_puts( EE_INT8 *buf )
{
	EE_UINT8 i = 0;
	
	while (buf[i] != '\0')
		EE_lcd_putc(buf[i++]);
}

/* /\* Check if the display is busy.  *\/ */
__INLINE__ unsigned char __ALWAYS_INLINE__ EE_lcd_busy( void )
{
	EE_INT8 buf;
	
	EE_LCD_TRISDATA |= 0x00FF;
	EE_LCD_RW = 0;
	EE_LCD_RS = 1;
	EE_lcd_pulse_enable();
	//Delay_Us( Delay200uS_count );
	buf = EE_LCD_DATAPORT & 0x00FF;
	EE_LCD_RS = 0;
	EE_LCD_TRISDATA &= 0xFF00;
	return ( (buf & 0x80) ? 1 : 0 );
}

__INLINE__ void __ALWAYS_INLINE__ EE_lcd_clear (void)		{ EE_lcd_command( 0x01 ); }

__INLINE__ void __ALWAYS_INLINE__ EE_lcd_home  (void)		{ EE_lcd_command( 0x02 ); }
__INLINE__ void __ALWAYS_INLINE__ EE_lcd_line2 (void)		{ EE_lcd_command( 0xC0 ); }

__INLINE__ void __ALWAYS_INLINE__ EE_lcd_curs_right (void)	{ EE_lcd_command( 0x14 ); }
__INLINE__ void __ALWAYS_INLINE__ EE_lcd_curs_left (void)	{ EE_lcd_command( 0x10 ); }
__INLINE__ void __ALWAYS_INLINE__ EE_lcd_shift (void)		{ EE_lcd_command( 0x1C ); }

__INLINE__ void __ALWAYS_INLINE__ EE_lcd_goto (EE_UINT8 posx, EE_UINT8 posy)
{
	EE_UINT8 tmp_pos;
	
	tmp_pos  = posy ? 0xC0 : 0x80;
	tmp_pos += 0x0F & posx;
	EE_lcd_command( tmp_pos ); 
}

#endif


/* /\************************************************************************* */
/*  Analog input */
/*  *************************************************************************\/ */

#ifdef __USE_ANALOG__

#define AVDD 3300

extern EE_UINT16 EE_analog_raw_temperature;
extern EE_UINT16 EE_analog_raw_potentiometer;

__INLINE__ void __ALWAYS_INLINE__ EE_analog_init( void ) {
	
	/* Reset local variables */
	EE_analog_raw_temperature = 0;
	EE_analog_raw_potentiometer = 0;
	
	/* set configuration bits as ADC input */ 		
 	AD1PCFGLbits.PCFG4 = 0;         // Temp Sensor  -> AN4/RB4
 	AD1PCFGLbits.PCFG5 = 0;         // Potentiometer -> AN5/RB5
 
	/* Set control register 1 */
	/* 12-bit, unsigned integer format, autoconvert, autosampling */
	AD1CON1 = 0x04E4;
	
	/* Set control register 2 */
	/* Vref = AVcc/AVdd, Scan Inputs */
	AD1CON2 = 0x0000;
	
	/* Set Samples and bit conversion time */
	/* AS = 16 Tad, Tad = 32 Tcy */
	AD1CON3 = 0x1020;
        	
	/* set channel scanning here for AN4 and AN5 */
	AD1CSSL = 0x0000;
	
	/* channel select AN4 and AN5 */
	AD1CHS0 = 0x0004;
	
	/* reset ADC interrupt flag */
	IFS0bits.AD1IF = 0;           

	/* enable ADC interrupts, disable this interrupt if the DMA is enabled */	  
	IEC0bits.AD1IE = 1;       

	/* turn on ADC module */
	AD1CON1bits.ADON = 1;          	
} 

__INLINE__ EE_UINT16 __ALWAYS_INLINE__ EE_analog_get_volt( void ) { return ((long)EE_analog_raw_potentiometer * AVDD) >> 12;  }
__INLINE__ EE_UINT16 __ALWAYS_INLINE__ EE_analog_get_temp( void ) { return EE_analog_raw_temperature >> 2; }

__INLINE__ void __ALWAYS_INLINE__ EE_analog_start( void ) { IEC0bits.AD1IE = 1; AD1CON1bits.ADON = 1; }
__INLINE__ void __ALWAYS_INLINE__ EE_analog_stop ( void ) { IEC0bits.AD1IE = 0; AD1CON1bits.ADON = 0; }

#endif



/* ************************************************************************* */

#endif
