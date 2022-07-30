TORRE = torre
HANGAR = hangar
AEREO = aereo
LIB = lib
CC = gcc
FLAGS = -pthread # per abilitare il multithreading

all: $(LIB) $(TORRE) $(HANGAR) $(AEREO) 

$(LIB): lib.o
	$(CC) -c lib.c

$(TORRE): torre.o
	$(CC) $(FLAGS) -o $(TORRE) torre.o lib.o -Wall
	
$(HANGAR): hangar.o
	$(CC) $(FLAGS) -o $(HANGAR) hangar.o lib.o -Wall
	
$(AEREO): aereo.o
	$(CC) $(FLAGS) -o $(AEREO) aereo.o lib.o -Wall


torre.o: torre.c 
	$(CC) -c -o "$@" torre.c

aereo.o: aereo.c 
	$(CC) -c -o "$@" aereo.c

hangar.o: hangar.c 
	$(CC) -c -o "$@" hangar.c
