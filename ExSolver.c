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


/*
 * Function:  resetCell
 * --------------------
 *  resets cell num to 0 and put 0 in array of the previous numbers it tried during backtracking
 *
 *  table: 2d array containing sudoku cells
 *	cellRow: row of cell to reset
 *	cellCol: column of cell to reset
 */
void resetCell(Cell** board, int cellRow, int cellCol){
	int i;
	board[cellRow][cellCol].currentNum = 0;

	/* initializes prevNums as not used nums*/
	for (i = 0; i < 9; i++) {/*TODO: change to dynamic size of prevNums*/
		board[cellRow][cellCol].prevNums[i] = 0;
	}

}
/*
 * Function:  resetCells
 * --------------------
 *  resets cells between [cellRow][CellCol] to tempIndex, num to 0 and put 0 in array of the previous numbers it tried during backtracking
 *
 *	table: 2d array containing sudoku cells
 *  tempIndex: first index which doesn't need to be reset
 *	cellRow: row of first cell to reset
 *	cellCol: column of first cell to reset
 */
void resetCells(Cell** board, cellIndex tempIndex, int cellCol, int cellRow){
	int dstRow = tempIndex.row;
	int dstCol = tempIndex.col;
	int boardSize = 9;/* TODO: need to change to dynamic value */

	while(dstRow != cellRow || dstCol != cellCol){
		resetCell(board, cellRow, cellCol);
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
 *  table: 2d array containing sudoku cells
 *  numIndex: index of valid number from the cell valid numbers array
 *	cellRow: row of cell to reset
 *	cellCol: column of cell to reset
 */
void updateCell(Cell** table, int numIndex, int cellRow, int cellCol){

	int num;
	num = table[cellRow][cellCol].validNums[numIndex];
	table[cellRow][cellCol].currentNum = num;
	table[cellRow][cellCol].prevNums[num - 1] = 1;
}

/*
 * Function:  rowCheck
 * --------------------
 *  checks if a given number is in a specific row
 *
 *  table: 2d array containing sudoku cells
 *  num: number to check
 *	cellRow: row to check
 *
 *	returns: -1 if number already appears in row, else it returns 0
 */
int rowCheck(Cell** table, int num, int cellRow){
	int i, numCompare;
	int rowSize = 9;/*TODO:change back to dynamic value or to old value:BLOCK_ROW_SIZE * BLOCK_COL_SIZE;*/
	for(i = 0; i < rowSize; i++){
		numCompare = table[cellRow][i].currentNum;
		if(numCompare != 0){
			if(numCompare == num){
				return -1;
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
 *  table: 2d array containing sudoku cells
 *  num: number to check
 *	cellCol: column to check
 *
 *	returns: -1 if number already appears in row, else it returns 0
 */
int colCheck(Cell** table, int num,  int cellCol){
	int i, numCompare;
	int colSize = 9;/*TODO:change back to dynamic value or to old value:BLOCK_ROW_SIZE * BLOCK_COL_SIZE;*/
	for(i = 0; i < colSize; i++){
		numCompare = table[i][cellCol].currentNum;
		if(numCompare != 0){
			if(numCompare == num){
				return -1;
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
	int boardSize = 9;/*TODO:change back to dynamic value or to old value:BLOCK_ROW_SIZE * BLOCK_COL_SIZE;*/
	int i = 3 - 1;/*TODO:change back to dynamic value or to old value:BLOCK_COL_SIZE - 1;*/
	float calcPos = 0;/* calculates relation between block end and cell position */
	while(i <= boardSize){
		calcPos = cellCol / (float) i;
		if(calcPos <= 1.0){
			return i;
		}
		i += 3;/*TODO:change back to dynamic value or to old value:BLOCK_COL_SIZE;*/
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
	int boardSize = 9;/*TODO:change back to dynamic value or to old value:BLOCK_ROW_SIZE * BLOCK_COL_SIZE;*/
	int i = 3 - 1;/*TODO:change back to dynamic value or to old value:BLOCK_COL_SIZE - 1;*/
	float calcPos = 0;/* calculates relation between block end and cell position */
	while(i <= boardSize){
		calcPos = cellRow / (float) i;
		if(calcPos <= 1.0){
			return i;
		}
		i += 3;/*TODO:change back to dynamic value or to old BLOCK_ROW_SIZE;*/
	}
	return -1;
}

/*
 * Function:  blockCheck
 * --------------------
 *  checks if numToCheck is in the block of the cell
 *  at column of cellCol and row of cellRow in table
 *
 *	table: 2d array containing sudoku cells
 *	numToCheck: number we want to check
 *	cellRow: cell's row
 *	cellCol: cells's column
 *
 *	returns: 0 numToCheck is not in the block,
 *			-1 numToCheck is in the block
 */
int blockCheck(Cell** table,int numToCheck , int cellRow, int cellCol){
	int currentNum;
	int i,j;
	int currentblockRow, currentblockCol;
	int minBlockLimitRow, minBlockLimitCol;
	currentblockRow = getcurrentblockRow(cellRow) + 1;
	currentblockCol = getcurrentblockCol(cellCol) + 1;
	minBlockLimitRow = currentblockRow - 3;/*TODO:change back to dynamic value or to old currentblockRow - BLOCK_ROW_SIZE;*/
	minBlockLimitCol = currentblockCol - 3;/*TODO:change back to dynamic value or to old currentblockRow - BLOCK_COL_SIZE;*/
	for(i = minBlockLimitRow; i < currentblockRow; i++){
		for(j = minBlockLimitCol; j < currentblockCol; j++){
			currentNum = table[i][j].currentNum;
			if(numToCheck == currentNum){
				return -1;
			}
		}
	}
	return 0;
}

int validAssignment(Cell** table, int numToCheck, int cellRow, int cellCol){
	int temp = 0;

	temp = rowCheck(table, numToCheck, cellRow);
	if(temp < 0){
		return -1;
	}
	temp = colCheck(table, numToCheck, cellCol);
	if(temp < 0){
		return -1;
	}
	temp = blockCheck(table, numToCheck, cellRow, cellCol);
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
 *	table: 2d array containing sudoku cells
 *	cellRow: cell's row
 *	cellCol: cells's column
 *
 */
void availableNumbers(Cell** table, int cellRow, int cellCol){
	int prevNumFlag;
	int counter = 0;/* counts the amount of valid numbers*/
	int num;
	for(num = 1; num < 10; num++){
		/* TODO: problem here! prevnumflag is getting an int from somewhere else, it makes the
		 * valid nums empty */
		prevNumFlag = table[cellRow][cellCol].prevNums[num - 1];
		if(prevNumFlag == 0){/* checks if num was previously used */
			if(validAssignment(table, num, cellRow, cellCol) == 0){/* value is 0 if num is a valid assignment*/
				table[cellRow][cellCol].validNums[counter] = num;
				counter++;
			}
		}
	}
	table[cellRow][cellCol].limit = counter;
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
/*TODO: Add different error messages */
int exBacktrack(Cell** board){
	int limit, flag = 1, cellCol = 0, cellRow = 0, boardSize = 9, countSols = 0;
	int frstColIndex = -1; /* column of the first empty cell */
	int frstRowIndex = -1; /* row of the first empty cell */
	cellIndex tempIndex;
	node* lastEmpty = NULL; /* pointer to last empty cell */

	while(flag){
		if(board[cellRow][cellCol].fixed == 1 || board[cellRow][cellCol].isInput == 1){ /* checks if cell is fixed or an input */
			if(cellCol < boardSize - 1){
				cellCol++;
			}
			else if(cellRow < boardSize - 1){
				cellRow++;
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
				}/*TODO: decide where to add new empty cell */
			}else{
				updateCell(board, 0, cellRow, cellCol);
				if(cellCol < boardSize - 1){
					cellCol++;
				}else if(cellRow < boardSize - 1){
					cellRow++;
				}else{
					countSols++;
				}
			}
		}
	}
	return countSols;
}
