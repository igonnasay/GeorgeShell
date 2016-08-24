#include <string.h>
using namespace std;

#ifndef PARSECMD_H
#define PARSECMD_H

int parseLine(char * line, char * subCmds[]);

int parseCmd(char * line, char * cmdAndArgs[]);

#endif
