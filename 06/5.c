#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

enum
{
    OWNER_SHIFT = 6u,
    GROUP_SHIFT = 3u,
    OTHER_SHIFT = 0u
};

struct Task
{
    unsigned uid;
    int gid_count;
    unsigned *gids;
};

int myaccess(const struct stat *stb, const struct Task *task, int access) {
    if (task->uid == 0) {
        return 1;
    }
    if (task->uid == stb->st_uid) {
        if (((stb->st_mode >> OWNER_SHIFT) & access) == access) {
            return 1;
        } else {
            return 0;
        }
    }
    for (int i = 0; i < task->gid_count; i++) {
        if (task->gids[i] == stb->st_gid) {
            if (((stb->st_mode >> GROUP_SHIFT) & access) == access) {
                return 1;
            } else {
                return 0;
            }        }
    }
    if (((stb->st_mode >> OTHER_SHIFT) & access) == access) {
        return 1;
    } else {
        return 0;
    }
}
