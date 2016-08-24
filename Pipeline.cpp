#include <unistd.h>
#include <sys/types.h>

#include "Pipeline.h"

Pipeline * convertToPipeline (char * subCmds[]){
    Pipeline * head = new Pipeline;
    //there is at least one cmd
    head->cmd = subCmds[0];
    
    Pipeline * current = head;
    int i = 1;
    while(subCmds[i]){
        Pipeline * next = new Pipeline;
        next->cmd = subCmds[i];
        next->prev = current;
        current->next = next;
        
        ++i;
        current = next;
    }
    
    return head;
}


void closeOpenPipes(Pipeline * p){
    Pipeline * cursor = p;
    if(cursor) {
        close(cursor->fds[0]);
        close(cursor->fds[1]);
        cursor = cursor->next;
    }
}


void freePipeline(Pipeline * p){
    while(p){
        Pipeline * tmp = nullptr;
        if(p->next) {
            tmp = p->next;
        }
        delete p;
        p = tmp;
    }
}
