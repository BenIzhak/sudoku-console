CC = gcc
OBJS = main.o MainAux.o ILPSolver.o CommandsList.o ExSolver.o FilesHandler.o game.o Parser.o Stack.o
EXEC = sudoku-console
COMP_FLAG = -ansi -Wall -Wextra \
-Werror -errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@
main.o: main.c MainAux.h SPBufferset.h
	$(CC) $(COMP_FLAG) -c $*.c
MainAux.o: MainAux.c MainAux.h Cell.h Parser.h game.h FilesHandler.h ExSolver.h def.h
	$(CC) $(COMP_FLAG) -c $*.c
ILPSolver.o: ILPSolver.c ILPSolver.h Cell.h MainAux.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
CommandsList.o: CommandsList.c CommandsList.h MainAux.h Cell.h
	$(CC) $(COMP_FLAG) -c $*.c
ExSolver.o: ExSolver.c ExSolver.h Stack.h Cell.h MainAux.h game.h
	$(CC) $(COMP_FLAG) -c $*.c
FilesHandler.o: FilesHandler.c FilesHandler.h MainAux.h game.h CommandsList.h CommandsList.h def.h
	$(CC) $(COMP_FLAG) -c $*.c
game.o: game.c game.h Cell.h ExSolver.h MainAux.h CommandsList.h ILPSolver.h FilesHandler.h def.h
	$(CC) $(COMP_FLAG) -c $*.c
Parser.o: Parser.c Parser.h MainAux.h game.h def.h
	$(CC) $(COMP_FLAG) -c $*.c	
Stack.o: Stack.c Stack.h
	$(CC) $(COMP_FLAG) -c $*.c	
clean:
	rm -f $(OBJS) $(EXEC)