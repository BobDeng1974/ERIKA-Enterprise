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
 * Author: 2005 Antonio Romano
 * CVS: $Id: ee_board.h,v 1.1 2007/06/22 13:47:22 romano Exp $
 */

#ifndef __INCLUDE_MIB510_BOARD_H__
#define __INCLUDE_MIB510_BOARD_H__

#include "mcu/atmel_atmega128/inc/ee_mcu.h"



/*************************************************************************
 LEDs
 *************************************************************************/


#ifdef __LEDS_USED__

/* On MIB510 there are 3 leds connected through IO Port A */

__INLINE__ void EE_led_3_on(void) {
led_3_on();
}
__INLINE__ void EE_led_2_on(void) {
led_2_on();
}
__INLINE__ void EE_led_1_on(void) {
led_1_on();
}

__INLINE__ void EE_led_3_off(void) {
led_3_off();
}
__INLINE__ void EE_led_2_off(void) {
led_2_off();
}
__INLINE__ void EE_led_1_off(void) {
led_1_off();
}
#endif /* __LEDS_USED__ */

#endif
