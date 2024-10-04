#include "commands.h"

// Add  Global variables here
#define ARGMAX 20

char *arguments[ARGMAX];
FILE *out_fh;
int argnum = 0;

void initialize_args()
{
    for (int i = 0; i < ARGMAX; i++)
    {
        arguments[i] = NULL;
    }
    argnum = 0;
}

void collect_args(char *command)
{
    arguments[0] = strtok(command, " \n\t\r");
    argnum = 1;
    for (int i = 0; i < ARGMAX; i++)
    {
        arguments[i] = strtok(NULL, " \n\r\t");
        if (arguments[i] == NULL)
        {
            break;
        }
        argnum++;
    }
}

FILE *handle_redirection()
{
    for (int i = 1; i < argnum; i++)
    {
        if (strcmp(arguments[i], ">") == 0)
        {
            if (i == (argnum - 1))
            {
                fprintf(stderr, "\033[1;31mParse error: No file specified for redirection.\n\033[0m");
                return NULL;
            }

            char *filedest = arguments[i+1];
            FILE *file = fopen(filedest, "w");
            if (file == NULL)
            {
                fprintf(stderr, "\033[1;31mError opening file %s: %s\n\033[0m", filedest, strerror(errno));
            }
            return file;
        }   
    }
    return stdout;
}

void edsh_echo()
{
    for (int i = 1; i < argnum; i++)
    {
        if (strcmp(arguments[i], ">") == 0)
        {
            break;
        }
        fprintf(out_fh, "%s", arguments[i]);
        
        if (i < argnum - 1)
        {
            fprintf(out_fh, " ");
        }
    }

    fprintf(out_fh, "\n");
}

void edsh_cat()
{
    if (argnum == 1)
    {
        fprintf(stderr, "\033[1;31mMissing filename.\n\033[0m");
        return;
    }
    
    char *filename = arguments[1];
    if (filename)
    {
        FILE *file = fopen(filename, "r");
        if (file == NULL)
        {
            fprintf(stderr, "\033[1;31mError opening file %s: %s\n\033[0m", filename, strerror(errno));
            return;
        }

        int c;
        while ((c = fgetc(file)) != EOF)
        {
            fputc(c, out_fh);
        }
        fclose(file);
    }
}

void executeCommands(char *command)
{
    initialize_args();
    collect_args(command);
    out_fh = handle_redirection();
    if (out_fh == NULL)
    {
        return;
    }
    
    if (strcmp(arguments[0], "exit") == 0)
    {
        fprintf(stdout, "Closing edsh\033[5m...\033[0m\n");
        sleep(1);
        exit(0);
    }
    else if (strcmp(arguments[0], "clear") == 0)
    {
        printf("\033[2J\033[1;1H");
    }
    else if (strcmp(arguments[0], "echo") == 0)
    {
        edsh_echo();
    }
    else if (strcmp(arguments[0], "cat") == 0)
    {
        edsh_cat();
    }
    
    else
    {
        printf("\033[1;31mInvalid Command\n\033[0m");
    }

    if (out_fh != stdout)
    {
        fclose(out_fh);
    }    
}