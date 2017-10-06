#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct my_node //struct for node
{
	char name[64]; //name
	char type;
	struct Node *childPtr, *siblingPtr, *parentPtr;

}Node;

//Variables
Node *root, *cwd;
char line[128];
char command[16], pathname[64];
char dirname[64], basename[64];
int run = 1;

void initialize() //make the parent node
{
	root = malloc(sizeof(Node)); //allocate memory for base node
	strcpy(root->name, "/"); //set the root node name to /
	root->type = 'D'; //make root type D
	root->childPtr = NULL; //no child nodes yet
	root->siblingPtr = NULL; //no siblings yet
	root->parentPtr = NULL; //no parents yet

	cwd = root;
}

void init_tree() //initialize tree
{
	Node *child = malloc(sizeof(Node)); //new child node
	Node *child2 = malloc(sizeof(Node));
	Node *child3 = malloc(sizeof(Node));

	child->type = 'D'; //similar to the initialize function
	strcpy(child->name, "a");
	child->childPtr = NULL;
	child->siblingPtr = NULL;
	child->parentPtr = root;
	root->childPtr = child;

	child2->type = 'D';
	strcpy(child2->name, "b");
	child2->childPtr = NULL;
	child2->siblingPtr = NULL;

	child2->parentPtr = child;
	child->childPtr = child2;

	child3->type = 'D';
	strcpy(child3->name, "c");
	child3->childPtr = NULL;
	child3->siblingPtr = NULL;
	child3->parentPtr = child2;
	child2->childPtr = child3;
}

int findCommand(char* command)
{
	//function finds the entered command
	//implemented this way as it was more logical than class examples
	if (strcmp(command, "menu") == 0)
		return 0;
	else if (strcmp(command, "mkdir") == 0)
		return 1;
	else if (strcmp(command, "rmdir") == 0)
		return 2;
	else if (strcmp(command, "cd") == 0)
		return 3;
	else if (strcmp(command, "ls") == 0)
		return 4;
	else if (strcmp(command, "pwd") == 0)
		return 5;
	else if (strcmp(command, "creat") == 0)
		return 6;
	else if (strcmp(command, "rm") == 0)
		return 7;
	else if (strcmp(command, "save") == 0)
		return 8;
	else if (strcmp(command, "reload") == 0)
		return 9;
	else if (strcmp(command, "quit") == 0)
		return 10;

	return -1;
}

void menu()
{
	printf("Available commands: menu, mkdir, rmdir, cd, ls, pwd, creat, rm, save, reload, quit\n");
}

int absoluteCheck()
{
	//function checks to see if the command is followed by a pathname
	//with a preceeding /
	if (pathname[0] == '/')
	{
		return 1;
	}
	else
		return 0;
	return -1;
}

void dirSetup(int abs) //global variable setup
{
	int i = 0;
	int j = 0;
	int num = 0;
	int counter = 0;

	if (abs == 1)
	{
		for (i = 0; i < strlen(pathname); i++)
		{
			if (pathname[i] == '/')
				num++;
		}

		for (i = 0; i < strlen(pathname); i++)
		{
			if (pathname[i] == '/')
			{
				counter++;
			}
			if (counter != num)
			{
				dirname[i] = pathname[i];
				j++;
			}
			else
				basename[i - j] = pathname[i + 1];
		}
	}
	else
	{
		for (i = 0; i < strlen(pathname); i++)
		{
			if (pathname[i] == '/')
				num++;
		}

		for (i = 0; i < strlen(pathname); i++)
		{
			if (pathname[i] == '/')
			{
				counter++;
			}
			if (counter != num)
			{
				dirname[i] = pathname[i];
				j++;
			}
			else
				basename[i - j] = pathname[i + 1];
		}
	}
}

void addDir(Node* parentnode, char typ)
{
	Node *tempnode = malloc(sizeof(Node)); //making temp node
	tempnode->type = typ;
	strcpy(tempnode->name, basename); //copying the base name to temp node
	tempnode->childPtr = NULL; //making the pointers the same
	tempnode->siblingPtr = NULL;
	tempnode->parentPtr = parentnode;

	if (parentnode->childPtr == NULL) //For the case when the given pathname has nothing in it
	{
		parentnode->childPtr = tempnode;
	}

	else
	{
		Node *tempnode2 = malloc(sizeof(Node)); //another temp node to hold the pathname
		tempnode2 = parentnode->childPtr; //take its pointers
		if ((strcmp(tempnode2->name, basename)) == 0) //If only one is same as add
		{
			printf("This directory already exists!\n"); //let user know the directory already exists
			return;
		}
		while (tempnode2->siblingPtr != NULL) //if the entered directory is /a/b/c/d keep doing it until null is reached
		{
			tempnode2 = tempnode2->siblingPtr;
			if ((strcmp(tempnode2->name, basename)) == 0)
			{
				printf("This directory already exists!\n"); //again, let user know it already exists
				return;
			}
		}
		tempnode2->siblingPtr = tempnode;
	}
}

Node* search_dir(int abs) //for searching for a particular directory
{
	int i = 0, j = 0;
	Node *temp = malloc(sizeof(Node)); //make new node
	char tempstr[64];

	if (abs == 1) //if the value of entered command contained one value
	{
		temp = root; //keep the value of the root to temp
		for (i = 1; i < strlen(dirname) + 1; i++)
		{
			if (dirname[i] != '/' && dirname[i] != '\0')
			{
				tempstr[j] = dirname[i]; //copying directory name into temp value
			}
			else
			{
				tempstr[j + 1] = '\0'; //add in null char
				temp = temp->childPtr; //update pointer
				if (temp == NULL)
				{
					return NULL;
				}
				while (temp != NULL)
				{
					if (strcmp(temp->name, tempstr) == 0 && temp->type == 'D')
					{
						for (j = 0; j < strlen(tempstr); j++)
						{
							tempstr[j] = '\0';
						}
						j = 0;
						break;
					}
					else
					{
						temp = temp->siblingPtr;
					}
				}
			}
		}
		return temp;
	}
	else if (abs == 0) //doesn't contain any values
	{
		temp = cwd;
		for (i = 0; i < strlen(dirname) + 1; i++) //same logic as above
		{
			if (dirname[i] != '/' && dirname[i] != '\0')
			{
				tempstr[j] = dirname[i];
			}
			else
			{
				tempstr[j + 1] = '\0';
				temp = temp->childPtr;
				if (temp == NULL)
				{
					return NULL;
				}
				while (temp != NULL)
				{
					if (strcmp(temp->name, tempstr) == 0 && temp->type == 'D')
					{
						for (j = 0; j < strlen(tempstr); j++)
						{
							tempstr[j] = '\0';
						}
						j = 0;
						break;
					}
					else
					{
						temp = temp->siblingPtr;
					}
				}

			}
		}
		return temp;
	}
	return NULL;
}

void mkdir() //the various functions contain a lot of the same logic
{
	int absolute = -1; //initally set absolute to -1, will make sure there is a /
	int flag = 0;
	Node *tempnode = malloc(sizeof(Node)); //new node
	char basenametemp[128]; //temp for the name
	printf("Entered command was mkdir\n"); //let use know what entered command was
	printf("Pathname: %s\n", pathname); //list the pathname
	absolute = absoluteCheck(); //check to make sure there was a /
	dirSetup(absolute); //set up the directory with the returned absolute check value
	printf("Dirname: %s\n", dirname); //print directory name
	printf("Basename: %s\n", basename); //print basename
	tempnode = search_dir(absolute); //new tempnode with value of absolute

	if (tempnode == NULL) //no existing directory
	{
		printf("This directory does not exist!\n");
		return;
	}
	else
	{
		addDir(tempnode, 'D'); //add the new directory
	}
}

void removeDir(Node* parentnode, char typ) //remove directory function
{
	Node *tempnode = malloc(sizeof(Node)); //new node
	Node *tempnode2 = malloc(sizeof(Node)); //new node

	tempnode = parentnode; //set temps to parent
	tempnode2 = parentnode;
	tempnode = tempnode->childPtr;

	if (strcmp(tempnode->name, basename) == 0) //take all info from base and copy to temp
	{
		tempnode2->childPtr = tempnode->siblingPtr;
		strcpy(tempnode->name, "\0");
		tempnode->childPtr = NULL;
		tempnode->parentPtr == NULL;
		tempnode->siblingPtr == NULL;
		return;
	}

	tempnode2 = tempnode;
	tempnode = tempnode->siblingPtr;

	while (tempnode != NULL) //while the tempnode is not empty
	{
		if (strcmp(tempnode->name, basename) == 0)
		{
			if (tempnode->childPtr == NULL && tempnode->type == typ)
			{
				tempnode2->siblingPtr = tempnode->siblingPtr; //copy the contents into the temp node
				strcpy(tempnode->name, "\0");
				tempnode->childPtr = NULL;
				tempnode->parentPtr == NULL;
				tempnode->siblingPtr == NULL;
				return;
			}
		}
		tempnode2 = tempnode;
		tempnode = tempnode->siblingPtr;
	}

}

void rmdir() //remove directory function
{
	//much of the logic is the same as previous functions
	int absolute = -1;
	Node *tempnode = malloc(sizeof(Node));
	printf("Entered command was rmdir\n");
	printf("Pathname: %s\n", pathname);
	absolute = absoluteCheck();
	dirSetup(absolute);
	printf("Dirname: %s\n", dirname);
	printf("Basename: %s\n", basename);
	tempnode = search_dir(absolute);

	if (tempnode == NULL)
		printf("This directory does not exist!\n");
	else
	{
		removeDir(tempnode, 'D'); //d for directory
	}
}

void cd()
{
	Node *tempnode = malloc(sizeof(Node));
	char basenametemp[128];
	int absolute = -1;
	int flag = 0;
	absolute = absoluteCheck();
	dirSetup(absolute);
	if (strcmp(pathname, "/") == 0)
	{
		cwd = root;
		return;
	}
	else if (strcmp(pathname, "") == 0 && cwd == root)
	{
		return;
	}
	else if (strcmp(pathname, "") == 0 && cwd != root)
	{
		cwd = cwd->parentPtr;
		return;
	}
	if (strcmp(dirname, "") == 0)
	{
		flag = 1;
		strcpy(basenametemp, pathname);
	}
	tempnode = search_dir(absolute);

	printf("Entered command was cd\n");
	printf("Pathname: %s\n", pathname);
	printf("Dirname: %s\n", dirname);
	printf("Basename: %s\n", basename);

	if (tempnode == NULL) //node that was entered DNE
	{
		if (flag == 1)
		{
			tempnode = cwd;
			tempnode = tempnode->childPtr;
			while (tempnode != NULL)
			{
				if (strcmp(tempnode->name, basenametemp) == 0)
				{
					cwd = tempnode;
					return;
				}
				tempnode = tempnode->siblingPtr;
			}
		}
		printf("Not a valid direcotry\n"); //letting user know the directory DNE
		return;
	}
	tempnode = tempnode->childPtr; //checking the child node or child directory
	if (tempnode == NULL)
	{
		printf("Not a valid direcotry\n");
		return;
	}
	if (strcmp(tempnode->name, basename) == 0)
	{
		cwd = tempnode;
		return;
	}
	else
	{
		while (strcmp(tempnode->name, basename) != 0 && tempnode->siblingPtr != NULL)
		{
			tempnode = tempnode->siblingPtr;
			if (strcmp(tempnode->name, basename) == 0)
			{
				cwd = tempnode;
				return;
			}
		}
	}

	printf("Directory not found!\n");
}

void ls() //prints out contents of specific directory
{
	Node *temp = malloc(sizeof(Node));
	printf("Entered command was ls\n");
	temp = cwd;
	temp = temp->childPtr;
	while (temp != NULL)
	{
		printf(temp->name);
		printf("\n");
		temp = temp->siblingPtr;
	}

}

void pwd() //print the path name
{
	Node *temp = malloc(sizeof(Node));
	char string1[128], string2[128] = "\0";
	printf("Entered command was pwd\n");
	temp = cwd;

	while (temp->parentPtr != NULL)
	{
		strcpy(string1, temp->name);
		strcat(string1, "/");
		strcat(string1, string2);
		strcpy(string2, string1);
		temp = temp->parentPtr;
	}
	strcpy(string1, temp->name);
	strcat(string1, string2);
	strcpy(string2, string1);
	printf("%s", string1);
	printf("\n");
}

char* rpwd() //needed for printing to file, copies path names into temp values for printing
{
	Node *temp = malloc(sizeof(Node));
	Node *temp2 = malloc(sizeof(Node));
	int flag = 0;
	char string1[128], string2[128] = "\0";
	temp = cwd;
	temp2 = cwd;
	while (temp->parentPtr != NULL)
	{
		strcpy(string1, temp->name);
		if (flag == 1)
		{
			strcat(string1, "/");
		}
		strcat(string1, string2);
		strcpy(string2, string1);
		temp = temp->parentPtr;
		flag = 1;
	}
	strcpy(string1, temp->name);
	strcat(string1, string2);
	strcpy(string2, string1);
	if (temp2->type == 'D')
	{
		strcpy(string2, "D");
		strcat(string2, "  ");
		strcat(string2, string1);
		strcpy(string1, string2);
	}
	else
	{
		strcpy(string2, "F");
		strcat(string2, "  ");
		strcat(string2, string1);
		strcpy(string1, string2);
	}
	printf("%s", string1);
	printf("\n");
	return string1;
}

void creat() //creating new file node
{
	int absolute = -1;
	Node *tempnode = malloc(sizeof(Node));
	printf("Entered command was creat\n");
	printf("Pathname: %s\n", pathname);
	absolute = absoluteCheck();
	dirSetup(absolute);
	printf("Dirname: %s\n", dirname);
	printf("Basename: %s\n", basename);
	tempnode = search_dir(absolute);
	if (tempnode == NULL)
		printf("The given directory does not exist!\n");
	else
	{
		addDir(tempnode, 'F'); //f for file
	}
}

void rm() //delete a file node
{
	int absolute = -1;
	Node *tempnode = malloc(sizeof(Node));
	printf("Entered command was rmdir\n");
	printf("Pathname: %s\n", pathname);
	absolute = absoluteCheck();
	dirSetup(absolute);
	printf("Dirname: %s\n", dirname);
	printf("Basename: %s\n", basename);
	tempnode = search_dir(absolute);
	if (tempnode == NULL)
		printf("The given directory does not exist!\n");
	else
	{
		removeDir(tempnode, 'F');
	}
}

void save() //save the tree to a file
{
	Node *tempnode = malloc(sizeof(Node));
	FILE *f = fopen(pathname, "w+"); //open file for writing
	char string1[128];
	int flag = 0;
	if (strcmp(pathname, "") == 0) //checks for a file name
		printf("Must give the file a name.\n");
	if (f == NULL) //makes sure the file can be opened
	{
		printf("Error opening file!\n"); //if not, let user know
		return;
	}
	tempnode = cwd;
	cwd = root;
	while (cwd != NULL) //this loop writes tree to file
	{
		if (cwd->childPtr != NULL && flag == 0)
		{
			strcpy(string1, rpwd());
			fprintf(f, "%s\n", string1);
			cwd = cwd->childPtr;
		}
		else if (cwd->siblingPtr != NULL)
		{
			if (flag == 0)
			{
				strcpy(string1, rpwd());
				fprintf(f, "%s\n", string1);
			}
			cwd = cwd->siblingPtr;
			flag = 0;
		}
		else
		{
			if (flag == 0)
			{
				strcpy(string1, rpwd());
				fprintf(f, "%s\n", string1);
			}
			cwd = cwd->parentPtr;
			flag = 1;
		}
	}
	fclose(f);
	cwd = tempnode;
}

void reload() //opens a tree for reading
{
	FILE *f = fopen(pathname, "r"); //open file for reading
	char string1[128];
	int flag = 0;
	if (strcmp(pathname, "") == 0) //checks for a file name
		printf("Must give the file a name.\n");
	if (f == NULL) //makes sure the file can be opened
	{
		printf("Error opening file!\n"); //let user know file couldn't be opened
		return;
	}
	while (!feof(f))
	{
		memset(line, 0, strlen(line));
		memset(command, 0, strlen(command));
		memset(pathname, 0, strlen(pathname));
		memset(dirname, 0, strlen(dirname));
		memset(basename, 0, strlen(basename));
		fscanf(f, "%s", string1);
		if (strcmp(string1, "") == 0)
			return;
		if (strcmp(string1, "D") == 0)
		{
			fscanf(f, "%s", string1);
			if (strcmp(string1, "/") != 0)
			{
				strcpy(pathname, string1);
				mkdir();
			}
		}
		else if (strcmp(string1, "F") == 0)
		{
			fscanf(f, "%s", string1);
			strcpy(pathname, string1);
			creat();
		}
		printf("\n");
	}

	fclose(f);
}

void quit() //quit the program
{
	run = 0;
}
