/*
 * game.c
 *
 *  Created on: 1 Aug 2018
 *      Author: Raz
 */
#include "Cell.h"
#include "ExSolver.h"
#include <stdlib.h>
#include "MainAux.h"
#include "CommandsList.h"
#include <stdio.h>


static int markErrors = 1;/* TODO:each time we begin a puzzle we need to set it back to 1 */
static int errorsFlag = 0;/* flag which is 0 if there are no errors in board, 1 if there are errors */
extern Cell** userBoard;
extern Cell** solvedBoard;
extern Cell** tempBoard;
extern int gameMode;
extern int blockRowSize;
extern int blockColSize;
dllNode** head;
dllNode** lastNode;
dllNode** currentNode;

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

	addCommand(head, lastNode, currentNode, userBoard);

	return 0;
}


void reset(){
	/* undo all moves, reverting the board to its original loaded state. */
	(*currentNode) = (*head);
	deleteFromCurrent(lastNode,currentNode);
	copyBoard(userBoard, (*currentNode)->info);
	printf("Board reset\n");
}

void hardReset(Cell** info){
	/* delete the whole command list and initialize a new one */
	if(head != NULL){
		deleteList(head, lastNode, currentNode);
	}
	initList(head, lastNode, currentNode, info);
}

void undo(){
	dllNode *prevCommnad;
	if(currentNode == head){
		printf("%s","Error: no moves to undo\n");
		return;
	}
	boardDiff(currentNode, &((*currentNode)->previous),"Undo");
	prevCommnad = (*currentNode)->previous;
	currentNode = &prevCommnad;
	copyBoard(userBoard, (*currentNode)->info);
}

void redo(){
	dllNode *nextCommand;
	if(currentNode == lastNode){
		printf("%s","Error: no moves to redo\n");
		return;
	}
	boardDiff(currentNode, &((*currentNode)->next) ,"Redo");
	nextCommand = (*currentNode)->next;
	currentNode = &nextCommand;
	copyBoard(userBoard, (*currentNode)->info);
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

int fillAndKeep(int cellsToFill, int cellsToKeep){/*TODO: debug carefully after adding the call to ILPSOLVER */
	int rowAndColSize = blockRowSize * blockColSize;
	int i, j, randCol, randRow, randNum, flag = 0;

	for(i = 0; i < cellsToFill; i++){
		while(flag == 0){
			randCol = rand() % rowAndColSize;
			randRow = rand() % rowAndColSize;
			/* checking that i havn't chose this cell already */
			if(userBoard[randRow][randCol].currentNum == 0){
				flag = 1;
			}
		}

		availableNumbers(userBoard, randRow, randCol);
		if(userBoard[randRow][randCol].limit == 0){
			return 0;
		}
		randNum = rand() % userBoard[randRow][randCol].limit; /* choose a random index from the validNums array */

		userBoard[randRow][randCol].currentNum = userBoard[randRow][randCol].validNums[randNum];
	}

	/* TODO: add call to ILPsolver and save it in solvedBoard
	 *
	 * also check if ILP says there is a solution, if not return 0
	 *
	 *  */

	flag = 0;
	for(i = 0; i < cellsToKeep; i++){
		/*generate is only available in edit mode, so fixed field is not used.
		 * i'm using it just to mark cells to keep */
		while(flag == 0){
			randCol = rand() % rowAndColSize;
			randRow = rand() % rowAndColSize;
			/* checking that i havn't chose this cell already */
			if(userBoard[randRow][randCol].fixed == 0){
				flag = 1;
				solvedBoard[randRow][randCol].fixed = 1;
			}
		}
	}

	for(i = 0; i < (blockRowSize * blockColSize); i++ ){
		for( j = 0; j < (blockRowSize * blockColSize); j++){
			if(solvedBoard[i][j].fixed == 0){
				solvedBoard[i][j].currentNum = 0;
			}else{
				solvedBoard[i][j].fixed = 0;
				/* no need to mark with fixed for now */
			}
		}
	}
	return 1;
}

int generate(int cellsToFill, int cellsToKeep){
	int rowAndColSize = blockRowSize * blockColSize;
	int emptyCells = (rowAndColSize)*(rowAndColSize);/* if the board is empty, amount of empty cells is N*N */
	int i;

	if(isEmptyBoard() == 0){/* checks that board is empty */
		return 0;
	}

	if((cellsToFill > emptyCells) || (cellsToKeep > emptyCells)){/* checks that parameters are valid */
		return 1;
	}

	/* tries 1000 to get a valid board*/
	for(i = 0; i < 1000; i++){
		if(fillAndKeep(cellsToFill, cellsToKeep) == 1){
			return 3;
		}else{
			boardInit(userBoard);
			boardInit(solvedBoard);
		}
	}

	return 2;
	/*TODO: should i move the solution to the user board?? yes, use copyboard */
}

void startDefaultBoard(){
	int boardRowAndColSize;

	/* free memory of previous boards */
	freeBoardMem(userBoard, blockRowSize, blockColSize);
	freeBoardMem(tempBoard, blockRowSize, blockColSize);
	freeBoardMem(solvedBoard, blockRowSize, blockColSize);

	/* set new values to blockRowSize and blockColSize */
	blockColSize = 3;
	blockRowSize = 3;

	/* allocate memory for news boards */
	boardRowAndColSize = blockColSize * blockRowSize;
	userBoard = setAllocatedMem(boardRowAndColSize);
	tempBoard = setAllocatedMem(boardRowAndColSize);
	solvedBoard = setAllocatedMem(boardRowAndColSize);

	/* init the boards */
	boardInit(userBoard);
	boardInit(tempBoard);
	boardInit(solvedBoard);

	/* update the command list */
	hardReset(userBoard);
	initList(head, lastNode, currentNode, userBoard);
}

