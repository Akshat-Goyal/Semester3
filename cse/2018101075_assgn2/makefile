# install:
# 	gcc -Wall main.c nightswatch.c history.c cd.c execute_cmd.c ls.c parse.c pwd.c echo.c pinfo.c util.c -o shell -I. -g -lncurses

CC=gcc
CFLAGS=-I ./ -lncurses
DEPS=shell.h
OBJ= main.o cd.o echo.o execute_cmd.o history.o ls.o nightswatch.o parse.o pinfo.o pwd.o util.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
# 	here -o $@ denotes left side of (:) and $< is the first item on the right side of (:)

# all: clean
shell: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
# 	here $^ denotes all the items which are on the right side of (:)

.PHONY: clean

clean:
	rm -f *.o 