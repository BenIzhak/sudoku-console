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
	int i, j, boardRowAndColSize = brdData.blockRowSize * brdData.blockColSize;
 	for (i = 0; i < boardRowAndColSize; i++){
		for (j = 0; j < boardRowAndColSize; j++){
			board[i][j].countBackTracks = 0;
			board[i][j].maxOptions = 0;
			board[i][j].validNums = (int *) calloc((boardRowAndColSize), sizeof(int));
			if(board[i][j].validNums == NULL){
				printf("%s","Error: initBoardSolver has failed\n");
			}
		}
	}
}

void exitSolver(Cell** board){
	boardData brdData = getBoardData();
	int i, j, boardRowAndColSize = brdData.blockRowSize * brdData.blockColSize;
 	for(i = 0; i < boardRowAndColSize; i++){
		for(j = 0; j < boardRowAndColSize; j++){
			free(board[i][j].validNums);
		}
	}
}

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
int findValidNum(Cell** board, int cellRow, int cellCol){
	boardData brdData = getBoardData();
	int num , maxNum = brdData.blockRowSize * brdData.blockColSize, counter = 0;

		for(num = 1; num <= maxNum; num++){
			if(validAssignment(board, num, cellRow, cellCol) == 0){/* value is 0 if num is a valid assignment*/
				board[cellRow][cellCol].validNums[counter] = num;
				counter++;
			}else{
				board[cellRow][cellCol].validNums[num - 1] = 0;
			}
		}
		board[cellRow][cellCol].countBackTracks = 1;
		board[cellRow][cellCol].maxOptions = counter;
		return board[cellRow][cellCol].validNums[0];

}

cellIndex findEmptyCell(Cell** board, int cellRow, int cellCol){
	int i, j, flag = 0;
	boardData brdData = getBoardData();
	int rowSize = brdData.blockRowSize * brdData.blockColSize;
	int colSize = brdData.blockRowSize * brdData.blockColSize;
	cellIndex emptyIndex;

	for(i = cellRow ; i < rowSize ; i++){
		for(j = 0; j < colSize ; j++){
			if(i == cellRow && flag == 0){
				flag = 1;
				j = cellCol;
			}
			if(board[i][j].currentNum == 0){
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

void exBacktrack(Cell** board){
	cellIndex tempIndex;
	int flag = 1, cellCol = 0, cellRow = 0,  countSols = 0, foundNum = 0;
	int frstColIndex = -1; /* column of the first empty cell */
	int frstRowIndex = -1; /* row of the first empty cell */
	node* lastEmpty = NULL; /* pointer to last empty cell */

	initBoardSolver(board);

	if(getErrorsFlag() == 1){
		printf("%s", "Error: board contains erroneous values\n");
		return;
	}

	tempIndex = findEmptyCell(board, cellRow, cellCol);
	cellRow = tempIndex.row;
	cellCol = tempIndex.col;
	frstColIndex = cellCol;
	frstRowIndex = cellRow;
	push(&lastEmpty, cellRow, cellCol);

	while(flag){
			if(cellRow != -1 || cellCol != -1){/* find empty from prev iteration found an empty cell, sudoku board isn't full, */
				tempIndex = peek(lastEmpty);
				if(tempIndex.col != cellCol || tempIndex.row != cellRow){
					push(&lastEmpty, cellRow, cellCol);
				}

				if(board[cellRow][cellCol].countBackTracks == 0){
					foundNum = findValidNum(board, cellRow, cellCol);
				}else{
					if(board[cellRow][cellCol].maxOptions >= board[cellRow][cellCol].countBackTracks){
						foundNum =  board[cellRow][cellCol].validNums[board[cellRow][cellCol].countBackTracks];
						board[cellRow][cellCol].countBackTracks++;
					}else{
						foundNum = 0;
					}
				}

				if(foundNum == 0){
					if(cellRow == frstRowIndex && cellCol == frstColIndex){/*got back to the first empty cell and there are no more options */
						flag = 0;
						pop(&lastEmpty);
						board[cellRow][cellCol].countBackTracks = 0;
					}else{
						/*got stuck need to trackback*/
						pop(&lastEmpty);
						tempIndex = peek(lastEmpty);
						board[cellRow][cellCol].currentNum = 0;
						board[cellRow][cellCol].countBackTracks = 0;
						cellCol = tempIndex.col;
						cellRow = tempIndex.row;
					}
				}else{/* there is a number I can put in cell */
					board[cellRow][cellCol].currentNum = foundNum;
					/*printBoard(board);*/
					tempIndex = findEmptyCell(board, cellRow, cellCol);
					cellRow = tempIndex.row;
					cellCol = tempIndex.col;
				}
			}else{/* sudoku board is full, go back to prev empty */
				countSols++;
				tempIndex = peek(lastEmpty);
				cellCol = tempIndex.col;
				cellRow = tempIndex.row;
			}
	}
	printf("Number of solutions: %d\n", countSols);
	if(countSols == 1){printf("%s", "This is a good board!\n");}
	if(countSols > 1){printf("%s", "The puzzle has more than 1 solution, try to edit it further\n");}
	resetAllCells(board);
	exitSolver(board);
}
