#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define MAX_LIMIT 999

int pageSequence[20] = {7, 2, 3, 1, 2, 5, 3, 4, 6, 7, 7, 1, 0, 5, 4, 6, 2, 3, 0, 1};

void ShowPageSequence(){
    for (int i=0; i<20; i++) {
    	printf("%d ", pageSequence[i]);
    }
    printf("\n");
}

void ShowState(int *state, int frameNums){
    for (int i=0; i < frameNums; i++) {
    	printf("%d ", state[i]);
    }
    printf("\n");
}

int frameChecks(int *state, int frameNums, int pageIndex){
    int check = -1;
    for (int i=0; i<frameNums; i++) {
    	if(state[i]==pageIndex) {
    	    check = i;
    	}
    }
    return check;
}

void FIFO_Algorithm(int frameNums){
    int pageFaults = 0;
    int *state;
    state = (int *)malloc(sizeof(int) * frameNums);
    printf("\nFIFO Alorithm:\n");
    for (int i=0; i<frameNums; i++) state[i] = 0;
    for (int i=0; i<20; i++){
        if(frameChecks(state, frameNums, pageSequence[i])==-1){
            pageFaults++;
            if(state[0]!=-1) {
            	for (int i=frameNums-1; i>0; i--) {
            		state[i] = state[i-1];
            	}
            }
    	    state[0] = pageSequence[i];
        }
        ShowState(state, frameNums);
    }
    printf("=> There are %d page faults in FIFO Alforithm.\n", pageFaults);
}

void LRU_Algorithm(int frameNums){
    int pageFaults = 0;
    int *state;
    state = (int *)malloc(sizeof(int) * frameNums);
    printf("\nLRU Alorithm:\n");
    for (int i=0; i<frameNums; i++) state[i] = 0;
    for (int i=0; i<20; i++){
        int index = frameChecks(state, frameNums, pageSequence[i]);
        if(index == -1){
            pageFaults++;
            for (int i=frameNums-1; i>0; i--) {
            	state[i] = state[i-1];
            }
    	    state[0] = pageSequence[i];
        }
        else{
            for (int i=index; i>0; i--) state[i] = state[i-1];
    	    state[0] = pageSequence[i];
        }
        ShowState(state, frameNums);
    }
    printf("=> There are %d page faults in LRU Alforithm.\n", pageFaults);
}

int findPage(int pageIndex, int start){
    for (int i=start; i<20; i++) {
    	if(pageSequence[i] == pageIndex) {
    	    return i;
    	}
    }
    return -1;
}

int findExchange(int *state, int frameNums, int current){
    int max = -1;
    int maxIndex = -1;
    int tempIndex;
    int nonFind = 0;
    for (int i=0; i<frameNums; i++){
        tempIndex = findPage(state[i], current+1);
        if (tempIndex > maxIndex || tempIndex == -1) {
            maxIndex = tempIndex;
            if (tempIndex == -1) {
                maxIndex = MAX_LIMIT + nonFind;
                nonFind++;
            }
            max = state[i];
        }
    }
    return max;
}

int findPageInState(int *state, int frameNums,int pageIndex){
    for (int i=0; i< frameNums; i++) {
    	if(state[i]==pageIndex) {
    	    return i;
    	}
    }
}

void Optimal_Algorithm(int frameNums){
    int pageFaults = 0;
    int *state;
    int init = 0;
    int exchange;
    int exchangeState;
    state = (int *)malloc(sizeof(int) * frameNums);
    printf("\nOptimal Alorithm:\n");
    for (int i=0; i<frameNums; i++) state[i] = 0;
    for (int i=0; i<20; i++){
        int index = frameChecks(state, frameNums, pageSequence[i]);
        if(index == -1){
            pageFaults++;
	    for (int i=0; i<frameNums; i++) {
		if(state[i] == -1){
		    state[i] = pageSequence[i];
		    init = 1;
		    break;
		}
	    }
	    if (init == 0){
		exchange = findExchange(state, frameNums, i);
		exchangeState = findPageInState(state, frameNums, exchange);
		for (int i=exchangeState; i>0; i--) state[i] = state[i-1];
    		state[0] = pageSequence[i];
	    }
        }
        ShowState(state, frameNums);
    }
    printf("=> There are %d page faults in Optimal Alforithm.\n",pageFaults);
}

int main(int argc,char* argv[]){
    srand(time(NULL));
    ShowPageSequence();
    int frameNums;
    printf("Input the frame number: ");
    scanf("%d", &frameNums);
    
    FIFO_Algorithm(frameNums);
    LRU_Algorithm(frameNums);
    Optimal_Algorithm(frameNums);
    return 0;
}
