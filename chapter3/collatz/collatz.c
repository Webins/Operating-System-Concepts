#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
int main(int argc, char ** argv){

    pid_t pid;

    if(argc > 1 ){
        char * number = argv[1];
        int num;
        for(num = 0; number[num] != '\0'; num++){
            if(!isdigit(number[num])) {
                fprintf(stderr, "The input was not a number\nExiting...\n");
                return 1;
            }
        }
        if(atoi(number) < 0){
            fprintf(stderr, "The input was not a positive number\nExiting...\n");
            return 1;
        }
        num = atoi(number);
        pid = fork();
        if(pid == 0) {
            if(num == 1){
                printf("1\n");
                return 0;
            }else if(num == 0){
                printf("0\n");
                return 0;
            }
            printf("%d, ",num);
            while(num > 1){
                if(num % 2 == 0){
                    num = num /2;
                }else{
                    num = (3 * num) +1;
                }
                if(num / 2 == 0){
                    printf("%d\n", num);
                }else{
                    printf("%d, ", num);
                }
            }   

        }else if(pid > 0){
            wait(NULL);
            printf("Done\n");
        }
    }else{
        fprintf(stderr,"No arguments provided\nExiting...\n");
        return 1;
    }
    return 0;
}