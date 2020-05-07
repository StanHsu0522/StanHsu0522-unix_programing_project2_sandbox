#include "main.h"

using namespace std;

int main(int argc, char* argv[]) {
    vector<char*> command;     // stores command to be executed and also parameters for that command
    map<string, string> options;

    // set default parameters
    options["sopath"] = DEFAULT_LIBRARY_PATH;
    options["basedir"] = DEFAULT_BASE_DIR;

    // process args
    parse_opt(argc, &argv[0], options, command);
    
    setenv("LD_PRELOAD", options["sopath"].c_str(), 1);     // set LD_PRELOAD to inject librery
    setenv("BASE_DIR", options["basedir"].c_str(), 1);       // set env. variable BASE_DIR in order to pass parameter to shared library

    if (command.size() - 1 == 0)                    ERROR_OUTPUT(1, "no command given.")
    if (execvp(command[0], &command[0]) == -1)      ERROR_OUTPUT(3, "Unknown command : [", command[0], "].")

    exit(EXIT_SUCCESS);
}

void parse_opt(int argc, char *argv[], map<string, string> &options, vector<char*> &command) {
    const char *optstring = "p:d:";     // for short options
                                        // If such a character is followed by a colon ':', the option requires an argument,
                                        // so getopt() places a pointer to the following text in the same argv-element,
                                        // or the text of the following argv-element, in optarg.
                                        // Two colons for optional arg (e.g. -oarg).
    int c ;
    struct option opts[] = {
        {0, 0, 0, 0}
    };

    while ((c = getopt_long(argc, argv, optstring, opts, NULL)) != -1){
        // cout << optopt << endl;      // By default, getopt() prints an error message on standard error,
                                        // and places the erroneous option character in optopt, and returns '?'.

        switch (c)
        {
        case 'p':
            options["sopath"] = optarg;
            break;
        case 'd':
            options["basedir"] = optarg;
            break;
        default:        // '?', 'h'
            cerr << "Usage: " << argv[0] << " [-p sopath] [-d basedir] [--] cmd [cmd args ...]\n";
            cerr << "\t-p: set the path to sandbox.so, default = ./sandbox.so\n"
                 << "\t-d: the base directory that is allowed to access, default = .\n"
                 << "\t--: separate the arguments for sandbox and for the executed command" << endl;
            exit(EXIT_FAILURE);
        }
    }

    // have more args?
    // optind is the index of the next element to be processed in argv[].
    while (argc > optind){
        command.push_back(argv[optind++]);
    }
    command.push_back(NULL);        // execvp expects NULL as the last element
}
