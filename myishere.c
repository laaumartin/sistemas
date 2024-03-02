//P1-SSOO-23/24

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>


int myishere(const char *dirname , const char *filename){
    if((dirname==NULL)||(filename==NULL)){
        perror("Not enough arguments provided");
        return -1;
    }else{
        DIR *direct;
        direct= opendir(dirname);
        if(direct==NULL){
            perror("Error opening directory");
            return -1;
        }else{
            int match=0;
            struct dirent *entry;
            while(((entry = readdir(direct)) != NULL) && (match==0)){
                const char *entryname=entry->d_name;
                if(strcmp(filename,entryname)==0){
                    match=1;
                }
            }
            if(match==1){
                printf("File %s is in directory %s\n",filename,dirname);
            }else{
                printf("File %s is not in directory %s\n",filename,dirname);
            }
            closedir(direct);
            return 0;
        }
    }
}



int main(int argc, char *argv[])
{
	/* If less than three arguments (argv[0] -> program, argv[1] -> directory to search, argv[2] -> file to find) print an error y return -1 */
	if(argc < 3)
	{
		printf("Too few arguments\n");
		return -1;
	}
  else if (argc > 3){
    return -1;
  }
  int var;
  var = myishere(argv[1],argv[2]);
  if (var==-1){
    return -1;
  }
	return 0;
}

