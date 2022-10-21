#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

int main(int argc, char **argv)
{
    char *app_name = argv[0];
    char *count = argv[1];
    char *dev_name = "/dev/hcsr04";
    int fd = -1;
    char c;
    char d[64];
    int count_num = atoi(count);
    fd = open(dev_name, O_RDWR);
    c = 1;

    if (count_num > 0)
    {
        for (int i = 1; i <= (count_num); i++)
        {
            printf("%d iteration of %d\n", i, count_num);
            write(fd, &c, 1);
            printf("Triggered pulse. Attempting to read pulse");
            read(fd, &d, 64);
            printf("%s\n", &d);
            sleep(1);
        }
    }
    else
    {
        printf("Reading Infinitely");
        while (1)
        {
            write(fd, &c, 1);
            read(fd, &d, 64);
            printf("%s\n", &d);
            sleep(1);
        }
    }
    close(fd);
    return 0;
}
