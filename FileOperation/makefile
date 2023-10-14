CC = gcc
CFLAGS = -w -m32
OBJS = superBlock.c

make_out:
	$(CC) $(CFLAGS) -o a.out $(OBJS)
check: make_out
	./a.out

clean: 
	rm -f a.out