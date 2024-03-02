//P1-SSOO-23/24

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int mywc(char *filename){
    if (sizeof(filename)==0){
        return -1;
    }
    
    int fd = open(filename,O_RDONLY);
    if (fd == -1){
        perror("Error opening file");
        return -1;
    }
    
    int cLines=0, cWords=0, cBytes=0;
    char buffer[1], prev=' ';
    while(read(fd,buffer,1)>0){
        char byte_read= buffer[0];
        cBytes += 1;
        if ((prev == ' ' || prev == '\t' || prev == '\n') && (byte_read != ' ' && byte_read != '\t' && byte_read != '\n')) {
            cWords += 1;
        }

        if (byte_read =='\n'){
            cLines += 1;
        }
        prev = byte_read;
    }
    printf("%d %d %d %s\n",cLines,cWords,cBytes,filename);
    close(fd);
    return 0;
            
        
    
}

int main(int argc, char *argv[])
{
	/*If less than two arguments (argv[0] -> program, argv[1] -> file to process) print an error y return -1*/
	if(argc < 2)
	{
		printf("Too few arguments\n");
		return -1;
	}
  int var;
  var = mywc(argv[1]);
  if (var == -1){
    return -1;
  }
	return 0;
}







