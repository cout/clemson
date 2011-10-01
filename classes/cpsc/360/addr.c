#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

/* Note: files are closed after each i/o operation.  While this is not
 * efficient, efficiency is not the purpose here.
 */

#define FILENAME "data.dat"

/* struct for a record */
typedef struct {
	char fname[20];
	char lname[30];
	char addr[100];
	char phone[13];
} record_type;

enum {SEARCH_FNAME, SEARCH_LNAME, SEARCH_ADDR, SEARCH_PHONE};

/* strip_newline() -- strips all cr/lf characters from a string */
void strip_newline(char *s) {
	char *p = s;
	while(*s != 0) {
		if(*s != '\n' && *s != '\r') *p++ = *s;
		s++;
	}
	*p = 0;
}

/* do_read() -- prints a message then inputs a string and strips newlines */
/* return value is 0 if the input string is null, nonzero otherwise */
int do_read(char *output, char *buf, int size) {
	printf(output);
	fflush(stdout);
	fgets(buf, size, stdin);
	strip_newline(buf);
	if(!strcmp(buf, "")) return 0;
	return 1;
}

/* add_records() adds new records */
void add_records() {
	int fd;
	record_type rec;

	/* open FILENAME for writing, create mode 0600 (user has rw perms)*/
	if((fd = open(FILENAME, O_CREAT | O_WRONLY, 0600)) == -1) {
		fprintf(stderr, "Error opening file %s for writing.\n", FILENAME);
		exit(1);
	}


	/* Read data from the keyboard until done, write output to a file
	 * along the way
	 */
	while(do_read("First name: ", rec.fname, sizeof(rec.fname))) {
		do_read("Last name: ", rec.lname, sizeof(rec.lname));
		do_read("Address: ", rec.addr, sizeof(rec.addr));
		do_read("Phone: ", rec.phone, sizeof(rec.phone));

		write(fd, &rec, sizeof(rec));
	}

	close(fd);
}

/* print_record prints a single record rec */
void print_record(record_type rec) {
	printf("First name: %s\n", rec.fname);
	printf("Last name: %s\n", rec.lname);
	printf("Address: %s\n", rec.addr);
	printf("Phone: %s\n", rec.phone);
}

/* print_records prints all records in the file */
void print_records() {
	int fd;
	record_type rec;

	/* Reopen the file for reading */
	if((fd = open(FILENAME, O_RDONLY)) == -1) {
		fprintf(stderr, "Error opening file %s for reading.\n", FILENAME);
		exit(1);
	}

	/* Print out all data from the file */
	while(read(fd, &rec, sizeof(rec)) > 0)
		print_record(rec);

	close(fd);
}

/* search searches for a record using a given field */
void search(char *s, int stype) {
	int fd, flag;
	record_type rec;

	/* Reopen the file for reading */
	if((fd = open(FILENAME, O_RDONLY)) == -1) {
		fprintf(stderr, "Error opening file %s for reading.\n", FILENAME);
		exit(1);
	}

	/* Print all matching records from the file */
	while(read(fd, &rec, sizeof(rec)) > 0) {
		flag = 0;
		switch(stype) {
			case SEARCH_FNAME:
				flag = !strncmp(rec.fname, s,
					sizeof(rec.fname));
				break;
			case SEARCH_LNAME:
				flag = !strncmp(rec.lname, s,
					sizeof(rec.lname));
				break;
			case SEARCH_ADDR:	
				flag = !strncmp(rec.addr, s,
					sizeof(rec.addr));
				break;
			case SEARCH_PHONE:
				flag = !strncmp(rec.phone, s,
					sizeof(rec.phone));
				break;
		}
		if(flag) print_record(rec);
	}

	close(fd);
}

/* print record number prints the nth record */
void print_record_number(int n) {
	int fd;
	record_type rec;

	/* Reopen the file for reading */
	if((fd = open(FILENAME, O_RDONLY)) == -1) {
		fprintf(stderr, "Error opening file %s for reading.\n", FILENAME);
		exit(1);
	}

	/* Print out the nth record from the file */
	lseek(fd, n*sizeof(rec), SEEK_SET);
	read(fd, &rec, sizeof(rec));
	print_record(rec);

	close(fd);
}

main() {
	char buf[2048];

	int i = -1;
	while(i != 0) {
		printf("0. Exit program\n"
		       "1. Add records\n"
                       "2. Print records\n"
                       "3. Search by first name\n"
		       "4. Search by last name\n"
                       "5. Search by record number\n-> ");
		fflush(stdout);
		fgets(buf, sizeof(buf), stdin);
		if(!isdigit(buf[0])) continue;
		i = atoi(buf);
		switch(i) {
			case 1: add_records(); break;
			case 2: print_records(); break;
			case 3: 
				printf("First name to find: ");
				fflush(stdout);
				fgets(buf, sizeof(buf), stdin);
				search(buf, SEARCH_FNAME);
				break;
			case 4:
				printf("Last name to find: ");
				fflush(stdout);
				fgets(buf, sizeof(buf), stdin);
				search(buf, SEARCH_LNAME);
				break;
			case 5:
				printf("Record number to display: ");
				fflush(stdout);
				fgets(buf, sizeof(buf), stdin);
				print_record_number(atoi(buf));
				break;
		}
	}
	printf("May the force be with you.\n");
	return 0;
}
