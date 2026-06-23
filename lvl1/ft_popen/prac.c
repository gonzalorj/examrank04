#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int ft_popen(const char *file, char *const argv[], char type)
{
    int pipefd[2];
    pid_t pid;
    if(!file || !argv || type != 'r' && type != 'w')
        return -1;
    pid = fork();
    if (pid == -1)
    {
        close(pipefd[1]);
        close(pipefd[1]);
        return -1;
    }
    if(pid == 0)
    {
        if(type == 'r')
        {
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[0]);
            close(pipefd[1]);
        }
        else
        {
            dup2(pipefd[1], STDIN_FILENO);
            close(pipefd[0]);
            close(pipefd[1]);
        }
        execv(file, (char * const*)argv);
        exit(1);
    }
    else
    {
        if(type == 'r')
        {
            close(pipefd[1]);
            return(pipefd[0]);
        }
        else
        {
            close(pipefd[0]);
            return(pipefd[1]);
        }
    }
}