#include <stdlib.h>
#include <string>
#include <iostream>

#include "Parser.h"

using namespace std;

int findTokens(char * line, char * delimiters, char * cmdAndArgs[]){
    int i = 0;
    char * token = strtok(line, delimiters);
    while(token) {
        char *cstr = new char[strlen(token) + 1];
        cstr[strlen(token)] = '\0';
        strcpy(cstr, token);
        cmdAndArgs[i++] = cstr;
        token = strtok(nullptr, delimiters);
    }
    return i;
}

int parseLine(char * line, char * subCmds[]){
    char * d = (char *)"|";
    return findTokens(line, d, subCmds);
}

int parseCmd(char * cmd, char * cmdAndArgs[]){
    char * d = (char *)" \t";
    return findTokens(cmd, d, cmdAndArgs);
}
