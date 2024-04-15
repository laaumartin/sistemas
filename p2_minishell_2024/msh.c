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
		argv_execvp[i] = argvv[num_command][i];
}
int accsum = 0;
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
				//print_command(argvv, filev, in_background);
			}
		}
         for (int i =0; i< command_counter; i++){ 
            //here we store commands in array argvv
		    getCompleteCommand(argvv,i); 
		}

        //mycalc
		if(strcmp(argvv[0][0], "mycalc") == 0) {
			
			if (argvv[0][1] != NULL && argvv[0][2] != NULL && argvv[0][3] != NULL) {
				if (strcmp(argvv[0][2], "add") == 0) {
					
					int add1 = atoi(argvv[0][1]);
					int add2 = atoi(argvv[0][3]);
					int result = add1 + add2;
					
					accsum += result;
					char array[20];
					sprintf(array, "%d", accsum);
					const char *value = array;

					if (setenv("Acc", value, 1) < 0) {
						perror("Error with the value of the environment variable\n");
					}

					fprintf(stderr, "[OK] %d + %d = %d; Acc %s \n", add1, add2, result, getenv("Acc"));
					
				} else if (strcmp(argvv[0][2], "mul") == 0) {
					
					int num1 = atoi(argvv[0][1]);
					int num2 = atoi(argvv[0][3]);
					int result = num1 * num2;

					fprintf(stderr, "[OK] %d * %d = %d \n", num1, num2, result);
					
				} else if (strcmp(argvv[0][2], "div") == 0) {
			
					int num1 = atoi(argvv[0][1]);
					int num2 = atoi(argvv[0][3]);
					if (num2 != 0) {
						int result = num1 / num2;
						int remainder = num1 % num2;

						fprintf(stderr, "[OK] %d / %d = %d; Remainder %d \n", num1, num2, result, remainder);
					
					} else {
						// error if division by 0
						perror("ERROR. Division by 0");
					}
				} else {
					// sintaxis control if there are 3 arguments but no add, mul or div
					printf("[ERROR] The structure of the command is mycalc <operand_1> <add/mul/div> <operand_2>\n");
			
				}
			} else {
				// sintaxis control if there are no 3 arguments
				printf("[ERROR] The structure of the command is mycalc <operand_1> <add/mul/div> <operand_2>\n");
			}
		}

		else if(strcmp(argvv[0][0], "myhistory") == 0) {
			if (argvv[0][1] == NULL) {
				// Caso: listar los últimos 20 comandos
				for (int i = 0; i < n_elem; i++) {
					fprintf(stderr,"%d ", i);
                	for (int j=0; j<history[i].num_commands; j++){
						for (int k=0; k<history[i].args[j]; k++){
							fprintf(stderr,"%s", history[i].argvv[j][k]);
							if (k!=history[i].args[j] -1){
								fprintf(stderr," ");
							}
						}
						if (j!= history[i].num_commands -1 ){
							fprintf(stderr," | ");
						}
					}
					if(strcmp(history[i].filev[0],"0") !=0){
						fprintf(stderr, " < %s",history[i].filev[0]);
					}
					if(strcmp(history[i].filev[1],"0") !=0){
						fprintf(stderr, " > %s",history[i].filev[1]);
					}
					if(strcmp(history[i].filev[2],"0") !=0){
						fprintf(stderr, " ! > %s",history[i].filev[2]);
					}
					if (history[i].in_background){
						fprintf(stderr," &");
					}
					fprintf(stderr,"\n");
            	}
            
			} else{
				int index = atoi(argvv[0][1]);
				if (index>=0 && index<n_elem){
					// Caso: ejecutar el comando especificado
					fprintf(stderr,"Running command %d\n",atoi(argvv[0][1]));
					int index = atoi(argvv[0][1]);
					struct command *cmd = &history[index];
					command_counter=history[index].num_commands;
					argvv= history[index].argvv;
					run_history=1;
				}
				else {
				// Error: comando fuera de rango
				printf("ERROR: Command not found\n");
				}
			}
		} 
		else {
			// Cada vez que se ejecuta un comando, se almacena en 'history'
			if (n_elem < 20) {
				if (n_elem == 0) {
					head = tail = 0; // Inicializar head y tail
				}
				store_command(argvv, filev, in_background, &history[tail]);
				tail = (tail + 1) % history_size;
				if (n_elem < history_size) {
					n_elem++;
				}
			}
			else {
				free_command(&history[head]); // Eliminar comando en head
				store_command(argvv, filev, in_background, &history[tail]);
				head = (head + 1) % history_size;
				tail = (tail + 1) % history_size;
			}

	
			//simple commmand
			if (command_counter == 1) {
				int pid, status, fd=0;
				pid = fork();

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
			else if(command_counter>1) {
				int fdes=0;
						int fd[2];
						int pid, status;
				
				
				if (strcmp(filev[0], "0")!= 0){ // If there is an input file, close STDOUT input & open it
					if ((close(0))<0){
					perror("Error closing default descriptor 0");
					}
					if (fdes = open(filev[0],O_RDONLY,0666)<0){
					perror("Error opening file");}
				}
				if (strcmp(filev[1], "0")!= 0){ // If there is an output file, close STDOUT & open file
					if ((close(1))<0){
					perror("Error closing default descriptor 1");
					}
					if (fdes = open(filev[1],O_WRONLY|O_CREAT|O_TRUNC,0666)<0){
					perror("Error opening file");}
				}					
				if (strcmp(filev[2], "0")!= 0){ // If error file, close error and open error file
					if ((close(2))<0){
					perror("Error closing default descriptor 2");
					}
					if (fdes = open(filev[2],O_WRONLY|O_CREAT|O_TRUNC,0666)<0){
					perror("Error opening file");}
				}	

				int newinput = dup(STDIN_FILENO);

				for (int i = 0; i < command_counter; i++) {
						//We create as many pipes as commands but one
								if (i != command_counter - 1) {
									if (pipe(fd) < 0) {
									perror("Error en pipe\n");
									exit(0);
									}
							}
							

								/* Create the process to connect a sequence of commands */
								pid = fork();
								if (pid==0){ // child 
								
						//the standard input is now the one given by previous child
						if (dup2(newinput,STDIN_FILENO) < 0){;// we position newinput in place of the standard input
							perror("Error duplicating and placing the descriptor");	
						}

						if (close(newinput)<0){
							perror("Error closing the descriptor");
						}
						
						if (i!= command_counter-1){
						//enviar input 
							if (dup2(fd[1],STDOUT_FILENO)<0){ //we position fd[1] in place of the standard output
							perror("Error duplicating and placing the descriptor");
							}
							if (close(fd[1])<0){
								perror("Error closing the descriptor");
							}
							if (close(fd[0])<0){
								perror("Error closing the descriptor");
							}
						}

						if(execvp(argvv[i][0],argvv[i]) <0) {
							perror("Error executing the command");
							exit(0);
						} // we use execvp as we expect input arguments and the path for the program to
						break;

					} else if(pid<0){
						perror("An error occured during the fork");
					} else{//father
						
						if (close(newinput)<0){
							perror("Error closing the descriptor");
						}

						if (i!= command_counter-1){
							//the father gives value to the standard input so next child can use it if it is not the last child

							if ((newinput = dup(fd[0])) < 0){;// if it is not the last process the father gives a value so that the child can use it in next command and this way we connect the sequence of commands
								perror("Error duplicating the descriptor");	
							}
							
							if (dup(fd[0])<0){
								perror("Error duplicating the descriptor");	
							}
							if (close(fd[1])<0){
								perror("Error closing the descriptor");
							}
						}
						
							
					}// close switch
				} // close for
					if (fdes!=0){// If a file has been opened, then close the file			
						if((close(fdes)) < 0){
																												perror("Error closing the file opened");}
				
					}

					if(in_background !=1){ // back to the parent waiting
						while(pid != wait (&status));
						
					}
					else{ // back to the parent without waiting
					printf("Pid = [%d]\n",getpid());} 
									
							
					
				}
			
		}
	}

}

       

