#ifndef BULITIN_H
#define BULITIN_H

bool isBuiltIn(char * cmd);
void runBuiltIn(char * cmdAndArgs[]);
void echoCmd(char * cmdAndArgs[]);
void cdCmd(char * cmdAndArgs[]);
void exitCmd(char * cmdAndArgs[]);

#endif
