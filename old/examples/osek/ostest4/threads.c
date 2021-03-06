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
 * Author: 2002 Paolo Gai
 * CVS: $Id: threads.c,v 1.1.1.1 2004/11/05 16:03:03 pj Exp $
 */

#include "ee.h"

/* the events */
DeclareEvent(myevent);

void pause_and_display(int disp);

TASK(thread0)
{
#ifdef __LEDS_USED__
  EE_led_1_on();
#endif

  pause_and_display(0x0);

  SetEvent(thread3, myevent);
  // preemption!

  pause_and_display(0x0);
  ActivateTask(thread2);

  pause_and_display(0x0);

#ifdef __LEDS_USED__
  EE_led_1_off();
#endif

  TerminateTask();
}

volatile int myerror=0;
TASK(thread1)
{
  EventMaskType m;

#ifdef __LEDS_USED__
  EE_led_2_on();
#endif

  pause_and_display(0x1);

  WaitEvent(myevent);
  //block

  pause_and_display(0x1);

  GetEvent(thread1, &m);

  if (m & myevent) {
    ClearEvent(myevent);
  }

  GetEvent(thread1, &m);

  if (m) {
    myerror++;
  }

  WaitEvent(myevent);

#ifdef __LEDS_USED__
  EE_led_2_off();
#endif

  TerminateTask();
}

TASK(thread2)
{
  EventMaskType m;

#ifdef __LEDS_USED__
  EE_led_3_on();
#endif

  pause_and_display(0x2);

  SetEvent(thread1, myevent);
  
  GetEvent(thread1, &m);
  if (m != myevent) {
    myerror++;
  }

#ifdef __LEDS_USED__
  EE_led_3_off();
#endif

  TerminateTask();
}

TASK(thread3)
{
  EventMaskType m;

#ifdef __LEDS_USED__
  EE_led_4_on();
#endif

  pause_and_display(0x3);

  WaitEvent(myevent);
  //block

  pause_and_display(0x3);

  GetEvent(thread3, &m);

  if (m & myevent) {
    ClearEvent(myevent);
  }

  GetEvent(thread3, &m);

  if (m) {
    myerror++;
  }

  WaitEvent(myevent);

#ifdef __LEDS_USED__
  EE_led_4_off();
#endif

  TerminateTask();
}
