#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "buf.h"

int main (int argc, char *argv[]) {
	int fd;
	char done[] = "**";
	char fname[2048], lname[2048], addr[2048], phone[2048];
	record_type rec;

	if ((fd = open(DATAFILE, O_CREAT|O_TRUNC|O_RDWR, 0600)) == -1) {
		fprintf (stderr, "Error: unable to open %s for output\n", DATAFILE);
		exit (1);
	}

	for (;;) {
		fgets (fname, sizeof (fname), stdin);
		strip_newline (fname);
		if (!strcmp (fname, done)) break;

		fgets (lname, sizeof (lname), stdin);
		strip_newline (lname);
		if (!strcmp (lname, done)) break;

		fgets (addr, sizeof (addr), stdin);
		strip_newline (addr);
		if (!strcmp (addr, done)) break;

		fgets (phone, sizeof (phone), stdin);
		strip_newline (phone);
		if (!strcmp (phone, done)) break;

		rec = make_rec (fname, lname, addr, phone);
		write_rec (fd, rec);
		free_rec (rec);
	}

	/* Read back the database */
	lseek (fd, 0, SEEK_SET);

	for (;;) {
		rec = read_rec (fd);
		if (rec.buf == NULL) break;

		printf ("First name: %s\n", rec.r.fname);
		printf ("Last name: %s\n", rec.r.lname);
		printf ("Address: %s\n", rec.r.addr);
		printf ("Phone: %s\n\n", rec.r.phone);

		free_rec (rec);
	}

	close (fd);
	return 0;
}
