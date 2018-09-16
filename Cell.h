/*
 * Cell.h
 *
 * this is the cell's struct, it has the fields needed for each cell in the sudoku board
 *
 *  Created on: 29 Jul 2018
 *      Author: Raz
 */

#ifndef CELL_H_
#define CELL_H_

struct _Cell
{
	int currentNum; /* the cell current appearing number */
	int fixed;/* flag if cell is a fixed number */
	int limit;/* current last index of validnums  */
	int isInput;/* flag if the value in this cell is an input by the user */
	int isError; /* flag if the value in this cell is erroneous */
	int blockRow; /* ending row of the block of the cell */
	int blockCol; /* ending column of the block of the cell */
	int* validNums; /* array holding valid numbers of current cell */
};

typedef struct _Cell Cell;

#endif /* CELL_H_ */
