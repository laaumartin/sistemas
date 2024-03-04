#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>


int myishere(const char *dirname , const char *filename){
    if((dirname==NULL)||(filename==NULL)){		//checks if there are enough arguments provided
        perror("Not enough arguments provided");
        return -1;					//returns -1 if not enough arguments 	
    }else{
        DIR *direct;
        direct= opendir(dirname);			//opens specified directory
        if(direct==NULL){				//checks if there is any error opening directory
            perror("Error opening directory");
            return -1;					//returns -1 if error
        }else{
            int match=0;				//aux variable to indicate if file is in directory or not
            struct dirent *entry;
            while(((entry = readdir(direct)) != NULL) && (match==0)){		//loop that reads each entry of directory
                const char *entryname=entry->d_name;				//obtains current entry
                if(strcmp(filename,entryname)==0){				//compares current entry with the input filename
                    match=1;							//if they are equal saves 1 in match
                }
            }
            if(match==1){
                printf("File %s is in directory %s\n",filename,dirname);	//if match = 1, file in directory
            }else{
                printf("File %s is not in directory %s\n",filename,dirname);	//if match = 0, file is not in directory
            }
            closedir(direct);							//closes directory
            return 0;								//if no error, returns 0
        }
    }
}

int main(int argc, char *argv[])
{
  if (argc > 3){		//if more arguments provided -> error
    return -1;
  }
  int var;			//auxiliary variable to recognize an error in the function
  var = myishere(argv[1],argv[2]);
  if (var==-1){			//if there is an error, program returns -1
    return -1;
  }
	return 0;
}

