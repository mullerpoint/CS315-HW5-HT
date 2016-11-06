///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Gary Muller
// Fall 2016
// CS 315 Assignment 5
//
///////////////////////////////////////////////////////////////////////////////////////////////////
// A program that creates and utilizes a Huffman tree, it will utilize a
// struct type def, and includes an add functionality; it will utilize a
// stack to allow for traversals
//
///////////////////////////////////////////////////////////////////////////////////////////////////


//Header files standard libraries and classes
#include <stdio.h> //standard io functions for C
#include <stdlib.h> //used for better malloc() function
#include <ctype.h> //used for tolower() function
#include <stdbool.h> //used for bool types
#include <string.h> //string functions


//Gloabal Variables and Defines
//Structure Type Definition for a Huffman Tree structure
typedef struct Huffman_Tree_Item
{
	//character
	char keyValue;
	//character frequency
	int frequency;
	//pointer to child nodes
	struct Huffman_Tree_Item *lNode, *rNode; //left and right subtree/items
}HT_ITEM;

//Structure Type Definition for a stack structure
typedef struct Stack_Item
{
	//key
	HT_ITEM *data;
	//pointer to child trees
	struct Stack_Item *next;
}STACK_ITEM;

//#define for the maximum size of the little end up heap
#define MAX_HEAP 10

//structure type definition for a heap
typedef struct Heap
{
	HT_ITEM* HT_Ptrs[MAX_HEAP];
}HEAP;

//Function Prototypes
int newHTNode(char, int, HEAP*); //creates a HT Node and inserts it into the heap
int insertToHeap(HT_ITEM*, HEAP*); //insert a HT Node into the heap
int buildHT(/*heap*/); //builds the HT from the nodes stored in the heap
int combineNodes(HT_ITEM**, HT_ITEM**); //build trees from the nodes provided
int preorderTrav(HT_ITEM*); //preorder traverse the HT
int inorderTrav(HT_ITEM*); //inorder traverse the HT
//int tabulateHT(HT_ITEM*); //build the table for HT Code

//Stack Functions
int push(STACK_ITEM**, HT_ITEM*); //push HT node onto Stack
HT_ITEM* pop(STACK_ITEM**); //pop HT node from stack
bool isEmpty(STACK_ITEM*); //check if the stack is empty

//

//main driver function
main()
{
	//Variable Declarations
	char entered = ' ';
	int frequency = -1;
	int count = 0;

	//Huffman Tree Declaration
	HT_ITEM *ptrToHT_A = NULL;

	//Heap declaration
	HEAP heap_A;

	//loop for program body
	while ((entered != '$') && (count <= MAX_HEAP))
	{
		//query user for input on next input and normalize input
		printf("Enter a character (enter a '$' to quit entering characters): ");
		scanf(" %c", &entered);

		//check if the input was a '$'
		if (entered != '$')
		{
			// ensure there are no negative frequencies
			do
			{
				printf("\tEnter '%d's frequency: ", entered);
				scanf(" %d", &frequency);
			} while (frequency < 0); 
			
			//build the HT node with the character and frequency provided
			newHTNode(entered, frequency, &heap_A);


			//reset inputs and increase count
			entered = ' ';
			frequency = -1;
			count++;
		}//if

	}//while

	//build the HT
	buildHT();

	//Print out the traversals to the user
	inorderTrav(ptrToHT_A);
	preorderTrav(ptrToHT_A);

	////tabulate the HT
	//tabulateHT(ptrToHT_A);

}//main


///////////////////////////////////////////////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////////////////////////////////////////////

//creates a HT Node and inserts it into the heap
int newHTNode(char input, int freq, HEAP* heapToUse)
{
	//build the new node
	//get memory
	HT_ITEM *newNode;
	newNode = malloc(sizeof(HT_ITEM));

	//set values
	newNode->frequency = freq;
	newNode->keyValue = input;
	newNode->lNode = NULL;
	newNode->rNode = NULL;
	

	//put newNode into the heap
	insertToHeap(newNode, heapToUse);
	
	//return Success
	return 1;
}

//insert a HT Node into the heap
int insertToHeap(HT_ITEM* inputnode, HEAP* heapToUse)
{
	//STUB
	printf("STUB");
	return 1;
}

//builds the HT from the nodes stored in the heap
int buildHT(/*heap*/)
{
	//STUB
	printf("STUB");
	return 1;
}

//build trees from the nodes provided
int combineNodes(HT_ITEM** rootNode, HT_ITEM** lNode) 
{
	//STUB
	printf("STUB");
	return 1;
}

//preorder traverse the HT
int preorderTrav(HT_ITEM* root)
{
	//variables
	HT_ITEM *currentRoot = root;
	STACK_ITEM *stackPtr = NULL;

	//check that the tree is not empty
	if (root == NULL)
	{
		//return error - theres no tree to traverse
		return -1;
	}

	//traversal loop
	while (true)
	{
		//move left and push to stack
		while (currentRoot != NULL)
		{
			//print root
			printf("%c ", currentRoot->keyValue);

			//push root to stack
			push(&stackPtr, currentRoot);

			//go left
			currentRoot = currentRoot->lNode;
		}//while

		//if root it null and stack is empty we've finished
		if ((currentRoot == NULL) && (isEmpty(stackPtr)/*stackPtr == NULL*/))
		{
			//return success
			return 1;
		}//if

		//pop the last node and move right
		currentRoot = pop(&stackPtr);
		currentRoot = currentRoot->rNode;


	}//while


	//return success
	return 1;

}

//inorder traverse the HT
int inorderTrav(HT_ITEM* root)
{
	//variables
	bool done = false;
	HT_ITEM *currentRoot = root;
	STACK_ITEM *stackPtr = NULL;

	//traversal loop
	while (!done)
	{
		//traverse left
		if (currentRoot != NULL)
		{
			//save root for right traverse later
			push(&stackPtr, currentRoot);

			//traverse left
			currentRoot = currentRoot->lNode;
		}
		//traverse right
		else
		{
			//check that the stack is not empty
			if (!isEmpty(stackPtr)/*stackPtr != NULL*/)
			{
				//backtrack
				currentRoot = pop(&stackPtr);
				//visit root
				printf("%c ", currentRoot->keyValue);

				//begin right traversal
				currentRoot = currentRoot->rNode;
			}
			//end the loop
			else
			{
				done = true;
			}//else
		}//else
	}//while

	//return success
	return 1;

}

////build the table for HT Code
//int tabulateHT(HT_ITEM* root) 
//{
//	//STUB
//	printf("STUB");
//	return 1;
//}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Stack Functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//push BST node to stack
int push(STACK_ITEM** stackTop, HT_ITEM* node)
{
	//get new stack item
	STACK_ITEM *tempStackItem;
	tempStackItem = malloc(sizeof(STACK_ITEM));

	//set values
	tempStackItem->data = node;
	tempStackItem->next = *stackTop;

	//set the top to the new item
	*stackTop = tempStackItem;

	//return success
	return 1;
}//push

//pop BST node from stack
HT_ITEM* pop(STACK_ITEM** stackTop)
{
	if (*stackTop != NULL)//as long as the stack is not empty
	{
		STACK_ITEM *tempItem = *stackTop;
		*stackTop = (*stackTop)->next;

		return tempItem->data; //return the bst item not the stack item
	}//if
	else
	{
		return NULL;
	}

}//pop

//check if the stack is empty
bool isEmpty(STACK_ITEM* stackPtr)
{
	if (stackPtr == NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}
