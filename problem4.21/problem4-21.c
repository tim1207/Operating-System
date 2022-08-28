#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int n = 0;
int *array = NULL;

void* child(void *arg){
	array = (int*)malloc(n*sizeof(int));
	array[0] = 0;
	array[1] = 1;
	for(int i=2;i<n;i++){
		array[i] = array[i-2] + array[i-1];
	}
	pthread_exit(NULL);
}

int main(void){
	pthread_t p1;
	printf("Please enter a number : ");
	scanf("%d", &n);
	pthread_create(&p1, NULL, child, NULL);
	pthread_join(p1, 0);
	printf("parent\n");
	for(int i=0;i<n;i++){
		printf("Fibonacci numbers of %dst is : %d\n", i+1, array[i]);
	}
	free(array);
	return 0;
}
