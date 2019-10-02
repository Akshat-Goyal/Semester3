# Shell in C

This is a linux shell written in C with basic functionalities like inbuilt commands (eg. `ls` and `cd`) along with user-defined functions (eg. `pinfo`) and normal terminal commands that are executed in it. It has the capability to run both background and foreground processes. Also takes in semicolon separated commands. Appropriate signal handling has also been added.

## RUNNING

- `cd` into directory.

- sudo apt-get install libncurses5-dev libncursesw5-dev;sudo apt install ncurses-doc 

- run `make`

- to delete '.o' files, run `make clean`

- type `exit` or `quit` to exit the shell

## Working Of Shell:

1. Displays
   `username@systemname:current_directory`
   Home Directory of shell is indicated by `~`
2. `echo`, `cd`, `pwd`, `ls`, `pinfo`, `history`, `nightswatch` is build without `execvp` and ignores `&`
3. `cd`, `ls`. Options: ../, ./, ~/
4. `ls`. flags: -l, -a
5. `pinfo` <pid> (Optional). Outputs:-
	pid, Process Status, memory, Executable Path
6. `history` <num> (Optional).
7. `nightswatch` [options] <command>, ex. 'nightswatch -n 2 <command>'. command can be:- interrupt, dirty, press q for exit.
8. Foreground and Background process for System commands.

## FILES
- main.c : main file which contains shell loop
- execute_cmd : contains execution loop, calls every function based on user command
- parse.c : parses user input from command
- cd.c : changes directory
- echo.c : echoes whatever the user typed
- ls.c : displays list of all files and directories
- pwd.c : displays present working directory
- pinfo.c : prints the process related info
- history.c : prints the previous commands given
- nightswatch.c : nightswatch executes the command after every n seconds until q is pressed.
- util.c : contains quit, display, signal_handler, get_shell_path functions. 
- shell.h : header file, contains definitions of the functions and structs used in above files