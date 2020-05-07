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

#define DEFAULT_LIBRARY_PATH "./sandbox.so"
#define DEFAULT_BASE_DIR "."

void parse_opt(int argc, char *argv[], std::map<std::string, std::string> &options, std::vector<char*> &command);

#endif      // MAIN_H__