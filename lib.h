// Questa libreria serve a contenere costanti e dichiarazioni di funzioni (presenti in "lib.c") 
#include <stdio.h>

enum COLORS
{
	BLACK = 30,
	RED = 31,
	GREEN = 32,
	YELLOW = 33,
	BLUE = 34,
	MAGENTA = 35,
	CYAN = 36,
	WHITE = 37,
	BOLD = 1
};

void changecolor(int color);
void SigHandler(int iSignalCode);
void print_time();