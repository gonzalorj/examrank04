#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int ft_popen(const char *file, const char *av[], char type)
{
    int pipefd[2];
    pid_t pid;
    if(!file || !av || type != 'r' && type != 'w')
        return 1;
    if(pipe(pipefd) == -1)
        return -1;
    pid = fork();
    if(pid == -1)
    {
        close(pipefd[1]);
        close(pipefd[0]);
        return 1;
    }
    if(pid == -1)
    {
        if(type == 'r')
        {
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
            close(pipefd[0]);
        }
        else
        {
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[1]);
            close(pipefd[0]);
        }
        execvp(file, (char * const *)av);
        exit(1);
    }
}