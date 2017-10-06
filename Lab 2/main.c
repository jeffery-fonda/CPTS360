#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

char *home;

int main(int argc, char *argv[], char *env[])
{
  char input[1024], *commandArray[1024], *QUESTION = "shell> ";
	int i = 0, j = 1;
	home = getenv("HOME"); //get home directory

	printf("Type 'shellhelp' for help.\n"); //prompt user for help menu
	printf("%s", QUESTION); //get user input for help menu

	while (fgets(input, 1024, stdin) != NULL) //get user input until quit is registered
	{
		if (strcmp(input, "") != 0 && strcmp(input, "\n") != 0) //check to to see if the input is valid
		{
			commandArray[0] = strtok(input, "|"); //break up input by '|'
			while (commandArray[j] = strtok(0, "|"))
				j++;

			if (j == 1) //single command
			{
				processCommand(commandArray[0]);
			}
			else //multiple commands (max 2)
			{
				for (i = 0; i < j - 1; i++)
					processPipe(commandArray[i], commandArray[i + 1]);
			}
		}

		memset(input, 0, sizeof(input)); //reset variables and repeat input question
		j = 1;
		i = 0;
		printf("%s", QUESTION);
	}
	return(0);
}

void processPipe(char commandA[512], char commandB[512]) //process piped commands
{
	char *Aargs[512], *Bargs[512], *cmd, *arg;
	int pd[2], pid, i = 1, statusA, statusB;
	int redirA = 0, rediriA = 0, redirB = 0, rediriB = 0;

	cmd = strtok(commandA, " "); //parse commandA
	Aargs[0] = cmd;
	arg = strtok(0, " ");
	while (arg)
	{
		if (arg[strlen(arg) - 1] == '\n')
			arg[strlen(arg) - 1] = '\0';
		Aargs[i] = arg;

    //handle redirects
		if (strcmp(arg, "<") == 0) //input
		{
			rediriA = i;
			redirA = 1;
		}
		else if (strcmp(arg, ">") == 0) //output
		{
			rediriA = i;
			redirA = 2;
		}
		else if (strcmp(arg, ">>") == 0) //output and append
		{
			rediriA = i;
			redirA = 3;
		}
		arg = strtok(0, " ");
		i++;
	}
	if (!rediriA)
		Aargs[i] = NULL;
	else
		Aargs[rediriA] = NULL;

	cmd = strtok(commandB, " "); //parse commandB
	Bargs[0] = cmd;
	arg = strtok(0, " ");
	i = 1;
	while (arg)
	{
		if (arg[strlen(arg) - 1] == '\n')
			arg[strlen(arg) - 1] = '\0';
		Bargs[i] = arg;

		//handle redirects
		if (strcmp(arg, "<") == 0) //input
		{
			rediriB = i;
			redirB = 1;
		}
		else if (strcmp(arg, ">") == 0) //output
		{
			rediriB = i;
			redirB = 2;
		}
		else if (strcmp(arg, ">>") == 0) //output and append
		{
			rediriB = i;
			redirB = 3;
		}
		arg = strtok(0, " ");
		i++;
	}
	if (!redirB)
		Bargs[i] = NULL;
	else
		Bargs[rediriB] = NULL;

  //do the commands
	pipe(pd);
	if (fork() == 0)
	{
		dup2(pd[1], 1);
		close(pd[0]);
		close(pd[1]);
		if (redirA == 1) //infile
		{
			close(0);
			open(Aargs[i - 1], O_RDONLY);
		}
		else if (redirA == 2) //outfile
		{
			close(1);
			open(Aargs[i - 1], O_WRONLY | O_CREAT, 0644);
		}
		else if (redirA == 3) //outfile append
		{
			close(1);
			open(Aargs[i - 1], O_WRONLY | O_APPEND);
		}
		execvp(Aargs[0], Aargs);
	}
	if (fork() == 0)
	{
		dup2(pd[0], 0);
		close(pd[0]);
		close(pd[1]);
		if (redirB == 1) //infile
		{
			close(0);
			open(Bargs[i - 1], O_RDONLY);
		}
		else if (redirB == 2) //outfile
		{
			close(1);
			open(Bargs[i - 1], O_WRONLY | O_CREAT, 0644);
		}
		else if (redirB == 3) //outfile append
		{
			close(1);
			open(Bargs[i - 1], O_WRONLY | O_APPEND);
		}
		execvp(Bargs[0], Bargs);
	}
	close(pd[0]);
	close(pd[1]);
	wait(&statusA);
	wait(&statusB);
	printf("Command 1 Exit: %d, Command 2 Exit: %d\n", statusA, statusB); //print exit status codes
}

void processCommand(char command[512]) //process single command
{
	char *args[512], *cmd, *arg;
	int pid, status, redir = 0, rediri = 0, i = 1, builtin = 0;
	command[strlen(command) - 1] = 0;

	//get the command
	cmd = strtok(command, " ");
	arg = strtok(0, " ");

	//check to see if it is a built in command
	if (!builtInCommands(cmd, arg))
	{
		//get the arguments
		args[0] = cmd;
		while (arg != NULL)
		{
			args[i] = arg;

			//handle redirects
			if (strcmp(arg, "<") == 0) //input
			{
				rediri = i;
				redir = 1;
			}
			else if (strcmp(arg, ">") == 0) //output
			{
				rediri = i;
				redir = 2;
			}
			else if (strcmp(arg, ">>") == 0) //output and append
			{
				rediri = i;
				redir = 3;
			}
			arg = strtok(0, " ");
			i++;
		}
		// add null pointer
		if (!rediri)
			args[i] = NULL;
		else
			args[rediri] = NULL;

		//fork off new process
		pid = fork();
		if (!pid)
		{
			if (redir == 1) //infile
			{
				close(0);
				open(args[i - 1], O_RDONLY);
			}
			else if (redir == 2) //outfile
			{
				close(1);
				open(args[i - 1], O_WRONLY | O_CREAT, 0644);
			}
			else if (redir == 3) //outfile append
			{
				close(1);
				open(args[i - 1], O_WRONLY | O_APPEND);
			}
			execvp(cmd, args);
			printf("couldn't execute: %s", cmd);
		}
		else if (pid)
		{
			pid = wait(&status);
			printf("Child Exit Code: %d\n", status);
		}
	}
}

int builtInCommands(char *cmd, char *arg) //execute built-in command
{
	int done = 0;
	//cd command
	if (strcmp(cmd, "cd") == 0)
	{
		done = 1;
		//no directory specified and if home directory is specified
		if (!arg)
		{
			if (home)
			{
				chdir(home);
			}
		}
		//directory specified
		else
			chdir(arg);
	}
	//pwd command
	else if (strcmp(cmd, "pwd") == 0)
	{
		printf("%s\n", get_current_dir_name());
		done = 1;
	}
	//exit command
	else if (strcmp(cmd, "exit") == 0)
	{
		exit(0);
		done = 1;
	}
	//help command
	else if (strcmp(cmd, "shellhelp") == 0)
	{
		printf("Possible Commands:\n");
		printf("   Built-In Commands:\n");
		printf("   - shellhelp - displays the help screen\n");
		printf("   - cd [directory] - changes the directory, defaulting to the home directory if nothing specified\n");
		printf("   - pwd - prints the current working directory\n");
		printf("   - exit - exits the shell\n");
		printf("   Other Commands: Any Linux shell commands.\n");
		done = 1;
	}
	return done;
}
