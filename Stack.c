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
 *  allocating memory for node and setting the node index object and next fields
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
 * checks if the stack is empty by checking if the root node exists
 *
 * returns 1 if empty, otherwise returns 0
 *
 * root: the top most node
 */
int isEmpty(node* root){
    return !root;
}

/*
 * Function:  push
 * --------------------
 *  creates a new node and by calling the newNode function
 *  then setting it's next field and setting it as the new root
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
 *	checking if the stack is empty, if it does return -1 for both indexes
 *
 *	else return the value of root and free it's memory
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
 *	checking if the stack is empty, if it isn't
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
