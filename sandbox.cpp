#include "sandbox.h"

using namespace std;

string base_dir;
// int shmid;

// static uid_t (*uni_getuid)(void) = NULL;        // function pointer
static DIR *(*old_opendir)(const char *name) = NULL;

extern "C" {

// uid_t getuid(void) {
//     if(uni_getuid == NULL) {
//         void *handle = dlopen("libc.so.6", RTLD_LAZY);
//         if(handle != NULL){
//             *(void**) (&uni_getuid) = dlsym(handle, "getuid");
//         }
//     }
//     cerr << "injected getuid, always return 0" << endl;
//     if(uni_getuid != NULL){
//         cerr << "real uid = " << uni_getuid() << endl;
//     }
// }

// __attribute__((constructor))
// static void init(string base_dir) {

//     cout << "constructor in" << endl;
//     void *shmaddr = NULL;
//     key_t key = (key_t) SHM_KEY;
//     shmid = shmget(key, 1024, 0666 | IPC_CREAT);
//     if (shmid == -1) {
//         cerr << "shmget: " << strerror(errno) << endl;
//         exit(EXIT_FAILURE);
//     }
//     shmaddr = shmat(shmid, NULL, 0);
//     if (shmaddr == (void*)-1) {
//         cerr << "shmat: " << strerror(errno) << endl;
//         exit(EXIT_FAILURE);
//     }

//     base_dir = (char*)shmaddr;

//     if (shmdt(shmaddr) == -1) {
//         cerr << "shmdt: " << strerror(errno) << endl;
//         exit(EXIT_FAILURE);
//     }
// }

// __attribute__((destructor))
// static void after(string base_dir) {
//     if (shmctl(shmid, IPC_RMID, 0) == -1) {
//         cerr << "shmctl: " << strerror(errno) << endl;
//         exit(EXIT_FAILURE);
//     }
// }

DIR *opendir(const char *name) {
    char* error = NULL;
    // if (options["basedir"].compare(name) != 0) {
    //     cerr << "[sandbox] opendir: access to " << name << " is not allowed" << endl;
    //     return NULL;
    // }

    if (!base_dir.empty())       cout << "basedir: " << base_dir << endl;

    if (!old_opendir) {
        void *handle = dlopen("libc.so.6", RTLD_LAZY);
        if (handle) {
            *(void**) (&old_opendir) = dlsym(handle, "opendir");
            // *(void**) (&old_opendir) = dlsym(RTLD_NEXT, "opendir");
            if ((error = dlerror()) != NULL) {
                cerr << "dlsym: " << error << endl;
                exit(EXIT_FAILURE);
            }
        }
    }
    return old_opendir(name);
}

int execl(const char *path, const char *arg, ...) {
    cerr << "[sandbox] execl(" << path << "): ";
    cerr << "not allowed" << endl;
    errno = EACCES;     // set errno to "Permission Denied"
    return -1;
}

int execle(const char *path, const char *arg, char * const envp[]) {
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