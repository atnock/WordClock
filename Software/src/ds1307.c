/*
 * ds1307.c
 *
 * Created: 1/11/2016 11:19:12 AM
 *  Author: Atnock
 */ 

#include <avr/io.h>
#include "ds1307.h"
#include "i2c.h"

// returns hours, minutes, and seconds in BCD format
//BCD format : la dizaine et l'unité sont séparées
void DS1307_GetTime(uint8_t *hours, uint8_t *minutes, uint8_t *seconds) {
	*hours = I2C_ReadRegister(DS1307,HOURS_REGISTER);
	*minutes = I2C_ReadRegister(DS1307,MINUTES_REGISTER);
	*seconds = I2C_ReadRegister(DS1307,SECONDS_REGISTER);
	if (*hours & 0x40) // 12hr mode:
	*hours &= 0x1F; // use bottom 5 bits (pm bit = temp & 0x20)
	else
	*hours &= 0x3F; // 24hr mode: use bottom 6 bits
}

// returns months, days, and years in BCD format
void DS1307_GetDate(uint8_t *months, uint8_t *days, uint8_t *years) {
	*months	= I2C_ReadRegister(DS1307,MONTHS_REGISTER);
	*days = I2C_ReadRegister(DS1307,DAYS_REGISTER);
	*years = I2C_ReadRegister(DS1307,YEARS_REGISTER);
}

// initialise l'heure et la date; codé en dur;
void SetTimeDate(void) {
	I2C_WriteRegister(DS1307,MONTHS_REGISTER, 0x02);
	I2C_WriteRegister(DS1307,DAYS_REGISTER, 0x15);
	I2C_WriteRegister(DS1307,YEARS_REGISTER, 0x16);
	I2C_WriteRegister(DS1307,HOURS_REGISTER, 0x22+0x00); // add 0x40 for PM
	I2C_WriteRegister(DS1307,MINUTES_REGISTER, 0x15);
	I2C_WriteRegister(DS1307,SECONDS_REGISTER, 0x10);
}

//transforme du format BCD en int;
uint8_t BCD2INT(uint8_t data) {
	uint8_t temp = data>>4;
	data &= 0x0F;
	return temp*10+data;
}

//retourne la dizaine du format BCD;
uint8_t BCD2Dizaine(uint8_t data) {
	return data>>4;
}

//retourne l'unité du format BCD;
uint8_t BCD2Unite(uint8_t data) {
	uint8_t temp = data>>4;
	data &= 0x0F;
	return data;
}