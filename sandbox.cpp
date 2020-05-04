#include "sandbox.h"

using namespace std;

void *handle = NULL;
static char *base_dir = getenv("BASE_DIR");
static int (*old_chdir)(const char *path) = NULL;
static int (*old_chmod)(const char *pathname, mode_t mode) = NULL;
static int (*old_chown)(const char *pathname, uid_t owner, gid_t group) = NULL;
static DIR *(*old_opendir)(const char *name) = NULL;

extern "C" {
void __attribute__((constructor)) contor() {
    handle = dlopen("libc.so.6", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "dlopen error\n");
        exit(EXIT_FAILURE);
    }
}

int chdir(const char *path) {
    cout << "xX" << endl;
    char *error = NULL;
    if ( !base_dir){
        cerr << "getenv: environment variable 'BASE_DIR' doesn't exist." << endl;
        exit(EXIT_FAILURE);
    }
    // directory access check
    if (strncmp(base_dir, path, strlen(path)) != 0) {
        cerr << "[sandbox] chdir: access to " << path << " is not allowed" << endl;
        errno = EACCES;     // set errno to "Permission Denied"
        return -1;
    }

    // callback
    if (!old_chdir) {
        *(void**) (&old_chdir) = dlsym(handle, "chdir");
        if ((error = dlerror()) != NULL) {
            cerr << "dlsym: " << error << endl;
            exit(EXIT_FAILURE);
        }
    }
    return old_chdir(path);
}

int chmod(const char *pathname, mode_t mode) {
    cout << "xX" << endl;
    char *error = NULL;
    if ( !base_dir){
        cerr << "getenv: environment variable 'BASE_DIR' doesn't exist." << endl;
        exit(EXIT_FAILURE);
    }
    // directory access check
    if (strncmp(base_dir, pathname, strlen(pathname)) != 0) {
        cerr << "[sandbox] chmod: access to " << pathname << " is not allowed" << endl;
        errno = EACCES;     // set errno to "Permission Denied"
        return -1;
    }

    // callback
    if (!old_chmod) {
        *(void**) (&old_chmod) = dlsym(handle, "chmod");
        if ((error = dlerror()) != NULL) {
            cerr << "dlsym: " << error << endl;
            exit(EXIT_FAILURE);
        }
    }
    return old_chmod(pathname, mode);
}

int chown(const char *pathname, uid_t owner, gid_t group) {
    cout << "xX" << endl;
    char *error = NULL;
    if ( !base_dir){
        cerr << "getenv: environment variable 'BASE_DIR' doesn't exist." << endl;
        exit(EXIT_FAILURE);
    }
    // directory access check
    if (strncmp(base_dir, pathname, strlen(pathname)) != 0) {
        cerr << "[sandbox] chown: access to " << pathname << " is not allowed" << endl;
        errno = EACCES;     // set errno to "Permission Denied"
        return -1;
    }

    // callback
    if (!old_chown) {
        *(void**) (&old_chown) = dlsym(handle, "chown");
        if ((error = dlerror()) != NULL) {
            cerr << "dlsym: " << error << endl;
            exit(EXIT_FAILURE);
        }
    }
    return old_chown(pathname, owner, group);
}


DIR *opendir(const char *name) {
    cout << "xX" << endl;
    char *error = NULL;
    if ( !base_dir){
        cerr << "getenv: environment variable 'BASE_DIR' doesn't exist." << endl;
        exit(EXIT_FAILURE);
    }

    // directory access check
    if (strncmp(base_dir, name, strlen(name)) != 0) {
        cerr << "[sandbox] opendir: access to " << name << " is not allowed" << endl;
        return NULL;
    }

    // callback
    if (!old_opendir) {
        // void *handle = dlopen("libc.so.6", RTLD_LAZY);
        // if (handle) {
            *(void**) (&old_opendir) = dlsym(handle, "opendir");
            // *(void**) (&old_opendir) = dlsym(RTLD_NEXT, "opendir");
            if ((error = dlerror()) != NULL) {
                cerr << "dlsym: " << error << endl;
                exit(EXIT_FAILURE);
            }
        // }
    }
    return old_opendir(name);
}

int execl(const char *path, const char *arg, ...) {
    cerr << "[sandbox] execl(" << path << "): ";
    cerr << "not allowed" << endl;
    errno = EACCES;     // set errno to "Permission Denied"
    return -1;
}

int execle(const char *path, const char *arg, ...) {
    cerr << "[sandbox] execle(" << path << "): ";
    cerr << "not allowed" << endl;
    errno = EACCES;     // set errno to "Permission Denied"
    return -1;
}

int execlp(const char *file, const char *arg, ...) {
    cerr << "[sandbox] execlp(" << file << "): ";
    cerr << "not allowed" << endl;
    errno = EACCES;     // set errno to "Permission Denied"
    return -1;
}

int execv(const char *path, char *const argv[]) {
    cerr << "[sandbox] execv(" << path << "): ";
    cerr << "not allowed" << endl;
    errno = EACCES;     // set errno to "Permission Denied"
    return -1;
}

int execve(const char *pathname, char *const argv[], char *const envp[]) {
    cerr << "[sandbox] execve(" << pathname << "): ";
    cerr << "not allowed" << endl;
    errno = EACCES;     // set errno to "Permission Denied"
    return -1;
}

int execvp(const char *file, char *const argv[]) {
    cerr << "[sandbox] execvp(" << file << "): ";
    cerr << "not allowed" << endl;
    errno = EACCES;     // set errno to "Permission Denied"
    return -1;
}

int system(const char *command) {
    cerr << "[sandbox] system(" << command << "): ";
    cerr << "not allowed" << endl;
    errno = EACCES;
    return -1;
}
}