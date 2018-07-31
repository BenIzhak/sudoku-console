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

Cell** setAllocatedMem(){
	int boardRowSize = 9; /*BLOCK_COL_SIZE * BLOCK_ROW_SIZE; will use variables from command router and will make them global */
	int boardColSize = 9; /* change back to original values */
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

/*void commmandRouter(Cell** userBoard, Cell** solvedBoard, Cell** tempBoard, int command[], char* filePath) {
	switch (command[0]) {
		case 0: solve X

			break;
		case 1:  edit X
			if(command[1] == -1){

			}
			break;
	}
}*/


void boardInit(Cell** table){/*TODO: NEED TO BE REMOVED, here just for exBacktrack testing */
	int i, j, k;
	int boardRow = 9;
	int boardCol = boardRow;
	for(i = 0; i < boardRow; i++){
		for(j = 0; j < boardCol; j++){

			for (k = 0; k < 9; k++) {/* initializes prevNums as not used nums*/
				table[i][j].prevNums[k] = 0;
			}

			table[i][j].currentNum = 0;
			table[i][j].fixed = 0;
			table[i][j].isInput = 0;
			table[i][j].limit = 9;

		}
	}
}

void gameLoop() {
	char input[256];
	int command[4];
	int exitFlag = 0;
	char* filePath = NULL;
	/*Cell** userBoard = NULL;
	Cell** solvedBoard = NULL;
	Cell** tempBoard = NULL;*/

	while (exitFlag == 0) {
		exitFlag = getInput(input, command, filePath);
		/*commmandRouter(userBoard, solvedBoard, tempBoard, command, filePath);*/
	}/* will take command of edit X and Solve X and initilize the boards */

	/*if (initGame(gameBoard, solvedBoard, tempBoard) == -1) {
		 EOF CASE
		exitGame(gameBoard, solvedBoard, tempBoard);
	} ** this one initializes the boards, need to move after allocating memory***/

}
