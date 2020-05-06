#ifndef SANDBOX_H__
#define SANDBOX_H__

#include <iostream>
#include <dlfcn.h>
#include <cstring>
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
#define ERROR_OUTPUT(x, ...)  ERROR_OUTPUT##x(__VA_ARGS__)
#define ERROR_OUTPUT3(m3, ...)     cerr << m3 << " "; ERROR_OUTPUT2(__VA_ARGS__)
#define ERROR_OUTPUT2(m2, ...)     cerr << m2 << " "; ERROR_OUTPUT1(__VA_ARGS__)
#define ERROR_OUTPUT1(m1)  \
    cerr << m1;            \
    cerr << endl;          \
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

#define FUNC_CALL(x, func_name, return_type, macro_name, ...)                                                               \
    cout << "***" << #func_name << " has been injected***" << endl;                                                         \
    size_t len, r_len;                                                                                                      \
    char *error = NULL;                                                                                                     \
    char relative_path[MAX_PATH_NAME_LENGTH];                                                                               \
    char absolute_base_dir[MAX_PATH_NAME_LENGTH];                                                                           \
    char absolute_path[MAX_PATH_NAME_LENGTH];                                                                               \
    string f_name(#func_name);                                                                                              \
    return_type (*old_##func_name)(__FC_ARG##x(__VA_ARGS__)) = NULL;                                                        \
    if ( !base_dir) { ERROR_OUTPUT(1, "getenv: environment variable BASE_DIR doesn't exist."); }                            \
    if (f_name == "mkdir" || f_name == "creat")                                                                             \
        r_len = strrchr(macro_name(__VA_ARGS__), '/') - macro_name(__VA_ARGS__);                                            \
    else        r_len = strlen(macro_name(__VA_ARGS__));                                                                    \
    strncpy(relative_path, macro_name(__VA_ARGS__), r_len);                                                                 \
    cout << "path: " << relative_path << endl;      \
    if (realpath(base_dir, absolute_base_dir) ==NULL) { ERROR_OUTPUT(1, "realpath error"); }                                \
    if (realpath(relative_path, absolute_path) ==NULL) { ERROR_OUTPUT(1, "realpath error"); }                               \
    len = strlen(absolute_base_dir);                                                                                        \
    if (strncmp(absolute_base_dir, absolute_path, len) != 0) {                                                              \
        cerr << "[sandbox] " << #func_name << ": access to " << macro_name(__VA_ARGS__) << " is not allowed" << endl;       \
        return (typeid(int) == typeid(return_type)) ? (return_type)-1 : (return_type)NULL;                                  \
    }                                                                                                                       \
    if (!old_##func_name) {                                                                                                 \
        *(void**) (&old_##func_name) = dlsym(handle, #func_name);                                                           \
        if ((error = dlerror()) != NULL) { ERROR_OUTPUT(2, "dlsym", error); }                                               \
    }                                                                                                                       \
    return old_##func_name(__FC_EVERY_ARGS##x(__VA_ARGS__));

#define FORBIDDEN_FUNC_CALL(func_name, message)     \
    cerr << "[sandbox] " << func_name;              \
    cerr <<  "(" << message << "): ";               \
    cerr << "not allowed" << endl;                  \
    errno = EACCES;                                 \
    return -1;

#endif      // SANDBOX_H__
