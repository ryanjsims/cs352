#ifndef __EXPR_H__INCLUDED__
#define __EXPR_H__INCLUDED__


/* Expr
 *
 * The Expr struct represents, in a (somewhat) object-oriented way, a numeric
 * expression which resolves to a float.
 *
 * It has two Expr parameters (for the various binary operators), and also a
 * float value, for literals.  It also has a string, which is the original
 * string that we parsed.
 *
 * The most interesting part, however, is that it has a function pointer to
 * give the correct 'eval' method for this object.
 */

typedef struct Expr Expr;
struct Expr
{
	char *origText;

	/* parameters:
	 *
	 * There are three parameters: two pointers to other Expr objects,
	 * and one float variable.  If the pointers are non-NULL, then
	 * the student code must recurse into both of them, and then pass
	 * the values that it finds as the first two parameters (and zero
	 * as the second).  But if they are NULL, then the student code
	 * must pass zeroes as the first two argument to eval(), and
	 * the float variable as the third argument.
	 */
	Expr  *left, *right;
	float  floatVal;
	

	/* this is the function pointer, which the student code must call
	 * to evaluate the value of this expression.
	 */
	float (*eval)(float left, float right, float floatVar);
};


#endif

