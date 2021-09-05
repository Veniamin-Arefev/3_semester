#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>


int
main(int argc, char const *argv[])
{
    int fdesc = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR );
    unsigned long long temp;
    unsigned char buf[4];
    while (scanf("%llu", &temp) == 1) {
        buf[0] = (temp & 0xF00000) >> 20;
        buf[1] = (temp & 0x0FF000) >> 12;
        buf[2] = (temp & 0x000F00) >> 8;
        buf[3] =  temp & 0x00000FF;
        write(fdesc, buf, 4);
    }
    close(fdesc);
    return 0;
}
