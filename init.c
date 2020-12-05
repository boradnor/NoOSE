#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

FILE *popen(const char *command, const char *mode);
int pclose(FILE *stream);

int main(void)
{
    FILE *cmd;
    char result[1024];

    cmd = popen("grep '^/dev' /proc/mounts", "r");
    if (cmd == NULL) {
        perror("popen");
        exit(EXIT_FAILURE);
    }
    while (fgets(result, sizeof(result), cmd)) {
        printf("%s", result);
    }
    pclose(cmd);
    
    while(1){
        printf("Hello World\n");
        sleep(1);
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