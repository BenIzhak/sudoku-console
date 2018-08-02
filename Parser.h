/*
 * Parser.h
 *
 *  Created on: 29 αιεμι 2018
 *      Author: beniz
 */

#ifndef PARSER_H_
#define PARSER_H_

int parseCommand(char* input, int* command, char* filePath, int* numOfArgs);

/*
 * Function:  validInput
 * --------------------
 * 	TODO:add description
 *
 *	returns: 0 if input is OK, otherwise returns -1
 *
 *	command: TODO:add description
 */
int validInput(int* command, int numOfArgs);


#endif /* PARSER_H_ */
