/*
 * Parser.c
 *
 *  Created on: 29 αιεμι 2018
 *      Author: beniz
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MainAux.h"
#include "def.h"
#include "Game.h"

enum COMMAND {
	solve,
	edit,
	mark_errors,
	print_board,
	set,
	validate,
	generate,
	undo,
	redo,
	save,
	hint,
	num_solutions,
	autofill,
	reset,
	exit1,
	error
};

/*
 * -------------------------------
 * parseCommand Documentation is in header file
 * -------------------------------
 */

int parseCommand(char* input, int* command, char* filePath, int* numOfArgs, int* notDigitFlag) {
	int isDigit(char* token);
	void notDigitError(int* command, int* notDigitFlag);
	const char delim[7] = " \t\r\n";
	char *token;
	int i;
	(*numOfArgs) = 0;
	(*notDigitFlag) = 0;
	if (strlen(input) > 256) {
		command[0] = error;
		printf("%s", "ERROR: invalid command\n");
		return -1;
	}
	token = strtok(input, delim);
	for (i = 0; i < 4; i++) {
		command[i] = -1;
	}
	if (token == NULL) {
		command[0] = error;
		return -1;
	}
	/* Get the command type */
	if (strcmp(token, "solve") == 0) {
		command[0] = solve;
	} else if (strcmp(token, "edit") == 0) {
		command[0] = edit;
	} else if (strcmp(token, "mark_errors") == 0) {
		command[0] = mark_errors;
	} else if (strcmp(token, "print_board") == 0) {
		command[0] = print_board;
	} else if (strcmp(token, "set") == 0) {
		command[0] = set;
	} else if (strcmp(token, "validate") == 0) {
		command[0] = validate;
	} else if (strcmp(token, "generate") == 0) {
		command[0] = generate;
	} else if (strcmp(token, "undo") == 0) {
		command[0] = undo;
	} else if (strcmp(token, "redo") == 0) {
		command[0] = redo;
	} else if (strcmp(token, "save") == 0) {
		command[0] = save;
	} else if (strcmp(token, "hint") == 0) {
		command[0] = hint;
	} else if (strcmp(token, "num_solutions") == 0) {
		command[0] = num_solutions;
	} else if (strcmp(token, "autofill") == 0) {
		command[0] = autofill;
	} else if (strcmp(token, "reset") == 0) {
		command[0] = reset;
	} else if (strcmp(token, "exit") == 0) {
		command[0] = exit1;
	} else {
		printf("%s", "ERROR: invalid command\n");
		command[0] = error;
	}
	token = strtok(NULL, delim);
	if (token != NULL
			&& (command[0] == solve || command[0] == edit || command[0] == save)) {
		sscanf(token, "%s", filePath);
		(*numOfArgs)++;
	} else {
		if (token != NULL) {
			if (isDigit(token) != -1) {
				sscanf(token, "%d", &command[1]);
			} else {
				notDigitError(command, notDigitFlag);
			}
			(*numOfArgs)++;
		}
	}
	token = strtok(NULL, delim);
	if (token != NULL) {
		if (isDigit(token) != -1) {
			sscanf(token, "%d", &command[2]);
		} else {
			notDigitError(command, notDigitFlag);
		}
		(*numOfArgs)++;
	}
	token = strtok(NULL, delim);
	if (token != NULL) {
		if (isDigit(token) != -1) {
			sscanf(token, "%d", &command[3]);
		} else {
			notDigitError(command, notDigitFlag);
		}
		(*numOfArgs)++;
	}
	return 0;
}

/*
 * Function:  validRange
 * --------------------
 * check if the input is in the correct range.
 *
 * command: array holding types of commands
 */
int validRange(int* command) {
	int i;
	boardData brdData = getBoardData();
	int boardRowAndColSize = brdData.blockRowSize * brdData.blockColSize;
	if (command[0] == mark_errors) {
		if ((command[1] != 0) && (command[1] != 1)) {
			printf("%s", "Error: the value should be 0 or 1\n");
			return -1;
		}
	} else if (command[0] == set) {
		for (i = 1; i <= 2; i++) {
			/* check <X,Y> */
			if ((command[i] > boardRowAndColSize) || (command[i] < 1)) {
				printf("Error: value not in range 0-%d\n", boardRowAndColSize);
				return -1;
			}
		}
		if ((command[3] > boardRowAndColSize) || (command[3] < 0)) {
			/* check <Z> */
			printf("Error: value not in range 0-%d\n", boardRowAndColSize);
			return -1;
		}
	} else if (command[0] == hint) {
		for (i = 1; i <= 2; i++) {
			/* check <X,Y> */
			if ((command[i] > boardRowAndColSize) || (command[i] < 1)) {
				printf("Error: value not in range 1-%d\n", boardRowAndColSize);
				return -1;
			}
		}
	}
	return 0;
}

/*
 * -------------------------------
 * validInput Documentation is in header file
 * -------------------------------
 */
int validInput(int* command, int numOfArgs, int notDigitFlage) {
	int notDigitErrorPrint(int notDigitFlag);
	int gameMode = getGameMode();
	if (command[0] == error) {
		return -1;
	}
	if(gameMode == INIT_MODE){
		if(command[0] != edit && command[0] != solve && command[0] != exit1){
			printf("%s", "ERROR: invalid command\n");
			return -1;
		}
	}
	if (command[0] == solve && numOfArgs < 1) {
		printf("%s", "ERROR: invalid command\n");
		return -1;
	}
	if (command[0] == set && numOfArgs < 3) {
		printf("%s", "ERROR: invalid command\n");
		return -1;
	}
	if (command[0] == generate && numOfArgs < 2) {
		printf("%s", "ERROR: invalid command\n");
		return -1;
	}
	if (command[0] == mark_errors && numOfArgs < 1) {
		printf("%s", "ERROR: invalid command\n");
		return -1;
	}
	if (command[0] == save && numOfArgs < 1) {
		printf("%s", "ERROR: invalid command\n");
		return -1;
	}
	if (command[0] == hint && numOfArgs < 2) {
		printf("%s", "ERROR: invalid command\n");
		return -1;
	}
	if (notDigitFlage != 0) {
		if (notDigitErrorPrint(notDigitFlage) == -1) {
			return -1;
		}
	}
	if (command[0] == mark_errors || command[0] == set || command[0] == hint) {
		/* call to validRange  if range check is needed */
		return validRange(command);
	}
	return 0;
}

/*
 * Function: isDigit
 * --------------------
 * check if the input represent a digit
 *
 * token: the input we want check
 */
int isDigit(char* token) {
	/* check if the char array contains only digits */
	int i = 0;
	char c;
	while (token[i] != 0) {
		if ((i == 0 && token[i] == '-') && token[1] != 0) {
			/* negative num case */
			i++;
			continue;
		}
		c = token[i];
		if (c > 47 && c < 58) {
			i++;
		} else {
			return -1;
		}
	}
	return 1;
}

/*
 * Function: notDigitError
 * --------------------
 * if one of the arguments which given by the user is not a number
 * (and is should be) change notDigitFlage to it's error code so we know which error massage
 * to print.
 *
 * command: array holding types of commands
 * notDigitFlage: a pointer to notDigitFlage.
 */
void notDigitError(int* command, int* notDigitFlag) {
	if (*notDigitFlag != 0) {
		return;
	}
	if (command[0] == mark_errors) {
		*notDigitFlag = 1;
	} else if (command[0] == set) {
		*notDigitFlag = 2;
	} else if (command[0] == generate) {
		*notDigitFlag = 3;
	} else if (command[0] == hint) {
		*notDigitFlag = 4;
	}
}

/*
 * Function: notDigitErrorPrint
 * --------------------
 * print the appropriate error massage according to notDigitFlag.
 *
 * notDigitFlage: tell us which error message to print.
 */
int notDigitErrorPrint(int notDigitFlag) {
	boardData brdData = getBoardData();
	int boardRowAndColSize = brdData.blockRowSize * brdData.blockColSize;
	switch (notDigitFlag) {
	case 1:
		/* mark_errors */
		printf("%s", "Error: the value should be 0 or 1\n");
		return -1;
	case 2:
		/* set */
		printf("Error: value not in range 0-%d\n", boardRowAndColSize);
		return -1;
	case 3:
		/* generate */
		printf("Error: value not in range 0-%d\n",
				(boardRowAndColSize * boardRowAndColSize));
		return -1;
	case 4:
		/* hint */
		printf("Error: value not in range 1-%d\n", boardRowAndColSize);
		return -1;
	}
	return 0;
}

