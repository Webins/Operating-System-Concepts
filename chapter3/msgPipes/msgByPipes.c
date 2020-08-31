#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define READ_END 0
#define WRITE_END 1

int main(){
char msg[15] = "Hello There";
pid_t pid;
int fd[2];
int fd2[2];

if(pipe(fd) == -1){
    fprintf(stderr, "The pipe couldn't be created\nExiting\n");
    return 1;
}
if(pipe(fd2) == -1){
    fprintf(stderr, "The pipe couldn't be created\nExiting\n");
    return 1;
}

pid = fork(); // fork a new process

if(pid == 0){ //child
int i;
for(i=0; i < strlen(msg); i++){
    if(islower(msg[i])) msg[i] = toupper(msg[i]);
    else msg[i] = tolower(msg[i]);
}

    close(fd[READ_END]);
    close(fd2[WRITE_END]);
    write(fd[WRITE_END], msg, strlen(msg) +1);
    read(fd2[READ_END], msg, strlen(msg)+1);

}else if(pid > 0){ // father
    close(fd[WRITE_END]);
    close(fd2[READ_END]);
    read(fd[READ_END], msg, strlen(msg)+1);
    write(fd2[WRITE_END], msg, strlen(msg+1));
    printf("%s", msg);
     wait(NULL);
}else{
    fprintf(stderr, "Fork Failed\nExiting\n");
    return 1;
}


return 0;


}
