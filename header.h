#ifndef HEADER_H                 
#define HEADER_H

#include <stdio.h>               // Standard I/O
#include <stdlib.h>              // Memory management
#include <string.h>              // String handling
#include <unistd.h>              // POSIX APIs
#include <signal.h>              // Signal handling
#include <sys/wait.h>            // waitpid()

#define MAX_INPUT 256            // Maximum input length
#define MAX_ARGS  20             // Maximum arguments

#define BUILTIN  1               // Built-in command
#define EXTERNAL 2               // External command
#define NO_CMD   3               // Invalid command

// Job structure for background/stopped processes
typedef struct Job
{
    pid_t pid;                   // Process ID
    char cmd[256];               // Command string
    struct Job *next;            // Next job
} Job;

extern Job *job_head;            // Job list head
extern pid_t fg_pid;             // Foreground process PID
extern int last_status;          // Last command exit status

void scan_input(char *prompt, char *input);          // Read input
void execute_internal_commands(char *input);         // Built-in commands
void execute_external_commands(char *input);         // External commands
void execute_pipes(char *input);                     // Pipe handling
void signal_handler(int sig);                         // Signal handler

#endif                            