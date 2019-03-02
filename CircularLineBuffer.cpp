//
// Created by Brian Lochan on 2019-02-11.
//

#include "CircularLineBuffer.h"

using namespace std;

bool CircularLineBuffer::_writeChars(const char *chars, size_t nchars) {

    // if freespace is less then the chars you want to write then it returns false.
    if(nchars > freeSpace()){
        return false;
    }

    int i = 0;

    for (int j = nextFreeIndex(); j < (nchars + nextFreeIndex()); j++) {
        char endline = chars[i];
        endline += chars[i+1];


        if(endline == '\n'){
            buffer[j] = '\n';
            count = (--nchars + nextFreeIndex());
            return true;
        }
        else{
            buffer[j] = chars[i];
            i++;
        }
    }
    count = (nchars + nextFreeIndex());
    return true;
}

std::string CircularLineBuffer::_readLine() {
    string line;
    for (int i = start; i < count; ++i) {
        line += buffer[i];
        if(buffer[i] == '\n'){

            findNewline();
            return std::string(line);
        }
    }
    return std::string();
}

int CircularLineBuffer::freeSpace() {
    return (bufferSize - strlen(buffer) + 1);
}

bool CircularLineBuffer::isFull() {
    if(freeSpace() == 0){
        return true;
    }
    return false;
}

bool CircularLineBuffer::isEmpty() {

    if(strlen(buffer) == 0){
        return true;
    }
    return false;
}

int CircularLineBuffer::nextFreeIndex() {
    return strlen(buffer);
}

int CircularLineBuffer::findNewline() {
    for (int i = start; i < strlen(buffer); i++) {
        if(buffer[i] == '\n'){
            start = i+1;
            return start;
        }
    }
    return 0;
}

bool CircularLineBuffer::hasLine() {
    for (int i = start; i < strlen(buffer); i++) {
        if(buffer[i] == '\n'){
            return true;
        }
    }
    return false;
}

