#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

using namespace std;
int main() {
    int num = 0;
    pid_t pid;
    do {
	cout << "Enter a number " << endl;
  	cin >> num;	
    }while (num <= 0);
    pid = fork();
    if (pid == 0) {
        cout << "Child process is working" << endl;
	cout << num << endl;
	while (num != 1) {
	    if (num%2 == 0) {
		num = num/2;
	    } else if (num%2 == 1) {
		num = 3 * (num) + 1;
	    }	
		cout << num << endl;
	    }
	cout << "Child process is done." << endl;
    } else {
	cout << "Parent process is waiting on child process..." << endl;
	wait(NULL);
	cout << "Parent process is done." << endl;
   }
   return 0; 
}
