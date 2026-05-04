#include "header.h"

void scan_input(char *prompt, char *input)
{
    printf("%s", prompt);          // Display prompt
    fflush(stdout);                // Flush output buffer

    if (!fgets(input, MAX_INPUT, stdin)) // Read input
        exit(0);                   // Exit on Ctrl+D

    input[strcspn(input, "\n")] = 0; // Remove newline

    if (strncmp(input, "PS1=", 4) == 0 && input[4] != ' ') // Change prompt
    {
        strcpy(prompt, input + 4); // Update prompt
        strcat(prompt, " ");       // Append space
        input[0] = '\0';           // Clear input
    }
}
