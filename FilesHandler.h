/*
 * FilesHandler.h
 *
 *  Created on: 30 αιεμι 2018
 *      Author: beniz
 */

#ifndef FILESHANDLER_H_
#define FILESHANDLER_H_
#include "Cell.h"

/*
 *  Function:  loadBoard
 * --------------------
 *  loads file into boards
 *  returns 0 on success and -1 if failed
 *
 *  filePath: string of the file's path
 *  gameMode: the game mode (init, edit or solve)
 */
int loadBoard(char* filePath, int gameMode);

/*
 *  Function:  saveBoard
 * --------------------
 *  save the tempBoard into a given file
 *
 *  fp: pointer to the file
 */
void saveBoard(FILE* fp);

/*
 * Function:  openFile
 * --------------------
 *  open a new file. returns a pointer to the file.
 *
 *  filePath: path to the file
 *  mode: the mode that the file would be open.
 */
FILE* openFile(char* filePath, const char* mode);

#endif /* FILESHANDLER_H_ */
