# unix_programming_project2_snadbox

Sandbox using c++ for moitoring execution of shell commands.

## Monitored Function

```c
    int chdir(const char *path);
    int chmod(const char *pathname, mode_t mode);
    int chown(const char *pathname, uid_t owner, gid_t group);
    int creat(const char *path, mode_t mode);
    FILE *fopen(const char *pathname, const char *mode);
    int link(const char *oldpath, const char *newpath);
    int mkdir(const char *pathname, mode_t mode);
    ssize_t readlink(const char *path, char *buf, size_t bufsiz);
    int remove(const char *pathname);
    int rename(const char *oldpath, const char *newpath);
    int rmdir(const char *pathname);
    int __xstat(int ver, const char * path, struct stat * stat_buf);
    int symlink(const char *target, const char *linkpath);
    int unlink(const char *pathname);
    DIR *opendir(const char *name);
    int open(const char *pathname, int flags, ...);
    int openat(int dirfd, const char *pathname, int flags, ...);

    // forbidden function
    int execl(const char *path, const char *arg, ...);
    int execle(const char *path, const char *arg, ...);
    int execlp(const char *file, const char *arg, ...);
    int execv(const char *path, char *const argv[]);
    int execve(const char *pathname, char *const argv[], char *const envp[]);
    int execvp(const char *file, char *const argv[]);
    int system(const char *command);
```

## Shared Library

Inject the shared library (e.g. sandbox.so) into a process.

```bash
    LD_PRELOAD=./sandbox.so ./sandbox ls
```

## Installation

Use Makefile

```bash
    make
```

## Usage

```bash
    ./sandbox ls
    ./sandbox -d / -- ls -al
    ./sandbox mkdir ~/aaa
    ...
```
