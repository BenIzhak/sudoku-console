/*
 * Stack.c
 *
 *  Created on: 30 Jul 2018
 *      Author: Raz
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "Stack.h"

/*
 * Function:  newNode
 * --------------------
 *  creates a new node, initializing it's data and connected node
 *
 *  col: the current cell column
 *  row: the current cell row
 */

node* newNode(int row, int col){
	node* stackNode = (node*) malloc(sizeof(node));

    stackNode->index.row = row;
    stackNode->index.col = col;
    stackNode->next = NULL;

    return stackNode;
}

/*
 * Function:  isEmpty
 * --------------------
 * checks if the stack is empty returns 1 if empty, otherwise returns 0
 *
 *  root: the top most node
 */

int isEmpty(node* root){
    return !root;
}

/*
 * Function:  push
 * --------------------
 * creates a new node and sets it's data and next as root
 * then makes it the new root
 *
 *  root: the top most node
 *  col: the current cell column
 *  row: the current cell row
 */
void push(node** root, int row, int col){
	node* stackNode = newNode(row, col);

    stackNode->next = *root;
    *root = stackNode;
}

/*
 * Function:  pop
 * --------------------
 *	removes the topmost node and returns it's value
 *
 *  root: the top most node
 */
cellIndex pop(node** root){
	node* temp;
	cellIndex popped;

    if (isEmpty(*root)){
    	popped.col = -1;
    	popped.row = -1;
        return popped;
    }
    temp = *root;

    *root = (*root)->next;
    popped = temp->index;

    free(temp);

    return popped;
}

/*
 * Function:  peek
 * --------------------
 *	returns the top most node data
 *
 *  root: the top most node
 */
cellIndex peek(node* root){
	cellIndex popped;
    if (isEmpty(root)){
    	popped.col = -1;
    	popped.row = -1;
        return popped;
    }
    return root->index;
}
