#include <stdio.h>

int main() {

    unsigned int ch;
    while ((ch=getchar()) != EOF) {
        if (ch >= '0' && ch <= '9') {
            ch = 1 + ch - '0';
        } else if (ch >= 'a' && ch <= 'z') {
            ch = 11 + ch - 'a';
        } else if (ch >= 'A' && ch <= 'Z') {
            ch = 37 + ch - 'A';
        } else {
            continue;
        }

        ch = ch ^ 8;
        ch = ch & 59;

//        printf("%d ", ch);
        if (ch >= 1 && ch <= 10) {
            ch = '0' + ch - 1;
        } else if (ch >= 11 && ch <= 36) {
            ch = 'a' + ch - 11;
        } else if (ch >= 37 && ch <= 62) {
            ch = 'A' + ch - 37;
        } else if (ch == 0) {
            ch = '@';
        } else if (ch == 63) {
            ch = '#';
        }
        putchar(ch);
    }
    return 0;
}
