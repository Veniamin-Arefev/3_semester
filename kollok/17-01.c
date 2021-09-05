#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>

// Virt_A == 23 bits of virt page number and 9 bits of offset
// in Page_Tab 23 bits of phys page number and 9 bits of flags, last of them is present bit

unsigned To_PA(unsigned *Page_Tab, unsigned Virt_A, int PID) {
    unsigned virt_page_content = Page_Tab[Virt_A >> 9];
    unsigned offset = Virt_A & 0777;
    if (virt_page_content & 1) {
        return (virt_page_content & 0xFFFFFE00) | offset;
    } else {
        kill(PID, SIGPL);
        _exit(No_Page);
    }
}

int
main(int argc, char const *argv[]) {


    return 0;
}
