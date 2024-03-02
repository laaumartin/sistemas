#include <stdio.h>		// Header file for system call printf
#include <unistd.h>		// Header file for system call gtcwd
#include <sys/types.h>	// Header file for system calls opendir, readdir y closedir
#include <dirent.h>
#include <string.h>
#define PATH_MAX 4096

int myls(const char *dirname){
    DIR *direct;
    if(dirname!=NULL){
        direct= opendir(dirname);
    }else{
        char buffer[PATH_MAX];
        getcwd(buffer,PATH_MAX);
        direct=opendir(buffer);
    }
    if (direct == NULL) {
        return -1;
    }
    struct dirent *entry;
    while ((entry = readdir(direct)) != NULL){
        printf("%s\n", entry->d_name);
    }
    closedir(direct);
    return 0;
}

int main(int argc, char *argv[])
{
  if (argc == 1){
     myls(NULL);
  }  
  else if (argc == 2){
    int var;
    var = myls(argv[1]);
    if (var==-1){
      return -1;
    }
  }
  else{
    return -1;
  }
	return 0;
}
