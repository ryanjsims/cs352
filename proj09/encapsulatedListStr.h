#ifndef __ENCAPSULATED_LIST_STR_H__INCLUDED__
#define __ENCAPSULATED_LIST_STR_H__INCLUDED__

/* encapsulatedListStr.h
 *
 * Declares that the EncList_Str "class," which represents an "encapsulated"
 * list - that is, a list that has a wrapper object.  With this type (unlike
 * DblList_Int), you can create a truly "empty" list.  It also has some
 * higher-level methods, like count(), addTail() (without using a node as a
 * parameter) and sort().
 *
 * ERROR HANDLING
 *
 * Most functions below have some ERRORS defined.  Check for these conditions.
 * If they occur, print out an error message to stderr, and then terminate
 * the method.  If the method returns non-void, the description will tell you
 * what you can expect the function will return.
 */



// this declaration tells C that there *EXISTS* some type which is a struct,
// named EncapulatedList_Str.  However, it doesn't actually give the contents
// of the struct.  It's a bit like a function prototype.
//
// With this, we are allowed to do the following:
//   - Declare *POINTER* variables (including parameters and return types) to
//     this struct.  This works because all pointers are the same size - and
//     down in the guts, all pointers are really the same thing (just the
//     address of a certain byte in memory).
//   - Declare the 'typedef' for shorthand use, as below
//
// However, we are *NOT* above to do the following:
//   - Declare any non-pointer variable using this type (since we don't know
//     the size).  This includes local variables, globals, *AND* any fields
//     inside other struts.
//   - Ask for the sizeof() the struct
//   - Access any field inside the struct
//
// In one of the C files - the one that impelements all of the methods for
// this "class" - you will give complete definition of the type.  That C file
// will be able to "look inside" one of these structs - no other file will be
// able to do so.
struct EncapsulatedList_Str;

// It's legal to rename the struct into a nicer-to-use name, even though we
// don't know the body.  The typedef has all the same limitations we listed
// above.
typedef struct EncapsulatedList_Str EncList_Str;


// Above, we gave the name of the "wrapper" class.  Here, we'll do the same,
// but for the "node" class.
struct EncapsulatedList_Str_Node;
typedef struct EncapsulatedList_Str_Node EncNode_Str;



/* "methods" of the EncList_Str "class" */

// ------------- alloc() - Constructor ---------------
// Parameters: None
//
// Allocates a new EncList_Str object, and initializes it to hold an empty
// list (that is, no node objects should be allocated).
//
// ERRORS:
//   - malloc() fails.  Print error and return NULL

EncList_Str *encList_Str__alloc();

// -------------- free() - Destructor ----------------
// Parameters: 'this' pointer (for the wrapper object)
//
// Frees an existing EncList_Str object.  If there are any nodes inside this
// list, this also frees all of them.
//
// ERRORS:
//   - Pointer is NULL

void encList_Str__free(EncList_Str*);


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

void encList_Str__addHead(EncList_Str*, char *string, int dup);


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

void encList_Str__addTail(EncList_Str*, char *string, int dup);


// ---------------- count ----------------------------
// Parameters: 'this' pointer (of the wrapper class)
//
// Returns the number of nodes in the list.
//
// ERRORS:
//   'this' is NULL.  Print error and return -1.

int encList_Str__count(EncList_Str*);


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

char *encList_Str__getMin(EncList_Str*);
char *encList_Str__getMax(EncList_Str*);


// ---------------- sort ----------------------------
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

void encList_Str__merge(EncList_Str*, EncList_Str*);


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

void encList_Str__append(EncList_Str*, EncList_Str*);


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

EncNode_Str *encList_Str__index(EncList_Str*, int index);


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

EncList_Str *encList_Str__splitAt(EncList_Str*, int index);


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

EncNode_Str *encList_Str__getHead(EncList_Str*);
EncNode_Str *encList_Str__getTail(EncList_Str*);


// ---------------- gettors (various) -----------------------
// Parameters: 'this' pointer (for the NODE!)
//
// Returns various properties of the list node.  next/prev will return NULL
// when we hit the extreme of the list.
//
// ERRORS:
//   - None
//     (the pointer might be NULL; just let it segfault)

char        *encNode_Str__getStr (EncNode_Str*);
EncNode_Str *encNode_Str__getNext(EncNode_Str*);
EncNode_Str *encNode_Str__getPrev(EncNode_Str*);


#endif

