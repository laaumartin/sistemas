

//P2-SSOO-22/23




// MSH main file
// Write your msh source code here




//#include "parser.h"
#include <stddef.h>     	/* NULL */
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
#include <time.h>
#include <pthread.h>




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








/* Timer */
pthread_t timer_thread;
unsigned long  mytime = 0;




void* timer_run ( )
{
	while (1)
	{
    	usleep(1000);
    	mytime++;
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








/**
 * Main sheell  Loop  
 */
int main(int argc, char* argv[]){
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




	pthread_create(&timer_thread,NULL,timer_run, NULL);




	/*********************************/




	char ***argvv = NULL;
	int num_commands;








	while (1){
    	int status = 0;
    	int command_counter = 0;
    	int in_background = 0;
    	signal(SIGINT, siginthandler);




    	// Prompt
    	write(STDERR_FILENO, "MSH>>", strlen("MSH>>"));




    	// Get command
    	//********** DO NOT MODIFY THIS PART. IT DISTINGUISH BETWEEN NORMAL/CORRECTION MODE***************
    	executed_cmd_lines++;
    	if( end != 0 && executed_cmd_lines < end) {
        	command_counter = read_command_correction(&argvv, filev, &in_background, cmd_lines[executed_cmd_lines]);
    	}
    	else if( end != 0 && executed_cmd_lines == end) {
        	return 0;
    	}
    	else {
        	command_counter = read_command(&argvv, filev, &in_background); //NORMAL MODE
    	}
    	//************************************************************************************************








    	/************************ STUDENTS CODE ********************************/
    	int fd0;
    	int fd1;
    	int fd2;
    	if (command_counter > 0) {
        	if (command_counter > MAX_COMMANDS){
            	printf("Error: Maximum number of commands is %d \n", MAX_COMMANDS);
        	}
        	else {
            	// Print command
            	print_command(argvv, filev, in_background);
        	}
    	if (strcmp(argvv[0][0],"mycalc")){
            	//llamar a la funcion (hay que implementar)
        	}else if (strcmp(argvv[0][0],"mytime")){
                	//llamar a la funcion (hay que implementar)
        	}else{
            	int tuberia[2];
   	 int auxreadanterior;
   	 int auxwriteanterior;
            	for (int i=0; i<num_commands; i++){
                	int pid, status;
                	pid=fork();
                	switch(pid){
                    	case -1:
                        	perror("error in fork");
                        	return -1;
                    	case 0:
                        	if (i!=(num_commands-1)){
                            	if (i != 0){tuberia[0]=auxreadanterior;}
                            	if(pipe(tuberia)==-1){
                                	perror("Error while creating pipe");
                                	exit(-1);
                            	}
                            	auxreadanterior= dup(tuberia[0]);
                            	if (auxreadanterior<0){
                                	perror("Error while duplicating pipes");
                                	exit(-1);
                            	}
                            	if(close(tuberia[0])== -1){
                                	perror("Error while closing pipe descriptor");
                                	exit(-1);
                            	}
                            	auxwriteanterior= dup(tuberia[1]);
                            	if (auxwriteanterior<0){
                                	perror("Error while duplicating pipes");
                                	exit(-1);
                            	}
                            	if(close(tuberia[1])== -1){
                                	perror("Error while closing pipe descriptor");
                                	exit(-1);
                            	}
                        	}
                        	if (!strcmp((filev[2]),"0")){
                            	if (close(STDERR_FILENO)<0){
                                	perror("Error while changing standard error file for specified one");
                                	exit(-1);
                            	}
                            	if((fd2 = open(filev[2], O_CREAT | O_WRONLY | O_TRUNC, 0666)) == -1){
                                	perror("Error while changing standard error file for specified one");
                                	exit(-1);
                            	}
                        	}  
                      	 
              	 
                        	if(i==0){/*caso de que estemos en el primer command*/
                            	if (!strcmp(filev[0],"0")){
                                	if (close(STDIN_FILENO)<0){
                                	perror("Error while changing standard input file for specified one");
                                	exit(-1);
                                	}
                                	if((fd0 = open(filev[0],O_RDONLY)) <0){
                                    	perror("Error while changing standard input file for specified one");
                                    	exit(-1);
                                	}
                            	}
                        	if (i==(num_commands-1)){
                            	if (!strcmp(filev[1],"0")){
                                	if (close(STDOUT_FILENO)<0){
                                    	perror("Error while changing standard output file for specified one");
                                    	exit(-1);
                                	}
                                	if((fd1 = open(filev[1],O_WRONLY)) <0){
                                    	perror("Error while changing standard output file for specified one");
                                    	exit(-1);
                                	}
                            	}
                        	}
                      	 
                        	if (i != 0){tuberia[0]=auxreadanterior; }


                        	getCompleteCommand(argvv, i);
                        	execvp(argvv[i][0], argvv[i]);
                        	perror("Error in exec. If the execution is correct this would never be executed");
                        	break;
                    	default:
                        	if (!in_background){//condicional de background
                        	while(wait(&status) > 0);
                        	if (status<0){
                            	perror("Error waiting for child process");
                        	}
                    	}


                    	if (fd0 > 0){ //check if it is open
                        	if ((close(fd0)) == -1){
                            	perror("Error closing input file descriptor");
                        	}
                    	}
                    	if (fd1 > 0){ //check if it is open
                        	if ((close(fd1)) == -1){
                            	perror("Error closing output file descriptor");
                        	}
                    	}
                    	if (fd2 > 0){//check if it is open
                        	if ((close(fd2)) == -1){
                            	perror("Error closing error file descriptor");
                        	}
                    	}








                      	 
                	}
            	}


        	}


    	}


	}
   
	return 0;
}
