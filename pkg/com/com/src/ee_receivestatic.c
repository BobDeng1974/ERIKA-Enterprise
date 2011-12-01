GetResource/* ###*B*###
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
 * Author: 2003 Francesco Bertozzi
 * CVS: $Id: ee_receivestatic.c,v 1.2 2005/07/17 13:58:36 pj Exp $
 */

#include "com/com/inc/ee_comprv.h"

/* 2.9.2.4.2 */
#ifndef __PRIVATE_COM_RECEIVEMESSAGE__
StatusType EE_com_ReceiveMessage(SymbolicName Message,
          ApplicationDataRef DataRef)
{
  register struct EE_com_msg_RAM_TYPE * msg_RAM;
  register const struct EE_com_msg_ROM_TYPE * msg_ROM;
  StatusType ret_code;
#ifdef __COM_HAS_ERRORHOOK__  	
  register EE_FREG flags;
#endif	
        
#ifdef __EE_COM_EXTENDED__
  if ((Message > EE_COM_N_MSG) || 
      (EE_com_msg_ROM[Message] == NULL) || 
      ((EE_com_msg_RAM[Message]->property & 0x8000) ==  0x0000) || 
      ((EE_com_msg_RAM[Message]->property & 0x0100) == 
       EE_COM_QUEUED) || 
      (EE_com_msg_ROM[Message]->size == 0)) 
  {
    EE_com_sys2user.service_error = COMServiceId_ReceiveMessage;
#ifdef __COM_HAS_ERRORHOOK__ 
    flags = EE_hal_begin_nested_primitive();    
      COMError_ReceiveMessage_Message = Message;
      COMError_ReceiveMessage_DataRef = DataRef;
  
      if (!EE_com_ErrorHook.already_executed)
      {        
        EE_com_ErrorHook.already_executed = EE_COM_TRUE;
        COMErrorHook(E_COM_ID);    
        EE_com_ErrorHook.already_executed = EE_COM_FALSE;
      }
    EE_hal_end_nested_primitive(flags);  
#endif
    return E_COM_ID;
  }
#endif
   
  msg_RAM = EE_com_msg_RAM[Message];
  msg_ROM = EE_com_msg_ROM[Message];
    
  ret_code = msg_RAM->property & EE_MASK_MSG_STAT;

  if (ret_code != E_COM_NOMSG) 
  {
    #if defined(__COM_CCC0__) || defined(__COM_CCC1__)
    GetResource (EE_MUTEX_COM_IPDU);
    #endif
    GetResource (EE_MUTEX_COM_MSG);
    
#if defined(__COM_CCCB__) || defined(__COM_CCC1__)
    if ((msg_RAM->property & EE_MASK_MSG_QUEUNQUE) 
          == EE_COM_QUEUED) 
    {  
      /* Message queued */
      register struct EE_COM_msg_queue *temp = 
            (struct EE_COM_msg_queue *)msg_ROM->data;
      EE_com_memo(&(temp->data[temp->first * EE_com_bit2byte(msg_ROM->size)]), 
            0, DataRef, 0, msg_ROM->size); 

      // temp->first = ++temp->first % temp->max;
      // substituted with an if statemet to avoid the mod operation
      temp->first++;
      if (temp->first == temp->max) 
        temp->first = 0;

      temp->tot--;
      if (temp->tot == 0) 
      {
        msg_RAM->property &= 0xfff8;
        msg_RAM->property |= 0x0004; /* E_COM_NOMSG */
      } 
      else 
        msg_RAM->property &= 0xfff8; /* E_OK */
    }
    else
#endif
    { /* Message unqueued */
      EE_com_memo((EE_UINT8 *)msg_ROM->data, 0, 
         DataRef, 0, msg_ROM->size);
      msg_RAM->property &= 0xfff8; /* E_OK */ 
    }
    
    ReleaseResource (EE_MUTEX_COM_MSG);
    #if defined(__COM_CCC0__) || defined(__COM_CCC1__)
    ReleaseResource (EE_MUTEX_COM_IPDU);
    #endif
  }
   
#ifndef __COM_CCCA__
  if ((msg_RAM->property & EE_MASK_MSG_N_OK) == EE_COM_F_OK)  
    *(FlagValue *)msg_ROM->notify_call = EE_COM_FALSE;
#ifdef __COM_CCC1__
  if ((msg_RAM->property & EE_MASK_MSG_N_ER) == EE_COM_F_ER) 
    *(FlagValue *)msg_ROM->error_call = EE_COM_FALSE;
#endif
#endif
  
  if (ret_code != E_OK)
  {
    EE_com_sys2user.service_error = COMServiceId_ReceiveMessage;
#ifdef __COM_HAS_ERRORHOOK__    
    flags = EE_hal_begin_nested_primitive(); 
      COMError_ReceiveMessage_Message = Message;
      COMError_ReceiveMessage_DataRef = DataRef;
      if (!EE_com_ErrorHook.already_executed)
      {        
        EE_com_ErrorHook.already_executed = EE_COM_TRUE;
        COMErrorHook(ret_code);    
        EE_com_ErrorHook.already_executed = EE_COM_FALSE;
      }
    EE_hal_end_nested_primitive(flags);  
#endif
  }
      
  return ret_code;
}
#endif
