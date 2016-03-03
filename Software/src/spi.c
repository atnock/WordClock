/*
 * spi.c
 *
 * Created: 1/11/2016 11:03:55 AM
 *  Author: Atnock
 */ 

#include <avr/io.h>
#include "spi.h"

void spiSendByte (char databyte)
{
	// Copy data into the SPI data register
	SPDR = databyte;
	// Wait until transfer is complete
	while (!(SPSR & (1 << SPIF)));
}