/*
 * CommandsList.c
 *
 *  Created on: 2 баев„ 2018
 *      Author: beniz
 */

#include <stdlib.h>
#include "CommandsList.h"
#include "MainAux.h"


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
		(*lastNode) = (*lastNode)-> previous;
		/*TODO: free info memory*/
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
		(*lastNode) = (*lastNode)-> previous;
		free((*lastNode)->next);
		(*lastNode)->next = NULL;
	}
	/* delete the head */
	free(*head);
	(*head) = NULL;
	(*lastNode) = NULL;
	(*currentNode) = NULL;
}
