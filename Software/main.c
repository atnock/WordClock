/*
 * GccApplication2.cpp
 *
 * Created: 1/5/2016 9:34:59 AM
 * Author : Atnock
 */ 

#define F_CPU 16000000

#include <stdio.h>
#include <interrupt.h>
//#include <util/setbaud.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "src/max7219.h"
#include "src/ds1307.h"
#include "src/i2c.h"
#include "src/uart.h"
#include "src/adc.h"
#include "src/jeuDeLaVie.h"

void ShowDevices(void);

FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE uart_input = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);

// Scan I2C addresses and display addresses of all devices found
void ShowDevices(void) {
	printf("Found :");
	uint8_t addr = 1;
	while (addr>0) {
		printf(" ");
		addr = I2C_FindDevice(addr);
		if (addr>0)
			printf("%#03x",addr++);
	}
}

int main(void) {

	I2C_Init();
	uart_init();
	MAX7219_init();
	adc_init();
	
	uint8_t hours, minutes, seconds, days, months, years;
	
	stdout = &uart_output;
	stdin  = &uart_input;
	MAX7219_clearDisplay(); 
	_delay_ms(50);
	//MAX7219_writeSprite(&p1);
	printf("Initialisation de la WordClock\n");
	ShowDevices();
	SetTimeDate();
	int h = 9;
	int d = 0;
	int u = 0;
	
	MAX7219_printAnim(20);
	MAX7219_clearDisplay();
	//MAX7219_testEcritBit();
	//MAX7219_clearDisplay();
	
	//uint8_t matrice[16][16];
	//creaMatrice(matrice);
	//_delay_ms(10000);
	
	//MAX7219_bonneAnnee();
	
	while(1) {
		//ADCSRA |= (1 << ADSC);  // Start A2D Conversions
		//DS1307_GetTime(&hours,&minutes,&seconds);
		//DS1307_GetDate(&days,&months,&years);
		
		//printf("%d:%d:%d %d-%d-%d, ",BCD2INT(hours),BCD2INT(minutes),BCD2INT(seconds),BCD2INT(days),BCD2INT(months),BCD2INT(years));
		MAX7219_writeToAll(MAX7219_MODE_TEST, ON);
		//MAX7219_printTime(BCD2INT(hours),BCD2Dizaine(minutes),BCD2Unite(minutes));
		//MAX7219_printTime(h,d,u);
		_delay_ms(6000);
		
		if(d==5&&u==9){
			MAX7219_changementMinute(h,d,u,20);
		}
		
		u++;
		if(u == 10)
		{
			d++;
			u=0;
			if (d == 6){
				d=0;
				h++;
				if (h == 25)
					h = 1;
			}
		}
		//if(BCD2Dizaine(seconds)==5&&BCD2Unite(seconds)==9){
			//MAX7219_changementMinute(BCD2INT(hours),BCD2Dizaine(minutes),BCD2Unite(minutes),20);
		//}
	}
}

ISR(ADC_vect)
{
	//printf("lumi %d\n", (int)15-ADCH/16);
	printf("ADCH = %d, ", ADCH);
	printf("temp %f\n", (double)35.71*(0.00322*ADCH-0.25));
	//MAX7219_writeData(MAX7219_MODE_INTENSITY, 15-ADCH/16);
	_delay_ms(50);
}