/*HANGAR*/
#include "lib.h"
#include "torre.h"
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define N 10

int main(int argc, char *argv[]) {
  changecolor(CYAN);
  print_time();
  printf(" Processo hangar (pid: %d) creato\n", getpid());

  // inizializzazione tempo
  char s[256];
  time_t timet;
  time(&timet);
  struct tm *pTm = localtime(&timet);
  sprintf(s, "%02d:%02d:%02d", pTm->tm_hour, pTm->tm_min, pTm->tm_sec);

  int wstatus;
  char arg[1];
  struct Data stData;
  sem_t *sem = sem_open("/semaforo", O_CREAT, S_IRWXU | S_IRGRP | S_IWGRP,
                        2); // 2 = n. piste

  // creazione 10 processi aereo
  pid_t pid[10];
  for (int n = 0; n < N; n++) {
    pid[n] = fork();
    sleep(2); // TESTO: Tra la creazione di un Aereo e la successiva devono
              // trascorrere 2 secondi

    if (pid[n] < 0) { /* error occurred */
      fprintf(stderr, "Fork Fallito, error: %s\n", strerror(errno));
      return 1;
    } else if (pid[n] == 0) { /* processo aereo */
      changecolor(CYAN);
      print_time();
      printf(" hangar: Creazione processo aereo %d\n", n);

      sprintf(arg, "%d", n); // <-> scopo: n da int a char*
      printf("arg: %s", arg);
      execlp("./aereo", "aereo", arg, NULL);
      fprintf(stderr, "Exec Failed! (Hangar)\n");
      return 1;
    }
  }

  // Alla fine della generazione degli Aerei il processo Hangar deve attendere
  // che tutti i processi Aereo generati siano terminati.
  changecolor(YELLOW);
  print_time();
  printf(" Hangar: attesa terminazione aerei ...\n");

  for (int i = 0; i < N; i++) {
    waitpid(pid[i], &wstatus, 0); 
    changecolor(RED);
    if (WIFEXITED(wstatus)) {
      print_time();
      printf("[⇓] Hangar: exit code aereo %d = %d\n", pid[i],
             WEXITSTATUS(wstatus));
    }
  }

  int fd = open(PIPE_PATH, O_WRONLY);
  strcpy(stData.s, "exit");
  if (write(fd, &stData, sizeof(stData)) == -1) {
    perror("Hangar: Errore in write");
    return 1;
  }
  changecolor(CYAN);
  print_time();
  printf(" hangar -> torre: \"non ci sono più aerei da far decollare”\n");

  sem_close(sem);
  sem_unlink("semaforo");
  unlink(PIPE_PATH);
  changecolor(CYAN);
  print_time();
  printf(" hangar: processo terminato\n");


  return 0;
}
