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
	int inserted;
	HT_ITEM* HT_Ptrs[MAX_HEAP];
}HEAP;

//Function Prototypes
//Huffman Tree Functions
int newHTNode(char, int, HEAP*); //creates a HT Node and inserts it into the heap
HT_ITEM* buildHT(HEAP*); //builds the HT from the nodes stored in the heap
HT_ITEM* combineNodes(HT_ITEM*, HT_ITEM*); //build trees from the nodes provided
int preorderTrav(HT_ITEM*); //preorder traverse the HT
int inorderTrav(HT_ITEM*); //inorder traverse the HT
//int tabulateHT(HT_ITEM*); //build the table for HT Code

//Heap Functions
int heapInsert(HT_ITEM*, HEAP*); //insert a HT Node into the heap
int heapBubbleUp(HEAP*);
HT_ITEM* heapExtractMin(HEAP*);
int heapSinkDown(HEAP*, int);
int heapSwap(int, int, HEAP*);

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
	HEAP* heap_A;
	heap_A = malloc(sizeof(HEAP));
	
	//use the 0th item for a count of the number of inserted items
	heap_A->inserted = 0;

	//loop for program body
	while ((entered != '$') && (count <= MAX_HEAP))
	{
		//query user for input on next input
		printf("Enter a character (enter a '$' to quit entering characters): ");
		scanf(" %c", &entered);

		//check if the input was a '$'
		if (entered != '$')
		{
			// ensure there are no negative frequencies
			do
			{
				printf("\tEnter '%c's frequency: ", entered);
				scanf(" %d", &frequency);
			} while (frequency < 0); 
			
			//build the HT node with the character and frequency provided
			newHTNode(entered, frequency, heap_A);


			//reset inputs and increase count
			entered = ' ';
			frequency = -1;
			count++;
		}//if

	}//while

	//build the HT
	ptrToHT_A = buildHT(heap_A);

	//Print out the traversals to the user
	
	printf("\n\nThe nodes of the Huffman tree in In-order are:\n\n");
	inorderTrav(ptrToHT_A);
	printf("\n\nand in Pre-order are:\n\n");
	preorderTrav(ptrToHT_A);

	////tabulate the HT
	//tabulateHT(ptrToHT_A);

}//main


///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Functions
//
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// Huffman Tree Functions
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
	heapInsert(newNode, heapToUse);
	
	//return Success
	return 1;
}

//builds the HT from the nodes stored in the heap
HT_ITEM* buildHT(HEAP* heapToUse)
{
	//while there is more than 1 HT node in the heap keep combining them
	while (heapToUse->inserted != 1)
	{
		//get the two smallest nodes in the heap
		HT_ITEM *tempNode_A, *tempNode_B, *newNode;
		tempNode_A = heapExtractMin(heapToUse);
		tempNode_B = heapExtractMin(heapToUse);

		//combine the two smallest nodes
		newNode = combineNodes(tempNode_A, tempNode_B);

		//insert the new node into the heap again
		heapInsert(newNode, heapToUse);
	}
	
	//return the final HT
	return heapExtractMin(heapToUse);

}

//build trees from the nodes provided
HT_ITEM* combineNodes(HT_ITEM* rItem, HT_ITEM* lItem)
{
	//create a new HT
	HT_ITEM * newHTNode;
	newHTNode = malloc(sizeof(HT_ITEM));

	//build the node
	newHTNode->frequency = (rItem->frequency) + (lItem->frequency);
	newHTNode->keyValue = '-';
	newHTNode->lNode = lItem;
	newHTNode->rNode = rItem;

	//return the newly create node

	return newHTNode;
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
			printf("%c\t", currentRoot->keyValue);
			printf("%d\n", currentRoot->frequency);

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
				printf("%c\t", currentRoot->keyValue);
				printf("%d\n", currentRoot->frequency);

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


///////////////////////////////////////////////////////////////////////////////////////////////////
// Heap Functions
///////////////////////////////////////////////////////////////////////////////////////////////////

//insert a HT Node into the heap
int heapInsert(HT_ITEM* inputnode, HEAP* heapToUse)
{
	//store the number of inserted items in a count
	int count = heapToUse->inserted;


	//if the heap is empty
	if (count == 0)
	{
		//add new node
		heapToUse->HT_Ptrs[count + 1] = inputnode;
		//increase count
		heapToUse->inserted = (count + 1);
	}
	else //the heap is not empty
	{
		//add new node
		heapToUse->HT_Ptrs[count + 1] = inputnode;
		//increase count
		heapToUse->inserted = (count + 1);
		//sort nodes
		heapBubbleUp(heapToUse);
	}

	//increase count
	heapToUse->inserted = (count+1);
	
	//return success
	return 1;
}


//perform a heap bubble up for the last item inserted
int heapBubbleUp(HEAP* heapToUse)
{
	int curPos = heapToUse->inserted;

	//while the current position is not 0 and the parent is greater than the current node
	while ((curPos > 1) && (((heapToUse->HT_Ptrs[curPos / 2])->frequency)>((heapToUse->HT_Ptrs[curPos])->frequency)))
	{
		//swap the child and parent nodes
		heapSwap(curPos, curPos / 2, heapToUse);
		
		//set curPos to the parent of the current node
		curPos = curPos / 2;
	}//while

	//return success
	return 1;

}//heapBubbleUp

//extract the Min (root) node from the heap
HT_ITEM* heapExtractMin(HEAP* heapToUse)
{
	//save the number of items in a variable
	int curPos = heapToUse->inserted;

	//get the smallest item
	HT_ITEM* min = heapToUse->HT_Ptrs[1];

	//get a node to replace the one we just took
	heapToUse->HT_Ptrs[1] = heapToUse->HT_Ptrs[curPos];

	//clear the location where the old node was
	heapToUse->HT_Ptrs[curPos] = heapToUse->HT_Ptrs[0];

	//reduce the heap size by 1
	heapToUse->inserted = (curPos-1);

	//Sink Down the root
	heapSinkDown(heapToUse, 1);

	//return the min node
	return min;

}

//extract the Min (root) node from the heap
int heapSinkDown(HEAP* heapToUse, int sinkingNode)
{
	//save the sinking node to a temp variable
	HT_ITEM* tempHTPtr = (heapToUse->HT_Ptrs[sinkingNode]);

	//create a variable for the smallest node out of the two that will be compared
	int smallestNode = sinkingNode;

	//if the node in the left child is smaller than parent change the smallest variable
	if ((2 * sinkingNode < (heapToUse->inserted)) && 
		(((heapToUse->HT_Ptrs[smallestNode])->frequency)>((heapToUse->HT_Ptrs[2 * sinkingNode])->frequency)))
	{
		smallestNode = 2 * sinkingNode;
	}
	//if the node in the right child is smaller than parent change the smallest variable
	else if ((((2 * sinkingNode) + 1) < (heapToUse->inserted)) &&
		(((heapToUse->HT_Ptrs[smallestNode])->frequency)>((heapToUse->HT_Ptrs[(2 * sinkingNode) + 1])->frequency)))
	{
		smallestNode = (2 * sinkingNode)+1;
	}


	//if we found a smaller node swap the two (smallest/sinkingNode) and call heapSinkDown on the smallest node
	if (smallestNode != sinkingNode)
	{
		//do the swap
		heapSwap(smallestNode, sinkingNode, heapToUse);

		//call heapSinkDown again
		heapSinkDown(heapToUse, smallestNode);
	}

	//return success
	return 1;

}

//swap the nodes in positions a and b
int heapSwap(int posA, int posB, HEAP* heapToUse)
{
	//save the node at posA
	HT_ITEM* tempHTPtr = heapToUse->HT_Ptrs[posA];

	//make the current node at posA the same as the node at posB
	heapToUse->HT_Ptrs[posA] = heapToUse->HT_Ptrs[posB];

	//make the node at posB the same as the saved node (posA)
	heapToUse->HT_Ptrs[posB] = tempHTPtr;

	//return success
	return 1;
}

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
