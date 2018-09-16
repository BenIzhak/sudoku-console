/*
 * ILPSolver.h
 *
 *	ILPSolver --- this is the module that is responsible for the ILP solver algorithm, the header has
 *	just the ilpsolver function itself which solves the board
 *
 *
 *  Created on: 5 баев„ 2018
 *      Author: beniz
 */

#ifndef ILPSOLVER_H_
#define ILPSOLVER_H_

/*
 * Function:  ILPSolver
 * --------------------
 * try to solve the userBoard using ILP.
 * if the board is solvable it writes the solution to the tempBoard and returns 1,
 * if the board is unsolvable it doesn't change the tempBoard and returns 2,
 * if error occur return -1;
 */
int ILPSolver();

#endif /* ILPSOLVER_H_ */
