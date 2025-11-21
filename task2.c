#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    pid_t cpid[3] = {0}; 
    int ret = 0;   

    setbuf(stdout, NULL);

	int filedes1[2];
	int filedes2[2];
	int filedes3[2];
	char c;
	if(pipe(filedes1) == -1) return 1;
	if(pipe(filedes2) == -1) return 1;
	if(pipe(filedes3) == -1) return 1;

    cpid[0] = fork();
    if (cpid[0] < 0) {
        fprintf(stderr, "fork() 1 failed: %s\n", strerror(errno));
        return 0;
    }
    else if (0 == cpid[0]) { // CHILD-1
		close(filedes1[1]);
		close(filedes2[0]);
		close(filedes3[0]);
		close(filedes3[1]);

		read(filedes1[0], &c, 1);
        printf("CHILD-1 (PID=%d) is running.\n", getpid());
		write(filedes2[1], &c, 1);

		close(filedes1[0]);
		close(filedes2[1]);
        exit(0);
    }

    cpid[1] = fork();
    if (cpid < 0) {
        fprintf(stderr, "fork() 2 failed: %s\n", strerror(errno));
        return 0;
    }
    else if (0 == cpid[1]) { // CHILD-2
		close(filedes1[0]);
		close(filedes2[0]);
		close(filedes2[1]);
		close(filedes3[0]);
		close(filedes3[1]);

        printf("CHILD-2 (PID=%d) is running.\n", getpid());
		write(filedes1[1], &c, 1);

		close(filedes1[1]);
        exit(0);
    }
    
    cpid[2] = fork();
    if (cpid < 0) {
        fprintf(stderr, "fork() 3 failed: %s\n", strerror(errno));
        return 0;
    }
    else if (0 == cpid[2]) { // CHILD-3
		close(filedes1[0]);
		close(filedes1[1]);
		close(filedes2[1]);
		close(filedes3[0]);

		read(filedes2[0], &c, 1);
        printf("CHILD-3 (PID=%d) is running.\n", getpid());
		write(filedes3[1], &c, 1);

		close(filedes2[0]);
		close(filedes3[1]);
        exit(0);
    }

    while ((ret = wait(NULL)) > 0)
    {
		close(filedes1[0]);
		close(filedes1[1]);
		close(filedes2[0]);
		close(filedes2[1]);
		close(filedes3[1]);

		read(filedes3[0], &c, 1);
        printf("In PARENT (PID=%d): successfully reaped child (PID=%d)\n", getpid(), ret);

		close(filedes3[0]);
    }

    return 0;
}
/*
		close(filedes1[0]);
		close(filedes1[1]);
		close(filedes2[0]);
		close(filedes2[1]);
		close(filedes3[0]);
		close(filedes3[1]);
*/
