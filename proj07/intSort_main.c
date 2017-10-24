/* intSort_main.c
 *
 * Author: Russ Lewis
 *
 * main() file for the intSort program.  Provided by Russ, for use in student
 * programs.
 */


#include <stdio.h>

#include "proj07_intList.h"



int main(int argc, char **argv)
{
	/* sanity check the arguments. */
	if (argc != 3)
	{
		fprintf(stderr, "SYNTAX: %s <in> <out>\n", argv[0]);
		return 1;
	}

	/* open both the input and output files.  We open the input first,
	 * instead of opening both, so that if the input doesn't exist,
	 * then we won't create an empty output file.
	 */
	FILE *in  = fopen(argv[1], "r");
	if (in == NULL)
	{
		perror("Cannot open the input file");
		return 1;
	}

	FILE *out = fopen(argv[2], "w");
	if (out == NULL)
	{
		perror("Cannot open the output file");
		return 1;
	}


	/* this function must be provided by the student code.  It reads the
	 * elements one at a time, and builds a linked list (sorted) as
	 * output.  We close the file when we're done with it.
	 */
	IntList *list = readInput(in);
	fclose(in);

	/* the student must implement a function to get the length of the
	 * list
	 */
	fprintf(out, "%d elements read.\n", getLen(list));

	/* the student must implement a function to print out all of the
	 * contents of the list.
	 */
	writeOutput(list, out);
	fclose(out);

	/* the student must implement a function to free the entire list
	 * before we terminate.
	 */
	freeList(list);

	return 0;
}

