EXE = aereporto
CHILD = aereo
CC = gcc
FLAGS = -pthread # per abilitare il multithreading

all: $(CHILD) $(EXE)

$(EXE): torre.o hangar.o 
	$(CC) $(FLAGS) -o $(EXE) torre.o hangar.o 
	
$(CHILD): aereo.o
	$(CC) $(FLAGS) -o $(CHILD) aereo.o

torre.o: torre.c torre.h
	$(CC) -c -o "$@" torre.c

aereo.o: aereo.c torre.h
	$(CC) -c -o "$@" aereo.c

hangar.o: hangar.c torre.h
	$(CC) -c -o "$@" hangar.c

.PHONY: clean
clean:
	@rm -rf *.o $(EXE) $(CHILD)
