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
 * Author: 2008 Paolo Tiberi & Francesco Focacci
 * CVS: $Id: ee_common.h,v 1.6 2008/07/18 09:53:55 tiberipa Exp $
 */

#ifndef __INCLUDE_FRSH_COMMON_H__
#define __INCLUDE_FRSH_COMMON_H__


/*************************************************************************
 Note
 *************************************************************************

Some details about the FRSH implementation:

FRSH uses a stacked queue.

The stacked queue contains all the tasks that has been executed for
some time, and that have locked a resource

Note that if a task has been executed it must have been that the task
preemption level was greater than the system ceiling, and that their
absolute deadline was earlier than the task on the top of the stacked
queue.

This also means that the stacked queue is by construction ordered by
increasing deadlines.

When a task is preempted and it has a locked resource, then the task
ends up in the stacked queue, otherwise it ends up into the ready
queue. 

In fact, when activating a task the ActivateTask does a preemption
Check between the running task and the first task in the ready queue,
including the preeemption level.

When a task terminates, we suppose that NO RESOURCES are locked. If
the task has a nact>0 the task is inserted in the ready queue (there
are no resources locked), and the next running task is chosen between
the ready and the stacked queue. The task on the ready queue wins if
it has a deadline earlier than the task on the stacked queue and if it
has a preemption level greater than the system ceiling (the system
ceiling subsumes in a single value all the ceilings of the locked
resources by the tasks in the stacked queue)

If a task to execute is chosen from the stacked queue, then the task
is removed from the stacked queue and it is put into exec.

Please note the following difference between (FP, EDF) and FRSH.  FRSH
does a distinction between exec and stkfirst. the rationale is that
exec may not influence the system ceiling (for example, the running
task did not lock any resource). The stacked queue maintains the
meaning of containing all the tasks that modified the system ceiling
(that is, that locked a resource).

There is a need to have a separate stacked queue because if the top 
ready task fails the preemption test at the end of a task, then the 
running task must become the highest priority task (the one with the
earliest deadline) among those who modified the system ceiling. 

Finally note that since FP and EDF implements preemption thresholds, 
all the tasks at the end make a modification to the system ceiling, 
and then the exec task can coincide without problems to the first 
stacked task.


Notes on EE_th_active
---------------------
The FRSH Kernel is using a timer representation embedded in a timer
value. All times in the system are relative to the value of the 
current timer.

When a task stop executing for some time, its deadline is no more 
updated, and it will tend to go far in the past. Since the time 
reference is relative to the current timer value, it may happen 
after some time (half the lifetime of the timer), that this deadline 
in the past becomes a deadline "in the future". At that point, all 
the deadline update strategies will fail.

To avoid that, the user have to periodically "take a look" at 
all the task deadlines to see if they go too much in the past 
(if a deadline goes in the past it measn the task has not executed 
for a while). If that happens, then the function 
EE_frsh_deadlinecheck will set these tasks to "inactive", and then
when the task wuill be activated again a fresh deadline will be 
assigned.

EE_th_active tracks exactly the fact that a task is inactive due 
to the deadline aging. when a task is inactive, when activated a 
task will be assigned a new proper deadline, in the function 
EE_frsh_updatecapacity

-- end note */

/*************************************************************************
 Kernel Constants
 *************************************************************************/

/* invalid TID */
#define EE_NIL       ((EE_TID)-1)

/* Thread statuses */


#define EE_SUSPENDED  0

#define EE_READY      1

/* used for tasks which go recharging. A task goes recharging when it
   has exausted its budget but it could still be ready */
#define EE_RECHARGING 2

/* used by semaphores and blocking primitives in general */
#define EE_BLOCKED    4

/* used to know if a task has some space allocated on its stack */
#define EE_WASSTACKED 8

/*************************************************************************
 Kernel Types
 *************************************************************************/

/* priority type, used for system ceiling and preemption level */
#ifndef EE_TYPEPRIO
#define EE_TYPEPRIO EE_UREG
#endif

/* status type */
#ifndef EE_TYPESTATUS
#define EE_TYPESTATUS EE_UREG
#endif

/* relative deadline type */
#ifndef EE_TYPERELDLINE
#define EE_TYPERELDLINE EE_TIME
#endif

/* absolute deadline type */
#ifndef EE_TYPEABSDLINE
#define EE_TYPEABSDLINE EE_TIME
#endif

/* capacity type */
#ifndef EE_TYPECAPACITY
#define EE_TYPECAPACITY EE_STIME
#endif

/* pending activation type */
#ifndef EE_TYPENACT
#define EE_TYPENACT EE_UREG
#endif

/* mutex ID type */
#ifndef EE_MUTEX
#define EE_MUTEX EE_UREG
#endif


/*************************************************************************
 Kernel Variables
 *************************************************************************/

/* ROM */
extern const EE_TYPERELDLINE EE_th_prlevel[];        /* task preemption level */
extern const EE_TYPEPRIO     EE_mutex_ceiling[];     /* mutex ceiling */

extern EE_TYPECAPACITY EE_th_budget[];         /* FRSH mean execution time */
extern EE_TYPERELDLINE EE_th_period[];         /* FRSH replenishment period */

typedef struct {
  EE_TYPECAPACITY capacity;
  EE_TYPERELDLINE period;
  EE_TYPERELDLINE inv_proc_util;
} EE_TYPECONTRACT;

/* This structure contains the statically defined contractd defined into the OIL File */
extern const EE_TYPECONTRACT EE_contracts[];
	
/* This vector contains the inverse processor utilization of each task 
   if the budget and the period are power of 2, it contains the logarithm of the
   ratio in order to speed up the multiplication */
extern EE_TYPERELDLINE EE_inv_proc_util[];     /* inverse processor utilization */

/* RAM */
extern EE_TYPECAPACITY EE_th_budget_avail[];	 /* available budget (initvalue: 0) */
extern EE_TYPEABSDLINE EE_th_absdline[];         /* task absolute deadline (initvalue: 0) */

extern EE_TYPESTATUS   EE_th_status[];	         /* thread status (initvalue: EE_READY) */
extern EE_TYPENACT     EE_th_nact[];		 /* pending activations (initvalue: 0) */
extern EE_TID          EE_th_next[];		 /* next task in queue (initvalue: EE_NIL) */

extern EE_TYPEPRIO     EE_mutex_oldceiling[];	 /* old mutex ceiling (initvalue: none) */
extern EE_UREG         EE_th_lockedcounter[];	 /* number of mutexes locked (initvalue: 0) */
/* EE_mutex_lockedcounter is used to know if a task have to be put in the ready or 
   in the stacked queue */

extern EE_UREG         EE_th_active[];           /* task is active ? (initvalue: 0) see note above */
   
#ifdef __SEM_FRSH__
#include "../syncobj/inc/ee_sem.h"
extern EE_TID          EE_th_nextsem[];	 /* next task in the sem wait queue (initvalue: EE_NIL) */
extern SemRefType      EE_th_semrefs[];    /* Task reference to the used semaphore */
extern EE_TIME         EE_th_timeouts[];   /* Task semaphore timeouts */
extern char            EE_th_timedout[];   /* used to check if timedout*/
#endif

/* system ceiling (initvalue: 0) */
extern EE_TYPEPRIO EE_sys_ceiling;

/* The first task into the ready queue -> ready means that the task
   has been activated without never running or that it has been
   preempted by another task, in both cases without currently locking
   any resource 
   (initvalue: EE_NIL)
*/
extern EE_TID EE_rqfirst;

/* The first stacked task -> stacked means a preempted task that owns a mutex 
   (initvalue: EE_NIL)
*/
extern EE_TID EE_stkfirst;

/* The first task in the recharging queue
*/
extern EE_TID EE_rcgfirst;

/* The running task 
   (initvalue: EE_NIL)
*/
extern EE_TID EE_exec;

/* a temporary value that stores the timer read that was done before
   executing a task. That value is useful for capacity accounting 
   (initvalue: none)
*/
extern EE_TIME EE_last_time;

/* This variable is used to check whether a the schedule procedure 
 * at the end of an ISR should be executed or not
*/
extern int EE_served;

#endif


