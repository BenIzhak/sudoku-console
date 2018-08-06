/*
 * CommandsList.h
 *
 *  Created on: 2 баев„ 2018
 *      Author: beniz
 */

#ifndef COMMANDSLIST_H_
#define COMMANDSLIST_H_

#include "Cell.h"

typedef struct dllNode{
	Cell** info;
	struct dllNode* next;
    struct dllNode* previous;
}dllNode;

typedef struct dll{/* definition of the Doubly Linked List */
	struct dllNode* head;
    struct dllNode* currentNode;
    struct dllNode* lastNode;
}dll;

void addCommand(dll* list, Cell** info);

void deleteFromCurrent(dll* list);

dll* allocateListMem();

void initList(dll* list, Cell** info);

void deleteListNodes(dll* list);

void boardDiff(dll* list, dllNode* otherNode,char *command);

void freeListMem(dll* list);


#endif /* COMMANDSLIST_H_ */
