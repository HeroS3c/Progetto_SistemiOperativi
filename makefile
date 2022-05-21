all: torre_di_controllo hangar aereo

torre_di_controllo:
	gcc torre_di_controllo.c -o torre_di_controllo
hangar:
	gcc hangar.c -o hangar
aereo:
	gcc aereo.c -o aereo