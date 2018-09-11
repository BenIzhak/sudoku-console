/*
$
 * ILPSolver.c
 *
 *  Created on: 31 αιεμι 2018
 *      Author: beniz
 @

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gurobi_c.h"
#include "Cell.h"
#include "MainAux.h"

extern Cell** userBoard;
extern Cell ** solvedBoard;

$
 * Function: freeMem
 * --------------------
 *  free the memory we allocated for the ILPSolver
 *	the arguments are pointers to the memory we want to free.
 @
void freeMem(double *lowerBounds, double *val, char* vtype, int* ind) {
	free(lowerBounds);
	free(val);
	free(vtype);
	free(ind);
}

$
 * Function: ILPAllocateMem
 * --------------------
 *  allocate memory to the given pointers.
 *  if allocation has failed return -1, else return 0;
 *
 *	the arguments are pointers that point to the memory we allocate,
 *	and the size of the memory we want to allocate.
 @
int ILPAllocateMem(double** lowerBounds, char** vtype, int** ind, double** val,
		double** sol, int valuesMatrixDim, int boardRowAndColSize) {
	(*lowerBounds) = (double*) malloc(valuesMatrixDim * sizeof(double));
	(*vtype) = (char*) malloc(valuesMatrixDim * sizeof(char));
	(*ind) = (int*) malloc(boardRowAndColSize * sizeof(int));
	(*val) = (double*) malloc(boardRowAndColSize * sizeof(double));
	(*sol) = (double*) malloc(valuesMatrixDim * sizeof(double));
	if ((*lowerBounds) == NULL || (*vtype) == NULL || (*ind) == NULL || (*val) == NULL
			|| (*sol) == NULL) {
		return -1;
	}
	return 0;
}

$
 * Function: writeSolToBoard
 * --------------------
 *  write the the solution (if exist) from the sol array (the output of the ILP)
 *  to the actual board.
 *
 *  sol: the array that contains the solution for the board.
 @
void writeSolToBoard(double *sol) {
	int i, j, v;
	boardData brdData = getBoardData();
	int boardRowAndColSize = brdData.blockColSize * brdData.blockRowSize;
	for (i = 0; i < boardRowAndColSize; i++) {
		for (j = 0; j < boardRowAndColSize; j++) {
			for (v = 0; v < boardRowAndColSize; v++) {
				if (sol[i * boardRowAndColSize * boardRowAndColSize
						+ j * boardRowAndColSize + v] == 1) {
					solvedBoard[i][j].currentNum = v + 1;
				}
			}
		}
	}
}

$
 * Function: exitILP
 * --------------------
 *  check if an error occurred, or there is no solution to the board, or there is a solution.
 *  the function handles the cases appropriately.
 *  if an error occurred the function free the memory and return -1.
 *  if the is no solution to the board the optimStatus equal to GRB_INFEASIBLE,
 *  in that case the function will free the memory and return 2.
 *  if there is a solution to the board the optimStatus equal to GRB_OPTIMAL,
 *  in that case the function will get the solution to sol array, call writeSolToBoard
 *  to copy the solution to the board, free memory and return 1.
 *
 *  env: the gurobi environment we used.
 *  model: the gurobi model we used.
 *  error: a flag that tell us if an error occurred.
 *  optimStatus: the optimization status.
 *  sol: the array that will contains the solution.
 *  valuesMatrixDim: the size of the 3D binary matrix.
 @
int exitILP(GRBenv **env, GRBmodel **model, int error, int optimStatus,
		double *sol, int valuesMatrixDim) {
	int errorGetSol = 0;
	if (error == 0 && optimStatus == GRB_OPTIMAL) {
		$ the board is solveable @
		errorGetSol = GRBgetdblattrarray(*model, GRB_DBL_ATTR_X, 0,
				valuesMatrixDim, sol);
		if (errorGetSol == 0) {
			$ retrive the solution successfully @
			writeSolToBoard(sol);
			free(sol);
			GRBfreemodel(*model);
			GRBfreeenv(*env);
			return 1;
		}
		free(sol);
		GRBfreemodel(*model);
		GRBfreeenv(*env);
		return -1;
	} else if (error == 0 && optimStatus == GRB_INFEASIBLE) {
		$ the board is unsolveable @
		GRBfreemodel(*model);
		GRBfreeenv(*env);
		return 2;
	} else {
		$ error occured @
		GRBfreemodel(*model);
		GRBfreeenv(*env);
		return -1;
	}
}

$
 * Function: solInfo
 * --------------------
 *  get the optimization status and put it in the optimStatus variable.
 *  optimStatus == GRB_OPTIMAL -> board is solvable.
 *  optimStatus == GRB_INFEASIBLE -> board is unsolvable.
 *  the function returns the error value.
 *
 *  optimStatus: pointer to the optimization status variable.
 @
int solInfo(GRBmodel **model, int *optimStatus) {
	return GRBgetintattr(*model, GRB_INT_ATTR_STATUS, optimStatus);
}

$
 * Function: optimizemodel
 * --------------------
 *  call the GRBoptimize function to optimize the given model.
 *  the function returns the error value.
 *
 *  model: the model we want to optimize.
 @
int optimizemodel(GRBmodel **model) {
	return GRBoptimize(*model);
}

$
 * Function: oneValuePerBlock
 * --------------------
 *  the function add the constraint that can't be more than one of the same value in one block.
 *  the function returns the error value.
 *
 *  model: the model we want to optimize.
 *  ind: indices for non-zero values in the new constraint.
 *  val: the values of the variables in the new constraint.
 @
int oneValuePerBlock(GRBmodel **model, int* ind, double* val) {
	int i, j, v, k, l;
	int count, error = 0;
	boardData brdData = getBoardData();
	int boardRowAndColSize = brdData.blockColSize * brdData.blockRowSize;
	for (v = 0; v < boardRowAndColSize; v++) {
		for (k = 0; k < brdData.blockColSize; k++) {
			for (l = 0; l < brdData.blockRowSize; l++) {
				count = 0;
				for (i = k * brdData.blockRowSize; i < (k + 1) * brdData.blockRowSize; i++) {
					for (j = l * brdData.blockColSize; j < (l + 1) * brdData.blockColSize; j++) {
						ind[count] = i * boardRowAndColSize * boardRowAndColSize + j * boardRowAndColSize + v;
						val[count] = 1.0;
						count++;
					}
				}
				error = GRBaddconstr(*model, boardRowAndColSize, ind, val, GRB_EQUAL, 1.0, NULL);
				if (error) {
					return 1;
				}
			}
		}
	}
	return 0;
}

$
 * Function: oneValuePerCol
 * --------------------
 *  the function add the constraint that can't be more than one of the same value in one column.
 *  the function returns the error value.
 *
 *  model: the model we want to optimize.
 *  ind: indices for non-zero values in the new constraint.
 *  val: the values of the variables in the new constraint.
 @
int oneValuePerCol(GRBmodel **model, int* ind, double* val) {
	int i, j, v, error = 0;
	boardData brdData = getBoardData();
	int boardRowAndColSize = brdData.blockColSize * brdData.blockRowSize;
	for (v = 0; v < boardRowAndColSize; v++) {
		for (i = 0; i < boardRowAndColSize; i++) {
			for (j = 0; j < boardRowAndColSize; j++) {
				ind[j] = i * boardRowAndColSize * boardRowAndColSize + j * boardRowAndColSize + v;
				val[j] = 1.0;
			}
			error = GRBaddconstr(*model, boardRowAndColSize, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				return 1;
			}
		}
	}
	return 0;
}

$
 * Function: oneValuePerRow
 * --------------------
 *  the function add the constraint that can't be more than one of the same value in one row.
 *  the function returns the error value.
 *
 *  model: the model we want to optimize.
 *  ind: indices for non-zero values in the new constraint.
 *  val: the values of the variables in the new constraint.
 @
int oneValuePerRow(GRBmodel **model, int* ind, double* val) {
	int i, j, v, error = 0;
	boardData brdData = getBoardData();
	int boardRowAndColSize = brdData.blockColSize * brdData.blockRowSize;
	for (v = 0; v < boardRowAndColSize; v++) {
		for (j = 0; j < boardRowAndColSize; j++) {
			for (i = 0; i < boardRowAndColSize; i++) {
				ind[i] = i * boardRowAndColSize * boardRowAndColSize + j * boardRowAndColSize + v;
				val[i] = 1.0;
			}
			error = GRBaddconstr(*model, boardRowAndColSize, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				return 1;
			}
		}
	}
	return 0;
}

$
 * Function: oneValuePerCell
 * --------------------
 *  the function add the constraint that only one value can be in one cell.
 *  the function returns the error value.
 *
 *  model: the model we want to optimize.
 *  ind: indices for non-zero values in the new constraint.
 *  val: the values of the variables in the new constraint.
 @
int oneValuePerCell(GRBmodel **model, int* ind, double* val) {
	int i, j, v, error = 0;
	boardData brdData = getBoardData();
	int boardRowAndColSize = brdData.blockColSize * brdData.blockRowSize;
	for (i = 0; i < boardRowAndColSize; i++) {
		for (j = 0; j < boardRowAndColSize; j++) {
			for (v = 0; v < boardRowAndColSize; v++) {
				ind[v] = i * boardRowAndColSize * boardRowAndColSize + j * boardRowAndColSize + v;
				val[v] = 1.0;
			}
			error = GRBaddconstr(*model, boardRowAndColSize, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				return 1;
			}
		}
	}
	return 0;
}

$
 * Function: createNewModel
 * --------------------
 *  the function creates a new model by calling the  createNewModel.
 *  the function returns the error value.
 *
 *	env: the gurobi environment we used.
 *  model: the model we want to optimize.
 *  lowerBounds: lower bounds for the new variables.
 *  vtype: types for the variables. in our case GRB_BINARY.
 *  valuesMatrixDim: the size of the 3D binary matrix.
 @
int createNewModel(GRBenv **env, GRBmodel **model, double *lowerBounds,
		char *vtype, int valuesMatrixDim) {
	return GRBnewmodel(*env, model, "sudokuILP", valuesMatrixDim, NULL,
			lowerBounds, NULL, vtype, NULL);
}

$
 * Function: cancelPrints
 * --------------------
 *  the function canceling prints from Gurobi methods.
 *  the function returns the error value.
 *
 *	env: the gurobi environment we used.
 @
int cancelPrints(GRBenv **env) {
	return GRBsetintparam(*env, GRB_INT_PAR_LOGTOCONSOLE, 0);
}

$
 * Function: createEnvironment
 * --------------------
 *  the function create new environment using GRBloadenv.
 *  the function returns the error value.
 *
 *	env: the gurobi environment we use.
 @
int createEnvironment(GRBenv **env) {
	return GRBloadenv(env, "sudokuILP.log");
}

$
 * Function: addVariables
 * --------------------
 *  the function transfers variables from the board we want to solve to the arrays the gurobi
 *  use to solve.
 *
 *  lowerBounds: lower bounds for the new variables.
 *  vtype: types for the variables. in our case GRB_BINARY.
 @
void addVariables(double *lowerBounds, char *vtype) {
	int i, j, v;
	boardData brdData = getBoardData();
	int boardRowAndColSize = brdData.blockColSize * brdData.blockRowSize;
	for (i = 0; i < boardRowAndColSize; i++) {
		for (j = 0; j < boardRowAndColSize; j++) {
			for (v = 0; v < boardRowAndColSize; v++) {
				if (userBoard[i][j].currentNum == v + 1) {
					lowerBounds[i * boardRowAndColSize * boardRowAndColSize
							+ j * boardRowAndColSize + v] = 1;
				} else {
					lowerBounds[i * boardRowAndColSize * boardRowAndColSize
							+ j * boardRowAndColSize + v] = 0;
				}
				vtype[i * boardRowAndColSize * boardRowAndColSize
						+ j * boardRowAndColSize + v] = GRB_BINARY;
			}
		}
	}
}

$
 * -------------------------------
 * ILPSolver Documentation is in header file
 * -------------------------------
 @
int ILPSolver() {
	GRBenv *env = NULL;
	GRBmodel *model = NULL;
	int *ind, valuesMatrixDim, boardRowAndColSize, optimStatus, error = 0;
	double *lowerBounds, *val, *sol;
	char* vtype;
	boardData brdData = getBoardData();
	boardRowAndColSize = brdData.blockColSize * brdData.blockRowSize;
	valuesMatrixDim = boardRowAndColSize * boardRowAndColSize
			* boardRowAndColSize;

	error = ILPAllocateMem(&lowerBounds, &vtype, &ind, &val, &sol,valuesMatrixDim, boardRowAndColSize);
	if (error == -1) {
		printf("%s", "Error: ILPAllocateMem has failed\n");
		return -1;
	}

	addVariables(lowerBounds, vtype);

	error = createEnvironment(&env);
	if (error) {
		printf("createEnvironment");
		freeMem(lowerBounds, val, vtype, ind);
		return exitILP(&env, &model, error, optimStatus, sol, valuesMatrixDim);
	}

	error = cancelPrints(&env);
	if (error) {
		freeMem(lowerBounds, val, vtype, ind);
		return exitILP(&env, &model, error, optimStatus, sol, valuesMatrixDim);
	}

	error = createNewModel(&env, &model, lowerBounds, vtype, valuesMatrixDim);
	if (error) {
		printf("createNewModel");
		freeMem(lowerBounds, val, vtype, ind);
		return exitILP(&env, &model, error, optimStatus, sol, valuesMatrixDim);
	}

	error = oneValuePerCell(&model, ind, val);
	if (error) {
		printf("oneValuePerCell");
		freeMem(lowerBounds, val, vtype, ind);
		return exitILP(&env, &model, error, optimStatus, sol, valuesMatrixDim);
	}

	error = oneValuePerRow(&model, ind, val);
	if (error) {
		printf("oneValuePerRow");
		freeMem(lowerBounds, val, vtype, ind);
		return exitILP(&env, &model, error, optimStatus, sol, valuesMatrixDim);
	}

	error = oneValuePerCol(&model, ind, val);
	if (error) {
		printf("oneValuePerCol");
		freeMem(lowerBounds, val, vtype, ind);
		return exitILP(&env, &model, error, optimStatus, sol, valuesMatrixDim);
	}

	error = oneValuePerBlock(&model, ind, val);
	if (error) {
		printf("oneValuePerBlock");
		freeMem(lowerBounds, val, vtype, ind);
		return exitILP(&env, &model, error, optimStatus, sol, valuesMatrixDim);
	}

	error = optimizemodel(&model);
	if (error) {
		printf("optimizemodel");
		freeMem(lowerBounds, val, vtype, ind);
		return exitILP(&env, &model, error, optimStatus, sol, valuesMatrixDim);
	}

	error = solInfo(&model, &optimStatus);
	if (error) {
		printf("solInfo");
		freeMem(lowerBounds, val, vtype, ind);
		return exitILP(&env, &model, error, optimStatus, sol, valuesMatrixDim);
	}

	freeMem(lowerBounds, val, vtype, ind);
	return exitILP(&env, &model, error, optimStatus, sol, valuesMatrixDim);
}
*/

int ILPSolver() {
	return -1;
}

