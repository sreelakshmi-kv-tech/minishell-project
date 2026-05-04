/*
Name:Sreelakshmi kv
Date:08/01/26
MiniShell is a Linux-based custom shell that executes built-in and external commands, supports pipes, job control, and signal handling using system calls.
*/
#include "header.h"

Job *job_head = NULL;             // Initialize job list
pid_t fg_pid = 0;                 // No foreground job initially
int last_status = 0;              // Initialize exit status

int main()
{
    char prompt[50] = "minishell$ ";   // Default shell prompt
    char input[MAX_INPUT];             // Input buffer

    signal(SIGINT, signal_handler);    // Handle Ctrl+C
    signal(SIGTSTP, signal_handler);   // Handle Ctrl+Z

    while (1)                          // Infinite shell loop
    {
        scan_input(prompt, input);     // Read user input

        if (strlen(input) == 0)        // Ignore empty input
            continue;

        if (strchr(input, '|'))        // Check for pipe
            execute_pipes(input);      // Execute piped commands
        else
            if (
        strcmp(input, "bg") == 0 ||
        strcmp(input, "fg") == 0 ||
        strcmp(input, "jobs") == 0 ||
        strcmp(input, "exit") == 0 ||
        strncmp(input, "cd", 2) == 0 ||
        strcmp(input, "pwd") == 0 ||
        strncmp(input, "echo", 4) == 0
    )
    {
        execute_internal_commands(input);   // handle built-ins
    }
    else
    {
        execute_external_commands(input);   // handle external commands
    }
    }
}
