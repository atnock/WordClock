/*
 * i2c.h
 *
 * Created: 1/11/2016 11:13:32 AM
 *  Author: Atnock
 */ 


#ifndef I2C_H_
#define I2C_H_

#define F_CPU			16000000
#define F_SCL			100000L // I2C clock speed 100 KHz
#define READ			1
#define TW_START		0xA4 // send start condition (TWINT,TWSTA,TWEN)
#define TW_STOP			0x94 // send stop condition (TWINT,TWSTO,TWEN)
#define TW_ACK			0xC4 // return ACK to slave
#define TW_NACK			0x84 // don't return ACK to slave
#define TW_SEND			0x84 // send data (TWINT,TWEN)
#define TW_READY		(TWCR & 0x80)// ready when TWINT returns to logic 1.
#define TW_STATUS		(TWSR & 0xF8) // returns value of status register
#define I2C_Stop()		TWCR = TW_STOP // inline macro for stop condition

void I2C_Init(void);
uint8_t I2C_Detect(uint8_t addr);
uint8_t I2C_FindDevice(uint8_t start);
void I2C_Start (uint8_t slaveAddr);
uint8_t I2C_Write(uint8_t data);
uint8_t I2C_ReadACK(void);
uint8_t I2C_ReadNACK(void);
void I2C_WriteByte(uint8_t busAddr, uint8_t data);
void I2C_WriteRegister(uint8_t busAddr, uint8_t deviceRegister, uint8_t data);
uint8_t I2C_ReadRegister(uint8_t busAddr, uint8_t deviceRegister);

#endif /* I2C_H_ */