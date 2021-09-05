#include <stdio.h>
#include <string.h>

int
parse_rwx_permissions(const char *str)
{
    const char s[] = "rwxrwxrwx";
    if (str == NULL) {
        return -1;
    }
    int answer = 0;
    const unsigned long long length = sizeof(s);
    for (int i = 0; i < length; i++) {
        answer <<= 1;
        if (str[i] == s[i]) {
            answer = answer | 1;
        } else if (str[i] != '-') {
            return -1;
        }
    }
    return answer >> 1;
}
