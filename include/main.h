#ifndef MAIN_H__
#define MAIN_H__

#include <iostream>
#include <map>
#include <string>
#include <cstring>
#include <vector>
#include <getopt.h>
#include <utility>
#include <sys/wait.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define DEFAULT_LIBRARY_PATH "./sandbox.so"
#define DEFAULT_BASE_DIR "."

// error message output and exit
// 'x' is message string number
#define ERROR_OUTPUT(x, ...)        { ERROR_OUTPUT##x(__VA_ARGS__) }
#define ERROR_OUTPUT3(m3, ...)     fprintf(stderr, "%s ", m3); ERROR_OUTPUT2(__VA_ARGS__)
#define ERROR_OUTPUT2(m2, ...)     fprintf(stderr, "%s ", m2); ERROR_OUTPUT1(__VA_ARGS__)
#define ERROR_OUTPUT1(m1)           \
    fprintf(stderr, "%s\n", m1);    \
    exit(EXIT_FAILURE);
#define ARG_ERROR                                                                                   \
    cerr << "Usage: " << argv[0] << " [-p sopath] [-d basedir] [--] cmd [cmd args ...]\n";          \
    cerr << "\t-p: set the path to sandbox.so, default = ./sandbox.so\n"                            \
            << "\t-d: the base directory that is allowed to access, default = .\n"                  \
            << "\t--: separate the arguments for sandbox and for the executed command" << endl;     \
    exit(EXIT_FAILURE);

void parse_opt(int argc, char *argv[], std::map<std::string, std::string> &options, std::vector<char*> &command);

#endif      // MAIN_H__