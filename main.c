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


Cell** userBoard;
Cell** solvedBoard;
Cell** tempBoard;


int main(){
	int seed = time(NULL);
	SP_BUFF_SET();


	srand(seed);

	loadBoard("in.txt");
	saveBoard("out.txt");

	printBoard(userBoard, 0);



	return 0;
}
