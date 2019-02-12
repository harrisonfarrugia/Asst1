#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "mymalloc.h"

#define TEST_SIZE 150
#define FULL_TEST_SIZE 100
#define MEMORY_SIZE 4968

/*Receives current time for use in determining runtime */
double getTime() {
	struct timeval start;
	int time_received;
	time_received = gettimeofday(&start, NULL);
	if(time_received == 0) {
		double start_time =  (double) start.tv_sec +  (double) (start.tv_usec * .000001);
		return start_time;
	}
	else {
		printf("Failed to receive time\n");
		exit(0);
	}
}

/*malloc() 1 byte and immediately free it 150 times */
void A() {
	printf("\n\nInitiating test function A\n");
	char *byte;
	int count;
	for(count = 0; count < TEST_SIZE; count++) {
		byte = (char*) malloc(sizeof(char));
		free(byte);
	}	
	
}

/*malloc() 1 byte, store the pointer in an array TEST_SIZE times, then free entire array */
void B() {
	printf("\n\nInitiating test function B\n");
	void *array[TEST_SIZE];
	int count;
	for(count = 0; count < TEST_SIZE; count++) {
		array[count] = (void*) malloc(sizeof(char));
	//	printMemory();
	}
	for(count = 0; count < TEST_SIZE; count++) {	
		free(array[count]);
	//	printMemory();
	}

}

/*Randomly choose between a 1 byte malloc() or free()ing a byte pointer, done TEST_SIZE times */
void C() {
	printf("\n\nInitiating test function C\n");
	void *array[TEST_SIZE];
	int random;
	int malCount = 0;
	int ptr = 0;
	while(malCount < TEST_SIZE) {
		random = rand() % 2;
		if(random == 1) {
			array[ptr] = malloc(sizeof(char));
			malCount++;
			ptr++;
		} else {
			if(ptr > 0) {
				ptr--;
				free(array[ptr]);
			}
		}
	}
	for(malCount = 0; malCount < ptr; malCount++)
		free(array[malCount]);
}

/*Randomly choose between a randomly-sized malloc() or free()ing a pointer, done TEST_SIZE times */
void D(){
	printf("\n\nInitiaing test function D\n");
	void *array[TEST_SIZE];
	int random, size;
	int malCount = 0;
	int ptr = 0;
	while(malCount < TEST_SIZE) {
		random = rand() % 2;
		if(random == 1) {
			size = rand() % 65;
			array[ptr] = malloc(sizeof(size));
			malCount++;
			ptr++;
		} else {
			if(ptr > 0) {
				ptr--;
				free(array[ptr]);
			}
		}	
	}
	for(malCount = 0; malCount < ptr; malCount++)
		free(array[malCount]);
}

/* Fills up memory, then repeatedly attempts to malloc() a 1 byte pointer */
void E() {
	printf("\n\nInitiaing test function E\n");
	int count;
	void *ptr1, *ptr2;
	ptr1 = malloc(MEMORY_SIZE);
	for(count = 0; count < TEST_SIZE; count++) {
		ptr2 = malloc(sizeof(char));	
	}
	free(ptr1);
	free(ptr2);
}

/*Fills up memory, frees every other block, then attempts to malloc() 
 * a size between each free block size and the total available space */

void F() {
	printf("\n\nInitialing test function F\n");
	int count;
	void *array[TEST_SIZE];
	void *ptr;
	for(count = 0; count < TEST_SIZE; count++) {
		array[count] = malloc(sizeof(char));
	}

	for(count = 0; count < TEST_SIZE; count+=2) {
		free(array[count]);
	}
	
	for(count = 0; count < TEST_SIZE; count+=2) {
		ptr = malloc((MEMORY_SIZE - TEST_SIZE)*sizeof(char) + 2);
		if(ptr != NULL)
			free(ptr);
	}	
	for(count = 1; count < TEST_SIZE; count+=2) {	
		free(array[count]);
	}
}

/*Runs every function 100 times, records runtime for each workload, then prints 
 * average time per workload, average time for all workloads, and total runtime */
int main(int argc, char** argv){
	double all_start, all_end, start_time, end_time, mean_time = 0.0;
	double total_time[6];
	int count;
	all_start = getTime();
	for(count = 0; count < FULL_TEST_SIZE; count++) {
		start_time = getTime();
		A();
		end_time = getTime();
		total_time[0] += (double) end_time - (double) start_time; 
		
		start_time = getTime();
		B();
		end_time = getTime();
		total_time[1] += (double) end_time - (double) start_time; 
 
		start_time = getTime();
		C();
		end_time = getTime();
		total_time[2] += (double) end_time - (double) start_time;

		start_time = getTime();
		D();
		end_time = getTime();
		total_time[3] += (double) end_time - (double) start_time;

		start_time = getTime();  
		E();
		end_time = getTime();
		total_time[4] += (double) end_time - (double) start_time; 
 
		start_time = getTime(); 
		F();
		end_time = getTime();
		total_time[5] += (double) end_time - (double) start_time;
	}
	all_end = getTime();

	int i;
	char c = 'A';
	for(i = 0; i < 6; i++){
		printf("Average time to run workload %c: %lf seconds\n", c+i, (double) total_time[i] / (double) FULL_TEST_SIZE);
		mean_time += total_time[i];
	}

	mean_time = mean_time / (double) FULL_TEST_SIZE;
	printf("Average time for workloads: %lf seconds\n", mean_time);

	printf("Total runtime: %lf seconds\n", (double) all_end - (double) all_start);
	return 0;
}
