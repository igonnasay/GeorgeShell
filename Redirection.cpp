#include <stdlib.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "Redirection.h"

using namespace std;

//will change input string
char * getFileName(char * head) {
    if(!*head)
        return nullptr;

    char * fileName = new char[strlen(head) + 1];;

    while(*head) {
        if(*head == ' ' || *head == '\t')
            ++head;
        else
            break;
    }

    char * tail = head;

    while(*head){
        char * next = head + 1;
        if(*next == '\0' || *next == ' ' || *next == '\t')
            break;
        ++head;
    }

    int i = 0;
    while(tail <= head){
        fileName[i] = *tail;
        *tail = ' ';
        ++tail;
        ++i;
    }
    fileName[i] = '\0';
    return fileName;
}

Redirection * findRedirection(char * line){
    Redirection * redirection = new Redirection;
    char * head = line;
    char * following = nullptr;

    while(*head){
        switch(*head){
            case '<':
                *head = ' ';
                if(following && *following == '0'){
                    *following = ' ';
                }
                redirection->inputFile = getFileName(head + 1);
                break;
            case '>':
                *head = ' ';
                if(!following || (*following != '1' && *following != '2')){
                    redirection->outputFile = getFileName(head + 1);
                } else if(following && *following == '1') {
                    *following = ' ';
                    redirection->outputFile = getFileName(head + 1);
                } else if(following && *following == '2') {
                    *following = ' ';
                    redirection->errorFile = getFileName(head + 1);
                }
                break;
        }

        following = head;
        ++head;
    }

    return redirection;
}

void isFileExistAndRegular(char * fileName, bool &isExist, bool &isRegular) {
    struct stat sbuf;
    if(stat(fileName, &sbuf) < 0) {
        isExist = false;
        isRegular = false;
        return;
    }

    isExist = true;
    if(S_ISREG(sbuf.st_mode)) {
        isRegular = true;
    } else {
        isRegular = false;
    }
}

int handleRedirection(Redirection * r){
    bool isExist = false, isRegular = false;
    if(r->inputFile) {
        isFileExistAndRegular(r->inputFile, isExist, isRegular);
        if(isExist && isRegular) {
            int inputFD = open(r->inputFile, O_RDONLY, 0644);
            dup2(inputFD, 0);
            close(inputFD);
        } else {
            cerr << r->inputFile << " : redirected to STDIN is not exist or not a file." << endl;
            return -1;
        }
    }


    if(r->outputFile) {
        isFileExistAndRegular(r->outputFile, isExist, isRegular);
        if(isExist && !isRegular){
            cerr << r->outputFile << " : redirected to STDOUT is not a file."  << endl;
            return -1;
        }

        int outputFD = -1;
        if (!isExist){
            outputFD = open(r->outputFile, O_CREAT | O_RDWR, 0644);
        } else {
            outputFD = open(r->outputFile, O_RDWR, 0644);
        }
        dup2(outputFD, 1);
        close(outputFD);
    }

    if(r->errorFile) {
        isFileExistAndRegular(r->errorFile, isExist, isRegular);
        if(isExist && !isRegular){
            cerr << r->outputFile << " : redirected to STDERR is not a file."  << endl;
            return -1;
        }

        int errorFD = -1;
        if (!isExist){
            errorFD = open(r->errorFile, O_CREAT | O_RDWR, 0644);
        } else {
            errorFD = open(r->errorFile, O_RDWR, 0644);
        }
        dup2(errorFD, 2);
        close(errorFD);
    }

    return 0;
}

void freeRedirection(Redirection * r) {
    if(r->inputFile)
        delete r->inputFile;
    if(r->outputFile)
        delete r->outputFile;
    if(r->errorFile)
        delete r->errorFile;
    delete r;
}
