/*
 * CommandsList.c
 *
 *  Created on: 2 баев„ 2018
 *      Author: beniz
 */

#include <stdlib.h>
#include <stdio.h>
#include "CommandsList.h"
#include "MainAux.h"
#include "Cell.h"

/*
 * Function: addCommand
 * --------------------
 * create a new node, update the node accordingly to the game, and add it to the commandsList
 *
 * list: the command list
 * info: the board that we want to save in the node
 * errorsFlag: the errorFlage value that we want to save in the node.
 */
void addCommand(dll* list, Cell** info, int errorsFlag){
	/* allocate memory to the new node and put info */
	dllNode* newCommand = NULL;
	Cell** boardCopy = NULL;
	boardData brdData = getBoardData();
	newCommand = (dllNode*) malloc(sizeof(dllNode));
	boardCopy = setAllocatedMem();
	copyBoard(boardCopy, info);
	newCommand->info = boardCopy;
	newCommand->boardContainError = errorsFlag;
	newCommand->infoColSize = brdData.blockColSize;
	newCommand->infoRowSize = brdData.blockRowSize;
	newCommand->next = NULL;

	if(list->head == NULL){
		/* in case the newCommand is the first command */
		newCommand->previous = NULL;
		list->head = newCommand;
		list->currentNode = newCommand;
		list->lastNode = newCommand;
		return;
	}
	newCommand->previous = list->lastNode;
	(list->lastNode)->next = newCommand;
	list->currentNode = newCommand;
	list->lastNode = newCommand;
}

/*
 * Function: deleteFromCurrent
 * --------------------
 * delete all the nodes from the current node (exclude)
 *
 * list: the command list
 */
void deleteFromCurrent(dll* list){
	while((list->lastNode) != list->currentNode){
		freeBoardMem((list->lastNode)->info, (list->lastNode)->infoRowSize, (list->lastNode)->infoColSize);
		(list->lastNode) = (list->lastNode)-> previous;
		free((list->lastNode)->next);
		(list->lastNode)->next = NULL;
	}
}

/*
 * Function: allocateListMem
 * --------------------
 * allocate memory to the list
 */
dll* allocateListMem(){
	/* allocate memory for the dll struct */
	dll *lst = NULL;
	lst = (dll *) malloc(sizeof(dll));
	return lst;
}

/*
 * Function: initList
 * --------------------
 * initialize new list with the first node in it.
 *
 * list: the command list
 * info: the board that we want to save in the node
 * errorsFlag: the errorFlage value that we want to save in the node.
 */
void initList(dll* list, Cell** info, int errorsFlag){
	dllNode* firstNode = NULL;
	Cell** boardCopy = NULL;
	boardData brdData = getBoardData();
	firstNode = (dllNode*) malloc(sizeof(dllNode));
	boardCopy = setAllocatedMem();
	copyBoard(boardCopy, info);
	firstNode->info = boardCopy;
	firstNode->boardContainError = errorsFlag;
	firstNode->infoColSize = brdData.blockColSize;
	firstNode->infoRowSize = brdData.blockRowSize;
	firstNode->next = NULL;
	firstNode->previous = NULL;
	list->head = firstNode;
	list->currentNode = firstNode;
	list->lastNode = firstNode;
}


/*
 * Function: deleteListNodes
 * --------------------
 * delete all the nodes are currently in the list.
 * after that head, lastNode and currentNode are point to NULL
 * the list it's self is NOT deleted
 *
 * list: the command list we want to delete
 */
void deleteListNodes(dll* list){
	if(list == NULL){
		return;
	}
	if((list->head) == NULL){
		/* there is nothing to delete */
		(list->currentNode) = NULL;
		(list->lastNode) = NULL;
		return;
	}
	while((list->lastNode) != (list->head)){
		/* delete all the nodes except the head */
		freeBoardMem((list->lastNode)->info, (list->lastNode)->infoRowSize, (list->lastNode)->infoColSize);
		(list->lastNode) = (list->lastNode)-> previous;
		free((list->lastNode)->next);
		(list->lastNode)->next = NULL;
	}
	/* delete the head */
	freeBoardMem((list->head)->info, (list->head)->infoRowSize, (list->head)->infoColSize);
	if(list->head != NULL){
		free(list->head);
	}
	(list->head) = NULL;
	(list->currentNode) = NULL;
	(list->lastNode) = NULL;
}

/*
 * Function: freeListMem
 * --------------------
 * free the memory of the given list
 */
void freeListMem(dll* list){
	/* free ONLY the list memory and NOT it's nodes
	 * use only AFTER deleteListNodes to avoid memory leaks */
	if(list != NULL){
		free(list);
	}
}

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
void boardDiff(dll* list, dllNode* otherNode,char *command){
	/* command is string: Redo/Undo */
	int i, j;
	Cell **currentBoard, **otherBoard;
	boardData brdData = getBoardData();
	int boardRowAndColSize = brdData.blockColSize * brdData.blockRowSize;
	currentBoard = (list->currentNode)->info;
	otherBoard = otherNode->info;
	for(i = 0; i < boardRowAndColSize; i++){
		for(j = 0; j < boardRowAndColSize; j++){
			if(currentBoard[i][j].currentNum != otherBoard[i][j].currentNum){
				if(currentBoard[i][j].currentNum != 0 && otherBoard[i][j].currentNum != 0){
					printf("%s %d,%d: from %d to %d\n", command, (j+1), (i+1), currentBoard[i][j].currentNum, otherBoard[i][j].currentNum);
				}else{
					if(currentBoard[i][j].currentNum == 0){
						printf("%s %d,%d: from %c to %d\n", command, (j+1), (i+1), '_', otherBoard[i][j].currentNum);
					}else{
						printf("%s %d,%d: from %d to %c\n", command, (j+1), (i+1), currentBoard[i][j].currentNum, '_');
					}
				}
			}
		}
	}
}
