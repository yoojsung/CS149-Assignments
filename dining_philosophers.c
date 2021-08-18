#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;
enum {THINKING, HUNGRY, EATING} state[5];	
pthread_cond_t cond_vars[5];
int thread_id[5];
pthread_t tid[5];

void test(int number) 
{
	if ((state[(number+1)%5] != EATING) && (state[number] == HUNGRY) && (state[(number+4)%5] != EATING)) {
		state[number] = EATING;
		printf("Philosopher %d eats.\n", number);
		pthread_cond_signal(&cond_vars[number]);
	}
}

void pickup_forks(int number)
{
	pthread_mutex_lock(&mutex);
	printf("Philosopher %d is hungry.\n", number);
	state[number] = HUNGRY;
	test(number);
	if (state[number] != EATING)
		pthread_cond_wait(&cond_vars[number], &mutex);
	pthread_mutex_unlock(&mutex);
}

void return_forks(int number) 
{
	pthread_mutex_lock(&mutex);
	printf("Philosopher %d returns forks.\n", number);
	state[number] = THINKING;
	test((number+1)%5);
	test((number+4)%5);
	pthread_mutex_unlock(&mutex);
}

void *runner(void *param) 
{
	int *temp = (int *)param;
	int number = *temp;
	pickup_forks(number);
	sleep(0.0050);
	return_forks(number);
}

int main() 
{
	pthread_mutex_init(&mutex, NULL);
	for (int i = 0; i < 5; i++) {
		state[i] = THINKING;
		thread_id[i] = i;
	}
	for (int i = 0; i < 5; i++) {
		pthread_create(&tid[i], 0, runner, (void *)&thread_id[i]);
	}
	for (int i = 0; i < 5; i++) {
		pthread_join(tid[i], NULL);
	}
}

