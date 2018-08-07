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
#include "filesHandler.h"
#include "def.h"


static int gameMode;


static int markErrors;/* TODO:each time we begin a puzzle we need to set it back to 1 */
static int errorsFlag = 0;/* TODO:reset back to 0 when starting a new board; flag which is 0 if there are no errors in board, 1 if there are errors */
static dll* commandsList;


int getGameMode(){
	return gameMode;
}

void setGameMode(int modeNum){
	gameMode = modeNum;
}



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
	Cell** userBoard = getUserBoard();
	for(i = 0; i < (brdData.blockRowSize * brdData.blockColSize); i++ ){
		for( j = 0; j < (brdData.blockRowSize * brdData.blockColSize); j++){
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
/*
 * 	Function:  newSetCommand
 * --------------------
 *	this function adds a new set or an autofill to the commands list,
 *
 */
void newSetCommand(){
	Cell** userBoard = getUserBoard();
	if(commandsList->currentNode != commandsList->lastNode){
		/* clear the redo part of the list */
		deleteFromCurrent(commandsList);
	}
	addCommand(commandsList, userBoard);
}

int setCell(int col, int row, int val){
	Cell** userBoard = getUserBoard();
	if(userBoard[row][col].fixed == 1){/* no need to check which game mode because fixed cells are only available while in solve mode */
			return -1;
	}

	if(userBoard[row][col].currentNum == val){
		return 0;
	}

	userBoard[row][col].isInput = 1;
	userBoard[row][col].currentNum = val;

	findAndMarkErrors();

	newSetCommand();

	return 0;
}

void reset(){
	/* undo all moves, reverting the board to its original loaded state. */
	Cell** userBoard = getUserBoard();
	(commandsList->currentNode) = (commandsList->head);
	deleteFromCurrent(commandsList);
	copyBoard(userBoard, (commandsList->currentNode)->info);
	printf("Board reset\n");
}

void hardReset(Cell** info){
	/* delete the whole command list and initialize a new one */
	if((commandsList->head) != NULL){
		deleteListNodes(commandsList);
	}
	initList(commandsList, info);
}

void startNewCommandsList(){
	/* if commandsList does not exist -> create new one
	 * if commandsList exist -> hardReset it*/
	Cell** userBoard = getUserBoard();
	if(commandsList == NULL){
		/* commandList doesn't exist, so create and initialize one */
		commandsList = allocateListMem();
		initList(commandsList, userBoard);
	}else{
		/* commandList exists, so update the command list */
		hardReset(userBoard);
	}
}

void undo(){
	dllNode *prevCommnad;
	Cell** userBoard = getUserBoard();

	if((commandsList->currentNode) == (commandsList->head)){
		printf("%s","Error: no moves to undo\n");
		return;
	}

	prevCommnad = (commandsList->currentNode)->previous;
	boardDiff(commandsList, prevCommnad,"Undo");
	commandsList->currentNode = prevCommnad;
	copyBoard(userBoard, prevCommnad->info);
}

void redo(){
	dllNode *nextCommand;
	Cell** userBoard = getUserBoard();

	if((commandsList->currentNode) == (commandsList->lastNode)){
		printf("%s","Error: no moves to redo\n");
		return;
	}

	nextCommand = (commandsList->currentNode)->next;
	boardDiff(commandsList, nextCommand,"Redo");
	nextCommand = (commandsList->currentNode)->next;
	commandsList->currentNode = nextCommand;
	copyBoard(userBoard, nextCommand->info);
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
	Cell** userBoard = getUserBoard();
	for(i = 0; i < (brdData.blockRowSize * brdData.blockColSize); i++ ){
		for( j = 0; j < (brdData.blockRowSize * brdData.blockColSize); j++){
			if(userBoard[i][j].currentNum != 0){flag = 0;}
		}
	}
	return flag;
}

int fillAndKeep(int cellsToFill, int cellsToKeep){
	boardData brdData = getBoardData();
	Cell** userBoard = getUserBoard();
	Cell** solvedBoard = getSolvedBoard();
	int rowAndColSize = brdData.blockRowSize * brdData.blockColSize;
	int i, j, randCol, randRow, randNum, flag = 0;
	/*isSolved = 0;*/


	for(i = 0; i < cellsToFill; i++){
		while(flag == 0){
			randCol = rand() % rowAndColSize;
			randRow = rand() % rowAndColSize;
			/* checking that i havn't chose this cell already */
			if(userBoard[randRow][randCol].currentNum == 0){
				flag = 1;
			}
		}

		flag = 0;
		availableNumbers(userBoard, randRow, randCol);
		if(userBoard[randRow][randCol].limit == 0){
			return 0;
		}
		randNum = rand() % userBoard[randRow][randCol].limit; /* choose a random index from the validNums array */
		userBoard[randRow][randCol].currentNum = userBoard[randRow][randCol].validNums[randNum];
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
			if(userBoard[randRow][randCol].fixed == 0){
				flag = 1;
				solvedBoard[randRow][randCol].fixed = 1;
			}
		}
		flag = 0;
	}

	/* going over cells with fixed = 0 and removing them from the board */
	for(i = 0; i < (brdData.blockRowSize * brdData.blockColSize); i++ ){
		for( j = 0; j < (brdData.blockRowSize * brdData.blockColSize); j++){
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
	boardData brdData = getBoardData();
	Cell** userBoard = getUserBoard();
	Cell** solvedBoard = getSolvedBoard();
	int rowAndColSize = brdData.blockRowSize * brdData.blockColSize;
	int emptyCells = (rowAndColSize)*(rowAndColSize);/* if the board is empty, amount of empty cells is N*N */
	int i;


	if(isEmptyBoard() == 0){/* checks that board is empty */
		return 0;
	}

	if((cellsToFill > emptyCells) || (cellsToKeep > emptyCells)){/* checks that parameters are valid */
		return 1;
	}

	initBoardSolver(userBoard);/* getting ready to use ExSolver's functions */

	/* tries for 1000 iterations to get a valid board*/
	for(i = 0; i < 1000; i++){
		if(fillAndKeep(cellsToFill, cellsToKeep) == 1){
			exitSolver(userBoard);
			copyBoard(userBoard, solvedBoard);
			return 3;
		}else{
			boardInit(userBoard);
			boardInit(solvedBoard);
		}
	}
	exitSolver(userBoard);/* no need for ExSolver's fields any more, can free it. */
	return 2;/* generate failed */
}

void startDefaultBoard(){
	/* TODO: add constants */
	int boardRowAndColSize;
	boardData brdData = getBoardData();
	Cell** userBoard = getUserBoard();
	Cell** solvedBoard = getSolvedBoard();
	Cell** tempBoard = getTempBoard();

	/* free memory of previous boards */
	freeBoardMem(userBoard);
	freeBoardMem(tempBoard);
	freeBoardMem(solvedBoard);

	/* set new values to blockRowSize and blockColSize */

	setBlockColSize(DEFAULT_BLOCK_COL_SIZE);
	setBlockRowSize(DEFAULT_BLOCK_ROW_SIZE);


	/* allocate memory for news boards */
	boardRowAndColSize = brdData.blockColSize * brdData.blockRowSize;
	userBoard = setAllocatedMem(boardRowAndColSize);
	tempBoard = setAllocatedMem(boardRowAndColSize);
	solvedBoard = setAllocatedMem(boardRowAndColSize);

	/* init the boards */
	boardInit(userBoard);
	boardInit(tempBoard);
	boardInit(solvedBoard);

	startNewCommandsList();

}
/* (-1) - this is a board with an error */
int autoFill(){
	/* TODO: do something about the case when the board doesn't change
	 * TODO: delete all the nodes after autofill command */
	int i,j;
	boardData brdData = getBoardData();
	Cell** userBoard = getUserBoard();
	Cell** tempBoard = getTempBoard();
	int anyChanges = 0;
	if(errorsFlag){
		return -1;
	}

	boardInit(tempBoard);
	copyBoard(tempBoard, userBoard);/* copy the user board to tempboard, so i can make changes without changing the userboard */
	initBoardSolver(userBoard);

	for(i = 0; i < (brdData.blockRowSize * brdData.blockColSize); i++ ){
		for( j = 0; j < (brdData.blockRowSize * brdData.blockColSize); j++){
			availableNumbers(userBoard, i, j);/* checking available numbers in the original board */
			if(userBoard[i][j].limit == 1){/* there's only one number available */
				anyChanges = 1;
				tempBoard[i][j].currentNum = tempBoard[i][j].validNums[0];
				printf("Cell <%d,%d> set to %d\n", j, i, tempBoard[i][j].validNums[0]);
			}
		}
	}


	copyBoard(userBoard, tempBoard);
	findAndMarkErrors();

	exitSolver(userBoard);
	if(anyChanges){
		newSetCommand();
	}
	return 0;
}

void setHint(int col, int row){
	/* col and row starting with zero
	 * boardIsSolvable = 2 <-> board is unsolvable
	 * boardISSolvable = 1 <-> board is solvable */
	/*int boardIsSolvable;*/
	Cell** userBoard = getUserBoard();

	if(getErrorsFlag()){
		printf("%s", "Error: board contains erroneous values\n");
		return;
	}
	if(userBoard[row][col].fixed == 1){
		printf("%s", "Error: cell is fixed\n");
		return;
	}
	if(userBoard[row][col].currentNum != 0){
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


void solveCommand(char* filePath){
	gameMode = SOLVE_MODE;
	markErrors = 1;
	if(loadBoard(filePath) == -1){
		printf("%s", "Error: File cannot be opened\n");
	}
}

void editCommand(char* filePath , int numOfArgs){
	gameMode = EDIT_MODE;
	markErrors = 1;
	if(numOfArgs > 0){
		if(loadBoard(filePath) == -1){
			printf("%s", "Error: File cannot be opened\n");
		}
	}else{
		startDefaultBoard();
	}
}

void exitGameCommand(){

	Cell** userBoard = getUserBoard();
	Cell** tempBoard = getTempBoard();
	Cell** solvedBoard = getSolvedBoard();
	/* free boards memory */
	freeBoardMem(userBoard);
	freeBoardMem(tempBoard);
	freeBoardMem(solvedBoard);

	/* free command list memory */
	deleteListNodes(commandsList);
	freeListMem(commandsList);
	printf("%s", "Exiting...\n");

}


