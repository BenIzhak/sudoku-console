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

extern int blockRowSize;
extern int blockColSize;

/* 0 - Init mode, 1 - Edit mode, 2 - Solve mode */
int gameMode;

Cell** setAllocatedMem(int boardRowAndColSize){
	int i,j;
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
	for (i = 0; i < boardRowAndColSize; i++){
		for (j = 0; j < boardRowAndColSize; j++){
			temp[i][j].prevNums = (int *) malloc((blockRowSize * blockColSize) * sizeof(int));
			temp[i][j].validNums = (int *) malloc((blockRowSize * blockColSize) * sizeof(int));
			if(temp[i][j].prevNums == NULL){
				printf("%s","Error: setAllocatedMem has failed\n");
				return NULL;
			}
			if(temp[i][j].validNums == NULL){
				printf("%s","Error: setAllocatedMem has failed\n");
					return NULL;
			}
		}
	}
	return temp;
}

int getInput(char input[], int command[], char* filePath) {
	if (fgets(input, 256, stdin) == NULL) {
		/* EOF CASE */
		return -1;
	}
	parseCommand(input, command, filePath);
	while (validInput(command) == -1) {
		if (fgets(input, 256, stdin) == NULL) {
			/* EOF CASE */
			return -1;
		}
		parseCommand(input, command, filePath);
	}
	return 0;
}

/*
void commmandRouter(Cell** gameBoard, Cell** solvedBoard, Cell** tempBoard, int command[], char* filePath) {
	switch (command[0]) {
		case 0: solve X

			break;
		case 1:  edit X
			if(command[1] == -1){

			}
			break;
	}
}

*/

void boardInit(Cell** table){
	int i, j, k;
	int boardRowAndColSize = blockRowSize * blockColSize;
	for(i = 0; i < boardRowAndColSize; i++){
		for(j = 0; j < boardRowAndColSize; j++){
			for (k = 0; k < boardRowAndColSize; k++) {/* initializes prevNums as not used nums*/
				table[i][j].prevNums[k] = 0;
			}
			table[i][j].currentNum = 0;
			table[i][j].fixed = 0;
			table[i][j].isInput = 0;
			table[i][j].limit = boardRowAndColSize;
			table[i][j].isError = 0;

		}
	}
}

void freeBoardMem(Cell** gameBoard, Cell** solvedBoard, Cell** tempBoard, int BlockRowSize, int BlockColSize){
	int boardRowAndColSize = BlockRowSize * BlockColSize;
	int i, j;
	for(i = 0; i < boardRowAndColSize; i++){
		for(j = 0; j < boardRowAndColSize; j++){
			free(gameBoard[i][j].prevNums);
			free(gameBoard[i][j].validNums);
		}
	}
	for(i = 0; i < boardRowAndColSize; i++){
		for(j = 0; j < boardRowAndColSize; j++){
			free(solvedBoard[i][j].prevNums);
			free(solvedBoard[i][j].validNums);
		}
	}
	for(i = 0; i < boardRowAndColSize; i++){
		for(j = 0; j < boardRowAndColSize; j++){
			free(tempBoard[i][j].prevNums);
			free(tempBoard[i][j].validNums);
		}
	}
	for (i = 0; i < boardRowAndColSize; i++) {
		free(tempBoard[i]);
	}
	for (i = 0; i < boardRowAndColSize; i++) {
		free(solvedBoard[i]);
	}
	for (i = 0; i < boardRowAndColSize; i++) {
		free(gameBoard[i]);
	}
	free(tempBoard);
	free(solvedBoard);
	free(gameBoard);
}

void printBoard(Cell** table, int markErrors){
	int i, j;
	int boardRowAndColSize = blockColSize * blockRowSize;
	int separatorRowNum = (4 * boardRowAndColSize) + blockRowSize + 1;
	int currentNum;
	char * separatorRow;

	separatorRow = (char*) calloc(separatorRowNum, sizeof(char));
	for(i = 0; i < separatorRowNum; i++){
		separatorRow[i] = '-';
	}
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
	printf("%s", separatorRow);
}


/*
void gameLoop() {
	char input[256];
	int command[4];
	int exitFlag = 0;
	char* filePath = NULL;
	Cell** userBoard = NULL;
	Cell** solvedBoard = NULL;
	Cell** tempBoard = NULL;


	while (exitFlag == 0) {
		exitFlag = getInput(input, command, filePath);
		commmandRouter(userBoard, solvedBoard, tempBoard, command, filePath);
	}*//* will take command of edit X and Solve X and initilize the boards */

	/*if (initGame(gameBoard, solvedBoard, tempBoard) == -1) {
		 EOF CASE
		exitGame(gameBoard, solvedBoard, tempBoard);
	} ** this one initializes the boards, need to move after allocating memory***/




