/*AEREO*/
#include "lib.h"
#include "torre.h"
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

long get_random(long lMin, long lMax);

int main(int argc, char *argv[]) {
	// avviso avvio processo aereo
	changecolor(CYAN);
	print_time();
	printf(" Processo aereo avviato (pid: %d)\n", getpid());

	int n = atoi(argv[1]);
	struct Data stData;
	stData.iCod = n + 1;
	// Ogni Aereo attende un tempo variabile tra 3 e 8 secondi, determinato
	// randomicamente
	int r = get_random(3, 8);
	changecolor(CYAN);
	print_time();
	printf(" Aereo %d: preparazione, tempo necessario %ds\n", stData.iCod, r);

	sleep(r);


	int fd = open(
		PIPE_PATH,
		O_WRONLY); // apertura della pipe NOTA: aprire in scrittura implica
					// bloccare aereo finchè aereoporto (torre) non apre in lettura
	if (fd == -1)
		perror("Errore open PIPE");
	sem_t *sem = sem_open("/semaforo", O_CREAT, S_IRWXU | S_IRGRP | S_IWGRP,
							0); // apertura del semaforo

	// Aereo richiede al processo Torre l’autorizzazione al decollo ed attende
	sem_wait(sem); // blocca l'aereo finchè il semaforo non diventa > 0 (chiede
					// autorizzazione al decollo)

	changecolor(BLUE);
	printf("\n");
	print_time();
	printf("[-] Aereo: %d richiesta di partenza inviata alla torre ",
			stData.iCod);
	strcpy(stData.s, "PARTENZA\n");
	if (write(fd, &stData, sizeof(stData)) == -1) {
		perror("Aereo: Errore in write (partenza)");
		return 1;
	} else {
		printf("\n");
		print_time();
		printf("[⇑] Aereo: %d attende la partenza ", stData.iCod);
	}

	// Ottenuta l’autorizzazione decollo
	changecolor(GREEN);
	printf("\n");
	print_time();
	printf("[⇑] Aereo: %d decollo \n", stData.iCod);

	// Il decollo dura un tempo variabile tra 5 e 15 secondi, determinato
	// randomicamente.
	r = get_random(5, 15);
	sleep(r);

	// Trascorso il tempo necessario per il decollo il processo invia una notifica
	// di fine decollo alla Torre e termina.
	strcpy(stData.s, "ARRIVO");
	if (write(fd, &stData, sizeof(stData)) == -1) {
		perror("Aereo: Errore in write");
		return 1;
	}

	changecolor(RED);
	print_time();
	printf("[⇓] Aereo %d: atterrato\n", stData.iCod);

	close(fd);
	sem_close(sem); // chiude il semaforo

	return 0;
}

long get_random(long lMin, long lMax) {
  static int bSeed = 0;
  if (!bSeed) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srandom(tv.tv_usec % 1000);
    bSeed = 1;
  }
  long l = random();
  return ((l % (lMax - lMin + 1L)) + lMin);
}