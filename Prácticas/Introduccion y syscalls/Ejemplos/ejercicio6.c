#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, const char **argv){
    //const char *arg = "ls -la";

    //system(arg);


    int childPid = fork();

    if (childPid == 0)
    {
        int i = 0;
        while (argv[1][i] != '\0')
        {
            i++;
        }
        
        char arg[i];
        
        strcpy(arg, argv[1]);
        
        //printf("%s\n", arg);
        
        char espacio[] = " ";
        
        if (argc > 2)
        {
            strcat(arg, espacio);
        }

        for (int i = 2; i < argc; i++)
        {
            strcat(arg, argv[i]);
            if (i < argc - 1)
            {
                strcat(arg, espacio);
            }

            //printf("%s\n", arg);
        }
        
        execl("/bin/sh", "sh", "-c", arg, (char *) NULL);
        exit(EXIT_SUCCESS);
    }

    wait(NULL);

    return 0;
}