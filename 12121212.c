#include <stdlib.h>             
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <strings.h>
#include <semaphore.h>


pthread_mutex_t* f;
int* hungerOfPhylosophs;
int N;

void takeLeftFork(int);
void takeRightFork(int);
void putLeftFork(int);
void putRightFork(int);
void* meal(int*);


int main()
{
	int Hunger, *Nums;
	pthread_t* P;
	while (1){
		printf("Type number of phylosophs and hunger of them:\n");
		scanf("%d%d", &N, &Hunger);
		if (N == 0) break;
	
		f = (pthread_mutex_t*)malloc(N * sizeof(pthread_mutex_t));
		hungerOfPhylosophs = (int*)malloc(N * sizeof(int));
		P = (pthread_t*)malloc(N * sizeof(pthread_t));
		Nums = (int*)malloc(N * sizeof(int));
		int i;
		for (i = 0; i < N; i++){
			pthread_mutex_init(f + i, NULL);
			hungerOfPhylosophs[i] = Hunger;
			Nums[i] = i;
		}
		
		for (i = 0; i < N; i++) {
				pthread_create(P + i, NULL, (void * (*)(void *))meal, Nums + i);
		} 
		for (i = 0; i < N; i++) {
			pthread_join(P[i], NULL);
		}
		for (i = 0; i < N; i++) {
			pthread_mutex_destroy(f + i);
		}
		free(f);
		free(hungerOfPhylosophs);
		free(P);
		free(Nums);
		printf("All phylosophs finished\n");
	}
	exit(0);
}


void takeLeftFork(int name) {
	if (name == 0)  
		pthread_mutex_lock(f + N - 1);
	else            
		pthread_mutex_lock(f + name - 1);
}

void takeRightFork(int name) {
	pthread_mutex_lock(f + name);
}

void putLeftFork(int name) {
	if (name == 0)  
		pthread_mutex_unlock(f + N - 1);
	else            
		pthread_mutex_unlock(f + name - 1);
}

void putRightFork(int name) {
	pthread_mutex_unlock(f + name);
}


void* meal(int* pName) {
	int name = *pName;
	while (hungerOfPhylosophs[name] > 0) {
		takeLeftFork(name);
		takeRightFork(name);
		hungerOfPhylosophs[name]--;
		printf("Ph %d is eating %d sec, hunger left: %d\n", name, 3, hungerOfPhylosophs[name]);
		sleep(3);
		putLeftFork(name);
		putRightFork(name);
		printf("Ph %d is thinking %d sec\n", name, 3);
		sleep(3);
	}
	printf("Ph %d has finished, now thinks and doesn't eat\n", name);
}





