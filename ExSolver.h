/*
 * ExSolver.h
 *
 *  Created on: 30 Jul 2018
 *      Author: Raz
 */

#ifndef EXSOLVER_H_
#define EXSOLVER_H_

/*  Function:  initBoardSolver
 *	--------------------
 *	initialize prevnums and validnums arrays which are used only in ExSolver.c functions
 *
 *  input:
 *  board-intialized 2d array*/
void initBoardSolver(Cell** board);

/*  Function:  exitSolver
 *	--------------------
 *	free validnums and prevnums memory,
 *	when exiting the exSolver there is no need of prevnums and validnums memory any more,
 *
 *  input:
 *  board-intialized 2d array*/
void exitSolver(Cell** board);

/*
 * Function:  exBacktrack
 * --------------------
 * 	Exhaustive back tracking algorithm
 *
 *	board: 2d array containing sudoku cells
 *
 *	returns: amount of different solutions
 *
 */
void exBacktrack(Cell** board);

/*  Function:  validAssignment
 *	--------------------
 *  checks if assigning numToCheck at cellCol,cellRow is a valid assignment,
 *  it doesn't assign it yet
 *
 *  input:
 *  board-intialized 2d array
 *  numToCheck - number to assign
 *  cellRow - row of cell to be assigned to
 *  cellCol - column of cell to be assigned to
 *
 *  output:
 *  0 - valid assignment
 *  -1 - invalid assignment*/

int validAssignment(Cell** board, int numToCheck, int cellRow, int cellCol);

/*
 * Function:  availableNumbers
 * --------------------
 * 	checks which numbers are available to the cell
 * 	at column of cellCol and row of cellRow and updates
 * 	the validNums array with them and the limit field accordingly
 *
 *	board: 2d array containing sudoku cells
 *	cellRow: cell's row
 *	cellCol: cells's column
 *
 */
void availableNumbers(Cell** board, int cellRow, int cellCol);

#endif /* EXSOLVER_H_ */
