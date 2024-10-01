#include "commands.h"

// Add  Global variables here

void executeCommands(char *command)
{
    if (strncmp(command, "exit", 4) == 0)
    {
        fprintf(stdout, "Closing edsh\033[5m...\033[0m\n");
        sleep(1);
        exit(0);
    }
    else if (strncmp(command, "clear", 5) == 0)
    {
        printf("\033[2J\033[1;1H");
    }
    else
    {
        printf("\033[1;31mInvalid Command\n\033[0m");
    }  
}