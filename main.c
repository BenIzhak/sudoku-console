/*
 * main.c
 *
 *  Created on: 29 ����� 2018
 *      Author: beniz
 */
#include <stdio.h>
#include <time.h>
#include "Cell.h"
#include <stdlib.h>

int main(int argc, char *argv[]){
	int seed = time(NULL);

	SP_BUFF_SET();

	srand(seed);

	gameLoop();

	return 0;
}
