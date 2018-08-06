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
#include "FilesHandler.h"
#include "game.h"
#include "ExSolver.h"

Cell** userBoard;/* TODO: delete, just for testing */
Cell** solvedBoard;/* TODO: delete, just for testing */
Cell** tempBoard;/* TODO: delete, just for testing */

/* callsolver function is just for testing  */
int callSolver(char* boardStr){
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
	/*int code;*/
	char* boardStr;
	int sols,i;
	SP_BUFF_SET();
	/*int seed = time(NULL);


	srand(seed);

	gameLoop();*/

	boardStr = "134602000567090000028000000000203050070510090010006080701000200050040030400000000";

	/* 134602070567000020020000500600203050002500090012006080701000200250040030400020105 -- 0 solutions*/
	/* 134602070567090020928000516600203050072510090010006080701365240256840030400020105 -- 1 solutions */
	/* 134602070567000020020000500600203050002500090010006080701000200250040030400020105 -- 10 solutions */
	/* 134602000567090000028000000000203050070510090010006080701000200050040030400000000 -- 44 solutions */
	for(i = 0; i < 1000; i++){
		sols = callSolver(boardStr);
		printf("%s", "number of solutions is: -- ");
		printf("%d", sols);
		printf("%s", " --\n");
	}

	/*
	userBoard = setAllocatedMem();
	solvedBoard = setAllocatedMem();
	tempBoard = setAllocatedMem();

	boardInit(userBoard);
	boardInit(solvedBoard);
	boardInit(tempBoard);
	code = generate(20, 5);
	switch(code){
		case 0:
			printf("%s", "not empty");
			break;
		case 1:
			printf("%s", "parameters are not valid");
			break;
		case 2:
			printf("%s","generator failed");
			break;
		case 3:
			printBoard(userBoard);
			break;
		default:
			break;
	}
	printf("%s","\n");
	*/
	return 0;
}
