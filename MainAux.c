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

Cell** setAllocatedMem(){
	int boardRowSize = 3; /*BLOCK_COL_SIZE * BLOCK_ROW_SIZE; will use variables from command router and will make them global */
	int boardColSize = boardRowSize;
	int i;

	Cell** temp = (Cell **) malloc(boardRowSize * sizeof(Cell*));
	if(temp == NULL){
		printf("%s","Error: setAllocatedMem has failed\n");
		return NULL;
	}
	for (i = 0; i < boardRowSize; i++) {
		temp [i] = (Cell *) malloc(boardColSize * sizeof(Cell));
		if(temp[i] == NULL){
				printf("%s","Error: setAllocatedMem has failed\n");
				return NULL;
			}
	}
	return temp;
}

int getInput(char input[], int command[], char* filePath) {
	if (fgets(input, 1024, stdin) == NULL) {
		/* EOF CASE */
		return -1;
	}
	parseCommand(input, command);
	while (validInput(command) == -1) {
		if (fgets(input, 1024, stdin) == NULL) {
			/* EOF CASE */
			return -1;
		}
		parseCommand(input, command);
	}
	return 0;
}

void commmandRouter(Cell** gameBoard, Cell** solvedBoard, Cell** tempBoard, int command[]) {
	switch (command[0]) {
	case 0:/* Set X Y Z */
		setCell(gameBoard, command[1], command[2], command[3]);
		break;
	case 1: /* Hint X Y */
		hintCell(solvedBoard, command[1], command[2]);
		break;
	case 2: /* validate */
		validateBoard(gameBoard, solvedBoard, tempBoard);
		break;
	case 3:/* restart */
		if(initGame(gameBoard, solvedBoard, tempBoard) == -1){
			/* EOF CASE */
			exitGame(gameBoard, solvedBoard, tempBoard);
		}
		break;
	case 4:
		exitGame(gameBoard, solvedBoard, tempBoard);
		break;
	}
}

void gameLoop() {
	char input[1024];
	int command[4];
	int exitFlag = 0;
	char* filePath = NULL;
	Cell** userBoard = NULL;
	Cell** solvedBoard = NULL;
	Cell** tempBoard = NULL;

	while (exitFlag == 0) {
		exitFlag = getInput(input, command, filePath);
		commmandRouter(userBoard, solvedBoard, tempBoard, command);
	}/* will take command of edit X and Solve X and initilize the boards */

	/*if (initGame(gameBoard, solvedBoard, tempBoard) == -1) {
		 EOF CASE
		exitGame(gameBoard, solvedBoard, tempBoard);
	} ** this one initializes the boards, need to move after allocating memory***/

}
