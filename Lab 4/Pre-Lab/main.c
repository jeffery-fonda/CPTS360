// Pre-Lab for Lab 4
// 1. Write a C program to display the group descriptor of an EXT2 file system on
//    a device.
// 2. Modify the imap.c program to print the inodes bitmap in char map form, i.e.
//    for each bit, print a „0‟ if the bit is 0, print a „1‟ if the bit is 1.
// 3. Write a C program to display the blocks bitmap of an Ext2 file system, also
//    in char map form.
// 4. Write a C program to print the dir_entries of a directory.
// 5. Mount mydisk under Linux. Create new directories and copy files to it, then
//    umount is. Run the dir.c program on mydisk again to see the outputs.

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <linux/ext2_fs.h>
#include <time.h>

// define shorter TYPES
typedef struct ext2_group_desc GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode INODE;
typedef struct ext2_dir_entry DIR; //ext2_dir_entry_2 doesn't exist in the ext2_fs.h file

// define pointer variables
GD    *gp;
SUPER *sp;
INODE *ip;
DIR   *dp;

int block_size;
int inode_table;
int inode_size;
int first_block;
int inodes_per_block;
int inode_bitmap;
int block_bitmap;
int block_offset;
int inode_count;
int inode_offset;

int get_block(int fd, int blk, char *buf)
{
        lseek(fd, (long)(blk*1024), 0);
        read(fd, buf, 1024);
}

int super(int fd) //from reading
{
        char buf[1024];

        // read SUPER block at byte offset 1024
        get_block(fd, 1, buf);
        sp = (SUPER *)buf;

        // print out fields of SUPER block
        printf("\n******************* superblock *******************\n");
        printf("s_inodes_count \t\t=\t%d\n", sp->s_inodes_count);
        printf("s_blocks_count \t\t=\t%d\n", sp->s_blocks_count);
        printf("s_free_inodes_count \t=\t%d\n", sp->s_free_inodes_count);
        printf("s_free_blocks_count \t=\t%d\n", sp->s_free_blocks_count);
        printf("s_log_block_size \t=\t%d\n", sp->s_log_block_size);
        printf("s_blocks_per_group \t=\t%d\n", sp->s_blocks_per_group);
        printf("s_inodes_per_group \t=\t%d\n", sp->s_inodes_per_group);
        printf("s_mnt_count \t\t=\t%d\n", sp->s_mnt_count);
        printf("s_max_mnt_count \t=\t%d\n", sp->s_max_mnt_count);
        printf("s_magic \t\t=\t%x\n", sp->s_magic);
        printf("s_mtime \t\t=\t%s", ctime(&(sp->s_mtime)));
        printf("s_inode_size \t\t=\t%d\n", sp->s_inode_size);
        inode_count = sp->s_inodes_count;
        block_size = 1024 << sp->s_log_block_size;
        inode_size = sp->s_inode_size;
        printf("\n**************************************\n");
}

int gd (int fd) //group descriptor
{
        char buf[1024];

        // read GD block at byte offset 1024
        get_block(fd, 2, buf);
        gp = (GD *)buf;

        // check EXT 2 FS magic number:
        printf("\n**************** group descriptor ****************\n");
        printf("bg_block_bitmap \t=\t%d\n", gp->bg_block_bitmap);
        block_bitmap = gp->bg_block_bitmap;
        printf("bg_inode_bitmap \t=\t%d\n", gp->bg_inode_bitmap);
        inode_bitmap = gp->bg_inode_bitmap;
        printf("bg_inode_table \t\t=\t%d\n", gp->bg_inode_table);
        inode_table = gp->bg_inode_table;
        printf("bg_free_blocks_count \t=\t%d\n", gp->bg_free_blocks_count);
        printf("bg_free_inodes_count \t=\t%d\n", gp->bg_free_inodes_count);
        printf("bg_used_dirs_count \t=\t%d\n", gp->bg_used_dirs_count);
        printf("\n**************************************\n");
}

int bmap(int fd) //bitmaps
{
        char buf[1024];
        char bits[8192];
        unsigned char bitmap[block_size];
        uint i = 0, j, k;
        get_block(fd, 8, bitmap);
        printf("\n**************** bmap ****************\n");
        while (i < 1024)
        {
                bitmap [i] = bitmap[i]/8;
                bits[j] = bitmap[i] % 8;

                if (buf[i] & (1<<bits[j]))
                {
                        printf("1");
                }
                else
                {
                        printf("0");
                }
                i++;

                if((i%8) == 0)
                {
                        printf(" ");
                }
                if ((i%80) == 0)
                {
                        printf("\n");
                }
        }
        printf("\n**************************************\n");
}

int imap(int fd) //inode map
{
        char buf[1024];
        char bits[8192];
        unsigned char bitmap[inode_size];
        uint i = 0, j, k;
        get_block(fd, 9, buf);
        printf("\n**************** imap ****************\n");
        while (i < 160)
        {
                bitmap [i] = bitmap[i]/8;
                bits[j] = bitmap[i] % 8;

                if (buf[i] & (1<<bits[j]))
                {
                        printf("1");
                }
                else
                {
                        printf("0");
                }
                i++;

                if((i%8) == 0)
                {
                        printf(" ");
                }
                if ((i%80) == 0)
                {
                        printf("\n");
                }
        }
        printf("\n**************************************\n");
}

int inode(int fd)
{
        int i = 0, j;
        char buf[1024];

        get_block(fd, block_size*inode_table + inode_size, buf);

        ip = (INODE *)buf;

        printf("\n\n**************** inode ****************\n");
        printf("inode_block=%d\n", inode_count / 8);

        printf("mode=%x\n", ip->i_mode);
        printf("uid=%d\n", ip->i_uid);
        printf("gid=%d\n", ip->i_gid);
        printf("size=%d\n", ip->i_size);
        printf("time=%s", ctime(&(ip->i_ctime)));
        printf("link=%d\n", ip->i_links_count);
        printf("i_block[0]=%d\n", ip->i_block[i]);
        printf("\n**************************************\n");
}

int dir(int fd)
{
        char buf[1024];

        dp = (DIR *)buf;
        char *cpp = buf;

        get_block(fd, 10, buf);
        printf("\n**************** directories ****************\n");
        printf("%d %d %s", dp->inode, dp->rec_len, dp->name);
        printf("\n**************************************\n");
}

char *device = "mydisk";

main(int argc, char *argv[])
{
        int fd;

        if (argc > 1)
        {
                device = argv[1];
        }

        fd = open(device, O_RDONLY);
        if (fd < 0)
        {
                printf("open %s failed\n", device);
                exit(1);
        }
        gd(fd);     // 1. prints group descriptor
        inode(fd);  // 2. print inodes
        bmap(fd);   // 3. print bitmaps
        dir(fd);    // 4. print dir

        super(fd);  // extra
        imap(fd);   // extra
}
