#include <iostream>
#include <dlfcn.h>

#include <unistd.h>
#include <sys/types.h>

using namespace std;

int main(int argc, char* argv[]) {
    cout << "UID: " << getuid() << endl;;
    return 0;
}