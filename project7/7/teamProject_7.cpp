#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define CUSTOMERS_NUM 5
#define RESOURCES_NUM 4

int releaseResources(int customer_num);
int requestResources(int customer_num, int request[]);
int Banker(int customer_num,int request[]);
void *thread_func(void* customer_numt);
void printResults();

int available[RESOURCES_NUM];
int maximum[CUSTOMERS_NUM][RESOURCES_NUM];
int allocation[CUSTOMERS_NUM][RESOURCES_NUM]={0};
int need[CUSTOMERS_NUM][RESOURCES_NUM];
int Bavailable[RESOURCES_NUM];
int Bmaximum[CUSTOMERS_NUM][RESOURCES_NUM];
int Ballocation[CUSTOMERS_NUM][RESOURCES_NUM];
int Bneed[CUSTOMERS_NUM][RESOURCES_NUM];
int Finish[CUSTOMERS_NUM]={0};
int safeSeq[CUSTOMERS_NUM]={0};
char string[RESOURCES_NUM*2]={0};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, const char * argv[]) {
    for(int i=0 ;i<argc-1;i++) available[i]=atoi(argv[i+1]);    
    for(int i=0; i<RESOURCES_NUM; i++){
        for(int j=0; j<CUSTOMERS_NUM; j++){
            maximum[j][i] = rand() % (available[i]+1);     
            need[j][i] = maximum[j][i];      
        }
    }
    
    for (int i=0; i<RESOURCES_NUM; i++) {
        string[i*2] = i + 'A';
        string[i*2+1] = ' ';
    }

    printf("Maximum:\n");
    printf("   %s\n",string);
    for(int i=0; i<CUSTOMERS_NUM;i++){
        printf("P%d ",i+1);
        for(int j=0;j<RESOURCES_NUM;j++) printf("%d ",maximum[i][j]);
        printf("\n");
    }
    printResults();
    
    pthread_mutex_init(&mutex, nullptr);   
    pthread_t p1,p2,p3,p4,p5;
    int a=0,b=1,c=2,d=3,e=4;
    pthread_create(&p1,nullptr,thread_func,&a);
    pthread_create(&p2,nullptr,thread_func,&b);
    pthread_create(&p3,nullptr,thread_func,&c);
    pthread_create(&p4,nullptr,thread_func,&d);
    pthread_create(&p5,nullptr,thread_func,&e);     
    char *returnV;
    
    pthread_join(p1,(void**)&returnV);
    pthread_join(p2,(void**)&returnV);
    pthread_join(p3,(void**)&returnV);
    pthread_join(p4,(void**)&returnV);
    pthread_join(p5,(void**)&returnV); 
    return 0;
}

void *thread_func(void* Tcustomer_num){
    int *c = (int*) Tcustomer_num;
    int customer_num = *c;
    int requestSum = 0;
    while(!Finish[customer_num]){   
        requestSum = 0;
        int request[RESOURCES_NUM] = {0};
        for(int i=0; i<RESOURCES_NUM; i++){
            request[i] = rand() % (need[customer_num][i]+1); 
            requestSum = requestSum + request[i];
        }
        if(requestSum!=0)  
            while(requestResources(customer_num,request)==-1); 
    }
    return 0;
}

int requestResources(int customer_num, int request[]){  
    int returnValue=-1;
    pthread_mutex_lock(&mutex);
    printf("\nP%d requests for ", customer_num+1);
    for(int i=0; i<RESOURCES_NUM; i++) printf("%d ",request[i]);
    printf("\n");
    for(int i=0; i<RESOURCES_NUM; i++){                                
    	if(request[i] > available[i]){
            printf("P%d is waiting for the resources...\n",customer_num+1);
            pthread_mutex_unlock(&mutex);   
            return -1;
    	}        
    }
    returnValue=Banker(customer_num,request);  
    if(returnValue==0){ 
        int needIsZero=1;
        printf("The request has been granted.\n");
        for(int j=0; j<RESOURCES_NUM; j++){ 
            allocation[customer_num][j] = allocation[customer_num][j] + request[j];
            available[j] = available[j] - request[j];
            need[customer_num][j] = need[customer_num][j] - request[j];
            if(need[customer_num][j] != 0){
                needIsZero=0;
            }  
        }
        if(needIsZero){
            Finish[customer_num] = 1; 
            releaseResources(customer_num);        
        }
        printResults();   
    }
    else{
        printf("cannot find a safe sequence\n");
    }
    pthread_mutex_unlock(&mutex); 
    return returnValue;
}

int releaseResources(int customer_num){
    printf("*P%d have released all the resources.\n", customer_num+1);
    for(int i=0; i<RESOURCES_NUM; i++){
        available[i]=available[i] + allocation[customer_num][i]; 
        allocation[customer_num][i] = 0;   
    }
    return 0;
}

int Banker(int customer_num,int request[]){
    int finish[CUSTOMERS_NUM] = {0};
    int counter = 0;
    for(int i=0; i<RESOURCES_NUM; i++){ 
        Bavailable[i] = available[i];
        for(int j=0; j<CUSTOMERS_NUM; j++){
            Ballocation[j][i] = allocation[j][i];
            Bmaximum[j][i] = maximum[j][i];
            Bneed[j][i] = need[j][i];  
        }
    }
    for(int i=0; i<RESOURCES_NUM; i++){ 
    	Bavailable[i] = Bavailable[i] - request[i];
    	Ballocation[customer_num][i] = Ballocation[customer_num][i] + request[i];
    	Bneed[customer_num][i] = Bneed[customer_num][i] - request[i];
    }
    while(1){
        int I=-1;
        for(int i=0; i<CUSTOMERS_NUM; i++){ 
            int nLessThanA=1;
            for(int j=0; j<RESOURCES_NUM; j++){
                if(Bneed[i][j] > Bavailable[j] || finish[i]==1){
                    nLessThanA=0;
                    break;
                }     
            }
            if(nLessThanA){ 
                I=i;
                break;
            }       
        }  
        if(I!=-1){
            safeSeq[counter] = I; 
            counter++;
            finish[I] = 1; 
            for(int k=0; k<RESOURCES_NUM; k++) Bavailable[k] = Bavailable[k] + Ballocation[I][k];
        }
        else{      
            for(int i=0; i<CUSTOMERS_NUM; i++){
                if(finish[i] == 0){ 
                    return -1;
                }     
            }
            return 0;            
        }
    } 
}

void printResults(){
    printf("Allocation:\n");
    printf("   %s\n",string);
    for(int i=0; i<CUSTOMERS_NUM; i++){
        printf("P%d ",i);
        for(int j=0; j<RESOURCES_NUM; j++) printf("%d ", allocation[i][j]);
        printf("\n");
    }
    printf("Need:\n");
    printf("   %s\n",string);
    for(int i=0; i<CUSTOMERS_NUM; i++){
        printf("P%d ",i);
        for(int j=0; j<RESOURCES_NUM; j++) printf("%d ", need[i][j]);
        printf("\n");
    }
    printf("Available:\n");
    printf("%s\n",string);
    for (int i=0; i<RESOURCES_NUM; i++) printf("%d ", available[i]);
    printf("\n");
}

