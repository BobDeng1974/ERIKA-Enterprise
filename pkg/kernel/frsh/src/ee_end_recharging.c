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

#include "ee_internal.h"

#ifndef __PRIVATE_IRQ_END_RECHARGING__
/* This primitive shall be atomic.
   This primitive shall be inserted as the last function in the IRQ handler
   of the timer used for capacity.
   If the HAL allow IRQ nesting the C_end_instance should work as follows:
   - it must implement the preemption test only if it is the last IRQ
     on the stack
   - if there are other interrupts on the stack the IRQ scheduler
     should do nothing
*/

/*
 * This routine is called when the recharging time of a task is finished
 * and when the a timeout of semaphore is exipired
 */

int EE_served=0;

void EE_IRQ_end_recharging(void)
{
  //this routine does rescheduling
  EE_served=1;

  register EE_TIME tmp_time;
  register EE_TID t,tmp_rq;

  // Check for negative budget and in case re-insert in the rcg queue
  tmp_time = EE_hal_gettime();
  
#ifdef DEBUG  
  if(EE_rcg_queryfirst() == EE_NIL) for(;;);
#endif

  do{
    t = EE_rcg_queryfirst();
    EE_rcg_getfirst();
    if(EE_th_status[t] & EE_RECHARGING){
      if( (EE_th_budget_avail[t] += EE_th_budget[t]) > EE_TIMER_MINCAPACITY ) {
        EE_th_status[t] = EE_READY | EE_WASSTACKED;
        EE_rq_insert(t);
        EE_th_absdline[t]=tmp_time+EE_th_period[t];
      }
      else {
        EE_th_absdline[t]+=EE_th_period[t];
        EE_rcg_insert(t);
      }
    }
#ifdef __SEM_FRSH__
    else if(EE_th_status[t] & EE_BLOCKED)
    {
      EE_TID p,q; // blocked status
    
      p = EE_NIL;
      q = EE_th_semrefs[t]->first;
    
      while ((q != EE_NIL) && (q != t)) {
        p = q;
        q = EE_th_nextsem[q];
      }
      if( q == EE_NIL ) for(;;);
      if( p == EE_NIL) {
        EE_th_semrefs[t]->first=EE_th_nextsem[t];
        if(EE_th_semrefs[t]->last == t){
          EE_th_semrefs[t]->last=EE_NIL;
        }
      }
      else {
        EE_th_nextsem[p] = EE_th_nextsem[t];
        if(EE_th_nextsem[p] == EE_NIL){
          EE_th_semrefs[t]->last = p;
        }
      }
 
      EE_th_status[t] = EE_READY | EE_WASSTACKED;
      EE_th_timedout[t]=1;
      EE_rq_insert(t);
    }
#endif
#ifdef DEBUG
    else for(;;);
#endif
    
  }while((EE_rcg_queryfirst() !=EE_NIL && 
      (EE_th_status[EE_rcg_queryfirst()] & EE_RECHARGING) &&
      (EE_STIME)(EE_th_absdline[EE_rcg_queryfirst()]-tmp_time) <= 100) 
#ifdef __SEM_FRSH__
		||
      (EE_rcg_queryfirst() !=EE_NIL &&
      (EE_th_status[EE_rcg_queryfirst()] & EE_BLOCKED) &&  
      (EE_STIME)(EE_th_timeouts[EE_rcg_queryfirst()]-tmp_time) <= 100)
#endif
		);
  
  tmp_rq = EE_rq_queryfirst();
  /* check if there is a preemption */
  
  if (EE_exec == EE_NIL ||	/* main task! */
    ((tmp_rq != EE_NIL) && (EE_STIME)(EE_th_absdline[EE_exec] - EE_th_absdline[tmp_rq]) > 0
     && EE_sys_ceiling < EE_th_prlevel[tmp_rq])) {
         /* we have to schedule a ready thread */

    register int flag;
    register EE_TID old_exec;

    old_exec = EE_exec;
    EE_exec = tmp_rq;

    /* remove the first task from the ready queue, and set the new
       exec task as READY */
    flag = EE_th_status[tmp_rq] & EE_WASSTACKED;
    EE_th_status[tmp_rq] = EE_READY;
    EE_rq_getfirst();
    
    if((EE_STIME)(tmp_time - EE_th_absdline[EE_exec])> EE_TIMER_MINCAPACITY){
      EE_th_absdline[EE_exec]=tmp_time+EE_th_period[EE_exec];
      EE_th_budget_avail[EE_exec]=EE_th_budget[EE_exec];
    }
    
    /* manage the old exec task */
    if (old_exec != EE_NIL) {
      /* account the capacity to the task that is currently executing */
      EE_th_budget_avail[old_exec] -= tmp_time - EE_last_time;
      
      EE_th_status[old_exec] |= EE_WASSTACKED;

      if (EE_th_lockedcounter[old_exec])
	      EE_stk_insertfirst(old_exec);
      else
        EE_rq_insert(old_exec);
    }
    /* program the capacity interrupt */
    EE_hal_capacityIRQ(EE_th_budget_avail[EE_exec]);
    
    if (flag)
      EE_hal_IRQ_stacked(EE_exec);
    else
      EE_hal_IRQ_ready(EE_exec);
  }else {
    if(EE_exec == EE_NIL)
      EE_hal_stop_budget_timer();
    EE_hal_IRQ_stacked(EE_exec);
  }
  
  /* this has to be done in any case */
  EE_last_time = tmp_time;

#ifdef DEBUG
if(EE_exec == EE_NIL)
    for(;;);
#endif

  // Program the recharging timer
  if(EE_rcg_queryfirst() != EE_NIL){
#ifdef __SEM_FRSH__
    if(EE_th_status[EE_rcg_queryfirst()] & EE_BLOCKED) {
      EE_hal_rechargingIRQ(EE_th_timeouts[EE_rcg_queryfirst()]-tmp_time);
    }
    else
#endif
      EE_hal_rechargingIRQ(EE_th_absdline[EE_rcg_queryfirst()] - tmp_time);
  }

}

#endif

