kapish: kapish.o
	gcc kapish.o -o kapish
kapish.o: kapish.c
	gcc -Wall -Werror -c kapish.c -o kapish.o