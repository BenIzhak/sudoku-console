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

extern int blockRowSize;
extern int blockColSize;

void addCommand(dllNode** head, dllNode** lastNode, dllNode** currentNode, Cell** info){
	/* head is a pointer to the start of the list */
	dllNode* newCommand = NULL;

	/* allocate memory and put info */
	newCommand = (dllNode*) malloc(sizeof(dllNode));
	newCommand->info = info;
	newCommand->next = NULL;

	if(*head == NULL){
		/* in case the newCommand is the first command */
		newCommand->previous = NULL;
		*head = newCommand;
		*lastNode = newCommand;
		*currentNode = newCommand;
		return;
	}

	(*lastNode)->next = newCommand;
	newCommand->previous = *lastNode;
	lastNode = &newCommand;
}

void deleteFromCurrent(dllNode** lastNode, dllNode** currentNode){
	/* delete all the nodes from the current node (exclude) */
	while(*lastNode != *currentNode){
		free((*lastNode)->info);
		(*lastNode) = (*lastNode)-> previous;
		free((*lastNode)->next);
		(*lastNode)->next = NULL;
	}
}

void initList(dllNode** head, dllNode** lastNode, dllNode** currentNode, Cell** info){
	/* create new list (new node) */
	dllNode* firstNode = NULL;
	firstNode = (dllNode*) malloc(sizeof(dllNode));
	firstNode->info = info;
	(*head) = firstNode;
	(*lastNode) = firstNode;
	(*currentNode) = firstNode;
}

void deleteList(dllNode** head, dllNode** lastNode, dllNode** currentNode){
	/* delete all the nodes are currently in the list.
	 * then head, lastNode and currentNode are point to NULL */
	if((*head) == NULL){
		/* there is nothing to delete */
		(*lastNode) = NULL;
		(*currentNode) = NULL;
		return;
	}
	while(*lastNode != *head){
		/* delete all the nodes except the head */
		free((*lastNode)->info);
		(*lastNode) = (*lastNode)-> previous;
		free((*lastNode)->next);
		(*lastNode)->next = NULL;
	}
	/* delete the head */
	free((*head)->info);
	free(*head);
	(*head) = NULL;
	(*lastNode) = NULL;
	(*currentNode) = NULL;
}

void boardDiff(dllNode** currentNode, dllNode** otherNode,char *command){
	/* command is string: Redo/Undo */
	int i, j;
	Cell **currentBoard, **otherBoard;
	currentBoard = (*currentNode)->info;
	otherBoard = (*otherNode)->info;
	for(i = 0; i < blockRowSize; i++){
		for(j = 0; j < blockColSize; j++){
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
