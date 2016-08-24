#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>

#include "BuiltIn.h"
#include "GeorgeShell.h"

using namespace std;

const char * cmds[] = {"cd", "echo", "exit"};
const int cmdNumber = 3;

bool isBuiltIn(char * cmd){
    bool result = false;
    int length = sizeof(cmds)/sizeof(cmds[0]);
    for(int i = 0; i < length; i++) {
        if(strcmp(cmds[i], cmd) == 0) {
            result = true;
            break;
        }
    }

    return result;
}

void runBuiltIn(char * cmdAndArgs[]){
    char * cmd = cmdAndArgs[0];

    if(strcmp(cmds[0], cmd) == 0) {
        cdCmd(cmdAndArgs);
    } else if(strcmp(cmds[1], cmd) == 0) {
        echoCmd(cmdAndArgs);
    } else if(strcmp(cmds[2], cmd) == 0) {
        exitCmd(cmdAndArgs);
    }
}

void echoCmd(char * cmdAndArgs[]){
    if(!cmdAndArgs[1]) {
        return;
    }
    int i = 1;
    while(cmdAndArgs[i])
        cout << cmdAndArgs[i++] << " ";
    cout << endl;
}

void cdCmd(char * cmdAndArgs[]){
    struct stat sbuf;
    if(!cmdAndArgs[1]) {
        return;
    }
    if(stat(cmdAndArgs[1], &sbuf) < 0){
        cerr << cmdAndArgs[1] << " : no such file or directory." << endl;
        return;
    }

    if(!S_ISDIR(sbuf.st_mode)){
        cerr << cmdAndArgs[1] << " : is not a directory." << endl;
    } else {
        if(chdir(cmdAndArgs[1]) == 0){
            //char * pwd = getenv("PWD");
            //char * newPwd = new char[strlen(cmdAndArgs[1]) + strlen(pwd) + 2];
            //strcat(newPwd, pwd);
            //strcat(newPwd, "/");
            //strcat(newPwd, cmdAndArgs[1]);
            //setenv("PWD",newPwd, 1);
            //delete [] newPwd;
        }
    }
}

void exitCmd(char * cmdAndArgs[]){
    //send exit signal by setting env varialbe
    setenv(SHOULD_EXIT, EXIT_VALUE, 1);
}
