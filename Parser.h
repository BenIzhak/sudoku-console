/*
 * Parser.h
 *
 *  Created on: 29 αιεμι 2018
 *      Author: beniz
 */

#ifndef PARSER_H_
#define PARSER_H_

/*
 * Function:  parseCommand
 * --------------------
 * get array of chars which represent the command from the user*
 * and change the result array accordingly. the first variable *
 * in the result array is the command type and the rest are the*
 * parameters for the command
 *
 *  input: string of the input by the user
 *  command: array holding types of commands
 *  filePath: the string that will hold the path that given by the user
 *  numOfArgs: pointer to the variable that holds the number of arguments that supplied by the user
 *
 *  returns: -1 if input is empty or 0 if valid
 */
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
