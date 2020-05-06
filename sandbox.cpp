#include "sandbox.h"

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

int chdir(const char *path)                                             { FUNC_CALL(1, chdir, int,__FC_FIRST_ARG, const char*, path) }
int chmod(const char *pathname, mode_t mode)                            { FUNC_CALL(2, chmod, int,__FC_FIRST_ARG, const char*, pathname, mode_t, mode) }
int chown(const char *pathname, uid_t owner, gid_t group)               { FUNC_CALL(3, chown, int,__FC_FIRST_ARG, const char *, pathname, uid_t, owner, gid_t, group) }
int creat(const char *path, mode_t mode)                                { FUNC_CALL(2, creat, int,__FC_FIRST_ARG, const char *, path, mode_t, mode) }
FILE *fopen(const char *pathname, const char *mode)                     { FUNC_CALL(2, fopen, FILE*,__FC_FIRST_ARG, const char *, pathname, const char *, mode) }
int link(const char *oldpath, const char *newpath)                      { FUNC_CALL(2, link, int,__FC_FIRST_ARG, const char *, oldpath, const char *, newpath) }
int mkdir(const char *pathname, mode_t mode)                            { FUNC_CALL(2, link, int,__FC_FIRST_ARG, const char *, pathname, mode_t, mode) }
ssize_t readlink(const char *path, char *buf, size_t bufsiz)            { FUNC_CALL(3, readlink, ssize_t,__FC_FIRST_ARG, const char *, path, char *, buf, size_t, bufsiz) }
int remove(const char *pathname)                                        { FUNC_CALL(1, remove, int,__FC_FIRST_ARG, const char *, pathname) }
int rename(const char *oldpath, const char *newpath)                    { FUNC_CALL(2, rename, int,__FC_FIRST_ARG, const char *, oldpath, const char *, newpath) }
int rmdir(const char *pathname)                                         { FUNC_CALL(1, rmdir, int,__FC_FIRST_ARG, const char *, pathname) }
int __xstat(int ver, const char * path, struct stat * stat_buf)         { FUNC_CALL(3, __xstat, int,__FC_SECOND_ARG, int, ver, const char *, path, struct stat *, stat_buf) }
int symlink(const char *target, const char *linkpath)                   { FUNC_CALL(2, symlink, int,__FC_FIRST_ARG, const char *, target, const char *, linkpath) }
int unlink(const char *pathname)                                        { FUNC_CALL(1, unlink, int,__FC_FIRST_ARG, const char *, pathname) }
DIR *opendir(const char *name)                                          { FUNC_CALL(1, opendir, DIR*,__FC_FIRST_ARG, const char*, name) }

int open(const char *pathname, int flags, ...) {
    cout << "***open has been injected***" << endl;
    size_t len;
    char *error = NULL;
    char absolute_base_dir[MAX_PATH_NAME_LENGTH];
    char absolute_path[MAX_PATH_NAME_LENGTH];
    int (*old_open)(const char *pathname, int flags, ...) = NULL;       // function pointer
    mode_t mode;

    if ( !base_dir) { ERROR_OUTPUT(1, "getenv: environment variable BASE_DIR doesn't exist."); }

    // path rosolution
    if (realpath(base_dir, absolute_base_dir) ==NULL) { ERROR_OUTPUT(1, "realpath error"); }
    if (realpath(pathname, absolute_path) ==NULL) { ERROR_OUTPUT(1, "realpath error"); }
    len = strlen(absolute_base_dir);

    // directory access check
    if (strncmp(absolute_base_dir, absolute_path, len) != 0) {
        cerr << "[sandbox] opendir: access to " << pathname << " is not allowed" << endl;
        return -1;
    }
    
    // get additional args (i.e. mode)
    va_list vl;
    va_start(vl, flags);
    mode = va_arg(vl, mode_t);
    va_end(vl);

    // callback
    if (!old_open) {
        *(void**) (&old_open) = dlsym(handle, "open");
        if ((error = dlerror()) != NULL) { ERROR_OUTPUT(2, "dlsym", error); }
    }

    return old_open(pathname, flags, mode);
}

int openat(int dirfd, const char *pathname, int flags, ...) {
    cout << "***openat has been injected***" << endl;
    size_t len;
    char *error = NULL;
    char absolute_base_dir[MAX_PATH_NAME_LENGTH];
    char absolute_path[MAX_PATH_NAME_LENGTH];
    int (*old_open)(const char *pathname, int flags, ...) = NULL;       // function pointer
    mode_t mode;

    if ( !base_dir) { ERROR_OUTPUT(1, "getenv: environment variable BASE_DIR doesn't exist."); }

    // path rosolution
    if (realpath(base_dir, absolute_base_dir) ==NULL) { ERROR_OUTPUT(1, "realpath error"); }
    if (realpath(pathname, absolute_path) ==NULL) { ERROR_OUTPUT(1, "realpath error"); }
    len = strlen(absolute_base_dir);

    // directory access check
    if (strncmp(absolute_base_dir, absolute_path, len) != 0) {
        cerr << "[sandbox] opendir: access to " << pathname << " is not allowed" << endl;
        return -1;
    }
    
    // get additional args (i.e. mode)
    va_list vl;
    va_start(vl, flags);
    mode = va_arg(vl, mode_t);
    va_end(vl);

    // callback
    if (!old_open) {
        *(void**) (&old_open) = dlsym(handle, "open");
        if ((error = dlerror()) != NULL) { ERROR_OUTPUT(2, "dlsym", error); }
    }

    return old_open(pathname, flags, mode);
}

int execl(const char *path, const char *arg, ...)           { FORBIDDEN_FUNC_CALL("execl", path) }
int execle(const char *path, const char *arg, ...)          { FORBIDDEN_FUNC_CALL("execle", path) }
int execlp(const char *file, const char *arg, ...)          { FORBIDDEN_FUNC_CALL("execlp", file) }
int execv(const char *path, char *const argv[])             { FORBIDDEN_FUNC_CALL("execv", path) }
int execve(const char *pathname, char *const argv[],
     char *const envp[])                                    { FORBIDDEN_FUNC_CALL("execve", pathname) }
int execvp(const char *file, char *const argv[])            { FORBIDDEN_FUNC_CALL("execvp", file) }
int system(const char *command)                             { FORBIDDEN_FUNC_CALL("system", command) }
