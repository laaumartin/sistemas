#include <stdio.h>		
#include <unistd.h>		
#include <sys/types.h>	
#include <dirent.h>
#include <string.h>
#define PATH_MAX 4096

int myls(const char *dirname){
    DIR *direct;
    if(dirname!=NULL){				//checks if there is input
        direct= opendir(dirname);		//if there is input, opens directory specified
    }else{
        char buffer[PATH_MAX];			//if no input, declares buffer to save current directory
        getcwd(buffer,PATH_MAX);		//saves current directory in buffer
        direct=opendir(buffer);			//opens current directory
    }
    if (direct == NULL) {			//checks if there was an error opening directory
	perror("Error opening directory");
        return -1;				//if there was an error, returns -1
    }
    struct dirent *entry;			//pointer that represents an entry of a directory
    while ((entry = readdir(direct)) != NULL){	//loop to read each entry of directory
        printf("%s\n", entry->d_name);		//prints name of each entry
    }
    closedir(direct);				//closes directory
    return 0;					//if there were no errors, returns 0
}

int main(int argc, char *argv[])
{
  if (argc == 1){		//checks if there are arguments as input
     myls(NULL);		//if there are no inputs calls function myls with no input
  }  
  else if (argc == 2){		//if there is an input arguments, calls function myls with that input
    int var;			//aux variable to detect errors
    var = myls(argv[1]);
    if (var==-1){		//if there are errors, returns -1
      return -1;
    }
  }
  else{
    return -1;			// if more inputs than required, error detected
  }
	return 0;		
}
