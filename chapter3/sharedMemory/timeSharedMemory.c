#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(int argc, char **argv){
	pid_t pid;
	/* the size (in bytes) of shared memory object */
	const int SIZE = 4096;
	/* name of the shared memory object */
	const char * name = "time";
	/*Object*/
	struct timeval current;
	/* shared memory file descriptor */
	int fd;
	/* pointer to shared memory object */
	struct timeval * ptr;
	if(argc > 1){
		char * command = argv[1];
		char path[100];
		strcpy(path, "/bin/");
		strcat(path,command);

		pid = fork();
		if(pid ==0){ //child
			/* create the shared memory object */
			fd = shm_open(name, O_CREAT | O_RDWR, 0666);
			/* configure the size of the shared memory object */
			ftruncate(fd, SIZE);
			/* memory map the shared memory object */
			ptr = (struct timeval *)
			mmap (0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
			/* write to the shared memory object */
			gettimeofday(&current, NULL);
			if(execlp(path, command, NULL) == -1){
				strcpy(path, "/usr/bin/");
				strcat(path, command);
				execlp(path, command, NULL);
			}
		}else if(pid > 0){ //parent
			wait(NULL);
			/* open the shared memory object */
			fd = shm_open (name, O_RDONLY, 0666);
			/* memory map the shared memory object */
			ptr = (struct timeval *)
			mmap (0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
			/* read from the shared memory object */
			 printf("seconds elapsed: %ld\nmicro seconds elapsed : %ld\n",current.tv_sec, current.tv_usec);
			/* remove the shared memory object */
			shm_unlink(name);
		}
	}else{
		printf("No arguments provided\nExiting...\n");
	}
	return 0;
}
