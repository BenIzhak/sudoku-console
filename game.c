/*
 * game.c
 *
 *  Created on: 1 Aug 2018
 *      Author: Raz
 */
#include "Cell.h"

static int markErrors = 1;/* TODO:each time we begin a puzzle we need to set it back to 1 */
extern Cell** userBoard;
extern Cell** solvedBoard;
extern Cell** tempBoard;
extern int gameMode;

void setMarkErrors(int setting){
	markErrors = setting;
}

int getMarkErrors(){
	return markErrors;
}

int setCell(int X, int Y, int Z){
	if(userBoard[Y][X].fixed == 1){
		if(gameMode == 2){
			return -1;
		}
		if(gameMode == 1){
			userBoard[Y][X].fixed = 0;
			userBoard[Y][X].isInput = 1;
		}
	}
	userBoard[Y][X].currentNum = Z;
	/*TODO: mark num with asterik if it is an error */
	return 0;
}

/*int findError(){

}
int validate(){
	TODO:add call to find simple errors
	 add ILP call to check if solvable
}*/
