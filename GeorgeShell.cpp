#include <stdlib.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>

#include "Parser.h"
#include "BuiltIn.h"
#include "NonBuiltIn.h"
#include "Redirection.h"
#include "Pipeline.h"
#include "GeorgeShell.h"
using namespace std;

void printPrompt() {
    cout << PROMPT;
}

bool checkExitSignal() {
    char * exit = getenv(SHOULD_EXIT);
    if(!exit)
        return false;
    return strcmp(exit, EXIT_VALUE) == 0 ? true : false;
}

int main(int argc, char *argv[]){

    while(true){
        printPrompt();

        string line;
        getline(cin, line);
        char * cline = new char[line.length() + 1];
        strcpy(cline, line.c_str());

        char ** subCmds = new char * [MAX_CMD_SIZE];
        int cmdSize = parseLine(cline, subCmds);
        subCmds[cmdSize] = nullptr;

        //convert an array of cmds into a linked list
        //each node contains a cmd and a pair of file descriptors
        Pipeline * pipeline = convertToPipeline(subCmds);
        Pipeline * cursor = pipeline;
        while(cursor){
            if(cursor->next){
                pipe(cursor->fds);
                cout << "Pipe " << cursor->fds[0] << " " << cursor->fds[1] <<endl;
            }

            //find redirection in a single cmd
            Redirection * redirection = findRedirection(cursor->cmd);

            //tokenize a single cmd
            char ** cmdAndArgs = new char * [MAX_ARG_SIZE + 1 + 1];
            int tokenSize = parseCmd(cursor->cmd, cmdAndArgs);
            cmdAndArgs[tokenSize] = nullptr;

            if(tokenSize == 0) {
                delete [] cmdAndArgs;
                cmdAndArgs = nullptr;
                freeRedirection(redirection);
                break;
            }

            if(isBuiltIn(cmdAndArgs[0])){
                runBuiltIn(cmdAndArgs);
                delete [] cmdAndArgs;
                cmdAndArgs = nullptr;
                freeRedirection(redirection);
                break;
            } else {
                int pid, status;
                pid = fork();
                if(pid == 0){
                    if(cursor->prev){
                        close(cursor->prev->fds[1]);
                        dup2(cursor->prev->fds[0], 0);
                        close(cursor->prev->fds[0]);
                    }
                    if(cursor->next){
                        close(cursor->fds[0]);
                        dup2(cursor->fds[1], 1);
                        close(cursor->fds[1]);
                    }
                    if(handleRedirection(redirection) == 0)
                        runNonBuiltIn(cmdAndArgs);
                    cerr << "Failed to run command :" << cmdAndArgs[0] << endl;
                    exit(EXIT_FAILURE);
                } else {
                    if(cursor->prev){
                        close(cursor->prev->fds[0]);
                        close(cursor->prev->fds[1]);
                    }
                    wait(&status);
                }
            }

            delete [] cmdAndArgs;
            cmdAndArgs = nullptr;
            freeRedirection(redirection);

            cursor = cursor->next;
        }
        waitpid(0,0,0);

        closeOpenPipes(pipeline);
        freePipeline(pipeline);
        delete [] subCmds;
        subCmds = nullptr;
        delete [] cline;
        cline = nullptr;

        if(checkExitSignal() == true)
            break;
    }
    return 0;
}
