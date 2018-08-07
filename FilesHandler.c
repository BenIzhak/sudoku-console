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

extern dll* commandsList;


FILE* openFile(char* filePath, const char* mode){
	FILE* fp = NULL;
	fp = fopen(filePath, mode);
	return fp;
}

void cellAssignment(int rowCordinate, int colCordinate ,int num, int fixed){
	boardData brdData = getBoardData();

	/* fixed = 1 if cell is fixed and fixed = 0 otherwise */
	brdData.userBoard[rowCordinate][colCordinate].currentNum = num;
	if(fixed){
		brdData.userBoard[rowCordinate][colCordinate].fixed = 1;
	}else{
		brdData.userBoard[rowCordinate][colCordinate].fixed = 0;
	}
	brdData.tempBoard[rowCordinate][colCordinate].currentNum = num;
	if(fixed){
		brdData.tempBoard[rowCordinate][colCordinate].fixed = 1;
	}else{
		brdData.tempBoard[rowCordinate][colCordinate].fixed = 0;
	}
}

/*
 *  Function:  loadBoard
 * --------------------
 *  loads file into boards
 *  returns 0 on success and -1 if failed
 *
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
	boardData brdData = getBoardData();
	if(fp == NULL){
		return -1;
	}
	fscanf(fp, "%d", &m);
	fscanf(fp, "%d", &n);

	/* free memory of previous boards */
	freeBoardMem(brdData.userBoard);
	freeBoardMem(brdData.tempBoard);
	freeBoardMem(brdData.solvedBoard);

	/* set new values to blockRowSize and blockColSize */
	setBlockColSize(m);
	setBlockRowSize(n);

	/* allocate memory for news boards */
	boardRowAndColSize = brdData.blockColSize * brdData.blockRowSize;
	brdData.userBoard = setAllocatedMem(boardRowAndColSize);
	brdData.tempBoard = setAllocatedMem(boardRowAndColSize);
	brdData.solvedBoard = setAllocatedMem(boardRowAndColSize);

	/* init the boards */
	boardInit(brdData.userBoard);
	boardInit(brdData.tempBoard);
	boardInit(brdData.solvedBoard);


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
	if(commandsList == NULL){
		/* commandList doesn't exist, so create and initialize one */
		commandsList = allocateListMem();
		initList(commandsList, brdData.userBoard);
	}else{
		/* commandList exists, so update the command list */
		hardReset(brdData.userBoard);
	}

	return 1;
}

int saveBoard(char* filePath){
	FILE* fp = openFile(filePath,"w");
	int i, j;
	int currentNum, fixed;
	boardData brdData = getBoardData();
	int boardRowAndColSize = brdData.blockColSize * brdData.blockRowSize;
	if(fp == NULL){
		return -1;
	}
	fprintf(fp, "%d %d \n", brdData.blockColSize, brdData.blockRowSize);
	for(i = 0; i < boardRowAndColSize; i++){
		for(j = 0; j < boardRowAndColSize; j++){
			currentNum = brdData.userBoard[i][j].currentNum;
			fixed = brdData.userBoard[i][j].fixed;
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




