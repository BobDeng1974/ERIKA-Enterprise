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
 * Author: 2002-2008 Paolo Gai
 */

#include "ee.h"
#include "cpu/cosmic_hs12xs/inc/ee_irqstub.h"

#include "myapp.h"

/* insert a stub for the functions not directly supported by __FP__ */
#ifdef __FP__
__INLINE__ void __ALWAYS_INLINE__ DisableAllInterrupts(void)
{
  EE_hal_disableIRQ();
}

__INLINE__ void __ALWAYS_INLINE__ EnableAllInterrupts(void)
{
  EE_hal_enableIRQ();
}

__INLINE__ void __ALWAYS_INLINE__ TerminateTask(void)
{
}

#define OSDEFAULTAPPMODE 1
__INLINE__ void __ALWAYS_INLINE__ StartOS(int i)
{
}
#endif

/* Let's declare the tasks identifiers */
DeclareTask(Task1);
DeclareTask(Task2);

volatile int timer_fired=0;
volatile int button_fired=0;
volatile int task1_fired=0;
volatile int task2_fired=0;
volatile int timer_divisor = 0;
volatile unsigned char led_status = 0;

/* just a dummy delay */ 
void mydelay(long int end)
{
  	long int i;
  	for (i=0; i<end; i++);
    
  	return;  
}

/* sets and resets a led configuration passed as parameter, leaving the other
 * bits unchanged
 * 
 * Note: led_blink is called both from Task1 and Task2. To avoid race 
 * conditions, we forced the atomicity of the led manipulation using IRQ
 * enabling/disabling. We did not use Resources in this case because the 
 * critical section is -really- small. An example of critical section using 
 * resources can be found in the osek_resource example.
 */
void led_blink(unsigned char theled)
{
  DisableAllInterrupts();
  led_status |= theled;
  EE_leds(led_status);
  EnableAllInterrupts();

  mydelay((long int)60000);

  DisableAllInterrupts();
  led_status &= ~theled;
  EE_leds(led_status);
  EnableAllInterrupts();
}

/* Task1: just call the ChristmasTree */
TASK(Task1)
{
  task1_fired++;
  
  /* First half of the christmas tree */
  led_blink(LED_0);
  led_blink(LED_1);
  
  /* CONFIGURATION 3 and 4: we put an additional Schedule() here! */
#ifdef MYSCHEDULE
  Schedule();
#endif

  /* Second half of the christmas tree */
  led_blink(LED_2);
  
  TerminateTask();
}

/* Task2: Print the counters on the JTAG UART */
TASK(Task2)
{
  static int which_led = 0;
  char *msg_tmr =  "ISR_Timer: ";
  char *msg_tsk1 = "Task_1: ";
  char *msg_btn =  "Button_0: ";
  char *msg_tsk2 = "Task_2: ";
  unsigned char byte = 0;
  /* count the number of Task2 activations */
  task2_fired++;

  /* let blink leds 6 or 7 */
  if (which_led) 
  {
	led_status &= (~LED_3);
    EE_led_3_off();
    which_led = 0;
  }
  else 
  {
	led_status |= LED_3;
	EE_led_3_on();
    which_led = 1;
  }

  /* prints a report
   * Note: after the first printf in main(), then only this task uses printf
   * In this way we avoid raceconditions in the usage of stdout.
   */
  
  EE_SCISendChars(SCI_0, msg_tmr,ALL);
  byte = ((timer_fired%1000)/100)+'0';
  EE_SCISendBuffer(SCI_0,byte);
  byte = ((timer_fired%100)/10)+'0';
  EE_SCISendBuffer(SCI_0,byte);
  byte = (timer_fired%10)+'0';
  EE_SCISendBuffer(SCI_0,byte);
  EE_SCISendBuffer(SCI_0,' ');

  EE_SCISendChars(SCI_0, msg_tsk1,ALL);
  byte = ((task1_fired%1000)/100)+'0';
  EE_SCISendBuffer(SCI_0,byte);
  byte = ((task1_fired%100)/10)+'0';
  EE_SCISendBuffer(SCI_0,byte);
  byte = (task1_fired%10)+'0';
  EE_SCISendBuffer(SCI_0,byte);
  EE_SCISendBuffer(SCI_0,' ');

  EE_SCISendChars(SCI_0, msg_btn,ALL);
  byte = ((button_fired%1000)/100)+'0';
  EE_SCISendBuffer(SCI_0,byte);
  byte = ((button_fired%100)/10)+'0';
  EE_SCISendBuffer(SCI_0,byte);
  byte = (button_fired%10)+'0';
  EE_SCISendBuffer(SCI_0,byte);
  EE_SCISendBuffer(SCI_0,' ');

  EE_SCISendChars(SCI_0, msg_tsk2,ALL);
  byte = ((task2_fired%1000)/100)+'0';
  EE_SCISendBuffer(SCI_0,byte);
  byte = ((task2_fired%100)/10)+'0';
  EE_SCISendBuffer(SCI_0,byte);
  byte = (task2_fired%10)+'0';
  EE_SCISendBuffer(SCI_0,byte);

  EE_SCISendBuffer(SCI_0,'\n');
  TerminateTask();
}
  
// MAIN function 
int main()
{ 
  /* Analog to digital converter */
  //EE_adc_init( (unsigned char)ATDRES_8BIT, 0x02 );
  
  /* Serial interface */
  EE_SCIOpenCommunication(SCI_0);
  
  ///* Program Timer 1 to raise interrupts */
  EE_PIT0_init(99, 14, 2);
  
  /* Init devices */
  EE_buttons_init(BUTTON_0,3);
  
  /* Init leds */
  EE_leds_init();

  mydelay(10);
  
  message();
  
  /* let's start the multiprogramming environment...*/
  StartOS(OSDEFAULTAPPMODE);
  
  /* now the background activities... */
  for (;;)
  {
     // if(EE_button_get_B0())        			// PP0 pushed
//      {
//          handle_button_interrupts(); 
//      }   
  }
  return 0;
}

void message(void)
{
	char * msg = "I Love OSEK and Erika Enterprise!!!";
	EE_SCISendChars(SCI_0, msg,ALL);
	EE_SCISendBuffer(SCI_0,'\n');
	return;	
}
