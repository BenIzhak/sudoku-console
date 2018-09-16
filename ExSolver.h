/*
 * ExSolver.h
 *
 *`ExSolver Module --- This module is responsible for the Exhaustive backtracking algorithm,
 * the header has the exhaustive backtracking itself for use of the num_solutions command and the
 * valid assignment function which is used by other functions
 *`
 *  Created on: 30 Jul 2018
 *      Author: Raz
 */
#ifndef EXSOLVER_H_
#define EXSOLVER_H_

/*
 * Function:  exBacktrack
 * --------------------
 * 	Exhaustive back tracking algorithm,
 * 	expecting to get  tempBoard when using this function
 *
 *	board: tempboard, 2d array containing sudoku cells
 *
 *	returns: amount of different solutions
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


#endif /* EXSOLVER_H_ */
