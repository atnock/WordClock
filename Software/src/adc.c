/*
 * adc.c
 *
 * Created: 1/11/2016 11:33:59 AM
 *  Author: Atnock
 */ 

#include <interrupt.h>
#include <avr/io.h>

void adc_init(void){
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescalar to 128 - 125KHz sample rate @ 16MHz

	ADMUX |= (1 << REFS0); // Set ADC reference to AVCC
	ADMUX |= (1 << ADLAR); // Left adjust ADC result to allow easy 8 bit reading

	// No MUX values needed to be changed to use ADC0

	ADCSRA |= (1 << ADEN);  // Enable ADC
	
	ADCSRA |= (1 << ADIE);
	sei();
	
	ADCSRA |= (1 << ADSC);  // Start A2D Conversions
}