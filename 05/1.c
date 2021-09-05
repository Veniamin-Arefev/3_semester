#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


int
main(int argc, char const *argv[])
{
    struct stat stbuf;
    unsigned long long totalsize = 0;
    for (int i = 1; i < argc; i ++) {
        if (lstat(argv[i], &stbuf) != -1) {
            if ((stbuf.st_size % 1024 == 0) && stbuf.st_nlink == 1) {
                if  (((stbuf.st_mode & S_IFMT) == S_IFREG) && ((stbuf.st_mode & S_IFMT) != S_IFLNK)) {
                    totalsize += stbuf.st_size;
                }
            }
        }
    }
    printf("%lld", totalsize);
    return 0;
}
