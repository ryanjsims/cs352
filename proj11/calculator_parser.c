#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

#include "expr.h"
#include "expr_student.h"


/* callback functions, for the 'eval' function pointer */
float eval_digit(float,float, float right);

float eval_add(float,float, float);
float eval_sub(float,float, float);
float eval_mul(float,float, float);
float eval_div(float,float, float);
float eval_mod(float,float, float);


/* helper function, with the ability to modify the caller's pointer */
Expr *parseExpr_internal(char **pLine);


Expr *parseExpr(char *in)
{
	char *line = in;

	Expr *retval = parseExpr_internal(&line);
	if (retval == NULL)
		return NULL;

	/* consume whitespace (if any) that was after the expression - so that
	 * we can determine whether or not there are any additional errors.
	 */
	while (isspace(*line))
		line++;

	if (*line != '\0')
	{
		fprintf(stderr, "SYNTAX ERROR: trailing symbols at: %s\n", line);

		freeExpr(retval);
		return NULL;
	}

	return retval;
}


Expr *parseExpr_internal(char **pLine)
{
	char *line = *pLine;
	char *line_orig = line;

	/* consume leading whitespace. */
	while (isspace(*line))
		line++;

	/* if there's nothing left in the buffer, then return NULL.  This is
	 * sometimes an error - but not always, so we won't print an error
	 * message here.
	 */
	if (*line == '\0')
		return NULL;


	/* if the character is not a valid character, then return an error */
	if (*line != '(' && ! isdigit(*line))
	{
		fprintf(stderr, "SYNTAX ERROR: invalid character at: %s\n", line);
		return NULL;
	}


	/* in both of the possibilities below (parens or digit), we have to
	 * create a local Expr, but then also look for an operator that might
	 * follow.
	 *
	 * If we find anything *OTHER* than an operator, then we'll return the
	 * single Expr we've parsed, and let our caller handle the rest.  But
	 * if we find the operator, then we have more work to do here.
	 */
	Expr *left;


	/* if there is an open paren, then recurse.  Afterwards, we'll confirm
	 * that there's a close paren.  But then we have to *KEEP* parsing,
	 * because the close parent might be followed by an operator.
	 */
	if (*line == '(')
	{
		line++;

		left = parseExpr_internal(&line);
		if (left == NULL)
			return NULL;

		/* consume whitespace after the sub-expression */
		while (isspace(*line))
			line++;

		/* confirm that the closing paren exists */
		if (*line != ')')
		{
			fprintf(stderr, "ERROR: Missing closing paren at: %s\n", line);
			freeExpr(left);
			return NULL;
		}

		/* good case - successful paren parsed */
		line++;
		*pLine = line;
	}
	else
	{
		/* the expression must be a numeral.  It might then be
		 * followed by an operator, or we might hit end-of-expression
		 */
		int intVal = *line - '0';
		line++;

		left = malloc(sizeof(Expr));
		if (left == NULL)
		{
			perror("Could not allocate an Expr for a digit");
			return NULL;
		}

		left->origText = strndup(line-1, 1);
		if (left->origText == NULL)
		{
			perror("Could not strndup() a digit");
			freeExpr(left);
			return NULL;
		}

		left->left = left->right = NULL;
		left->floatVal = intVal;
		left->eval = &eval_digit;
	}


	/* consume the whitespace after the 'left' expression */
	while (isspace(*line))
		line++;


	/* do we see an operator?  If not, then just update pLine and
	 * then return immediately.  The caller might escalate this to
	 * an error - but we don't need to do that yet.
	 */
	if (*line != '+' && *line != '-' &&
	    *line != '*' && *line != '/' && *line != '%')
	{
		*pLine = line;
		return left;
	}


	char op = *line;
	line++;

	/* parse the next expression; it will consume the leading
	 * whitespace, if any
	 */
	Expr *right = parseExpr_internal(&line);
	if (right == NULL)
	{
		freeExpr(left);
		return NULL;
	}


	/* allocate the Expr to represent the binary operator */
	Expr *binOp = malloc(sizeof(Expr));
	if (binOp == NULL)
	{
		perror("Could not malloc() an Expr for a binary operator");
		freeExpr(left);
		freeExpr(right);
		return NULL;
	}

	/* save the text of the original expression */
	binOp->origText = strndup(line_orig, line-line_orig);
	if (binOp->origText == NULL)
	{
		perror("Could not strndup() an expression");
		freeExpr(left);
		freeExpr(right);
		freeExpr(binOp);
		return NULL;
	}

	binOp->left     = left;
	binOp->right    = right;
	binOp->floatVal = 0;

	if (op == '+')
		binOp->eval = &eval_add;
	else if (op == '-')
		binOp->eval = &eval_sub;
	else if (op == '*')
		binOp->eval = &eval_mul;
	else if (op == '/')
		binOp->eval = &eval_div;
	else if (op == '%')
		binOp->eval = &eval_mod;
	else
		assert(0);


	/* return the newly-completed binary operator.  But update
	 * pLine first!
	 */
	*pLine = line;
	return binOp;
}



float eval_digit(float left, float right, float digit)
{
	return digit;
}

float eval_add(float left, float right, float digit)
{
	return left+right;
}

float eval_sub(float left, float right, float digit)
{
	return left-right;
}

float eval_mul(float left, float right, float digit)
{
	return left*right;
}

float eval_div(float left, float right, float digit)
{
	return left/right;
}

float eval_mod(float left, float right, float digit)
{
	float quotF = left/right;
	int   quotI = (int)quotF;
	return left - (quotI*right);
}


