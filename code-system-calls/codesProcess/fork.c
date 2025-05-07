#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    pid = fork();

    if ( pid < 0 ) {
	    fprintf(stderr,"erro in fork()\n"); 
	    exit(1);
    }

    if ( pid == 0 ) {
	    printf("CHILD: My ID is %d, pid (value)is %d\n",getpid(), pid);
    }
    else {
	    printf("PARENT: My ID is %d, pid (child)is %d\n", getpid(), pid);
	    wait(NULL);
    }

    system("date");

    return 0;
}
