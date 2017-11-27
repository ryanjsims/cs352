all: parseConfigFile calculator

parseConfigFile: parseConfigFile.c
	gcc -fprofile-arcs -ftest-coverage -std=c99 -g -Wall parseConfigFile.c -o parseConfigFile


calculator: calculator.c calculator_parser.c expr.h expr_student.h
	gcc -fprofile-arcs -ftest-coverage -std=c99 -D _GNU_SOURCE -g -Wall calculator.c calculator_parser.c -o calculator

clean:
	rm parseConfigFile calculator
