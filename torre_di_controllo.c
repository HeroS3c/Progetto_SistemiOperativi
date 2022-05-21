/*TORRE DI CONTROLLO*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define DATI_DA_SCRIVERE "Dati dal child"

char sParentela[64], // serve per verificare che child e parent hanno spazi di indirizzamento diversi
	sBuf[64];
long lDataSize;
int fd; // per verificare che il child eredita una copia dei file descriptor MA due copie dello stesso descrittore
		// puntano alla stessa "voce" nella tabella dei file aperti (puntatore alla posizione corrente in comune).
		// Provate a commentare lseek(), otterrete EOF!
pid_t pid;
		
void aereo();
void hangar(int iForkRet);
		
int main(int argc, char *argv[])
{
	pid_t iForkRet;

	if(argc < 2) {
		printf("Usage:   %s   output_file\n", argv[0]);
		exit(1);
	}
		
	pid = getpid();
	strcpy(sBuf, DATI_DA_SCRIVERE);
	lDataSize = strlen(sBuf);
	strcpy(sParentela, "Parent");	
	fd = open(argv[1], O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
	
	iForkRet = fork();
	if (iForkRet < 0) { /* error occurred */
		fprintf(stderr, "Fork Failed, errno = %d\n", errno);
		perror((const char *)NULL);
		return 1;
	}
	else if (iForkRet == 0) { /* child process */
		aereo();
	}
	else { /* parent process */
		hangar(iForkRet);
	}
	
	printf("%s: exiting, my Pid = %d, parent Pid = %d\n", sParentela, pid, getppid());
	
	if(close(fd) == -1)
		printf("%s: errore in close, errno = %d\n", sParentela);

	if(iForkRet == 0)
		return 17;
	
	return 0;
}

