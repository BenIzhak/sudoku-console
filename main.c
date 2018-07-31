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

/* callsolver function is just for testing  */
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
			}else{
				userBoard[i][j].fixed = 0;
			}
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

	boardStr = "134602000567090000028000000000203050070510090010006080701000200050040030400000000";

	/* 134602070567000020020000500600203050002500090012006080701000200250040030400020105 -- 0 solutions*/
	/* 134602070567090020928000516600203050072510090010006080701365240256840030400020105 -- 1 solutions */
	/* 134602070567000020020000500600203050002500090010006080701000200250040030400020105 -- 10 solutions */
	/* 134602000567090000028000000000203050070510090010006080701000200050040030400000000 -- 44 solutions */
	sols = callSolver(boardStr);
	printf("%s", "number of solutions is: -- ");
	printf("%d", sols);
	printf("%s", " --");

	srand(seed);

	gameLoop();

	return 1;
}

