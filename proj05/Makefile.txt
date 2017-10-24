all:

rollingStrings:	rollingStrings.c
	gcc -g -Wall -std=c99 rollingStrings.c -o rollingStrings
