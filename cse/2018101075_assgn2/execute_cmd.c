#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"


int max_proc = 1024, running_proc_size = 0;
struct Proc running[1024];

	int execute_cmd(char*buf, char* shell_path){		// executes command

		int flag = 0;
		for(int i=0; buf[i]!='\0' ; i++){
			if(buf[i] == '&'){
				flag = 1;
				buf[i] = '\0';
				break;
			}
		}

		char **cmd;
		int num;										// splits command to remove extra spaces, tabs
		cmd = split_cmd(buf, &num, " \t\r\f\v\a\b\n");
		if(cmd[0] == NULL){
			return 1;
		}

		if(strcmp(cmd[0], "cd") == 0){
			return cd(cmd, num, shell_path);
		}
		else if(strcmp(cmd[0], "exit") == 0 || strcmp(cmd[0], "quit") == 0){
			return quit();
		}
		else if(strcmp(cmd[0], "pwd") == 0){
			return pwd();
		}
		else if(strcmp(cmd[0], "ls") == 0){
			return ls(cmd, num, shell_path);
		}
		else if(strcmp(cmd[0], "pinfo") == 0){
			return pinfo(cmd, num, shell_path);
		}
		else if(strcmp(cmd[0], "echo") == 0){
			return Echo(cmd, num);
		}
		else if(strcmp(cmd[0], "history") == 0){
			return history(cmd, num);
		}
		else if(strcmp(cmd[0], "nightswatch") == 0){
			int pid = fork();
			if(pid == -1){
				perror("Error Forking");
			}
			else if(pid == 0){
				nightswatch(cmd, num);
				exit(0);
			}
			else{
				int status;
				waitpid(pid, &status, WUNTRACED);
			}
			return 1;
		}
		
		if(flag && running_proc_size == max_proc){
			printf("Could not execute command: number of processes running reached to max limit\n");
			return 1;
		}

		cmd = (char**) realloc(cmd, (num + 1)*sizeof(char*));
		cmd[num] = NULL;

		int pid = fork();								// creates child process
		if(pid == -1){
			perror("Error Forking");
		}
		else if(pid == 0){
			if(flag){
				setpgid(0,0);							// changes gpid to process id
				close(STDERR_FILENO);					// closes stderr for background process
			}
			if(execvp(cmd[0], cmd) == -1){				// execvp executes system commands 
				perror("Could not execute command");
			}
			exit(0);
		}
		else if(pid > 0){
			if(flag != 1){
				int status;
				waitpid(pid, &status, WUNTRACED);		// waits for child process
			}	
			else{
				printf("%d\n",pid);				
				char* temp = (char*) malloc(strlen(cmd[0])*sizeof(char));
				strcpy(temp, cmd[0]);					// stores pid of background process in array
				running_proc_size++;
				for(int i=0; i<max_proc; i++){
					if(running[i].pid == -1){
						running[i].pname = temp;
						running[i].pid = pid;
						break;		
					}
				} 
			}
		}

		return 1;
	}