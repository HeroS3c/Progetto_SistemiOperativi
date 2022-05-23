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
#include "torre.h"

#define N 10

int parent(pid_t *childpid){
	int wstatus;
    char *s;
	sem_t *sem = sem_open("/semaforo", O_CREAT, S_IRWXU|S_IRGRP|S_IWGRP, 0);

    // creazione 10 processi aereo
    pid_t pid[10];
    for(int n = 0; n < N; n++) {
        sleep(2);
		pid[n] = fork();
		if (pid[n] < 0) { /* error occurred */
			fprintf(stderr, "Fork Fallito");
			return 1;
		}
		else if (pid[n] == 0) { /* processo aereo */
			sprintf(s, "%d", n);
			execlp("./aereo", "aereo", s, NULL);
			fprintf(stderr, "Exec Failed!\n");
			return 1;
		}
	}

	printf("Hangar: attesa terminazione aerei ...\n");
	
	sem_post(sem);
	sem_post(sem);
	
	for(int i=0; i < 10; i++){
		waitpid(childpid[0], &wstatus, 0);
		if(WIFEXITED(wstatus))
			printf("Hangar: exit code aereo %d = %d\n", i, WEXITSTATUS(wstatus));
	}
			
	sem_close(sem);
	sem_unlink("np1");

	return 0;
}
