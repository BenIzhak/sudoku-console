/*
 * game.c
 *
 *  Game Module --- this module is responsoble for different game functions, functions which are required for game operations
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
static int markErrors;
static int errorsFlag = 0;/* flag which is 0 if there are no errors in board, 1 if there are errors */
static dll* commandsList;


/*
 * Function:  getGameMode
 * --------------------
 *  returns current gameMode code
 *
 */
int getGameMode(){
	return gameMode;
}

/*
 * Function:  setGameMode
 * --------------------
 *  sets gameMode value
 *
 *  modeNum: code of gameMode to set to
 *
 */
void setGameMode(int modeNum){
	gameMode = modeNum;
}


/*
 * Function:  setMarkErrors
 * --------------------
 * 	sets markErrors value to setting value
 *
 *	setting:0 or 1 for mark_errors
 */
void setMarkErrors(int setting){
	if(setting == 0 || setting == 1){/* checking that the setting is valid */
		markErrors = setting;
	}else{
		printf("%s","Error: the value should be 0 or 1\n");
	}
}

/*
 * Function:  getMarkErrors
 * --------------------
 * 	returns mark_errors value

 */
int getMarkErrors(){
	return markErrors;
}

/*
 * Function:  getErrorsFlag
 * --------------------
 * 	returns errorsFlag value
 */
int getErrorsFlag(){
	return errorsFlag;
}

/*
 * Function: boardDiff
 * --------------------
 * print the changed cells between two boards.
 * we check each cell in the two boards and compare them,
 * then we print the result according to the format (if needed).
 *
 * list: the commandsList
 * otheNode: the node that contains one of the boards board
 * the another one is current board.
 * command: is string Redo/Undo, we  need this for the prints format.
 */
void boardDiff(dll* list, dllNode* otherNode, char *command){
	int i, j;
	Cell **currentBoard, **otherBoard;
	boardData brdData = getBoardData();
	int boardRowAndColSize = brdData.blockColSize * brdData.blockRowSize;
	currentBoard = (list->currentNode)->info;
	otherBoard = otherNode->info;
	for(i = 0; i < boardRowAndColSize; i++){
		for(j = 0; j < boardRowAndColSize; j++){
			if(currentBoard[i][j].currentNum != otherBoard[i][j].currentNum){
				if(currentBoard[i][j].currentNum != 0 && otherBoard[i][j].currentNum != 0){
					printf("%s %d,%d: from %d to %d\n", command, (j+1), (i+1), currentBoard[i][j].currentNum, otherBoard[i][j].currentNum);
				}else{
					if(currentBoard[i][j].currentNum == 0){
						printf("%s %d,%d: from %c to %d\n", command, (j+1), (i+1), '_', otherBoard[i][j].currentNum);
					}else{
						printf("%s %d,%d: from %d to %c\n", command, (j+1), (i+1), currentBoard[i][j].currentNum, '_');
					}
				}
			}
		}
	}
}

/*
 * Function:  findAndMarkErrors
 * --------------------
 * 	goes over all cells in board and checks whether their assignment is valid.
 * 	if it isn't valid mark their cell as an error and change errorsFlag to -1 if there are no
 * 	errors change errorsFlag to 0
 *
 *	returns: -1 if an error was found, 0 if no error was found
 *
 */
void findAndMarkErrors(){
	int i, j, validAssignmentFlag;
	int flag = 0;
	boardData brdData = getBoardData();
	for(i = 0; i < (brdData.blockRowSize * brdData.blockColSize); i++ ){
		for( j = 0; j < (brdData.blockRowSize * brdData.blockColSize); j++){
			if(userBoard[i][j].fixed == 0){/* the cell needs to be non-fixed cell */
				validAssignmentFlag = validAssignment(userBoard, userBoard[i][j].currentNum, i , j);
				if(validAssignmentFlag == -1){
					userBoard[i][j].isError = 1;/* marking cell as an error */
					flag = 1;
					errorsFlag = flag;/* marking the board as an errorneus */
				}else{
					userBoard[i][j].isError = 0;
				}
			}
		}
	}
	if(flag == 0){
		errorsFlag = 0;
	}
}

/*
 * 	Function:  newSetCommand
 * --------------------
 *	this function adds a new set or an autofill commands to the commands list.
 *	if we are not point to the last node of the commands list we delete the nodes
 *	from the node we point to and on. the new command will be the last command.
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
 * Function:  validateCommand
 * --------------------
 *	checks if board is valid or not, first by checking for simple errors(same number more than once in a row/column/block
 *	then using ILP to check if board is solvable
 *
 *	isDone: used as flag, 0 - validating but the board is not completed, 1 - validating and all cells are filled
 *
 */
void validateCommand(int isDone){
	int isSolved = 0;
	if(errorsFlag == 1){/* checking the errors flag for cells with an error */
		if(isDone == 0){
			printf("%s", "Error: board contains erroneous values\n");
		}else{
			printf("%s", "Puzzle solution erroneous\n");
		}
	}else{
		isSolved = ILPSolver();/* solving the board */
		if(isSolved == 1){
			if(isDone == 0){/* there's a solution to the board */
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
	/* first checks that board has no empty cells */
	for(i = 0; i < (brdData.blockRowSize * brdData.blockColSize); i++){
		for(j = 0; j < (brdData.blockRowSize * brdData.blockColSize); j++){
			if(board[i][j].currentNum == 0){
				flag = 1;
			}
		}
	}

	if(flag == 0 && gameMode == SOLVE_MODE){/* if board is not empty */
		validateCommand(1); /* validate the board, using 1 input show that board is complete*/
	}
}

/*
 * Function:  setCell
 * --------------------
 * 	sets cell in column of X and row of Y to value of Z
 * 	if in solve mode set isInput to 1, if in edit mode set fixed to 1
 *
 *	returns: -1 if in solve mode and cell is fixed, if set is successful returns 0
 *
 *	X: column of cell
 *  Y: row of cell
 *	Z: new value to set cell to
 */
void setCell(int col, int row, int val){
	if(userBoard[row][col].fixed == 1){/* no need to check which game mode because fixed cells are only available while in solve mode */
		printf("%s", "Error: cell is fixed\n");
		return;
	}

	if(val == 0){/* value to change cell to is 0 */
		userBoard[row][col].isInput = 0; /* mark as not an input */
		userBoard[row][col].currentNum = val;
	}else{
		userBoard[row][col].isInput = 1;
		userBoard[row][col].currentNum = val;
	}

	findAndMarkErrors();/* checking for errors in board */

	newSetCommand();/* adding to the commands list */
	printBoard(userBoard);
	isFinished(userBoard);/* checking if user is finished */
}

/*
 * Function:  resetCommand
 * --------------------
 *  we point to the first node which is the original board we loaded,
 *  then we delete all of the nodes from the first one.
 *  finally we update the userBoard.
 */
void resetCommand(){
	(commandsList->currentNode) = (commandsList->head);
	deleteFromCurrent(commandsList);
	copyBoard(userBoard, (commandsList->currentNode)->info);
	printf("Board reset\n");
}

/*
 * Function:  hardReset
 * --------------------
 * we check if the command list is empty, if it's not empty we delete all of it's
 * nodes and initialize it. if the command list is not empty we just initialize it.
 */
void hardReset(Cell** info){
	if((commandsList->head) != NULL){
		deleteListNodes(commandsList);
	}
	initList(commandsList, info, errorsFlag);
}

/*
 * Function:  startNewCommandsList
 * --------------------
 * if commandsList does not exist -> create new one
 * if commandsList exists -> hardReset it
 */
void startNewCommandsList(){
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
 * Function:  undoCommand
 * --------------------
 * check if we point to the head, if we do so we don't have commands to undo.
 * else we print the previous board, use boardDiff to prints what changed,
 * update the commandsList and errorFlag.
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
 * Function:  redoCommand
 * --------------------
 * check if we point to the last node, if we do so we don't have commands to redo.
 * else we print the next board, use boardDiff to prints what changed,
 * update the commandsList and errorFlag.
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
 * Function:  validNumsAlloc
 * --------------------
 *
 *  allocating memory for validnums array field
 *
 *	board: 2d array containing sudoku cells
 *	cellRow: cell's row
 *	cellCol: cells's column
 *
 */
void validNumsAlloc(Cell** board, int cellRow, int cellCol){
	boardData brdData = getBoardData();
	int boardRowAndColSize = brdData.blockRowSize * brdData.blockColSize;

	board[cellRow][cellCol].validNums = (int *) malloc((boardRowAndColSize) * sizeof(int));
}

/*
 * Function:  availableNumbers
 * --------------------
 * 	checks which numbers are available to the cell
 * 	at column of cellCol and row of cellRow and returns
 * 	a random legal value
 *
 * 	validNums memory need to be allocated prior to the call of this function
 *
 *	board: 2d array containing sudoku cells
 *	cellRow: cell's row
 *	cellCol: cells's column
 *
 */
int availableNumbers(Cell** board, int cellRow, int cellCol){
	int counter = 0;/* counts the amount of valid numbers*/
	int num, randIndex, randNum = 0;
	boardData brdData = getBoardData();
	int maxNum = brdData.blockRowSize * brdData.blockColSize;


	for(num = 1; num <= maxNum; num++){
		if(validAssignment(board, num, cellRow, cellCol) == 0){/* value is 0 if num is a valid assignment*/
			board[cellRow][cellCol].validNums[counter] = num;/* putting the valid num in the validNums array */
			counter++;
		}
	}

	board[cellRow][cellCol].limit = counter;/* changing the limit field to show how many numbers are valid */

	if(counter != 0){
		randIndex = rand() % counter;
		randNum = board[cellRow][cellCol].validNums[randIndex];/* get a random value from the array */
	}else{
		randNum = 0;
	}
	return randNum;
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
	int i, j, randCol, randRow, randNum, flag = 0, isSolved = 0;

	for(i = 0; i < cellsToFill; i++){/* getting the right amount of cell to fill */
		while(flag == 0){/* while a cell to fill hasn't been found*/
			randCol = rand() % rowAndColSize;
			randRow = rand() % rowAndColSize;
			if(userBoard[randRow][randCol].currentNum == 0){/* checking that i havn't chose this cell already if it was chosen before it's currentNum wont be 0 */
				flag = 1;
			}
		}
		flag = 0;
		validNumsAlloc(userBoard, randRow, randCol);/* allocating memory for the available num function */
		randNum = availableNumbers(userBoard, randRow, randCol);
		free(userBoard[randRow][randCol].validNums);
		if(randNum == 0){/* if availableNumbers didn't find a valid number for the cell */
			return 0;
		}
		userBoard[randRow][randCol].currentNum = randNum;
	}
	isSolved = ILPSolver();
	if(isSolved != 1){
		return 0;/* error in ILPSolver, need to try again*/
	}
	flag = 0;
	for(i = 0; i < cellsToKeep; i++){/* now need to keep only cellsToKeep cells*/
		while(flag == 0){
			randCol = rand() % rowAndColSize;
			randRow = rand() % rowAndColSize;

			/* checking that i havn't chose this cell already */
			if(solvedBoard[randRow][randCol].fixed == 0){/*generate is only available in edit mode, so fixed field is not used. i'm using it for marking cells to keep */
				flag = 1;
				solvedBoard[randRow][randCol].fixed = 1;
			}
		}
		flag = 0;
	}
	for(i = 0; i < (brdData.blockRowSize * brdData.blockColSize); i++ ){/* going over cells with fixed = 0 and removing them from the board */
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
 * Function:  generateCommand
 * --------------------
 *	Generates a puzzle by using fillAndKeep function
 *
 *
 *	cellsToFill: amount of cells to fill with random values
 *	cellsToKeep: amount of cells to keep after generating random values
 */

void generateCommand(int cellsToFill, int cellsToKeep){
	boardData brdData = getBoardData();
	int rowAndColSize = brdData.blockRowSize * brdData.blockColSize;
	int emptyCells = (rowAndColSize)*(rowAndColSize);/* if the board is empty, amount of empty cells is N*N */
	int i, anyChanges = 0;/* anychanges is a flag for a change in the board */

	if((cellsToFill > emptyCells) || (cellsToKeep > emptyCells)){/* checks that parameters are valid */
		printf("Error: value not in range 0-%d\n", emptyCells);
		return;
	}

	if(isEmptyBoard() == 0){/* checks that board is empty */
		printf("%s", "Error: board is not empty\n");
		return;
	}

	/* tries for 1000 iterations to get a valid board*/
	for(i = 0; i < 1000; i++){
		if(fillAndKeep(cellsToFill, cellsToKeep) == 1){/* was successful */
			copyBoard(userBoard, solvedBoard);
			anyChanges = 1;
			if(anyChanges == 1 && cellsToKeep > 0){
				/* the board was changed */
				newSetCommand();
			}
			printBoard(userBoard);
			return;/* Done successfully */
		}else{
			boardInit(userBoard);
			boardInit(solvedBoard);
		}
	}
	printf("%s", "Error: puzzle generator failed\n");
}

/*
 * Function:  startDefaultBoard
 * --------------------
 * first we free memory of previous boards (if there is not previous boards we do nothing.
 * then we set new values to blockRowSize and blockColSize and allocate memory for news boards.
 * we initialize the boards, change the error flag to 0 (because now the board is empty),
 * start new command list, and finally print the board.
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
	userBoard = boardAllocatedMem(boardRowAndColSize);
	tempBoard = boardAllocatedMem(boardRowAndColSize);
	solvedBoard = boardAllocatedMem(boardRowAndColSize);

	/* init the boards */
	boardInit(userBoard);
	boardInit(tempBoard);
	boardInit(solvedBoard);

	/* change errorFlag back to 0 */
	errorsFlag = 0;

	startNewCommandsList();

	printBoard(userBoard);
}

/*
 * Function:  autoFillCommand
 * --------------------
 *  automatically fills obvious values in the board
 */
void autoFillCommand(){
	int i,j, anyChanges = 0;
	boardData brdData = getBoardData();

	if(errorsFlag){
		printf("%s", "Error: board contains erroneous values\n");
		return;
	}

	boardInit(tempBoard);
	copyBoard(tempBoard, userBoard);/* copy the user board to tempboard, so i can make changes without changing the userboard */

	for(i = 0; i < (brdData.blockRowSize * brdData.blockColSize); i++ ){
		for( j = 0; j < (brdData.blockRowSize * brdData.blockColSize); j++){
			if(tempBoard[i][j].currentNum == 0){
				validNumsAlloc(userBoard, i, j);
				availableNumbers(userBoard, i, j);/* checking available numbers in the original board */
				if(userBoard[i][j].limit == 1){/* there's only one number available */
					anyChanges = 1;

					tempBoard[i][j].currentNum = userBoard[i][j].validNums[0];;
					printf("Cell <%d,%d> set to %d\n", (j + 1), (i + 1), userBoard[i][j].validNums[0]);
				}
				free(userBoard[i][j].validNums);
			}
		}
	}

	copyBoard(userBoard, tempBoard);
	findAndMarkErrors();/* checking again for errors */

	if(anyChanges){
		newSetCommand();
	}
	printBoard(userBoard);
	isFinished(userBoard);
}

/*
 * Function:  setHint
 * --------------------
 *  gives a hint for a cell at column of col and row of row
 *
 *  col: column of cell to show hint for
 *  row: rowof cell to show hint for
 */
void setHint(int col, int row){
	/* col and row starting with zero
	 * boardIsSolvable = 2 <-> board is unsolvable
	 * boardISSolvable = 1 <-> board is solvable */
	int boardIsSolvable;

	if(getErrorsFlag()){/* checking for errors */
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
	boardIsSolvable = ILPSolver();/* solving the board */
	if(boardIsSolvable == 2){
		printf("%s", "Error: board is unsolvable\n");
	}else{/* board is solvable */
		printf("Hint: set cell to %d\n", solvedBoard[row][col].currentNum); /* print the hint */
	}


}

/*
 * -------------------------------
 * solveCommand Documentation is in header file
 * -------------------------------
 */
void solveCommand(char* filePath){
	if(loadBoard(filePath, SOLVE_MODE) == -1){
		printf("%s", "Error: File doesn't exist or cannot be opened\n");
		return;
	}
	gameMode = SOLVE_MODE;
	findAndMarkErrors();
	/* print the board we just loaded */
	printBoard(userBoard);
}

/*
 * Function:  editCommand
 * --------------------
 *	we check if file path provided.
 *	if we have file path we load the file and begin the game in edit mode,
 *	else we start default 9*9 board.
 */
void editCommand(char* filePath , int numOfArgs){
	if(numOfArgs > 0){
		if(loadBoard(filePath, EDIT_MODE) == -1){
			printf("%s", "Error: File cannot be opened\n");
			return;
		}
		gameMode = EDIT_MODE;
		findAndMarkErrors();
		/* print the board we just loaded */
		printBoard(userBoard);
	}else{
		gameMode = EDIT_MODE;
		startDefaultBoard();
	}

}

/*
 * Function:  saveCommand
 * --------------------
 *	saving the current board to a txt file.
 *	the save format is depends in the game mode.
 *	before saving we check if the board is solvable or contain errors if needed.
 *
 *	filePath: path of file of board to be edited
 *
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
 * Function:  exitGameCommand
 * --------------------
 * exits the program and frees all resources of boards and commands list.
 *
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


