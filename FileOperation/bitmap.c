#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>
#include <sys/types.h>

typedef struct ext2_super_block SUPER;
typedef struct ext2_group_desc GD;

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
#define BLKSIZE 1024

SUPER *sp;
GD    *gp;

char buf[BLKSIZE];
int fd;
// char *device = "mydisk";
char *device = "/dev/loop16";
int get_block(int fd, int blk, char *buf)
{
    lseek(fd, (long)BLKSIZE * blk, 0);
    read(fd, buf, BLKSIZE);
}

int imap(char *device)
{
    u32 i, ninodes, blksize, imapblk;
    fd = open(device, O_RDONLY);
    if(fd < 0)
    {
        printf("Open file %s failed",device);
        exit(1);
    }
    get_block(fd, 1, buf);
    sp = (SUPER *)buf;

    ninodes = sp->s_inodes_count;
    printf("inodes_count = %u\n",sp->s_inodes_count);

    get_block(fd, 2, buf);
    gp = (GD *)buf;

    imapblk = gp->bg_block_bitmap;

    printf("imapblk = %d\n",imapblk);
    get_block(fd, imapblk, buf);
    for (i = 0; i <= ninodes/8; i++) {
        printf("%02x ", (u8)buf[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    if(argc > 1)
        device = argv[1];
    imap(device);
}