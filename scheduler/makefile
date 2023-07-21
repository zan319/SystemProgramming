CC = gcc
CFLAGS = -m32 -w
OBJS = schedule.c switch.s

make_out: queue.c type.h 
	$(CC) $(CFLAGS) -o a.out $(OBJS)
check: make_out
	./a.out

clean: 
	rm -f a.out
