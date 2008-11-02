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

/* this is a set of defines that should help reducing the coding in
   the OIL files */

#ifdef e7t_rtdruid
#define evaluator7t
#endif

#ifdef e7t_dist_src
#define evaluator7t
#endif

#ifdef e7t_dist_bin_full
#define evaluator7t
#endif

#ifdef e7t_dist_bin_lim
#define evaluator7t
#endif

#ifdef nios2_dist_bin_full
#define nios2
#endif

#ifdef nios2_dist_src
#define nios2
#endif

#ifdef nios2_dist_trace
#define nios2
#endif

/* Nios II */
//#define EE_NIOS2_SYSTEM_LIBRARY_PATH "C:/altera/kits/nios2_60/examples/verilog/niosII_stratix_1s40/standard/software/standard_syslib"
//#define EE_NIOS2_SYSTEM_LIBRARY_PATH_FULLFEATURED "C:/altera/kits/nios2_60/examples/verilog/niosII_stratix_1s40/full_featured/software/full_featured_syslib"
#define EE_NIOS2_SYSTEM_LIBRARY_PATH "C:/altera/80/nios2eds/examples/verilog/niosII_stratix_2s60/standard/software/standard_syslib"


#ifdef pic30_dist_bin_full
#define pic30
#endif

#ifdef pic30_dist_src
#define pic30
#endif
