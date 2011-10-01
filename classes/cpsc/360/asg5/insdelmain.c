#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "insdel.h"

static const char *fnames[] = {
	"Al", "Albert", "Alfred", "Alice", "Allen",
	"Barbarah", "Betsy", "Bill", "Billy", "Bo", "Bob",
	"Cameron", "Candice", "Chris", "Christopher", "Christie",
	"Debby", "Deborah", "Duke", 
	"Ed", "Eddie", "Edward",
	"Gil",
	"Harry", "Hattie",
	"Iris",
	"Jack", "Jackie", "Jake", "Jane", "Jason", "Jeremy", "Joe", "John",
	"Kris", "Kristen",
	"Lana", "Lewis", "Linus", "Lou", "Louis",
	"Mark", "Matt", "Matthew", "Mary",
	"Paul", "Paula", "Paulline", "Pat", "Patrick", "Patsy", "Peter",
	"Rose",
	"Sally", "Sam", "Samantha", "Sandy", "Sara", "Sarah", "Scott",
	"Thomas", "Tom",
	"Veronica",
	"Wally", "Wilson"
};
static const fname_total = sizeof(fnames) / sizeof(*fnames);

static const char *lnames[] = {
	"Albertson", "Anderson", "Atkins",
	"Baker", "Barnes", "Bell", "Blackwood", "Bradley", "Brown",
	"Calhoun", "Carson", "Clarendon", "Cobb", "Cunningham",
	"Davis", "Dodge",
	"Einstein", "Ellison",
	"Ferguson",
	"Graham"
	"Harvell", "Hendrick", "Hernandez", "Hill", "Holmes", "Hunter",
	"Jackson", "Jefferson", "Johnson", "Jones", "Jordan",
	"Kelley", "Kline", "Knight",
	"Lawson", "Lee", "Livingston", "Long", "Lowe", "Lyon"
	"Macdowell", "Mahoney", "Marshall", "McGregor", "Mullins",
	"Norwood",
	"O'Reilly", 
	"Parker", "Pinckney", "Powers", "Price",
	"Ratliff", "Reynolds", "Richards", "Richardson", "Robinson",
	"Savage", "Scott", "Simmons", "Singleton", "Smalls", "Smith",
	"Taylor", "Thompson", "Turner",
	"Vaughn",
	"Walker", "Wilkinson", "Williams", "Wilson"
};
static int lname_total = sizeof(lnames) / sizeof(*lnames);

static const char *streets[] = {
	"Street", "Avenue", "Lane", "Circle", "Terrace", "Blvd.", "Rd."
};
static int street_total = sizeof(streets) / sizeof(*streets);

int main (int argc, char *argv[]) {
	DB *db;
	FILE *fp;
	const char *fname, *lname;
	char addr[2048], phone[2048];
	record_type rec;
	int recs = 0;
	int i, j, max;
	int recsum, freesum;
	db_node *n;

	if ((db = db_open(DATAFILE, O_CREAT|O_TRUNC|O_RDWR, 0600)) == NULL) {
		fprintf (stderr, "Error: unable to open %s for output\n",
			DATAFILE);
		exit (1);
	}

	if(argc > 1 && argv[1][0] == '-' &&
	   (argv[1][1] == 'w' || argv[1][1] == 'W'))
		db->placement_type = WORST_FIT;

	switch(db->placement_type) {
		case BEST_FIT:
			printf("Using a best fit placement policy.\n");
			break;
		case WORST_FIT:
			printf("Using a worst fit placement policy.\n");
			break;
		default:
			printf("Not using a placement policy.  Something"
			       "is wrong.  No records will be written.\n");
	}

	for (i  = 0; i < 10000; i++) {
		if(rand() < RAND_MAX / 3 && recs > 0) {
			if(delete_rec(db, rand() % recs)) recs--;
		} else {
			fname = fnames[rand() % fname_total];
			lname = lnames[rand() % lname_total];

			sprintf(addr, "%d", rand() % 1000);
			max = 100 + rand() % 800;
			j = strlen(addr);
			addr[j++] = ' ';
			addr[j++] = 'A' + rand() % 26;
			for(; j < max; j++) addr[j] = 'a' + rand() % 26;
			addr[j] = 0;
			strcat(addr, streets[rand() % street_total]);

			sprintf(phone, "%d-%d", 111 + rand() % 889,
				1111 + rand() % 8889);

			rec = make_rec(fname, lname, addr, phone);
			insert_rec(db, rec);
			free_rec(rec);

			recs++;
		}
	}

	recsum = add_rec_sizes(db);
	freesum = add_free_sizes(db);
	printf("Total number of bytes in records: %d\n", recsum);
	printf("Total number of bytes in free space: %d\n", freesum);
	printf("Total file size: %d\n", db->db_end);
	printf("Percent utilization: %d%%\n",
		((recsum * 100) / (recsum + freesum)) % 100);

	if((fp = fopen(OUTFILE, "w")) == NULL) {
		fprintf(stderr, "Unable to open file %s for writing.\n",
			OUTFILE);
		db_close(db);
		exit(1);
	}
	printf("\nWriting record list to %s\n", OUTFILE);

	for(n = db->reclist->offset_next; n != NULL; n = n->offset_next) {
		lseek(db->fd, n->offset, SEEK_SET);
		rec = read_rec(db->fd);

		if(rec.buf == NULL) {
			fprintf(fp, "Warning: bad record at %d\n\n", n->offset);
		} else {
			fprintf(fp, "First name: %s\n", rec.r.fname);
			fprintf(fp, "Last name: %s\n", rec.r.lname);
			fprintf(fp, "Address: %s\n", rec.r.addr);
			fprintf(fp, "Phone: %s\n\n", rec.r.phone);
			free_rec(rec);
		}
	}

	fclose(fp);
	db_close(db);
	return 0;
}
