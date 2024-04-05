//P2-SSOO-23/24

// MSH main file
// Write your msh source code here

//include "parser.h"
#include <stddef.h>			/* NULL */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_COMMANDS 8


// files in case of redirection
char filev[3][64];

//to store the execvp second parameter
char *argv_execvp[8];

void siginthandler(int param)
{
	printf("****  Exiting MSH **** \n");
	//signal(SIGINT, siginthandler);
	exit(0);
}

/* myhistory */

/* myhistory */

struct command
{
  // Store the number of commands in argvv
  int num_commands;
  // Store the number of arguments of each command
  int *args;
  // Store the commands
  char ***argvv;
  // Store the I/O redirection
  char filev[3][64];
  // Store if the command is executed in background or foreground
  int in_background;
};

int history_size = 20;
struct command * history;
int head = 0;
int tail = 0;
int n_elem = 0;

void free_command(struct command *cmd)
{
    if((*cmd).argvv != NULL)
    {
        char **argv;
        for (; (*cmd).argvv && *(*cmd).argvv; (*cmd).argvv++)
        {
            for (argv = *(*cmd).argvv; argv && *argv; argv++)
            {
                if(*argv){
                    free(*argv);
                    *argv = NULL;
                }
            }
        }
    }
    free((*cmd).args);
}

void store_command(char ***argvv, char filev[3][64], int in_background, struct command* cmd)
{
    int num_commands = 0;
    while(argvv[num_commands] != NULL){
        num_commands++;
    }

    for(int f=0;f < 3; f++)
    {
        if(strcmp(filev[f], "0") != 0)
        {
            strcpy((*cmd).filev[f], filev[f]);
        }
        else{
            strcpy((*cmd).filev[f], "0");
        }
    }

    (*cmd).in_background = in_background;
    (*cmd).num_commands = num_commands-1;
    (*cmd).argvv = (char ***) calloc((num_commands) ,sizeof(char **));
    (*cmd).args = (int*) calloc(num_commands , sizeof(int));

    for( int i = 0; i < num_commands; i++)
    {
        int args= 0;
        while( argvv[i][args] != NULL ){
            args++;
        }
        (*cmd).args[i] = args;
        (*cmd).argvv[i] = (char **) calloc((args+1) ,sizeof(char *));
        int j;
        for (j=0; j<args; j++)
        {
            (*cmd).argvv[i][j] = (char *)calloc(strlen(argvv[i][j]),sizeof(char));
            strcpy((*cmd).argvv[i][j], argvv[i][j] );
        }
    }
}


/**
 * Get the command with its parameters for execvp
 * Execute this instruction before run an execvp to obtain the complete command
 * @param argvv
 * @param num_command
 * @return
 */
void getCompleteCommand(char*** argvv, int num_command) {
	//reset first
	for(int j = 0; j < 8; j++)
		argv_execvp[j] = NULL;

	int i = 0;
	for ( i = 0; argvv[num_command][i] != NULL; i++)
		argv_execvp[i] = argvv[num_command][i]; //
}


/**
 * Main sheell  Loop  
 */
int main(int argc, char* argv[])
{
	/**** Do not delete this code.****/
	int end = 0; 
	int executed_cmd_lines = -1;
	char *cmd_line = NULL;
	char *cmd_lines[10];

	if (!isatty(STDIN_FILENO)) {
		cmd_line = (char*)malloc(100);
		while (scanf(" %[^\n]", cmd_line) != EOF){
			if(strlen(cmd_line) <= 0) return 0;
			cmd_lines[end] = (char*)malloc(strlen(cmd_line)+1);
			strcpy(cmd_lines[end], cmd_line);
			end++;
			fflush (stdin);
			fflush(stdout);
		}
	}

	/*********************************/

	char ***argvv = NULL;
	int num_commands;

	history = (struct command*) malloc(history_size *sizeof(struct command));
	int run_history = 0;

	while (1) 
	{
		int status = 0;
		int command_counter = 0;
		int in_background = 0;
		signal(SIGINT, siginthandler);

		if (run_history)
    {
        run_history=0;
    }
    else{
        // Prompt 
        write(STDERR_FILENO, "MSH>>", strlen("MSH>>"));

        // Get command
        //********** DO NOT MODIFY THIS PART. IT DISTINGUISH BETWEEN NORMAL/CORRECTION MODE***************
        executed_cmd_lines++;
        if( end != 0 && executed_cmd_lines < end) {
            command_counter = read_command_correction(&argvv, filev, &in_background, cmd_lines[executed_cmd_lines]);
        }
        else if( end != 0 && executed_cmd_lines == end)
            return 0;
        else
            command_counter = read_command(&argvv, filev, &in_background); //NORMAL MODE
    }
		//************************************************************************************************


		/************************ STUDENTS CODE ********************************/
     
     
	   if (command_counter > 0) {
			if (command_counter > MAX_COMMANDS){
				printf("Error: Maximum number of commands is %d \n", MAX_COMMANDS);
			}
			else {
				// Print command
				print_command(argvv, filev, in_background);
			}
		}
  
        //simple commmand
        if (command_counter == 1) {
            int pid, status, fd=0;
            pid = fork();

<<<<<<< HEAD
            // redireccionamientos de 0,1,2 input,output,error
			if (strcmp(filev[0], "0")!= 0){ // If there is an input file, close STDOUT input & open it
				if ((close(0))<0){
				perror("Error closing default descriptor 0");
				}
				if (fd = open(filev[0],O_RDONLY,0666)<0){
				perror("Error opening file");}
			}
			if (strcmp(filev[1], "0")!= 0){ // If there is an output file, close STDOUT & open file
				if ((close(1))<0){
				perror("Error closing default descriptor 1");
				}
				if (fd = open(filev[1],O_WRONLY|O_CREAT|O_TRUNC,0666)<0){
				perror("Error opening file");}
			}			
			if (strcmp(filev[2], "0")!= 0){ // If there is error file, close error and open file
				if ((close(2))<0){
				perror("Error closing default descriptor 2");
				}
				if (fd = open(filev[2],O_WRONLY|O_CREAT|O_TRUNC,0666)<0){
				perror("Error opening file");}
			}	

=======
	    // redireccionamientos de 0,1,2 input,output,error
		if (strcmp(filev[0], "0")!= 0){ // If there is an input file, close STDOUT input & open it
			if ((close(0))<0){
			perror("Error closing default descriptor 0");
			}
			if (fd = open(filev[0],O_RDONLY,0666)<0){
			perror("Error opening file");}
		}
		if (strcmp(filev[1], "0")!= 0){ // If there is an output file, close STDOUT & open file
			if ((close(1))<0){
			perror("Error closing default descriptor 1");
			}
			if (fd = open(filev[1],O_WRONLY|O_CREAT|O_TRUNC,0666)<0){
			perror("Error opening file");}
		}			
		if (strcmp(filev[2], "0")!= 0){ // If there is error file, close error and open file
			if ((close(2))<0){
			perror("Error closing default descriptor 2");
			}
			if (fd = open(filev[2],O_WRONLY|O_CREAT|O_TRUNC,0666)<0){
			perror("Error opening file");}
		}	

            
>>>>>>> bf8fcb7c6c7d12a5524d7f438f51981b2216b2cf
            switch (pid){
            
            case -1:  //error case
            
                perror("Error in fork");
                return -1;

                
            case 0:  //child
            
                execvp(argvv[0][0],argvv[0]);
                perror("Error in exec");
                break;
            
            default:  //parent proces

                if(in_background !=1){  // back to the parent waiting
					while(pid !=wait (&status) );
					
				} else{     // back to the parent without waiting
					printf("Pid = [%d]\n",getpid());
				} 
            }
        }

        //multiple commands
        else if (command_counter > 1){

            int pipes[command_counter - 1][2];  //create same number of pipes as many commands except of one

            for (int i = 0; i < command_counter - 1; i++) {
                if (pipe(pipes[i]) == -1) {
                    perror("error in pipe");
                    exit(EXIT_FAILURE);
                }
            }

            for (int i = 0; i < command_counter; i++) {

                int pid = fork();
                if (pid == -1) {
<<<<<<< HEAD
                    perror("error in fork");
=======
                    perror("fork");
>>>>>>> bf8fcb7c6c7d12a5524d7f438f51981b2216b2cf
                    exit(EXIT_FAILURE);
                } 
                else if (pid == 0) {  // child process
                    if (i > 0) {
                        dup2(pipes[i - 1][0], STDIN_FILENO);
                        close(pipes[i - 1][0]);
                        close(pipes[i - 1][1]);
                    }
                    if (i < command_counter - 1) {
                        dup2(pipes[i][1], STDOUT_FILENO);
                        close(pipes[i][0]);
                        close(pipes[i][1]);
                    }
                    execvp(argvv[i][0], argvv[i]);
                    perror("error in execvp");
                    exit(EXIT_FAILURE);
                }
<<<<<<< HEAD

                
            }
=======
>>>>>>> bf8fcb7c6c7d12a5524d7f438f51981b2216b2cf

                
            }
            for (int i = 0; i < command_counter - 1; i++) {
                close(pipes[i][0]);
                close(pipes[i][1]);
            }
<<<<<<< HEAD

=======
>>>>>>> bf8fcb7c6c7d12a5524d7f438f51981b2216b2cf
            if (in_background != 1) { // back to the parent waiting
                for (int i = 0; i < command_counter; i++) {
                    wait(NULL); // Wait for all child processes to finish
                }
<<<<<<< HEAD
            } 
            
            else { // back to the parent without waiting
                printf("Pid = [%d]\n", getpid()); // Print PID of child process
            }

        }

        }
        return 0;
	}
	

=======
            } else { // back to the parent without waiting
                printf("Pid = [%d]\n", getpid()); // Print PID of each child process
            }

	}

	}
return 0;
}
	
>>>>>>> bf8fcb7c6c7d12a5524d7f438f51981b2216b2cf
