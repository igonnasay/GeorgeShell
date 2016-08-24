#ifndef GEORGESHELL_H
#define GEORGESHELL_H

const char PROMPT[] = "GeorgeShell > ";
const int MAX_CMD_SIZE = 100;
const int MAX_ARG_SIZE = 100;

const char SHOULD_EXIT[] = "SHOULD_EXIT";
const char EXIT_VALUE[] = "EXIT";

void printPrompt();

bool checkExitSignal();

#endif
