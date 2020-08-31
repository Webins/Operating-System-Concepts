#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>

#define READ_END 0
#define WRITE_END 1

int main(int argc, char **argv){
	pid_t pid;
	struct timeval current;
	int fd[2];
	if(argc > 1){
		char * command = argv[1];
		char path[100];
		strcpy(path, "/bin/");
		strcat(path,command);
		/*Create the pipe*/
		if(pipe(fd) == -1){
			fprintf(stderr, "Pipe failed\n");
			return 1;
		}
		pid = fork();
		if(pid ==0){ //child
		/* close the unused end of the pipe */
		close(fd[READ_END]);
		/* write to the pipe */
		write(fd[WRITE_END], &current, sizeof(current));
		/* close the write end of the pipe */
		close(fd[WRITE_END]);
			gettimeofday(&current, NULL);
			if(execlp(path, command, NULL) == -1){
				strcpy(path, "/usr/bin/");
				strcat(path, command);
				execlp(path, command, NULL);
			}
		}else if(pid > 0){ //parent
			wait(NULL);
			/* close the unused end of the pipe */
			close(fd[WRITE_END]);
			/* read from the pipe */
			read(fd[READ_END], &current, sizeof(current));
			/* close the read end of the pipe */
			close(fd[READ_END]);
			printf("seconds elapsed: %ld\nmicro seconds elapsed : %ld\n",current.tv_sec, current.tv_usec);
		}
	}
	else{
		fprintf(stderr,"No arguments provided\nExiting...\n");
	}
	return 0;
}
