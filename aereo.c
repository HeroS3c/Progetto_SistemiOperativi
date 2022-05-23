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

int aereo(int argc, char *argv[]){
	
	sem_t *sem = sem_open("/semaforo", O_CREAT, S_IRWXU|S_IRGRP|S_IWGRP, 0);

	printf("Aereo partito");



	sem_wait(sem);
	sem_close(sem);
	
	return 0;
}
