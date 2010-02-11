/** 
* @file fm25h20_hal_ee_pic32.c
* @brief FM25H20 Hw Abstraction Layer using Erika OS over microchip pic32.
* @author Christian Nastasi
* @author Marco Ghibaudi 
* @date 2010-01-12
*
* This file is the  hardware abstraction layer used by all the module
* of the FM25H20 which uses the Erika kernel drivers. 
*
*/

#include <fm25h20_hal.h>
      
#ifdef __MICROCHIP_PIC32__

static uint8_t spi_port; 
/* TODO: metterla come inline ?*/
void fm25h20_delay_us(uint16_t delay_count) 
{	/* Provide a delay with an accuracy of approx. 2% @ 80Mhz*/
	asm volatile(	"add $t0, $a0, $zero\n\t"			  	
			"1:\n\t"
			"addi $t0, $t0, -1\n\t"
			"nop\n\t"
			"nop\n\t"
			"nop\n\t"
			"bne $t0, $zero, 1b\n\t");			
}

void fm25h20_hal_init(void)
{
	/* Set the IO pins direction */
	fm25h20_set_cs_as_out();
	fm25h20_set_hold_as_out();
	fm25h20_set_write_protection_as_out();	
}



int8_t	fm25h20_spi_init(uint8_t port, EE_UINT32 baudrate, EE_UINT16 flags )
{
	spi_port = port;
	return EE_spi_init(spi_port, baudrate, flags); 
			   
}

int8_t	fm25h20_spi_close(void)
{
	return EE_spi_close(spi_port);
}

/* Modificato */
int8_t	fm25h20_spi_write(uint8_t *data, uint16_t len)
{
	return EE_spi_write(spi_port, data, len);
}


/* Modificato */
int8_t	fm25h20_spi_read(uint8_t *data, uint16_t len)
{
	return EE_spi_read(spi_port, data, len);
}

#else

#error "FM25H20_HAL: Architecture not supported!"

#endif
