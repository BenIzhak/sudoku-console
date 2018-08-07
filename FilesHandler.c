/*
 * FilesHandler.c
 *
 *  Created on: 29 αιεμι 2018
 *      Author: beniz
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "MainAux.h"
#include "game.h"
#include "CommandsList.h"

int blockRowSize;
int blockColSize;

extern Cell** userBoard;
extern Cell** solvedBoard;
extern Cell** tempBoard;


FILE* openFile(char* filePath, const char* mode){
	FILE* fp = NULL;
	fp = fopen(filePath, mode);
	return fp;
}

void cellAssignment(int rowCordinate, int colCordinate ,int num, int fixed){
	/* fixed = 1 if cell is fixed and fixed = 0 otherwise */
	userBoard[rowCordinate][colCordinate].currentNum = num;
	if(fixed){
		userBoard[rowCordinate][colCordinate].fixed = 1;
	}else{
		userBoard[rowCordinate][colCordinate].fixed = 0;
	}
	tempBoard[rowCordinate][colCordinate].currentNum = num;
	if(fixed){
		tempBoard[rowCordinate][colCordinate].fixed = 1;
	}else{
		tempBoard[rowCordinate][colCordinate].fixed = 0;
	}
}

/*
 *  Function:  loadBoard
 * --------------------
 *  loads file into boards
 *  returns 0 on success and -1 if failed
 *
 *  userBoard: 2d array holding a sudoku board, it holds the board the user is using
 *  tempBoard: 2d array holding a sudoku board, just a temp board, values doesn't matter
 *  filePath: string of the file's path
 */
int loadBoard(char* filePath){
	/* reads new m and n values from file, if reading fails returns -1 */
	/*TODO: check if fscanf success*/
	FILE* fp = openFile(filePath,"r");
	int m, n, i, j;
	int currentNum = 0;
	char dot;
	int boardRowAndColSize;
	if(fp == NULL){
		return -1;
	}
	fscanf(fp, "%d", &m);
	fscanf(fp, "%d", &n);

	/* free memory of previous boards */
	freeBoardMem(userBoard);
	freeBoardMem(tempBoard);
	freeBoardMem(solvedBoard);

	/* set new values to blockRowSize and blockColSize */
	blockColSize = m;
	blockRowSize = n;

	/* allocate memory for news boards */
	boardRowAndColSize = blockColSize * blockRowSize;
	userBoard = setAllocatedMem(boardRowAndColSize);
	tempBoard = setAllocatedMem(boardRowAndColSize);
	solvedBoard = setAllocatedMem(boardRowAndColSize);

	/* init the boards */
	boardInit(userBoard);
	boardInit(tempBoard);
	boardInit(solvedBoard);


	/* update the cells*/
	i = 0, j = 0;
	while(fscanf(fp, "%d", &currentNum) != EOF){
		dot = fgetc(fp);
		if(dot == '.'){
			cellAssignment(i, j , currentNum, 1);
		}else{
			cellAssignment(i, j , currentNum, 0);
		}
		/* move to the next cell */
		if(j < boardRowAndColSize-1){
			j++;
		}else if(j == boardRowAndColSize-1){
			j = 0;
			i++;
		}
	}
	/* we don't need the file anymore, we can close it */
	fclose(fp);

	/* update the command list */
	startNewCommandsList();

	return 1;
}

int saveBoard(char* filePath){
	FILE* fp = openFile(filePath,"w");
	int i, j;
	int currentNum, fixed;
	int boardRowAndColSize = blockColSize * blockRowSize;
	if(fp == NULL){
		return -1;
	}
	fprintf(fp, "%d %d \n", blockColSize, blockRowSize);
	for(i = 0; i < boardRowAndColSize; i++){
		for(j = 0; j < boardRowAndColSize; j++){
			currentNum = userBoard[i][j].currentNum;
			fixed = userBoard[i][j].fixed;
			if(j == 0){
				if(fixed){
					fprintf(fp, "%d.", currentNum);
				}else{
					fprintf(fp, "%d", currentNum);
				}
			}else if(j == boardRowAndColSize - 1){
				if(fixed){
					fprintf(fp, " %d.\n", currentNum);
				}else{
					fprintf(fp, " %d\n", currentNum);
			}
			}else{
				if(fixed){
					fprintf(fp, " %d.", currentNum);
				}else{
					fprintf(fp, " %d", currentNum);
				}
			}
		}
	}
	fclose(fp);
	return 1;
}




