/*
 * game.c
 *
 *  Created on: 31 Jul 2018
 *      Author: Raz
 */

#include "CommandsList.h"
#include <stdio.h>

dllNode** head;
dllNode** lastNode;
dllNode** currentNode;

void reset(){
	/* TODO: load the new board */
	(*currentNode) = (*head);
	deleteFromCurrent(lastNode,currentNode);
	printf("Board reset\n");
}

void hardReset(Cell** info){
	deleteList(head, lastNode, currentNode);
	initList(head, lastNode, currentNode, info);
}
