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
u32 fd, firstdata, inodesize, blksize, iblock;

char *device = "/dev/loop16";
// char *device = "mydisk";
int get_block(int fd, int blk, char *buf)
{
    lseek(fd, blk * BLKSIZE, SEEK_SET);
    read(fd, buf, BLKSIZE);
}

int inode(char *dev)
{
    int i;
    fd = open(dev, O_RDONLY < 0);
    if(fd < 0)
    {
        printf("Open file %s failed",dev);
        exit(1);
    }
    get_block(fd, 1, buf);
    sp = (SUPER *)buf;
    inodesize = sp->s_inode_size;
    blksize = 1024 * (1 << sp->s_log_block_size);
    firstdata = sp->s_first_data_block;

    printf("first_data_block = %u\nblock_size = %u\ninode_size = %u\n", 
                                                firstdata, blksize, inodesize);

    get_block(fd, (firstdata + 1), buf);
    gp = (GD *)buf;

    printf("block_bitmap = %u\ninode_bitmap = %u\ninode_table = %d\n",
            gp->bg_block_bitmap, gp->bg_inode_bitmap, gp->bg_inode_table);
    printf("free_block =%d \nfress_inodes = %d \n", 
                    gp->bg_free_inodes_count, gp->bg_used_dirs_count);

    iblock = gp->bg_inode_table;
    printf("root inode information:\n");
    printf("-----------------------------------------------\n");
    printf("size of inode = %u", sizeof(struct ext2_inode));
    get_block(fd, iblock, buf);
    ip = (INODE *)buf + 1;//ip point at #2 INODE
    printf("mode = %4x\n",ip->i_mode);
    printf("uid = %u, gid = %u\n",ip->i_uid, ip->i_gid);
    printf("size = %u\n", ip->i_size);
    printf("ctime = %s", ctime(&ip->i_ctime));
    printf("links = %u\n", ip->i_links_count);
    for(i = 0; i < 15; i++)
    {
        if(ip->i_block[i])
        {
            printf("i_blocks[%d] = %u\n", i, ip->i_block[i]);
        }
    }
}

int main (int argc, char *argv[])
{
    if(argc > 1)
        device = argv[1];
    inode(device);
}

