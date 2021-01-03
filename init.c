#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h> 
char buffer[1024] = { 0 };
int i= 0;

#define NORMAL_COLOR  "\x1B[0m"
#define GREEN  "\x1B[32m"
#define BLUE  "\x1B[34m"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>


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

//Heap objects network
#define MAX_NETWORK 4
int nets[MAX_NETWORK];
int ips[MAX_NETWORK];
int pnet= 0;

int main(void)
{
    printf("\nStart NoOSE ...\n");
    FILE *cmd;
    char * pch;

    cmd = popen("grep '^/dev' /proc/mounts", "r");
    if (cmd == NULL) {
        perror("popen");
        exit(EXIT_FAILURE);
    }
    while (fgets(buffer, sizeof(buffer), cmd)) {
        printf("%s\n", buffer);
        //sleep(10);
        pch = strtok (buffer," \t\r\n");
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
            //sleep(10);
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
    i= 0;
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
      //sleep(5);
    }
    closedir(d);
  }    
  
  
  
  
  struct ifaddrs *ifaddr, *ifa;
  struct sockaddr_in *sa;
  char *addr;
  if (getifaddrs(&ifaddr) == -1) {
          while(1){
              printf("No network found\n");
              sleep(2*10);
          }
  }
  
  for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
      if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_PACKET) continue;
      sa = (struct sockaddr_in *) ifa->ifa_addr;
      addr = inet_ntoa(sa->sin_addr);
      printf("Interface: %s\tAddress: %s\n", ifa->ifa_name, addr);      
      
      if(pnet >= MAX_NETWORK) {
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
      len= strlen(ifa->ifa_name) +1; // count the '\0'-character also
      printf ("Network(%d) pheap(%d): %s\n", len, pheap, ifa->ifa_name);
      //sleep(10);
      if(pheap + len >= MAX_HEAP) {
          while(1){
              printf("Heap is full, pheap is %d en len device is %d\n", pheap, len);
              sleep(2*10);
          }
      }
      else {
          obj_arr[pobj]= &(heap[pheap]); 
          strcpy(&(heap[pheap]), ifa->ifa_name);
          nets[pnet]= pobj;
          pheap+= len;
          pobj++;
      }
      if(pobj >= MAX_OBJS) {
          while(1){
              printf("To many objects needed, max is %d\n", pobj);
              sleep(2*10);
              }
      }
      len= strlen(addr) +1; // count the '\0'-character also
      printf ("Ip(%d) pheap(%d): %s\n", len, pheap, addr);
      //sleep(10);
      if(pheap + len >= MAX_HEAP) {
          while(1){
              printf("Heap is full, pheap is %d en len device is %d\n", pheap, len);
              sleep(2*10);
          }
      }
      else {
          obj_arr[pobj]= &(heap[pheap]); 
          strcpy(&(heap[pheap]), addr);
          ips[pnet]= pobj;
          pheap+= len;
          pobj++;
      }
      pnet++;
      
  }
  freeifaddrs(ifaddr);
  
  printf ("Show Networks(num= %d)\n",pnet);
  printf ("net\t\t\tip\n");
  i= 0;
  while(i < pnet){
      printf("%s\t\t\t%s\n",(char *)obj_arr[nets[i]],(char *)obj_arr[ips[i]]);
      i++;
  }
  
 

#include <sys/sysinfo.h>

// struct sysinfo {
               // long uptime;             /* Seconds since boot */
               // unsigned long loads[3];  /* 1, 5, and 15 minute load averages */
               // unsigned long totalram;  /* Total usable main memory size */
               // unsigned long freeram;   /* Available memory size */
               // unsigned long sharedram; /* Amount of shared memory */
               // unsigned long bufferram; /* Memory used by buffers */
               // unsigned long totalswap; /* Total swap space size */
               // unsigned long freeswap;  /* swap space still available */
               // unsigned short procs;    /* Number of current processes */
               // unsigned long totalhigh; /* Total high memory size */
               // unsigned long freehigh;  /* Available high memory size */
               // unsigned int mem_unit;   /* Memory unit size in bytes */
               // char _f[20-2*sizeof(long)-sizeof(int)]; /* Padding to 64 bytes */
           // };

struct sysinfo info;
sysinfo(&info);           
printf("totalram: %lu\n",info.totalram);
printf("freeram : %lu\n",info.freeram);
           

 
 
 
 
  
  
  
  cmd = popen("cat /proc/meminfo", "r");
  long totalmem;
  long freemem;
  int loop;
  int len;
  char ch;
  while (fgets(buffer, sizeof(buffer), cmd) != NULL) {
    printf("%s\n", buffer);
    if (strncmp(buffer,"MemTotal:",9)==0) {
        totalmem= -1;
        // Extract mem free from the line.
        for(loop=0;loop<=strlen(buffer);loop++)
        {
          ch = buffer[loop];
          if (ch == ':')
          {
             totalmem = 0L;
             continue;
          }
          if (ch == 0)
          {
              break;
          }
          if (totalmem >=0L)
          {
             if (ch >='A')
             {
                break;
             }
             if ((ch >='0') && (ch <='9'))
             {
                totalmem = totalmem * 10 + (ch-'0');
             }
          }
        }
        totalmem*= 1024;
        printf("MemTotal: %ld\n",totalmem);    
    }
    if (strncmp(buffer,"MemFree:",8)==0) {
        freemem= -1;
        // Extract mem free from the line.
        for(loop=0;loop<=strlen(buffer);loop++)
        {
          ch = buffer[loop];
          if (ch == ':')
          {
             freemem = 0;
             continue;
          }
          if (ch == 0)
          {
              break;
          }
          if (freemem >=0)
          {
             if (ch >='A')
             {
                break;
             }
             if ((ch >='0') && (ch <='9'))
             {
                freemem = freemem * 10 + (ch-'0');
             }
          }
        }
        freemem*= 1024;
        printf("MemFree: %ld\n",freemem);
    }
    sleep(5);
  }
  
  pclose(cmd); 
/*  
    struct ifaddrs *ifap, *ifa;
    struct sockaddr_in *sa;
    char *addr;

    getifaddrs (&ifap);
    for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family==AF_INET) {
            sa = (struct sockaddr_in *) ifa->ifa_addr;
            addr = inet_ntoa(sa->sin_addr);
            printf("Interface: %s\tAddress: %s\n", ifa->ifa_name, addr);
        }
    }

    freeifaddrs(ifap); 
*/  
  
  
  if (pnet <= 0) {
          while(1){
              printf("No network found\n");
              sleep(2*10);
          }
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