/*
 * JeuDeLaVie.c
 *
 * Created: 06/02/2016 11:28:00 PM
 *  Author: Chaillou
 */ 


#include <stdio.h>
#include <avr/io.h>
#include "max7219.h"
#include <util/delay.h>
#include <stdlib.h>
//#include <util/time.h>
#include "mat.h"


void creaMatrice(uint8_t matrice[][16]){
	uint8_t matrice2[32];
	for(int i=0;i<32;i++){
		matrice2[i]=0;
	}
	uint8_t valeur = 0;
	for (int ligne = 0;ligne<16;ligne ++){
		for (int colonne = 0; colonne<16; colonne++){
			valeur = rand()*2;
			matrice[ligne][colonne] = valeur;
			MAX7219_ecritBitMatrice(matrice2,ligne,colonne,valeur);
		}
	}
}

