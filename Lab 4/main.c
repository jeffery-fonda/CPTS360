#include "header.h"

int get_block(int fd, int blk, char buf[ ])
{
  lseek(fd, (long)blk*BLKSIZE, 0);
  read(fd, buf, BLKSIZE);
}

int find_file(char *path[])
{
	int i =0, ino = 1;

	while(path[i] != NULL && ino != 0)
	{
		ino = search(ip, path[i]);
		if(ino)
		{
			get_block(fd, (ino - 1) / 8 + inode_table, buf);
			ip = (INODE *)buf + (ino-1) % 8;
			i++;

			if((S_ISDIR(ip->i_mode) && path[i] == NULL) || (S_ISREG(ip->i_mode) && path[i] != NULL))
			{
				printf("\nERROR: not a working path\n");
				return 0;
			}
		}
	}

	return ino;
}

int search(INODE *inode, char *name)
{
	char *cp;
	int i = 0;
	char temp[256];

	for(i; i < 12; i++)
	{
		printf("inode[%d] = %d\n", i, inode->i_block[i]);

		if(inode->i_block[i] == 0)
    {
      return 0;
    }

		get_block(fd, inode->i_block[i], buf);
		cp = dp = (DIR *)buf;

		print_dir(dp);

		while(cp < buf+BLKSIZE)
		{
			memset(temp, 0, 256);
			strncpy(temp, dp->name, dp->name_len);

			if(strcmp(temp, name) == 0)
			{
				return dp->inode;
			}

			cp += dp->rec_len;
			dp = (DIR *)cp;
		}
	}

	return -1;
}

int print_dir(DIR *dir)
{
	char *cp;
	int i =0, j=0;
	DIR *dp = dir;
	cp = dp;

  printf("Working...\n" );

	while(dp->rec_len != 0)
	{
		for(i = 0; i < dp->name_len; i++)
		{
			putchar(dp->name[i]);
		}
		printf(" | ");
		cp += dp->rec_len;
		dp = (DIR *)cp;
	}
	putchar('\n');
}

int printInode(INODE *ip)
{
	int i = 0, num_blocks;
	char buf2[BLKSIZE];

	printf("\n----- INODE CONTENTS -----\n");
	printf("inode->i_mode:\t%d\n", ip->i_mode);
	printf("inode->i_uid:\t%d\n", ip->i_uid);
	printf("inode->i_size:\t%d\n", ip->i_size);
	printf("inode->i_atime:\t%d\n", ip->i_atime);
	printf("inode->i_ctime:\t%d\n", ip->i_ctime);
	printf("inode->i_mtime:\t%d\n", ip->i_mtime);
	printf("inode->i_dtime:\t%d\n", ip->i_dtime);
	printf("inode->i_gid:\t%d\n", ip->i_gid);
	printf("inode->i_links_count:\t%d\n", ip->i_links_count);
	printf("Number of blocks:\t%d\n", (num_blocks = ip->i_size / BLKSIZE + 1));
	getchar();
	printf("\n*********Disk Blocks**********\n");
    for(i = 0; i < 15; i++)
    {
        if(ip->i_block[i] != 0)
        {
            printf("i_block[%d] = %d\n", i, ip->i_block[i]);
        }
    }

    printf("\n-------------Direct Blocks------------\n");
    for(i = 0; i < 12; i++)
    {
        if(ip->i_block[i])
        {
        	num_blocks--;
            if(i == 6)
            {
                printf("\n");
            }
            printf("%d ", ip->i_block[i]);
        }
    }
    printf("Number of blocks remaining:\t%d\n", num_blocks);
    if(ip->i_block[12] && num_blocks > 0)
    {
        printf("\n--------------Indirect Blocks--------------\n");
        //Print indirect blocks
        get_block(fd, ip->i_block[12], buf2);
        int *indirect = (int *)buf2;
        for(i = 0; i < 256; i++)
        {
            if(indirect[i])
            {
            	num_blocks--;
                if(i%10 == 0)
                {
                    printf("\n");
                }
                printf("%d ", indirect[i]);
            }
        }
    }


    printf("Number of blocks remaining:\t%d\n", num_blocks);
    getchar();
    if(ip->i_block[13] && num_blocks > 0)
    {
        printf("\n--------------Double Indirect Blocks---------------\n");
        int j = 0, remaining = 0;
        int buf3[256], buf4[256];
        int *dbl_indirect;

        get_block(fd, ip->i_block[13], buf3);
        printf("13: %d\n", ip->i_block[13]);

        for (i = 0; i < 256; i++)
			{
				if (buf3[i] == 0) break;

				if (i%10 == 0)putchar('\n');

				printf("\t%d: \n", buf3[i]);
				get_block(fd, buf3[i], buf4);
				//dbl_indirect = (int *)buf4;

				for (j = 0; j < 256; j++)
				{
					if (buf4[j])
					{
						//num_blocks--;
						printf("%d ", buf4[j]);
						if (j%10 == 0)
							printf("\n");
					}
					else{
					break;}
				}
				printf("Number of blocks remaining: %d\n", num_blocks);
				getchar();
			}
    }
	printf("\n\n----- INODE CONTENTS -----\n");
}

int split_path(char *PATH[], char *path)
{
	char *token = strtok(path, "/");
	int i = 0;

	PATH[i] = malloc(sizeof(char)*sizeof(strlen(token)));
	strcpy(PATH[i], token);
	i++;
	while((token = strtok(NULL, "/")) != NULL)
	{
		PATH[i] = malloc(sizeof(char)*sizeof(strlen(token)));
		strcpy(PATH[i], token);
	  i++;
	}
	PATH[i] = NULL;
}



void gd()
{
  // read group descriptor block
  get_block(fd, 2, buf);
  gp = (GD *)buf;

	inode_table = gp->bg_inode_table;

	printf("*** GD ***\n");
  printf("bg_block_bitmap = %d\n", gp->bg_block_bitmap);
  printf("bg_inode_bitmap = %d\n", gp->bg_inode_bitmap);

  printf("bg_inode_table = %d\n", gp->bg_inode_table);
  printf("bg_free_blocks_count = %d\n", gp->bg_free_blocks_count);
  printf("bg_free_inodes_count = %d\n", gp->bg_free_inodes_count);
  printf("bg_used_dirs_count = %d\n", gp->bg_used_dirs_count);
}

void super()
{
  // read SUPER block
  get_block(fd, 1, buf);
  sp = (SUPER *)buf;

	printf("*** SUPER ***\n");
 	printf("s_magic = %x\n", sp->s_magic);
	if(sp->s_magic != 0xEF53)
	{
		printf("Not an EXT2 file system.\n");
		exit(1);
	}

	printf("num_inodes: %d\n", sp->s_inodes_count);
	printf("num_blocks: %d\n", sp->s_blocks_count);
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("USE: showblock DEVICE PATHNAME\n");
		exit(1);
	}

	fd = open(argv[1], O_RDONLY);
	if (fd < 0){
	    printf("open failed\n");
	    exit(1);
	}

	// split path
	char *path[64];
	split_path(path, argv[2]);

	//read in superblock
	super();

	//read in gd
	gd();

	get_block(fd, gp->bg_inode_table, buf);
	// get '/'
	ip = (INODE *)buf + 1;

	// search for path
	if(find_file(path))
	{
		printInode(ip);
	}
}
