CC=gcc
CFLAGS=-Wall -Werror

kapish: dll.o kapish.o
	$(CC) $(CFLAGS) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $^
clean:
	rm kapish *.o