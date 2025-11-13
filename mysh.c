#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>
#include <fcntl.h>


int tokenize(char* buf, char* delims, char* tokens[], int maxTokens)
{
	int token_counter = 0;
	char* token = strtok(buf, delims);
	while(token != NULL && token_counter < maxTokens)
	{
		tokens[token_counter] = token;
		token_counter++;
		token = strtok(NULL, delims);	
	}
	
	return token_counter;
}

bool run(char* line)
{
	char* delims = " \n"; 
	char* tokens[50];
	int token_counter = tokenize(line, delims, tokens, sizeof(tokens) / sizeof(char*));
	bool bg = false;
	int fd = -1;
	int fd_stdout = dup(STDOUT_FILENO);
	
	for (int i = 0; i < token_counter; i++)
	{
		if(strcmp(tokens[i], ">") == 0)
		{
			tokens[i] = (char *)0;
			fd = open(tokens[i + 1], O_RDWR | O_CREAT | O_TRUNC, 0644);
		}
		
		else if(strcmp(tokens[i], ">>") == 0)
		{
			tokens[i] = (char *)0;
			fd = open(tokens[i + 1], O_RDWR | O_CREAT | O_APPEND, 0644);
		}
	}
	
	
	if(strcmp(tokens[token_counter - 1], "&") == 0)
	{
		bg = true;
		tokens[token_counter - 1] = (char *)0;
		token_counter--;
		
	}
	else
	{
		tokens[token_counter] = (char *)0;
	}
	
	if((strcmp(tokens[0], "exit") == 0) || (strcmp(tokens[0], "quit")) == 0)	
	{
		return false;
	}
	
	if(strcmp(tokens[0], "help") == 0 )
	{
		if(fd != -1)
		{
			dup2(fd, STDOUT_FILENO);
		}
		
		printf("##########simple shell##########\n\n");
		printf("***Internal command***\n");
		printf("quit : quit the shell!\n");
		printf("exit : exit the shell!\n");
		printf("help : show this help!\n");
		printf("pwd  : show current working directory!\n");
		printf("###############################\n");
		
		if(fd != -1)
		{
			dup2(fd_stdout, STDOUT_FILENO);
			close(fd);
		}
		return true;
	}
	
	if(strcmp(tokens[0], "pwd") == 0)
	{
		char cwd2[PATH_MAX];
		
		if(fd != -1)
		{
			dup2(fd, STDOUT_FILENO);
		}
		
		if(getcwd(cwd2, sizeof(cwd2)) != NULL)
		{
			printf("%s\n", cwd2);
		}
		else
		{
			printf("getcwd() error!!\n");
		}
		
		if(fd != -1)
		{
			dup2(fd_stdout, STDOUT_FILENO);
			close(fd);
		}
		return true;
	}
	
	if((strcmp(tokens[0], "ls") == 0) || (strcmp(tokens[0], "cat") == 0) || (strcmp(tokens[0], "rm") == 0) || (strcmp(tokens[0], "ps") == 0))
	{	
		pid_t pid;
		int exit_status = -1;
		
		pid = fork();
		
		if(fd != -1)
		{
			dup2(fd, STDOUT_FILENO);
		}
		
		if(pid < 0)
		{
			printf("fork error!!\n");
			exit(1);
		}
		else if(pid == 0)
		{
			execvp(tokens[0], tokens);
			
			printf("exec error!!\n");
			exit(1);
		}
		else
		{
			if(bg == false)
			{
				waitpid(pid, &exit_status, 0);
			}
			else
			{
				printf("Running in background!! (don't use wait())\n");
			}
		}
		
		if(fd != -1)
		{
			dup2(0, STDOUT_FILENO);
			close(fd);
		}
	}
	
	return true;
}
 
int main()
{
	char line[1024];
	char cwd[PATH_MAX];
	while(1)
	{
		if(getcwd(cwd, sizeof(cwd)) != NULL)
		{
			printf("%s $ ", cwd);
		}
		else
		{
			printf("getcwd() error!!\n");
			break;
		}
		fgets(line, sizeof(line) - 1, stdin);
		if(run(line) == false)
		{
			break;
		}
	}
	return 0;
}


