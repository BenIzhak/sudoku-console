/*
 * game.c
 *
 *  Created on: 1 Aug 2018
 *      Author: Raz
 */
#include <stdio.h>
#include <stdlib.h>
#include "Cell.h"
#include "ExSolver.h"
#include "MainAux.h"
#include "CommandsList.h"
#include "ILPSolver.h"


static int markErrors = 1;/* TODO:each time we begin a puzzle we need to set it back to 1 */
static int errorsFlag = 0;/* TODO:reset back to 0 when starting a new board; flag which is 0 if there are no errors in board, 1 if there are errors */
extern int gameMode;
dll* commandsList;

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
	boardData brdData = getBoardData();
	for(i = 0; i < (brdData.blockRowSize * brdData.blockColSize); i++ ){
		for( j = 0; j < (brdData.blockRowSize * brdData.blockColSize); j++){
			if(brdData.userBoard[i][j].fixed == 0){
				if( (validAssignment(brdData.userBoard, brdData.userBoard[i][j].currentNum, i , j) == -1) ){
					brdData.userBoard[i][j].isError = 1;
					flag = 1;
					errorsFlag = flag;
				}else{
					brdData.userBoard[i][j].isError = 0;
				}
			}
		}
	}
	if(flag == 0){ errorsFlag = 0; }
}
/*
 * 	Function:  newSetCommand
 * --------------------
 *	this function adds a new set or an autofill to the commands list,
 *
 */
void newSetCommand(){
	boardData brdData = getBoardData();
	if(commandsList->currentNode != commandsList->lastNode){
		/* clear the redo part of the list */
		deleteFromCurrent(commandsList);
	}
	addCommand(commandsList, brdData.userBoard);
}

int setCell(int col, int row, int val){
	boardData brdData = getBoardData();
	if(brdData.userBoard[row][col].fixed == 1){/* no need to check which game mode because fixed cells are only available while in solve mode */
			return -1;
	}

	if(brdData.userBoard[row][col].currentNum == val){
		return 0;
	}

	brdData.userBoard[row][col].isInput = 1;
	brdData.userBoard[row][col].currentNum = val;

	findAndMarkErrors();

	newSetCommand();

	return 0;
}


void reset(){
	/* undo all moves, reverting the board to its original loaded state. */
	boardData brdData = getBoardData();
	(commandsList->currentNode) = (commandsList->head);
	deleteFromCurrent(commandsList);
	copyBoard(brdData.userBoard, (commandsList->currentNode)->info);
	printf("Board reset\n");
}

void hardReset(Cell** info){
	/* delete the whole command list and initialize a new one */
	if((commandsList->head) != NULL){
		deleteListNodes(commandsList);
	}
	initList(commandsList, info);
}

void undo(){
	dllNode *prevCommnad;
	boardData brdData = getBoardData();

	if((commandsList->currentNode) == (commandsList->head)){
		printf("%s","Error: no moves to undo\n");
		return;
	}

	prevCommnad = (commandsList->currentNode)->previous;
	boardDiff(commandsList, prevCommnad,"Undo");
	commandsList->currentNode = prevCommnad;
	copyBoard(brdData.userBoard, prevCommnad->info);
}

void redo(){
	dllNode *nextCommand;
	boardData brdData = getBoardData();

	if((commandsList->currentNode) == (commandsList->lastNode)){
		printf("%s","Error: no moves to redo\n");
		return;
	}

	nextCommand = (commandsList->currentNode)->next;
	boardDiff(commandsList, nextCommand,"Redo");
	nextCommand = (commandsList->currentNode)->next;
	commandsList->currentNode = nextCommand;
	copyBoard(brdData.userBoard, nextCommand->info);
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
	boardData brdData = getBoardData();

	for(i = 0; i < (brdData.blockRowSize * brdData.blockColSize); i++ ){
		for( j = 0; j < (brdData.blockRowSize * brdData.blockColSize); j++){
			if(brdData.userBoard[i][j].currentNum != 0){flag = 0;}
		}
	}
	return flag;
}

int fillAndKeep(int cellsToFill, int cellsToKeep){
	boardData brdData = getBoardData();
	int rowAndColSize = brdData.blockRowSize * brdData.blockColSize;
	int i, j, randCol, randRow, randNum, flag = 0;
	/*isSolved = 0;*/


	for(i = 0; i < cellsToFill; i++){
		while(flag == 0){
			randCol = rand() % rowAndColSize;
			randRow = rand() % rowAndColSize;
			/* checking that i havn't chose this cell already */
			if(brdData.userBoard[randRow][randCol].currentNum == 0){
				flag = 1;
			}
		}

		flag = 0;
		availableNumbers(brdData.userBoard, randRow, randCol);
		if(brdData.userBoard[randRow][randCol].limit == 0){
			return 0;
		}
		randNum = rand() % brdData.userBoard[randRow][randCol].limit; /* choose a random index from the validNums array */
		brdData.userBoard[randRow][randCol].currentNum = brdData.userBoard[randRow][randCol].validNums[randNum];
	}

	/*isSolved = ILPSolver();
	if(isSolved == 1){
		copyBoard(solvedBoard, tempBoard);
	}else{
		return 0;
		 error in ILPSolver, need to try again
	}*/

	flag = 0;
	/* now need to keep only cellsToKeep cells*/
	for(i = 0; i < cellsToKeep; i++){
		/*generate is only available in edit mode, so fixed field is not used.
		 * i'm using it for marking cells to keep */
		while(flag == 0){
			randCol = rand() % rowAndColSize;
			randRow = rand() % rowAndColSize;
			/* checking that i havn't chose this cell already */
			if(brdData.userBoard[randRow][randCol].fixed == 0){
				flag = 1;
				brdData.solvedBoard[randRow][randCol].fixed = 1;
			}
		}
		flag = 0;
	}

	/* going over cells with fixed = 0 and removing them from the board */
	for(i = 0; i < (brdData.blockRowSize * brdData.blockColSize); i++ ){
		for( j = 0; j < (brdData.blockRowSize * brdData.blockColSize); j++){
			if(brdData.solvedBoard[i][j].fixed == 0){
				brdData.solvedBoard[i][j].currentNum = 0;
			}else{
				brdData.solvedBoard[i][j].fixed = 0;
				/* no need to mark with fixed for now */
			}
		}
	}
	return 1;
}

int generate(int cellsToFill, int cellsToKeep){
	boardData brdData = getBoardData();
	int rowAndColSize = brdData.blockRowSize * brdData.blockColSize;
	int emptyCells = (rowAndColSize)*(rowAndColSize);/* if the board is empty, amount of empty cells is N*N */
	int i;


	if(isEmptyBoard() == 0){/* checks that board is empty */
		return 0;
	}

	if((cellsToFill > emptyCells) || (cellsToKeep > emptyCells)){/* checks that parameters are valid */
		return 1;
	}

	initBoardSolver(brdData.userBoard);/* getting ready to use ExSolver's functions */

	/* tries for 1000 iterations to get a valid board*/
	for(i = 0; i < 1000; i++){
		if(fillAndKeep(cellsToFill, cellsToKeep) == 1){
			exitSolver(brdData.userBoard);
			copyBoard(brdData.userBoard, brdData.solvedBoard);
			return 3;
		}else{
			boardInit(brdData.userBoard);
			boardInit(brdData.solvedBoard);
		}
	}
	exitSolver(brdData.userBoard);/* no need for ExSolver's fields any more, can free it. */
	return 2;/* generate failed */
}

void startDefaultBoard(){
	int boardRowAndColSize;
	boardData brdData = getBoardData();

	/* free memory of previous boards */
	freeBoardMem(brdData.userBoard);
	freeBoardMem(brdData.tempBoard);
	freeBoardMem(brdData.solvedBoard);

	/* set new values to blockRowSize and blockColSize */
	setBlockColSize(3);
	setBlockRowSize(3);

	/* allocate memory for news boards */
	boardRowAndColSize = brdData.blockColSize * brdData.blockRowSize;
	brdData.userBoard = setAllocatedMem(boardRowAndColSize);
	brdData.tempBoard = setAllocatedMem(boardRowAndColSize);
	brdData.solvedBoard = setAllocatedMem(boardRowAndColSize);

	/* init the boards */
	boardInit(brdData.userBoard);
	boardInit(brdData.tempBoard);
	boardInit(brdData.solvedBoard);

	if(commandsList == NULL){
		/* commandList doesn't exist, so create and initialize one */
		commandsList = allocateListMem();
		initList(commandsList, brdData.userBoard);
	}else{
		/* commandList exists, so update the command list */
		hardReset(brdData.userBoard);
	}
}
/* (-1) - this is a board with an error */
int autoFill(){
	/* TODO: do something about the case when the board doesn't change
	 * TODO: delete all the nodes after autofill command */
	int i,j;
	boardData brdData = getBoardData();

	if(errorsFlag){
		return -1;
	}

	boardInit(brdData.tempBoard);
	copyBoard(brdData.tempBoard, brdData.userBoard);/* copy the user board to tempboard, so i can make changes without changing the userboard */
	initBoardSolver(brdData.userBoard);

	for(i = 0; i < (brdData.blockRowSize * brdData.blockColSize); i++ ){
		for( j = 0; j < (brdData.blockRowSize * brdData.blockColSize); j++){
			availableNumbers(brdData.userBoard, i, j);/* checking available numbers in the original board */
			if(brdData.userBoard[i][j].limit == 1){/* there's only one number available */
				brdData.tempBoard[i][j].currentNum = brdData.tempBoard[i][j].validNums[0];
				printf("Cell <%d,%d> set to %d\n", j, i, brdData.tempBoard[i][j].validNums[0]);
			}
		}
	}


	copyBoard(brdData.userBoard, brdData.tempBoard);
	findAndMarkErrors();
	exitSolver(brdData.userBoard);
	newSetCommand();
	return 0;
}

void setHint(int col, int row){
	/* col and row starting with zero
	 * boardIsSolvable = 2 <-> board is unsolvable
	 * boardISSolvable = 1 <-> board is solvable */
	/*int boardIsSolvable;*/
	boardData brdData = getBoardData();
	if(getErrorsFlag()){
		printf("%s", "Error: board contains erroneous values\n");
		return;
	}
	if(brdData.userBoard[row][col].fixed == 1){
		printf("%s", "Error: cell is fixed\n");
		return;
	}
	if(brdData.userBoard[row][col].currentNum != 0){
		printf("%s", "Error: cell already contains a value\n");
		return;
	}
	/* TODO: uncomment the line below AND the decleration of boardIsSolvable */
	/*boardIsSolvable = ILPSolver();
	if(boardIsSolvable == 2){
		printf("%s", "Error: board is unsolvable\n");
		return;
	}else{
		userBoard[row][col].currentNum = solvedBoard[row][col].currentNum;
		userBoard[row][col].isInput = 1;
		printf("Hint: set cell to %d\n", solvedBoard[row][col].currentNum);
	}
	*/

}

void exitGame(){
	boardData brdData = getBoardData();
	/* free boards memory */
	freeBoardMem(brdData.userBoard);
	freeBoardMem(brdData.tempBoard);
	freeBoardMem(brdData.solvedBoard);

	/* free command list memory */
	deleteListNodes(commandsList);
	freeListMem(commandsList);
	printf("%s", "Exiting...\n");

}
