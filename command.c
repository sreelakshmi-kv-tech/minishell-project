#include "header.h"

void execute_internal_commands(char *input)
{
    if (strcmp(input, "exit") == 0)        // Exit shell
        exit(0);

    else if (strcmp(input, "pwd") == 0)    // Print working directory
    {
        char buf[256];
        getcwd(buf, sizeof(buf));
        printf("%s\n", buf);
    }
    else if (strncmp(input, "cd ", 3) == 0)// Change directory
        chdir(input + 3);

    else if (strcmp(input, "echo $$") == 0)// Print shell PID
        printf("%d\n", getpid());

    else if (strcmp(input, "echo $?") == 0)// Print last exit status
        printf("%d\n", last_status);

    else if (strcmp(input, "echo $SHELL") == 0)// Print shell path
        printf("%s\n", getenv("SHELL"));

    else if (strcmp(input, "jobs") == 0)   // List background jobs
    {
        Job *j = job_head;
        while (j)
        {
            printf("[%d] %s\n", j->pid, j->cmd);
            j = j->next;
        }
    }
    else if (strcmp(input, "fg") == 0)     // Bring job to foreground
    {
        if (!job_head)
        {
            printf("fg: no jobs\n");
            return;
        }

        fg_pid = job_head->pid;            // Set foreground PID
        kill(fg_pid, SIGCONT);             // Resume job
        waitpid(fg_pid, &last_status, WUNTRACED); // Wait
        Job *tmp = job_head;               // Remove job
        job_head = job_head->next;
        free(tmp);
        fg_pid = 0;                        // Reset PID
    }
    else if (strcmp(input, "bg") == 0)     // Resume job in background
    {
        if (!job_head)
        {
            printf("bg: no jobs\n");
            return;
        }
        kill(job_head->pid, SIGCONT);      // Continue job
        printf("[%d] resumed\n", job_head->pid);
    }
}

void execute_external_commands(char *input)
{
    char copy[MAX_INPUT];                  // Copy input
    strcpy(copy, input);

    char *args[MAX_ARGS];                  // Argument list
    int i = 0;
    args[i] = strtok(copy, " ");
    while (args[i])
        args[++i] = strtok(NULL, " ");

    int bg = 0;                            // Background flag
    if (args[i - 1] && strcmp(args[i - 1], "&") == 0)
    {
        bg = 1;
        args[i - 1] = NULL;
    }

    pid_t pid = fork();                    // Create process

    if (pid == 0)                          // Child process
    {
        signal(SIGINT, SIG_DFL);           // Default signals
        signal(SIGTSTP, SIG_DFL);
        execvp(args[0], args);             // Execute command
        perror("exec");                   // Error if fails
        exit(1);
    }
    else                                  // Parent process
    {
        if (bg)
        {
            Job *j = malloc(sizeof(Job)); // Store background job
            j->pid = pid;
            strcpy(j->cmd, input);
            j->next = job_head;
            job_head = j;
            printf("[%d] %s &\n", pid, input);
        }
        else
        {
            fg_pid = pid;                 // Set foreground PID
            waitpid(pid, &last_status, WUNTRACED);
            fg_pid = 0;
        }
    }
}
