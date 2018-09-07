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

/*
 * Function:  newNode
 * --------------------
 *  creates a new node, initializing it's data and connected node
 *
 *  col: the current cell column
 *  row: the current cell row
 */
node* newNode(int row, int col);

/*
 * Function:  isEmpty
 * --------------------
 * checks if the stack is empty returns 1 if empty, otherwise returns 0
 *
 *  root: the top most node
 */
int isEmpty(node* root);

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
void push(node** root, int row, int col);

/*
 * Function:  pop
 * --------------------
 *	removes the topmost node and returns it's value
 *
 *  root: the top most node
 */
cellIndex pop(node** root);

/*
 * Function:  peek
 * --------------------
 *	returns the top most node data
 *
 *  root: the top most node
 */
cellIndex peek(node* root);

void emptyStack(node* root);

#endif /* STACK_H_ */
