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

int checkFormatting(char* line);

int combineDuplicates(Group** groups, size_t len);
void printGroups(Group** groups, size_t len);
void printVar(Var* var);
void freeGroup(Group* group);
void freeAll(Group** groups, size_t len);

int main(int argc, char** argv){
	size_t groupsLen = 0;
	Group** groups = parseInput(stdin, &groupsLen);
	if(groups == NULL)
		return 1;
	printGroups(groups, groupsLen);
	printf("----- SORTING THE GROUPS -----\n");
	qsort(groups, groupsLen, sizeof(Group*), compGroup);
	printf("----- JOINING DUPLICATE GROUP NAMES -----\n");
	groupsLen = combineDuplicates(groups, groupsLen);
	printf("----- SORTING THE VARIABLES INSIDE EACH GROUP -----\n");
	for(size_t i = 0; i < groupsLen; i++){
		Var** vars = groups[i]->variables;
		size_t varLen = groups[i]->len;
		qsort(vars, varLen, sizeof(Var*), compVar);
	}
	printf("----- FINAL VERSION -----\n");
	printGroups(groups, groupsLen);
	freeAll(groups, groupsLen);
	return 0;
}

int compVar(const void* var1, const void* var2){
	const Var* first = *((const Var**)var1);
	const Var* second = *((const Var**)var2);
	return strcmp(first->name, second->name);
}

int compGroup(const void* group1, const void* group2){
	const Group* first = *((const Group**)group1);
	const Group* second = *((const Group**)group2);
	return strcmp(first->name, second->name);
}

Group** parseInput(FILE* input, size_t* len){
	size_t groups = 0, capacity = 2, linelen = 0;
	Group** toReturn = NULL;
	char* line = NULL;
	while(1){
		int rc = getline(&line, &linelen, input);
		if(rc == EOF){
			break;
		}
		if(rc <= 1)
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
		int fmt = checkFormatting(line);
		if(fmt == 1){
			line[strlen(line) - 1] = '\0';
			fprintf(stderr, "ERROR: The first character in the line '%s' was not a letter!\n", line);
			break;
		} else if (fmt == 2){
			line[strlen(line) - 1] = '\0';
			fprintf(stderr, "ERROR: The line '%s' must separate its halves by '=' or ':'\n", line);
			continue;
		}
		if(isGroupLine(line, rc)){
			toReturn[groups] = malloc(sizeof(Group));
			if(toReturn[groups] == NULL){
				perror("Failed to allocate memory.");
				return NULL;
			}
			toReturn[groups]->name = getName(line, rc);
			toReturn[groups]->len = 0;
			toReturn[groups]->cap = 1;
			toReturn[groups]->variables = malloc(sizeof(Var*)); 
			if(toReturn[groups]->variables == NULL){
				perror("Failed to allocate memory.");
				return NULL;
			}
			groups++;
		} else if(groups == 0){
			line[strlen(line) - 1] = '\0';
			fprintf(stderr, "ERROR: Variable before group in line \"%s\"\n", line);
		} else {
			Var* newVar = malloc(sizeof(Var));
			if(newVar == NULL){
				perror("Failed to allocate memory.");
				return NULL;
			}
			newVar->name = getName(line, rc);
			newVar->val  = getVal(line, rc);
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
	*len = groups;
	free(line);
	return toReturn;
}

int isGroupLine(char* line, size_t len){
	size_t pos = len - 1;
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
	end = pos;
	return strndup(line + start, end - start);
}
char* getVal(char* line, size_t len){
	size_t pos = 0, start;
	while(line[pos] != '=')
		pos++;
	pos++;
	while(isspace(line[pos]))
		pos++;
	start = pos;
	char* toReturn = strndup(line + start, len - start);
	if(strcmp(toReturn, "") == 0 || toReturn[strlen(toReturn) - 1] != '\n'){
		toReturn = realloc(toReturn, strlen(toReturn) + 2);
		toReturn[strlen(toReturn) + 1] = '\0';
		toReturn[strlen(toReturn)] = '\n';
	}
	return toReturn;
}

int checkFormatting(char* line){
	int i = 0;
	int len = strlen(line);
	while(i < len && isspace(line[i]))
		i++;
	if(i < len && !isalpha(line[i]))
		return 1;
	while(i < len && (isalpha(line[i]) || isspace(line[i])))
		i++;
	if(i < len && line[i] != '=' && line[i] != ':')
		return 2;
	return 0;
}

int combineDuplicates(Group** groups, size_t len){
	if(len <= 1)
		return len;
	int i = 0;
	int uniq = 0;
	while(i < len){
		if(uniq != i && strcmp(groups[uniq]->name, groups[i]->name) == 0){
			size_t newlen = groups[uniq]->len + groups[i]->len;
			size_t currlen = groups[uniq]->len;
			size_t duplen = groups[i]->len;

			groups[uniq]->variables = realloc(groups[uniq]->variables, newlen * sizeof(Var*));
			if(groups[uniq]->variables == NULL){
				perror("Unable to allocate memory.");
				return -1;
			}
			Var** dest = groups[uniq]->variables + currlen;
			for(int j = 0; j < duplen; j++){
				dest[j] = malloc(sizeof(Var));
				if(dest[j] == NULL){
					perror("Unable to allocate memory.");
					return -1;
				}
				dest[j]->name = strdup(groups[i]->variables[j]->name);
				dest[j]->val = strdup(groups[i]->variables[j]->val);
				if(dest[j]->name == NULL || dest[j]->val == NULL){
					perror("Unable to allocate memory.");
					return -1;
				}
			}
			//memcpy(dest, groups[i]->variables, duplen);

			groups[uniq]->len = newlen;
			groups[uniq]->cap = newlen;

			freeGroup(groups[i]);
		}
		else if(uniq != i){
			uniq++;
			groups[uniq] = groups[i];
		}
		i++;
	}
	return uniq + 1;
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
	printf("    %s = %s", var->name, var->val);
}

void freeGroup(Group* group){
	for(int i = 0; i < group->len; i++){
		free(group->variables[i]->name);
		free(group->variables[i]->val);
		free(group->variables[i]);
	}
	free(group->variables);
	free(group->name);
	free(group);
}

void freeAll(Group** groups, size_t len){
	for(int i = 0; i < len; i++){
		freeGroup(groups[i]);
	}
	free(groups);
}
