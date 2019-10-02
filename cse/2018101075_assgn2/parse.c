#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "shell.h"

	char** split_cmd(char* cmd, int* num, char delim[]){       // parses the command according to delim string
		int lines_no = 32, cnt = 0;

		char *token;
		char **tokens = (char**) malloc(lines_no * sizeof(char*));

		token = strtok(cmd, delim);
		do{
			tokens[cnt++] = token;

			if(cnt >= lines_no){
				lines_no *= 2;
				tokens = (char**) realloc(tokens, lines_no * sizeof(char*));
			}

			token = strtok(NULL, delim);

		} while(token != NULL);

		*num = cnt;
		return tokens;
	}