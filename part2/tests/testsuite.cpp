#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int cpid;
    if(cpid = fork())
    {
        
    }
    else
    {
        if(cpid = fork())
        {
            chdir("..");
        }
        else
        {
            puts("running tests");
            char *temp[8];
            temp[0] = "make";
            temp[1] = "build";
            temp[2] = ";";
            temp[3] = "./main";
            execv(temp[0], temp);
            exit(0);
        }

    }
    return 0;
}