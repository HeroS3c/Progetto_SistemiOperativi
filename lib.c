// Questo file serve a contenere funzioni condivise (dichiarate in "lib.h") 

#include "lib.h"
#include <time.h>    // time()

// non bastava l'intestanzione sennò aereo (compilato a sè) non trova la funzione
void changecolor(int color){ 
	printf("\e[0;%dm", color);
	return;
}

 
void SigHandler(int iSignalCode) {
	changecolor(RED);
	printf("\n-Terminato-\n");
	changecolor(WHITE);
	return;
}


void print_time(){
	char s[256];
	time_t timet;
	time(&timet);
	struct tm *pTm = localtime(&timet);
	printf("(%02d:%02d:%02d)", pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
	return;
}