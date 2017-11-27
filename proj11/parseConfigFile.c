#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Var{
	char* name;
	char* val;
} Var;

typedef struct Group{
	char* name;
	size_t len, cap;
	Var**  variables;
} Group;

int compVar(const void* var1, const void* var2);
int compGroup(const void* group1, const void* group2);

Group** parseInput(FILE* input, size_t* len);
int isGroupLine(char* line, size_t len);
char* getName(char* line, size_t len);
char* getVal(char* line, size_t len);

void printGroups(Group** groups, size_t len);
void printVar(Var* var);

int main(int argc, char** argv){
	printf("%d\n", isGroupLine("Testing:\n", 9));
	printf("%d\n", isGroupLine("Testing:\n", 10));
	/*size_t groupsLen = 0;
	Group** groups = parseInput(stdin, &groupsLen);
	if(groups == NULL)
		return 1;

	qsort(groups, groupsLen, sizeof(Group), compGroup);
	for(size_t i = 0; i < groupsLen; i++){
		Var** vars = groups[i]->variables;
		size_t varLen = groups[i]->len;
		qsort(vars, varLen, sizeof(Var), compVar);
	}
	
	printGroups(groups, groupsLen);
	return 0;*/
}

int compVar(const void* var1, const void* var2){
	const Var* first = (const Var*)var1;
	const Var* second = (const Var*)var2;
	return strcmp(first->name, second->name);
}

int compGroup(const void* group1, const void* group2){
	const Group* first = (const Group*)group1;
	const Group* second = (const Group*)group2;
	return strcmp(first->name, second->name);
}

Group** parseInput(FILE* input, size_t* len){
	size_t groups = 0, capacity = 2, linelen = 0;
	Group** toReturn = NULL;
	char* line = NULL;
	while(1){
		int rc = getline(&line, &linelen, input);
		if(rc == EOF){
			return toReturn;
		}
		if(linelen <= 1)
			continue;
		if(toReturn == NULL){
			toReturn = malloc(capacity * sizeof(Group*));
		}
		else if(groups == capacity){
			toReturn = realloc(toReturn, capacity * 2 * sizeof(Group*));
			capacity = 2 * capacity;
		}
		if(toReturn == NULL){
			perror("Failed to allocate memory.");
			return NULL;
		}
		if(isGroupLine(line, linelen)){
			toReturn[groups] = malloc(sizeof(Group));
			if(toReturn[groups] == NULL){
				perror("Failed to allocate memory.");
				return NULL;
			}
			toReturn[groups]->name = getName(line, linelen);
			toReturn[groups]->len = 0;
			toReturn[groups]->cap = 1;
			toReturn[groups]->variables = malloc(sizeof(Var*)); 
			if(toReturn[groups]->variables == NULL){
				perror("Failed to allocate memory.");
				return NULL;
			}
			groups++;
		} else if(groups == 0){
			perror("Incorrectly formatted input, variable without group!");
			return NULL;
		} else {
			Var* newVar = malloc(sizeof(Var));
			if(newVar == NULL){
				perror("Failed to allocate memory.");
				return NULL;
			}
			newVar->name = getName(line, linelen);
			newVar->val  = getVal(line, linelen);
			Group* target = toReturn[groups-1];
			if(target->len == target->cap){
				target->variables = realloc(target->variables, target->cap * 2 * sizeof(Var*));
				if(target->variables == NULL){
					perror("Failed to allocate memory.");
					return NULL;
				}
				target->cap *= 2;
			}
			target->variables[target->len] = newVar;
			target->len++;
		}
	}
}

int isGroupLine(char* line, size_t len){
	size_t pos = len - 1;
	printf("%c\n", line[pos]);
	printf("%c\n", line[pos - 1]);
	while(isspace(line[pos]) || line[pos] == '\0')
		pos--;
	return line[pos] == ':';
}

char* getName(char* line, size_t len){
	size_t pos = 0, start, end;
	while(isspace(line[pos]))
		pos++;
	start = pos;
	while(isalpha(line[pos]))
		pos++;
	end = pos - 1;
	return strndup(line + start, end - start);
}
char* getVal(char* line, size_t len){
	size_t pos = len - 1, start, end;
	while(isspace(line[pos]))
		pos--;
	end = pos;
	while(isalpha(line[pos]))
		pos--;
	start = pos + 1;
	return strndup(line + start, end - start);
}

void printGroups(Group** groups, size_t len){
	for(size_t i = 0; i < len; i++){
		printf("%s:\n", groups[i]->name);
		for(size_t j = 0; j < groups[i]->len; j++){
			printVar(groups[i]->variables[j]);
		}
	}
}
void printVar(Var* var){
	printf("\t%s = %s\n", var->name, var->val);
}
