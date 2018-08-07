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


void gameLoop();
Cell** setAllocatedMem();
void printBoard(Cell** table);
void freeBoardMem(Cell** Board);
void boardInit(Cell** table);
void copyBoard(Cell** dstBoard, Cell** srcBoard);

#endif /* MAINAUX_H_ */
