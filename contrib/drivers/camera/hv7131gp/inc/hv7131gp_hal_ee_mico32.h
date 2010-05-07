/*
Copyright (C) 2009 -  Claudio Salvadori and Christian Nastasi
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

/*
  Name: hv7131gp_hal_ee_mico32.h
  Copyright: Evidence Srl
  Author: Dario Di Stefano
  Date: 29/03/10 18.28
  Description: camera driver hal for mico32. 
*/

#ifndef HV7131GP_HAL_EE_MICO32_H_
#define HV7131GP_HAL_EE_MICO2_H_

/* Required header files */
#include "hv7131gp.h"
#include "cpu/mico32/inc/ee_internal.h"
#include "mcu/mico32/inc/ee_internal.h"
#include <cpu/mico32/inc/ee_irq.h>
/* Other used libraries: */
#include "mcu/mico32/inc/ee_i2c.h"
#include "MicoUtils.h"
#ifdef __USE_MICO32BOARD_CAMERA_HV7131GP__
#include "board/fpga_camera_mico32/inc/ee_camera_hv7131gp.h"
//#include "board/fpga_camera_mico32/inc/ee_camera_hv7131gp_reg.h"
#endif

/******************************************************************************/
/*                         MicoCamera.h                                       */
/******************************************************************************/

/* Camera controller registers */
typedef struct st_MicoCamera{
	volatile void* addr;
	volatile unsigned int state;
	volatile unsigned int flag;
    volatile unsigned int cntr1;
    volatile unsigned int cntr2;
    volatile unsigned int div;
}MicoCamera_t;

/* Camera controller bit masks */
#define MICO_CAM_IF_FLAG_MASK 		0x0001				/* bit 1 in FLAG register */
#define MICO_CAM_ERR_FLAG_MASK 		0x0002				/* bit 2 in FLAG register */
#define MICO_CAM_START_MASK			0x0001				/* bit 1 in CNTR1 register */
#define MICO_CAM_IFACK_MASK			0x0002				/* bit 2 in CNTR1 register */
#define MICO_CAM_RESET_MASK			0x0004				/* bit 3 in CNTR1 register */
#define MICO_CAM_ENABLE_IRQ_MASK	0x0001				/* bit 1 in CNTR2 register */

/* MICO CAMERA Macros */

//#define Mico_camera_read_address(cam)			(((MicoCamera_t*)cam)->addr)
__INLINE__ void* __ALWAYS_INLINE__ Mico_camera_read_address(MicoCamera_t* cam)
{
	return (void*)cam->addr;	
}

//#define Mico_camera_write_address(cam, address)	(((MicoCamera_t*)cam)->addr = address)
__INLINE__ int __ALWAYS_INLINE__ Mico_camera_write_address(MicoCamera_t* cam, void* address)
{
	cam->addr = address;
	return 0;	
}

//#define Mico_camera_read_status_register(cam)	(((MicoCamera_t*)cam)->state)
__INLINE__ int __ALWAYS_INLINE__ Mico_camera_read_status_register(MicoCamera_t* cam)
{
	return cam->state;
}

//#define Mico_camera_read_IF_flag(cam)			(((MicoCamera_t*)cam)->flag & MICO_CAM_IF_FLAG_MASK)
__INLINE__ int __ALWAYS_INLINE__ Mico_camera_read_IF_flag(MicoCamera_t* cam)
{
	return cam->flag & MICO_CAM_IF_FLAG_MASK;
}

//#define Mico_camera_read_ERR_flag(cam)			(((MicoCamera_t*)cam)->flag & MICO_CAM_ERR_FLAG_MASK)
__INLINE__ int __ALWAYS_INLINE__ Mico_camera_read_ERR_flag(MicoCamera_t* cam)
{
	return cam->flag & MICO_CAM_ERR_FLAG_MASK;
}

//#define Mico_camera_start(cam)					(((MicoCamera_t*)cam)->cntr1 = MICO_CAM_START_MASK)
__INLINE__ int __ALWAYS_INLINE__ Mico_camera_start(MicoCamera_t* cam)
{
	return cam->cntr1 = MICO_CAM_START_MASK;
}

//#define Mico_camera_clear_IRQ_flag(cam)			(((MicoCamera_t*)cam)->cntr1 = 0)
__INLINE__ int __ALWAYS_INLINE__ Mico_camera_clear_IRQ_flag(MicoCamera_t* cam)
{
	cam->cntr1 = 0;
	return 0;
}

//#define Mico_camera_reset(cam)					(((MicoCamera_t*)cam)->cntr1 = MICO_CAM_RESET_MASK)
__INLINE__ int __ALWAYS_INLINE__ Mico_camera_reset(MicoCamera_t* cam)
{
	cam->cntr1 = MICO_CAM_RESET_MASK;
	return 0;
}

//#define Mico_camera_enable_IRQ(cam)				(((MicoCamera_t*)cam)->cntr2 = MICO_CAM_ENABLE_IRQ_MASK)
__INLINE__ int __ALWAYS_INLINE__ Mico_camera_enable_IRQ(MicoCamera_t* cam)
{
	cam->cntr2 = MICO_CAM_ENABLE_IRQ_MASK;
	return 0;
}

//#define Mico_camera_disable_IRQ(cam)			(((MicoCamera_t*)cam)->cntr2 = 0)
__INLINE__ int __ALWAYS_INLINE__ Mico_camera_disable_IRQ(MicoCamera_t* cam)
{
	cam->cntr2 = 0;
	return 0;
}

//#define Mico_camera_IRQ_enabled(cam)			(((MicoCamera_t*)cam)->cntr2 & MICO_CAM_ENABLE_IRQ_MASK)
__INLINE__ int __ALWAYS_INLINE__ Mico_camera_IRQ_enabled(MicoCamera_t* cam)
{
	return cam->cntr2 & MICO_CAM_ENABLE_IRQ_MASK;
}

//#define Mico_camera_write_divisor(cam, divisor) (((MicoCamera_t*)cam)->div = divisor)
__INLINE__ int __ALWAYS_INLINE__ Mico_camera_write_divisor(MicoCamera_t* cam, int divisor)
{
	cam->div = divisor;
	return 0;
}

//#define Mico_camera_read_divisor(cam)			(((MicoCamera_t*)cam)->div)
__INLINE__ int __ALWAYS_INLINE__ Mico_camera_read_divisor(MicoCamera_t* cam)
{
	return cam->div;
}

//#define Mico_camera_frame_terminated(cam)		(((MicoCamera_t*)cam)->flag & MICO_CAM_IF_FLAG_MASK)
__INLINE__ int __ALWAYS_INLINE__ Mico_camera_frame_terminated(MicoCamera_t* cam)
{
	return cam->flag & MICO_CAM_IF_FLAG_MASK;
}

/******************************************************************************/
/*                        Symbols and macros                                  */
/******************************************************************************/

/* Symbols and macros */
#define EE_HV7131GP_DEVICE_ID				(HV7131GP_DEVICE_ID)
#define EE_HV7131GP_ISR_MODE				(0x01)
#define EE_HV7131GP_POLLING_MODE			(0x00)
#define EE_hv7131gp_need_enable_int(mode)	(mode)

/* Default configuration */
#define EE_HV7131GP_DEFAULT_FORMAT			(HV7131GP_OUTFMT_DEFAULT & (~HV7131GP_8BIT_OUTPUT))
#define EE_HV7131GP_DEFAULT_DIVISOR 		(HV7131GP_T_1)
#define EE_HV7131GP_DEFAULT_RESOLUTION 		(HV7131GP_1_16_SUB)
#define EE_HV7131GP_DEFAULT_WIDTH 			(640)
#define EE_HV7131GP_DEFAULT_HEIGTH 			(480)
#define EE_HV7131GP_DEFAULT_COLUMN 			(0x2)
#define EE_HV7131GP_DEFAULT_ROW 			(0x2)
#define EE_HV7131GP_DEFAULT_VBLANK			(8)
#define EE_HV7131GP_DEFAULT_HBLANK			(208)
#define EE_HV7131GP_DEFAULT_FRAME_WIDTH		(160)
#define EE_HV7131GP_DEFAULT_FRAME_HEIGHT	(120)

#if(0)
/* Exit status for the EE_CAMERA driver 
	(see EE_CAMERA_init EE_CAMERA_configure EE_CAMERA_capture EE_CAMERA_reg_write EE_CAMERA_reg_read)
*/
#define	HV7131GP_SUCCESS 					(0)		/**< No error */
#define	HV7131GP_FAILURE					(-19)	/**< Generic error */
#define	HV7131GP_ERR_I2C_INIT				(-20)	/**< I2C bus initialization error */
#define	HV7131GP_ERR_I2C_READ				(-21)	/**< I2C bus read error */
#define	HV7131GP_ERR_I2C_WRITE				(-22)	/**< I2C bus write error */
#define	HV7131GP_ERR_DEV_NOINIT				(-23)	/**< Device is not initialized */
#define	HV7131GP_ERR_MEM_NULL				(-24)	/**< Invalid NULL memory address */
/* Resolution value */
#define	HV7131GP_NO_SUB  					(HV7131GP_VIDEO_NORMAL)		/**< 640x480 pixel resolution*/
#define	HV7131GP_1_4_SUB					(HV7131GP_VIDEO_SUB_4)		/**< 320x240 pixel resolution*/
#define	HV7131GP_1_16_SUB					(HV7131GP_VIDEO_SUB_16)		/**< 160x120 pixel resolution */
/* Time divisor */
#define	HV7131GP_T_1  						(HV7131GP_DCF_1)		/**< Divide MCLK for 1 */
#define	HV7131GP_T_2						(HV7131GP_DCF_2)		/**< Divide MCLK for 2 */
#define	HV7131GP_T_3						(HV7131GP_DCF_4)		/**< Divide MCLK for 4 */
#define	HV7131GP_T_4						(HV7131GP_DCF_8)		/**< Divide MCLK for 8 */
#define	HV7131GP_T_5						(HV7131GP_DCF_16)		/**< Divide MCLK for 16 */
#define	HV7131GP_T_6						(HV7131GP_DCF_32)		/**< Divide MCLK for 32 */
#define	HV7131GP_T_7						(HV7131GP_DCF_64)		/**< Divide MCLK for 64 */
#define	HV7131GP_T_8						(HV7131GP_DCF_128)		/**< Divide MCLK for 128 */
/* Quality value (high level) */
#define	HV7131GP_160x120_FAST  				(HV7131GP_T_2)		/**< 160x120 + T2 divisor*/
#define	HV7131GP_160x120_MEDIUM				(HV7131GP_T_5)		/**< 160x120 + T5 divisor*/
#define	HV7131GP_160x120_SLOW				(HV7131GP_T_8)		/**< 160x120 + T8 divisor */
#endif // #if(0)

/******************************************************************************/
/*                         Functions Prototypes                               */
/******************************************************************************/

/* Callback function called by camera driver handler  */
extern EE_ISR_callback ee_hv7131gp_cbk;

/* Functions */
hv7131gp_status_t hv7131gp_hal_init(uint8_t DMA_CH);
void hv7131gp_oc_hal_init(uint16_t period);
void hv7131gp_oc_hal_start(void);
void hv7131gp_oc_hal_stop(void);
void hv7131gp_hal_delay_us(uint32_t delay_count);
hv7131gp_status_t hv7131gp_i2c_hal_init(void);
hv7131gp_status_t hv7131gp_i2c_hal_reg_write(hv7131gp_reg_t reg, uint8_t  val);
hv7131gp_status_t hv7131gp_i2c_hal_reg_read(hv7131gp_reg_t reg, uint8_t *val);
hv7131gp_status_t hv7131gp_dma_hal_init(uint8_t dma_ch);
	
/* Macros for Camera functions */  
#define DECLARE_FUNC_HV7131GP(uc, lc) \
__INLINE__ int __ALWAYS_INLINE__ EE_hv7131gp_init(int mode){ \
	if(mode) \
		return EE_hv7131gp_ISR_init((MicoCamera_t*)EE_BASE_ADD(uc), (int)EE_IRQ_NAME(uc)); \
	else \
		return EE_hv7131gp_polling_init((MicoCamera_t*)EE_BASE_ADD(uc)); } \
__INLINE__ void* __ALWAYS_INLINE__ EE_camera_read_address(void){ \
	return Mico_camera_read_address((MicoCamera_t*)EE_BASE_ADD(uc)); } \
__INLINE__ void __ALWAYS_INLINE__ EE_camera_write_address(void* addr){ \
	Mico_camera_write_address((MicoCamera_t*)EE_BASE_ADD(uc),addr); } \
__INLINE__ int __ALWAYS_INLINE__ EE_camera_read_status_register(void) {\
	return Mico_camera_read_status_register((MicoCamera_t*)EE_BASE_ADD(uc)); } \
__INLINE__ int __ALWAYS_INLINE__ EE_camera_read_IF_flag(void){ \
	return Mico_camera_read_IF_flag((MicoCamera_t*)EE_BASE_ADD(uc)); } \
__INLINE__ int __ALWAYS_INLINE__ EE_camera_read_ERR_flag(void){ \
	return Mico_camera_read_ERR_flag((MicoCamera_t*)EE_BASE_ADD(uc)); } \
__INLINE__ void __ALWAYS_INLINE__ EE_camera_start(void){ \
	Mico_camera_start((MicoCamera_t*)EE_BASE_ADD(uc)); } \
__INLINE__ void __ALWAYS_INLINE__ EE_camera_clear_IRQ_flag(void){ \
	Mico_camera_clear_IRQ_flag((MicoCamera_t*)EE_BASE_ADD(uc)); } \
__INLINE__ void __ALWAYS_INLINE__ EE_camera_reset(void){ \
	Mico_camera_reset((MicoCamera_t*)EE_BASE_ADD(uc)); } \
__INLINE__ void __ALWAYS_INLINE__ EE_camera_enable_IRQ(void){ \
	Mico_camera_enable_IRQ((MicoCamera_t*)EE_BASE_ADD(uc)); } \
__INLINE__ void __ALWAYS_INLINE__ EE_camera_disable_IRQ(void){ \
	Mico_camera_disable_IRQ((MicoCamera_t*)EE_BASE_ADD(uc)); } \
__INLINE__ int __ALWAYS_INLINE__ EE_camera_IRQ_enabled(void){ \
	return Mico_camera_IRQ_enabled((MicoCamera_t*)EE_BASE_ADD(uc)); } \
__INLINE__ void __ALWAYS_INLINE__ EE_camera_write_divisor(unsigned int div){ \
	Mico_camera_write_divisor((MicoCamera_t*)EE_BASE_ADD(uc), div); } \
__INLINE__ int __ALWAYS_INLINE__ EE_camera_read_divisor(void){ \
	return Mico_camera_read_divisor((MicoCamera_t*)EE_BASE_ADD(uc)); } \
__INLINE__ int __ALWAYS_INLINE__ EE_camera_frame_terminated(void){ \
	return Mico_camera_frame_terminated((MicoCamera_t*)EE_BASE_ADD(uc)); }

/* Macros for I2C-based Camera functions */  
#define DECLARE_FUNC_I2C_HV7131GP(uc, lc) \
__INLINE__ int __ALWAYS_INLINE__ EE_hv7131gp_i2c_config(int baudrate,int settings){ \
	return cat3(EE_, lc, _config)(baudrate, settings); } \
__INLINE__ int __ALWAYS_INLINE__  EE_hv7131gp_i2c_write_byte(EE_UINT8 address, EE_UINT8 data){ \
	return cat3(EE_, lc, _send_byte)(EE_HV7131GP_DEVICE_ID, address, data); } \
__INLINE__ int __ALWAYS_INLINE__  EE_hv7131gp_i2c_write_buffer(EE_UINT8 address, EE_UINT8 *data, int len){ \
	return cat3(EE_, lc, _send_buffer)(EE_HV7131GP_DEVICE_ID, address, data, len); } \
__INLINE__ int __ALWAYS_INLINE__  EE_hv7131gp_i2c_read_byte(EE_UINT8 address){ \
	return cat3(EE_, lc, _receive_byte)(EE_HV7131GP_DEVICE_ID, address); } \
__INLINE__ int __ALWAYS_INLINE__  EE_hv7131gp_i2c_read_buffer(EE_UINT8 address, EE_UINT8 *data, int len){ \
	return cat3(EE_, lc, _receive_buffer)(EE_HV7131GP_DEVICE_ID, address, data, len); }

/* Camera initialization */
int EE_hv7131gp_ISR_init(MicoCamera_t* cam, int irqf);
int EE_hv7131gp_polling_init(MicoCamera_t* cam);

/* Set configuration */
int EE_hv7131gp_set_configuration(int div, int fmt, int res, int w, int h, int x, int y, int hb, int vb);

/* Set default configuration */
int EE_hv7131gp_set_default_configuration(void);

/* Set width */
int EE_hv7131gp_set_width(int width);

/* Get width */
int EE_hv7131gp_get_width(void);

/* Set height */
int EE_hv7131gp_set_height(int height);

/* Get height */
int EE_hv7131gp_get_height(void);

/* Get size */
int EE_hv7131gp_get_size(void);

/* Set x position */
int EE_hv7131gp_set_xpos(int x);

/* Get x position */
int EE_hv7131gp_get_xpos(void);

/* Set y position */
int EE_hv7131gp_set_ypos(int y);

/* Get y position */
int EE_hv7131gp_get_ypos(void);

/* Set hblank */
int EE_hv7131gp_set_hblank(int hb);

/* Get hblank */
int EE_hv7131gp_get_hblank(void);

/* Set vblank */
int EE_hv7131gp_set_vblank(int vb);

/* Get vblank */
int EE_hv7131gp_get_vblank(void);

/* Set window */
int EE_hv7131gp_set_window(int width, int height, int x, int y);

/* Capture an image */
int EE_hv7131gp_capture(void *image, EE_ISR_callback cam_cbk);

/* Set sleep mode */
int EE_hv7131gp_set_sleep_status(void);

/* Set active mode */
int EE_hv7131gp_set_active_status(void);

DECLARE_FUNC_I2C_HV7131GP(CAMERA_I2C, camera_i2c)
DECLARE_FUNC_HV7131GP(CAMERA, camera)

/* Set time divisor */
__INLINE__ int __ALWAYS_INLINE__  EE_hv7131gp_set_time_divisor(int div)
{
	return EE_hv7131gp_i2c_write_byte(HV7131GP_REG_SCTRB, div);
}

/* Set format */
__INLINE__ int __ALWAYS_INLINE__ EE_hv7131gp_set_format(EE_UINT8 val)
{
	//HV7131GP_OUTFMT_DEFAULT & (~HV7131GP_8BIT_OUTPUT)
	return EE_hv7131gp_i2c_write_byte(HV7131GP_REG_OUTFMT, val);
}

/* Get format */
__INLINE__ int __ALWAYS_INLINE__ EE_hv7131gp_get_format(void)
{
	//HV7131GP_OUTFMT_DEFAULT & (~HV7131GP_8BIT_OUTPUT)
	return EE_hv7131gp_i2c_read_byte(HV7131GP_REG_OUTFMT);
}

/* Set resolution */
__INLINE__ int __ALWAYS_INLINE__ EE_hv7131gp_set_resolution(EE_UINT8 val)
{
	//HV7131GP_REG_SCTRA, res | HV7131GP_X_FLIP
	return EE_hv7131gp_i2c_write_byte(HV7131GP_REG_SCTRA, val);
}

/* Get resolution */
__INLINE__ int __ALWAYS_INLINE__ EE_hv7131gp_get_resolution(void)
{
	//HV7131GP_REG_SCTRA, res | HV7131GP_X_FLIP
	return EE_hv7131gp_i2c_read_byte(HV7131GP_REG_SCTRA);
}

/* Get Y average */
__INLINE__ int __ALWAYS_INLINE__ EE_hv7131gp_get_Y_average(void)
{
	return EE_hv7131gp_i2c_read_byte(HV7131GP_REG_YFMEAN);
}


/******************************************************************************/
/*                           Hardware Abstraction Layer                       */
/******************************************************************************/
#define EE_HV7131GP_DEVICE_ID				(HV7131GP_DEVICE_ID)

//Main clock speed
#define SYSTEM_INSTRUCTION_CLOCK	80000000ul
#define	HV7131GP_MCLK_PERIOD		0x02
#define HV7131GP_MAX_SIZE			19200	/**< For MICOMX */

/*
// GPIO settings
*/

/**
* @name Enable pin
* \brief Settings for D2 pin used like camera enable pin (see HV7131GP documentation)
*/
#define HV7131GP_PIN_ENABLE_INIT()	/* Init enable pin */
#define HV7131GP_PIN_ENABLE_HIGH()	/* Set pin to high value*/
#define HV7131GP_PIN_ENABLE_LOW()	/* Set pin to low value*/
/**  @} */

/**
* @name Reset pin
* \brief Settings for D1 pin used like camera reset pin (see HV7131GP documentation)
*/
#define HV7131GP_PIN_RESETB_INIT() 	/* Init reset pin */
#define HV7131GP_PIN_RESETB_HIGH()	/* Set pin to high value*/
#define HV7131GP_PIN_RESETB_LOW()	/* Set pin to low value*/
/**  @} */

/**
* @name Vertical Sync
*
* INT3. Functions to set, reset, start, stop the vertical sync interrupt
*
* @{ */
#define HV7131GP_PIN_VSYNC_INIT_POSITIVE()
#define HV7131GP_PIN_VSYNC_INIT_NEGATIVE()
#define HV7131GP_PIN_VSYNC_START()
#define HV7131GP_PIN_VSYNC_STOP()
#define HV7131GP_VSYNC_RESET_IF()
#define HV7131GP_VSYNC_IF_VALUE	0	
#define HV7131GP_VSYNC_INTERRUPT()  void vsync_handler(void)

/**  @} */
/**
* @name Horizontal Sync
*
* CN15 (RD6). Functions to set, reset, start, stop the horizontal sync interrupt
*
* @{ */
#define HV7131GP_PIN_HSYNC_INIT()
#define HV7131GP_PIN_HSYNC_START()
#define HV7131GP_PIN_HSYNC_STOP()
#define HV7131GP_HSYNC_RESET_IF()	
#define HV7131GP_HSYNC_INTERRUPT() void hsync_handler(void)
#define HV7131GP_HSYNC_VALUE()	0	

/**  @} */
/**
* @name Video (Pixels) Clock
*
* INT4. It's the pixel interrupt: everytime this interrupt occurs a pixel is
* captured.
*
* @{ */
#define HV7131GP_PIN_VCLK_INIT()
#define HV7131GP_PIN_VCLK_START()
#define HV7131GP_PIN_VCLK_STOP()
#define HV7131GP_VCLK_INTERRUPT()	void vclk_handler(void)	
#define HV7131GP_VCLK_RESET_IF() 	
#define HV7131GP_VCLK_IF_VALUE() 	0

/**  @} */
/**
* @name Master Clock
*
* OC1 with Timer3. The output of this pin is the master clock for HV7131GP camera.
* The master clock is the time reference of the camera. It's set a master clock
* with a frequency of 10MHz
*
* @{ */

#define HV7131GP_PIN_MCLK_INIT(p)	hv7131gp_oc_hal_init(p)	/* Init master clock */
#define HV7131GP_PIN_MCLK_START()	hv7131gp_oc_hal_start()	/* Start master clock */
#define HV7131GP_PIN_MCLK_STOP()	hv7131gp_oc_hal_stop()	/* Stop master clock */
/**  @} */

/**
* @name Pixel pins. In this set of 8 pins are received the 8 bits of each pixel.
* The pixel reception is regulated by the pixel clock
* @{ */
#define HV7131GP_PIN_Y_INIT()	
#define HV7131GP_PIN_Y_VALUE	0
/**  @} */

/**
* @name Software interrupt: end of frame acquisition
*
* INT0. This interrupt is generated when an entire frame is captured. This
* interrupt call inside his ISR the callback function passed on the capture
* function
* \see hv7131gp_capture
*
* @{ */

#define HV7131GP_PIN_EOF_INIT()	
#define HV7131GP_PIN_EOF_STOP()	
#define HV7131GP_EOF_INTERRUPT() void eof_handler(void)
#define HV7131GP_EOF_RESET_IF() 
#define HV7131GP_EOF_ACTIVATE_IF() 

#define HV7131GP_HAL_DISABLE_INTERRUPTS() EE_mico32_disableIRQ()
#define HV7131GP_HAL_ENABLE_INTERRUPTS() EE_mico32_enableIRQ()


#endif /* HV7131GP_HAL_H_ */