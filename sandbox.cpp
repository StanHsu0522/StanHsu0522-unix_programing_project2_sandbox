#include <iostream>
#include <dlfcn.h>

#include <unistd.h>

using namespace std;

uid_t getuid(void) {
    cerr << "injected getuid, always return 0" << endl;
    return 0;
}