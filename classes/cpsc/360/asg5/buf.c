#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "buf.h"

/* strip_newline() -- strips all cr/lf characters from a string */
void strip_newline(char *s) {
	char *p = s;
	while(*s != 0) {
		if(*s != '\n' && *s != '\r') *p++ = *s;
		s++;
	}
	*p = 0;
}

/* Return the sum of all fields, plus the spaces for the delimeters and
 * terminating character
 */
int add_field_sizes(const char *fname, const char *lname, const char *addr,
                    const char *phone) {
	return strlen(fname) + strlen(lname) + strlen(addr) + strlen(phone)
		+ 5;
}

/* Return the size of a record */
int record_size(const record_type rec) {
	return add_field_sizes(rec.r.fname, rec.r.lname, rec.r.addr,
		rec.r.phone);
}

/* Create a record from the given fields */
record_type make_rec(const char *fname, const char *lname, const char *addr,
                     const char *phone) {
	record_type rec;
	int len = add_field_sizes(fname, lname, addr, phone);
	int tot;

	if((rec.buf = malloc(len)) == NULL) return rec.buf;

	/* Copy fname */
	/* No need to set rec.r.fname, since it is already equal to rec.buf */
	strcpy(rec.r.fname, fname);
	tot = strlen(fname) + 1;

	/* Copy lname */
	rec.r.lname = rec.buf + tot;
	strcpy(rec.r.lname, lname);
	tot += strlen(lname) + 1;

	/* Copy addr */
	rec.r.addr = rec.buf + tot;
	strcpy(rec.r.addr, addr);
	tot += strlen(addr) + 1;

	/* Copy phone */
	rec.r.phone = rec.buf + tot;
	strcpy(rec.r.phone, phone);
	tot += strlen(phone) + 1;

	/* Terminate the record */
	rec.buf[tot] = 0;

	return rec;
}

/* Release a record's memory */
record_type free_rec(record_type rec) {
	free(rec.buf);
	rec.buf = NULL;
	return rec;
}

/* Write a record to disk */
int write_rec(int fd, const record_type rec) {
	int len = record_size(rec);
	return write(fd, rec.buf, len);
}

/* Determine if the buffer includes two delimeters next two each other.
 * This indicates end of record.
 */
int end_of_record(const char *buf, int size) {
	int j;
	for(j = 1; j < size; j++)
		if(buf[j-1] == 0 && buf[j] == 0) return j;
	return 0;
}

/* Read a record from disk and allocate memory for it */
record_type read_rec(int fd) {
	record_type rec;
	int size = 40;
	char *tmp;
	int tot, ret, buf_end;

	if((rec.buf = (char *)malloc(size)) == NULL) return rec;
	if((tot = read(fd, rec.buf, size)) <= 0) {
		rec = free_rec(rec);
		return rec;
	}

	while(!(buf_end = end_of_record(rec.buf, size))) {
		rec.buf = realloc(rec.buf, size<<1);
		if((ret = read(fd, rec.buf + size, size)) <= 0) {
			rec = free_rec(rec);
			return rec;
		}
		tot += ret;
		size = size << 1;
	}

	/* We really should keep data that is already read in a buffer,
	 * but for simplicity, we'll let the os handle it for now.
	 */
	lseek(fd, buf_end - tot + 1, SEEK_CUR);

	/* We already have fname, so move to the next slot */
	for(tmp = rec.buf; *tmp != 0; tmp++);

	tmp++; rec.r.lname = tmp;
	if(*tmp == 0) {
		rec.r.addr = tmp;
		rec.r.phone = tmp;
		return rec;
	}

	for(tmp++; *tmp != 0; tmp++);

	tmp++; rec.r.addr = tmp;
	if(*tmp == 0) {
		rec.r.phone = tmp;
		return rec;
	}

	for(tmp++; *tmp != 0; tmp++);

	tmp++; rec.r.phone = tmp;
	
	return rec;
}
