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
 * CVS: $Id: ee_swait.c,v 1.3 2008/07/18 09:53:55 tiberipa Exp $
 */


/*
  From the FRESCOR documentation:

  frsh_synchobj_wait_with_timeout()

  This call is the same as frsh_synchobj_wait() but with an extra absolute timeout. The timed_out argument,
  indicates whether the function returned because of the expiration of the timeout or not.

  Returns: 
  0 if success
  FRSH_ERR_BAD_ARGUMENT : if synch_handle is 0 or the abs_timeout argument is NULL or its value is in the past
  FRSH_ERR_INTERNAL_ERROR : if the task still uses a resource

 */


#include "ee_internal.h"
#include "frsh_core_types.h"
#include "frsh_error.h"

/* This file contains all what is needed to handle the timeout operation */

#ifndef __PRIVATE_TIMEOUT_INSERT__
void EE_frsh_timeout_insert(EE_TID t)
{
  EE_TYPEABSDLINE prio;
  EE_TID p,q;

  p = EE_NIL;
  q = EE_frsh_timeout_first;
  prio = EE_frsh_timeout[t].timeout;

  while ((q != EE_NIL) && 
	 (EE_STIME)(prio - EE_frsh_timeout[q].timeout) >= 0
  	) {
    p = q;
    q = EE_frsh_timeout[q].next;
  }

  if (p != EE_NIL)
    EE_frsh_timeout[p].next = t;
  else
    EE_frsh_timeout_first = t;

  EE_frsh_timeout[t].next = q;
}

#endif


#ifndef __PRIVATE_TIMEOUT_EXTRACT__
void EE_frsh_timeout_extract(EE_TID t, EE_TIME tmp_time)
{
  EE_TID p,q;

  p = EE_NIL;
  q = EE_frsh_timeout_first;

  while ( (q != EE_NIL) && (q != t)) {
    p = q;
    q = EE_frsh_timeout[q].next;
  }
  
  if (q == EE_NIL) {
    /* the thread is not there ??? */
  } else {
    /* q == t !!! */
    if ( p == EE_NIL ) {
      // remove the first item in the timeout queue
      EE_frsh_timeout_first = EE_frsh_timeout[EE_frsh_timeout_first].next;

      // reprogram the timer
      if (EE_frsh_timeout_first == EE_NIL) {
	EE_hal_stop_synchobj_timeout_timer();
      } else {
	EE_hal_set_synchobj_timeout_timer(EE_frsh_timeout[EE_frsh_timeout_first].timeout - tmp_time);
      }
    } else {
      // remove an item in the middle of the timeout queue
      EE_frsh_timeout[p].next = EE_frsh_timeout[q].next;
    }
  }
}

#endif



/* removes a task from the synchobj queue */
#ifndef __PRIVATE_SYNCHOBJ_EXTRACT__
void EE_frsh_synchobj_extract(EE_TID thread)
{
  EE_TID p;
  EE_TID t;
  frsh_synchobj_handle_t synch_handle = EE_frsh_timeout[thread].synchobj;
  
  p = EE_NIL;
  t = synch_handle->first;
  
  while ( (t != EE_NIL) && (t != thread)) {
    p = t;
    t = EE_th[t].next;
  }
  
  if (t == EE_NIL) {
    /* the thread is not there ??? */
  } else {
    /* t == thread !!! */
    if ( p == EE_NIL ) {
      // remove the first item in the synchronization object queue
      synch_handle->first = EE_th[synch_handle->first].next;
    } else {
      // remove an item in the middle of the synchobject queue
      EE_th[p].next = EE_th[t].next;
    }
  }
}
#endif





/*
 * This routine is called when a primitiev with timeout is called
 * The result is that one or more tasks are put back into the ready queue with the imeout set.
 */
#ifndef __PRIVATE_IRQ_SYNCHOBJ_TIMEOUT__
void EE_frsh_IRQ_synchobj_timeout(void)
{
  register EE_TIME tmp_time;
  register EE_FREG flag;
  register EE_TID t;
  
  flag = EE_hal_begin_nested_primitive();

  /* read the current time */
  tmp_time = EE_hal_gettime();

  /* Check for expired timeouts */
  t = EE_frsh_timeout_first;
  while (t != EE_NIL) {
    if ((EE_STIME)(EE_frsh_timeout[t].timeout - tmp_time) <= EE_TIMER_MINCAPACITY) {
      /* remove the task from the timeout queue */
      EE_frsh_timeout_first = EE_frsh_timeout[t].next;

      /* remove the task from the synchobj queue */
      EE_frsh_synchobj_extract(t);

      /* reset the synchobj value, to say that the task is no more waiting on a synchobject, useful for the signal */
      EE_frsh_timeout[t].synchobj = 0;

      /* set the timeout flag */
      EE_frsh_timeout[t].flag = 1;

      /* wakeup the task pointed by EE_frsh_timeout_first */
      EE_th[t].status = EE_TASK_READY | EE_TASK_WASSTACKED;
      EE_rq_insert(t);

    } else {
      /* the tasks are ordered by deadline. if one fails, the others are for sure in the future */
      break;
    }

    t = EE_frsh_timeout_first;
  }

  /* t points to the top of the recharging queue */

  // Program the recharging timer
  if (t == EE_NIL) {
      EE_hal_stop_synchobj_timeout_timer();
  } else {
      EE_hal_set_synchobj_timeout_timer(EE_frsh_timeout[t].timeout - tmp_time);
  }

  EE_hal_end_nested_primitive(flag);
}

#endif










#ifndef __PRIVATE_FRSH_SYNCOBJ_WAIT_TIMEOUT__
int EE_frsh_synchobj_wait_with_timeout (const frsh_synchobj_handle_t synch_handle, 
					const frsh_abs_time_t *abs_timeout,
					bool *timed_out,
					frsh_rel_time_t *next_budget,
					frsh_rel_time_t *next_period,
					bool *was_deadline_missed,
					bool *was_budget_overran)
{
  register EE_FREG flag;
  register EE_TIME tmp_time;
  register EE_TID tmp_exec;
  register int returnvalue = 0;

  if (synch_handle == 0) {
    return FRSH_ERR_BAD_ARGUMENT;
  }

  flag = EE_hal_begin_nested_primitive();

  tmp_time = EE_hal_gettime();

  /* timeout not valid or in the past */
  /* I compare with 0, but the IRQ could be really near < min-capacity! */
  if (!abs_timeout || ((EE_STIME)(*abs_timeout-tmp_time))<0) {
    EE_hal_end_nested_primitive(flag);
    return FRSH_ERR_BAD_ARGUMENT;
  }
  
  /* save the current running task into a temporary variable */
  tmp_exec = EE_exec;
  /* --- */
  
  /* we always have to exec the check_slice and all the rescheduling
     also when the call is non-blocking because we have to fill the
     next_budget values! */

  /* check_slice: checks the elapsed time on the exec task, putting it into the right
     queue (recharging or ready). at the end EE_exec is EE_NIL */
  EE_frsh_check_slice(tmp_time);
  /* --- */

  /* implement the wait behavior */
  if (synch_handle->count)
    synch_handle->count--;
  else {
    /* The running task blocks: 
       - it must be removed from the ready queue
       - and then it must be inserted into the blocked queue */

    // TODO: what if the task has still locked a resource?
    /* the task has to be removed from the ready queue */

    if (EE_th[tmp_exec].status & EE_TASK_READY) {
      EE_rq_extract(tmp_exec);

      /* The task state switch from STACKED TO BLOCKED */
      EE_th[tmp_exec].status = EE_TASK_BLOCKED | EE_TASK_WASSTACKED;
      
      /* the system ceiling is not touched because it is only modified 
	 when locking a mutex */
      
      if (synch_handle->first != EE_NIL)
	// the synchobj queue is not empty
	EE_th[synch_handle->last].next = tmp_exec;
      else
	// the synchobj queue is empty
	synch_handle->first = tmp_exec;
      
      synch_handle->last = tmp_exec;
      EE_th[tmp_exec].next = EE_NIL;

      /* record the timeout into the data structures */
      EE_frsh_timeout[tmp_exec].flag = 0;
      EE_frsh_timeout[tmp_exec].timeout = *abs_timeout;
      EE_frsh_timeout[tmp_exec].synchobj = synch_handle;
      EE_frsh_timeout_insert(tmp_exec);
      if (EE_frsh_timeout_first == tmp_exec)
	EE_hal_set_synchobj_timeout_timer(*abs_timeout - tmp_time);
    }
    else
      returnvalue = FRSH_ERR_INTERNAL_ERROR;
  }

  /* check_recharging: if ready and stacked queue are empty pulls from the recharging queue */
  EE_frsh_check_recharging(tmp_time);
  /* --- */
  
  /* at this point, exec is for sure EE_NIL (it is set by check_slice) */
  /* select the first task from the ready or stacked queue */
  /* the function set the EE_exec value, removing the task from the queue
     the status is untouched */
  
  EE_frsh_select_exec();
  /* --- */
    
  EE_frsh_run_exec(tmp_exec);

  /* Fill the timeout information */
  if (timed_out)
    *timed_out = EE_frsh_timeout[EE_exec].flag;

  /* Fill the budget information */
  if (next_budget)
    *next_budget = EE_vres[EE_th[EE_exec].vres].budget_avail;

  if (next_period)
    *next_period = EE_ct[EE_th[EE_exec].vres].period;
    
  if (was_deadline_missed)
    was_deadline_missed = 0;

  if (was_budget_overran)
    was_budget_overran = 0;

  EE_hal_end_nested_primitive(flag);

  return returnvalue;
}
#endif



