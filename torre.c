/* TORRE */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h> /* For O_* constants */
#include <sys/stat.h> /* For mode constants */
#include <semaphore.h>
#include <errno.h>


int hangar(pid_t *);
char s[4];

int main(){
	int iRet = 0, pid;
	
	pid = fork();

	if (pid < 0) { /* error occurred */
		fprintf(stderr, "Fork Failed");
		return 1;
	}
	else if (pid == 0) { /* child process */
		hangar(&pid);
		fprintf(stderr, "Exec Failed! (Torre): %d\n", errno);
		return 1;
	}


	iRet = hangar(&pid);
	return iRet;
}
