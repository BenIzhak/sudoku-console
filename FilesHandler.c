/*
 * FilesHandler.c
 *
 *  Created on: 29 αιεμι 2018
 *      Author: beniz
 */
#include <stdlib.h>
#include <stdio.h>
FILE* openFile(char* filePath, const char* mode){
	FILE* fp = NULL;

	fp = fopen(filePath, mode);

	if(fp == NULL){
		printf("%s","Error: File cannot be opened\n");
	}

	return fp;
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
 *
 */
int loadBoard(Cell** userBoard, Cell** tempBoard, char* filePath){
	FILE* fp = openFile(filePath,"r");

	if(fp == NULL){
		return -1;
	}


}

