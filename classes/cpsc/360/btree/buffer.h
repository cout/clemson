#ifndef __BUFFER_H

#define __BUFFER_H

class FileBuffer {
public:
    FileBuffer(int n, int s);
    ~FileBuffer();
    const char *find(int n);
    void add(int n, const void *buf);
    bool update(int n, const void *buf);

private:
    void lru();

    int numElements, elementSize;
    char **buffer;
    unsigned int *refct;
    int *key;
};

#endif
