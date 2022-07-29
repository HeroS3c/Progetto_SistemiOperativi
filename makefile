TORRE = torre
HANGAR = hangar
AEREO = aereo
CC = gcc
FLAGS = -pthread # per abilitare il multithreading

all: $(TORRE) $(HANGAR) $(AEREO)

$(TORRE): torre.o
	$(CC) $(FLAGS) -o $(TORRE) torre.o
	
$(HANGAR): hangar.o
	$(CC) $(FLAGS) -o $(HANGAR) hangar.o
	
$(AEREO): aereo.o
	$(CC) $(FLAGS) -o $(AEREO) aereo.o

torre.o: torre.c torre.h
	$(CC) -c -o "$@" torre.c

aereo.o: aereo.c torre.h
	$(CC) -c -o "$@" aereo.c

hangar.o: hangar.c torre.h
	$(CC) -c -o "$@" hangar.c

.PHONY: clean
clean:
	@rm -rf *.o $(EXE) $(CHILD)
