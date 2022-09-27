#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

void do_ls(char []);

int main(int ac, int *av[])
{
    if (ac == 1)
        do_ls(".");
    el
        while (--ac){
            printf("%s:\n",*++av);
            do_ls(*av);
        }
    return 0;
}

void do_ls(char dirname[])
{
    DIR *dir_ptr;
    struct dirent *direntp;

    if((dir_ptr = opendir(dirname))==NULL)
        fprintf(stderr,"ls1: cannot open %s\n",dirname);
    else{
        while((direntp=readdir(dir_ptr))!=NULL)
            printf("%s\n",direntp->d_name);
        closedir(dir_ptr);
    }
}