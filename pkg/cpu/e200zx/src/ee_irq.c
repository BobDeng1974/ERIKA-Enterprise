/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2010  Evidence Srl
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
 * Derived from the mico32 code.
 * Author: 2010 Fabio Checconi
 */

#include <ee_internal.h>
#include <cpu/e200zx/inc/ee_irq.h>
#include <cpu/e200zx/inc/ee_irq_internal.h>
#include <cpu/common/inc/ee_irqstub.h>
#include <cpu/e200zx/inc/ee_mcu_regs.h>


#define EE_E200Z7_MAX_IRQ	488

void EE_e200z7_irq(int level)
{
	EE_e200z7_ISR_handler f;

	EE_increment_IRQ_nesting_level();
	f = EE_e200z7_ISR_table[level];
	if (f) {
		EE_e200z7_call_ISR_new_stack(level, f, EE_IRQ_nesting_level);
	}

	EE_decrement_IRQ_nesting_level();
	if (!EE_is_inside_ISR_call()) {
		/*
		 * Outer nesting level: call the scheduler.  If we have
		 * also type-ISR1 interrupts, the scheduler should be
		 * called only for type-ISR2 interrupts.
		 * WTF?  It doesn't work, does it?
		 */
		EE_std_after_IRQ_schedule();
	}
}

#ifndef __STATIC_ISR_TABLE__

EE_e200z7_ISR_handler EE_e200z7_ISR_table[EE_E200Z7_MAX_IRQ + 1];

void EE_e200z7_register_ISR(int level, EE_e200z7_ISR_handler fun, EE_UINT8 pri)
{
	EE_FREG intst = EE_e200z7_disableIRQ();

	EE_e200z7_ISR_table[level] = fun;

	if (level >= 16) {
		INTC.PSR[level - 16].R = pri;
	}

	if (EE_e200z7_are_IRQs_enabled(intst)) {
		EE_e200z7_enableIRQ();
	}
}

#endif