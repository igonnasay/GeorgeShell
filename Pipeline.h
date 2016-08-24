#include <stdlib.h>

#ifndef PIPELINE_H
#define PIPELINE_H

struct Pipeline {
	char * cmd = nullptr;
	Pipeline * next = nullptr;
	Pipeline * prev = nullptr;
	int fds[2] = {-1, -1};
};

Pipeline * convertToPipeline (char * subCmds[]);

void closeOpenPipes(Pipeline * p);

void freePipeline(Pipeline * p);

#endif
