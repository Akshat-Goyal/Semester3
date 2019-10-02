#ifndef SHELL_H

	#define SHELL_H
	
	struct Proc{
		char *pname;
		int pid;
	};
	extern char *shell_path;
	extern int running_proc_size, max_proc;
	extern struct Proc running[1024];
	char* get_shell_path(char*);
	void display(char*);
	char** split_cmd(char*, int*, char[]);
	int execute_cmd(char *, char*);
	int quit();
	int Echo(char **, int);
	int pwd();
	int cd(char **, int, char*);
	int ls(char**, int, char*);
	int pinfo(char**, int, char*);
	void insert_cmd(char*);
	void load_history();
	void close_history();
	int history(char**, int);
	int watch_interrupt(int);
	int watch_dirty(int);
	int nightswatch(char**, char);
	void signal_handler(int);

#endif