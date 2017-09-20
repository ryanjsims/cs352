#ifndef __PROJ04_H__INCLUDED__
#define __PROJ04_H__INCLUDED__


struct Foo
{
	int  arr[3];
	unsigned char c1,c2;
};
typedef struct Foo Foo;


struct Bar
{
	int  x,y,z;
	unsigned char c1,c2;
};
typedef struct Bar Bar;


struct Baz
{
	int  x;
	unsigned char c1;
	int  y;
	unsigned char c2;
	int  z;
};
typedef struct Baz Baz;



/* you must implmement these functions  in structPacking.c */

int fillOneFoo(Foo*);

void printInts(int *, int count);

void printOneBar(Bar*);
void printOneBaz(Baz*);


#endif

