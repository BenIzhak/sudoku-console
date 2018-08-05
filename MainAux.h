/*
 * MainAux.h
 *
 *  Created on: 29 αιεμι 2018
 *      Author: beniz
 */

#ifndef MAINAUX_H_
#define MAINAUX_H_

#include "Cell.h"

Cell** userBoard;
Cell** solvedBoard;
Cell** tempBoard;

/* 0 - Init mode, 1 - Edit mode, 2 - Solve mode */
int gameMode;

void gameLoop();
void freeBoardMem(Cell** Board, int BlockRowSize, int BlockColSize);
Cell** setAllocatedMem(int boardRowAndColSize);
void printBoard(Cell** table, int markErrors);
void boardInit(Cell** table);
void copyBoard(Cell** dstBoard, Cell** srcBoard);

#endif /* MAINAUX_H_ */
