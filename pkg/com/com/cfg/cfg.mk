# ###*B*###
# ERIKA Enterprise - a tiny RTOS for small microcontrollers
# 
# Copyright (C) 2002-2008  Evidence Srl
# 
# This file is part of ERIKA Enterprise.
# 
# ERIKA Enterprise is free software; you can redistribute it
# and/or modify it under the terms of the GNU General Public License
# version 2 as published by the Free Software Foundation, 
# (with a special exception described below).
# 
# Linking this code statically or dynamically with other modules is
# making a combined work based on this code.  Thus, the terms and
# conditions of the GNU General Public License cover the whole
# combination.
# 
# As a special exception, the copyright holders of this library give you
# permission to link this code with independent modules to produce an
# executable, regardless of the license terms of these independent
# modules, and to copy and distribute the resulting executable under
# terms of your choice, provided that you also meet, for each linked
# independent module, the terms and conditions of the license of that
# module.  An independent module is a module which is not derived from
# or based on this library.  If you modify this code, you may extend
# this exception to your version of the code, but you are not
# obligated to do so.  If you do not wish to do so, delete this
# exception statement from your version.
# 
# ERIKA Enterprise is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License version 2 for more details.
# 
# You should have received a copy of the GNU General Public License
# version 2 along with ERIKA Enterprise; if not, write to the
# Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301 USA.
# ###*E*###

## Author: 2004 Paolo Gai
## CVS: $Id: cfg.mk,v 1.3 2006/11/03 11:05:21 pj Exp $

########### COM CCCA #############
ifeq ($(findstring __COM_CCCA__,$(EEOPT)) , __COM_CCCA__) 
EE_SRCS += pkg/src/com/com/src/com.c
EE_SRCS += pkg/src/com/com/src/msgreception.c
EE_SRCS += pkg/src/com/com/src/filters.c
EE_SRCS += pkg/src/com/com/src/memo.c
EE_SRCS += pkg/src/com/com/src/startcom.c
EE_SRCS += pkg/src/com/com/src/stopcom.c
EE_SRCS += pkg/src/com/com/src/initmessage.c
EE_SRCS += pkg/src/com/com/src/getcomapplicationmode.c
EE_SRCS += pkg/src/com/com/src/comerrorgetserviceid.c
EE_SRCS += pkg/src/com/com/src/sendstatic.c
EE_SRCS += pkg/src/com/com/src/receivestatic.c
endif

########### COM CCCB #############
ifeq ($(findstring __COM_CCCB__,$(EEOPT)) , __COM_CCCB__) 
EE_SRCS += pkg/src/com/com/src/com.c
EE_SRCS += pkg/src/com/com/src/msgreception.c
EE_SRCS += pkg/src/com/com/src/filters.c
EE_SRCS += pkg/src/com/com/src/memo.c
EE_SRCS += pkg/src/com/com/src/startcom.c
EE_SRCS += pkg/src/com/com/src/stopcom.c
EE_SRCS += pkg/src/com/com/src/initmessage.c
EE_SRCS += pkg/src/com/com/src/getcomapplicationmode.c
EE_SRCS += pkg/src/com/com/src/comerrorgetserviceid.c
EE_SRCS += pkg/src/com/com/src/sendstatic.c
EE_SRCS += pkg/src/com/com/src/receivestatic.c
EE_SRCS += pkg/src/com/com/src/msgstatus.c
endif

########### COM CCC0 #############
ifeq ($(findstring __COM_CCC0__,$(EEOPT)) , __COM_CCC0__) 
EE_SRCS += pkg/src/com/com/src/com.c
EE_SRCS += pkg/src/com/com/src/msgreception.c
EE_SRCS += pkg/src/com/com/src/filters.c
EE_SRCS += pkg/src/com/com/src/memo.c
EE_SRCS += pkg/src/com/com/src/startcom.c
EE_SRCS += pkg/src/com/com/src/stopcom.c
EE_SRCS += pkg/src/com/com/src/initmessage.c
EE_SRCS += pkg/src/com/com/src/getcomapplicationmode.c
EE_SRCS += pkg/src/com/com/src/comerrorgetserviceid.c
EE_SRCS += pkg/src/com/com/src/sendstatic.c
EE_SRCS += pkg/src/com/com/src/receivestatic.c
EE_SRCS += pkg/src/com/com/src/convert.c
EE_SRCS += pkg/src/com/com/src/queue_TM.c
EE_SRCS += pkg/src/com/com/src/dmonitor.c
endif

########### COM CCC1 #############
ifeq ($(findstring __COM_CCC1__,$(EEOPT)) , __COM_CCC1__) 
EE_SRCS += pkg/src/com/com/src/com.c
EE_SRCS += pkg/src/com/com/src/msgreception.c
EE_SRCS += pkg/src/com/com/src/filters.c
EE_SRCS += pkg/src/com/com/src/memo.c
EE_SRCS += pkg/src/com/com/src/startcom.c
EE_SRCS += pkg/src/com/com/src/stopcom.c
EE_SRCS += pkg/src/com/com/src/initmessage.c
EE_SRCS += pkg/src/com/com/src/getcomapplicationmode.c
EE_SRCS += pkg/src/com/com/src/comerrorgetserviceid.c
EE_SRCS += pkg/src/com/com/src/sendstatic.c
EE_SRCS += pkg/src/com/com/src/receivestatic.c
EE_SRCS += pkg/src/com/com/src/msgstatus.c
EE_SRCS += pkg/src/com/com/src/convert.c
EE_SRCS += pkg/src/com/com/src/queue_TM.c
EE_SRCS += pkg/src/com/com/src/dmonitor.c
EE_SRCS += pkg/src/com/com/src/sendzero.c
EE_SRCS += pkg/src/com/com/src/senddynamic.c
EE_SRCS += pkg/src/com/com/src/receivedynamic.c
EE_SRCS += pkg/src/com/com/src/notify_error.c
EE_SRCS += pkg/src/com/com/src/startperiodic.c
EE_SRCS += pkg/src/com/com/src/stopperiodic.c
EE_SRCS += pkg/src/com/com/src/queue_DM.c
endif
