/*
 * main.c
 *
 *  Created on: 29 ����� 2018
 *      Author: beniz
 */
#include <stdio.h>
#include <time.h>
#include "Cell.h"
#include "SPBufferset.h"
#include <stdlib.h>
#include "MainAux.h"
#include "ExSolver.h"

int callSolver(char* boardStr){
	Cell** userBoard = NULL;
	int i = 0;
	int j = 0;

	userBoard = setAllocatedMem();
	boardInit(userBoard);
	for(i = 0; i < 9 ; i++){
		for(j = 0; j < 9; j++){
			userBoard[i][j].currentNum = boardStr[(i * 9) + j] -'0';
			if(userBoard[i][j].currentNum != 0){
				userBoard[i][j].fixed = 1;
			}
			userBoard[i][j].fixed = 0;
			userBoard[i][j].isError = 0;
			userBoard[i][j].isInput = 0;
			userBoard[i][j].limit = 0;
		}
	}

	return exBacktrack(userBoard);
}

int main(){
	int seed = time(NULL);
	int sols;
	char* boardStr;
	SP_BUFF_SET();

	boardStr = "090020500004005010060000003001800060000900002080072000500001070000300901003000000";

	sols = callSolver(boardStr);
	printf("%s", "number of solutions is: - ");
	printf("%d", sols);
	printf("%s", " -");

	srand(seed);

	gameLoop();

	return 1;
}

