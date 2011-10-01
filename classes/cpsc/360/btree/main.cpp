#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "btreedb.h"
#include "secidx.h"
#include "record.h"

// strip all cr/lf characters from a string
void strip_newline(char *s) {
    char *p = s;
    while(*s != 0) {
        if(*s != '\n' && *s != '\r') *p++ = *s;
        s++;
    }
    *p = 0;
}

void get_string(char *s, int len) {
    fgets(s, len, stdin);
    strip_newline(s);
}

int get_num() {
    char buf[256];
    do {
        get_string(buf, sizeof(buf));
    } while(buf[0] == 0);
    return strtol(buf, NULL, 16);
}

void title() {
    puts("BTree-based business card system");
    puts("Paul Brannan, April 1999");
    puts("CPSC 360, Clemson University\n");
}

int menu() {
    puts("\n1) Select a database (Default=test.db)");
    puts("2) Select a primary key index file (Default=test.idx)");
    puts("3) Select a secondary key index file (Default=test.ids)");
    puts("4) Select a secondary key bucket file (Default=test.idd)");
    puts("5) [Re]load database");
    puts("6) Wipe database");
    puts("7) Rebuild indexes");
    puts("8) Add/delete a record");
    puts("9) Search for a record");
    puts("A) Display all records");
    puts("0) Exit");
    return get_num();
}

int dbmenu() {
    puts("1) Add a record");
    puts("2) Delete a record");
    puts("0) Return to main menu");
    return get_num();
}

int searchmenu() {
    puts("1) Search for a record by primary key");
    puts("2) Search for a record by last name");
    puts("0) Return to main menu");
    return get_num();
}

void getfilename(char *buf, int size) {
    printf("Enter filename (currently using %s):", buf);
    get_string(buf, sizeof(buf));
}

void printrec(record *r) {
    printf("%s %s\n", r->fname, r->lname);
    puts(r->addr1);
    puts(r->addr2);
    printf("%s, %s %s\n", r->city, r->state, r->zip);
    puts(r->phone);
}

void getrec(record *r) {
    puts("Primary key:");
    get_string(r->pkey, sizeof(r->pkey));
    puts("First name:");
    get_string(r->fname, sizeof(r->fname));
    puts("Last name:");
    get_string(r->lname, sizeof(r->lname));
    puts("Address line 1:");
    get_string(r->addr1, sizeof(r->addr2));
    puts("Address line 2:");
    get_string(r->addr2, sizeof(r->addr2));
    puts("City:");
    get_string(r->city, sizeof(r->city));
    puts("State:");
    get_string(r->state, sizeof(r->state));
    puts("Zip:");
    get_string(r->zip, sizeof(r->zip));
    puts("Phone:");
    get_string(r->phone, sizeof(r->phone));
}

void dbfuncs(BTreeDB &db, SecondaryIndex &secidx) {
    int c;
    record r;
    char pkey[PKEYSIZE];
    while((c = dbmenu())) {
        switch(c) {
        case 1:
            // add record
            getrec(&r);
            if(!db.insertRecord(r.pkey, &r)) {
                puts("Error inserting record");
            } else {
                if(!secidx.insertRecord(r.lname, r.pkey))
                    puts("Error inserting secondary key");
            }
            break;
        case 2:
            // delete record
            puts("Primary key of record to delete:");
            get_string(pkey, sizeof(pkey));
            if(!db.findRecord(pkey, &r)) {
                puts("Error retrieving record");
            } else {
                if(!db.deleteRecord(pkey))
                    puts("Error deleting record");
                if(!secidx.deleteRecord(r.lname, pkey))
                    puts("Error deleting secondary key");
            }
            break;
        }
    }
}

void dbsearch(BTreeDB &db, SecondaryIndex &secidx) {
    int c;
    record r;
    char lname[LNAMESIZE];
    char pkey[PKEYSIZE];
    while((c = searchmenu())) {
        switch(c) {
        case 1:
            // search by primary key
            puts("Primary key:");
            get_string(pkey, sizeof(pkey));
            if(db.findRecord(pkey, &r)) printrec(&r);
            else puts("No such record");
            break;
        case 2:
            // search by last name
            puts("Last name:");
            get_string(lname, sizeof(lname));
            c = secidx.findFirstRecord(lname, pkey);
            if(!c) puts("No such record exists");
            while(c) {
                if(db.findRecord(pkey, &r)) printrec(&r);
                c = secidx.findNextRecord(pkey);
            }
            break;
        }
    }
}

void reconstruct(BTreeDB &db, SecondaryIndex &secidx) {
    record rec;
    int n = db.firstRecord(&rec);
    while(n) {
        db.insertKey(rec.pkey, n);
        secidx.insertRecord(rec.lname, rec.pkey);
        n = db.nextRecord(&rec);
    }
}

void displayall(BTreeDB &db) {
    record rec;
    int n = db.firstRecord(&rec);
    while(n) {
        printrec(&rec);
        n = db.nextRecord(&rec);
    }
}

main() {
    BTreeDB db(PKEYSIZE, sizeof(record));
    SecondaryIndex secidx(PKEYSIZE, LNAMESIZE);

    char database[256] = "test.db";
    char primary[256] = "test.idx";
    char secindex[256] = "test.ids";
    char secbucket[256] = "test.idd";

    title();
    int c;
    while((c = menu())) {
        switch(c) {
        case 1:
            // select a database
            getfilename(database, sizeof(database));
            break;
        case 2:
            // select a primary key index file
            getfilename(primary, sizeof(database));
            break;
        case 3:
            // select a secondary key index file
            getfilename(secindex, sizeof(database));
            break;
        case 4:
            // select a secondary key bucket file
            getfilename(secbucket, sizeof(database));
            break;
        case 5:
            // reload database
            db.close();
            secidx.close();
            db.open(database, primary, false);
            secidx.open(secindex, secbucket, false);
            break;
        case 6:
            // wipe database
            puts("Are you sure you want to wipe the database?");
            c = getchar();
            if(c != 'y' && c != 'Y') break;

            db.close();
            secidx.close();

            unlink(database);
            unlink(primary);
            unlink(secindex);
            unlink(secbucket);

            db.open(database, primary);
            secidx.open(secindex, secbucket);
            break;
        case 7:
            // rebuild indexes
            db.close();
            secidx.close();

            unlink(primary);
            unlink(secindex);
            unlink(secbucket);

            db.open(database, primary, false);
            secidx.open(secindex, secbucket);

            reconstruct(db, secidx);
            break;
        case 8:
            // add/delete a record
            dbfuncs(db, secidx);
            break;
        case 9:
            // search for a record
            dbsearch(db, secidx);
            break;
        case 10:
            // display all records
            displayall(db);
            break;
        }
    }
    return 0;
}
