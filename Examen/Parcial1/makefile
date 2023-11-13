PREFIX  = /home/$(shell whoami)
DEBUG   =  
#DEBUG   =  -g
OMP = -fopenmp
FLAGS   = -Wall
#FLAGS   = -Wall -O3
INC     = -I.
#RPATH   = -Wl,-R$(PREFIX)/lib
LIB     = -L/usr/lib64 -pthread -lm
CC      = gcc

#------------------------------------------------------------------------------
PROGRAM1 = MultiVector
PROGRAM2 = MultiVector-OMP
PROGRAM3 = MultiVector-PTh
default : $(PROGRAM1)
all		: $(PROGRAM1) $(PROGRAM2) $(PROGRAM3)

#------------------------------------------------------------------------------
$(PROGRAM1): $(PROGRAM1).c makefile argshand.o getmem.o utils.o 
	$(CC) $(DEBUG) $(FLAGS) $(INC)  argshand.o getmem.o utils.o\
	$(RPATH) $(LIB) -o $(PROGRAM1) $(PROGRAM1).c

$(PROGRAM2): $(PROGRAM1).c makefile argshand.o getmem.o utils.o 
	$(CC) $(DEBUG) $(OMP) $(FLAGS) $(INC)  argshand.o getmem.o utils.o\
	$(RPATH) $(LIB) -o $(PROGRAM2) $(PROGRAM1).c

$(PROGRAM3): $(PROGRAM3).c makefile argshand.o getmem.o utils.o 01-DivideVector.o
	$(CC) $(DEBUG) $(FLAGS) $(INC)  argshand.o getmem.o utils.o 01-DivideVector.o\
	$(RPATH) $(LIB) -o $(PROGRAM3) $(PROGRAM3).c

utils.o : utils.c utils.h 
	$(CC) $(DEBUG) $(OMP) $(FLAGS) -c -o utils.o utils.c

getmem.o : getmem.c getmem.h 
	$(CC) $(DEBUG) $(OMP) $(FLAGS) -c -o getmem.o getmem.c

argshand.o : argshand.c argshand.h 
	$(CC) $(DEBUG) $(OMP) $(FLAGS) -c -o argshand.o argshand.c

01-DivideVector.o : 01-DivideVector.c 01-DivideVector.h
	$(CC) $(DEBUG) $(OMP) $(FLAGS) -c -o 01-DivideVector.o 01-DivideVector.c

#------------------------------------------------------------------------------
clean:
	@rm $(PROGRAM1) $(PROGRAM2) $(PROGRAM3) *.o *.~ *.bck 2>/dev/null || true