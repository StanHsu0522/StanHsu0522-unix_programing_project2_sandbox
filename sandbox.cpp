#include "sandbox.h"

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


#define FUNC_CALL(x, func_name, return_type, ...)                                                                           \
    cout << "***" << #func_name << " has been injected***" << endl;                                                         \
    size_t len;                                                                                                             \
    char *error = NULL;                                                                                                     \
    char absolute_base_dir[MAX_PATH_NAME_LENGTH];                                                                           \
    char absolute_path[MAX_PATH_NAME_LENGTH];                                                                               \
    return_type (*old_##func_name)(__FC_ARG##x(__VA_ARGS__)) = NULL;                                                        \
    if ( !base_dir) { ERROR_OUTPUT(1, "getenv: environment variable BASE_DIR doesn't exist."); }                            \
    if (realpath(base_dir, absolute_base_dir) ==NULL) { ERROR_OUTPUT(1, "realpath error"); }                                \
    if (realpath(__FC_FIRST_ARG(__VA_ARGS__), absolute_path) ==NULL) { ERROR_OUTPUT(1, "realpath error"); }                 \
    len = strlen(absolute_base_dir);                                                                                        \
    if (strncmp(absolute_base_dir, absolute_path, len) != 0) {                                                              \
        cerr << "[sandbox] " << #func_name << ": access to " << __FC_FIRST_ARG(__VA_ARGS__) << " is not allowed" << endl;   \
        return (typeid(int) == typeid(return_type)) ? (return_type)-1 : (return_type)NULL;                                  \
    }                                                                                                                       \
    if (!old_##func_name) {                                                                                                 \
        *(void**) (&old_##func_name) = dlsym(handle, #func_name);                                                           \
        if ((error = dlerror()) != NULL) { ERROR_OUTPUT(2, "dlsym", error); }                                               \
    }                                                                                                                       \
    return old_##func_name(__FC_EVERY_ARGS##x(__VA_ARGS__));

#define FUNC_CALL_OPENAT(x, func_name, return_type, ...)                                                                    \
    cout << "***" << #func_name << " has been injected***" << endl;                                                         \
    size_t len;                                                                                                             \
    char *error = NULL;                                                                                                     \
    char absolute_base_dir[MAX_PATH_NAME_LENGTH];                                                                           \
    char absolute_path[MAX_PATH_NAME_LENGTH];                                                                               \
    return_type (*old_##func_name)(__FC_ARG##x(__VA_ARGS__)) = NULL;                                                        \
    if ( !base_dir) { ERROR_OUTPUT(1, "getenv: environment variable BASE_DIR doesn't exist."); }                            \
    if (realpath(base_dir, absolute_base_dir) ==NULL) { ERROR_OUTPUT(1, "realpath error"); }                                \
    if (realpath(__FC_SECOND_ARG(__VA_ARGS__), absolute_path) ==NULL) { ERROR_OUTPUT(1, "realpath error"); }                \
    len = strlen(absolute_base_dir);                                                                                        \
    if (strncmp(absolute_base_dir, absolute_path, len) != 0) {                                                              \
        cerr << "[sandbox] " << #func_name << ": access to " << __FC_SECOND_ARG(__VA_ARGS__) << " is not allowed" << endl;  \
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


using namespace std;

void *handle = NULL;
static char *base_dir = getenv("BASE_DIR");


void __attribute__((constructor)) contor() {
    handle = dlopen("libc.so.6", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "dlopen error\n");
        exit(EXIT_FAILURE);
    }
}

void __attribute((destructor)) detor() {
    dlclose(handle);
}
int chdir(const char *path)                                             { FUNC_CALL(1, chdir, int, const char*, path) }
int chmod(const char *pathname, mode_t mode)                            { FUNC_CALL(2, chmod, int, const char*, pathname, mode_t, mode) }
int chown(const char *pathname, uid_t owner, gid_t group)               { FUNC_CALL(3, chown, int, const char *, pathname, uid_t, owner, gid_t, group) }
int creat(const char *path, mode_t mode)                                { FUNC_CALL(2, creat, int, const char *, path, mode_t, mode) }
FILE *fopen(const char *pathname, const char *mode)                     { FUNC_CALL(2, fopen, FILE*, const char *, pathname, const char *, mode) }
int link(const char *oldpath, const char *newpath)                      { FUNC_CALL(2, link, int, const char *, oldpath, const char *, newpath) }
int mkdir(const char *pathname, mode_t mode)                            { FUNC_CALL(2, link, int, const char *, pathname, mode_t, mode) }
int open(const char *pathname, int flags)                               { FUNC_CALL(2, open, int, const char *, pathname, int, flags) }
int open(const char *pathname, int flags, mode_t mode)                  { FUNC_CALL(3, open, int, const char *, pathname, int, flags, mode_t, mode) }
int openat(int dirfd, const char *pathname, int flags)                  { FUNC_CALL_OPENAT(3, openat, int, int, dirfd, const char *, pathname, int, flags) }
int openat(int dirfd, const char *pathname, int flags, mode_t mode)     { FUNC_CALL_OPENAT(4, openat, int, int, dirfd, const char *, pathname, int, flags, mode_t, mode) }
ssize_t readlink(const char *path, char *buf, size_t bufsiz)            { FUNC_CALL(3, readlink, ssize_t, const char *, path, char *, buf, size_t, bufsiz) }
int remove(const char *pathname)                                        { FUNC_CALL(1, remove, int, const char *, pathname) }
int rename(const char *oldpath, const char *newpath)                    { FUNC_CALL(2, rename, int, const char *, oldpath, const char *, newpath) }
int rmdir(const char *pathname)                                         { FUNC_CALL(1, rmdir, int, const char *, pathname) }
int stat(const char * __file, struct stat * __buf)                    { FUNC_CALL(2, stat, int, const char *, __file, struct stat *, __buf) }
int symlink(const char *target, const char *linkpath)                   { FUNC_CALL(2, symlink, int, const char *, target, const char *, linkpath) }
int unlink(const char *pathname)                                        { FUNC_CALL(1, unlink, int, const char *, pathname) }
DIR *opendir(const char *name)                                          { FUNC_CALL(1, opendir, DIR*, const char*, name) }
// }
//     size_t len;
//     char *error = NULL;
//     char absolute_base_dir[MAX_PATH_NAME_LENGTH];
//     char absolute_path[MAX_PATH_NAME_LENGTH];
//     DIR *(*old_opendir)(const char *name) = NULL;

//     if ( !base_dir) { ERROR_OUTPUT(1, "getenv: environment variable BASE_DIR doesn't exist."); }

//     if (realpath(base_dir, absolute_base_dir) ==NULL) { ERROR_OUTPUT(1, "realpath error"); }
//     if (realpath(name, absolute_path) ==NULL) { ERROR_OUTPUT(1, "realpath error"); }
//     len = strlen(absolute_base_dir);

//     // directory access check   
//     if (strncmp(absolute_base_dir, absolute_path, len) != 0) {
//         cerr << "[sandbox] opendir: access to " << name << " is not allowed" << endl;
//         return NULL;
//     }

//     // callback
//     if (!old_opendir) {
//         *(void**) (&old_opendir) = dlsym(handle, "opendir");
//         if ((error = dlerror()) != NULL) { ERROR_OUTPUT(2, "dlsym", error); }
//     }

//     return old_opendir(name);
// }

int execl(const char *path, const char *arg, ...)           { FORBIDDEN_FUNC_CALL("execl", path) }
int execle(const char *path, const char *arg, ...)          { FORBIDDEN_FUNC_CALL("execle", path) }
int execlp(const char *file, const char *arg, ...)          { FORBIDDEN_FUNC_CALL("execlp", file) }
int execv(const char *path, char *const argv[])             { FORBIDDEN_FUNC_CALL("execv", path) }
int execve(const char *pathname, char *const argv[],
     char *const envp[])                                    { FORBIDDEN_FUNC_CALL("execve", pathname) }
int execvp(const char *file, char *const argv[])            { FORBIDDEN_FUNC_CALL("execvp", file) }
int system(const char *command)                             { FORBIDDEN_FUNC_CALL("system", command) }
