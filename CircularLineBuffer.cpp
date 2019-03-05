//
// Created by Brian Lochan on 2019-02-11.
//

#include "CircularLineBuffer.h"

using namespace std;

int newIndex;

/**
 * Checks until '\n' is found and then writes to the buffer.
 * @param chars a line
 * @param nchars number of characters of the line
 * @return true if the characters are succesfully written to the buffer. return false if the buffer is full.
 */
bool CircularLineBuffer::_writeChars(const char *chars, size_t nchars) {

//    // if freespace is less then the chars you want to write then it returns false.
//    if(nchars > freeSpace()){
//        return false;
//    }

    // checks if size of characters is greater than what can be put inside the buffer.
    if(nchars > bufferSize){
        return false;
    }

    // checks if the buffer is full.
    if(freeSpace() == 0){
        return false;
    }

    // declare an int to loop through the given line called, chars.
    int i = 0;

    //
    if(freeSpace() <= nchars){

        start = bufferSize - freeSpace();
        count = 0;
    }

    // set the start variable at the new index that has not been taken.
    // Loop nchar times so that it can write the char.
    for (int j = nextFreeIndex(); j < (nchars + nextFreeIndex()); j++) {

        // check for '\n'.
        char endline = chars[i];
        endline += chars[i+1];

        // circle through the buffer.
        if(j == bufferSize){
            j = 0;
        }

        // return true when '\n' is found.
        if(endline == '\n'){

            // 1.1. sets the end character of the line to '\n', because you want '\n' only to take up one character.
            buffer[j] = '\n';

            // do another count + 1, because you don't go to the else again.
            count++;

            // 1.2. the new slot that has not been taken is j+1, because the last j is the '\n'. See comment 1.1.
            newIndex = j+1;

            // check if the line that was the first in the line was overwritten. If yes then overwrite the start variable
            // with the new start variable.
            if(start <= j){
                start = newIndex - nchars;
            }
            return true;
        }

        // write chars to the buffer.
        else{
            buffer[j] = chars[i];
            i++;

            // count how many chars have been written to the buffer.
            count++;
        }
    }
    return true;
}

/**
 * Reads a line in the buffer
 * @return
 */
std::string CircularLineBuffer::_readLine() {
    string line;

    // loops through all the lines in the buffer, because of count.
    for (int i = 0; i < count; i++) {

        line += buffer[start];

        // checks for the first line with '\n' and return the string. Also count updates, because you don't have to loop
        // through all of the lines again.
        if(buffer[start++] == '\n'){
            count = count - start;

            return std::string(line);
        }

        // find the next character in the buffer when it circles.
        if(start == bufferSize){
            start = 0;
        }
    }
    return std::string();
}

/**
 * Checks freespace through (bufferSize - the space that is free), So when you have only the sixth slot left and your
 * bufferSize = 7, then it returns 1, because 7-6=1.
 * @return
 */
int CircularLineBuffer::freeSpace() {
    return (bufferSize - nextFreeIndex());
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
    return newIndex;
}

int CircularLineBuffer::findNewline() {
    for (int i = newIndex-1; i-1 < bufferSize; i++) {
        if(buffer[i] == '\n'){
            return i;
        }
    }
    return 0;
}

bool CircularLineBuffer::hasLine() {
    for (int i = 0; i < strlen(buffer); i++) {
        if(buffer[i] == '\n'){
            return true;
        }
    }
    return false;
}

