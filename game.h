/*
 * game.h
 *
 *  Created on: 1 Aug 2018
 *      Author: Raz
 */

#ifndef GAME_H_
#define GAME_H_

/*
 * Function:  setMarkErrors
 * --------------------
 * 	sets markErrors value to setting value
 *
 *
 *	setting:0 or 1 for mark_errors
 */
void setMarkErrors(int setting);

/*
 * Function:  getMarkErrors
 * --------------------
 * 	returns mark_errors value
 *
 *
 */
int getMarkErrors();

/*
 * Function:  findAndMarkErrors
 * --------------------
 * 	goes over all cells in board and checks whether their assignment is valid.
 * 	if it isn't valid mark their cell as an error and change errorsFlag to -1 if there are no
 * 	errors change errorsFlag to 0
 *
 *	returns: -1 if an error was found, 0 if no error was found
 *
 */
void findAndMarkErrors();

/*
 * Function:  setCell
 * --------------------
 * 	sets cell in column of X and row of Y to value of Z
 * 	if in solve mode set isInput to 1, if in edit mode set fixed to 1
 *
 *	returns: -1 if in solve mode and cell is fixed, if set is successful returns 0
 *
 *	X: column of cell
 *  Y: row of cell
 *	Z: new value to set cell to
 */

int setCell(int X, int Y, int Z);

/*
 * Function:  validate
 * --------------------
 *	checks if board is valid or not, first by checking for simple errors(same number more than once in a row/column/block
 *	then using ILP to check if board is solvable
 *
 *	returns: 0 - simple error, 1 - board is not solvable, 2 - board is solvable
 *
 */
int validate();

/*
 * Function:  generate
 * --------------------
 *	Generates a puzzle by using fillAndKeep function
 *
 *
 *	returns: 0 - board is not empty, 1 - cellsToFill or cellsToKeep are more than the amount of empty cells
 *			 2 - generator failed, 3 - success print board
 *
 *	cellsToFill: amount of cells to fill with random values
 *	cellsToKeep: amount of cells to keep after generating random values
 */

int generate(int cellsToFill, int cellsToKeep);

void solveCommand(char* filePath);
void editCommand(char* filePath , int numOfArgs);

int getGameMode();
void setGameMode(int modeNum);

void reset();
void hardReset(Cell** info);

void startNewCommandsList();

void startDefaultBoard();

void redo();
void undo();

void setHint(int col, int row);
void exitGameCommand();

#endif /* GAME_H_ */
