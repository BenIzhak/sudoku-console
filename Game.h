/*
 * Game.h
 *
 *	Game Module --- this module is responsoble for different game functions, functions which are required for game operations
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
 * Function:  validate
 * --------------------
 *	checks if board is valid or not, first by checking for simple errors(same number more than once in a row/column/block
 *	then using ILP to check if board is solvable
 *
 *	isDone: used as flag, 0 - validating but the board is not completed, 1 - validating and all cells are filled
 *
 */
void validateCommand(int isDone);
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
 * Function:  getErrorsFlag
 * --------------------
 * 	returns errorsFlag value
 */
int getErrorsFlag();

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
 * Function:  generateCommand
 * --------------------
 *	Generates a puzzle by using fillAndKeep function
 *
 *
 *	cellsToFill: amount of cells to fill with random values
 *	cellsToKeep: amount of cells to keep after generating random values
 */

void generateCommand(int cellsToFill, int cellsToKeep);

/*
 * Function:  solveCommand
 * --------------------
 *	starting a solve mode with file at filepath
 *
 *	filePath: path of file of board to be solved
 */
void solveCommand(char* filePath);

/*
 * Function:  editCommand
 * --------------------
 *	starting an edit mode with file at filepath
 *
 *	filePath: path of file of board to be edited
 */
void editCommand(char* filePath , int numOfArgs);

/*
 * Function:  getGameMode
 * --------------------
 *  returns current gameMode code
 *
 */
int getGameMode();

/*
 * Function:  setGameMode
 * --------------------
 *  sets gameMode value
 *
 *  modeNum: code of gameMode to set to
 *
 */
void setGameMode(int modeNum);

/*
 * Function:  resetCommand
 * --------------------
 *  undo all moves, reverting the board to its original loaded state.
 */
void resetCommand();

/*
 * Function:  hardReset
 * --------------------
 * delete the whole command list and initialize a new one
 */
void hardReset(Cell** info);

/*
 * Function:  startNewCommandsList
 * --------------------
 * creating a new commands list or initialize an old one
 */
void startNewCommandsList();

/*
 * Function:  startDefaultBoard
 * --------------------
 * initialising a 9 by 9 board and starting a new command list
 *
 */
void startDefaultBoard();

/*
 * Function:  saveCommand
 * --------------------
 *	saving the current board to a txt file
 *
 *	filePath: path of file of board to be edited
 *
 */
void saveCommand(char* filePath);

/*
 * Function:  redoCommand
 * --------------------
 *  redo a command
 */
void redoCommand();

/*
 * Function:  undoCommand
 * --------------------
 *  undo a command
 */
void undoCommand();

/*
 * Function:  setHint
 * --------------------
 *  gives a hint for a cell at column of col and row of row
 *
 *  col: column of cell to show hint for
 *  row: rowof cell to show hint for
 */
void setHint(int col, int row);

/*
 * Function:  exitGameCommand
 * --------------------
 * exits the program and frees all resources of boards and commands list.
 *
 */
void exitGameCommand();

/*
 * Function:  autoFillCommand
 * --------------------
 *  automatically fills obvious values
 */
void autoFillCommand();

#endif /* GAME_H_ */
