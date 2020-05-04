#include "main.h"

using namespace std;

extern string base_dir;

int main(int argc, char* argv[]) {
    vector<char*> command;     // stores command to be executed and also parameters for that command
    map<string, string> options;

    // set default parameters
    options["sopath"] = DEFAULT_LIBRARY_PATH;
    options["basedir"] = DEFAULT_BASE_DIR;

    // process args
    parse_opt(argc, &argv[0], options, command);
    // cout << "command: ";
    // for (auto it=command.begin() ; it!=command.end() ; ++it)
    //     cout << *it << " ";
    // cout << endl;
    // cout << "sopath: " << options["sopath"] << "\nbasedir: " << options["basedir"] << endl;
    
    base_dir = options["base_dir"];


    // void *shmaddr = NULL;
    // int shmid;
    // key_t key = (key_t) SHM_KEY;
    // int data_size = 1024;
    
    // shmid = shmget(key, data_size, 0666 | IPC_CREAT);
    // if (shmid == -1) {
    //     cerr << "shmget: " << strerror(errno) << endl;
    //     exit(EXIT_FAILURE);
    // }
    // shmaddr = shmat(shmid, NULL, 0);
    // if (shmaddr == (void*)-1) {
    //     cerr << "shmat: " << strerror(errno) << endl;
    //     exit(EXIT_FAILURE);
    // }
    // memset(shmaddr, 0, data_size);
    // memcpy(shmaddr, options["basedir"].data(), data_size);
    // cout << "shm setted: " << (char*)shmaddr << endl;
    // if (shmdt(shmaddr) == -1) {
    //     cerr << "shmdt: " << strerror(errno) << endl;
    //     exit(EXIT_FAILURE);
    // }


    // set LD_PRELOAD to inject librery
    setenv("LD_PRELOAD", options["sopath"].c_str(), 1);

    if ( !execute_cmd(command)) {
        cerr << "no command given." << endl;
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

bool execute_cmd(vector<char*> &command) {
    if (command.size() == 0)     return false;

    pid_t PID;
    int status;

    PID = fork();
    switch (PID)
    {
    case 0:
        if (execvp(command[0], &command[0]) == -1) {
            cerr << "Unknown command : [" << command[0] << "]." << endl;
            exit(EXIT_FAILURE);
        }
        break;
    case -1:
        cerr << "fork: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    default:
        waitpid(PID, &status, 0);
        break;
    }

    return true;
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