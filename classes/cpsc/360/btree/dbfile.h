#ifndef __DBFILE_H

#define __DBFILE_H

#define BLOCKSIZE 4096
#define BUFSIZE 256

#include "buffer.h"

class DBFile {
public:
	DBFile(int rec_size);
    DBFile(int rec_size, int block_size);
    ~DBFile();
	bool readRecord(int n, void *buf);
	bool writeRecord(int n, const void *rec);
	bool deleteRecord(int n);
	bool isDeleted(int n);
	bool open(const char *filename, bool destruct = true);
	bool close();
private:
	int fd;
	int recsize, blocksize;
    FileBuffer *buffer;
};

#endif
