/*
 * dblListInt.c
 * Programmer: Ryan Sims 
 * Purpose: dblListInt implements a doubly linked list of integers, using
 * a C struct to create a pseudo-object with methods defined to operate
 * with a pointer to the struct.
 */

#include <stdio.h>
#include <stdlib.h>
#include "dblListInt.h"


// ------------- alloc() - Constructor ---------------
// Parameters: int (value for the new node)
//             NOTE: no 'this' pointer
//
// Allocates a new DblList_Int object.  Initializes the val to the value
// provided.  Initializes the next,prev pointers to NULL (that is, the
// new node is not on any list).  Returns the newly allocated object.
//
// ERRORS:
//   - malloc() fails.  Print error and return NULL

DblList_Int *dblList_Int__alloc(int val){
	DblList_Int *new = malloc(sizeof(DblList_Int));
	if(new == NULL){
		perror("Malloc failed.");
		return NULL;
	}
	new->val = val;
	new->next = NULL;
	new->prev = NULL;
	return new;
}

// -------------- free() - Destructor ----------------
// Parameters: 'this' pointer
//
// Frees an existing DblList_Int object.  The object must not be part of any
// list (that is, its 'next' and 'prev' must both be NULL).
//
// ERRORS:
//   - Pointer is NULL
//   - The node has non-NULL next or prev pointers.  Print error, but still
//     free the object before returning.

void dblList_Int__free(DblList_Int *this){
	if(this == NULL){
		return;
	}
	if(this->next != NULL || this->prev != NULL){
		fprintf(stderr, "Node is still part of a list.\n");
	}
	free(this);
}


// ---------------- gettors (various) -----------------------
// Parameters: 'this' pointer
//
// Returns various properties of the list node.
//
// ERRORS: None
//         (the pointer might be NULL; just let it segfault)

int          dblList_Int__getVal (DblList_Int* this){
	return this->val;
}
DblList_Int *dblList_Int__getNext(DblList_Int* this){
	return this->next;
}
DblList_Int *dblList_Int__getPrev(DblList_Int* this){
	return this->prev;
}


// ---------------- getHead ---------------------------------
// Parameters: 'this' pointer
//
// Searches toward the front of the list, from the 'this' pointer; returns the
// node at the head of the list.  (This might be the 'this' pointer.)
//
// getTail(): Equivalent, but finds the last element
//
// ERRORS:
//   - Pointer is NULL.  Print error and return NULL.

DblList_Int *dblList_Int__getHead(DblList_Int* this){
	if(this == NULL){
		fprintf(stderr, "This was NULL.\n");
		return NULL;
	}
	DblList_Int *curr = this;
	while(curr->prev != NULL){
		curr = curr->prev;
	}
	return curr;
}
DblList_Int *dblList_Int__getTail(DblList_Int* this){
	if(this == NULL){
		fprintf(stderr, "This was NULL.\n");
		return NULL;
	}
	DblList_Int *curr = this;
	while(curr->next != NULL){
		curr = curr->next;
	}
	return curr;
}


// ---------------- addAfter --------------------------------
// Parameters: 'this' pointer
//             Pointer to another object
//
// Chains the 'other' node, immediately after the 'this' node.  We assume that
// the 'this' object is part of a list (though it might be a list of length 1),
// so we consider that there might already be a 'next' node; if that is, true,
// then the 'other' is inserted *BETWEEN* 'this' and its (old) next neighbor.
//
// For example imagine that node A is the 'this' pointer, node B is the one
// that comes after it, and we call addAfter(), passing X as the 'other' node.
//    This is what the list looked like before the method:
//                A - B
//    This is what it will look like after the method:
//                A - X - B
//
// ERRORS:
//   - Either Pointer is NULL.  Print error.
//   - The 'other' object is already on a list.  Print error and return; do
//     *NOT* change either list.

void dblList_Int__addAfter(DblList_Int* this, DblList_Int* other){
	if(this == NULL || other == NULL){
		fprintf(stderr, "One of the pointers was NULL!\n");
		return;
	}
	if(other->next != NULL || other->prev != NULL){
		fprintf(stderr, "Other is part of another list!\n");
		return;
	}
	if(this->next == NULL){
		this->next = other;
		other->prev = this;
	} else {
		other->next = this->next;
		other->prev = this;
		other->next->prev = other;
		this->next = other;
	}
}

// ---------------- addTail ---------------------------------
// Parameters: 'this' pointer
//             *value*
//
// Searches for the tail end of the list; allocates a new node (using the
// value given), and appends it to the tail of the list.  Returns nothing.
//
// ERRORS:
//   - Pointer is NULL.  Print error.

void dblList_Int__addTail(DblList_Int* this, int value){
	DblList_Int *tail = dblList_Int__getTail(this);
	if(tail == NULL)
		return;
	tail->next = dblList_Int__alloc(value);
	tail->next->prev = tail;
}

// ---------------- remove ---------------------------------
// Parameters: 'this' pointer
//
// Removes the current node from the list it is part of.
//
// ERRORS:
//   - Pointer is NULL.  Print error.
//   - Object is not part of any list (that is, next==prev==NULL).  Print
//     error.

void dblList_Int__remove(DblList_Int* this){
	if(this == NULL){
		fprintf(stderr, "This was NULL!\n");
		return;
	}
	if(this->next == NULL && this->prev == NULL){
		fprintf(stderr, "This is not part of any list!\n");
		return;
	}
	if(this->prev == NULL)
		this->next->prev = NULL;
	else if(this->next == NULL)
		this->prev->next = NULL;
	else{
		this->prev->next = this->next;
		this->next->prev = this->prev;
	}
	this->next = NULL;
	this->prev = NULL;
}

// ---------------- swapWithNext ----------------------------
// Parameters: 'this' pointer
//
// Swaps the position of the current node and the next on the list.  This
// function *MUST* actually change the 'next' and 'prev' pointers; it is *NOT*
// OK to copy the 'val' fields back and forth.
//
// For example, if we have the list
//    A - B - C - D
// and we call swapWithNext() on B, then the list will change to:
//    A - C - B - D
//
// Note that we assume that thee object (and its next object) are both
// non_NULL.  However, the function will work properly even if the object is
// the head (its successor becomes the new head) or the 'next' field is the
// tail (the 'this' object becomes the new tail).
//
// ERRORS:
//   'this', or the next object, are NULL.  Print error.

void dblList_Int__swapWithNext(DblList_Int* this){
	if(this == NULL || this->next == NULL){
		fprintf(stderr, "The object or it's next was NULL!\n");
		return;
	}
	DblList_Int *next = this->next;
	//passed list of two elements
	if(this->prev == NULL && next->next == NULL){
		this->next = NULL;
		next->prev = NULL;
		next->next = this;
		this->prev = next;
	}
	//passed head of list
	else if(this->prev == NULL){
		this->next = next->next;
		next->prev = NULL;
		next->next = this;
		this->next->prev = this;
		this->prev = next;
	}
	//passed tail - 1 of list
	else if(next->next == NULL){
		this->next = NULL;
		next->prev = this->prev;
		next->next = this;
		this->prev = next;
		next->prev->next = next;
	}
	//general case
	else{
		this->next = next->next;
		next->prev = this->prev;
		next->next = this;
		this->prev = next;
		next->prev->next = next;
		this->next->prev = this;
	}
}

