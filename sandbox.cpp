#include <iostream>
#include <dlfcn.h>

#include <unistd.h>

using namespace std;

static uid_t (*uni_getuid)(void) = NULL;        // function pointer

extern "C" {
    uid_t getuid(void) {
        if(uni_getuid == NULL) {
            void *handle = dlopen("libc.so.6", RTLD_LAZY);
            if(handle != NULL){
                *(void**) (&uni_getuid) = dlsym(handle, "getuid");
            }
        }
        cerr << "injected getuid, always return 0" << endl;
        if(uni_getuid != NULL){
            cerr << "real uid = " << uni_getuid() << endl;
        }

        return 0;
    }
}