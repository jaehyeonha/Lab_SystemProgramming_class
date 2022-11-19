#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/vfs.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void error_handling(char *buf);

typedef struct _mountinfo 
{
    FILE *fp;              
    long blocks;
    long used;
    long avail;
    int myuse;      
} MOUNTP;

void bB(MOUNTP *MP, double byte);
void kK(MOUNTP *MP, double byte);
void mM(MOUNTP *MP, double byte);
void gG(MOUNTP *MP, double byte);

MOUNTP *dfget(MOUNTP *MP)
{
    char buf[256];
    char devname[80];     
    char mountdir[80];
    struct statfs lstatfs;

    while(fgets(buf, 255, MP->fp))
    {
        sscanf(buf, "%s%s",devname, mountdir);
        if (strcmp(mountdir,"/") == 0)
        {   
                statfs(mountdir, &lstatfs);
                MP->blocks = lstatfs.f_blocks * (lstatfs.f_bsize/1024);
                MP->avail  = lstatfs.f_bavail * (lstatfs.f_bsize/1024);
                MP->used = MP->blocks - lstatfs.f_bfree * (lstatfs.f_bsize/1024);
                MP->myuse =(double)MP->used/ (double)MP->blocks*100 + 1; 
                return MP;
        }
    }
    rewind(MP->fp);
    return NULL;
}



int main(int ac, char *av[])
{
    if (ac != 3 && ac != 2 && strcmp(av[1], ".")){
        error_handling("error");
    }

    MOUNTP *MP;

    MP = (MOUNTP *)malloc(sizeof(MOUNTP));
    MP->fp = fopen("/proc/mounts", "r");
    if(MP->fp == NULL)
    {
        error_handling("open error");
    }

    double byte = 1;
    if(ac==2){
        kK(MP, byte);
        return 0;
    }
    else if(!strcmp(av[2], "-b")||!strcmp(av[2],"-B"))
    {
        byte = 1024;
        bB(MP, byte);
    }
    else if(!strcmp(av[2], "-k")||!strcmp(av[2],"-K"))
    {
        kK(MP,byte);
    }
    else if(!strcmp(av[2], "-m")||!strcmp(av[2],"-M"))
    {
        byte = 1/(double)1024;
        mM(MP,byte);
    }
    else if(!strcmp(av[2], "-g")||!strcmp(av[2],"-G"))
    {
        byte = 1/(double)(1024*1024);
        gG(MP,byte);
    }

    fclose(MP->fp);

    return 0;
}

void bB(MOUNTP *MP, double byte){

    while(dfget(MP))
    {   printf("    1b-blocks         Used    Available MyUse%%\n");
        printf("%13lu%13lu%13lu%6d%%\n", (unsigned long)(MP->blocks*byte), (unsigned long)(MP->used*byte) , (unsigned long)(MP->avail*byte), MP->myuse);
    }
}	


void kK(MOUNTP *MP, double byte){

    while(dfget(MP))
    {   printf(" 1K-blocks      Used Available MyUse%%\n");
        printf("%10lu%10lu%10lu%6d%%\n", (unsigned long)(MP->blocks*byte), (unsigned long)(MP->used*byte) , (unsigned long)(MP->avail*byte), MP->myuse);
    }
}	

void mM(MOUNTP *MP, double byte){

    while(dfget(MP))
    {   printf(" 1M-blocks      Used Available MyUse%%\n");
        printf("%10lu%10lu%10lu%6d%%\n",  (unsigned long)(MP->blocks*byte), (unsigned long)(MP->used*byte) , (unsigned long)(MP->avail*byte), MP->myuse);
    }
}	

void gG(MOUNTP *MP, double byte){

    while(dfget(MP))
    {   printf(" 1G-blocks      Used Available MyUse%%\n");
        printf("%10lu%10lu%10lu%6d%%\n", (unsigned long)(MP->blocks*byte), (unsigned long)(MP->used*byte) , (unsigned long)(MP->avail*byte), MP->myuse);
    }
}	



void error_handling(char *buf)
{
    fputs(buf, stderr);
    fputc('\n', stderr);
    exit(1);
}