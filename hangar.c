/*HANGAR*/

#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define DATI_DA_SCRIVERE "Dati dal child"

extern char sParentela[64];
extern char sBuf[64];
extern long lDataSize;
extern int fd; 
extern pid_t pid;

void hangar(int iChildPid) {
	int iRet;
	int wstatus;
	sleep(5);
	waitpid(iChildPid, &wstatus, 0);
	if(WIFEXITED(wstatus)) {
		printf("%s: Child %d normally terminated\n", sParentela, iChildPid);
		printf("%s: Child exit code = %d\n", sParentela, WEXITSTATUS(wstatus));
		lseek(fd, SEEK_SET, 0);
		iRet = read(fd, sBuf, lDataSize);
		if(iRet) {
			sBuf[iRet] = '\0';
			printf("%s: Il parent ha riletto \"%s\"\n", sParentela, sBuf);
			printf("%s: ritorno read = %d\n", sParentela, iRet);
		}
		else {
			printf("%s: read reached EOF\n", sParentela);
		}
	}
	else {
		printf("Child abnormally terminated\n");
	}	
	
	return;
}
