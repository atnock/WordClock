/*
 * max7219.c
 *
 * Created: 1/11/2016 10:34:17 AM
 *  Author: Atnock
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdio.h>

#include "max7219.h"
#include "spi.h"
#include "mat.h"

char columnInUse = 8;
const int chipInUse = 4;
extern const uint8_t heures[26][32] PROGMEM;
extern const uint8_t dizaines[6][32] PROGMEM;
extern const uint8_t unites[17][32] PROGMEM;
extern const uint8_t specials[8][32] PROGMEM;
extern const uint8_t eseo[4][32] PROGMEM;
const uint8_t anim1[20][32] PROGMEM;
const uint8_t bonneAnnee[6][32] PROGMEM;
const uint8_t ligneUnitaire[8] PROGMEM;

uint8_t matrice[32];


//écrit un octet dans un registre d'un driver;
//data_register : registre du driver;
//data : donnée à écrire dans le registre;
//chip : driver sur lequel il faut écrire;
void MAX7219_writeData(char data_register, char data,int chip) {
	MAX7219_LOAD0;
	for(int i=0; i<chip-1;i++){
		spiSendByte(MAX7219_MODE_NOOP); // Send the register where the data will be stored
		spiSendByte(MAX7219_MODE_NOOP); // Send the register where the data will be stored
	}
	
	spiSendByte(data_register); // Send the register where the data will be stored
	spiSendByte(data); // Send the data to be stored
	
	for(int i=0; i<chipInUse-chip;i++){
		spiSendByte(MAX7219_MODE_NOOP); // Send the register where the data will be stored
		spiSendByte(MAX7219_MODE_NOOP); // Send the register where the data will be stored
	}
	MAX7219_LOAD1;
}

// éteint toutes les leds de la matrice;
void MAX7219_clearDisplay(void)
{
	char i = columnInUse;
	// Loop until 0, but don't run for zero
	do {
		// Set each display in use to blank
		MAX7219_writeData(i, 0x00,1);
		MAX7219_writeData(i, 0x00,2);
		MAX7219_writeData(i, 0x00,3);
		MAX7219_writeData(i, 0x00,4);
	} while (--i);
}

//écrit des matrices de 8 par 32 sur l'horloge;
//sprite : tableau de 32 octets;
void MAX7219_writeSprite(uint8_t* sprite)
{
	int chip = chipInUse;
	int ligne = 0;
	for (int i = 0; i < columnInUse * chipInUse; i++){
		if (ligne == 8){
			chip--;
			ligne = 0;
		}
		MAX7219_writeData(ligne+1,pgm_read_byte(&sprite[i]),chip);
		ligne ++;
	}
}
//écrit un octet dans les registres de chaque driver;
//data_register : registre du driver;
//data : octet de donnée;
void MAX7219_writeToAll (char data_register, char data)
{
	MAX7219_LOAD0;
	for (int chip = 0; chip < chipInUse; chip++)
	{
		spiSendByte(data_register); // Send the register where the data will be stored
		spiSendByte(data); // Send the data to be stored
	}
	MAX7219_LOAD1;
}

void MAX7219_creeMatrice(int heure,int dizaine, int unite){
	int special = 0;
	int minute;
	minute  = dizaine * 10 + unite;
	if (minute > 1)//cas spécial pour 1 minute;
	{
		special = 4;
	}
	if (minute == 40 || minute == 50 || minute == 55 || minute == 35){//cas spécial pour les "moins x";
		heure++;
		special = 5;
		dizaine = 6-dizaine-unite%2;
	}
	if (minute == 15){//cas spécial " il est et quart"
		dizaine = 0;
		unite = 0;
		special = 2;
		} else if (minute == 30){//cas spécial " il est et demis"
		dizaine = 0;
		unite = 0;
		special = 3;
		} else if (minute == 45){//cas spécial " il est moins le quart"
		heure++;
		dizaine = 0;
		unite = 0;
		special = 1;
		} else if (minute > 10 && minute < 17){////cas spécial entre 11 et 16
		dizaine = 0;
		unite  = minute;
	}
	if (heure > 24){//si l'heure est supérieur a 24 on retourne a 1
		heure = 1;
	}
	for(int i=0;i<32; i++){
		matrice[i] = (pgm_read_byte(&heures[heure][i])) | (pgm_read_byte(&dizaines[dizaine][i])) | (pgm_read_byte(&unites[unite][i])) | (pgm_read_byte(&specials[special][i]));
	}
}

//affiche l'heure qu'il est sur l'horloge
//dizaine : dizaine de minutes
//unite : unité des minutes
void MAX7219_printTime (int heure, int dizaine, int unite){
	int chip = chipInUse;
	int ligne = 0;
	//lecture de la matrice ligne par ligne, et écriture dans les registres des différentes puces	
	for (int i = 0; i < columnInUse * chipInUse; i++){
		if (ligne == 8){
			chip--;
			ligne = 0;
		}
		//fusion des différentes matrices par un ou binaire;
		MAX7219_creeMatrice(heure,dizaine,unite);
		MAX7219_writeData(ligne+1,matrice[i],/*(pgm_read_byte(&heures[heure][i])) | (pgm_read_byte(&dizaines[dizaine][i])) | (pgm_read_byte(&unites[unite][i])) | (pgm_read_byte(&specials[special][i]))*/chip);
		ligne ++;
	}
}
void MAX7219_bonneAnnee(){
	int chip;
	int ligne;
	for(int k = 5 ; k>-1 ; k--){
		chip = chipInUse;
		ligne = 0;
		for (int i = 0; i < columnInUse * chipInUse; i++){
			if (ligne == 8){
				chip--;
				ligne = 0;
			}
			MAX7219_writeData(ligne+1,pgm_read_byte(&bonneAnnee[k][i]),chip);
			ligne ++;
		}
	_delay_ms(1000);
	}
_delay_ms(2000);
}


void MAX7219_changementMinute(int heure,int dizaine,int unite,int taille){
	int chip, ligne , frame_special;
	frame_special = 6;
	MAX7219_creeMatrice(heure,dizaine,unite);
	for (int frame = taille-1; frame > -1; frame--){
		chip = chipInUse;
		if (frame%5 == 0 && frame_special<9){
			frame_special++;
		}
		ligne = 0;
		_delay_ms(12);
		for (int i = 0; i < columnInUse * chipInUse; i++){
			if (ligne == 8){
				chip--;
				ligne = 0;
			}
			MAX7219_writeData(ligne+1,((pgm_read_byte(&anim1[frame][i]))|((matrice[i])&pgm_read_byte(&specials[frame_special][i]))),chip);
			ligne ++;
		}
	}
		unite++;
		if(unite==10){
			unite = 0;
			dizaine ++;
			if(dizaine==6){
				dizaine = 0;
				heure++;
			}
		}
		MAX7219_creeMatrice(heure,dizaine,unite);
		frame_special=10;
		for (int frame = 0; frame < taille; frame++){
			chip = chipInUse;
			if (frame%5 ==0 && frame_special>6){
				frame_special--;
			}
			ligne = 0;
			_delay_ms(12);
			for(int i = 0;i<frame;i++){
				_delay_ms(2);
			}

			for (int i = 0; i < columnInUse * chipInUse; i++){
				if (ligne == 8){
					chip--;
					ligne = 0;
				}
				MAX7219_writeData(ligne+1,((pgm_read_byte(&anim1[frame][i]))|((matrice[i])&pgm_read_byte(&specials[frame_special][i]))),chip);
				ligne ++;
			}
	}
	//_delay_ms(1000);
}



void MAX7219_printAnim (int taille){//affiche l'animation numéro 1;
	int chip, ligne;
	for (int frame = taille-1; frame > -1; frame--){
		chip = chipInUse;
		ligne = 0;
		_delay_ms(12);		
		for (int i = 0; i < columnInUse * chipInUse; i++){
			if (ligne == 8){
				chip--;
				ligne = 0;
			}
			MAX7219_writeData(ligne+1,(pgm_read_byte(&anim1[frame][i])),chip);
			ligne ++;
		}
	}
	for (int frame = 0; frame < taille; frame++){
		chip = chipInUse;
		ligne = 0;
		_delay_ms(10);
		for(int i = 0;i<frame;i++){
			_delay_ms(2);
		}
		for (int i = 0; i < columnInUse * chipInUse; i++){
			if (ligne == 8){
				chip--;
				ligne = 0;
			}
			MAX7219_writeData(ligne+1,(pgm_read_byte(&anim1[frame][i]) | pgm_read_byte(&eseo[frame/5][i])),chip);
			ligne ++;
		}
	}
	_delay_ms(1500);	
}


void MAX7219_ecritBitMatrice(uint8_t matrice[],int ligne,int colonne,int valeur){
	uint8_t chip;
	if (colonne>7){
		if (ligne<8){
			ligne=24+ligne;
		}else{
			ligne = 8+ligne;
		}
	}
	colonne=colonne%8;
	if(valeur){
		matrice[ligne]=matrice[ligne]|pgm_read_byte(&ligneUnitaire[colonne]);
		}else{
		matrice[ligne]=matrice[ligne]&(~pgm_read_byte(&ligneUnitaire[colonne]));
	}
	chip= 4-ligne/8;
	//printf("%d %d %d %d \n",ligne,colonne,chip,matrice[ligne]);
	colonne = ligne%8;
	MAX7219_writeData(colonne+1, matrice[ligne],chip);
}

void MAX7219_testEcritBit(void){
	uint8_t matrice[32];
	for(int i=0;i<32;i++){
		matrice[i]=0x00;
	}
	for(int ligne = 0 ; ligne<16; ligne++){
		for(int colonne = 0 ; colonne<16; colonne++){
			if (colonne>=ligne){
				MAX7219_ecritBitMatrice(matrice,ligne,colonne,1);
			}
		}
	}
	_delay_ms(10000);
	for(int ligne = 0 ; ligne<16; ligne++){
		for(int colonne = 0 ; colonne<16; colonne++){
			MAX7219_ecritBitMatrice(matrice,ligne,colonne,0);
		}
	}
}

//initialise les drivers de led
void MAX7219_init(void){
	// SCK MOSI CS/LOAD/SS
	DDRB |= (1 << PIN_SCK) | (1 << PIN_MOSI) | (1 << PIN_SS);

	// SPI Enable, Master mode
	SPCR |= (1 << SPE) | (1 << MSTR)| (1<<SPR1);

	// Decode mode to "Font Code-B"
	MAX7219_writeToAll(MAX7219_MODE_DECODE, 0x00);
	_delay_ms(50);
	// Scan limit runs from 0.
	MAX7219_writeToAll(MAX7219_MODE_SCAN_LIMIT, columnInUse - 1);
	MAX7219_writeToAll(MAX7219_MODE_INTENSITY, 0x0F);
	MAX7219_clearDisplay();
	_delay_ms(50);

	MAX7219_writeToAll(MAX7219_MODE_POWER, ON);
	MAX7219_writeToAll(MAX7219_MODE_TEST, OFF);
}