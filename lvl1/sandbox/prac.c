#define _POSIX_C_SOURCE 200809L


#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

void handle_timeout(int sig)
{
    (void)sig;
}

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
    pid_t pid;
    int     status;
    struct sigaction sa;

    sa.sa_handler = handle_timeout;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if(sigaction(SIGALRM, &sa, NULL) == -1)
        return -1;
    pid = fork();
    if (pid == -1)
        return -1;
    if(pid == 0)
    {
        f();
        exit(0);
    }
    alarm(timeout);
    if (waitpid(pid, &status, 0) == -1)
    {
        if(errno == EINTR)
        {
            kill(pid, SIGKILL);
            waitpid(pid, NULL, 0);
            if (verbose)
                printf("Bad function: timed out after %u seconds\n", timeout);
            return 0;
        }
        return -1;
    }
    alarm(0);
    if(WIFEXITED(status))
    {
        int exit_code = WEXITSTATUS(status);
        if (exit_code == 0)
        {
            if (verbose)
                printf("Nice function\n");
            return 1;
        }
        else
        {
            if (verbose)
                printf("Bad function, exited with code &d\n", exit_code);
            return 0;
        }
    }
    else if(WIFSIGNALED(status))
    {
        if(verbose)
            printf("Bad function: &s\n", strsignal(WTERMSIG(status)));
        return 0;
    }
    return 0;
}