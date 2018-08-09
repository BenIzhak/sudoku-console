/*
 * FilesHandler.h
 *
 *  Created on: 30 αιεμι 2018
 *      Author: beniz
 */

#ifndef FILESHANDLER_H_
#define FILESHANDLER_H_
#include "Cell.h"

int loadBoard(char* filePath, int gameMode);
void saveBoard(FILE* fp);
FILE* openFile(char* filePath, const char* mode);

#endif /* FILESHANDLER_H_ */
