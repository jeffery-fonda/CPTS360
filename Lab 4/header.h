#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

typedef unsigned int u32;

// define shorter TYPES, save typing efforts
typedef struct ext2_group_desc  GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;    // need this for new version of e2fs

GD    *gp;
SUPER *sp;
INODE *ip;
DIR   *dp;

#define BLKSIZE 1024

char buf[BLKSIZE];
int fd, inode_table;
