1. OBJECTIVES
   C programming; pointers, link-lists, trees

2. BACKGROUND (Unix File System Tree)

The logical organization of a Unix file system is a general tree, as shown by
the following diagram.


                     ---------------- / ---------------
                     |                |               |
               ----  a ---         -- b ---           c
              |          |        |   |   |           |
              b      --- c ---    d   e   f           g
                     |   |   |   
                     h   i   j

For simplicity, we shall assume that the tree contains only DIRs and FILEs.
The root directory is represented by the symbol /. A DIR node may have a 
variable number of children nodes. Children nodes of the same parent are called 
siblings. In a Unix file system, each node is represented by a unique pathname 
of the form /a/b/c or a/b/c. A pathname is absolute if it begins with /, 
indicating that it starts from the root. Otherwise, it is relative to the 
Current Working Directory (CWD). 

A general tree can be implemented as a binary tree. For each node, let childPtr
point to the oldest child, and let siblingPtr point to the oldest sibling. For 
convenience, each node also has a parentPtr pointing to its parent node. For the
root node, both parentPtr and siblingPtr point to itself.


3. REQUIREMENTS:

3-1. Write a C program to simulate the Unix file system tree.

3-2. Your program should work as follows:
     (1). Start with a / node, set both root, CWD point at the root node. 
     (2). Prompt the user for a command:
           mkdir, rmdir, cd, ls, pwd, creat, rm, save, reload, menu, quit
     (3). Execute the command, with appropriate tracing messages.
     (4). Repeat (2) until the "quit" command.

3-3. Commands Specification:

     mkdir  pathname  : make a new directory for a given pathname 
     rmdir  pathname  : rm the directory, if it is empty.
     cd    [pathname] : change CWD to pathname, or to / if no pathname.
     ls    [pathname] : list the directory contents of pathname or CWD
     pwd              : print the (absolute) pathname of CWD
     creat  pathname  : create a FILE node.
     rm     pathname  : rm a FILE node.
     save   filename  : save the current file system tree in a file
     reload filename  : re-initialize the file system tree from a file
     menu             : show a menu of valid commands
     quit             : save the file system tree, then terminate the program.

                    4. HELP INFO:
==============================================================================
1. NODE type:
     Define a C struct for the NODE type:
          64 chars      : name string of the node;
          char          : node type: 'D' for DIR or 'F' for FILE
          node pointers : *childPtr, *siblingPtr, *parentPtr;

2. Global Variables:
   NODE *root, *cwd;                             /* root and CWD pointers */
   char line[128];                               /* user input line */
   char command[16], pathname[64];               /* user inputs */
   char dirname[64], basename[64];               /* string holders */
   (Others as needed)

================= find command: return an index ========================
char *cmd[] = {"mkdir", "rmdir", "ls", "cd", "pwd", "creat", "rm",
               "reload", "save", "menu", "quit", 0};

int findCmd(char *command)
{
   int i = 0;
   while(cmd[i]){
     if (strcmp(command, cmd[i])==0)
         return i;
     i++;
   }
   return -1;
}

int index = findCmd(cmdString);  
e.g. cmdString = "creat" ==> index = 5.
======================================================================

3. main()
   {
      int index;  
      initialize(); // initialize root node /, set both root, cwd point at /
      while(1){
         printf("input a commad : ");
         read a line containting  command [pathname]; // [ ] means optional
         Find the command string and call the corresponding ACTION function;
         Example: 
              index = findCommand(command)
              switch(index){ 
                  case 0 : mkdir(pathname);    break;
                  case 1 : rmdir(pathname);    break;
                  case 2 : ls(pathname);       break;
                  case 3 : cd(pathname);       break;
                  case 4 : pwd(cwd);           break;
                   ...................
              }
      }
   }

   If the number of commands is small, e.g. only a few, it is OK to use a
   switch table, which is much better than a lot of if-else-if statements. 
   If the number of commands is large, it is better to use a table of
   FUNCTION POINTERs. Listen to the lecture on function pointers.

4. Action Functions: 

               HOW TO mkdir pathname:

   int mkdir(char *pathname)
   {
     (1). Break up pathname into dirname and basename, e.g.
          ABSOLUTE: pathname=/a/b/c/d. Then dirname=/a/b/c, basename=d
          RELATIVE: pathname= a/b/c/d. Then dirname=a/b/c,  basename=d
          
     (2). Search for the dirname node:
          ASSOLUTE pathname: search from /
          RELATIVE pathname: search from CWD.

                if nonexist ==> error messages and return
                if exist but not DIR ==> error message and return
               
     (3). (dirname exists and is a DIR):
           Search for basename in (under) the dirname node:
                if already exists ==> error message and return;
   
           ADD a new DIR node under dirname.
   }


5. Other Action Functions

5-1. HOW TO rmdir pathname:

(1). if pathname is absolute, start = /
     else                     start = CWD --> current DIR node
(2). search for pathname node:
        tokenize pathname into components;
        beginning from start, search for each component;
        return ERROR if fails
(3). pathname exists: 
        check it's a DIR type;
        check DIR is empty; can't rmdir if NOT empty;
(4). delete node from parent's child list;
      
5-2. HOW TO creat pathname
     SAME AS mkdir except that the node type is F

5-3. HOW TO rm pathname
     SAME AS rmdir except check it's a file, do not need to check for EMPTY.

      
5-4. HOW TO cd pathname
(1). find pathname node;
(2). check it's a DIR;
(3). change CWD to point at DIR

5-5. HOW TO ls pathname
(1). find pathname node
(2). list all children nodes in the form  [TYPE  NAME]  [TYPE  NAME],...
                                          ..........  

5-6. HOW TO pwd
/***********************************************************************
 rpwd() actually prints CWD's pathname
 **********************************************************************/
int rpwd(NODE *p)
{
  WRITE YOUR C CODE HERE
} 
/*************************************************************************
 pwd() prints the full pathname of CWD. call rpwd(cwd) to do it recursively.
 *************************************************************************/
int pwd()
{
   rpwd(cwd);
   printf("\n");
} 

                 HOW TO save tree to a FILE

FILE *fp = fopen("myfile", "w+");           // open a FILE stream for WRITE

fprintf(fp, "%c %s", 'D', "namestring\n");  // print a line to file by FORMAT
fprintf(fp, "%c %s", 'F', "namestring\n");  // print a line to file by FORMAT
 
fclose(fp);                                 // close FILE stream when done


6. save filename: Assume the file system tree is (shown on lefthand side)

       (tree)                      (binday tree)
         /                              /    
    ------------                        |       
    |     |    |                        |
    A     B    C                        A ===> B ==> C
   ---    |    |                        |      |     | 
   | |    z    E                        |      z     E 
   x y                                  x ==> y
   
   where A,B,C,D are DIRs and lower case names are FILEs
 
   The (binary) tree can be represented by the (text) lines
      type      path
      ----- --------------
       D         /
       D         /A
       F         /A/x
       F         /A/y
       D         /B
       F         /B/z
       D         /C
       D         /C/E
---------------------------------------------------------------------
   The paths are generated by PRE-ORDER traversal of the binary tree: 
       print node       name;       // node 
       print node.left  name;       // LEFT  node  (by childPtr)
       print node.right name;       // RIGHT node  (by siblingPtr)

What are IN-ORDER traversal? POST-ORDER traversal?
--------------------------------------------------------------------
   save(filename) writes the lines to a file.

7. reload(filename)
   Read each line from a (saved) file and re-creates the file system tree:

   Example: initialize root node as empty tree;
            read a line from file:
            line = "D pathname" ==> get type D and pathname, then
                   mkdir(pathname);
            
            line = "F pathname" ==> get type F and pathname, then
                   creat(pathname);
           
========================== MORE HELPS ========================================
1. Learn how to read Linux man pages:
    
   /usr/man/------- man1 : commonly used commands: ls, cat, mkdir ....
               |--- man2 : system calls
               |--- man3 : library functions: strtok, strcat, basename, dirname
                 etc.

  Examples: man ls     ==> show man pages of ls in man1
            man open   ==> show man page of open in man2
            man strtok ==> show man page of strtok in man 3, etc.
            man 3 dirname: show dirname in man3, NOT that of man1
 
2. gets(char s[128]);         // input a string, MAY overrun s[128]

   fgets(s[128], 128, stdin); // input at most 128 chars BUT has \n at end
   s[strlen(s)-1] = 0;        // kill the \n at end.

--------------------------------------------------------------------------
Assume: line[128] contains "mkdir /a/b/c/d"
HOW TO extract token strings from line[ ]:

    char cmd[32], pathname[64];
    sscanf(line, "%s %s", cmd, pathname); // similar to scanf() but from line[ ]
    
Assume char line[128];
HOW TO write items to line[ ]:

    sprintf(line, "%c %s %d", 'D',"namestring", 1234);
-------------------------------------------------------------------------

3. #include <libgen.h>
   
   char pathname[128], temp[128];
   char dname[64], bname[32];

   gets(pathname);                 // assume pathname = "/a/b/c"

   strcpy(temp, pathname);
   strcpy(dname, dirname(temp));   // dname="/a/b"

   strcpy(temp, pathname);
   strcpy(bname,basename(temp));   // bname="c"

4. Assume pathname="/a/b/c/d"
   s = strtok(pathname, "/");     // break up "a/b/c/d" into tokens
  
   printf("s = %s\n", s);         // print first token

   while(s = strtok(0, "/")){     // keep calling strtok() with NULL string
     printf("s = %s\n", s);       // until it return 0 pointer
   }

5. Function Pointers: Assume:

       int mkdir(char *pathname){..........}
       int rmdir(char *pathname){..........}
       etc.

are FUNCTIONS in your program.
                                       0     1   2   3    4    5     6
int (*fptr[ ])(char *) = {(int (*)())mkdir,rmdir,ls, cd, pwd, creat, rm};

defines a TABLE OF FUNCTION POINTERS, each corresponds to an index 0 to 6.

     int r = fptr[index](pathname)

invokes that function with pathname as parameter.

6. Samples Solutions:
   samples/LAB1/lab1.bin  OR  lab1.static
