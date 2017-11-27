#ifndef __DBL_LIST_INT_H__INCLUDED__
#define __DBL_LIST_INT_H__INCLUDED__

/* dblListInt.h
 *
 * Declares the DblList_Int struct (a doubly-linked list, holding integers)
 * and gives the prototypes for various "methods" associated with it.
 *
 * Some of this is a little excessive, frankly, for a C program.  But the
 * point is to illustrate how you can implement object-oriented style code in
 * C (and, of course, to show how object oriented languages like C++ and Java
 * work under the covers).
 *
 * ERROR HANDLING
 *
 * Most functions below have some ERRORS defined.  Check for these conditions.
 * If they occur, print out an error message to stderr, and then terminate
 * the method.  If the method returns non-void, the description will tell you
 * what you can expect the function will return.
 */



typedef struct DblList_Int
{
	int val;

	struct DblList_Int *next;
	struct DblList_Int *prev;
} DblList_Int;



/* "methods" of the DblList_Int "class" */

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

DblList_Int *dblList_Int__alloc(int val);

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

void dblList_Int__free(DblList_Int*);


// ---------------- gettors (various) -----------------------
// Parameters: 'this' pointer
//
// Returns various properties of the list node.
//
// ERRORS: None
//         (the pointer might be NULL; just let it segfault)

int          dblList_Int__getVal (DblList_Int*);
DblList_Int *dblList_Int__getNext(DblList_Int*);
DblList_Int *dblList_Int__getPrev(DblList_Int*);


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

DblList_Int *dblList_Int__getHead(DblList_Int*);
DblList_Int *dblList_Int__getTail(DblList_Int*);


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

void dblList_Int__addAfter(DblList_Int*, DblList_Int*);

// ---------------- addTail ---------------------------------
// Parameters: 'this' pointer
//             *value*
//
// Searches for the tail end of the list; allocates a new node (using the
// value given), and appends it to the tail of the list.  Returns nothing.
//
// ERRORS:
//   - Pointer is NULL.  Print error.

void dblList_Int__addTail(DblList_Int*, int value);

// ---------------- remove ---------------------------------
// Parameters: 'this' pointer
//
// Removes the current node from the list it is part of.
//
// ERRORS:
//   - Pointer is NULL.  Print error.
//   - Object is not part of any list (that is, next==prev==NULL).  Print
//     error.

void dblList_Int__remove(DblList_Int*);

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

void dblList_Int__swapWithNext(DblList_Int*);


#endif

