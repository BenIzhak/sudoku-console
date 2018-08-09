/*
 * exSolver.c
 *
 *  Created on: 30 Jul 2018
 *      Author: Raz
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Stack.h"
#include "Cell.h"
#include "MainAux.h"
#include "game.h"

void initBoardSolver(Cell** board){
	boardData brdData = getBoardData();
	int i, j, k, boardRowAndColSize = brdData.blockRowSize * brdData.blockColSize;

	for (i = 0; i < boardRowAndColSize; i++){
		for (j = 0; j < boardRowAndColSize; j++){
			board[i][j].prevNums = (int *) malloc((boardRowAndColSize) * sizeof(int));
			board[i][j].validNums = (int *) malloc((boardRowAndColSize) * sizeof(int));
			if(board[i][j].prevNums == NULL){
				printf("%s","Error: initBoardSolver has failed\n");
			}
			if(board[i][j].validNums == NULL){
				printf("%s","Error: initBoardSolver has failed\n");
			}
			for(k = 0; k < boardRowAndColSize; k++){/* initializes prevNums as not used nums*/
				board[i][j].prevNums[k] = 0;
			}
		}
	}
}

void exitSolver(Cell** board){
	boardData brdData = getBoardData();
	int i, j, boardRowAndColSize = brdData.blockRowSize * brdData.blockColSize;

	for(i = 0; i < boardRowAndColSize; i++){
		for(j = 0; j < boardRowAndColSize; j++){
			free(board[i][j].prevNums);
			free(board[i][j].validNums);
		}
	}
}
/*
 * Function:  resetCell
 * --------------------
 *  resets cell num to 0 and put 0 in array of the previous numbers it tried during backtracking
 *
 *  board: 2d array containing sudoku cells
 *	cellRow: row of cell to reset
 *	cellCol: column of cell to reset
 */
void resetCell(Cell** board, int cellRow, int cellCol){
	boardData brdData = getBoardData();
	int i, amountOfNums = brdData.blockRowSize * brdData.blockColSize;

	board[cellRow][cellCol].currentNum = 0;

	/* initializes prevNums as not used nums*/
	for (i = 0; i < amountOfNums; i++) {
		board[cellRow][cellCol].prevNums[i] = 0;
	}

}
/*
 * Function:  resetCells
 * --------------------
 *  resets cells between [cellRow][CellCol] to tempIndex, num to 0 and put 0 in array of the previous numbers it tried during backtracking
 *
 *	board: 2d array containing sudoku cells
 *  tempIndex: first index which doesn't need to be reset
 *	cellRow: row of first cell to reset
 *	cellCol: column of first cell to reset
 */
void resetCells(Cell** board, cellIndex tempIndex, int cellCol, int cellRow){
	int dstRow = tempIndex.row;
	int dstCol = tempIndex.col;

	boardData brdData = getBoardData();
	int boardSize = brdData.blockRowSize * brdData.blockColSize;
	if(dstRow == -1 && dstCol == -1){
		dstRow = 0;
		dstCol = 0;
	}
	while(dstRow != cellRow || dstCol != cellCol){

		if(board[cellRow][cellCol].fixed == 0 && board[cellRow][cellCol].isInput == 0){
			resetCell(board, cellRow, cellCol);
		}

		if(cellCol == 0){
			cellCol = boardSize - 1;
			cellRow--;
		}else{
			cellCol--;
		}
	}
}
/*
 * Function:  updateCell
 * --------------------
 *  updates cell with a new number with one of it's valid numbers
 *
 *  board: 2d array containing sudoku cells
 *  numIndex: index of valid number from the cell valid numbers array
 *	cellRow: row of cell to reset
 *	cellCol: column of cell to reset
 */
void updateCell(Cell** board, int numIndex, int cellRow, int cellCol){

	int num;
	num = board[cellRow][cellCol].validNums[numIndex];
	board[cellRow][cellCol].currentNum = num;
	board[cellRow][cellCol].prevNums[num - 1] = 1;
}

/*
 * Function:  rowCheck
 * --------------------
 *  checks if a given number is in a specific row
 *
 *  board: 2d array containing sudoku cells
 *  num: number to check
 *	cellRow: row to check
 *
 *	returns: -1 if number already appears in row, else it returns 0
 */
int rowCheck(Cell** board, int num, int cellRow, int cellCol){
	int i, numCompare;
	boardData brdData = getBoardData();
	int colSize = (brdData.blockRowSize * brdData.blockColSize);
	for(i = 0; i < colSize; i++){
		if(i != cellCol){
			numCompare = board[cellRow][i].currentNum;
			if(numCompare != 0){
				if(numCompare == num){
					return -1;
				}
			}
		}
	}
	return 0;
}

/*
 * Function:  colCheck
 * --------------------
 *  checks if a given number is in a specific col
 *
 *  board: 2d array containing sudoku cells
 *  num: number to check
 *	cellCol: column to check
 *
 *	returns: -1 if number already appears in row, else it returns 0
 */
int colCheck(Cell** board, int num, int cellRow, int cellCol){
	int i, numCompare;
	boardData brdData = getBoardData();
	int rowSize = brdData.blockRowSize * brdData.blockColSize;
	for(i = 0; i < rowSize; i++){
		if( i != cellRow){
			numCompare = board[i][cellCol].currentNum;
			if(numCompare != 0){
				if(numCompare == num){
					return -1;
				}
			}
		}
	}
	return 0;
}


/*
 * Function:  getcurrentblockCol
 * --------------------
 *  calculates the ending column of the block of the column cellCol
 *
 *	cellCol: column to check
 *
 *	returns: number of ending column of the block of the column cellCol
 */
int getcurrentblockCol(int cellCol){
	boardData brdData = getBoardData();
	int boardSize = brdData.blockRowSize * brdData.blockColSize;
	int i = brdData.blockColSize - 1;
	float calcPos = 0;/* calculates relation between block end and cell position */
	while(i <= boardSize){
		calcPos = cellCol / (float) i;
		if(calcPos <= 1.0){
			return i;
		}
		i += brdData.blockColSize;
	}
	return -1;
}


/*
 * Function:  getcurrentblockRow
 * --------------------
 *  calculates the ending row of the block of the column cellRow
 *
 *	cellRow: row to check
 *
 *	returns: number of ending row of the block of the column cellRow
 */
int getcurrentblockRow(int cellRow){
	boardData brdData = getBoardData();
	int boardSize = brdData.blockRowSize * brdData.blockColSize;
	int i = brdData.blockRowSize - 1;
	float calcPos = 0;/* calculates relation between block end and cell position */
	while(i <= boardSize){
		calcPos = cellRow / (float) i;
		if(calcPos <= 1.0){
			return i;
		}
		i += brdData.blockRowSize;
	}
	return -1;
}

/*
 * Function:  blockCheck
 * --------------------
 *  checks if numToCheck is in the block of the cell
 *  at column of cellCol and row of cellRow in board
 *
 *	board: 2d array containing sudoku cells
 *	numToCheck: number we want to check
 *	cellRow: cell's row
 *	cellCol: cells's column
 *
 *	returns: 0 numToCheck is not in the block,
 *			-1 numToCheck is in the block
 */
int blockCheck(Cell** board,int numToCheck , int cellRow, int cellCol){
	int currentNum;
	int i,j;
	int currentblockRow, currentblockCol;
	int minBlockLimitRow, minBlockLimitCol;
	boardData brdData = getBoardData();

	currentblockRow = getcurrentblockRow(cellRow) + 1;
	currentblockCol = getcurrentblockCol(cellCol) + 1;
	minBlockLimitRow = currentblockRow - brdData.blockRowSize;
	minBlockLimitCol = currentblockCol - brdData.blockColSize;
	for(i = minBlockLimitRow; i < currentblockRow; i++){
		for(j = minBlockLimitCol; j < currentblockCol; j++){
			if(i != cellRow || j != cellCol){
				currentNum = board[i][j].currentNum;
				if((numToCheck == currentNum) && (currentNum != 0)){
					return -1;
				}
			}
		}
	}
	return 0;
}

int validAssignment(Cell** board, int numToCheck, int cellRow, int cellCol){
	int temp = 0;

	temp = rowCheck(board, numToCheck, cellRow, cellCol);
	if(temp < 0){
		return -1;
	}
	temp = colCheck(board, numToCheck, cellRow, cellCol);
	if(temp < 0){
		return -1;
	}
	temp = blockCheck(board, numToCheck, cellRow, cellCol);
	if(temp < 0){
		return -1;
	}
	return 0;
}

/*
 * Function:  availableNumbers
 * --------------------
 * 	checks which numbers are available to the cell
 * 	at column of cellCol and row of cellRow and updates
 * 	the validNums array with them and the limit field accordingly
 *
 *	board: 2d array containing sudoku cells
 *	cellRow: cell's row
 *	cellCol: cells's column
 *
 */
void availableNumbers(Cell** board, int cellRow, int cellCol){
	int prevNumFlag;
	int counter = 0;/* counts the amount of valid numbers*/
	int num;
	boardData brdData = getBoardData();
	int maxNum = brdData.blockRowSize * brdData.blockColSize;
	for(num = 1; num <= maxNum; num++){
		prevNumFlag = board[cellRow][cellCol].prevNums[num - 1];
		if(prevNumFlag == 0){/* checks if num was previously used */
			if(validAssignment(board, num, cellRow, cellCol) == 0){/* value is 0 if num is a valid assignment*/
				board[cellRow][cellCol].validNums[counter] = num;
				counter++;
			}
		}
	}
	board[cellRow][cellCol].limit = counter;
}

/*
 * Function:  exBacktrack
 * --------------------
 * 	Exhaustive back tracking algorithm
 *
 *	board: 2d array containing sudoku cells
 *
 *	returns: amount of different solutions
 *
 */
void exBacktrack(Cell** board){
	boardData brdData = getBoardData();
	int limit, flag = 1, cellCol = 0, cellRow = 0, boardSize = (brdData.blockRowSize * brdData.blockColSize), countSols = 0;
	int frstColIndex = -1; /* column of the first empty cell */
	int frstRowIndex = -1; /* row of the first empty cell */
	cellIndex tempIndex;
	node* lastEmpty = NULL; /* pointer to last empty cell */

	if(getErrorsFlag() == 1){
		printf("%s", "Error: board contains erroneous values\n");
		return;
	}
	initBoardSolver(board);

	push(&lastEmpty, -1, -1);

	while(flag){
		if(board[cellRow][cellCol].fixed == 1 || board[cellRow][cellCol].isInput == 1){ /* checks if cell is fixed or an input */
			if(cellCol < boardSize - 1){
				cellCol++;
			}
			else if(cellRow < boardSize - 1){
				cellRow++;
				cellCol = 0;
			}else{
				/* increase counter, got to last cell and it's fixed or input
				 * need to get back to last empty cell */
				countSols++;

				tempIndex = peek(lastEmpty);

				resetCells(board, tempIndex, cellCol, cellRow);

				cellCol = tempIndex.col;
				cellRow = tempIndex.row;
			}
		}else{
			if(frstColIndex == -1 && frstRowIndex == -1){ /* save first empty cell index */
				frstColIndex = cellCol;
				frstRowIndex = cellRow;
			}
			tempIndex = peek(lastEmpty);
			if(tempIndex.col != cellCol || tempIndex.row != cellRow){
				push(&lastEmpty, cellRow, cellCol);
			}

			availableNumbers(board, cellRow, cellCol);
			limit = board[cellRow][cellCol].limit;

			if(limit == 0){
				if(cellRow == frstRowIndex && cellCol == frstColIndex){
					flag = 0;
					pop(&lastEmpty);
					/*got back to the first empty and there are no more options */
				}else{
					/*got stuck need to trackback*/
					pop(&lastEmpty);
					tempIndex = peek(lastEmpty);

					resetCells(board, tempIndex, cellCol, cellRow);

					cellCol = tempIndex.col;
					cellRow = tempIndex.row;
				}
			}else{
				/* there is a number I can put in cell */
				updateCell(board, 0, cellRow, cellCol);
				if(cellCol < boardSize - 1){
					cellCol++;
				}else if(cellRow < boardSize - 1){
					cellRow++;
					cellCol = 0;
				}else{
					/* got to last cell incremenent amount of solutions, no other cell to move to */
					countSols++;
				}
			}
		}
	}
	exitSolver(board);
	printf("Number of solutions: %d\n", countSols);
	if(countSols == 1){
		printf("%s", "This is a good board!\n");
	}
	if(countSols > 1){
		printf("%s", "The puzzle has more than 1 solution, try to edit it further\n");
	}
}
