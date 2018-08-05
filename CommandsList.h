/*
 * CommandsList.h
 *
 *  Created on: 2 баев„ 2018
 *      Author: beniz
 */

#ifndef COMMANDSLIST_H_
#define COMMANDSLIST_H_

#include "Cell.h"

typedef struct dllNode{/* definition of the stack's nodes */
	Cell** info;
	struct dllNode* next;
    struct dllNode* previous;
}dllNode;

void addCommand(dllNode** head, dllNode** lastNode, dllNode** currentNode, Cell** info);

void deleteFromCurrent(dllNode** lastNode, dllNode** currentNode);

void initList(dllNode** head, dllNode** lastNode, dllNode** currentNode, Cell** info);

void deleteList(dllNode** head, dllNode** lastNode, dllNode** currentNode);

void boardDiff(dllNode** currentNode, dllNode** otherNode,char *command);




#endif /* COMMANDSLIST_H_ */
