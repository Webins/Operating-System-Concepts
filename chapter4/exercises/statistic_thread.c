#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int max_value = 0;
int min_value = 0;
float avg_value = 0.0;
size_t SIZE = 0;

void * max(void * param); // function to be called by the max thread
void * min(void * param); // function to be called by the min thread
void * avg(void * param); // function to be called by the avg thread

 
int main(int argc, char ** argv){

	if(argc < 2){
		printf("Usage %s [list of number]\n", argv[0]);
		exit(1);
	}
	SIZE = argc -1;
	
	int list[SIZE], i, j, status;
	
	pthread_t tid_min, tid_max, tid_avg; // thread id
    	pthread_attr_t attr; // set of thread attributes
    
    	//Setting the default attrs of the threads
    	pthread_attr_init(&attr);
    
	//Initialize the array
	for(i = 1, j=0 ; i <= SIZE; i++, j++)
		list[j] = atoi(argv[i]);
	
	//Calculate the avg
	status = pthread_create(&tid_avg, &attr, avg, (void *)list);
	
	if(status){
		printf("Return code is %d\n", status);
		exit(0);
	}
	
	//Calculate the min
	status = pthread_create(&tid_min, &attr, min, list);
	
	if(status){
		printf("Return code is %d\n", status);
		exit(0);
	}
	
	//Calculate the max
	status = pthread_create(&tid_max, &attr, max, list);
	
	if(status){
		printf("Return code is %d\n", status);
		exit(0);
	}
	
	pthread_join(tid_avg, NULL);
	pthread_join(tid_min, NULL);
	pthread_join(tid_max, NULL);
	
	printf("The average value is %.2f\n", avg_value);
	printf("The minimun value is %d\n", min_value);
	printf("The maximum value is %d\n", max_value);
}

void * avg(void * param){
    	int i, sum = 0;
	for(i = 0; i < SIZE; i++)
		sum+= *(((int *)param)+i);
	
	avg_value = (float) (sum / SIZE);

    	pthread_exit(0);
}

void * max(void * param){
	int i, max = *(((int *)param)+0);
	for(i = 0; i < SIZE; i++){
		if(*(((int *)param)+i) > max )
			max = *(((int *)param)+i);
	}
	
	max_value = max;

	pthread_exit(0);
}

void * min(void * param){
	int i, min = *(((int *)param)+0);
	for(i = 0; i < SIZE; i++){
		if(*(((int *)param)+i) < min)
			min = *(((int *)param)+i);
	}
	
	min_value = min;

	pthread_exit(0);
}

