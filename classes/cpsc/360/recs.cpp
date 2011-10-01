#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <list>

#define DATAFILE	"data.dat"
#define IDXFILE		"data.idx"
#define BLOCKSIZE	2048
#define DELIM		'*'

// struct for a record
struct record_type {
	String fname;
	String lname;
	String addr;
	String phone;
};

// block information
struct Block {
	size_t size;
	off_t offset;
};

struct SizeBlock: Block {};
struct OffsetBlock: Block {};
struct RecBlock: Block {};

operator<(const SizeBlock &b1, const SizeBlock &b2) {
	return b1.size < b2.size;
}
operator<(const OffsetBlock &b1, const OffsetBlock &b2) {
	return b1.offset < b2.offset;
}
operator<(const RecBlock &b1, const RecBlock &b2) {
	return b1.offset < b2.offset;
}

operator==(const SizeBlock &b1, const SizeBlock &b2) {
	return b1.size == b2.size;
}
operator==(const OffsetBlock &b1, const OffsetBlock &b2) {
	return b1.offset == b2.offset;
}
operator==(const RecBlock &b1, const RecBlock &b2) {
	return b1.offset == b2.offset;
}

#define BEST_FIT 0
#define WORST_FIT 1

class Database {
public:
	size_t insertrec(const record_type &rec);
	size_t deleterec(int recnum);
	size_t record_size(const record_type &rec);
	size_t write_rec(const record_type &rec, off_t location);
	Database(int fd_tmp, int pt = BEST_FIT);
private:
	int fd;
	size_t db_end;
	list<sizeblock> sizelist;
	list<offsetblock> offsetlist;
	int placement_type;
}

Database::Database(int fd_tmp, int pt) {
	fd = fd_tmp;
	placement_type = pt;
	db_end = 0;
}

size_t Database::record_size(const record_type &rec) {
	return(rec.fname.length() + rec.lname.length() + rec.addr.length() +
		rec.phone.length() + 5);
}

size_t Database::insertrec(const record_type &rec) {
	size_t size = record_size(rec);
	off_t location;
	iterator it;
	switch(placement_type) {
		case BEST_FIT:
			for(it = sizelist.begin(); (*it).size < size &&
				it != sizelist.end(); it++);
			if(it == sizelist.end()) {
				location = db_end;
			} else {
				// FIXME!! We need to handle changing the size
				location = (*it).size;
			}
			break;
		case WORST_FIT:
			if(sizelist.empty()) {
				location = db_end;
			} else {
				if(sizelist.back().size >= size) {
					// FIXME!! We need to handle the size
					location = sizelist.back().offset;
				} else {
					location = db_end;
				}
			}
			break;
		default:
			return 0;
	}
	return write_rec(rec, location);
}

size_t Database::write_rec(const record_type &rec, off_t location) {
	char delim[] = {DELIM};
	size_t tot;
	tot = write(fd, rec.fname.c_str(), rec.fname.length());
	tot += write(fd, delim, sizeof(delim));
	tot += write(fd, rec.lname.c_str(), rec.lname.length());
	tot += write(fd, delim, sizeof(delim));
	tot += write(fd, rec.addr.c_str(), rec.addr.length());
	tot += write(fd, delim, sizeof(delim));
	tot += write(fd, rec.addr.c_str(), rec.phone.length());
	tot += write(fd, delim, sizeof(delim));
	tot += write(fd, delim, sizeof(delim));
	return tot;
}

size_t Database::deleterec(int recnum) {
	// We need to handle the following here:
	// - delete the record from the list of records
	// - add the record to the free list
	// - merge the new free space with adjacent free space (how?)
}

int main(int argc, char *argv[]) {
	if((int fd = open(DATAFILE, O_CREAT | O_TRUNC)) == -1) {
		fprintf(stderr, "Error: unable to open %s for output\n", DATAFILE);
		exit(1);
	}

	list<sizeblock> sizelist;
	list<offsetblock> offsetlist;

	record_type rec;
	rec.fname = malloc(2048);
	rec.lname = malloc(2048);
	rec.addr = malloc(2048);
	rec.phone = malloc(2048);

	for(;;) {
		fgets(rec.fname, sizeof(rec.fname), stdin);
		if(!strcmp(rec.fname, done)) break;
		rec.fname_len = strlen(rec.fname);

		fgets(rec.lname, sizeof(rec.lname), stdin);
		if(!strcmp(rec.lname, done)) break;
		rec.lname_len = strlen(rec.lname);

		fgets(rec.addr, sizeof(rec.addr), stdin);
		if(!strcmp(rec.addr, done)) break;
		rec.addr_len = strlen(rec.addr);

		fgets(rec.phone, sizeof(rec.phone), stdin);
		if(!strcmp(rec.phone, done)) break;
		rec.phone_len = strlen(rec.phone);

		write_rec(fp, btell(fp), &rec);
	}

	free(rec.fname);
	free(rec.lname);
	free(rec.addr);
	free(rec.phone);
	bclose(fp);

	return 0;
}
