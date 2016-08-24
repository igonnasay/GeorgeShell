#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include "NonBuiltIn.h"

void runNonBuiltIn(char *cmdAndArgs[]){
    int code = execvp(cmdAndArgs[0], cmdAndArgs);
}
