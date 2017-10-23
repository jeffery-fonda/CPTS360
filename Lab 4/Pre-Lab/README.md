Concerning the ext2_fs.h:  

When using the command:  

	sudo apt-get install ext2fs-dev  

The file should have been installed under the /usr/include/linux directory, however, that didn't happen.  

Included is this folder is the ext2_fs.h file. To install:  

1. Open a terminal.  
2. Run command sudo -s  
3. Enter password  
4. Run command nautilus, this will open a new "Files" window with superuser permissions  
5. Copy and paste the ext2_fs.h file in the /usr/include/linux directory  

Program should now compile.
