CC=gcc
#CFLAGS=-Wall -O3 -std=gnu99
CFLAGS=-Wall -g -std=gnu99



all: testcases mergeSort

testcases: test_dblList_01_allocFree
testcases: test_dblList_02_addAfter
testcases: test_dblList_03



# see http://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html 
#
# Also, note the new gcc syntax.  We're combining .c files and .o
# files on the same line.

mergeSort: mergeSort.c encapsulatedListStr.h encapsulatedListStr.o
	$(CC) $(CFLAGS) $^ -o $@



# 'make' allows you to write little rules which define the target/dependency
# relationships, but which do not actually add any new build rules.

dblListInt.o: dblListInt.c
	$(CC) $(CFLAGS) -c $^ -o $@
encapsulatedListStr.o: encapsulatedListStr.c
	$(CC) $(CFLAGS) -c $^ -o $@
test_dblList_01_allocFree: test_dblList_01_allocFree.c dblListInt.o dblListInt.h
	$(CC) $(CFLAGS) $^ -o $@
test_dblList_02_addAfter: test_dblList_02_addAfter.c dblListInt.o dblListInt.h
	$(CC) $(CFLAGS) $^ -o $@
test_dblList_03: test_dblList_03.c dblListInt.o dblListInt.h
	$(CC) $(CFLAGS) $^ -o $@
test_dblList_04: test_dblList_04.c dblListInt.o dblListInt.h
	$(CC) $(CFLAGS) $^ -o $@
test_dblList_05: test_dblList_05.c dblListInt.o dblListInt.h
	$(CC) $(CFLAGS) $^ -o $@


clean:
	-rm *.o test_dblList_01_allocFree test_dblList_02_addAfter test_dblList_03_swapWithNext mergeSort


