/*Osh shell from the book operating system concepts 10th*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>

#define MAX_LINE 80 /*THE MAXIMUN LENGHT OF ANY COMMAND*/
#define LENGHT (MAX_LINE /2) +1 /*THE LENGHT OF THE ARGUMENTS*/
#define MAX_HISTORY 1000
#define READ_END 0
#define WRITE_END 1

typedef struct CommandLine
{
    bool piped;
    bool redirect;
    int argCount;
    int completeCount;
    int error;
    int redirectDirect; /*0 nothing, 1 > , 2 < */
    char * file;
    char *args[LENGHT];
    char * complete[LENGHT];
} CommandLine;

static CommandLine history[sizeof(CommandLine) * MAX_HISTORY];

int callCommand(CommandLine * cmd);
void deleteNewLine(char *s, size_t lenght);
void readToken(char buff[MAX_LINE], CommandLine * cmd);

int main()
{

    int i, j, historyCount = 0;
    bool run = true, historyEmpty = true, historyExists=false;

    while(run)
    {
        /*Command initialization*/
        CommandLine * command = malloc(sizeof(CommandLine));
        command->argCount = 0;
        command->completeCount = 0;
        command->redirectDirect=0;
        command->redirect = false;
        command->piped=false;
        command->error=false;
        char buff[MAX_LINE];
reset:

        printf("osh>");
        fflush(stdout);

        fgets(buff, MAX_LINE, stdin); // read line
        if(!strcmp(buff, "exit\n"))
        {
            exit(0);
        }
        if(buff[0] == '\n')
        {
            fflush(stdin);
            goto reset;
        }

        if(!strcmp(buff, "history\n"))
        {
            /*show all the commands available in the history*/
            if(historyCount == 0)
            {
                printf("There are not commands in history yet\n");
            }
            for(i = 0; i < historyCount; i++)
            {
                printf("%d  ", i+1);
                for(j =0; j < history[i].completeCount; j++)
                {
                    printf("%s ", history[i].complete[j]);
                }
                printf("\n");
            }
            fflush(stdin);
            goto reset;
        }

        /*if the !! was entered, check if there are commands in the history. If there are load that command in the args array*/
        if(!strcmp(buff, "!!\n"))
        {
            if(historyEmpty)
            {
                printf("No commands in history\n");
                fflush(stdin);
                buff[0] = '\0';
                goto reset;
            }
            else
            {
                historyExists = true;
            }
        }
        /*If there are not history proceed with the normal input*/
        if(!historyExists)
        {
            readToken(buff, command);
            command->error = callCommand(command);
        }
        else
        {
            historyExists = false;
            callCommand(&history[historyCount--]);
        }




        if(command->error == -1)
        {
            fprintf(stderr, "osh: \"");
            for(i = 0; i < command->completeCount -1; i++)
            {
                if(command->complete) fprintf(stderr,"%s ", command->complete[i]);
            }
            fprintf(stderr, "\"unknown command\n");
        }
        else
        {
            /*Moving the command to the history*/
            history[historyCount].piped = command->piped;
            history[historyCount].redirect = command->redirect;
            history[historyCount].argCount = command->argCount;
            history[historyCount].completeCount = command->completeCount;
            history[historyCount].error = command->error;

            if(!command->file)
            {
                history[historyCount].file = NULL;
            }
            else
            {
                history[historyCount].file = malloc(strlen(command->file) +1);
                history[historyCount].file = command->file;
            }


            for(i = 0 ; i < command->argCount; i++)
            {
                history[historyCount].args[i] = malloc(strlen(command->args[i]) +1);
                history[historyCount].args[i] = strndup(command->args[i], strlen(command->args[i]) +1);
            }
            for(i = 0 ; i < command->completeCount; i++)
            {
                history[historyCount].complete[i] = malloc(strlen(command->complete[i]) +1);
                history[historyCount].complete[i] = strndup(command->complete[i], strlen(command->complete[i]) +1);
            }

            historyCount++;

            historyEmpty = false;

            /*Clearing the command for next inputs*/
            free(command);
        }

    }
    return 0;
}

void readToken(char buff[MAX_LINE], CommandLine * cmd)
{
    bool readFile = false;

    int i;
    char * token = strtok(buff, " ");

    deleteNewLine(token, strlen(token));

    cmd->args[cmd->argCount] = token;
    cmd->argCount++;

    cmd->complete[cmd->completeCount] = token;
    cmd->completeCount++;

    token = strtok(NULL, " "); /*Get the next token*/
    if(!token)   /*If there is nothing next just return. It is a simple command without args*/
    {
        cmd->args[cmd->argCount] = NULL;
        cmd->complete[cmd->completeCount] = NULL;
        return;
    }

    while(token != NULL) /*Read each character of the token string*/
    {
        for(i =0 ; i < strlen(token) +1; i++)
        {
            if(readFile) /*Set the file string to the command structure. it is only set when there is a redirection in the command*/
            {
                cmd->file = (char * ) malloc(strlen(token));
                cmd->file = token;
                deleteNewLine(cmd->file, strlen(token)+1);
                cmd->complete[cmd->completeCount] = token;
                deleteNewLine(cmd->complete[cmd->completeCount], cmd->completeCount);
                readFile = false;
                break;
            }
            /*Options of the command*/
            if(token[i] == '-')
            {
                deleteNewLine(token, strlen(token));
                cmd->args[cmd->argCount] = token;
                cmd->argCount++;
                cmd->complete[cmd->completeCount] = token;
                deleteNewLine(cmd->complete[cmd->completeCount], cmd->completeCount);
                break;
            }

            else if(token[i] == '|')
            {
                deleteNewLine(token, strlen(token));
                cmd->piped = true; /*means that there is a pipe in the command*/
                cmd->complete[cmd->completeCount] = token;
                deleteNewLine(cmd->complete[cmd->completeCount], cmd->completeCount);
                break;
            }
            else if(token[i] == '<' || token[i] == '>')
            {
                cmd->redirect=true; /*means that there is a redirection in the command*/
                readFile= true;
                if(token[i] == '>' ) cmd->redirectDirect =1;
                else cmd->redirectDirect= 2;
                cmd->complete[cmd->completeCount] = token;
                deleteNewLine(cmd->complete[cmd->completeCount], cmd->completeCount);
                break;
            }
            /*End of the command or part of the command*/
            else if(token[i] == '\n' || i == strlen(token) +1)
            {
                deleteNewLine(token, strlen(token));
                cmd->args[cmd->argCount] = token;
                cmd->argCount++;
                cmd->complete[cmd->completeCount] = token;
                deleteNewLine(cmd->complete[cmd->completeCount], cmd->completeCount);
                break;
            }
        }
        token = strtok(NULL, " ");
        cmd->completeCount++;
    }
    cmd->args[cmd->argCount] = NULL;
    cmd->complete[cmd->completeCount] = NULL;

}


void deleteNewLine(char *s, size_t lenght)
{
    int i;
    for(i =0; i <= lenght; i++)
    {
        if(s[i] =='\n')
        {
            s[i] = '\0';
            break;
        }
    }
}

int callCommand(CommandLine * cmd)
{
    pid_t pid;
    int fd, i, j;
    int fd2[1];
    pid = fork();

    if(pid == 0)  /*CHILD PROCESS*/
    {
        int status;
        if(cmd->redirect)
        {
            fflush(stdout);
            fd = open(cmd->file, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
            if(cmd->redirectDirect == 1)
            {
                dup2(fd, STDOUT_FILENO);
            }
            else if(cmd->redirectDirect==2)
            {
                dup2(fd, STDIN_FILENO);
            }
            close(fd);
        }

        if(cmd->piped)
        {
            pid_t pid2;
            if(pipe(fd2) == -1)
            {
                fprintf(stderr, "Pipe Failed\n");
                return 1;
            }

            char * part1[LENGHT];
            char * part2[LENGHT];
            for(i = 0; i < cmd->completeCount; i++)
            {
                if(!strcmp(cmd->complete[i], "|"))
                {
                    part1[i] = NULL;
                    i++;
                    for(j =0; j +i < cmd->completeCount; j++)
                    {
                        part2[j] = cmd->complete[i];
                        printf("PART 2: %s\n", part2[j]);
                    }
                    part2[j] = NULL;
                    break;
                }
                part1[i] = cmd->complete[i];
                printf("PART 1: %s\n", part1[i]);
            }
            pid2 = fork();
            if(pid2 == 0)
            {
                int status;
                close(fd2[READ_END]);
                dup2(fd2[WRITE_END], STDOUT_FILENO);
                close(fd2[WRITE_END]);
                status = execvp(part1[0], part1);
                status == -1 ? -1: 0;
            }
            else if(pid2 > 0)
            {
                wait(&status);
                if(status == -1) return -1;
                close(fd2[WRITE_END]);
                dup2(fd2[READ_END], STDIN_FILENO);
                close(fd2[READ_END]);
                status = execvp(part2[0], part2);
                status == -1 ? -1: 0;
            }
        }
        if(cmd->redirectDirect == 2)
        {
            status == execvp(cmd->args[0], cmd->args);
        }
        status = execvp(cmd->args[0], cmd->args);
        status == -1 ? -1: 0;

    }
    else if(pid < 0)  /*ERROR*/
    {
        fprintf(stderr, "The command couldn't be executed\nTry again\n");
    }
    else if(pid > 0 )    /*PARENT PROCESS*/
    {
        int status = 0;
        wait(&status);
        return status;
    }

    return -1;
}
