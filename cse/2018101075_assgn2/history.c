#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "shell.h"

char *h_command[20];
int h_back = -1, h_front = -1, h_max_size = 20, h_max_print = 10;
char h_path[] = "history.txt";


	void insert_cmd(char* cmd){					// inserts command in the queue
		if(h_front == -1){
			h_front = h_back = 0;
			h_command[h_back] = (char*) malloc(sizeof(char)*strlen(cmd));
			strcpy(h_command[h_back], cmd);
		}
		else if(strcmp(cmd, h_command[h_back]) != 0){
			h_back = (h_back + 1)%h_max_size;
			if(h_back == h_front){
				free(h_command[h_front]);
				h_front = (h_front + 1)%h_max_size;
			}
			h_command[h_back] = (char*) malloc(sizeof(char)*strlen(cmd));
			strcpy(h_command[h_back], cmd);
		}

	}
	
	void load_history(){						// loads the previous history in queue
		char *buf = NULL;	
		size_t len = 0;
		FILE *file;
		char hh_path[PATH_MAX + 4];
		sprintf(hh_path, "%s/%s", shell_path, h_path);
		file = fopen(hh_path, "r");
		if(file == NULL)
			return;
		while(getline(&buf, &len, file) != -1){
			insert_cmd(buf);
		}
		fclose(file);
	}

	void close_history(){						// loads history from queue to file  
		FILE *file;
		char hh_path[PATH_MAX + 4];
		sprintf(hh_path, "%s/%s", shell_path, h_path);
		file = fopen(hh_path, "w");
		int size = (h_back - h_front + h_max_size)%h_max_size + 1;
		for(int  i=0; i<size; i++){
			int k = (i+h_front)%h_max_size;
			fprintf(file, "%s", h_command[k]);
		}
		fclose(file);
	}

	int history(char** cmd, int num){			// prints the history on terminal
		int no = 0;
		if(num>2){
			printf("history: too many arguments\n");
			return 1;
		}
		else if(num == 1){
			no = h_max_print;
		}
		else if(num == 2){
			int len = strlen(cmd[1]);
			for(int i=0; i<len; i++){
				if(cmd[1][i]<'0' || cmd[1][i]>'9'){
					printf("history: numeric argument required\n");
					return 1;
				}
				no = no*10 + (cmd[1][i]-'0');
			}
		}

		int size = (h_back - h_front + h_max_size)%h_max_size + 1;
		no = no < size ? no : size;
		no = no < h_max_size ? no : h_max_size;

		for(int i=0, k=(h_back-no+1+h_max_size)%h_max_size; i<no; i++){
			printf("%s",h_command[(k + i)%h_max_size]);
		}
		return 1;
	}