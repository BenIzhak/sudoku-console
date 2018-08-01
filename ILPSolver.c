
/*$
 * ILPSolver.c
 *
 *  Created on: 31 ����� 2018
 *      Author: beniz
 $

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gurobi_c.h"
#include "Cell.h"

extern Cell** userBoard;
extern Cell ** tempBoard;

extern int blockRowSize;
extern int blockColSize;

void freeMem(double *lowerBounds, double *val, char* vtype, int* ind){
	$ free alocated memory except "sol" which free in exitILP function $
	free(lowerBounds);
	free(val);
	free(vtype);
	free(ind);
}

void writeSolToBoard(Cell** Board, double *sol, int boardRowAndColSize){
	int i, j, v;
	for (i = 0; i < boardRowAndColSize; i++) {
		for (j = 0; j < boardRowAndColSize; j++) {
			for (v = 0; v < boardRowAndColSize; v++) {
				if (sol[i * boardRowAndColSize * boardRowAndColSize + j * boardRowAndColSize + v] == 1) {
					Board[i][j].currentNum = v + 1;
				}
			}
		}
	}
}

int exitILP(GRBenv **env, GRBmodel **model, int error, int optimStatus,  double *sol, int boardRowAndColSize, int valuesMatrixDim){
	int errorGetSol = 0;
	if(error == 0 && optimStatus == GRB_OPTIMAL){
		$ the board is solveable $
		errorGetSol = GRBgetdblattrarray(*model, GRB_DBL_ATTR_X, 0, valuesMatrixDim, sol);
		if(errorGetSol == 0){
			$ retrive the solution successfully $
			printf("%s", "Validation passed: board is solvable\n");
			writeSolToBoard(tempBoard, sol, boardRowAndColSize);
			free(sol);
			GRBfreemodel(*model);
			GRBfreeenv(*env);
			return 1;
		}
		free(sol);
		GRBfreemodel(*model);
		GRBfreeenv(*env);
		return -1;
	}else if(error == 0 && optimStatus == GRB_INFEASIBLE){
		$ the board is unsolveable $
		printf("%s", "Validation failed: board is unsolvable\n");
		GRBfreemodel(*model);
		GRBfreeenv(*env);
		return 1;
	}else{
		$ error occured $
		GRBfreemodel(*model);
		GRBfreeenv(*env);
		return -1;
	}
}

int solInfo(GRBmodel **model, int *optimStatus){
	$ get the optimization status:
	 * optimStatus == GRB_OPTIMAL -> board is solvable
	 * optimStatus == GRB_INFEASIBLE -> board is unsolvable$
	return GRBgetintattr(*model, GRB_INT_ATTR_STATUS, optimStatus);
}

int optimizemodel(GRBmodel **model){
	return GRBoptimize(*model);
}

int oneValuePerBlock(GRBmodel **model, int* ind, double* val, int boardRowAndColSize){
	$ add constraint: each value is used exactly once per block $
	int i,j,v,k,l;
	int count, error = 0;
	for (v = 0; v < boardRowAndColSize; v++) {
		for (k = 0; k < blockRowSize; k++) {
			for (l = 0; l < blockColSize; l++) {
				count = 0;
				for (i = k * blockRowSize; i < (k + 1) * blockRowSize; i++) {
					for (j = l * blockColSize; j < (l + 1) * blockColSize; j++) {
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

int oneValuePerCol(GRBmodel **model, int* ind, double* val, int boardRowAndColSize){
	$ add constraint: each value is used exactly once per column $
	int i,j,v, error = 0;
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

int oneValuePerRow(GRBmodel **model, int* ind, double* val, int boardRowAndColSize){
	$ add constraint: each value is used exactly once per row $
	int i,j,v, error = 0;
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

int oneValuePerCell(GRBmodel **model, int* ind, double* val, int boardRowAndColSize){
	$ add constraint: each cell must have exactly one value $
	int i,j,v, error = 0;
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

int createNewModel(GRBenv **env, GRBmodel **model, double *lowerBounds, char *vtype, int valuesMatrixDim){
	return GRBnewmodel(*env, model, "sudokuILP", valuesMatrixDim, NULL, lowerBounds, NULL, vtype, NULL);
}

int cancelPrints(GRBenv **env){
	$ Canceling prints from Gurobi methods$
	return GRBsetintparam(*env, GRB_INT_PAR_LOGTOCONSOLE, 0);
}

int createEnvironment(GRBenv **env){
	return GRBloadenv(env, "sudokuILP.log");
}

void addVariables(double *lowerBounds, char *vtype, int boardRowAndColSize){
	int i, j, v;
	for (i = 0; i < boardRowAndColSize; i++) {
		for (j = 0; j < boardRowAndColSize; j++) {
			for (v = 0; v < boardRowAndColSize; v++) {
				if (userBoard[i][j].currentNum == v + 1) {
					lowerBounds[i * boardRowAndColSize * boardRowAndColSize + j * boardRowAndColSize + v] = 1;
				} else {
					lowerBounds[i * boardRowAndColSize * boardRowAndColSize + j * boardRowAndColSize + v] = 0;
				}
				vtype[i * boardRowAndColSize * boardRowAndColSize + j * boardRowAndColSize + v] = GRB_BINARY;
			}
		}
	}
}

int ILPSolver() {
	$ try to solve the userBoard using ILP.
	 * if the board is solvable it prints the massege and write the solution to the tempBoard and return 1,
	 * if the board is unsolvable it prints the massege and doesnt change the tempBoard and return 1,
	 * if error occur return -1; $
	GRBenv *env = NULL;
	GRBmodel *model = NULL;
	int boardRowAndColSize;
	int valuesMatrixDim; $ the size of the 3D binary matrix $
	int optimStatus, error = 0;
	double *lowerBounds, *val, *sol;
	char* vtype; $variable types$
	int* ind;

	boardRowAndColSize = blockRowSize * blockColSize;
	valuesMatrixDim = boardRowAndColSize * boardRowAndColSize * boardRowAndColSize;

	lowerBounds = (double*) malloc(valuesMatrixDim * sizeof(double));
	vtype = (char*) malloc(valuesMatrixDim * sizeof(char));
	ind = (int*) malloc(boardRowAndColSize * sizeof(int));
	val = (double*) malloc(boardRowAndColSize * sizeof(double));
	sol = (double*) malloc(valuesMatrixDim * sizeof(double));

	addVariables(lowerBounds, vtype, boardRowAndColSize);

	error = createEnvironment(&env);
	if(error){
		freeMem(lowerBounds, val, vtype, ind);
		return exitILP(&env, &model, error, optimStatus, sol, boardRowAndColSize, valuesMatrixDim);
	}

	error = cancelPrints(&env);
	if(error){
		freeMem(lowerBounds, val, vtype, ind);
		return exitILP(&env, &model, error, optimStatus, sol, boardRowAndColSize, valuesMatrixDim);
	}

	error = createNewModel(&env, &model, lowerBounds, vtype, valuesMatrixDim);
	if(error){
		freeMem(lowerBounds, val, vtype, ind);
		return exitILP(&env, &model, error, optimStatus, sol, boardRowAndColSize, valuesMatrixDim);
	}

	error = oneValuePerCell(&model, ind, val, boardRowAndColSize);
	if(error){
		freeMem(lowerBounds, val, vtype, ind);
		return exitILP(&env, &model, error, optimStatus, sol, boardRowAndColSize, valuesMatrixDim);
	}

	error = oneValuePerRow(&model, ind, val, boardRowAndColSize);
	if(error){
		freeMem(lowerBounds, val, vtype, ind);
		return exitILP(&env, &model, error, optimStatus, sol, boardRowAndColSize, valuesMatrixDim);
	}

	error = oneValuePerCol(&model, ind, val, boardRowAndColSize);
	if(error){
		freeMem(lowerBounds, val, vtype, ind);
		return exitILP(&env, &model, error, optimStatus, sol, boardRowAndColSize, valuesMatrixDim);
	}

	error = oneValuePerBlock(&model, ind, val, boardRowAndColSize);
	if(error){
		freeMem(lowerBounds, val, vtype, ind);
		return exitILP(&env, &model, error, optimStatus, sol, boardRowAndColSize, valuesMatrixDim);
	}

	error = optimizemodel(&model);
	if(error){
		freeMem(lowerBounds, val, vtype, ind);
		return exitILP(&env, &model, error, optimStatus, sol, boardRowAndColSize, valuesMatrixDim);
	}

	error = solInfo(&model, &optimStatus);
	if(error){
		freeMem(lowerBounds, val, vtype, ind);
		return exitILP(&env, &model, error, optimStatus, sol, boardRowAndColSize, valuesMatrixDim);
	}

	freeMem(lowerBounds, val, vtype, ind);
	return exitILP(&env, &model, error, optimStatus, sol, boardRowAndColSize, valuesMatrixDim);
}
*/
int foo(){
	return 1;
}
