/*
 * spi.h
 *
 * Created: 1/11/2016 11:05:40 AM
 *  Author: Atnock
 */ 


#ifndef SPI_H_
#define SPI_H_

#define PIN_SCK                   PORTB5
#define PIN_MOSI                  PORTB3
#define PIN_SS                    PORTB2

void spiSendByte (char databyte);

#endif /* SPI_H_ */