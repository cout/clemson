#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include "dbfile.h"

DBFile::DBFile(int rec_size) {
    buffer = new FileBuffer(BUFSIZE, rec_size);
	blocksize = recsize = rec_size;
	fd = -1;
}

DBFile::DBFile(int rec_size, int block_size) {
    buffer = new FileBuffer(BUFSIZE, rec_size);
	recsize = rec_size;
    blocksize = block_size;
	fd = -1;
}

DBFile::~DBFile() {
    delete buffer;
}

bool DBFile::readRecord(int n, void *buf) {
    const char *tmpbuf;
    if((tmpbuf = buffer->find(n)) != NULL) {
        memcpy(buf, tmpbuf, recsize);
        return true;
    }
	if(lseek(fd, n*blocksize, SEEK_SET) != n*blocksize) return false;
	if(read(fd, buf, recsize) <= 0) return false;
    buffer->add(n, buf);
	return true;
}

bool DBFile::writeRecord(int n, const void *rec) {
	if(lseek(fd, n*blocksize, SEEK_SET) != n*blocksize) return false;
	if(write(fd, rec, recsize) < 0) return false;
    buffer->update(n, rec);
	return true;
}

bool DBFile::deleteRecord(int n) {
	if(lseek(fd, n*blocksize, SEEK_SET) != n*blocksize) return false;
	if(write(fd, "\0", 1) < 0) return false;

    char *buf = new char[recsize];
    lseek(fd, n*blocksize, SEEK_SET);
    read(fd, buf, recsize);
    buffer->update(n, buf);
    delete[] buf;

	return true;
}

bool DBFile::isDeleted(int n) {
	char c;
	lseek(fd, n*blocksize, SEEK_SET);
	read(fd, &c, 1);
	if(c == '\0') return true;
	return false;
}

bool DBFile::open(const char *filename, bool destruct) {
	if(fd != -1) return false;
    int mode = O_CREAT | O_RDWR;
#ifdef _WIN32
    mode |= O_BINARY;
#endif
    if(destruct) mode |= O_TRUNC;
	if((fd = ::open(filename, mode, 0600)) == -1)
		return false;
	return true;
}

bool DBFile::close() {
	if(fd == -1) return false;
	::close(fd);
	fd = -1;
	return true;
}
