/* TORRE */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h> /* For O_* constants */
#include <sys/stat.h> /* For mode constants */
#include <semaphore.h>

int child(int);
int parent(pid_t *);
char s[4];

int main(){
	int iRet = 0, pid;
	
	pid = fork();
	if (pid < 0) { /* error occurred */
		fprintf(stderr, "Fork Failed");
		return 1;
	}
	else if (pid == 0) { /* child process */
		execlp("./hangar", "hangar", s, NULL);
		fprintf(stderr, "Exec Failed!\n");
		return 1;
	}


	iRet = parent(pid);
	return iRet;
}
