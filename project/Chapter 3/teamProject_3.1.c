#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE 1024 
#define MAX_PATH 1024

int main(void) {
	int run = 1; 
	char buffer[10][MAX_LINE];
	int buffer_flag = 0;
	char cmd[MAX_LINE]; 
	char cwd[MAX_PATH]; 
	const char delim[] = " \t\r\n\v\f"; 
	char *token[256]; 
	size_t len; 
	pid_t bg_pids[50];
	int bg_count = 0;

	while (run) { 	
   		if (getcwd(cwd, sizeof(cwd)) != NULL) {} 
   		else { 
   			perror("getcwd() error");
       		return 1;
   		}
	
		printf("karma ~%s: ", cwd); 
		fflush(stdout);
		
		int i = 0; 
		if (fgets(cmd, sizeof cmd, stdin)) {		
			strcpy(buffer[buffer_flag],cmd);
			buffer_flag++;
			token[i] = strtok(cmd, delim); 
			while (token[i] != NULL) {
				i++; 
				token[i] = strtok(NULL, delim); 
			}
		}
		
		if (strcmp("history!", cmd) == 0) {
			for(int i=0;i<buffer_flag;i++){
				if (strcmp(buffer[i],"history!\n") != 0 && buffer[i][0] != '!'){
					printf("%s",buffer[i]);
				}				
			}						
			continue;
		} 
		if (strcmp(token[0], "-h") == 0) {
			for(int i=0;i<atoi(token[1]);i++){
				if (strcmp(buffer[i],"history!\n") != 0){
					printf("%d ",i+1);
					printf("%s",buffer[i]);
				}				
			}						
			continue;
		} 
		len = strlen(cmd); 
		if (len == 1) {
			continue; 
		} 	
		
		if (strcmp("exit", cmd) == 0) {
			run = 0; 
			return 1;
		} 
		
  		int cdf = 0; 
  		if (strcmp(token[0], "cd") == 0) {
  			cdf = 1; 
			if (chdir(token[1]) == 0) {} 
			else if (i == 1) { chdir("/"); } 
			else { printf("cd: %s: No such file or directory\n", token[1]); } 
			continue;
  		}
		
		if (strcmp(token[0], "fg") == 0) {
			if (bg_count != 0) {
			 	pid_t bg_pid = bg_pids[bg_count-1]; 
				waitpid(bg_pid, NULL, 0); 
				bg_count--; 
			}
			
			continue;
		}
		
		int bgf = 0; 
		if (strcmp(token[i-1], "&") == 0) {
			bgf = 1; 
			token[--i] = NULL; 
		}
		
		int pipef = 0; 
		int n = 0;
		int p; 
		char *prog1[256]; 
		char *prog2[256]; 
		while (n < i) {
		  	if (strcmp(token[n], "|") == 0) {
		  		pipef = 1; 
		  		p = n; 
		  		
		  		n++;
		  		
				int v = 0;
				while (v <= p-1) { 
					prog1[v] = token[v]; 
					v++; 
					prog1[i] = strtok(NULL, delim); 
				}
				
				int k = p+1; 
				int l = 0;
				while (k <= i) {
					prog2[l] = token[k]; 
					l++; 
					k++; 
				}
		 	}
		  	n++; 
		}
		
		int rdf = 0; 
		for (int c = 0; c < i; c++) {
  			if (strcmp(token[c], ">") == 0) {
  				rdf = 1;
  			}
		}

 		pid_t pid = fork();
 		
		
		if (pid < 0) {
			return 1;
		}
		
		if (pid == 0) { 
		
			if (pipef == 1) {
				int pfd[2]; 
				pipe(pfd); 

				if (fork() == 0) {      
				    close(STDOUT_FILENO); 
				    dup(pfd[1]); 
				    close(pfd[1]); 

				    execvp(prog1[0], prog1); 
				}

				if (fork() == 0) {
				    close(STDIN_FILENO); 
				    dup(pfd[0]); 
				    close(pfd[0]); 

				   	execvp(prog2[0], prog2);
				}
				
		   	 	wait(NULL);	
			}  
			
			if (rdf == 1) {
				FILE *fp1; 
				FILE *fp2; 
			
				fp1 = fopen(token[1], "r");
				fp2 = fopen(token[3], "w");
				if (fp1 == NULL) {
					fprintf(stderr, "%s: No such file or directory\n", token[1]);
					return 0;
				}						
				
				dup2(fileno(fp1), STDIN_FILENO); 
				dup2(fileno(fp2), STDOUT_FILENO); 
								
				fclose(fp1); 
				fclose(fp2);		
				
				char *arg[] = {token[0], token[3], token[1], 0};
				execvp(arg[0], arg); 		
			}
			
			execvp(token[0], token); 
			
			if (cdf) {} 
			else { printf("\nCommand '%s' not found\n\nTry: apt install <deb name>\n\n", cmd); }
			return 0;
		} 
		
		else { 
			if (bgf) {
				bg_pids[bg_count] = pid; 
				bg_count++; 
				printf("[%d]+ Running (pid: %d) %s\n", bg_count, pid, cmd); 
			} else {
				waitpid(pid, NULL, 0); 
			}
		}
	}
	
	return 0; 
}
