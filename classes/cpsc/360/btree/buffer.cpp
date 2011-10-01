#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "buffer.h"

FileBuffer::FileBuffer(int n, int s) {
    numElements = n;
    elementSize = s;
    buffer = new char*[n];
    for(int j = 0; j < numElements; j++) {
        buffer[j] = new char[s];
    }
    refct = new unsigned int[n];
    key = new int[n];
}

FileBuffer::~FileBuffer() {
    for(int j = 0; j < numElements; j++) {
        delete[] buffer[j];
    }
    delete[] refct;
    delete[] key;
}

void FileBuffer::lru() {
    for(int i = 0; i < numElements; i++)
        if(refct[i] < INT_MAX) refct[i]++;
}

const char *FileBuffer::find(int n) {
    for(int j = 0; j < numElements; j++) {
        if(key[j] == n) {
            lru();
            refct[j] = 0;
            return buffer[j];
        }
    }
    return NULL;
}

void FileBuffer::add(int n, const void *buf) {
    int max = 0;
    for(int j = 0; j < numElements; j++) {
        if(refct[j] > refct[max]) max = j;
    }
    refct[max] = 0;
    key[max] = n;
    memcpy(buffer[max], buf, elementSize);
}

bool FileBuffer::update(int n, const void *buf) {
    for(int j = 0; j < numElements; j++) {
        if(key[j] == n) {
            lru();
            refct[j] = 0;
            memcpy(buffer[j], buf, elementSize);
            return true;
        }
    }
    return false;
}
