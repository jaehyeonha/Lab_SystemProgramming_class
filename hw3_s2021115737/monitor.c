#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define past 0
#define present 1

int main()
{

    time_t ct;
	struct tm tm;

    char id[20] = {0};
    char remain[200] = {0};
    double temp[9][16] = {0};

    double cpu[2][9][10] = {0};
    double total;

    double mem[24];

    double net[2][6][16] = {0};

    int start = 0;

    FILE *cpuStat;
    FILE *memStat;
    FILE *netStat;

	while(1){
        ct = time(NULL);
	    tm = *localtime(&ct);

        if((cpuStat = fopen("/proc/stat", "r")) == NULL){				
            fprintf(stderr, "fopen error");
            exit(1);
        }
        printf("%02d:%02d:%02d    CPU %%usr %%nice %%sys %%iowait %%irq %%soft %%steal %%idle \t %%cpuused\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
        for(int i = 0; i <= 8; i++){
            fscanf(cpuStat,"%s %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",id, &cpu[present][i][0], &cpu[present][i][1], &cpu[present][i][2], &cpu[present][i][3], 
            &cpu[present][i][4], &cpu[present][i][5], &cpu[present][i][6], &cpu[present][i][7], &cpu[present][i][8], &cpu[present][i][9]);
            
            total = 0;
            
            for(int j = 0; j <= 7; j++){
                temp[i][j] = cpu[present][i][j]-cpu[past][i][j];
                cpu[past][i][j] = cpu[present][i][j];
                total += temp[i][j];
            }
            if(start != 0){
                if(i != 0){
                    printf("%02d:%02d:%02d     %c  %.2lf  %.2lf  %.2lf  %.2lf  %.2lf  %.2lf  %.2lf  %.2lf \t  %.2lf\n", tm.tm_hour, tm.tm_min, tm.tm_sec, id[3], 100 * (temp[i][0]/total),  100 * (temp[i][1]/total),  100 * (temp[i][2]/total), 100 * (temp[i][4]/total),
                    100 * (temp[i][5]/total),  100 * (temp[i][6]/total), 100 * (temp[i][7]/total), 100 * (temp[i][3]/total), 100 * (1 - (temp[i][3]/total)));
                }
                else{
                    printf("%02d:%02d:%02d    all %.2lf  %.2lf  %.2lf  %.2lf  %.2lf  %.2lf  %.2lf  %.2lf \t  %.2lf\n",  tm.tm_hour, tm.tm_min, tm.tm_sec, 100 * (temp[i][0]/total),  100 * (temp[i][1]/total),  100 * (temp[i][2]/total), 100 * (temp[i][4]/total),
                    100 * (temp[i][5]/total),  100 * (temp[i][6]/total), 100 * (temp[i][7]/total), 100 * (temp[i][3]/total), 100 * (1 - (temp[i][3]/total)));
                }   
            }        
        }
        printf("\n");
        fclose(cpuStat);

        printf("%02d:%02d:%02d          total used free shared buff/cache available \t\t %%memused\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
        if((memStat = fopen("/proc/meminfo", "r")) == NULL){				
            fprintf(stderr, "fopen error");
            exit(1);
        }
        for (int i = 0; i <= 23; i++){
            fscanf(memStat,"%s %lf %s", id, &mem[i], remain);
        }
        
        if(start != 0){
            printf("%02d:%02d:%02d     Mem: %5.0lf %4.0lf %4.0lf %6.0lf %10.0lf %9.0lf \t %13.2lf\n", tm.tm_hour, tm.tm_min, tm.tm_sec, mem[0]/1024,(mem[0]-mem[1]-mem[3]-mem[4]-mem[23])/1024, mem[1]/1024, mem[20]/1024, (mem[3]+mem[4]+mem[23])/1024, mem[2]/1024, 100*(mem[0]-mem[1]-mem[3]-mem[4]-mem[23])/mem[0]);
            printf("%02d:%02d:%02d    Swap: %5.0lf %4.0lf %4.0lf \t\t\t\t %13.2lf\n", tm.tm_hour, tm.tm_min, tm.tm_sec, mem[14]/1024, (mem[14]-mem[15])/1024, mem[15]/1024, 100 * (mem[14]-mem[15])/mem[14]);
        }
        printf("\n");
        fclose(memStat);

        if((netStat = fopen("/proc/net/dev", "r")) == NULL){				
            fprintf(stderr, "fopen error");
            exit(1);
        }
        

        printf("%02d:%02d:%02d    IFACE  rxpck/s  txpck/s  rxkB/s  txkB/s  rxcmp/s  txcmp/s  rxmcst/s\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
        fgets(remain , 200 ,netStat);
        fgets(remain , 200 ,netStat);
        for(int i = 0; i <= 5; i++){
            fscanf(netStat,"%s %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",id, &net[present][i][0], &net[present][i][1], &net[present][i][2], &net[present][i][3], 
            &net[present][i][4], &net[present][i][5], &net[present][i][6], &net[present][i][7], &net[present][i][8], &net[present][i][9], &net[present][i][10],
            &net[present][i][11], &net[present][i][12], &net[present][i][13], &net[present][i][14], &net[present][i][15]);
            
            for(int j = 0; j <= 15; j++){
                temp[i][j] = net[present][i][j]-net[past][i][j];
                net[past][i][j] = net[present][i][j];
            }

            if(start != 0)
                printf("%02d:%02d:%02d  %7s %8.2lf  %7.2lf  %6.2lf  %6.2lf  %7.2lf  %7.2lf %9.2lf\n",  tm.tm_hour, tm.tm_min, tm.tm_sec, id,temp[i][1], temp[i][9], temp[i][0]/1024, temp[i][8]/1024, temp[i][6]/1024, temp[i][15]/1024, temp[i][7]/1024);           
        }
        fclose(netStat);
        printf("\n");

        if(start == 0){
            printf("%02d:%02d:%02d    fisrt-run : collecting data\n\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
            start = 1;
        }

        sleep(1); 
    }
}