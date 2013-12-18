/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2012  Evidence Srl
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

/**
 * @author Errico Guidieri
 * @date   2012
 **/

/*******************************************************************************
 * Interface header to Erika Kernel support.
 ******************************************************************************/

#ifndef INCLUDE_PKG_OS_H__
#define INCLUDE_PKG_OS_H__

#if defined(__cplusplus)
extern "C" {
#endif

/*
 *
 * CPU minimal support for OS
 *
 */

#if defined __PPCE200Z7__ || defined __PPCE200ZX__
#include "cpu/e200zx/inc/ee_cpu_os.h"
#endif /* defined __PPCE200Z7__ || defined __PPCE200ZX__ */

#ifdef EE_TRICORE__
#include "cpu/tricore/inc/ee_tc_cpu.h"
#include "cpu/tricore/inc/ee_tc_irq.h"
#endif /* EE_TRICORE__ */

/*
 * I need kernel inclusion before IRQ CPU inclusion because is CPU layer that
 * have to see Kernel API for IRQ Handling
 */

/*
 * Kernel
 *
 */
#ifdef __FP__
#include "kernel/fp/inc/ee_kernel.h"
#include "kernel/fp/inc/ee_irq.h"
#endif

#ifdef __EDF__
#include "kernel/edf/inc/ee_kernel.h"
#include "kernel/edf/inc/ee_irq.h"
#endif

#ifdef __FRSH__
#include "kernel/frsh/inc/ee_kernel.h"
#include "kernel/frsh/inc/ee_irq.h"
#endif

/* OO */
#if defined(__OO_BCC1__) || defined(__OO_BCC2__) || defined(__OO_ECC1__) || \
defined(__OO_ECC2__) || defined(__AS_SC4__)
#include "kernel/oo/inc/ee_oo_kernel.h"
#include "kernel/oo/inc/ee_oo_irq.h"
#endif

/*
 * CPU IRQ support
 */

#if defined __PPCE200Z7__ || defined __PPCE200ZX__
#include "cpu/e200zx/inc/ee_irq.h"
#endif

/* kernel API inclusion */
#include "ee_api.h"

#if defined(__cplusplus)
};
#endif


#endif /* INCLUDE_PKG_OS_H__ */

