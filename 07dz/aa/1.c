#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

enum
{
    PAGE_COUNT = 16,
    PAGE_SIZE = 512,
    FIRST_SEVEN_BYTES = 0177000u,
    FIRST_SEVEN_SHIFT = 9u,
    LAST_NINE_BYTES = 0777u
};



int
main(int argc, char const *argv[]) {

    unsigned int start;
    sscanf(argv[2], "%x", &start);

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        printf("Cant create file\n");
        return 1;
    }

    size_t size = getpagesize() * PAGE_COUNT;

    void *fpt = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
    if (fpt == MAP_FAILED) {
        printf("Cant map file\n");
        return 1;
    }

    u_int8_t *memorypt = (u_int8_t *) fpt;
    unsigned int virtaddress;
    while (scanf("%x", &virtaddress) == 1) {
        unsigned int virtpagename = (virtaddress & FIRST_SEVEN_BYTES) >> FIRST_SEVEN_SHIFT;
        unsigned int physpagename =  (*((u_int16_t *) (&memorypt[start + 2*virtpagename])) & FIRST_SEVEN_BYTES) \
                >> FIRST_SEVEN_SHIFT;
        unsigned int physaddress =  physpagename * PAGE_SIZE;
        unsigned int offset = virtaddress & LAST_NINE_BYTES;
        physaddress += offset;
        unsigned int out = *((u_int16_t *) (&memorypt[physaddress]));
        printf("%u\n", out);
    }

    munmap(fpt, size);
    close(fd);

    return 0;
}
