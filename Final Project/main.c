#include <stdio.h>
#include <stdlib.h>

#include "type.h" //header from textbook
#include "util.c" //helper functions such as get_block, put_block, iget, tokenize, getino, iput etc.
#include "alloc_dealloc.c" //allocation and deallocation of memory for inodes and blocks
#include "pwd.c" //my_pwd function
#include "mkdir.c" //my_mkdir function, includes helper functions
#include "rmdir.c" //my_rmdir function, includes helper functions
#include "creat_file.c" //my_creat function for creation of files
#include "chmod_file.c" //change mode function
#include "touch_file.c" //simple file creation
#include "mount_root.c" //mounts the disk, contains ls and cd
#include "link.c" //link files together
#include "open.c" //open file in preperation of read/write
#include "close.c" //close file after reading/writing
#include "pfd.c" //print file descripter of open files
#include "symlink.c" //links to another file sort of like a shortcut
#include "unlink.c" //removes links and deletes files
#include "stat.c" //print stats about file or dir
#include "cp.c" //copy file from location to location
#include "mv.c" //move file from location to location
#include "lseek.c" //changes the file offset for the open fd
#include "read.c" //reads the contents of a file
#include "write.c" //writes to files
#include "cat.c" //adds contents to file

/*
   LEVEL 1:
   mount_root;
   mkdir, rmdir, ls, cd, pwd;
   creat, link, unlink, symlink
   stat, chmod, touch;

   LEVEL 2:
   open, close, read, write
   lseek, cat, cp, mv

   LEVEL 3:
   mount, unmount
   File permission checking

   Simple EXT2 File System:
 |	 0		  1		   2		 3		  4	 | 5................49 |	50...............|
 | Boot | Super | GD | BMAP | IMAP | Inodes Blocks...... | data blocks...... |

   Each block group contains the following blocks:
    Boot (B0): is the boot block, which is not used by the file system.
    Superblock (B1): contains information about the entire file system.
        s_indoes_count - total number of indodes
       s_blocks_count - total number of blocks
       s_r_blocks_count
       s_free_blocks_count - current number of free blocks
       s_free_inodes_count - current number of free inodes
       s-magic - for checking file system, should be 0xEF53 for EXT2
    Group Descriptor (B2): EXT2 divides disk blocks into groups, each group contains 8192 blocks.
        bg_block_bitmap - Bmap block number
       bg_inode_bitmap - Imap block number
       bg_inode_table - inodes begin block number
       bg_free_blocks_count
       bg_free_inodes_count
       bg_used_dirs_count
    Bmap (B3): bitmap is a sequence of bits used to represent some kind of items e.g. disk blocks or Inodes
        0 means the bit is free
       1 means the bit is in use
    Inode Bmap (B4): data structure used to represent a file
    Inodes (B5): every file is represented by a unique inode structure of 128 bytes
        i_mode - 16 bits = |tttt|ugs|rwx|rwx|rwx|
       i_uid - owner user id
       i_size - file size in bytes
       i_atime - time fields in seconds
       i_ctime
       i_mtime
       i_dtime
       i_gid - group id
       i_links_count - hard link counts
       i_blocks - number of 512 byte sectors
 */

void menu() //displays the available commands
{
								printf("Level 1 COMMANDS:\n\n");
								printf("	mkdir\t\trmdir\t\tls\t\tcd\n");
								printf("	cd\t\tpwd\t\tcreat\t\tlink\n");
								printf("	unlink\t\tsymlink\t\tstat\t\tchmod\n");
								printf("	touch\t\tquit\t\tmenu\n\n");
								printf("Level 2 COMMANDS:\n\n");
								printf("	open\t\tclose\t\tread\t\twrite\n");
								printf("	lseek\t\tcat\t\tcp\t\tmv\n");
}

int main()
{
								char *commands[23] = {"mkdir", "rmdir", "ls", "cd", "pwd", "creat", "link", "unlink", "symlink", "stat", "chmod", "touch", "quit", "menu", "open", "close", "pfd", "write", "cat", "read", "cp", "mv", "lseek"};
								char device_name[64], input[128] = "";
								char command[64], pathname[64] = "";

								int (*functions[23]) (char path[]);
								int i;

								//set the function at index n to the name of the function in the specific file
								//program then goes through the functions array until it find the command that has been inputted
								//once the command has been found, it executes the defined function with the correct passed through values
								//all functions take at least one additional variable, path
								//path is used because file_name is obnoxious to type over and over again
								functions[0] = make_dir; //works
								functions[1] = remove_dir; //works
								functions[2] = ls; //works
								functions[3] = cd; //works

								functions[4] = my_pwd; //works
								functions[5] = creat_file; //works
								functions[6] = my_link; //works
								functions[7] = my_unlink; //works

								functions[8] = my_symlink; //works
								functions[9] = my_stat; //works
								functions[10] = chmod_file; //kind of works
								functions[11] = touch_file; //works
								functions[12] = quit; //works
								functions[13] = menu; //works
								functions[14] = open_file; //works
								functions[15] = my_close; //works
								functions[16] = my_pfd; //works
								functions[17] = do_write; //works
								functions[18] = my_cat; //works
								functions[19] = read_file; //works
								functions[20] = cp_file; //segmentation fault
								functions[21] = mv_file; //the file dissapears
								functions[22] = my_lseek; //works


								printf("Enter device name: "); //prompt user for device
								scanf("%s", device_name); //scan in device name

								//mount device
								init();
								mount_root(device_name);

								getchar();
								while(1) //run program forever until quit
								{
																printf("jfonda@ubuntu:~$ "); //terminal look
																fgets(input, 128, stdin);

																//remove \r at end of input
																input[strlen(input) - 1] = 0;
																if(input[0] == 0)
																								continue;

																//split up the input into the variables
																sscanf(input, "%s %s %s", command, pathname, third);

																for(i = 0; i < 23; i++)
																{
																								//commands is list of commands, command is the inputted command
																								if(!strcmp(commands[i], command))
																								{
																																(*functions[i])(pathname); //pathname is just file name
																																break; //break once command is found and executed
																								}
																}

																if(i == 23) //if no command has been found, then it doesn't exist
																								printf("Error: Invalid command!\n");

																//reset variables
																strcpy(pathname, "");
																strcpy(command, "");
																strcpy(input, "");
																strcpy(third, "");
								}
								return 0;
}
