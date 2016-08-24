#include <stdlib.h>

#ifndef REDIRECTION_H
#define REDIRECTION_H

struct Redirection {
	char * inputFile = nullptr;
	char * outputFile = nullptr;
	char * errorFile = nullptr;
};

const int FILE_NAME_LENGTH = 256;

Redirection * findRedirection(char * line);

int handleRedirection(Redirection * r);

void freeRedirection(Redirection * r);

#endif
