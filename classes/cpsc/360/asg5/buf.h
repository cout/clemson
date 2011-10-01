#ifndef __BUF_H

#define __BUF_H

#define DATAFILE        "data.dat"
#define IDXFILE         "data.idx"
#define OUTFILE		"data.out"
#define BLOCKSIZE       2048
#define DELIM           '\0'

#undef TRUE
#undef FALSE
#define FALSE 0
#define TRUE !FALSE

/* struct for a record
 * Note that all the fields MUST point to a substring of buf!
 * All strings are null-terminated, with buf terminated twice.
 */

typedef struct {
        char *fname;
        char *lname;
        char *addr;
        char *phone;
} rec_t;

typedef union {
        char *buf;
        rec_t r;
} record_type;

void strip_newline(char *s);

int add_field_sizes(const char *fname, const char *lname, const char *addr,
                    const char *phone);
int record_size(const record_type rec);

record_type make_rec(const char *fname, const char *lname, const char *addr,
                     const char *phone);
record_type free_rec(record_type rec);

int end_of_record(const char *buf, int size);
int write_rec(int fd, const record_type rec);
record_type read_rec(int fd);

#endif
