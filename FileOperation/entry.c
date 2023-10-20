#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>
#include <string.h>

#define BLKSIZE 1024

typedef unsigned char   u8;
typedef unsigned short u16;
typedef unsigned int   u32;

typedef struct ext2_group_desc     GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2   DIR;

SUPER *sp;
GD    *gp;
INODE *ip;
DIR   *dp;
char buf[BLKSIZE];
char tmp[256];
char *cp;
int fd;
u32 firstdata, inodesize, blksize, iblock, itable;

//char *device = "mydisk";
char *device = "/dev/loop16";

int get_block(int fd, int blk, char *buf)
{
    lseek(fd, BLKSIZE * blk, SEEK_SET);
    read(fd, buf, BLKSIZE);
}

int print(char *s, u32 x)
{
    printf("%-30s = %8x\n", s, x);
}

int main()
{
    fd = open(device, O_RDONLY);
    if(fd < 0)
    {
        printf("Open file %s failed", device);
        exit(1);
    }
    get_block(fd, 1, buf);
    sp = (SUPER *)buf;
    printf("%-30s = %8x\n", "s_magic", sp->s_magic);
    if(sp->s_magic != 0xEF53)
    {
        printf("Not an EXT2 FS\n");
        exit(2);
    }
    firstdata = sp->s_first_data_block;
    blksize = 1024 * (1 << sp->s_log_block_size);
    inodesize = sp->s_inode_size;
    printf("first_data_block = %u\nblksize = %u\ninodesize = %u\n", firstdata, blksize, inodesize);
    get_block(fd, firstdata + 1, buf);
    gp = (GD *)buf;


    itable = gp->bg_inode_table;
    get_block(fd, itable, buf);
    ip = (INODE *)buf + 1;
    iblock = ip->i_block[0];
    printf("inode_block = %d\n", iblock);
    get_block(fd, iblock, buf);
    dp = (DIR *)buf;
    cp = buf;
    printf("inode#  name_len  rec_len    name\n");
    while(cp < buf + BLKSIZE)
    {
        strncpy(tmp, dp->name, dp->name_len);
        tmp[dp->name_len] = 0;
        printf("%6d %7d %6d  %10s\n",dp->inode, dp->name_len, dp->rec_len, tmp);
        cp += dp->rec_len;
        dp = (DIR *)cp;
    }
}

