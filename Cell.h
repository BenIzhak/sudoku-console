/*
 * Cell.h
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
	int limit;/* current last index of validnums ++++MIGHT NOT BE NEEDED++++ */
	int isInput;/* flag if the value in this cell is an input by the user */
	int isError; /* flag if the value in this cell is erroneous */
	int* validNums; /* array holding current valid numbers ++++MIGHT NOT BE NEEDED++++ */
	int* prevNums; /* array holding numbers which were tried previously ++++MIGHT NOT BE NEEDED++++ */
};

typedef struct _Cell Cell;

#endif /* CELL_H_ */
