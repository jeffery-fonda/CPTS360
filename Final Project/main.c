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
#include "mount_root.c" //mounts the disk, contains ls
#include "link.c" //link files together
#include "open.c" //open file in preperation of read/write
#include "close.c" //close file after reading/writing
#include "pfd.c" //print file descripter
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
																								if(!strcmp(commands[i], command))
																								{
																																(*functions[i])(pathname);
																																break;
																								}
																}

																if(i == 23)
																								printf("Error: Invalid command\n");

																//reset variables
																strcpy(pathname, "");
																strcpy(command, "");
																strcpy(input, "");
																strcpy(third, "");
								}
								return 0;
}
