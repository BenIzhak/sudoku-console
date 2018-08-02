/*
 * game.c
 *
 *  Created on: 1 Aug 2018
 *      Author: Raz
 */
#include "Cell.h"
#include "ExSolver.h"
#include <stdlib.h>


static int markErrors = 1;/* TODO:each time we begin a puzzle we need to set it back to 1 */
static int errorsFlag = 0;/* flag which is 0 if there are no errors in board, 1 if there are errors */
extern Cell** userBoard;
extern Cell** solvedBoard;
extern Cell** tempBoard;
extern int gameMode;
extern int blockRowSize;
extern int blockColSize;

void setMarkErrors(int setting){
	markErrors = setting;
}

int getMarkErrors(){
	return markErrors;
}

/*
 * Function:  getErrorsFlag
 * --------------------
 * 	returns errorsFlag value
 *
 *
 */
int getErrorsFlag(){
	return errorsFlag;
}

void findAndMarkErrors(){
	/*TODO:if needed, can speed up by going over just cells in same row/col/block as the cell which was changed*/
	int i, j;
	int flag = 0;
	for(i = 0; i < (blockRowSize * blockColSize); i++ ){
		for( j = 0; j < (blockRowSize * blockColSize); j++){
			if(userBoard[i][j].fixed == 0){
				if( (validAssignment(userBoard, userBoard[i][j].currentNum, i , j) == -1) ){
					userBoard[i][j].isError = 1;
					flag = 1;
					errorsFlag = flag;
				}else{
					userBoard[i][j].isError = 0;
				}
			}
		}
	}
	if(flag == 0){ errorsFlag = 0; }
}

int setCell(int col, int row, int val){
	if(userBoard[row][col].fixed == 1){/* no need to check which game mode because fixed cells are only available while in solve mode */
			return -1;
	}

	if(userBoard[row][col].currentNum == val){
		return 0;
	}

	userBoard[row][col].isInput = 1;
	userBoard[row][col].currentNum = val;

	findAndMarkErrors();

	return 0;
}



int validate(){

	if(errorsFlag == 1){
		return 0;
	}

	/*TODO:add call to ILPSolver and print message if board is solvable or not */
	return 2;
}

int isEmptyBoard(){

	int i, j, flag = 1;
	for(i = 0; i < (blockRowSize * blockColSize); i++ ){
		for( j = 0; j < (blockRowSize * blockColSize); j++){
			if(userBoard[i][j].currentNum != 0){flag = 0;}
		}
	}
	return flag;
}

int generate(int cellsToFill, int cellsToKeep){
	int rowAndColSize = blockRowSize * blockColSize;
	int emptyCells = (rowAndColSize)*(rowAndColSize);/* if the board is empty, amount of empty cells is N*N */
	int i, randCol, randRow, randNum;

	if(isEmptyBoard() == 0){
		return 0;
	}

	if((cellsToFill > emptyCells) || (cellsToKeep > emptyCells)){
		return 1;
	}

	for(i = 0; i < cellsToFill; i++){
		/* TODO: need to choose a LEGAL! random value for cell, in sense of row/block/col (legal = between 1 to N)*/
		randCol = rand() % rowAndColSize;
		randRow = rand() % rowAndColSize;
		randNum = rand() % rowAndColSize;
		userBoard[randRow][randCol].currentNum = randNum;
		/* removing values is from solvedBoard */
	}
	return 0;/*TODO: just to make it compile */
}
