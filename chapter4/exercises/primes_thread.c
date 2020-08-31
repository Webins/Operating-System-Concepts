#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int mid = 0;
int n = 0;

void * check_prime(void * param);
void print_prime(int n);

int main(int argc, char ** argv){

	if(argc != 2){
		printf("Usage %s [Number]\n", argv[0]);
		exit(1);
	}
	
	int i, status;
	const int opt[3] = {0,1,2};
			
	n = atoi(argv[1]);
	mid = n / 2;
	
	
	if(n < 100){ // don't overhead
		check_prime((void *)&opt[0]);
	}else if(n >= 100){
		pthread_t tid_lhs, tid_rhs; // thread id
    		pthread_attr_t attr; // set of thread attributes
    
    		//Setting the default attrs of the threads
    		pthread_attr_init(&attr);
    		
    		status = pthread_create(&tid_rhs, &attr, check_prime, (void *)&opt[1]);
	
		if(status){
			printf("[Error in thread]: Return code is %d\n", status);
			exit(0);
		}
		
		
		status = pthread_create(&tid_lhs, &attr, check_prime, (void *)&opt[2]);
	
		if(status){
			printf("[Error in thread]: Return code is %d\n", status);
			exit(0);
		}
		
		pthread_join(tid_lhs, NULL);
		pthread_join(tid_rhs, NULL);
	
	}else{
		printf("Usage %s [Positive Number]\n", argv[0]);
	}
	return 0;
}

void * check_prime(void * param){
    	int i, opt = *(int *)param;
    	switch(opt){
    		case 0: { //Complete
    			printf("Primes: [");
    			for(i = 2; i < n; i++)
    				print_prime(i);
    			printf("]\n");
    		}; break;
    		case 1: { //Left hand side
    			printf("Primes: [");
    			for(i = 2; i < mid -1; i++)
    				print_prime(i);
    			pthread_exit(0);
    		}; break;
    		case 2: { //Right hand side
    			for(i = mid; i < n; i++)
    				print_prime(i);
    			printf("]\n");
    			pthread_exit(0);
    		}; break;
    	}
    	
}

void print_prime(int n){
    if (n <= 1) return; // zero and one are not prime
    int i;
    for (i=2; i*i <= n; i++) {
        if (n % i == 0) return;
    }
    printf("%d ", n);
}

