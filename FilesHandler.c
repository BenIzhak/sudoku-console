/*
 * FilesHandler.c
 *
 *	FilesHandler Module --- this module is responsible for files operations, the source has all functions that are needed for different file
 *	operations are defined here.
 *
 *  Created on: 29 ����� 2018
 *      Author: beniz
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "MainAux.h"
#include "CommandsList.h"
#include "def.h"
#include "Game.h"

extern Cell** userBoard;
extern Cell** tempBoard;
extern Cell** solvedBoard;


/*
 * Function:  openFile
 * --------------------
 *  open a new file. returns a pointer to the file.
 *
 *  filePath: path to the file
 *  mode: the mode that the file would be open.
 */
FILE* openFile(char* filePath, const char* mode){
	FILE* fp = NULL;
	fp = fopen(filePath, mode);
	return fp;
}

/*
 * Function:  loadCellsValues
 * --------------------
 *  update the cell that located in the given cordinate.
 *
 *  rowCordinate: the row cordinate of the desired cell.
 *  colCordinate: the column cordinate of the desired cell.
 *  num: the number we want placed in the cell.
 *  fixed: fixed = 1 if cell is fixed and fixed = 0 otherwise
 */
void loadCellsValues(int rowCordinate, int colCordinate ,int num, int fixed){
	userBoard[rowCordinate][colCordinate].currentNum = num;
	if(num == 0){
		userBoard[rowCordinate][colCordinate].fixed = 0;
		userBoard[rowCordinate][colCordinate].isInput = 0;
		return;
	}
	if(fixed){
		userBoard[rowCordinate][colCordinate].fixed = 1;
		userBoard[rowCordinate][colCordinate].isInput = 0;
	}else{
		userBoard[rowCordinate][colCordinate].fixed = 0;
		userBoard[rowCordinate][colCordinate].isInput = 1;
	}
	tempBoard[rowCordinate][colCordinate].currentNum = num;
	if(fixed){
		tempBoard[rowCordinate][colCordinate].fixed = 1;
		tempBoard[rowCordinate][colCordinate].isInput = 0;
	}else{
		tempBoard[rowCordinate][colCordinate].fixed = 0;
		tempBoard[rowCordinate][colCordinate].isInput = 1;
	}
}

/*
 *  Function:  loadBoard
 * --------------------
 *  loads file into boards.
 *  returns 0 on success and -1 if failed.
 *
 *  filePath: string of the file's path
 *  gameMode: the game mode (init, edit or solve)
 */
int loadBoard(char* filePath, int gameMode){
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
	freeBoardMem(userBoard, brdData.blockRowSize, brdData.blockColSize);
	freeBoardMem(tempBoard, brdData.blockRowSize, brdData.blockColSize);
	freeBoardMem(solvedBoard, brdData.blockRowSize, brdData.blockColSize);

	/* set new values to blockRowSize and blockColSize */
	setBlockRowSize(m);
	setBlockColSize(n);

	/* allocate memory for news boards */
	boardRowAndColSize = m * n;
	userBoard = boardAllocatedMem(boardRowAndColSize);
	tempBoard = boardAllocatedMem(boardRowAndColSize);
	solvedBoard = boardAllocatedMem(boardRowAndColSize);

	/* init the boards */
	boardInit(userBoard);
	boardInit(tempBoard);
	boardInit(solvedBoard);


	/* update the cells*/
	i = 0, j = 0;
	while(fscanf(fp, "%d", &currentNum) != EOF){
		dot = fgetc(fp);
		if(dot == '.' && gameMode != EDIT_MODE){
			loadCellsValues(i, j , currentNum, 1);
		}else{
			loadCellsValues(i, j , currentNum, 0);
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


/*
 *  Function:  saveBoard
 * --------------------
 *  save the tempBoard into a given file according to the format,
 *  after that we close the file.
 *
 *  fp: pointer to the file
 */
void saveBoard(FILE* fp){
	int i, j, currentNum, fixed;
	boardData brdData = getBoardData();
	int boardRowAndColSize = brdData.blockColSize * brdData.blockRowSize;
	fprintf(fp, "%d %d \n", brdData.blockRowSize, brdData.blockColSize);
	for(i = 0; i < boardRowAndColSize; i++){
		for(j = 0; j < boardRowAndColSize; j++){
			currentNum = tempBoard[i][j].currentNum;
			fixed = tempBoard[i][j].fixed;
			if(j == 0){
				/* first cell of each line */
				if(fixed){
					fprintf(fp, "%d.", currentNum);
				}else{
					fprintf(fp, "%d", currentNum);
				}
			}else if(j == boardRowAndColSize - 1){
				/* last cell of each line */
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
}





