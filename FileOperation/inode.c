#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>
#include <sys/types.h>
#include <time.h>

#define BLKSIZE 1024

typedef unsigned char   u8;
typedef unsigned short u16;
typedef unsigned int   u32;

typedef struct ext2_group_desc  GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;

SUPER *sp;
GD    *gp;
INODE *ip;
DIR   *dp;
char buf[BLKSIZE];
int fd, firstdata, inodesize, blksize, iblock;

char *dev = "mydisk";

int get_block(int fd, int blk, char *buf)
{
    lseek(fd, blk * BLKSIZE, 0);
    read(fd, buf, BLKSIZE);
}

int inode(char *dev)
{
    int i;
    fd = open(dev, O_RDONLY);
    if(fd < 0)
    {
        printf("Open file %s failed",dev);
        exit(1);
    }
    get_block(fd, 1, buf);
    gp = (GD *)buf;
    inodesize = sp->s_inode_size;
    blksize = 1024 * (1 << sp->s_log_block_size);
    firstdata = sp->s_first_data_block;

    printf("first_data_block = %d\nblock_size = %d\ninode_size = %d", 
                                                firstdata, blksize, firstdata);

    get_block(fd, (firstdata + 1), buf);
    gp = (GD *)buf;

    printf("block_bitmap = %d\ninode_bitmap = %d\n, inode_table = %d\n",
            gp->bg_block_bitmap, gp->bg_inode_bitmap, gp->bg_inode_table);

    iblock = gp->bg_inode_table;
    printf("root inode information: %d\n", iblock);
    printf("-----------------------------------------------\n");
    get_block(fd, iblock, buf);
    ip = (INODE *)buf + 1; //ip point at #2 INODE
    printf("mode = %4x\n",ip->i_mode);
    printf("uid = %d, gid = %d\n",ip->i_uid, ip->i_gid);
    printf("size = %d\n", ip->i_size);

    printf("ctime = %s", ctime(&ip->i_ctime));
    printf("links = %d\n", ip->i_links_count);
    for(i = 0; i < 15; i++)
    {
        if(ip->i_block[i])
        {
            printf("i_blocks[%d] = %d\n", i, ip->i_block[i]);
        }
    }
}

int main (int argc, char *argv[])
{
    inode(dev);
}

