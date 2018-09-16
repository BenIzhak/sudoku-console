/*
 * exSolver.c
 *
 *  ExSolver Module --- This module is responsible for the Exhaustive backtracking algorithm,
 *  the source file has all functions used by the exhaustive backtracking itself
 *`
 *  Created on: 30 Jul 2018
 *      Author: Raz
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Stack.h"
#include "Cell.h"
#include "Game.h"
#include "MainAux.h"

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
	int colSize = (brdData.blockRowSize * brdData.blockColSize);/* board's column size */
	for(i = 0; i < colSize; i++){
			numCompare = board[cellRow][i].currentNum;
			if(numCompare != 0 && numCompare == num && i != cellCol){/* checks if current cell's value appears here */
					return -1;
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
	int rowSize = brdData.blockRowSize * brdData.blockColSize;/* board's row size */
	for(i = 0; i < rowSize; i++){
			numCompare = board[i][cellCol].currentNum;
				if(numCompare == num && i != cellRow && numCompare != 0){
					return -1;
				}
	}
	return 0;
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
	int currentNum, i, j, currentblockRow, currentblockCol, minBlockLimitRow, minBlockLimitCol;
	boardData brdData = getBoardData();

	currentblockRow = board[cellRow][cellCol].blockRow + 1;/* incrementing by 1 for the for loop */
	currentblockCol = board[cellRow][cellCol].blockCol + 1;/* incrementing by 1 for the for loop */
	minBlockLimitRow = currentblockRow - brdData.blockRowSize;/* starting row of block */
	minBlockLimitCol = currentblockCol - brdData.blockColSize;/* starting column of block */

	for(i = minBlockLimitRow; i < currentblockRow; i++){
		for(j = minBlockLimitCol; j < currentblockCol; j++){
			if(i != cellRow || j != cellCol){/* check cell only if it's not the cell we were given */
				currentNum = board[i][j].currentNum;
				if((numToCheck == currentNum) && (currentNum != 0)){
					return -1;
				}
			}
		}
	}
	return 0;
}

/*  Function:  validAssignment
 *	--------------------
 *	checking if numToCheck is a  valid assignment to the cell,
 *	by checking row then column and then block
 *
 *  input:
 *  board-intialized 2d array
 *  numToCheck - number to assign
 *  cellRow - row of cell to be assigned to
 *  cellCol - column of cell to be assigned to
 *
 *  output:
 *  0 - valid assignment
 *  -1 - invalid assignment*/
int validAssignment(Cell** board, int numToCheck, int cellRow, int cellCol){
	int temp = 0;
	temp = rowCheck(board, numToCheck, cellRow, cellCol);/* check if numToCheck already in row  */
	if(temp < 0){
		return -1;
	}
	temp = colCheck(board, numToCheck, cellRow, cellCol);/* check if numToCheck already in column  */
	if(temp < 0){
		return -1;
	}
	temp = blockCheck(board, numToCheck, cellRow, cellCol);/* check if numToCheck already in block  */
	if(temp < 0){
		return -1;
	}
	return 0;
}

/*
 * Function:  findValidNum
 * --------------------
 *  finds the first valid value for the cell and stores it
 *  in the limit field of the cell
 *
 *	board: 2d array containing sudoku cells
 *	cellRow: cell's row
 *	cellCol: cells's column
 *
 */
int findValidNum(Cell** board, int cellRow, int cellCol){
	boardData brdData = getBoardData();
	int num = board[cellRow][cellCol].currentNum + 1, maxNum = brdData.blockRowSize * brdData.blockColSize;

	while(num <= maxNum){/* goes over possible numbers until it finds one  */
		if(validAssignment(board, num, cellRow, cellCol) == 0){/* value is 0 if num is a valid assignment*/
			return num;
		}
		num++;
	}
	return 0;
}

/*
 * Function:  findEmptyCell
 * --------------------
 *  finds the first empty cell in the board which appears after
 *  the cell which is at row of cellRow and column of cellCol
 *
 *	board: 2d array containing sudoku cells
 *	cellRow: cell's row
 *	cellCol: cells's column
 *
 */
cellIndex findEmptyCell(Cell** board, int cellRow, int cellCol){
	int i, j, flag = 0;
	boardData brdData = getBoardData();
	int rowSize = brdData.blockRowSize * brdData.blockColSize;/* calculates board's row size */
	int colSize = brdData.blockRowSize * brdData.blockColSize;/* calculates board's column size */
	cellIndex emptyIndex;

	for(i = cellRow ; i < rowSize ; i++){
		for(j = 0; j < colSize ; j++){
			if(i == cellRow && flag == 0){
			/* need to find the next empty cell so when in row of cellRow
			 * move to the column of cellCol */
				flag = 1;
				j = cellCol;
			}
			if(board[i][j].currentNum == 0){/* found an empty cell */
				emptyIndex.col = j;
				emptyIndex.row = i;
				return emptyIndex;
			}
		}
	}
	emptyIndex.col = -1;
	emptyIndex.row = -1;
	return emptyIndex;
}

/*
 * Function:  exBacktrack
 * --------------------
 * 	Exhaustive back tracking algorithm,
 * 	should use tempBoard when using this function
 *
 *	board: 2d array containing sudoku cells
 *
 *	returns: amount of different solutions
 */
void exBacktrack(Cell** board){
	cellIndex tempIndex;
	int flag = 1, cellCol = 0, cellRow = 0,  countSols = 0, foundNum = 0;
	int frstColIndex = -1; /* column of the first empty cell */
	int frstRowIndex = -1; /* row of the first empty cell */
	node* lastEmpty = NULL; /* pointer to last empty cell */
	/* the stack holds the indexes of the empty cells in the board */

	if(getErrorsFlag() == 1){/* checks if the board contains an error */
		printf("%s", "Error: board contains erroneous values\n");
		return;
	}

	tempIndex = findEmptyCell(board, cellRow, cellCol);/* find the first empty cell  */
	cellRow = tempIndex.row;
	cellCol = tempIndex.col;
	frstColIndex = cellCol;/* saving the first empty cell index */
	frstRowIndex = cellRow;/* saving the first empty cell index */
	push(&lastEmpty, cellRow, cellCol);/* pushing the first empty cell index to the stack */

	while(flag){/* as long as havn't tried to backtrack from the first empty cell*/
			if(cellRow != -1 || cellCol != -1){/* findEmptyCell function from prev iteration found an empty cell, so sudoku board isn't full. it returns -1 for both row and col if it didn't found an empty cell */
				tempIndex = peek(lastEmpty);
				if(tempIndex.col != cellCol || tempIndex.row != cellRow){/* if current empty cell is not in stack - push it */
					push(&lastEmpty, cellRow, cellCol);
				}
				foundNum = findValidNum(board, cellRow, cellCol);/* finding a valid num for the current cell */
				if(foundNum == 0){/* if there's no valid num */
					if(cellRow == frstRowIndex && cellCol == frstColIndex){/*got back to the first empty cell and there are no more options */
						flag = 0;
						pop(&lastEmpty);
					}else{
						/*got stuck need to trackback*/
						pop(&lastEmpty);
						tempIndex = peek(lastEmpty);
						board[cellRow][cellCol].currentNum = 0;
						cellCol = tempIndex.col;
						cellRow = tempIndex.row;
					}
				}else{/* there is a number I can put in cell */
					board[cellRow][cellCol].currentNum = foundNum;
					tempIndex = findEmptyCell(board, cellRow, cellCol);
					cellRow = tempIndex.row;
					cellCol = tempIndex.col;
				}
			}else{/* sudoku board is full, go back to the prev empty cell*/
				countSols++;
				tempIndex = peek(lastEmpty);
				cellCol = tempIndex.col;
				cellRow = tempIndex.row;
			}
	}
	printf("Number of solutions: %d\n", countSols);
	if(countSols == 1){printf("%s", "This is a good board!\n");}
	if(countSols > 1){printf("%s", "The puzzle has more than 1 solution, try to edit it further\n");}
}
