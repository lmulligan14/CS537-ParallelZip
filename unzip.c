#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    
    char *numStr = "1/4 \"%3d\"";
    char *charStr = "1/1 \"%_c\n\"";
    char *args[] = {"/bin/hexdump", "-s", NULL, "-n", NULL, "-e", NULL, argv[1], NULL};

    
    struct stat s;
    int fd = open(argv[1], O_RDONLY);
	fstat(fd, &s);
    int size = s.st_size;
    close(fd);

    int byte = 0;
    char byteStr[10];
    args[2] = "0";
    for (int i = 0; i < size; i += 5)
    {
        args[4] = "4";
        args[6] = numStr;
        if (fork() == 0)
            execv(args[0], args);
        else
            wait(NULL);

        args[4] = "1";
        args[6] = charStr;
        byte += 4;
        sprintf(byteStr, "%d", byte);
        args[2] = byteStr;
        if (fork() == 0)
            execv(args[0], args);
        else
            wait(NULL);

        byte += 1;
        sprintf(byteStr, "%d", byte);
        args[2] = byteStr;
    }

    return 0;
}