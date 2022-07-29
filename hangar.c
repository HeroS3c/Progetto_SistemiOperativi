/*HANGAR*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>
#include "torre.h"
#include "lib.h"

#define N 10

int main(int argc, char *argv[]){
	int wstatus;
    char arg[1];
	int childpid = atol(argv[1]);
	struct Data stData;
	sem_t *sem = sem_open("/semaforo", O_CREAT, S_IRWXU|S_IRGRP|S_IWGRP, 2); // 2 = n. piste

    // creazione 10 processi aereo
    pid_t pid[10];
    for(int n = 0; n < N; n++) {
		pid[n] = fork();
        sleep(2); // TESTO: Tra la creazione di un Aereo e la successiva devono trascorrere 2 secondi

		if (pid[n] < 0) { /* error occurred */
			fprintf(stderr, "Fork Fallito, error: %s\n", strerror(errno));
			return 1;
		}
		else if (pid[n] == 0) { /* processo aereo */
			sprintf(arg,"%d", n); //1 è la size dell'argomento (da cambiare in caso) <-> scopo: n da int a char*
			printf("arg: %s", arg);
			execlp("./aereo", "aereo", arg, NULL);
			fprintf(stderr, "Exec Failed! (Hangar)\n");			
			return 1;
		}
	}


	// Alla fine della generazione degli Aerei il processo Hangar deve attendere che tutti i processi Aereo generati siano terminati.
	changecolor(YELLOW);
	printf("\n Hangar: attesa terminazione aerei ...\n");
	
	for(int i=0; i < N; i++){
		waitpid(childpid, &wstatus, 0); // waitpid(childpid[0], &wstatus, 0);
		changecolor(RED);
		if(WIFEXITED(wstatus))
			printf("[⇓] Hangar: exit code aereo %d = %d\n", i, WEXITSTATUS(wstatus));
		int fd = open(PIPE_PATH, O_WRONLY); // apertura della pipe NOTA: è bloccante 
		strcpy(stData.s, "exit");
		if(write(fd, &stData, sizeof(stData)) == -1) {
			perror("Hangar: Errore in write");
			return 1;
		}
	}

	// Quando tutti i processi Aereo sono terminati l’Hangar notifica alla Torre che non ci sono più aerei che devono decollare e termina. *MANCANTE*


	sem_close(sem);
	sem_unlink("semaforo");
	unlink(PIPE_PATH);
	return 0;
}
