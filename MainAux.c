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
#include "game.h" /* TODO: DO NOT REMOVE */


extern int blockRowSize;
extern int blockColSize;

extern Cell** userBoard; /* TODO: delete extern, just for testing */
extern Cell** solvedBoard;/* TODO: delete extern, just for testing */
extern Cell** tempBoard;/* TODO: delete extern, just for testing */

/* 0 - Init mode, 1 - Edit mode, 2 - Solve mode */
int gameMode;

Cell** setAllocatedMem(){
	int i;
	int boardRowAndColSize = blockRowSize * blockColSize;

	Cell** temp = (Cell **) malloc(boardRowAndColSize * sizeof(Cell*));

	if(temp == NULL){
		printf("%s","Error: setAllocatedMem has failed\n");
		return NULL;
	}
	for (i = 0; i < boardRowAndColSize; i++) {
		temp [i] = (Cell *) malloc(boardRowAndColSize * sizeof(Cell));
		if(temp[i] == NULL){
				printf("%s","Error: setAllocatedMem has failed\n");
				return NULL;
		}
	}
	return temp;
}

int getInput(char input[], int command[], char* filePath, int* numOfArgs) {
	/* TODO: check on nova that fgets waits for input */
	printf("%s", "Enter your command:\n");
	if (fgets(input, 256, stdin) == NULL) {
		/* EOF CASE */
		return -1;
	}

	parseCommand(input, command, filePath, numOfArgs);
	if(strcmp(input, "exit") == 0){
		return 1;
	}
	while (validInput(command, *numOfArgs) == -1) {
		printf("%s", "Enter your command:\n");
		if (fgets(input, 256, stdin) == NULL) {
			/* EOF CASE */
			return -1;
		}
		parseCommand(input, command, filePath, numOfArgs);
		if(strcmp(input, "exit") == 0){
			return 1;
		}
	}
	return 0;
}

void commmandRouter(int command[], int numOfArgs ,char* filePath) {
	switch (command[0]) {
		case 0: /*solve X*/
			gameMode = 2;
			if(loadBoard(filePath) == -1){
				printf("%s", "Error: File cannot be opened\n");
			}
			break;
		case 1:  /*edit X*/
			gameMode = 1;
			if(numOfArgs > 0){
				if(loadBoard(filePath) == -1){
					printf("%s", "Error: File cannot be opened\n");
				}
			}else{
				startDefaultBoard();
			}
			break;
		case 2:	/*mark_errors X*/
			if(gameMode == 2){
				setMarkErrors(command[1]);
			}else{
				printf("%s", "ERROR: invalid command\n");
			}
			break;
		case 3: /* print_board */
			if(gameMode == 1 || gameMode == 2){
				printBoard(userBoard);
			}else{
				printf("%s", "ERROR: invalid command\n");
			}
			break;
		case 4: /* set X Y Z */
			if(gameMode == 1 || gameMode == 2){
				setCell(command[1]-1, command[2]-1, command[3]);
				printBoard(userBoard);
			}else{
				printf("%s", "ERROR: invalid command\n");
			}
			break;
		case 5: /* validate */
			printf("%s", "validate");
			break;
		case 6: /* generate X Y */
			printf("%s", "generate X Y");
			break;
		case 7: /* undo */
			if(gameMode == 1 || gameMode == 2){
				undo();
			}else{
				printf("%s", "ERROR: invalid command\n");
			}
			break;
		case 8: /* redo */
			if(gameMode == 1 || gameMode == 2){
				redo();
			}else{
				printf("%s", "ERROR: invalid command\n");
			}
			break;
		case 9: /* save X */
			printf("%s", "save X");
			break;
		case 10: /* hint X Y */
			if(gameMode == 2){
				setHint(command[1] - 1, command[2] - 1);
			}else{
				printf("%s", "ERROR: invalid command\n");
			}
			break;
		case 11: /* num_solutions */
			printf("%s", "num_solutions");
			break;
		case 12: /* autofill */
			printf("%s", "autofill");
			break;
		case 13: /* reset */
			if(gameMode == 1 || gameMode == 2){
				reset();
			}else{
				printf("%s", "ERROR: invalid command\n");
			}
			break;
		case 14: /* exit */
			exitGame();
			break;
	}
}


void boardInit(Cell** table){
	int i, j;
	int boardRowAndColSize = blockRowSize * blockColSize;
	for(i = 0; i < boardRowAndColSize; i++){
		for(j = 0; j < boardRowAndColSize; j++){
			table[i][j].currentNum = 0;
			table[i][j].fixed = 0;
			table[i][j].isInput = 0;
			table[i][j].limit = boardRowAndColSize;
			table[i][j].isError = 0;
		}
	}
}

void freeBoardMem(Cell** Board){
	int boardRowAndColSize = blockRowSize * blockColSize;
	int i;
	if(Board == NULL){
		return;
	}
	for (i = 0; i < boardRowAndColSize; i++) {
		free(Board[i]);
	}
	free(Board);
}

void printBoard(Cell** table){
	int i, j;
	int boardRowAndColSize = blockColSize * blockRowSize;
	int separatorRowNum = (4 * boardRowAndColSize) + blockRowSize + 2;
	int currentNum;
	char * separatorRow;
	int markErrors = getMarkErrors();

	separatorRow = (char*) calloc(separatorRowNum, sizeof(char));
	for(i = 0; i < separatorRowNum; i++){
		separatorRow[i] = '-';
	}
	separatorRow[separatorRowNum -1] = 0;
	for(i = 0; i < boardRowAndColSize; i++){
		if(i % blockRowSize == 0){
			printf("%s\n", separatorRow);
		}
		for(j = 0; j < boardRowAndColSize; j++){
			if(j % blockColSize == 0){
				printf("%s", "|");
			}
			currentNum = table[i][j].currentNum;
			printf("%s", " ");
			if(currentNum == 0){
				printf("%s", "  ");
			}else{
				printf("%2d", currentNum);
			}
			if(table[i][j].fixed){
				printf("%s", ".");
			}else if(table[i][j].isError && (gameMode == 1 || markErrors == 1)){
				printf("%s", "*");
			}else{
				printf("%s", " ");
			}

		}
		printf("%s", "|\n");
	}
	printf("%s\n", separatorRow);
	free(separatorRow);
}

/*
 * Function:  copyBoard
 * --------------------
 * copies srcBoard board to dstBoard
 *
 *  dstBoard: 2d array holding a sudoku board, copy destination
 *  srcBoard: 2d array holding a sudoku board, copy source
 *
 */
void copyBoard(Cell** dstBoard, Cell** srcBoard){
	int i, j;
	int boardRow = blockRowSize * blockColSize;
	int boardCol = boardRow;
	for(i = 0; i < boardRow; i++){
		for(j = 0; j < boardCol; j++){
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

	gameMode = 0;
	printf("%s", "Sudoku\n------\n");


	while (exitFlag == 0) {
		exitFlag = getInput(input, command, filePath, &numOfArgs);
		commmandRouter(command, numOfArgs, filePath);
	}
}



