/*
 * ds1307.h
 *
 * Created: 1/11/2016 11:20:34 AM
 *  Author: Atnock
 */ 


#ifndef DS1307_H_
#define DS1307_H_

#define DS1307				0xD0 // I2C bus address of DS1307 RTC
#define SECONDS_REGISTER	0x00
#define MINUTES_REGISTER	0x01
#define HOURS_REGISTER		0x02
#define DAYOFWK_REGISTER	0x03
#define DAYS_REGISTER		0x04
#define MONTHS_REGISTER		0x05
#define YEARS_REGISTER		0x06
#define CONTROL_REGISTER	0x07
#define RAM_BEGIN			0x08
#define RAM_END				0x3F

void DS1307_GetTime(uint8_t *hours, uint8_t *minutes, uint8_t *seconds);
void DS1307_GetDate(uint8_t *months, uint8_t *days, uint8_t *years);
void SetTimeDate(void);
uint8_t BCD2INT(uint8_t data);
uint8_t BCD2Dizaine(uint8_t data);
uint8_t BCD2Unite(uint8_t data);



#endif /* DS1307_H_ */