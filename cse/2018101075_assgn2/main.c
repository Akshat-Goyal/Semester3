#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "shell.h"

char *shell_path;

	int main(int argc, char **argv){
		signal(SIGINT, signal_handler);				// ctrl^c is ignored
		signal(SIGTSTP, signal_handler);			// ctrl^z is ignored

		int status = 1;
		size_t buf_size = 0;
		char *buf = (char*) malloc(buf_size);
		char **cmds;
		int cmd_no;

		for(int i=0; i<max_proc; i++){
			running[i].pid = -1;
		}

		shell_path = get_shell_path(argv[0]);
		load_history();								// previous history is loaded
		do{
			int pid, pid_status;					// waitpid provides pid of child process which changed its state
			while((pid = waitpid(-1, &pid_status, WNOHANG | WUNTRACED)) > 0){
				if(WIFEXITED(pid_status)){			// WIFEXITED returns 1 if process exited normally
					for(int i=0; i<running_proc_size; i++){
						if(running[i].pid == pid){
							running[i].pid = -1;
							running_proc_size--;
							fprintf(stderr, "%s with pid %d exited normally.\n", running[i].pname, pid);
							free(running[i].pname);
							break;
						}
					}
				}
				else if(WIFSIGNALED(pid_status)){		// WIFEXITED returns 1 if process exited normally
					for(int i=0; i<running_proc_size; i++){
						if(running[i].pid == pid){
							running[i].pid = -1;
							running_proc_size--;
							fprintf(stderr, "%s with pid %d terminated by a signal.\n", running[i].pname, pid);
							free(running[i].pname);
							break;
						}
					}
				}
			}

			display(shell_path);						// displays username@sysname:filepath
			getline(&buf, &buf_size, stdin);			// gets user input from terminal
			if(buf[0] != '\n' && buf[0] != ' '){
				insert_cmd(buf);						// command is added to history
			}
			cmds = split_cmd(buf, &cmd_no, ";");		// parses command from ';' 
			for(int i=0; i<cmd_no; i++){
				if (*cmds[i] == '\n') {
					continue;
				}

				status = execute_cmd(cmds[i], shell_path);		// executes the command

				if(status == 0)
					break;
			}
		} while(status != 0);

		free(shell_path);
		return 0;
	}