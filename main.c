/*
 * main.c
 *
 *  Created on: 29 ����� 2018
 *      Author: beniz
 */
#include <stdio.h>
#include <time.h>
#include "Cell.h"
#include "SPBufferset.h"
#include <stdlib.h>
#include "MainAux.h"
#include "FilesHandler.h"
#include "game.h"
#include "ExSolver.h"



int main(){
	int code;

	/*int seed = time(NULL);
	SP_BUFF_SET();

	srand(seed);

	gameLoop();*/
	boardInit(userBoard);
	boardInit(solvedBoard);
	boardInit(tempBoard);

	code = generate(20,5);
	switch(code){
		case 0:
			printf("%s", "not empty");
			break;
		case 1:
			printf("%s", "parameters are not valid");
			break;
		case 2:
			printf("%s","generator failed");
			break;
		case 3:
			printBoard(userBoard);
			break;
		default:
			break;
	}

	return 0;
}
