#ifndef SANDBOX_H__
#define SANDBOX_H__

#include <iostream>
#include <dlfcn.h>
#include <string.h>
#include <string>
#include <typeinfo>

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <limits.h>
#include <stdarg.h>

#define MAX_PATH_NAME_LENGTH 200

// error message output and exit
// 'x' is message string number
#define ERROR_OUTPUT(x, ...)        { ERROR_OUTPUT##x(__VA_ARGS__) }
#define ERROR_OUTPUT3(m3, ...)     fprintf(stderr, "%s ", m3); ERROR_OUTPUT2(__VA_ARGS__)
#define ERROR_OUTPUT2(m2, ...)     fprintf(stderr, "%s ", m2); ERROR_OUTPUT1(__VA_ARGS__)
#define ERROR_OUTPUT1(m1)           \
    fprintf(stderr, "%s\n", m1);    \
    exit(EXIT_FAILURE);

#define __FC_ARG1(t1, a1)       t1 a1
#define __FC_ARG2(t2, a2, ...)  t2 a2, __FC_ARG1(__VA_ARGS__)
#define __FC_ARG3(t3, a3, ...)  t3 a3, __FC_ARG2(__VA_ARGS__)
#define __FC_ARG4(t4, a4, ...)  t4 a4, __FC_ARG3(__VA_ARGS__)
#define __FC_ARG5(t5, a5, ...)  t5 a5, __FC_ARG4(__VA_ARGS__)
#define __FC_ARG6(t6, a6, ...)  t6 a6, __FC_ARG5(__VA_ARGS__)
#define __FC_FIRST_ARG(t1, a1, ...)   a1
#define __FC_SECOND_ARG(t1, a1, t2, a2, ...)    a2

#define __FC_EVERY_ARGS1(t1, a1) a1
#define __FC_EVERY_ARGS2(t2, a2, ...) a2, __FC_EVERY_ARGS1(__VA_ARGS__)
#define __FC_EVERY_ARGS3(t3, a3, ...) a3, __FC_EVERY_ARGS2(__VA_ARGS__)
#define __FC_EVERY_ARGS4(t4, a4, ...) a4, __FC_EVERY_ARGS3(__VA_ARGS__)
#define __FC_EVERY_ARGS5(t5, a5, ...) a5, __FC_EVERY_ARGS4(__VA_ARGS__)
#define __FC_EVERY_ARGS6(t6, a6, ...) a6, __FC_EVERY_ARGS5(__VA_ARGS__)

    // printf("***%s has been injected***\n", #func_name);
#define FUNC_CALL(x, func_name, return_type, macro_name, ...)                                                   \
    size_t r_len;                                                                                               \
    char *env;                                                                                                  \
    char base_dir[MAX_PATH_NAME_LENGTH];                                                                        \
    char relative_path[MAX_PATH_NAME_LENGTH];                                                                   \
    char absolute_base_dir[MAX_PATH_NAME_LENGTH];                                                               \
    char absolute_path[MAX_PATH_NAME_LENGTH];                                                                   \
    return_type (*old_##func_name)(__FC_ARG##x(__VA_ARGS__)) = NULL;                                            \
    if ((env = getenv("BASE_DIR")) == NULL)                                                                     \
        ERROR_OUTPUT(1, "getenv: environment variable BASE_DIR doesn't exist.")                                 \
    else                                                                                                        \
        strcpy(base_dir, env);                                                                                  \
    if (realpath(base_dir, absolute_base_dir) == NULL)                                                          \
        ERROR_OUTPUT(1, "realpath error")                                                                       \
    r_len = strlen(macro_name(__VA_ARGS__));                                                                    \
    memset(relative_path, 0, MAX_PATH_NAME_LENGTH);                                                             \
    strncpy(relative_path, macro_name(__VA_ARGS__), r_len);                                                     \
    for (int i=0 ;; i++) {                                                                                      \
        if (realpath(relative_path, absolute_path) != NULL)                                                     \
            break;                                                                                              \
        r_len = strrchr(macro_name(__VA_ARGS__), '/') - macro_name(__VA_ARGS__);                                \
        memset(relative_path, 0, MAX_PATH_NAME_LENGTH);                                                         \
        strncpy(relative_path, macro_name(__VA_ARGS__), r_len);                                                 \
        if (i==1 && absolute_path==NULL)                                                                        \
            ERROR_OUTPUT(2, "realpath error: path: ", relative_path)                                            \
    }                                                                                                           \
    if (strncmp(absolute_base_dir, absolute_path, strlen(absolute_base_dir)) != 0) {                            \
        fprintf(stderr, "[sandbox] %s: access to %s is not allowed\n", #func_name, macro_name(__VA_ARGS__));    \
        return (typeid(int) == typeid(return_type)) ? (return_type)-1 : (return_type)NULL;                      \
    }                                                                                                           \
    void *handle = NULL;                                                                                        \
    handle = dlopen("libc.so.6", RTLD_LAZY);                                                                    \
    if (!handle)        ERROR_OUTPUT(2, "dlopen: ", dlerror())                                                  \
    if (!old_##func_name) {                                                                                     \
        *(void**) (&old_##func_name) = dlsym(handle, #func_name);                                               \
        if (!old_##func_name)     ERROR_OUTPUT(2, "dlsym: ", dlerror())                                         \
    }                                                                                                           \
    dlclose(handle);                                                                                            \
    return old_##func_name(__FC_EVERY_ARGS##x(__VA_ARGS__));

#define FORBIDDEN_FUNC_CALL(func_name, message)             \
    fprintf(stderr, "[sandbox] %s", #func_name);            \
    fprintf(stderr, "(%s): net allowed\n", #message);       \
    errno = EACCES;                                         \
    return -1;

#endif      // SANDBOX_H__
