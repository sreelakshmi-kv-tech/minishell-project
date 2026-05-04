#include "header.h"

void signal_handler(int sig)
{
    if (sig == SIGINT)             // Ctrl+C pressed
    {
        if (fg_pid > 0)            // If foreground job exists
            kill(fg_pid, SIGINT);  // Kill foreground job
        else
            printf("\n");          // Just print new line
    }
    else if (sig == SIGTSTP)       // Ctrl+Z pressed
    {
        if (fg_pid > 0)            // If foreground job exists
        {
            kill(fg_pid, SIGTSTP); // Stop foreground job

            Job *j = malloc(sizeof(Job)); // Allocate job
            j->pid = fg_pid;              // Store PID
            strcpy(j->cmd, "stopped");    // Store status
            j->next = job_head;           // Add to job list
            job_head = j;

            fg_pid = 0;                   // Reset foreground PID
            printf("\n");                 // Print new line
        }
    }
}
