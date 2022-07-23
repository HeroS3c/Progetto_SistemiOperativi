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
#include <errno.h>
#include "torre.h"


#define N 10

int hangar(pid_t *childpid){
	int wstatus;
    char arg[1];
	sem_t *sem = sem_open("/semaforo", O_CREAT, S_IRWXU|S_IRGRP|S_IWGRP, 0);

    // creazione 10 processi aereo
    pid_t pid[10];
    for(int n = 0; n < N; n++) {
		pid[n] = fork();
        sleep(1);

		if (pid[n] < 0) { /* error occurred */
			fprintf(stderr, "Fork Fallito: %s", errno);
			return 1;
		}
		else if (pid[n] == 0) { /* processo aereo */
			snprintf(arg, 1, "%d", n); //1 è la size dell'argomento (da cambiare in caso)

			execlp("./aereo", "aereo", arg, NULL);
			fprintf(stderr, "Exec Failed! (Hangar)\n");			
			return 1;
		}

		// due piste di decollo = due semafori
		sem_post(sem);
		sem_post(sem);
	}
	changecolor(YELLOW);
	printf("Hangar: attesa terminazione aerei ...\n");
	

	

	
	for(int i=0; i < 10; i++){
		waitpid(childpid[0], &wstatus, 0);
		changecolor(RED);
		if(WIFEXITED(wstatus))
			printf("[⇓] Hangar: exit code aereo %d = %d\n", i, WEXITSTATUS(wstatus));
	}
			
	sem_close(sem);
	sem_unlink("np1");

	return 0;
}
