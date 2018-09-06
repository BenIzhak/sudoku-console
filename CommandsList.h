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
	int boardContainError;
	int infoColSize;
	int infoRowSize;
	struct dllNode* next;
    struct dllNode* previous;
}dllNode;

typedef struct dll{/* definition of the Doubly Linked List */
	struct dllNode* head;
    struct dllNode* currentNode;
    struct dllNode* lastNode;
}dll;

/*
 * Function: addCommand
 * --------------------
 * create a new node, update the node accordingly to the game, and add it to the commandsList
 *
 * list: the command list
 * info: the board that we want to save in the node
 * errorsFlag: the errorFlage value that we want to save in the node.
 */
void addCommand(dll* list, Cell** info, int errorsFlag);

/*
 * Function: deleteFromCurrent
 * --------------------
 * delete all the nodes from the current node (exclude)
 *
 * list: the command list
 */
void deleteFromCurrent(dll* list);

/*
 * Function: allocateListMem
 * --------------------
 * allocate memory to the list
 */
dll* allocateListMem();

/*
 * Function: initList
 * --------------------
 * initialize new list with the first node in it.
 *
 * list: the command list
 * info: the board that we want to save in the node
 * errorsFlag: the errorFlage value that we want to save in the node.
 */
void initList(dll* list, Cell** info, int errorsFlag);

/*
 * Function: deleteListNodes
 * --------------------
 * delete all the nodes are currently in the list.
 * after that head, lastNode and currentNode are point to NULL
 * the list it's self is NOT deleted
 *
 * list: the command list we want to delete
 */
void deleteListNodes(dll* list);

/*
 * Function: boardDiff
 * --------------------
 * print the changed cells between two boards
 *
 * list: the commandsList
 * otheNode: the node that contains one of the boards board
 * the another one is current board.
 * command: is string Redo/Undo
 */
void boardDiff(dll* list, dllNode* otherNode,char *command);

/*
 * Function: freeListMem
 * --------------------
 * free the memory of the given list
 */
void freeListMem(dll* list);


#endif /* COMMANDSLIST_H_ */
