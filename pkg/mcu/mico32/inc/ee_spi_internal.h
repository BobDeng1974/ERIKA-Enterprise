/*
* ee_spi_internal.h
*/

#ifndef __INCLUDE_EEMCUMICO32_SPI_INTERNAL_H__
#define __INCLUDE_EEMCUMICO32_SPI_INTERNAL_H__

#ifdef __USE_SPI__

#include "cpu/mico32/inc/ee_internal.h"
#include "mcu/mico32/inc/ee_internal.h"
#include "mcu/mico32/inc/ee_buffer.h"
#include "MicoSPI.h"

/*************************************************************************
 SPI
 *************************************************************************/

/****************************
 * SPI Register Structure
 ****************************/
typedef struct st_MicoSPI{
       volatile unsigned int rx;
       volatile unsigned int tx;
       volatile unsigned int status;
       volatile unsigned int control;
       volatile unsigned int sSelect;
}MicoSPI_t;
 
/* spi settings */
#define EE_SPI_MSGSIZE 				(1)		
#define EE_SPI_BUFSIZE 				(12)
#define EE_SLAVE_ID					(0x01)
#define EE_SPI_EVT_TX_DONE 			(0x01)
#define EE_SPI_EVT_RX_DONE 			(0x00)

/* spi bit masks */
#define EE_SPI_MASTER_MASK				(0x01)
#define EE_SPI_SLAVE_MASK				(0x00)

// status register masks 
#define EE_SPI_STATUS_RX_ERR_MASK      	(0x008)
#define EE_SPI_STATUS_TX_ERR_MASK      	(0x010)
#define EE_SPI_STATUS_TMT_RDY_MASK		(0x020)
#define EE_SPI_STATUS_TX_RDY_MASK      	(0x040)
#define EE_SPI_STATUS_RX_RDY_MASK      	(0x080)
// control-register masks 
#define EE_SPI_CTL_RX_ERR_INTR_EN_MASK	(0x008)
#define EE_SPI_CTL_TX_ERR_INTR_EN_MASK 	(0x010)
#define EE_SPI_CTL_TX_INTR_EN_MASK     	(0x040)
#define EE_SPI_CTL_RX_INTR_EN_MASK     	(0x080)
#define EE_SPI_CTL_RXTX_ERR_INTR_EN_MASK (0x100)
#define EE_SPI_CTL_ALL_INTR_DIS_MASK	(0xFFFFFE00)
#define EE_SPI_CTL_SSO_MASK				(0x400)

/* spi return values */			
#define EE_SPI_OK					(0)
#define EE_SPI_ERR_RECEPTION   		(-40)
#define EE_SPI_ERR_TRANSMISSION		(-41)
#define EE_SPI_ERR_BAD_VALUE		(-42)
#define EE_SPI_ERR_TX_NOT_READY		(-43)
#define EE_SPI_ERR_RX_NOT_READY		(-44)

/* spi operating modes */
#define EE_SPI_POLLING				(0x00)
#define EE_SPI_RX_ISR				(0x01)
#define EE_SPI_TX_ISR				(0x02)	
#define EE_SPI_RXTX_ISR				(0x03)	
#define EE_SPI_RX_BLOCK				(0x10)
#define EE_SPI_TX_BLOCK				(0x20)
#define EE_SPI_RXTX_BLOCK  			(0x30)	

/* Macros for tests */
#define EE_spi_need_init_rx_buf(old,new)	(!((old) & EE_SPI_RX_ISR) && ((new) & EE_SPI_RX_ISR))
#define EE_spi_need_init_tx_buf(old,new)	(!((old) & EE_SPI_TX_ISR) && ((new) & EE_SPI_TX_ISR))
#define EE_spi_need_enable_int(new)			( ((new) & EE_SPI_RX_ISR) || ((new) & EE_SPI_TX_ISR) )
#define EE_spi_tx_polling(mode)				( !((mode) & EE_SPI_TX_ISR) )
#define EE_spi_rx_polling(mode)				( !((mode) & EE_SPI_RX_ISR) )
//#define EE_spi_bus_idle(status)
//#define EE_spi_pend_for_TIP_done(status)
#define EE_spi_tx_ready(status)				(status & EE_SPI_STATUS_TX_RDY_MASK)
#define EE_spi_rx_ready(status)				(status & EE_SPI_STATUS_RX_RDY_MASK)
#define EE_spi_tmt_ready(status)			(status & EE_SPI_STATUS_TMT_RDY_MASK)
#define EE_spi_enable_tx_int(ctrl)			(ctrl |= EE_SPI_CTL_TX_INTR_EN_MASK)
#define EE_spi_enable_rx_int(ctrl)			(ctrl |= EE_SPI_CTL_RX_INTR_EN_MASK)
#define EE_spi_disable_tx_int(ctrl)			(ctrl &= (~EE_SPI_CTL_TX_INTR_EN_MASK))
#define EE_spi_disable_rx_int(ctrl)			(ctrl &= (~EE_SPI_CTL_RX_INTR_EN_MASK))
#define EE_spi_set_SSO(ctrl)				(ctrl |= EE_SPI_CTL_SSO_MASK)
#define EE_spi_clear_SSO(ctrl)				(ctrl &= (~EE_SPI_CTL_SSO_MASK))
//#define EE_spi_is_master(set)				(set & EE_SPI_MASTER_MASK)

/* Internal functions */

int EE_hal_spi_write_byte_polling(MicoSPI_t* spic, EE_UINT8 device, EE_UINT8 data);

int EE_hal_spi_read_byte_polling(MicoSPI_t* spic, EE_UINT8 device);

int EE_hal_spi_write_buffer_polling(MicoSPI_t* spic, EE_UINT8 device, EE_UINT8 address, EE_UINT8 *data, int len);

int EE_hal_spi_read_buffer_polling(MicoSPI_t* spic, EE_UINT8 device, EE_UINT8 address, EE_UINT8 *data, int len);

/* This function is used to set slave select register */
__INLINE__ int __ALWAYS_INLINE__ EE_hal_spi_set_slave(MicoSPI_t* spic, unsigned int mask)		
{
	//	if(EE_spi_is_master(set))
	spic->sSelect = mask;
	
    return EE_SPI_OK;
}

/* This function is used to get slave select register */
__INLINE__ int __ALWAYS_INLINE__ EE_hal_spi_get_slave(MicoSPI_t* spic, unsigned int *pmask)		
{
	//	if(EE_spi_is_master(set))
	if(pmask != 0)
        *pmask = spic->sSelect;

    return EE_SPI_OK;
}

#ifdef __USE_SPI_IRQ__
/*
	SPI structure (used in ISR mode):
*/
typedef struct {
	int err;							// last error condition
	int mode;							// spi operating mode (polling, isr, ...)
	int ir;								// flag to discriminate interrupt source (rx or tx)
    MicoSPI_t* base;					// controller base address
    int irqf;							// irq flag
    EE_ISR_callback rxcbk;				// rx callback
    EE_ISR_callback txcbk;				// tx callback
    EE_buffer rxbuf;					// rx buffer used in isr mode
    EE_buffer txbuf;					// tx buffer used in isr mode
} EE_spi_st;

/* Macro for Structure declaration */
#define DECLARE_STRUCT_SPI(uc, lc) \
  extern EE_spi_st EE_ST_NAME(lc);
  
/* Macro for structure definition */
#define DEFINE_STRUCT_SPI(uc, lc) \
EE_spi_st cat3(ee_, lc, _st) = { \
	.err=EE_SPI_OK, .mode= EE_SPI_POLLING | EE_SPI_RXTX_BLOCK, .base= (MicoSPI_t* )cat2(uc, _BASE_ADDRESS),\
	.irqf= cat2(uc, _IRQ), .rxcbk= EE_NULL_CBK, .txcbk= EE_NULL_CBK,\
	.rxbuf.data= EE_VETRX_NAME(lc),.txbuf.data= EE_VETTX_NAME(lc)};
/* Macro for vectors (buffers) definition */  
#define DEFINE_VET_SPI(uc, lc) \
EE_UINT8 EE_VETRX_NAME(lc)[EE_SPI_BUFSIZE]; \
EE_UINT8 EE_VETTX_NAME(lc)[EE_SPI_BUFSIZE];  

int EE_hal_spi_config(EE_spi_st* spisp, int settings);

int EE_hal_spi_set_mode(EE_spi_st* spisp, int mode);

int EE_hal_spi_handler_setup(EE_spi_st* spisp);

int EE_hal_spi_set_rx_ISR_callback(EE_spi_st* spisp, EE_ISR_callback isr_rx_callback);

int EE_hal_spi_set_tx_ISR_callback(EE_spi_st* spisp, EE_ISR_callback isr_tx_callback);
															
int EE_hal_spi_write_byte_irq(EE_spi_st* spisp, EE_UINT8 device, EE_UINT8 data);

int EE_hal_spi_read_byte_irq(EE_spi_st* spisp, EE_UINT8 device);
			
int EE_hal_spi_write_buffer_irq(EE_spi_st* spisp, EE_UINT8 device, EE_UINT8 address, EE_UINT8 *data, int len);
	
int EE_hal_spi_read_buffer_irq(EE_spi_st* spisp, EE_UINT8 device, EE_UINT8 address, EE_UINT8 *data, int len);

int EE_hal_spi_return_error(EE_spi_st* spisp);

__INLINE__ int __ALWAYS_INLINE__ EE_hal_spi_enable_IRQ(EE_spi_st *spisp)
{
	MicoSPI_t* spic = spisp->base;
	
	spic->control = spisp->mode;
	
	return EE_SPI_OK;
}

__INLINE__ int __ALWAYS_INLINE__ EE_hal_spi_disable_IRQ(EE_spi_st *spisp)
{
	MicoSPI_t* spic = spisp->base;
	
	spic->control &= EE_SPI_CTL_ALL_INTR_DIS_MASK;
	
	return EE_SPI_OK;
}

__INLINE__ int __ALWAYS_INLINE__ EE_hal_spi_enable(EE_spi_st *spisp)
{
	MicoSPI_t* spic = spisp->base;
	
	spic->control = spisp->mode;
	
	return EE_SPI_OK;
}

__INLINE__ int __ALWAYS_INLINE__ EE_hal_spi_disable(EE_spi_st *spisp)
{
	MicoSPI_t* spic = spisp->base;
	
	while( !EE_spi_tmt_ready(spic->status) );
	spic->control = 0;
	
	return EE_SPI_OK;
}

/* Interrupt handler */
void EE_spi_common_handler(int level);																	
							
/* Macros for User functions (API) */  
#define DECLARE_FUNC_SPI(uc, lc) \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _config)(int settings){ \
	return EE_hal_spi_config(& EE_ST_NAME(lc), settings); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _set_mode)(int mode){ \
	return EE_hal_spi_set_mode(& EE_ST_NAME(lc), mode); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _set_rx_ISR_callback)(EE_ISR_callback rxcbk){ \
	return EE_hal_spi_set_rx_ISR_callback(& EE_ST_NAME(lc), rxcbk); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _set_tx_ISR_callback)(EE_ISR_callback txcbk){ \
	return EE_hal_spi_set_tx_ISR_callback(& EE_ST_NAME(lc), txcbk); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _send_byte)(EE_UINT8 device, EE_UINT8 data){ \
	int ret; \
	if(EE_spi_tx_polling(EE_ST_NAME(lc).mode))\
		ret = EE_hal_spi_write_byte_polling((MicoSPI_t*)EE_BASE_ADD(uc), device, data); \
	else \
		ret = EE_hal_spi_write_byte_irq(& EE_ST_NAME(lc), device, data); \
	return ret; } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _receive_byte)(EE_UINT8 device){ \
	int ret; \
	if(EE_spi_rx_polling(EE_ST_NAME(lc).mode))\
		ret = EE_hal_spi_read_byte_polling((MicoSPI_t*)EE_BASE_ADD(uc), device); \
	else \
		ret = EE_hal_spi_read_byte_irq(& EE_ST_NAME(lc), device); \
	return ret; } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _send_buffer)(EE_UINT8 device, EE_UINT8 address, EE_UINT8* vet, int len){ \
	int ret; \
	if(EE_spi_tx_polling(EE_ST_NAME(lc).mode))\
		ret = EE_hal_spi_write_buffer_polling((MicoSPI_t*)EE_BASE_ADD(uc), device, address, vet, len); \
	else \
		ret = EE_hal_spi_write_buffer_irq(& EE_ST_NAME(lc), device, address, vet, len); \
	return ret; } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _receive_buffer)(EE_UINT8 device, EE_UINT8 address, EE_UINT8* vet, int len){ \
	int ret; \
	if(EE_spi_rx_polling(EE_ST_NAME(lc).mode))\
		ret = EE_hal_spi_read_buffer_polling((MicoSPI_t*)EE_BASE_ADD(uc), device, address, vet, len); \
	else \
		ret = EE_hal_spi_read_buffer_irq(& EE_ST_NAME(lc), device, address, vet, len); \
	return ret; } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _get_slave)(unsigned int *pmask){ \
	return EE_hal_spi_get_slave((MicoSPI_t*)EE_BASE_ADD(uc), pmask); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _set_slave)(unsigned int mask){ \
	return EE_hal_spi_set_slave((MicoSPI_t*)EE_BASE_ADD(uc), mask); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _return_error)(void){ \
	return EE_hal_spi_return_error(& EE_ST_NAME(lc)); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _enable_IRQ)(void){ \
	return EE_hal_spi_enable_IRQ(& EE_ST_NAME(lc)); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _disable_IRQ)(void){ \
	return EE_hal_spi_disable_IRQ(& EE_ST_NAME(lc)); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _enable)(void){ \
	return EE_hal_spi_enable(& EE_ST_NAME(lc)); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _disable)(void){ \
	return EE_hal_spi_disable(& EE_ST_NAME(lc)); }
	
/* User functions (API): */  
#ifdef EE_SPI1_NAME_UC
DECLARE_STRUCT_SPI(EE_SPI1_NAME_UC, EE_SPI1_NAME_LC)
#endif	//#ifdef EE_SPI1_NAME_UC

#ifdef EE_SPI2_NAME_UC
DECLARE_STRUCT_SPI(EE_SPI2_NAME_UC, EE_SPI2_NAME_LC)
#endif	//#ifdef EE_SPI2_NAME_UC
	
/* Return the SPI structure for the componente associated with the given IRQ
 * level */
__DECLARE_INLINE__ EE_spi_st *EE_get_spi_st_from_level(int level);
#ifndef EE_SPI2_NAME_UC
/* If there is only one component of this kind, no test is done */
__INLINE__ EE_spi_st * __ALWAYS_INLINE__ EE_get_spi_st_from_level(int level)
{
    return & EE_ST_NAME(EE_SPI1_NAME_LC);
}
#else /* #ifndef EE_SPI2_NAME_UC */
__INLINE__ EE_spi_st * __ALWAYS_INLINE__ EE_get_spi_st_from_level(int level)
{
    if (level == EE_IRQ_NAME(EE_SPI1_NAME_UC))
        return & EE_ST_NAME(EE_SPI1_NAME_LC);
    else
        return & EE_ST_NAME(EE_SPI2_NAME_LC);
}
#endif /* #ifndef EE_SPI2_NAME_UC */	
	
		
#else //#ifdef __USE_SPI_IRQ__

///*
//	SPI structure (used in polling mode):
//*/
//typedef struct {
//	int err;							// last error condition
//	int mode;							// spi operating mode (polling, isr, ...)
//} EE_spi_st;
	
int EE_hal_spi_config(MicoSPI_t* spic, int settings);

int EE_hal_spi_set_mode(MicoSPI_t* spic, int mode);
	
/* This function is used to turn off spi controller */
__INLINE__ int __ALWAYS_INLINE__ EE_hal_spi_enable(MicoSPI_t* spic)
{
	// to do...
	
	return EE_SPI_OK;
}

__INLINE__ int __ALWAYS_INLINE__ EE_hal_spi_disable(MicoSPI_t* spic)
{
	while( !EE_spi_tmt_ready(spic->status) );
	spic->control = 0;
	
	return EE_SPI_OK;
}

#define DECLARE_FUNC_SPI(uc, lc) \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _config)(int settings){ \
	return EE_hal_spi_config((MicoSPI_t*)EE_BASE_ADD(uc), settings); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _set_mode)(int mode){ \
	return EE_hal_spi_set_mode((MicoSPI_t*)EE_BASE_ADD(uc), mode); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _send_byte)(EE_UINT8 device, EE_UINT8 data){ \
	return EE_hal_spi_write_byte_polling((MicoSPI_t*)EE_BASE_ADD(uc), device, data); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _receive_byte)(EE_UINT8 device){ \
	return EE_hal_spi_read_byte_polling((MicoSPI_t*)EE_BASE_ADD(uc), device); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _send_buffer)(EE_UINT8 device, EE_UINT8 address, EE_UINT8* vet, int len){ \
	return EE_hal_spi_write_buffer_polling((MicoSPI_t*)EE_BASE_ADD(uc), device, address, vet, len); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _receive_buffer)(EE_UINT8 device, EE_UINT8 address, EE_UINT8* vet, int len){ \
	return EE_hal_spi_read_buffer_polling((MicoSPI_t*)EE_BASE_ADD(uc), device, address, vet, len); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _disable)(void){ \
	return EE_hal_spi_disable((MicoSPI_t*)EE_BASE_ADD(uc)); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _enable)(void){ \
	return EE_hal_spi_enable((MicoSPI_t*)EE_BASE_ADD(uc)); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _set_slave)(unsigned int mask){ \
	return EE_hal_spi_set_slave((MicoSPI_t*)EE_BASE_ADD(uc), mask); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _get_slave)(unsigned int *pmask ){ \
	return EE_hal_spi_get_slave((MicoSPI_t*)EE_BASE_ADD(uc), pmask); }
	
#endif //#ifdef __USE_SPI_IRQ__

/* User functions (API) declaration: */  
#ifdef EE_SPI1_NAME_UC
DECLARE_FUNC_SPI(EE_SPI1_NAME_UC, EE_SPI1_NAME_LC)
#endif	//#ifdef EE_SPI1_NAME_UC

#ifdef EE_SPI2_NAME_UC
DECLARE_FUNC_SPI(EE_SPI2_NAME_UC, EE_SPI2_NAME_LC)
#endif	//#ifdef EE_SPI2_NAME_UC

/* Macros for compatibility with pic32 SPI driver*/ 
#ifdef __USE_MICO_PIC_API__
#ifdef __USE_SPI_IRQ__
#define EE_mchp_spi_init(lc, flags)			EE_hal_spi_config(& EE_ST_NAME(lc), flags)
#else
#define EE_mchp_spi_init(uc, flags)			EE_hal_spi_config((MicoSPI_t *)EE_BASE_ADD(uc), flags)
#endif
#define EE_mchp_spi_set_tx_cbk(lc, cbk) 	EE_hal_spi_set_tx_ISR_callback(& EE_ST_NAME(lc), cbk)
#define EE_mchp_spi_set_rx_cbk(lc, cbk) 	EE_hal_spi_set_rx_ISR_callback(& EE_ST_NAME(lc), cbk)
#define EE_mchp_spi_write(uc, id, add, data, len)		EE_hal_spi_write_buffer_polling((MicoSPI_t *)EE_BASE_ADD(uc), id, add, data, len)
#define EE_mchp_spi_read(uc, id, add, data, len)		EE_hal_spi_read_buffer_polling((MicoSPI_t *)EE_BASE_ADD(uc), id, add, data, len)

__INLINE__ EE_INT8 __ALWAYS_INLINE__ EE_spi_init(EE_UINT8 port, EE_UINT32 baudrate, EE_UINT16 flags)
{
	EE_INT8 ret;
	
	#if defined(EE_SPI1_NAME_UC) && defined(EE_SPI2_NAME_UC)
	if(port==1)
		#ifdef __USE_SPI_IRQ__
		ret = EE_mchp_spi_init(EE_SPI1_NAME_LC, flags);
		#else
		ret = EE_mchp_spi_init(EE_SPI1_NAME_UC, flags);
		#endif
	else
		#ifdef __USE_SPI_IRQ__
		ret = EE_mchp_spi_init(EE_SPI2_NAME_LC, flags);
		#else
		ret = EE_mchp_spi_init(EE_SPI2_NAME_UC, flags);
		#endif
	#else
		#if defined(EE_SPI1_NAME_UC)
		#ifdef __USE_SPI_IRQ__
		ret = EE_mchp_spi_init(EE_SPI1_NAME_LC, flags);
		#else
		ret = EE_mchp_spi_init(EE_SPI1_NAME_UC, flags);
		#endif
		#else
		#ifdef __USE_SPI_IRQ__
		ret = EE_mchp_spi_init(EE_SPI2_NAME_LC, flags);
		#else
		ret = EE_mchp_spi_init(EE_SPI2_NAME_UC, flags);
		#endif
		#endif
	#endif
		
	return ret;
}

__INLINE__ EE_INT8 __ALWAYS_INLINE__ EE_spi_close(EE_UINT8 port)
{
	// not used on LM32 device...
	return EE_SPI_OK;
}

#ifdef __USE_SPI_IRQ__
__INLINE__ EE_INT8 __ALWAYS_INLINE__ EE_spi_set_callback(EE_UINT8 port, EE_UINT8 event, void (*f)(void))
{
	EE_INT8 ret;
	
	#if defined(EE_SPI1_NAME_UC) && defined(EE_SPI2_NAME_UC)
	if(port==1)
		if(event == EE_SPI_EVT_RX_DONE)
			ret = EE_mchp_spi_set_rx_cbk(EE_SPI1_NAME_LC, f);
		else
			ret = EE_mchp_spi_set_tx_cbk(EE_SPI1_NAME_LC, f);
	else
		if(event == EE_SPI_EVT_RX_DONE)
			ret = EE_mchp_spi_set_rx_cbk(EE_SPI2_NAME_LC, f);
		else
			ret = EE_mchp_spi_set_tx_cbk(EE_SPI2_NAME_LC, f); 
	#else
		#if defined(EE_SPI1_NAME_UC)
		if(event == EE_SPI_EVT_RX_DONE)
			ret = EE_mchp_spi_set_rx_cbk(EE_SPI1_NAME_LC, f);
		else
			ret = EE_mchp_spi_set_tx_cbk(EE_SPI1_NAME_LC, f);
		#else
		if(event == EE_SPI_EVT_RX_DONE)
			ret = EE_mchp_spi_set_rx_cbk(EE_SPI2_NAME_LC, f);
		else
			ret = EE_mchp_spi_set_tx_cbk(EE_SPI2_NAME_LC, f); 
		#endif
	#endif
		
	return ret;
}
#else	//#ifdef __USE_SPI_IRQ__
__INLINE__ EE_INT8 __ALWAYS_INLINE__ EE_spi_set_callback(EE_UINT8 port, EE_UINT8 event, void (*f)(void))
{
	// not used in polling mode...
	return 0;
}
#endif 	//#ifdef __USE_SPI_IRQ__

__INLINE__ EE_INT8 __ALWAYS_INLINE__ EE_spi_write(EE_UINT8 port, EE_UINT8 *data, EE_UINT32 len)
{
	EE_INT8 ret;
	
	#if defined(EE_SPI1_NAME_UC) && defined(EE_SPI2_NAME_UC)
	if(port==1)
		ret = EE_mchp_spi_write(EE_SPI1_NAME_UC, EE_SLAVE_ID, data[0], &data[1], len);
	else
		ret = EE_mchp_spi_write(EE_SPI2_NAME_UC, EE_SLAVE_ID, data[0], &data[1], len);
	#else
		#if defined(EE_SPI1_NAME_UC)
		ret = EE_mchp_spi_write(EE_SPI1_NAME_UC, EE_SLAVE_ID, data[0], &data[1], len);
		#else
		ret = EE_mchp_spi_write(EE_SPI2_NAME_UC, EE_SLAVE_ID, data[0], &data[1], len);
		#endif
	#endif
		
	return ret;
}

__INLINE__ EE_INT8 __ALWAYS_INLINE__ EE_spi_read(EE_UINT8 port, EE_UINT8 *data, EE_UINT32 len)
{
	EE_INT8 ret;
	
	#if defined(EE_SPI1_NAME_UC) && defined(EE_SPI2_NAME_UC)
	if(port==1)
		ret = EE_mchp_spi_read(EE_SPI1_NAME_UC, EE_SLAVE_ID, data[0], &data[1], len);
	else
		ret = EE_mchp_spi_read(EE_SPI2_NAME_UC, EE_SLAVE_ID, data[0], &data[1], len);
	#else
		#if defined(EE_SPI1_NAME_UC)
		ret = EE_mchp_spi_read(EE_SPI1_NAME_UC, EE_SLAVE_ID, data[0], &data[1], len);
		#else
		ret = EE_mchp_spi_read(EE_SPI2_NAME_UC, EE_SLAVE_ID, data[0], &data[1], len);
		#endif
	#endif
		
	return ret;
}

__INLINE__ EE_INT8 __ALWAYS_INLINE__ EE_spi_cycle(EE_UINT8 port, EE_UINT8 *data_out, EE_UINT8 *data_in, EE_UINT32 len)
{
	// not implemented...
	return 0;
}

#endif // #ifdef __USE_MICO_PIC_API__

#endif //#ifdef __USE_SPI__

#endif //__INCLUDE_EEMCUMICO32_SPI_INTERNAL_H__