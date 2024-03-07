#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem_1, sem_2;

void *start_routine_1(void *args)
{
	int i;
	
	for (i = 0; i < 10; i++)
	{
		printf("%d Hello ", i + 1);
	}
	
	return NULL;
}

void *start_routine_2(void *args)
{
	int i;
	
	for (i = 0; i < 10; i++)
	{
		printf("World!\n");
	}
	
	return NULL;
}

int main(void)
{
	/*
		sem_t sem_1, sem_2;
		
		if (sem_init(&sem_1, 0, 1) == -1)
			printf("sem_init");
	
		if (sem_init(&sem_2, 0, 0) == -1)
			printf("sem_init");
	
		sem_post(sem_t *sem);
		sem_wait(sem_t *sem);

	*/
	
	pthread_t tid_1, tid_2;
	
	pthread_create(&tid_1, NULL, start_routine_1, NULL);
	pthread_create(&tid_2, NULL, start_routine_2, NULL);
	pthread_join(tid_1, NULL);
	pthread_join(tid_2, NULL);
	
    return 0;
}
