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
 * -------------------------------
 * newNode Documentation is in header file
 * -------------------------------
 */
node* newNode(int row, int col){
	node* stackNode = (node*) malloc(sizeof(node));

    stackNode->index.row = row;
    stackNode->index.col = col;
    stackNode->next = NULL;

    return stackNode;
}

/*
 * -------------------------------
 * isEmpty Documentation is in header file
 * -------------------------------
 */
int isEmpty(node* root){
    return !root;
}

/*
 * -------------------------------
 * push Documentation is in header file
 * -------------------------------
 */
void push(node** root, int row, int col){
	node* stackNode = newNode(row, col);

    stackNode->next = *root;
    *root = stackNode;
}

/*
 * -------------------------------
 * pop Documentation is in header file
 * -------------------------------
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
 * -------------------------------
 * peek Documentation is in header file
 * -------------------------------
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
