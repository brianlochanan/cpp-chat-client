//
// Created by Brian Lochan on 2019-02-11.
//

#include "CircularLineBuffer.h"

using namespace std;

/**
 * Checks until '\n' is found and then writes to the buffer.
 * @param chars a line
 * @param nchars number of characters of the line
 * @return true if the characters are succesfully written to the buffer. return false if the buffer is full.
 */
bool CircularLineBuffer::_writeChars(const char *chars, size_t nchars) {

    // checks if size of characters is greater than what can be put inside the buffer.
    if(nchars > bufferSize-1){
        cout << "Size of input is larger than buffer size";
        return false;
    }

    // 1.1. checks if the buffer is full.
    if(freeSpace() == 0){
        cout << "Buffer is full";
        return false;
    }

    int j = 0;

    int next = nextFreeIndex();

    if(nextFreeIndex() >= bufferSize){
        next = nextFreeIndex() - bufferSize;
    }

    int untilNext = int(nchars) + next;

    cout << "next: " << next << endl;
    cout << "untilNext: " << untilNext << endl;

    for (int i = next; i < untilNext; i++) {

        // 1.2. extra check in case if the last character of buffer equals '\n' then take the next free space.
        // This can be done, because the array is not full because we already checked for it in 1.1.
        if(buffer[nextFreeIndex()] == '\n'){
            ++i;
        }
        buffer[i] = chars[j++];

        // circle through buffer.
        if(i >= bufferSize){
            i = -1;
            untilNext = untilNext - bufferSize;
        }
    }

    count += nchars;
    return true;
}

/**
 * Reads a line in the buffer
 * @return
 */
std::string CircularLineBuffer::_readLine() {
    string line;

    if(start > bufferSize){
        start -= bufferSize;
    }

    // assign the current start to the start that is now the current start.
    int currentStart = start;

    // sum the count with the start, so that we can loop through the new start.
    start += count;
    count = 0;

    // check the current start with the new start, so we can loop through the new array values.
    for (int i = currentStart; i < start; i++) {

        // 2.2 do this before checking if the buffer is circular, because we must set the last character in the array
        // and not skip it.
        line += buffer[i];

        // 2.1 check if the buffer is circular and set the value of i to 0, so that we can start from the beginning
        // also set the start variable to the new beginning variable by start - bufferSize-1.
        if(i == bufferSize){
            i = -1;
            start = start - bufferSize;
        }
    }
    return std::string(line);
}

/**
 * Checks freespace through (bufferSize-1 - the space that is free), So when you have only the sixth slot left and your
 * bufferSize-1 = 7, then it returns 1, because 7-6=1.
 * @return
 */
int CircularLineBuffer::freeSpace() {
    return (bufferSize-1 - nextFreeIndex());
}

bool CircularLineBuffer::isFull() {
    if(freeSpace() == 0){
        return true;
    }
    return false;
}

bool CircularLineBuffer::isEmpty() {
    return strlen(buffer) == 0;
}

int CircularLineBuffer::nextFreeIndex() {
//    if((start+count) >= bufferSize){
//        return start+count-bufferSize;
//    }
    return (start+count);
}

int CircularLineBuffer::findNewline() {
    for (int i = nextFreeIndex(); i < bufferSize; i++) {
        if(buffer[i] == '\n'){
            return i;
        }
    }
    return 0;
}

bool CircularLineBuffer::hasLine() {

    for (int i = start; i < nextFreeIndex()+1; i++) {

        if(i >= bufferSize){
            i = 0;
        }

        if(buffer[i] == '\n'){
            return true;
        }
    }
    return false;
}

