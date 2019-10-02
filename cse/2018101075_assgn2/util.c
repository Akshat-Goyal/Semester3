#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <pwd.h>
#include "shell.h"
#define YEL   "\x1B[33m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RST	  "\e[0m"


	char* get_shell_path(char* argv){				// returns the shell path
		char *shell_path = (char*) malloc((PATH_MAX + 1) * sizeof(char));
		char *current_path = (char*) malloc((PATH_MAX + 1) * sizeof(char));
		getcwd(current_path, PATH_MAX + 1);

		for(int i=strlen(argv)-1; i>=0; i--){
			if(argv[i] == '/'){
				argv[i] = '\0';
				break;
			}
		}
		chdir(argv);
		getcwd(shell_path, PATH_MAX + 1);
		chdir(current_path);
		free(current_path);
		return shell_path;
	}

	void display(char* shell_path){				// displays username@sysname:filepath taking care of ~
		struct utsname sys_name;
		uname(&sys_name);
		char cwd[PATH_MAX+1];
		getcwd(cwd, sizeof(cwd));
		int shell_path_len = strlen(shell_path);
		if(strncmp(shell_path, cwd, shell_path_len) == 0 && (strlen(cwd) == shell_path_len || cwd[shell_path_len] == '/')){
			printf(YEL "\e[1m%s@%s:" CYN "~%s> " RST,getpwuid(getuid())->pw_name, sys_name.nodename, &cwd[shell_path_len]);
		}
		else{
			printf(YEL "\e[1m%s@%s:" CYN "%s> " RST,getpwuid(getuid())->pw_name, sys_name.nodename, cwd);
		}
	} 	

	void signal_handler(int signal_num){			// handles the signals or interrupts
		return;
	}
		
	int quit() {									// quits the process
		close_history();
		return 0;
	}