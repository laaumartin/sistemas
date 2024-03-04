//P1-SSOO-23/24

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int mywc(char *filename){
    if (sizeof(filename)==0){		//checks that the filename is not empty
        return -1;			//in case there is an error, function returns -1
    }
    
    int fd = open(filename,O_RDONLY);	//system call for opnening a file and only read
    if (fd == -1){			//checks if an error has ocurred onening the file
        return -1;			//in case there is an error, function returns -1
    }
    
    int cLines=0, cWords=0, cBytes=0;	//declares variables to count the nº of words, lines and bytes in the file
    char buffer[1], prev=' ';		//declares buffer, which saves the byte read in each iteration,
					//and variable prev, which is initialized as a blank space that will be used as auxiliary variable for counting words.
    while(read(fd,buffer,1)>0){		//loop for reading each byte, in each iteration reads one byte.
        char byte_read= buffer[0];	//variable that saves the byte read in position 0 of buffer.
        cBytes += 1;		 	//in each iteration one byte is read, then counter for bytes increments in one unit.
        if ((prev == ' ' || prev == '\t' || prev == '\n') && (byte_read != ' ' && byte_read != '\t' && byte_read != '\n')) {
            cWords += 1;		//to count words previous byte must be a blank space and actual byte must not be a blank space
					//then counter for words increments one unit if the statement is true.
        }

        if (byte_read =='\n'){		//when \n appears, a new line starts, then counter for lines increments one unit
            cLines += 1;
        }
        prev = byte_read;		//variable prev saves the byte already read which will be the previous byte in the next loop
    }
    printf("%d %d %d %s\n",cLines,cWords,cBytes,filename);	//prints all values
    close(fd);				//closes file
    return 0;    			//if there are no errors returns 0
}

int main(int argc, char *argv[])
{
	/*If less than two arguments (argv[0] -> program, argv[1] -> file to process) print an error and return -1*/
	if(argc < 2)
	{
		printf("Too few arguments\n");
		return -1;
	}
  int var;			//aux variable to detect errors
  var = mywc(argv[1]);		//calls function mywc
  if (var == -1){		//checks if there are errors
    return -1;
  }
  return 0;
}







