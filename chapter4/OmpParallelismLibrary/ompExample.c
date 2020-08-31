#include <stdio.h>
#include <omp.h>

int main(){

	/*Secuential code*/

	#pragma omp parallel
	{
		printf("I am a parallel region");
	}
	
	/*Secuential code*/

	return 0;
}
