#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <ext2fs/ext2_fs.h>
#include <sys/types.h>

typedef unsigned char   u8;
typedef unsigned short u16; 
typedef unsigned int   u32;

typedef struct ext2_group_desc GD;

GD *gd;
char buf[1024];
int fd;
// char *device = "vdisk";
char *device = "/dev/loop16";

int print(char *s, u32 x)
{
    printf("%-30s = %8x\n", s, x);
}


int group_descriptor(char * device)
{
    fd = open(device, O_RDONLY);
    if(fd < 0)
    {
        printf("Open %s failed\n", device);
        exit(2);
    }
    lseek(fd, (long)1024 * 2, 0);
    read(fd, buf, 1024);
    gd = (GD *)buf;

    print("bg_block_bitmap", gd->bg_block_bitmap);
    print("bg_inode_bitmap", gd->bg_inode_bitmap);
    print("bg_inode_table", gd->bg_inode_table);
    print("bg_free_blocks_count", gd->bg_free_blocks_count);
    print("bg_free_inodes_count", gd->bg_free_inodes_count);
    print("bg_used_dirs_count", gd->bg_used_dirs_count);
}

int main(int argc, char *argv[])
{
    if(argc > 1)
        device = argv[1];
    group_descriptor(device);
}