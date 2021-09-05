#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>


enum
{
    DYNAMIC_LIB_NAME = 1,
    FUNC_NAME = 2,
    SYG_NAME = 3,
    COM_LINE_ARGS_START = 4
};


int
main(int argc, char const *argv[]) {

    void *handle;
    void (*voidfunc)();
    int (*intfunc)();
    double (*doublefunc)();
    char *(*strfunc)();

    setbuf(stdin, NULL);

    char *error;
    handle = dlopen(argv[DYNAMIC_LIB_NAME], RTLD_LAZY);
    if (!handle) {
        fputs(dlerror(), stderr);
        exit(1);
    }
    char retval = argv[SYG_NAME][0];
    if (retval == 'v') {
        voidfunc = dlsym(handle, argv[FUNC_NAME]);
    } else if (retval == 'i') {
        intfunc = dlsym(handle, argv[FUNC_NAME]);
    } else if (retval == 'd') {
        doublefunc = dlsym(handle, argv[FUNC_NAME]);
    } else if (retval == 's') {
        strfunc = dlsym(handle, argv[FUNC_NAME]);
    }

    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "%s\n", error);
        exit(1);
    }

    int args[16];
    memset(args, 0, 64);
    int inindex = 1, argsindex = 0, comlineindex = COM_LINE_ARGS_START;
    //inindex - index in function signature
    //argsindex - index in args array
    //comlineindex - index of arguments specified by command line arguments
    while (argv[SYG_NAME][inindex] != '\0') {
        if (argv[SYG_NAME][inindex] == 'i') {
            int tempint;
            sscanf(argv[comlineindex], "%d", &tempint);
            args[argsindex] = tempint;
            argsindex++;
        } else if (argv[SYG_NAME][inindex] == 'd') {
            double tempdouble;
            sscanf(argv[comlineindex], "%lf", &tempdouble);
            char *doublewraddr = (char *) &args[argsindex];
            *((double *) doublewraddr) = tempdouble;
            argsindex += 2;
        } else if (argv[SYG_NAME][inindex] == 's') {
            args[argsindex] = (int) argv[comlineindex];
            argsindex++;
        }
        comlineindex++;
        inindex++;
    }

    if (retval == 'v') {
        voidfunc(args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10],
                args[11], args[12], args[13], args[14], args[15]);
    } else if (retval == 'i') {
        int tempint = intfunc(args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9],
                args[10], args[11], args[12], args[13], args[14], args[15]);
        printf("%d\n", tempint);
    } else if (retval == 'd') {
        double tempdouble = doublefunc(args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8],
                args[9], args[10], args[11], args[12], args[13], args[14], args[15]);
        printf("%.10g\n", tempdouble);
    } else if (retval == 's') {
        char *tempstr = strfunc(args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8],
                args[9], args[10], args[11], args[12], args[13], args[14], args[15]);
        printf("%s\n", tempstr);
    }
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "%s\n", error);
        exit(1);
    }

    dlclose(handle);

    return 0;
}
