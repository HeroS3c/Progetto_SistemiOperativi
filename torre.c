/* TORRE */
#include "torre.h"
#include "lib.h"
#include <errno.h>
#include <fcntl.h> /* For O_* constants */
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/stat.h> /* For mode constants */
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int hangar(pid_t *);

int main() {
  printf("Progetto laboratorio 2021-22 (Loris Simonetti)\n"); // messaggio di avvio processo Torre

  struct Data stData;

  if (mkfifo(PIPE_PATH, S_IRWXU) == -1) // creata da bash
    perror("Errore in creazione FIFO");

  sem_t *sem = sem_open("/semaforo", O_CREAT, S_IRWXU | S_IRGRP | S_IWGRP,
                        2); // apertura semaforo (2 = n. piste)
  // n = valore corrente del semaforo
  int semvalue = 0;
  int *sval = &semvalue;
  sem_getvalue(sem, sval);
  

  int fd = 0; // file decriptor della pipe
  int iRet = 0, pid;
  int iReadCount = 1;
  stData.s[0] = '\0';
  pid = fork();

  if (pid < 0) { /* error occurred */
    fprintf(stderr, "Fork Failed");
    return 1;
  } else if (pid == 0) { /* child process */
    iRet = execlp("./hangar", "hangar", NULL);
    fprintf(stderr, "Exec Failed! (Torre), error: %s\n", strerror(errno));
    return 1;
  }
  printf("\n");
  print_time();
  printf("[T] Torre: inizio accettazione delle partenze \n");
  fd = open(PIPE_PATH, O_RDONLY); // apertura della pipe
  if (fd == -1) {
    perror("Errore nell'apertura della FIFO");
    exit(1);
  }

  // accettazione partenze
  while (strcmp(stData.s, "exit")) {
    changecolor(WHITE);
    if ((iReadCount = read(fd, &stData, sizeof(stData))) == -1) {
      perror("Parent: Errore in read");
      return 1;
    }

    if (strcmp(stData.s, "ARRIVO")) {
      printf("\n");
      print_time();
      printf("[T] Torre: aereo %d atterrato, notifico una pista libera\n", stData.iCod);
      sem_post(sem);
      continue;
    }

    if (strcmp(stData.s, "PARTENZA")) { // aereo invia la richiesta di partenza
      sleep(.1); // per evitare che gli output del processo aereo preceda questo
                 // (linea sotto)
      if (semvalue > 0) {
        printf("\n");
        print_time();
        printf("[T] Torre: do l'ok alla partenza dell'aereo %d\n", stData.iCod);
      }
    }
  }

  changecolor(RED);
  printf("\n");
  print_time();
  printf("[T] Torre: hangar ha richiesto: '%s' \n", stData.s);

  sem_close(sem);
  sem_unlink("semaforo");
  close(fd);

  return iRet;
}
