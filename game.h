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


#endif /* GAME_H_ */
