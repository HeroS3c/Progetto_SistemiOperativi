/* TORRE */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include "torre.h"

int child(int);
int parent(pid_t *);

int main()
{
	pid_t pid[2];
	int iRet = 0;
	char s[4];
	
	mkfifo(MYPIPE_PATH, S_IRWXU);
	
	for(int n = 0; n < 2; n++) {
		pid[n] = fork();
		if (pid[n] < 0) { /* error occurred */
			fprintf(stderr, "Fork Failed");
			return 1;
		}
		else if (pid[n] == 0) { /* child process */
			sprintf(s, "%d", n);
			execlp("./aereo", "aereo", s, NULL);
			fprintf(stderr, "Exec Failed!\n");
			return 1;
		}
	}

	iRet = parent(pid);
	
	unlink(MYPIPE_PATH);
	return iRet;
}
