/*
 * max7219.h
 *
 * Created: 1/11/2016 10:37:51 AM
 *  Author: Atnock
 */ 


#ifndef MAX7219_H_
#define MAX7219_H_

#define ON                        1
#define OFF                       0

#define MAX7219_LOAD1             PORTB |= (1<<PIN_SS)
#define MAX7219_LOAD0             PORTB &= ~(1<<PIN_SS)

//Création d'alias associés aux paramètres de notre matrice
#define MAX7219_MODE_DECODE			0x09
#define MAX7219_MODE_INTENSITY		0x0A
#define MAX7219_MODE_SCAN_LIMIT		0x0B
#define MAX7219_MODE_POWER			0x0C
#define MAX7219_MODE_TEST			0x0F
#define MAX7219_MODE_NOOP			0x00

//Création d'alias associés aux colonnes de notre matrice
#define MAX7219_COL1            0x01
#define MAX7219_COL2            0x02
#define MAX7219_COL3            0x03
#define MAX7219_COL4            0x04
#define MAX7219_COL5            0x05
#define MAX7219_COL6            0x06
#define MAX7219_COL7            0x07
#define MAX7219_COL8            0x08

void MAX7219_writeData(char data_register, char data,int chip);
void MAX7219_clearDisplay(void);
void MAX7219_writeSprite(uint8_t* sprite);
void MAX7219_init(void);
void MAX7219_writeToAll (char data_register, char data);
void MAX7219_printTime (int heure, int dizaine, int unite);
void MAX7219_printAnim(int taille);
void MAX7219_creeMatrice(int heure,int dizaine,int unite);
void MAX7219_changementMinute(int heure, int dizaine,int unite,int taille);
void MAX7219_bonneAnnee(void);
void MAX7219_ecritBitMatrice(uint8_t matrice[],int x,int y,int valeur);
void MAX7219_testEcritBit(void);

#endif /* MAX7219_H_ */