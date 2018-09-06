/*
 * MainAux.h
 *
 *  Created on: 29 αιεμι 2018
 *      Author: beniz
 */

#ifndef MAINAUX_H_
#define MAINAUX_H_

#include "Cell.h"

struct boardData {
	int blockRowSize;
	int blockColSize;
};

typedef struct boardData boardData;

/*
 * Function: gameLoop
 * --------------------
 * TODO document
 *
 *
 */
void gameLoop();

/*
 * Function: setAllocatedMem
 * --------------------
 * allocate memory for new board and return a pointer to the memory.
 * if memory has failed prints an error massage and returns NULL.
 */
Cell** setAllocatedMem();

/*
 * Function: printBoard
 * --------------------
 * we print the given board according to the format.
 *
 * table: the given board we want to print.
 */
void printBoard(Cell** table);

/*
 * Function: freeBoardMem
 * --------------------
 * we free the memory of a given board.
 *
 * Board: the given board we want to free it's memory.
 * blockRowSize: the block row size of the given board.
 * blockColSize: the block column size of the given board.
 */
void freeBoardMem(Cell** Board, int blockRowSize, int blockColSize);

/*
 * Function: boardInit
 * --------------------
 *  initialize the board fields.
 *
 *  table: the board we want to initialize.
 */
void boardInit(Cell** table);

/*
 * Function:  copyBoard
 * --------------------
 * copies srcBoard board to dstBoard.
 *
 *  dstBoard: 2d array holding a sudoku board, copy destination.
 *  srcBoard: 2d array holding a sudoku board, copy source.
 *
 */
void copyBoard(Cell** dstBoard, Cell** srcBoard);

/*
 * Function:  getBoardData
 * --------------------
 * return the boardData static object
 */
boardData getBoardData();

/*
 * Function:  setBlockRowSize
 * --------------------
 * set the block row size of boardData static object.
 */
void setBlockRowSize(int size);

/*
 * Function:  setBlockColSize
 * --------------------
 * set the block column size of boardData static object.
 */
void setBlockColSize(int size);

#endif /* MAINAUX_H_ */
