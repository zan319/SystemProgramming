CC = gcc
CFLAGS = -w -m32
OBJS1 = superBlock.c
OBJS2 = groupDescriptor.c
OBJS3 = inode.c
OBJS4 = bitmap.c
OBJS5 = myprintf.c
OBJS6 = entry.c

_compile_super:
	$(CC) $(CFLAGS) -o superblock.out $(OBJS1)
_check_super: _compile_super
	./superblock.out
_clean_super: 
	rm -f superBlock.out

_compile_GD:
	$(CC) $(CFLASS) -o groupdescriptor.out $(OBJS2)
_check_GD: _compile_GD
	./groupdescriptor.out
_clean_GD:
	rm -f groupdescriptor.out

_compile_inode:
	$(CC) $(CFLASS) -o inode.out $(OBJS3)
_check_inode: _compile_inode
	./inode.out
_clean_inode:
	rm -f inode.out

_compile_bitmap:
	$(CC) $(CFLASS) -o bitmap.out $(OBJS4)
_check_bitmap: _compile_bitmap
	./bitmap.out
_clean_bitmap:
	rm -f bitmap.out

_compile_myprintf:
	$(CC) $(CFLASS) -o myprintf.out $(OBJS5)
_check_myprintf: _compile_myprintf
	./myprintf.out
_clean_myprintf:
	rm -f myprintf.out

_compile_entry:
	$(CC) $(CFLASS) -o entry.out $(OBJS6)
_check_entry: _compile_entry
	./entry.out
_clean_entry:
	rm -f entry.out