#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t north;
sem_t south;
pthread_mutex_t accessible = PTHREAD_MUTEX_INITIALIZER;

int count = 0;

void *fromSouth(void *param){
    int* index = (int*)param;
    int randomTime = rand()%10 + 1;
    sem_wait(&south);
    sem_post(&south);
    pthread_mutex_lock(&accessible);
    count++;
    
    if (count == 1) sem_wait(&north);
    pthread_mutex_unlock(&accessible);
    printf("South farmer [%d] is acrossing the bridge...:%ds\n",*index, randomTime);
    sleep(randomTime);
    printf("South farmer [%d] is leaving the bridge...:%ds\n",*index, randomTime);
    pthread_mutex_lock(&accessible);
    count--;
    
    if (count == 0) {
        sem_post(&north);
        printf("\nThe bridge is nobody now!\n");
        printf("\n");
    }
    pthread_mutex_unlock(&accessible);
    pthread_exit(0);
}

void *fromNorth(void *param){
    int* index = (int*)param;
    int randomTime = rand()%10 + 1;
    sem_wait(&north);
    sem_post(&north);
    pthread_mutex_lock(&accessible);
    count++;
    
    if (count == 1) sem_wait(&south);
    pthread_mutex_unlock(&accessible);
    printf("Nouth farmer [%d] is acrossing the bridge...:%ds\n",*index, randomTime);
    sleep(randomTime);
    printf("Nouth farmer [%d] is leaving the bridge...:%ds\n",*index, randomTime);
    pthread_mutex_lock(&accessible);
    count--;
    
    if (count == 0) {
        sem_post(&south);
        printf("\nThe bridge is nobody now!\n");
        printf("\n");
    }
    pthread_mutex_unlock(&accessible);
    pthread_exit(0);
}

int main(int argc,char* argv[]){
    srand(time(NULL));
    int ids[5] = {0,1,2,3,4};
    sem_init(&north,0,1);
    sem_init(&south,0,1);
    pthread_t northFarmers[5];
    pthread_t southFarmers[5];

    for (int i = 0; i < 5; i++){
    	pthread_create(&southFarmers[i], NULL, fromSouth, (void*)&ids[i]);
    	pthread_create(&northFarmers[i], NULL, fromNorth, (void*)&ids[i]);
    }
    
    for (int i = 0; i < 5; i++){
        pthread_join(southFarmers[i],NULL);
        printf("south pthread [%d] ends\n",i);
    }
    
    for (int i = 0; i < 5; i++){
        pthread_join(northFarmers[i],NULL);
        printf("north pthread[%d] ends\n",i);
    }
    
    sem_destroy(&north);
    sem_destroy(&south);
    return 0;
}
