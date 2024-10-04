#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <pwd.h>
#include <ctype.h>
#include "commands.h"

#define COLOR_RESET "\033[0m"
#define COLOR_CYAN "\033[0;36m"

// Add global variables here
char input[INPUT_SIZE];
char hostname[HOSTNAME_SIZE];
char username[USERNAME_SIZE];
char directory[DIRECTORY_SIZE];

// Add Process command code here
void processCommand(char* command) {
    // removing leading whitespaces
    char *ptr = command;
    while ((ptr != NULL) && isspace((unsigned char)*ptr))
    {
        ptr++;
    }

    if (ptr != command)
    {
        memmove(command, ptr, strlen(ptr) + 1);
    }

    // removing trailing whitespaces
    ptr = command + strlen(command) - 1;
    while ( (ptr >= command ) && isspace((unsigned char)*ptr))
    {
        ptr--;
    }
    *(ptr+1) = '\0';

    executeCommands(command);
}

//Add Main function code here
int main(int argc, char *argv[]) {
    // getting hostname and logged in username
    if (gethostname(hostname, HOSTNAME_SIZE) != 0)
    {
        perror("gethostname");
        exit(EXIT_FAILURE);
    }

    if (getlogin_r(username, USERNAME_SIZE) != 0)
    {
        struct passwd *pw = getpwuid(geteuid());
        if (pw == NULL)
        {
            perror("getpwuid");
            exit(EXIT_FAILURE);
        }
        snprintf(username, sizeof(username), "%s", pw->pw_name);
    }

    while (1)
    {
        if (getcwd(directory, DIRECTORY_SIZE) == NULL)
        {
            perror("getcwid");
            exit(EXIT_FAILURE);
        }

        fprintf(stdout, "%s%s@%s:%s%s> ", COLOR_CYAN, username, hostname, directory, COLOR_RESET);

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            perror("fgets");
            exit(EXIT_FAILURE);
        }

        // removing terminating \n
        input[strlen(input) - 1] = '\0';

        if (strlen(input) > 0)
        {
            processCommand(input);
        }   
    }
}