#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

typedef struct ext2_super_block SUPER;

SUPER *sp;
char buf[1024];
int fd, blksize, inodesize;

int print(char *s, u32 x)
{
    printf("%-30s = %8d\n", s, x);
}

int super(char *device)
{
    fd = open(device, O_RDONLY);
    if(fd < 0)
    {
        printf("open %s failed", device);
        exit(1);
    }
    lseek(fd, (long)1024 * 1, 0);
    read(fd, buf, 1024);
    sp = (SUPER *)buf;

    printf("%-30s = %8x ", "s_magic", sp->s_magic);
    if(sp->s_magic != 0xEF53)
    {
        printf("Not an EXT2 FS\n");
        exit(2);
    }

    printf("EXT2 FS OK\n");
}

char *device = "mydisk";

int main(int argc, char *argv[]) 
{
    if(argc > 1)
        device  = argv[1];
    super(device);
    return 0;
}


