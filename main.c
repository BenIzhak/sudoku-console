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

extern Cell** userBoard;/*TODO:Remove */
extern int blockRowSize;/*TODO:Remove */
extern int blockColSize;/*TODO:Remove */

/* callsolver function is just for testing  */
void testing(char* boardStr){

	int i = 0;
	int j = 0;

	userBoard = setAllocatedMem();
	boardInit(userBoard);
	for(i = 0; i < (blockRowSize * blockColSize) ; i++){
		for(j = 0; j < (blockRowSize * blockColSize); j++){
			userBoard[i][j].currentNum = boardStr[(i * (blockRowSize * blockColSize)) + j] -'0';
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
}

int main(){
	char* boardStr;
	int seed = time(NULL);
	SP_BUFF_SET();


	boardStr = "134602000567090000028000000000203050070510090010006080701000200050040030400000000";

	/* 134602070567000020020000500600203050002500090012006080701000200250040030400020105 -- 0 solutions*/
	/* 134602070567090020928000516600203050072510090010006080701365240256840030400020105 -- 1 solutions */
	/* 134602070567000020020000500600203050002500090010006080701000200250040030400020105 -- 10 solutions */
	/* 134602000567090000028000000000203050070510090010006080701000200050040030400000000 -- 44 solutions */

	testing(boardStr);
	printBoard(userBoard);

	srand(seed);

	setCell(0,2,9);
	printf("%s","\n");
	printBoard(userBoard);


	return 0;
}
