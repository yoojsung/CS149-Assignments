#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

int average, min, max;

void *getAverage(void *param) {
	int *nums = (int*)param;
	int sum = 0;
	for (int i = 0; i < sizeof(nums)-1; i++)
		sum += nums[i];
	average = sum / (sizeof(nums) - 1);
}

void *getMinimum(void *param) {
	int *nums = (int*)param;
	min = nums[0];
	for (int i = 1; i < sizeof(nums)-1; i++) 
		if (nums[i] < min)
			min = nums[i];
}

void *getMaximum(void *param) {
	int *nums = (int*)param;
	max = nums[0];
	for (int i = 1; i < sizeof(nums)-1; i++)
		if (nums[i] > max)
			max = nums[i];
}

int main(int argc, char *argv[])
{
	int num_of_args = argc - 1;
	int *list = malloc(sizeof(int) * num_of_args);
	for (int i = 0; i < num_of_args; i++)
		list[i] = atoi(argv[i + 1]);
	pthread_t tid_1, tid_2, tid_3;

	pthread_create(&tid_1, NULL, getAverage, (void*) list);
	pthread_join(tid_1, NULL);

	pthread_create(&tid_2, NULL, getMinimum, (void*) list);
	pthread_join(tid_2, NULL);

	pthread_create(&tid_3, NULL, getMaximum, (void *) list);
	pthread_join(tid_3, NULL);

	printf("Average = %d\nMinimum = %d\nMaximum = %d\n", average, min, max);
	return 0;
}
