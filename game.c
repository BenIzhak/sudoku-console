/*
 * game.c
 *
 *  Created on: 31 Jul 2018
 *      Author: Raz
 */

#include "CommandsList.h"
#include "mainAux.h"
#include <stdio.h>

dllNode** head;
dllNode** lastNode;
dllNode** currentNode;

void reset(){
	/* undo all moves, reverting the board to its original loaded state. */
	(*currentNode) = (*head);
	deleteFromCurrent(lastNode,currentNode);
	copyBoard(userBoard, (*currentNode)->info);
	printf("Board reset\n");
}

void hardReset(Cell** info){
	/* delete the whole command list and initialize a new one */
	deleteList(head, lastNode, currentNode);
	initList(head, lastNode, currentNode, info);
}

void undo(){
	dllNode *prevCommnad;
	if(currentNode == head){
		printf("%s","Error: no moves to undo\n");
		return;
	}
	boardDiff(currentNode, &((*currentNode)->previous),"Undo");
	prevCommnad = (*currentNode)->previous;
	currentNode = &prevCommnad;
	copyBoard(userBoard, (*currentNode)->info);
}

void redo(){
	dllNode *nextCommand;
	if(currentNode == lastNode){
		printf("%s","Error: no moves to redo\n");
		return;
	}
	boardDiff(currentNode, &((*currentNode)->next) ,"Redo");
	nextCommand = (*currentNode)->next;
	currentNode = &nextCommand;
	copyBoard(userBoard, (*currentNode)->info);
}
