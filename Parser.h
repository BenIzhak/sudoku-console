/*
 * Parser.h
 *
 *	Parser Module --- this module is responsible for parsing user input,
 *	and making different operations on the user input
 *
 *  Created on: 29 αιεμι 2018
 *      Author: beniz
 */

#ifndef PARSER_H_
#define PARSER_H_


/*
 * Function:  parseCommand
 * --------------------
 *  get array of chars which represent the command from the user*
 *  and change the result array accordingly. the first variable *
 *  in the result array is the command type and the rest are the*
 *  parameters for the command
 *
 *  input: string of the input by the user
 *  command: array holding types of commands
 *  filePath: the string that will hold the path that given by the user
 *  numOfArgs: pointer to the variable that holds the number of arguments
 *  that supplied by the user
 *  notDigitFlag: change the value of it to 1 if a numerical value is not a numerical value.
 *
 *  returns: -1 if input is empty or 0 if valid
 */
int parseCommand(char* input, int* command, char* filePath, int* numOfArgs, int* notDigitFlag);

/*
 * Function:  validInput
 * --------------------
 * check if the input contain the correct amount of arguments
 *
 * command: array holding types of commands
 * numOfArgs: number of arguments that supplied by the user.
 * notDigitFlage: if the flag is 1 we don't print another error message.
 */
int validInput(int* command, int numOfArgs, int notDigitFlage);

#endif /* PARSER_H_ */
