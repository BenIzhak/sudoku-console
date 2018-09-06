/*
 * MainAux.c
 *
 *  Created on: 29 Jul 2018
 *      Author: Raz
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Cell.h"
#include "Parser.h"
#include "FilesHandler.h"
#include "MainAux.h"
#include "ExSolver.h"
#include "game.h"
#include "def.h"

Cell** userBoard;
Cell** solvedBoard;
Cell** tempBoard;
static boardData brdData;

boardData getBoardData() {
	return brdData;
}

void setBlockRowSize(int size) {
	brdData.blockRowSize = size;
}

void setBlockColSize(int size) {
	brdData.blockColSize = size;
}

Cell** setAllocatedMem() {
	int i;
	int boardRowAndColSize = brdData.blockRowSize * brdData.blockColSize;

	Cell** temp = (Cell **) malloc(boardRowAndColSize * sizeof(Cell*));

	if (temp == NULL) {
		printf("%s", "Error: setAllocatedMem has failed\n");
		return NULL;
	}
	for (i = 0; i < boardRowAndColSize; i++) {
		temp[i] = (Cell *) malloc(boardRowAndColSize * sizeof(Cell));
		if (temp[i] == NULL) {
			printf("%s", "Error: setAllocatedMem has failed\n");
			return NULL;
		}
	}
	return temp;
}

/*
 * Function: getInput
 * --------------------
 *  get input from the user, parse it, and check if the input is valid.
 *
 *  input: string of the input by the user.
 *  command: array holding types of commands.
 *  filePath: the string that will hold the path that given by the user.
 *  numOfArgs: pointer to the variable that holds the number of arguments
 *  that supplied by the user.
 */
int getInput(char input[], int command[], char* filePath, int* numOfArgs) {
	int notDigitFlag;
	printf("%s", "Enter your command:\n");
	if (fgets(input, 256, stdin) == NULL) {
		/* EOF CASE */
		return -1;
	}

	parseCommand(input, command, filePath, numOfArgs, &notDigitFlag);
	if (strcmp(input, "exit") == 0) {
		return 1;
	}
	while (validInput(command, *numOfArgs, notDigitFlag) == -1) {
		printf("%s", "Enter your command:\n");
		if (fgets(input, 256, stdin) == NULL) {
			/* EOF CASE */
			return -1;
		}
		parseCommand(input, command, filePath, numOfArgs, &notDigitFlag);
		if (strcmp(input, "exit") == 0) {
			return 1;
		}
	}
	return 0;
}

/*
 * Function: commmandRouter
 * --------------------
 *  decide which method to execute according to the command.
 *
 *  command: array holding types of commands.
 *  numOfArgs: pointer to the variable that holds the number of arguments
 *  that supplied by the user.
 *  filePath: the string that will hold the path that given by the user.
 */
void commmandRouter(int command[], int numOfArgs, char* filePath) {
	int gameMode = getGameMode();
	switch (command[0]) {
	case 0: /*solve X*/
		solveCommand(filePath);
		break;
	case 1: /*edit X*/
		editCommand(filePath, numOfArgs);
		break;
	case 2: /*mark_errors X*/
		if (gameMode == SOLVE_MODE) {
			setMarkErrors(command[1]);
		} else {
			printf("%s", "ERROR: invalid command\n");
		}
		break;
	case 3: /* print_board */
		if (gameMode == EDIT_MODE || gameMode == SOLVE_MODE) {
			printBoard(userBoard);
		} else {
			printf("%s", "ERROR: invalid command\n");
		}
		break;
	case 4: /* set X Y Z */
		if (gameMode == EDIT_MODE || gameMode == SOLVE_MODE) {
			setCell(command[1] - 1, command[2] - 1, command[3]);
		} else {
			printf("%s", "ERROR: invalid command\n");
		}
		break;
	case 5: /* validate */
		if (gameMode == EDIT_MODE || gameMode == SOLVE_MODE) {
			validateCommand(0);
		} else {
			printf("%s", "ERROR: invalid command\n");
		}
		break;
	case 6: /* generate X Y */
		if (gameMode == EDIT_MODE) {
			generateCommand(command[1], command[2]);
		} else {
			printf("%s", "ERROR: invalid command\n");
		}
		break;
	case 7: /* undo */
		if (gameMode == EDIT_MODE || gameMode == SOLVE_MODE) {
			undoCommand();
		} else {
			printf("%s", "ERROR: invalid command\n");
		}
		break;
	case 8: /* redo */
		if (gameMode == EDIT_MODE || gameMode == SOLVE_MODE) {
			redoCommand();
		} else {
			printf("%s", "ERROR: invalid command\n");
		}
		break;
	case 9: /* save X */
		if (gameMode == EDIT_MODE || gameMode == SOLVE_MODE) {
			saveCommand(filePath);
		} else {
			printf("%s", "ERROR: invalid command\n");
		}
		break;
	case 10: /* hint X Y */
		if (gameMode == SOLVE_MODE) {
			setHint(command[1] - 1, command[2] - 1);
		} else {
			printf("%s", "ERROR: invalid command\n");
		}
		break;
	case 11: /* num_solutions */
		if (gameMode == EDIT_MODE || gameMode == SOLVE_MODE) {
			exBacktrack(userBoard);
		} else {
			printf("%s", "ERROR: invalid command\n");
		}
		break;
	case 12: /* autofill */
		if (gameMode == SOLVE_MODE) {
			autoFillCommand();
		} else {
			printf("%s", "ERROR: invalid command\n");
		}
		break;
	case 13: /* reset */
		if (gameMode == EDIT_MODE || gameMode == SOLVE_MODE) {
			resetCommand();
		} else {
			printf("%s", "ERROR: invalid command\n");
		}
		break;
	case 14: /* exit */
		exitGameCommand();
		break;
	}
}

void boardInit(Cell** table) {
	int i, j;
	int boardRowAndColSize = brdData.blockRowSize * brdData.blockColSize;
	for (i = 0; i < boardRowAndColSize; i++) {
		for (j = 0; j < boardRowAndColSize; j++) {
			table[i][j].currentNum = 0;
			table[i][j].fixed = 0;
			table[i][j].isInput = 0;
			table[i][j].limit = boardRowAndColSize;
			table[i][j].isError = 0;
		}
	}
}

void freeBoardMem(Cell** Board, int blockRowSize, int blockColSize) {
	int boardRowAndColSize = blockRowSize * blockColSize;
	int i;
	if (Board == NULL) {
		return;
	}
	for (i = 0; i < boardRowAndColSize; i++) {
		free(Board[i]);
	}
	free(Board);
}

void printBoard(Cell** table) {
	int i, j;
	int boardRowAndColSize = brdData.blockColSize * brdData.blockRowSize;
	int separatorRowNum = (4 * boardRowAndColSize) + brdData.blockRowSize + 2;
	int currentNum;
	char * separatorRow;
	int markErrors = getMarkErrors();
	int gameMode = getGameMode();
	separatorRow = (char*) calloc(separatorRowNum, sizeof(char));
	for (i = 0; i < separatorRowNum; i++) {
		separatorRow[i] = '-';
	}
	separatorRow[separatorRowNum - 1] = 0;
	for (i = 0; i < boardRowAndColSize; i++) {
		if (i % brdData.blockRowSize == 0) {
			printf("%s\n", separatorRow);
		}
		for (j = 0; j < boardRowAndColSize; j++) {
			if (j % brdData.blockColSize == 0) {
				printf("%s", "|");
			}
			currentNum = table[i][j].currentNum;
			printf("%s", " ");
			if (currentNum == 0) {
				printf("%s", "  ");
			} else {
				printf("%2d", currentNum);
			}
			if (table[i][j].fixed) {
				printf("%s", ".");
			} else if (table[i][j].isError
					&& (gameMode == EDIT_MODE || markErrors == 1)) {
				printf("%s", "*");
			} else {
				printf("%s", " ");
			}

		}
		printf("%s", "|\n");
	}
	printf("%s\n", separatorRow);
	free(separatorRow);
}

void copyBoard(Cell** dstBoard, Cell** srcBoard) {
	int i, j;
	int boardRow = brdData.blockRowSize * brdData.blockColSize;
	int boardCol = boardRow;
	for (i = 0; i < boardRow; i++) {
		for (j = 0; j < boardCol; j++) {
			dstBoard[i][j].currentNum = srcBoard[i][j].currentNum;
			dstBoard[i][j].fixed = srcBoard[i][j].fixed;
			dstBoard[i][j].isInput = srcBoard[i][j].isInput;
			dstBoard[i][j].isError = srcBoard[i][j].isError;
		}
	}
}

void gameLoop() {
	char input[256];
	char filePath[256];
	int command[4];
	int exitFlag = 0;
	int numOfArgs;
	setGameMode(INIT_MODE);
	printf("%s", "Sudoku\n------\n");

	while (exitFlag == 0) {
		exitFlag = getInput(input, command, filePath, &numOfArgs);
		commmandRouter(command, numOfArgs, filePath);
	}
}

