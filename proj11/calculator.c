#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expr.h"
#include "expr_student.h"

float evalPostOrder(Expr *node);
char* getExprString();

int main(int argc, char** argv){
	int retCode = 0;
	while(1){
		int rc;
		char *exprText = getExprString(stdin, &rc);
		if(rc == EOF){
			free(exprText);
			break;
		}
		Expr *root = parseExpr(exprText);
		if(root == NULL){
			printf("Could not parse the expression: %s\n", exprText);
			//retCode = 1;
		} else {
			printf("%f\n", evalPostOrder(root));
		}
		freeExpr(root);
		free(exprText);
	}
	return retCode;
}

float evalPostOrder(Expr *node){
	if(node == NULL)
		return 0;
	float left, right, toReturn;
	left = evalPostOrder(node->left);
	right = evalPostOrder(node->right);
	if(node->left == NULL && node->right == NULL){
		toReturn = node->eval(left, right, node->floatVal);
	}
	else{
		toReturn = node->eval(left, right, 0);
		printf("%s = %f\n", node->origText, toReturn);
	}
	return toReturn;
}

char* getExprString(FILE* input, int* rc){
	printf("> ");
	size_t n = 100;
	char* toReturn = malloc(n * sizeof(char));
	memset(toReturn, '\0', n);
	*rc = getline(&toReturn, &n, input);
	if(*rc == EOF)
		return toReturn;
	char* newline = strchr(toReturn, '\n');
	if(newline != NULL)
		*newline = '\0';
	return toReturn;
}

void freeExpr(Expr* root){
	if(root == NULL)
		return;
	freeExpr(root->left);
	freeExpr(root->right);
	free(root->origText);
	free(root);
}
