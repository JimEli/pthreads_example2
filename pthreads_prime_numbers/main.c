/*
Instructions for implementing pthreads on MS Visual Studio 2017:

Download: http://cs.du.edu/~mitchell/pthreads_compiled.zip
add: pthread.h, sched.h & semaphore.h to "C:\Program Files (x86)\Windows Kits\10\Include\10.0.15063.0\ucrt"
add: pthreadVC2.lib to "C:\Program Files (x86)\Windows Kits\10\Lib\10.0.15063.0\ucrt\x86"
add: pthreadVC2.dll to "C:\Windows"
add: pthreadVC2.lib to the Project Properties > Configuration Properties > Linker > Input > Additional Dependencies
add: #define _TIMESPEC_DEFINED 
*/
#define _TIMESPEC_DEFINED // Precludes redefining timespec struct located in time.h

// pthread prime number generator.
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

// C/C++ Preprocessor Definitions: _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996) 

int totalCount = 0, maxNumberRange;
pthread_mutex_t tMutex = PTHREAD_MUTEX_INITIALIZER;

void *threadSum(void *n) {
	int i, *from = (int *)n;
	bool primeFlag = true;

	for (i = *from; i <= maxNumberRange; i++) {
		for (int j = 2; j < i; j++) {
			if (i % j == 0) {
				primeFlag = false;
				break;
			}
		}
		if (primeFlag) {
			printf("[ %d ] Thread\n", i);
			pthread_mutex_lock(&tMutex);
			totalCount++;
			pthread_mutex_unlock(&tMutex);
		}
		primeFlag = true;
	}
	pthread_exit(NULL);
	return NULL;
}

int main(void) {
	int i, threadParameter;
	bool primeFlag = true;
	pthread_t tid;

	puts("Enter upper number to count prime numbers to: ");
	scanf("%d", &maxNumberRange);
	threadParameter = maxNumberRange / 2 + 1;
	pthread_create(&tid, NULL, threadSum, (void *)&threadParameter);

	// Main thread.
	for (i = 2; i <= maxNumberRange / 2; i++) {
		for (int j = 2; j < i; j++) {
			if (i % j == 0) {
				primeFlag = false;
				break;
			}
		}
		if (primeFlag) {
			printf("[ %d ]  Main\n", i);
			pthread_mutex_lock(&tMutex);
			totalCount = totalCount++;
			pthread_mutex_unlock(&tMutex);
		}
		else
			primeFlag = true;
	}

	pthread_join(tid, NULL);
	printf("Final Count is : %d \n", totalCount);
	return 0;
}
