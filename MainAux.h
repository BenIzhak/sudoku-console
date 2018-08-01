/*
 * MainAux.h
 *
 *  Created on: 29 αιεμι 2018
 *      Author: beniz
 */

#ifndef MAINAUX_H_
#define MAINAUX_H_

#include "Cell.h"

void gameLoop();
void freeBoardMem(Cell** gameBoard, Cell** solvedBoard, Cell** tempBoard, int BlockRowSize, int BlockColSize);
Cell** setAllocatedMem();
void printBoard(Cell** table);
void boardInit(Cell** table);

Cell** setAllocatedMem();
void boardInit(Cell** table);

#endif /* MAINAUX_H_ */
