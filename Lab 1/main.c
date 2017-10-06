#include "functions.c"

int main()
{
	int ID = -1;
	int num = 0;
	int i = 0;

	initialize(); //initalize the tree
	while (run)
	{
		i = 0;
		//clear the global perameters
		memset(line, 0, strlen(line));
		memset(command, 0, strlen(command));
		memset(pathname, 0, strlen(pathname));
		memset(dirname, 0, strlen(dirname));
		memset(basename, 0, strlen(basename));

		printf("Enter a command: "); //Get command from user
		fgets(line, sizeof line, stdin); //Store it in global line

		while (line[i]) //reads line to get command and path name
		{
			if (line[i] == '\n') //ignore the newline characters that fgets gives
			{
				i++;
			}

			if (line[i] != ' ')
			{
				command[i] = line[i];
			}

			else
			{
				num = i + 1;
				i++;

				while (line[i] != '\0')
				{
					if (line[i] == '\n')
					{
						i++;
					}
					else
					{
						pathname[i - num] = line[i];
						i++;
					}
				}
				break;
			}
			i++;
		}

		ID = findCommand(command); //finding the entered command
		switch (ID){
		case 0: menu();     break;
		case 1: mkdir();    break;
		case 2: rmdir();    break;
		case 3: cd();       break;
		case 4: ls();       break;
		case 5: pwd();      break;
		case 6: creat();    break;
		case 7: rm();       break;
		case 8: save();     break;
		case 9: reload();   break;
		case 10: quit();    break;
		case -1: printf("Invalid input!\n"); break;
		}
	}

	printf("Done.\n");
}
