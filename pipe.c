#include "header.h"

void execute_pipes(char *input)
{
    char *cmds[MAX_ARGS];          // Commands separated by pipe
    int count = 0;

    cmds[count] = strtok(input, "|"); // Split by pipe
    while (cmds[count])
        cmds[++count] = strtok(NULL, "|");

    int fd[2], in = 0;             // Pipe descriptors

    for (int i = 0; i < count; i++)
    {
        pipe(fd);                  // Create pipe

        if (fork() == 0)           // Child process
        {
            dup2(in, 0);           // Redirect input
            if (i != count - 1)
                dup2(fd[1], 1);    // Redirect output

            close(fd[0]);          // Close read end

            char *args[MAX_ARGS];  // Arguments
            int j = 0;
            args[j] = strtok(cmds[i], " ");
            while (args[j])
                args[++j] = strtok(NULL, " ");

            execvp(args[0], args); // Execute command
            exit(1);
        }
        else
        {
            wait(NULL);            // Wait for child
            close(fd[1]);          // Close write end
            in = fd[0];            // Pass input
        }
    }
}
