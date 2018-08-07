/*
 * MainAux.h
 *
 *  Created on: 29 αιεμι 2018
 *      Author: beniz
 */

#ifndef MAINAUX_H_
#define MAINAUX_H_

#include "Cell.h"

struct boardData{
	 int blockRowSize;
	 int blockColSize;
};

typedef struct boardData boardData;


void gameLoop();
Cell** setAllocatedMem();
void printBoard(Cell** table);
void freeBoardMem(Cell** Board);
void boardInit(Cell** table);
void copyBoard(Cell** dstBoard, Cell** srcBoard);
boardData getBoardData();
void setBlockRowSize(int size);
void setBlockColSize(int size);
#endif /* MAINAUX_H_ */
