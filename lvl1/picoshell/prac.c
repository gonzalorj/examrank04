#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int picoshell(char **cmds[])
{
    int i = 0;
    int fd[2];
    int tmp_fd;
    pid_t pid;
    tmp_fd = dup(STDIN_FILENO);
    while(cmds[i])
    {
        if(cmds[i + 1])
        {
            if(pipe(fd) == -1)
            {
                dup2(tmp_fd, STDIN_FILENO);
                close(tmp_fd);
                return 1;
            }
        }
        pid = fork();
        if (pid == -1)
        {
            dup2(tmp_fd, STDIN_FILENO);
            close(tmp_fd);
            return 1;
        }
        if(pid == 0)
        {
            if(cmds[i + 1])
            {
                close(fd[0]);
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]);
            }
            close(tmp_fd);
            execvp(cmds[i][0], cmds[i]);
            exit(1);
        }
        else
        {
            if(cmds[i + 1])
            {
                close(fd[1]);
                dup2(fd[0], STDIN_FILENO);
                close(fd[0]);
            }
            i++;
        }
    }
    while(wait(NULL) != -1);
    dup2(tmp_fd, STDIN_FILENO);;
    close(tmp_fd);
    return 0;
}