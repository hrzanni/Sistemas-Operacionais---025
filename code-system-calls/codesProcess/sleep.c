#include <stdio.h>
#include <sys/time.h>  
#include <unistd.h>

int main() {
    for(int i = 1; i < 6; i++) {
	    printf("\r%d", i);
	    fflush(stdout);
	    if (i < 5) {
        sleep(1);
	    }
    }  
    
    printf("\n");
    return 0;
}
