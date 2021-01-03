#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h> 
#define NORMAL_COLOR  "\x1B[0m"
#define GREEN  "\x1B[32m"
#define BLUE  "\x1B[34m"

#define MAX_HEAP 2*1024
#define MAX_OBJS 64
char heap[MAX_HEAP];
int pheap=0;
void *obj_arr[MAX_OBJS];
int pobj=0;

//Heap objects devices
#define MAX_DEVICES 12
int devices[MAX_DEVICES];
int mounts[MAX_DEVICES];
int pdev= 0;


int main(void)
{
    printf("\nStart NoOSE ...\n");
    FILE *cmd;
    char result[1024];
    char * pch;

    cmd = popen("grep '^/dev' /proc/mounts", "r");
    if (cmd == NULL) {
        perror("popen");
        exit(EXIT_FAILURE);
    }
    while (fgets(result, sizeof(result), cmd)) {
        printf("%s\n", result);
        //sleep(10);
        pch = strtok (result," \t\r\n");
        printf ("%s\n",pch);
        //sleep(10);
        if(strncmp(pch, "/dev", 4) == 0) {
            if(pdev >= MAX_DEVICES) {
                while(1){
                    printf("To many devices detected, max is %d\n", pdev);
                    sleep(2*10);
                }
            }
            if(pobj >= MAX_OBJS) {
                while(1){
                    printf("To many objects needed, max is %d\n", pobj);
                    sleep(2*10);
                }
            }
            int len;
            len= strlen(pch) +1; // count the '\0'-character also
            printf ("Device(%d) pheap(%d): %s\n", len, pheap, pch);
            //sleep(10);
            if(pheap + len >= MAX_HEAP) {
                while(1){
                    printf("Heap is full, pheap is %d en len device is %d\n", pheap, len);
                    sleep(2*10);
                }
            }
            else {
                obj_arr[pobj]= &(heap[pheap]); 
                strcpy(&(heap[pheap]), pch);
                devices[pdev]= pobj;
                pheap+= len;
                pobj++;
            }
            pch = strtok (NULL, " \t\r\n");
            len= strlen(pch) +1; // count the '\0'-character also
            printf ("Mount(%d) pheap(%d): %s\n", len, pheap, pch);
            sleep(10);
            if(pheap + len >= MAX_HEAP) {
                while(1){
                    printf("Heap is full, pheap is %d en len mount is %d\n", pheap, len);
                    sleep(2*10);
                }
            }
            else {
                obj_arr[pobj]= &(heap[pheap]); 
                strcpy(&(heap[pheap]), pch);
                mounts[pdev]= pobj;
                pheap+= len;
                pobj++;
            }
            pdev++;
        }
            
    }
    pclose(cmd);
    printf ("Num of objs= %d\n",pobj);
    
    printf ("Show devices(num= %d)\n",pdev);
    printf ("dev\t\t\tmount\n");
    int i= 0;
    while(i < pdev){
        printf("%s\t\t\t%s\n",(char *)obj_arr[devices[i]],(char *)obj_arr[mounts[i]]);
        i++;
    }

  DIR *d;
  struct dirent *dir;
  d = opendir((char *)obj_arr[mounts[0]]);
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      if (dir->d_type == DT_REG) printf("%sFile: %s\n", BLUE, dir->d_name);
      else if (dir->d_type == DT_DIR) printf("%sDir: %s\n", GREEN, dir->d_name);
      else printf("%s%s\n", NORMAL_COLOR, dir->d_name);
      sleep(5);
    }
    closedir(d);
  }    
    
    while(1){
        printf("Hello World\n");
        sleep(2*10);
    }
    return 0;
}

/*
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("/proc/mounts", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        printf("Retrieved line of length %zu:\n", read);
        printf("%s", line);
    }

    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
}
*/
/*
#include <stdio.h>
#include <unistd.h>

int main(void){
    while(1){
        printf("Hello World\n");
        sleep(1);
    }
}
*/