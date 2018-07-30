/*
 * Stack.h
 *
 *  Created on: 30 Jul 2018
 *      Author: Raz
 */

#ifndef STACK_H_
#define STACK_H_

struct cellIndex{/* a struct representing the cell's row and column indexes */
	int row;
	int col;
};

typedef struct cellIndex cellIndex;

struct StackNode{/* definition of the stack's nodes */
	cellIndex index;
    struct StackNode* next;
};

typedef struct StackNode node;

node* newNode(int row, int col);
int isEmpty(node* root);
void push(node** root, int row, int col);
cellIndex pop(node** root);
cellIndex peek(node* root);


#endif /* STACK_H_ */
