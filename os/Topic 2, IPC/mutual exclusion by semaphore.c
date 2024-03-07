#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

static int g_data = 0;

void *start_routine_1(void *args)
{
	int i;
	
	for (i = 0; i < 100000; i++)
		g_data++;
	
	return NULL;
}

void *start_routine_2(void *args)
{
	int i;
	
	for (i = 0; i < 100000; i++)
		g_data--;
	
	return NULL;
}

int main(void)
{
	/*
		sem_t sem;
		if (sem_init(&sem, 0, 1) == -1)
			printf("sem_init");
		sem_post(sem_t *sem);
		sem_wait(sem_t *sem);
		
		static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
		pthread_mutex_lock(pthread_mutex_t *mutex);
		pthread_mutex_unlock(pthread_mutex_t *mutex);
	*/
	
	pthread_t tid_1, tid_2;
	
	pthread_create(&tid_1, NULL, start_routine_1, NULL);
	pthread_create(&tid_2, NULL, start_routine_2, NULL);
	pthread_join(tid_1, NULL);
	pthread_join(tid_2, NULL);
	
	printf("g_data=%d\n", g_data);
	
    return 0;
}
