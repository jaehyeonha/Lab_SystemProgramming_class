#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

void do_ls(char []);
void dostat(char *);
int compare(const void *a, const void *b); 

int main(int ac, char *av[])
{
    if (ac == 2 && !strcmp(av[1],"-R"))
        do_ls(".");
    else{
        printf("error\n");
        exit(1);
    }
    return 0;
}

void do_ls(char dirname[])
{
    DIR *dir_ptr;
    struct dirent *direntp;
    struct stat buf;
    
    char *list[255];
    char dir[255];
    int cnt=0;

    if((dir_ptr = opendir(dirname))==NULL)
        fprintf(stderr,"ls: cannot open %s\n",dirname);
    else{
        while((direntp=readdir(dir_ptr))!=NULL){
            if(strcmp(direntp->d_name,".")&&strcmp(direntp->d_name,"..")){
                list[cnt] = direntp->d_name;
                cnt++;
            }
        }

        qsort(list,cnt,sizeof(list[0]),compare);

        for(int i=0; i<cnt; i++){
            printf("%s    ",list[i]);
        }
        printf("\n");

        for(int i = 0; i<cnt; i++){
            strcpy(dir, dirname);
            strcat(dir, "/");
            strcat(dir, list[i]);
            stat(dir,&buf);
            if(S_ISDIR(buf.st_mode) == 1){
                printf("%s:\n",dir);
                do_ls(dir);
            }
        }


        closedir(dir_ptr);
    }
}

int compare(const void *a, const void *b) 
{ 
    return strcmp(*(char**)a, *(char**)b);
}
