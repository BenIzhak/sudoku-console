/*
 * Parser.c
 *
 *  Created on: 29 αιεμι 2018
 *      Author: beniz
 */

#include <stdio.h>
#include <string.h>

/* TODO:
 * 1. check if the line "Enter your command:" is printed even after blank line */


enum COMMAND{
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

int parseCommand(char* input, int* command, char* filePath){
	int isDigit(char* token);
	const char delim[7] = " \t\r\n";
	char *token;
	int i;
	filePath = NULL;

	if(strlen(input) > 256){
		command[0] = error;
		printf("%s", "ERROR: invalid command\n");
		return -1;
	}
	token = strtok(input,delim);
	for(i = 0; i < 4; i++){
		command[i] = -1;
	}
	if(token == NULL){
		command[0] = error;
		return -1;
	}
	/* Get the command type */
	if(strcmp(token,"solve") == 0){

		command[0] = solve;

	}else if(strcmp(token,"edit") == 0){

		command[0] = edit;
	}else if(strcmp(token,"mark_errors") == 0){
		command[0] = mark_errors;
	}else if(strcmp(token,"print_board") == 0){
		command[0] = print_board;
	}else if(strcmp(token,"set") == 0){
		command[0] = set;
	}else if(strcmp(token,"validate") == 0){
		command[0] = validate;
	}else if(strcmp(token,"generate") == 0){
		command[0] = generate;
	}else if(strcmp(token,"undo") == 0){
		command[0] = undo;
	}else if(strcmp(token,"redo") == 0){
		command[0] = redo;
	}else if(strcmp(token,"save") == 0){
		command[0] = save;
	}else if(strcmp(token,"hint") == 0){
		command[0] = hint;
	}else if(strcmp(token,"num_solutions") == 0){
		command[0] = num_solutions;
	}else if(strcmp(token,"autofill") == 0){
		command[0] = autofill;
	}else if(strcmp(token,"reset") == 0){
		command[0] = reset;
	}else if(strcmp(token,"exit") == 0){
		command[0] = exit1;
	}else{
		printf("%s", "ERROR: invalid command\n");
		command[0] = error;
	}
	token = strtok(NULL,delim);
	if(token!= NULL && (command[0] == solve || command[0] == edit || command[0] == save)){
		sscanf(token, "%s", filePath);
		command[1] = 0;
	}else{
		if(isDigit(token)){
			sscanf(token, "%d", &command[1]);
		}
	}
	token = strtok(NULL,delim);
	if(token!= NULL){
		if(isDigit(token)){
			sscanf(token, "%d", &command[2]);
	}
	}
	token = strtok(NULL,delim);
	if(token!= NULL){
		if(isDigit(token)){
			sscanf(token, "%d", &command[3]);
	}
	}
	return 0;
}


int validInput(int* command){
	/* check if the necessary parameters are supplied */
	if(command[0] == error){
		return -1;
	}
	if(command[0] == solve && (command[1] == -1)){
		printf("%s", "ERROR: invalid command\n");
		return -1;
	}if(command[0] == mark_errors && (command[1] == -1)){
		printf("%s", "ERROR: invalid command\n");
		return -1;
	}if(command[0] == set && (command[1] == -1 || command[2] == -1 || command[3] == -1)){
		printf("%s", "ERROR: invalid command\n");
		return -1;
	}if(command[0] == generate && (command[1] == -1 || command[2] == -1)){
		printf("%s", "ERROR: invalid command\n");
		return -1;
	}if(command[0] == mark_errors && (command[1] == -1 || command[2] == -1)){
		printf("%s", "ERROR: invalid command\n");
		return -1;
	}if(command[0] == save && (command[1] == -1)){
		printf("%s", "ERROR: invalid command\n");
		return -1;
	}if(command[0] == hint && (command[1] == -1 || command[2] == -1)){
		printf("%s", "ERROR: invalid command\n");
		return -1;
	}
	return 0;
}

int isDigit(char* token){
	/* check if the char array contains only digits */
	int i = 0;
	char c;
	while(token[i] != 0){
		c = token[i];
		if(c > 47 && c < 58){
			i++;
		}else{
			return -1;
		}
	}
	return 1;
}





