CC = gcc
CFLAGS = -w -m32
OBJS1 = superBlock.c
OBJS2 = groupDescriptor.c
OBJS3 = inode.c
OBJS4 = bitmap.c

_compiler_super:
	$(CC) $(CFLAGS) -o superblock.out $(OBJS1)
_check_super: _compiler_super
	./superblock.out
_clean_super: 
	rm -f superBlock.out

_compiler_GD:
	$(CC) $(CFLASS) -o groupdescriptor.out $(OBJS2)
_check_GD: _compiler_GD
	./groupdescriptor.out
_clean_GD:
	rm -f groupdescriptor.out

_compiler_inode:
	$(CC) $(CFLASS) -o inode.out $(OBJS3)
_check_inode: _compiler_inode
	./inode.out
_clean_inode:
	rm -f inode.out

_compiler_bitmap:
	$(CC) $(CFLASS) -o bitmap.out $(OBJS4)
_check_bitmap: _compiler_bitmap
	./bitmap.out
_clean_bitmap:
	rm -f bitmap.out

