#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<sys/mman.h>
#include<fcntl.h>
#include <cstring>
#include <iostream>

using namespace std;
int main(){
    const char *shm_name = "Shared";
    
    int shm_fd = shm_open(shm_name, O_CREAT|O_RDWR, 0);
    if(shm_fd == -1)
	cout << "Error in shm_open." << endl;
	
    int ftrunc_res = ftruncate(shm_fd, 64);
    if(ftrunc_res == -1)
	cout << "Error in ftruncate." << endl;
	
    int *sh_mm = (int*) mmap(0, 64, PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if(sh_mm==MAP_FAILED)
	cout << "Error in mmap." << endl;
    pid_t pid=fork();
    if(pid<0)
	cout << "Fork failed." << endl;
    else if(pid==0) {
	cout << "Child has been created." << endl;
	cout << "Enter a number: " << endl;
	int num;
	cin >> num;
	sh_mm[0] = num;
	int i = 1;
	while(num > 1) {
	    if(num%2 == 0) {
		sh_mm[i] = num/2;
		num = num/2;
		i++;
	    }else {
		sh_mm[i] = 3*num+1;
		num = 3*num+1;
		i++;
	    }
	}
	sh_mm[i] = -1;
	return 0;
    }else {
	wait(NULL);
	cout << "Child terminated. " << endl; 
	cout << "Showing Collatz conjecture..." << endl;
	int i = 0;
	while(sh_mm[i]!=-1){
	    cout << sh_mm[i] << endl;
	    i++;
	}
	int res=shm_unlink(shm_name);
	if(res == 0)
	    cout << "Shared memory unlinked." << endl;
	else
	    cout << "Error while unlinking shared memory." << endl;
    }
    return 0;
}
