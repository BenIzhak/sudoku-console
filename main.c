/*
 * main.c
 *
 *  Created on: 29 ����� 2018
 *      Author: beniz
 */

#include <time.h>
#include <stdlib.h>
#include "SPBufferset.h"
#include "MainAux.h"


int main(){
	int seed = time(NULL);
	srand(seed);
	SP_BUFF_SET();

	gameLoop();

	return 0;
}
