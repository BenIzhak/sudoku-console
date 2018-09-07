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

/*void initBoardSolver(Cell** board){
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
			for(k = 0; k < boardRowAndColSize; k++){ initializes prevNums as not used nums
				board[i][j].prevNums[k] = 0;
			}
		}
	}
}*/

/*void exitSolver(Cell** board){
	boardData brdData = getBoardData();
	int i, j, boardRowAndColSize = brdData.blockRowSize * brdData.blockColSize;

	for(i = 0; i < boardRowAndColSize; i++){
		for(j = 0; j < boardRowAndColSize; j++){
			free(board[i][j].prevNums);
			free(board[i][j].validNums);
		}
	}
}*/
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
/*void resetCells(Cell** board, cellIndex tempIndex, int cellCol, int cellRow){
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
			board[cellRow][cellCol].currentNum = 0;
		}

		if(cellCol == 0){
			cellCol = boardSize - 1;
			cellRow--;
		}else{
			cellCol--;
		}
	}
}*/
void resetAllCells(Cell** board){

	boardData brdData = getBoardData();
	int boardSize = brdData.blockRowSize * brdData.blockColSize;
	int cellRow = brdData.blockRowSize;
	int cellCol = brdData.blockColSize;
	while(0 != cellRow || 0 != cellCol){

		if(board[cellRow][cellCol].fixed == 0 && board[cellRow][cellCol].isInput == 0){
			board[cellRow][cellCol].currentNum = 0;
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
			numCompare = board[cellRow][i].currentNum;
			if(numCompare != 0 && numCompare == num && i != cellCol){
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
	int rowSize = brdData.blockRowSize * brdData.blockColSize;
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

	currentblockRow = board[cellRow][cellCol].blockRow + 1;
	currentblockCol = board[cellRow][cellCol].blockCol + 1;
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
void findValidNum(Cell** board, int cellRow, int cellCol){
	boardData brdData = getBoardData();
	int flag = 0;
	int num = board[cellRow][cellCol].currentNum + 1, maxNum = brdData.blockRowSize * brdData.blockColSize;

	board[cellRow][cellCol].limit = 0;

	while(flag == 0 && num <= maxNum){

		if(validAssignment(board, num, cellRow, cellCol) == 0){/* value is 0 if num is a valid assignment*/
			board[cellRow][cellCol].limit = num;
			flag = 1;
		}
		num++;
	}
}

void exBacktrack(Cell** board){
	boardData brdData = getBoardData(); cellIndex tempIndex;
	int flag = 1, cellCol = 0, cellRow = 0, boardSize = (brdData.blockRowSize * brdData.blockColSize), countSols = 0;
	int frstColIndex = -1; /* column of the first empty cell */
	int frstRowIndex = -1; /* row of the first empty cell */
	node* lastEmpty = NULL; /* pointer to last empty cell */

	if(getErrorsFlag() == 1){
		printf("%s", "Error: board contains erroneous values\n");
		return;
	}
	push(&lastEmpty, -1, -1);

	while(flag){
		if(board[cellRow][cellCol].fixed == 1 || board[cellRow][cellCol].isInput == 1){ /* checks if cell is fixed or an input */
			if(cellCol < boardSize - 1){
				cellCol++;
			}else if(cellRow < boardSize - 1){
				cellRow++;
				cellCol = 0;
			}else{/* increase counter, got to last cell and it's fixed or input need to get back to last empty cell */
				countSols++;
				tempIndex = peek(lastEmpty);
				cellCol = tempIndex.col;
				cellRow = tempIndex.row;
			}
		}else{/* cell is not fixed or input */
			if(frstColIndex == -1 && frstRowIndex == -1){ /* save first empty cell index */
				frstColIndex = cellCol;
				frstRowIndex = cellRow;
			}
			tempIndex = peek(lastEmpty);
			if(tempIndex.col != cellCol || tempIndex.row != cellRow){
				push(&lastEmpty, cellRow, cellCol);
			}
			findValidNum(board, cellRow, cellCol);
			if(board[cellRow][cellCol].limit == 0){
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
				board[cellRow][cellCol].currentNum = board[cellRow][cellCol].limit;
				if(cellCol < boardSize - 1){
					cellCol++;
				}else if(cellRow < boardSize - 1){
					cellRow++;
					cellCol = 0;
				}else{/* got to last cell increment amount of solutions, no other cell to move to */
					countSols++;
				}
			}
		}
	}
	printf("Number of solutions: %d\n", countSols);
	if(countSols == 1){printf("%s", "This is a good board!\n");}
	if(countSols > 1){printf("%s", "The puzzle has more than 1 solution, try to edit it further\n");}
	resetAllCells(board);
}
