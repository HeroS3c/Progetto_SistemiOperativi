/*AEREO*/

#include <string.h>
#include <unistd.h>

//#define DATI_DA_SCRIVERE "Dati dal child"
extern char sParentela[64];
extern char sBuf[64];
extern long lDataSize;
extern int fd; 
extern pid_t pid;

void aereo() {
		pid = getpid();
		strcpy(sParentela, "Child");
		write(fd, sBuf, lDataSize);
		return;
}
