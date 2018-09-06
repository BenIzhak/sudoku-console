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
#include "FilesHandler.h"
#include "def.h"


static int gameMode;
extern Cell** userBoard;
extern Cell** tempBoard;
extern Cell** solvedBoard;
static int markErrors;/* TODO:each time we begin a puzzle we need to set it back to 1 */
static int errorsFlag = 0;/* TODO:reset back to 0 when starting a new board; flag which is 0 if there are no errors in board, 1 if there are errors */
static dll* commandsList;


/*
 * -------------------------------
 * getGameMode Documentation is in header file
 * -------------------------------
 */
int getGameMode(){
	return gameMode;
}

/*
 * -------------------------------
 * setGameMode Documentation is in header file
 * -------------------------------
 */
void setGameMode(int modeNum){
	gameMode = modeNum;
}


/*
 * -------------------------------
 * setMarkErrors Documentation is in header file
 * -------------------------------
 */
void setMarkErrors(int setting){
	if(setting == 0 || setting == 1){
		markErrors = setting;
	}else{
		printf("%s","Error: the value should be 0 or 1\n");
	}
}

/*
 * -------------------------------
 * getMarkErrors Documentation is in header file
 * -------------------------------
 */
int getMarkErrors(){
	return markErrors;
}

/*
 * -------------------------------
 * getErrorsFlag Documentation is in header file
 * -------------------------------
 */
int getErrorsFlag(){
	return errorsFlag;
}

/*
 * -------------------------------
 * findAndMarkErrors Documentation is in header file
 * -------------------------------
 */
void findAndMarkErrors(){
	/*TODO:if needed, can speed up by going over just cells in same row/col/block as the cell which was changed*/
	int i, j;
	int flag = 0;
	boardData brdData = getBoardData();
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
	if(commandsList->currentNode != commandsList->lastNode){
		/* clear the redo part of the list */
		deleteFromCurrent(commandsList);
	}
	addCommand(commandsList, userBoard, errorsFlag);
}

/*
 * -------------------------------
 * validate Documentation is in header file
 * -------------------------------
 */
void validateCommand(int isDone){
	int isSolved = 0;
	if(errorsFlag == 1){
		if(isDone == 0){
			printf("%s", "Error: board contains erroneous values\n");
		}else{
			printf("%s", "Puzzle solution erroneous\n");
		}
	}else{
		/*TODO:add call to ILPSolver and print message if board is solvable or not */
		isSolved = ILPSolver();
		if(isSolved == 1){
			if(isDone == 0){
				printf("%s", "Validation passed: board is solvable\n");
			}else{
				/*game is done and solution is correct*/
				printf("%s", "Puzzle solved successfully\n");
				gameMode = INIT_MODE;
			}
		}else{
			if(isDone == 0){
				printf("%s", "Validation failed: board is unsolvable\n");
			}else{
				printf("%s", "Puzzle solution erroneous\n");
			}
			 /*error in ILPSolver, need to try again*/
		}
	}
}

/*
 * 	Function:  isFinished
 * --------------------
 *	checks if the user has finished filling all the board's cells
 *	and then uses validate to check his solution
 *
 *	board:  2d array containing sudoku cells
 */
void isFinished(Cell** board){
	int flag = 0, i, j;
	boardData brdData = getBoardData();
	for(i = 0; i < (brdData.blockRowSize * brdData.blockColSize); i++){
		for(j = 0; j < (brdData.blockRowSize * brdData.blockColSize); j++){
			if(board[i][j].currentNum == 0){
				flag = 1;
			}
		}
	}

	if(flag == 0 && gameMode == SOLVE_MODE){
		validateCommand(1);
	}
}

/*
 * -------------------------------
 * setCell Documentation is in header file
 * -------------------------------
 */
void setCell(int col, int row, int val){
	if(userBoard[row][col].fixed == 1){/* no need to check which game mode because fixed cells are only available while in solve mode */
		printf("%s", "Error: cell is fixed\n");
		return;
	}

	userBoard[row][col].isInput = 1;
	userBoard[row][col].currentNum = val;

	findAndMarkErrors();

	newSetCommand();
	printBoard(userBoard);
	isFinished(userBoard);
}

/*
 * -------------------------------
 * resetCommand Documentation is in header file
 * -------------------------------
 */
void resetCommand(){
	/* undo all moves, reverting the board to its original loaded state. */
	(commandsList->currentNode) = (commandsList->head);
	deleteFromCurrent(commandsList);
	copyBoard(userBoard, (commandsList->currentNode)->info);
	printf("Board reset\n");
}

/*
 * -------------------------------
 * hardReset Documentation is in header file
 * -------------------------------
 */
void hardReset(Cell** info){
	if((commandsList->head) != NULL){
		deleteListNodes(commandsList);
	}
	initList(commandsList, info, errorsFlag);
}

/*
 * -------------------------------
 * startNewCommandsList Documentation is in header file
 * -------------------------------
 */
void startNewCommandsList(){
	/* if commandsList does not exist -> create new one
	 * if commandsList exist -> hardReset it*/
	if(commandsList == NULL){
		/* commandList doesn't exist, so create and initialize one */
		commandsList = allocateListMem();
		initList(commandsList, userBoard, errorsFlag);
	}else{
		/* commandList exists, so init the command list */
		hardReset(userBoard);
	}
}

/*
 * -------------------------------
 * undoCommand Documentation is in header file
 * -------------------------------
 */
void undoCommand(){
	dllNode *prevCommand;

	if((commandsList->currentNode) == (commandsList->head)){
		printf("%s","Error: no moves to undo\n");
		return;
	}

	prevCommand = (commandsList->currentNode)->previous;
	printBoard(prevCommand->info);
	boardDiff(commandsList, prevCommand,"Undo");
	commandsList->currentNode = prevCommand;
	copyBoard(userBoard, prevCommand->info);
	errorsFlag = prevCommand->boardContainError;
}

/*
 * -------------------------------
 * redoCommand Documentation is in header file
 * -------------------------------
 */
void redoCommand(){
	dllNode *nextCommand;
	if((commandsList->currentNode) == (commandsList->lastNode)){
		printf("%s","Error: no moves to redo\n");
		return;
	}
	nextCommand = (commandsList->currentNode)->next;
	printBoard(nextCommand->info);
	boardDiff(commandsList, nextCommand,"Redo");
	commandsList->currentNode = nextCommand;
	copyBoard(userBoard, nextCommand->info);
	errorsFlag = nextCommand->boardContainError;
}

/*
 * Function:  isEmptyBoard
 * --------------------
 * goes over all cells and checks if the board is empty
 *
 *	return 0  if board is not empty, 1 if the board is empty
 *
 */
int isEmptyBoard(){
	int i, j, flag = 1;
	boardData brdData = getBoardData();

	for(i = 0; i < (brdData.blockRowSize * brdData.blockColSize); i++ ){
		for( j = 0; j < (brdData.blockRowSize * brdData.blockColSize); j++){
			if(userBoard[i][j].currentNum != 0){flag = 0;}
		}
	}
	return flag;
}

/*
 * Function:  fillAndKeep
 * --------------------
 *	randomizing cellsToFill cells and then choosing a random legal value for each cell
 *	then solves the filled board using the ILPSolver
 *
 *	randomizing cellsToKeep cells and keeping only them
 *
 *	return 0  if failed, 1 if successful
 *
 *
 *	cellsToFill: amount of cells to fill with random values
 *	cellsToKeep: amount of cells to keep after generating random values
 */
int fillAndKeep(int cellsToFill, int cellsToKeep){
	boardData brdData = getBoardData();
	int rowAndColSize = brdData.blockRowSize * brdData.blockColSize;
	int i, j, randCol, randRow, randNum, flag = 0;
	int isSolved = 0;


	for(i = 0; i < cellsToFill; i++){
		while(flag == 0){

			randCol = rand() % rowAndColSize;
			randRow = rand() % rowAndColSize;
			/* checking that i havn't chose this cell already */
			/* if it was chosen before it's currentNum wont be 0 */
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

	isSolved = ILPSolver();
	if(isSolved != 1){
		/* error in ILPSolver, need to try again*/
		return 0;
	}

	flag = 0;
	/* now need to keep only cellsToKeep cells*/
	for(i = 0; i < cellsToKeep; i++){
		/*generate is only available in edit mode, so fixed field is not used.
		 * i'm using it for marking cells to keep */
		while(flag == 0){
			randCol = rand() % rowAndColSize;
			randRow = rand() % rowAndColSize;

			/* checking that i havn't chose this cell already */
			if(solvedBoard[randRow][randCol].fixed == 0){
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

/*
 * -------------------------------
 * generateCommand Documentation is in header file
 * -------------------------------
 */
void generateCommand(int cellsToFill, int cellsToKeep){
	boardData brdData = getBoardData();
	int rowAndColSize = brdData.blockRowSize * brdData.blockColSize;
	int emptyCells = (rowAndColSize)*(rowAndColSize);/* if the board is empty, amount of empty cells is N*N */
	int i;

	if((cellsToFill > emptyCells) || (cellsToKeep > emptyCells)){/* checks that parameters are valid */
		printf("Error: value not in range 0-%d\n", emptyCells);
		return;
	}

	if(isEmptyBoard() == 0){/* checks that board is empty */
		printf("%s", "Error: board is not empty\n");
		return;
	}

	initBoardSolver(userBoard);/* getting ready to use ExSolver's functions */

	/* tries for 1000 iterations to get a valid board*/
	for(i = 0; i < 1000; i++){
		if(fillAndKeep(cellsToFill, cellsToKeep) == 1){
			exitSolver(userBoard);
			copyBoard(userBoard, solvedBoard);
			newSetCommand();
			printBoard(userBoard);
			return;/* Done successfully */
		}else{
			boardInit(userBoard);
			boardInit(solvedBoard);
		}
	}
	exitSolver(userBoard);/* no need for ExSolver's fields any more, can free it. */
	printf("%s", "Error: puzzle generator failed\n");
}

/*
 * -------------------------------
 * startDefaultBoard Documentation is in header file
 * -------------------------------
 */
void startDefaultBoard(){
	int boardRowAndColSize;
	boardData brdData = getBoardData();
	/* free memory of previous boards */
	freeBoardMem(userBoard, brdData.blockRowSize, brdData.blockColSize);
	freeBoardMem(tempBoard, brdData.blockRowSize, brdData.blockColSize);
	freeBoardMem(solvedBoard, brdData.blockRowSize, brdData.blockColSize);

	/* set new values to blockRowSize and blockColSize */
	setBlockColSize(DEFAULT_BLOCK_COL_SIZE);
	setBlockRowSize(DEFAULT_BLOCK_ROW_SIZE);


	/* allocate memory for news boards */
	boardRowAndColSize = DEFAULT_BLOCK_COL_SIZE * DEFAULT_BLOCK_ROW_SIZE;
	userBoard = setAllocatedMem(boardRowAndColSize);
	tempBoard = setAllocatedMem(boardRowAndColSize);
	solvedBoard = setAllocatedMem(boardRowAndColSize);

	/* init the boards */
	boardInit(userBoard);
	boardInit(tempBoard);
	boardInit(solvedBoard);

	/* change errorFlag back to 0 */
	errorsFlag = 0;

	startNewCommandsList();
}

/*
 * -------------------------------
 * autoFillCommand Documentation is in header file
 * -------------------------------
 */
void autoFillCommand(){
	int i,j;
	boardData brdData = getBoardData();
	int anyChanges = 0;
	if(errorsFlag){
		printf("%s", "Error: board contains erroneous values\n");
		return;
	}

	boardInit(tempBoard);
	copyBoard(tempBoard, userBoard);/* copy the user board to tempboard, so i can make changes without changing the userboard */
	initBoardSolver(userBoard);

	for(i = 0; i < (brdData.blockRowSize * brdData.blockColSize); i++ ){
		for( j = 0; j < (brdData.blockRowSize * brdData.blockColSize); j++){
			if(tempBoard[i][j].currentNum == 0){
				availableNumbers(userBoard, i, j);/* checking available numbers in the original board */
				if(userBoard[i][j].limit == 1){/* there's only one number available */
					anyChanges = 1;
					tempBoard[i][j].currentNum = userBoard[i][j].validNums[0];
					printf("Cell <%d,%d> set to %d\n", (j + 1), (i + 1), userBoard[i][j].validNums[0]);
				}
			}
		}
	}


	copyBoard(userBoard, tempBoard);
	findAndMarkErrors();

	exitSolver(userBoard);
	if(anyChanges){
		newSetCommand();
	}
	printBoard(userBoard);
	isFinished(userBoard);
}

/*
 * -------------------------------
 * setHint Documentation is in header file
 * -------------------------------
 */
void setHint(int col, int row){
	/* col and row starting with zero
	 * boardIsSolvable = 2 <-> board is unsolvable
	 * boardISSolvable = 1 <-> board is solvable */
	int boardIsSolvable;

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
	boardIsSolvable = ILPSolver();
	if(boardIsSolvable == 2){
		printf("%s", "Error: board is unsolvable\n");
	}else{
		printf("Hint: set cell to %d\n", solvedBoard[row][col].currentNum);
	}


}

/*
 * -------------------------------
 * solveCommand Documentation is in header file
 * -------------------------------
 */
void solveCommand(char* filePath){
	gameMode = SOLVE_MODE;
	markErrors = 1;
	if(loadBoard(filePath, gameMode) == -1){
		printf("%s", "Error: File doesn’t exist or cannot be opened\n");
		return;
	}
	findAndMarkErrors();
}

/*
 * -------------------------------
 * editCommand Documentation is in header file
 * -------------------------------
 */
void editCommand(char* filePath , int numOfArgs){
	gameMode = EDIT_MODE;
	markErrors = 1;
	if(numOfArgs > 0){
		if(loadBoard(filePath, gameMode) == -1){
			printf("%s", "Error: File cannot be opened\n");
			return;
		}
		findAndMarkErrors();
	}else{
		startDefaultBoard();
	}

}

/*
 * -------------------------------
 * saveCommand Documentation is in header file
 * -------------------------------
 */
void saveCommand(char* filePath){
	int i, j;
	FILE* fp;
	boardData brdData = getBoardData();
	int boardRowAndColSize = brdData.blockColSize * brdData.blockRowSize;
	if(gameMode == EDIT_MODE){
		if(errorsFlag){
			printf("%s", "Error: board contains erroneous values\n");
			return;
		}

		if(!(ILPSolver() == 1)){
			printf("%s", "Error: board validation failed\n");
			return;
		}

	}
	fp = openFile(filePath,"w");
	if(fp == NULL){
		printf("%s", "Error: File cannot be created or modified\n");
		return;
	}
	if(gameMode == EDIT_MODE){
		copyBoard(tempBoard, userBoard);
		for(i = 0; i < boardRowAndColSize; i++){
			for(j = 0; j < boardRowAndColSize; j++){
				if(tempBoard[i][j].currentNum != 0){
					tempBoard[i][j].fixed = 1;
				}
			}
		}
	}else if(gameMode == SOLVE_MODE){
		copyBoard(tempBoard, userBoard);
	}
	saveBoard(fp);
	printf("Saved to: %s\n", filePath);
}

/*
 * -------------------------------
 * exitGameCommand Documentation is in header file
 * -------------------------------
 */
void exitGameCommand(){
	boardData brdData = getBoardData();
	/* free boards memory */
	freeBoardMem(userBoard, brdData.blockRowSize, brdData.blockColSize);
	freeBoardMem(tempBoard, brdData.blockRowSize, brdData.blockColSize);
	freeBoardMem(solvedBoard, brdData.blockRowSize, brdData.blockColSize);

	/* free command list memory */
	deleteListNodes(commandsList);
	freeListMem(commandsList);
	printf("%s", "Exiting...\n");
	exit(0);

}


