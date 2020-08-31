#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int sum; // Data to be shared among threads
void * runner(void * param); // function to be called by the threads

int main(int argc, char ** argv){

    if(argc < 2){
        printf("Need to pass a number to calc the sum\n");
        exit(0);
    }

    pthread_t tid; // thread id
    pthread_attr_t attr; // set of thread attributes

    //Setting the default attrs of the threads
    pthread_attr_init(&attr);
    //Create the thread
    pthread_create(&tid, &attr, runner, argv[1]);
    //Wait for the thread to exit (Syncrhonous)
    pthread_join(tid, NULL);

    printf("Sum = %d\n", sum);

    return 0;
}


void  * runner(void * param){
    int i, upper = atoi(param);
    sum = 0;

    for(i = 1; i <= upper; i++)
        sum+=i;

    pthread_exit(0);
}
