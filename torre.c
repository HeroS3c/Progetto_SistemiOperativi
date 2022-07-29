/* TORRE */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h> /* For O_* constants */
#include <sys/stat.h> /* For mode constants */
#include <semaphore.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "torre.h"
#include "lib.h"


int hangar(pid_t *);
char s[4];

int main(){
	
	printf("Progetto laboratorio 2021-22 (Loris Simonetti)\n");

	struct Data stData;
	if (mkfifo(PIPE_PATH, S_IRWXU) == -1) // creata da bash
	 	perror("Errore in creazione FIFO");

	sem_t *sem = sem_open("/semaforo", O_CREAT, S_IRWXU|S_IRGRP|S_IWGRP, 2); // apertura semaforo (2 = n. piste)
	int fd = 0; // file decriptor della pipe
	int iRet = 0, pid;
	int iReadCount = 1;
	stData.s[0] = '\0';
	pid = fork();

	if (pid < 0) { /* error occurred */
		fprintf(stderr, "Fork Failed");
		return 1;
	}
	else if (pid == 0) { /* child process */
		iRet = execlp("./hangar", "hangar", &pid, NULL);
		fprintf(stderr, "Exec Failed! (Torre), error: %s\n", strerror(errno));
		return 1;
	}

	
	printf("\n[T] Torre: inizio accettazione delle partenze \n");
	fd = open(PIPE_PATH, O_RDONLY); // apertura della pipe
	if (fd == -1){
		perror("Errore nell'apertura della FIFO");
		exit(1);
	}


	// accettazione partenze
	while(strcmp(stData.s, "exit")) {
		if((iReadCount = read(fd, &stData, sizeof(stData))) == -1) {
			perror("Parent: Errore in read");
			return 1;
		}
		if (strcmp(stData.s, "PARTENZA")){
			changecolor(WHITE);
			sleep(.1); // per evitare che gli output del processo aereo preceda questo (linea sotto)
			printf("\n[T] Torre: do l'ok alla partenza\n");
			if (strcmp("exit", stData.s))
				printf("[T] Torre: aereo %d ha comunicato: '%s' \n", stData.iCod, stData.s);
			sem_post(sem); // segnala la pista libera (fa partire l'aereo in coda)
		}
		
	}

	changecolor(RED);
	printf("[T] Torre: hangar ha richiesto: '%s' \n", stData.s);

	sem_close(sem);
	sem_unlink("semaforo");
	close(fd);
	
	return iRet;
}


