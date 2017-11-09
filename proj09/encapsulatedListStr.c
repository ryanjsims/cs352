#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encapsulatedListStr.h"



struct EncapsulatedList_Str{
	EncNode_Str *head, *tail;
	int length;	
};


struct EncapsulatedList_Str_Node{
	char *val;
	struct EncapsulatedList_Str_Node *next, *prev;
	int dup;
};



/* "methods" of the EncList_Str "class" */

// ------------- alloc() - Constructor ---------------
// Parameters: None
//
// Allocates a new EncList_Str object, and initializes it to hold an empty
// list (that is, no node objects should be allocated).
//
// ERRORS:
//   - malloc() fails.  Print error and return NULL

EncList_Str *encList_Str__alloc(){
	EncList_Str *new = malloc(sizeof(EncList_Str));
	if(new == NULL){
		perror("Malloc failed!");
		return NULL;
	}
	new->head = NULL;
	new->tail = NULL;
	new->length = 0;
	return new;
}

// -------------- free() - Destructor ----------------
// Parameters: 'this' pointer (for the wrapper object)
//
// Frees an existing EncList_Str object.  If there are any nodes inside this
// list, this also frees all of them.
//
// ERRORS:
//   - Pointer is NULL

void encList_Str__free(EncList_Str* this){
	if(this == NULL){
		fprintf(stderr, "List was NULL!\n");
		return;
	}
	EncNode_Str* curr = this->head;
	EncNode_Str* prev;
	while(curr != NULL){
		prev = curr;
		curr = curr->next;
		if(prev->dup == 1)
			free(prev->val);
		free(prev);
	}
	free(this);
}


// ---------------- addHead ---------------------------------
// Parameters: 'this' pointer (for the wrapper object)
//             *string*
//             dup (boolean flag)
//
// Adds the given string to the front of the list (duplicates are allowed).  If
// dup=1, then this method will malloc() a new buffer, and copy the string
// into that buffer.  If dup=0, it will simply save the pointer into the node.
//
// When the node is destroyed later, free() will free the string if it was
// duplicated in this function - but if it was not duplicated, then it will
// *NOT* free it, and the caller is responsible for doing so.
//
// ERRORS:
//   - Pointer is NULL.  Print error.

void encList_Str__addHead(EncList_Str* this, char *string, int dup){
	if(this == NULL){
		fprintf(stderr, "List was NULL!\n");
		return;
	}
	
	char *toAdd;
	if(dup == 1){
		int length = strlen(string) + 1;
		toAdd = malloc(length * sizeof(char));
		memcpy(toAdd, string, length);
	}
	else
		toAdd = string;
	EncNode_Str *newHead = malloc(sizeof(EncNode_Str));
	newHead->val = toAdd;
	newHead->prev = NULL;
	newHead->next = this->head;
	newHead->dup = dup;
	if(this->head != NULL)
		this->head->prev = newHead;
	else
		this->tail = newHead;
	this->head = newHead;
	this->length++;
}


// ---------------- addTail ---------------------------------
// Parameters: 'this' pointer (for the wrapper object)
//             *string*
//             dup (boolean flag)
//
// Adds the given string to the end of the list (duplicates are allowed).  If
// dup=1, then this method will malloc() a new buffer, and copy the string
// into that buffer.  If dup=0, it will simply save the pointer into the node.
//
// When the node is destroyed later, free() will free the string if it was
// duplicated in this function - but if it was not duplicated, then it will
// *NOT* free it, and the caller is responsible for doing so.
//
// ERRORS:
//   - Pointer is NULL.  Print error.

void encList_Str__addTail(EncList_Str* this, char *string, int dup){
	if(this == NULL){
		fprintf(stderr, "List was NULL!\n");
		return;
	}
	
	char *toAdd;
	if(dup == 1){
		int length = strlen(string) + 1;
		toAdd = malloc(length * sizeof(char));
		memcpy(toAdd, string, length);
	}
	else
		toAdd = string;
	EncNode_Str *newTail = malloc(sizeof(EncNode_Str));

	newTail->val = toAdd;
	newTail->prev = this->tail;
	newTail->next = NULL;
	newTail->dup = dup;
	if(this->head == NULL)
		this->head = newTail;
	else
		this->tail->next = newTail;
	this->tail = newTail;
	this->length++;
}


// ---------------- count ----------------------------
// Parameters: 'this' pointer (of the wrapper class)
//
// Returns the number of nodes in the list.
//
// ERRORS:
//   'this' is NULL.  Print error and return -1.

int encList_Str__count(EncList_Str* this){
	if(this == NULL){
		fprintf(stderr, "List was NULL!\n");
		return -1;
	}
	return this->length;
}


// ---------------- getMin/getMax ----------------------------
// Parameters: 'this' pointer (of the wrapper class)
//
// Searches the list for the minimum or maximum string.  This does *NOT*
// assume that the list is sorted, and so it probably will perform a
// brute-force scan of the entire list.
//
// Returns NULL if the list is empty.
//
// ERRORS:
//   'this' is NULL.  Print error and return NULL.

char *encList_Str__getMin(EncList_Str* this){
	if(this == NULL){
		fprintf(stderr, "List was NULL!\n");
		return NULL;
	}
	if(this->head == NULL){
		return NULL;
	}
	EncNode_Str *curr = this->head;
	char *toReturn = curr->val;
	while(curr != NULL){
		if(strcmp(toReturn, curr->val) > 0)
			toReturn = curr->val;
		curr = curr->next;
	}
	return toReturn;
}
char *encList_Str__getMax(EncList_Str* this){
	if(this == NULL){
		fprintf(stderr, "List was NULL!\n");
		return NULL;
	}
	if(this->head == NULL){
		return NULL;
	}
	EncNode_Str *curr = this->head;
	char *toReturn = curr->val;
	while(curr != NULL){
		if(strcmp(toReturn, curr->val) < 0)
			toReturn = curr->val;
		curr = curr->next;
	}
	return toReturn;
}


// ---------------- merge ----------------------------
// Parameters: 'this' pointer (of the wrapper class)
//             another list
//
// This function assumes (but does not verify) that the two lists are both
// already sorted.  It iterates through the two lists in parallel, always
// taking the smaller value from each list and placing into the resulting,
// merged list.
//
// Thus, the resulting list will be sorted when the function completes.
//
// The merged list will be stored in the first parameter; all of the nodes
// will be removed from the second parameter (and placed in their proper
// locations within the first).
//
// EXAMPLE:
//   Before merge():
//       left  list: foo jkl qwerty
//       right list: asdf bar good zzzz
//   After merge():
//       left  list: asdf bar foo good jkl qwerty zzzz
//       right list: <empty>
//
// NOTE: Strings must *NOT* be copied back and forth between nodes!
//       This function *ONLY* changes the next/prev arrows that link nodes!
//
// ERRORS:
//   Either pointer is NULL.  Print error.

void encList_Str__merge(EncList_Str* left, EncList_Str* right){
	if(left == NULL || right == NULL){
		fprintf(stderr, "One of the lists were NULL!\n");
		return;
	}
	EncNode_Str* currLeft = left->head;
	EncNode_Str* currRight = right->head;
	while(currRight != NULL && currLeft != NULL){
		 if(strcmp(currLeft->val, currRight->val) <= 0){
			currLeft = currLeft->next;
		 }
		 else{
			EncNode_Str* nextRight = currRight->next;
			currRight->prev = currLeft->prev;
			if(currRight->prev != NULL)
				currRight->prev->next = currRight;
			else
				left->head = currRight;
			currLeft->prev = currRight;
			currRight->next = currLeft;
			currRight = nextRight;
			if(currRight != NULL)
				currRight->prev = NULL;
		 }
	}
	//All the remaining values in right list are > than the values in left list
	//Add tje rest of the right list to the end of the left list.
	if(currRight != NULL && left->head != NULL){
		left->tail->next = currRight;
		currRight->prev = left->tail;
		left->tail = right->tail;
		right->head = NULL;
		right->tail = NULL;
	} 
	//Left list is empty, add the nodes from the right list into it.
	else if(currRight != NULL && left->head == NULL){
		left->head = right->head;
		left->tail = right->tail;
		right->head = NULL;
		right->tail = NULL;
	} 
	//Otherwise, right was empty, so nothing needs to be done
	//Just need to ensure that the right list is emptied of pointers
	//that could now be in the middle of another list.
	right->head = NULL;
	right->tail = NULL;
	left->length = left->length + right->length;
	right->length = 0;
}


// ---------------- append ----------------------------
// Parameters: 'this' pointer (of the wrapper class)
//             pointer to another list
//
// Removes all of the nodes from the 'other' list, and adds them to the tail
// of the 'this' list.  After this function, the 'other' list will still be
// valid, but will be empty.
//
// ERRORS:
//   - Either pointer is NULL.  Print error.

void encList_Str__append(EncList_Str* this, EncList_Str* other){
	if(this == NULL || other == NULL){
		fprintf(stderr, "One of the lists were NULL!\n");
		return;
	}
	if(this->head != NULL && other->head != NULL);
	this->tail->next = other->head;
	other->head->prev = this->tail;
	this->tail = other->tail;
	other->head = NULL;
	other->tail = NULL;
	this->length = this->length + other->length;
	other->length = 0;
}


// ---------------- index ---------------------------------
// Parameters: 'this' pointer (for the wrapper object)
//             index
//
// Returns the node at a given "index."  Index 0 is the head; index 1 is the
// node immediately after the head.
//
// Just like an array, the valid indices are 0 through count()-1, inclusive.
//
// ERRORS:
//   - Pointer is NULL.  Print error and return NULL.
//   - Invalid index (negative, or too large).  Print error and return NULL.

EncNode_Str *encList_Str__index(EncList_Str* this, int index){
	if(this == NULL){
		fprintf(stderr, "List was NULL!\n");
		return NULL;
	}
	if(index >= this->length || index < 0){
		fprintf(stderr, "Index error\n");
		return NULL;
	}
	EncNode_Str *curr;
	int currIndex;
	if(index < this->length/2){
		curr = this->head;
		currIndex = 0;
		while(currIndex < index){
			curr = curr->next;
			currIndex++;
		}
	} else {
		curr = this->tail;
		currIndex = this->length - 1;
		while(currIndex > index){
			curr = curr->prev;
			currIndex--;
		}
	}
	return curr;
}


// ---------------- splitAt ----------------------------
// Parameters: 'this' pointer (of the wrapper class)
//             index into the list
//
// Splits the current list into two.  This allocates a new list object.  It
// then moves some of the "back" end of this list into the new object (these
// elements are removed from the current list).
//
// The position for the split is given by the 'index;' the index is the first
// node that will be *MOVED* to the other list.  So if index=0, then the
// *ENTIRE* list will be moved into the new object; if index=4, then 4 will
// remain in the current list, and the rest will be moved.
//
// Basically, the index parameter works like the index() method above, with
// one exception: the value count() is *LEGAL* in this function.  (This simply
// means, move *NOTHING* into the new list.)
//
// ERRORS:
//   - Either pointer is NULL.  Print error and return NULL
//   - Index is invalid.  Print error and return NULL

EncList_Str *encList_Str__splitAt(EncList_Str* this, int index){
	if(this == NULL){
		fprintf(stderr, "The list was NULL!\n");
		return NULL;
	}
	if(index < 0 || index > this->length){
		fprintf(stderr, "Invalid index!\n");
		return NULL;
	}
	EncList_Str *new = encList_Str__alloc();
	if(index == this->length){
		return new;
	}
	new->head = encList_Str__index(this, index);
	new->tail = this->tail;
	if(new->head == this->head){
		this->head = NULL;
		this->tail = NULL;
		new->length = this->length;
		this->length = 0;
	}
	else{
		this->tail = new->head->prev;
		this->tail->next = NULL;
		new->head->prev = NULL;
		new->length = this->length - index;
		this->length = this->length - new->length;
	}
	return new;
}


// ---------------- getHead ---------------------------------
// Parameters: 'this' pointer (for the wrapper object)
//
// Returns a pointer to the head of the list (if the list is empty, this
// returns NULL).  See below for the available "methods" you can call on a
// node.
//
// getTail(): Equivalent, but finds the last element
//
// ERRORS:
//   - Pointer is NULL.  Print error and return NULL.

EncNode_Str *encList_Str__getHead(EncList_Str* this){
	if(this == NULL){
		fprintf(stderr, "List was NULL!\n");
		return NULL;
	}
	return this->head;
}
EncNode_Str *encList_Str__getTail(EncList_Str* this){
	if(this == NULL){
		fprintf(stderr, "List was NULL!\n");
		return NULL;
	}
	return this->tail;
}


// ---------------- gettors (various) -----------------------
// Parameters: 'this' pointer (for the NODE!)
//
// Returns various properties of the list node.  next/prev will return NULL
// when we hit the extreme of the list.
//
// ERRORS:
//   - None
//     (the pointer might be NULL; just let it segfault)

char        *encNode_Str__getStr (EncNode_Str* node){
	return node->val;
}
EncNode_Str *encNode_Str__getNext(EncNode_Str* node){
	return node->next;
}
EncNode_Str *encNode_Str__getPrev(EncNode_Str* node){
	return node->prev;
}


