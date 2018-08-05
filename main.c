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

	int seed = time(NULL);
	SP_BUFF_SET();

	srand(seed);

	gameLoop();


	return 0;
}
