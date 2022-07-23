/*AEREO*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "torre.h"

int main(int argc, char *argv[]){
	
	sem_t *sem = sem_open("/semaforo", O_CREAT, S_IRWXU|S_IRGRP|S_IWGRP, 0);

	changecolor(GREEN);
	printf("[⇑] Aereo: %d partito \n", getpid());

	int value;

	sem_wait(sem); // blocca l'aereo finchè il semaforo non diventa > 0
	
	
	sem_close(sem); // chiude il semaforo
	
	return 0;
}
