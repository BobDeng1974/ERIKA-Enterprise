; ###*B*###
; ERIKA Enterprise - a tiny RTOS for small microcontrollers
;
; Copyright (C) 2002-2011  Evidence Srl
;
; This file is part of ERIKA Enterprise.
;
; ERIKA Enterprise is free software; you can redistribute it
; and/or modify it under the terms of the GNU General Public License
; version 2 as published by the Free Software Foundation, 
; (with a special exception described below).
;
; Linking this code statically or dynamically with other modules is
; making a combined work based on this code.  Thus, the terms and
; conditions of the GNU General Public License cover the whole
; combination.
;
; As a special exception, the copyright holders of this library give you
; permission to link this code with independent modules to produce an
; executable, regardless of the license terms of these independent
; modules, and to copy and distribute the resulting executable under
; terms of your choice, provided that you also meet, for each linked
; independent module, the terms and conditions of the license of that
; module.  An independent module is a module which is not derived from
; or based on this library.  If you modify this code, you may extend
; this exception to your version of the code, but you are not
; obligated to do so.  If you do not wish to do so, delete this
; exception statement from your version.
;
; ERIKA Enterprise is distributed in the hope that it will be
; useful, but WITHOUT ANY WARRANTY; without even the implied warranty
; of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License version 2 for more details.
;
; You should have received a copy of the GNU General Public License
; version 2 along with ERIKA Enterprise; if not, write to the
; Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
; Boston, MA 02110-1301 USA.
; ###*E*###

;/** 
;	@file ee_keil_change_context_isr.s
;	@brief Functions to active and manage the context switch for Cortex_MX 
;	@author Gianluca Franchino
;	@author Giuseppe Serano
;	@date 2011
;*/ 

;*******************************************************************************
;                         PUBLIC FUNCTIONS
;*******************************************************************************

	; Functions declared in this file 
	EXPORT	EE_switch_context		; void EE_switch_context(void);
#ifdef __CORTEX_M0__	
	EXPORT	PendSV_Handler		; void EE_cortex_mx_pendsv_ISR(void);
#else
	EXPORT	EE_cortex_mx_pendsv_ISR		; void EE_cortex_mx_pendsv_ISR(void);
#endif
	EXPORT	EE_set_switch_context_pri	; void EE_set_switch_context_pri(void);
	EXPORT	EE_cortex_mx_change_context_active	; EE_UREG EE_cortex_mx_change_context_active;

	IMPORT	EE_cortex_mx_change_context
	IMPORT	EE_std_endcycle_next_tid

;*******************************************************************************
;                              EQUATES
;*******************************************************************************
NVIC_INT_CTRL		EQU	0xE000ED04	; Interrupt control status register
NVIC_SHPR3		EQU	0xE000ED20	; System priority register (PendSV 14)
NVIC_PENDSV_PRI		EQU	0x00FF0000	; PendSV priority value (Lowest)
NVIC_PENDSVSET		EQU	0x10000000	; Value to trigger PendSV exception
NVIC_STKALIGN		EQU	0x00000200	; Stack Alignment on Exception Entry

EPSR_T_BIT_VAL		EQU	0x01000000	; Value to set the T-bit in EPSR (always Thumb mode)

;******************************************************************************
;
; Indicate that the code in this file preserves 8-byte alignment of the stack.
;
;******************************************************************************
	PRESERVE8

;*******************************************************************************
;                              DATA SECTION
;*******************************************************************************	
	AREA	|.data|,	READWRITE,	ALIGN=3
EE_cortex_mx_change_context_active	DCD	0	; EE_UREG EE_cortex_mx_change_context_active;

;*******************************************************************************
;                              CODE SECTION
;*******************************************************************************	
	AREA	|.text|,	CODE,	READONLY
	THUMB

; void EE_set_switch_context_pri(void)
EE_set_switch_context_pri
;Set PendSV priority to the minumum one
	LDR	R0, =NVIC_SHPR3
	LDR	R1, =NVIC_PENDSV_PRI
	LDR	R2, [R0];
	ORRS	R2, R2, R1;
	STR	R2, [R0];
	BX	LR

; void EE_switch_context(void)
EE_switch_context
	;Trigger the PendSV exception (causes context switch)
	LDR	R0, =NVIC_INT_CTRL
	LDR	R1, =NVIC_PENDSVSET
	STR	R1, [R0]
	BX	LR

; void EE_cortex_mx_pendsv_ISR(void)
#ifdef __CORTEX_M0__
PendSV_Handler
#else
EE_cortex_mx_pendsv_ISR
#endif
	CPSID	I			; Disable all interrupts except NMI (set PRIMASK)
					; Build a stack frame to jump into the EE_std_change_context(EE_TID)
					; at the end of PendSV_Handler.
	MRS	R2, PSR			; R2 = xPSR
	LDR	R0, =EPSR_T_BIT_VAL	; R0 = 0x01000000
	ORRS	R2, R2, R0		; R2 = (xPSR OR 0x01000000). This guarantees that Thumbs bit is set
					; to avoid an hard_fault exception
	MOVS	R0, #0x20		; R0 = 0x00000020
	RSBS	R0, R0, #0;		; R0 = 0xFFFFFFE0
	ANDS	R2, R2, R0		; R2 = (xPSR AND 0xFFFFFE0) -> In this way, we return in Thread mode. 
	LDR	R1, =EE_cortex_mx_change_context	; R1 = EE_cortex_mx_change_context (PC)
	LDR	R0, =exit_EE_cortex_mx_change_context	; R0 = exit_EE_cortex_mx_change_context (LR)
	PUSH	{R0-R2}			;
	MOV	R0, R12			;Since PUSH cannot use R12, put R0 onto stack as R12
	PUSH	{R0}
	LDR	R0, =EE_std_endcycle_next_tid		; R0 = address of EE_std_endcycle_next_tid
	LDR	R0,[R0]			; R0 = EE_std_endcycle_next_tid
					; Note: following the call convention, EE_cortex_mx_change_context
					; will get EE_std_endcycle_next_tid from R0.
	PUSH	{R0-R3}			;Fake IRQ handler frame on top of PendSV frame:
					;|xPSR|-> xPSR AND 0xFFFFFE0
					;| PC |-> EE_cortex_mx_change_context
					;| LR |-> exit_EE_cortex_mx_change_context
					;| R12|
					;| R3 |
					;| R2 |
					;| R1 |
					;| R0 | <- MSP

exitPendSV
	; EE_cortex_mx_change_context_active = 0
	SUBS	R0, R0, R0
	LDR	R1, =EE_cortex_mx_change_context_active
	STR	R0, [R1]
	
	MOVS	R0, #8			; R0 = 0x8
	RSBS	R0, R0, #0		; R0 = 0xFFFFFFF8
	ADDS	R0, R0, #1		; R0 = 0xFFFFFFF9 
					; EXC_RETURN = R0 -> Return to Thread mode.
					;		  -> Exception return gets state from MSP.
					;		  -> Execution uses MSP after return.
	BX	R0			; Exit interrupt

;
;This function restores the context before the IRQ that caused a context change
;through EE_cortex_mx_IRQ_active_change_context(void);
;
exit_EE_cortex_mx_change_context
	MRS	R0, MSP			; Get the stack pointer

	LDR	R3, [R0, #28]		; Status xPSR in R3

	LDR	R2, [R0, #20]		; Load LR from stack
	MOV	LR, R2			; Restore LR

	LDR	R1, [R0, #24]		; Get the value of PC from the stack
	ADDS	R1, R1, #0x01		; Set Bit[0] of R1 to ensure that T-bit in APSR is 1,

	LDR	R2, =NVIC_STKALIGN	; R2 = 0x00000200
	ANDS	R2, R2, R3		; Alignment test in R2

	CBZ	R2, stackAligned1
	ADDS	R0, R0, #4		; Adds alignment displacement

stackAligned1
	MSR	PSR, R3			; Restore xPSR
					; that is, the processor must work in Thumb mode
	STR	R1, [R0, #28]		; Store value of PC from stack has the
					; first value on the stack frame. 
					; This value is used as return address
	POP	{R1}			; Get R0 value from stack
	STR	R1, [R0, #24]		; Store value of R0 from the stack
					; onto the stack at the place of PC
	POP	{R1}			; Get R1 value from stack
	STR	R1, [R0, #20]		; Store value of R1 from the stack 
					; onto the stack at the place of LR
	MOV	R12, R4			; Use R12 to save the current value of R4
	MOV	R1, R2			; Aligment test in R1
	POP	{R2-R4}			; Restore Scratch registers
					; Note: in R4 we get the scratch register R12
	MOV	R0, R12			; Use R0 to save the value of R4 before the pop instruc.,
					; (which is stored in R12)
	MOV	R12, R4			; Restore Scratch register R12
	MOV	R4, R0			; Restore R4

	CBZ	R1, stackAligned2
	POP	{R0}

stackAligned2
	POP	{R1}			; Move stack pointer getting again R1
	CPSIE	I			; Enable interrupts (clear PRIMASK)
	POP	{R0, PC}		; Move stack pointer getting again R0 and return
					; updating PC

;******************************************************************************
;
; Make sure the end of this section is aligned.
;
;******************************************************************************
	ALIGN

;******************************************************************************
;
; Tell the assembler that we're done.
;
;******************************************************************************
	END

