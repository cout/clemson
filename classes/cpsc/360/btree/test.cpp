#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iostream>

using namespace std;

#include "dbfile.h"
#include "btreenode.h"
#include "btree.h"
#include "btreedb.h"
#include "secidx.h"

void rndstr(char *str, int n) {
	// get a random string
	for(int k = 0; k < n; k++)
		str[k] = rand() % 26 + 'A';
	str[n] = 0;
}

void DBFileTest(int n) {
	DBFile db(10);
	int i, j, tot = 0;
	char str[11];
	char deleted[100];
	char vals[100][11];

	db.open("test.db");

	// insert and delete nodes at random
	for(i = 0; i < n; i++) {
		for(j = 0; j < 100; j++) {
			switch(rand() % 3) {
			case 0:
			case 1:
                rndstr(str, 10);
                
				// insert it into the database
#ifdef DEBUG
				cout << "Inserting " << str << endl;
#endif
				tot++;
				if(!db.writeRecord(j, str))
					cout << "error" << endl;
				strcpy(vals[j], str);
				deleted[j] = 0;
				break;
			case 2:
				// retrieve the record to display it
				db.readRecord(j, str);
	
				// delete it from the database
#ifdef DEBUG
				cout << "Deleting record " << j << endl;
#endif
				tot--;
				if(!db.deleteRecord(j))
					cout << "error" << endl;
				deleted[j] = 1;
				break;
			}
		}
	}

#ifdef DEBUG
	// print the elements out
	for(j = 0; j < 100; j++) {
		if(deleted[j]) continue;
		db.readRecord(j, str);
		cout << "Record " << j << ": " << str << endl;
	}
#endif

	// check for errors
	for(j = 0; j < 100; j++) {
		if(deleted[j] && !db.isDeleted(j))
			cout << "Record " << j << " not properly deleted."
			     << endl;
		if(!deleted[j] && db.isDeleted(j))
			cout << "Record " << j << " not properly written."
			     << endl;
		if(!db.readRecord(j, str))
			cout << "Error reading record number " << j << "."
			     << endl;
		if(!deleted[j] && strcmp(vals[j], str))
			cout << "Record " << j << " does not match internal "
			     << "data." << endl;
	}

	db.close();

	cout << "DB File test done." << endl << endl;
}

void BTreeNodeTest() {
	// 1000 should be large enough; no need to calculate exact size
    char p[1000];
    memset(p, 0, sizeof(p));
	BTreeNode leafNode(10, 10), indexNode(10, 10, p);

	char str[11];
	char vals[100][11];
	int deleted[100];
	int tot = 0;
	int tmp;

	// everything starts out deleted
	memset(deleted, 1, sizeof(deleted));

	// Make the nodes leaf/index
	leafNode.makeLeaf(10);
	if(leafNode.getNextLeaf() != 10) cout << "error!" << endl;
	indexNode.makeIndex();

	// insert/delete at random
	int j, k;
	for(j = 0; j <= 100; j++) {
		if(tot >= 10 && !leafNode.isFull())
			cout << "Leaf node should be full!" << endl;
		if(tot >= 10 && !indexNode.isFull())
			cout << "Index node should be full!" << endl;
		if(tot < 10 && leafNode.isFull())
			cout << "Leaf node should not be full!" << endl;
		if(tot < 10 && indexNode.isFull())
			cout << "Index node should not be full!" << endl;

		switch(rand() % 3) {
		case 0:
		case 1:
			if(tot < 10) {
                rndstr(str, 10);
        		for(k = 0; k < 100; k++)
	        		if(deleted[k]) break;
		        strcpy(vals[k], str);
		        deleted[k] = 0;

				if(!leafNode.insertElement(str, k))
					cout << "Error inserting to leaf node"
					     << endl;
				if(!indexNode.insertElement(str, k))
					cout << "Error inserting to index node"
					     << endl;
				tot++;
			}
			break;
		case 2:
			tmp = rand() % 10;
			if(tot > 0 && !deleted[tmp]) {
				if(!leafNode.deleteElement(vals[tmp]))
					cout << "Error deleting from leaf node"
					     << endl;
				if(!indexNode.deleteElement(vals[tmp]))
					cout << "Error deleting from index node"
					     << endl;
				deleted[tmp] = 1;
				tot--;
			} else {
				if(leafNode.deleteElement(vals[tmp]))
					cout << "Should not successfully delete"
					     << "from leaf node!" << endl;
				if(indexNode.deleteElement(vals[tmp]))
					cout << "Should not successfully delete"
					     << "from index node!" << endl;
			}
			break;
		}

	}

	// Test the array
	for(j = 0; j < 100; j++) {
		if(!deleted[j]) {
			if(!leafNode.searchForElement(vals[j], &tmp))
				cout << "Error retrieving element" << endl;
			if(tmp != j)
				cout << "Offset mismatch in leaf node" << endl;
			if(!indexNode.searchForElement(vals[j], &tmp))
				cout << "Error retrieving element" << endl;
			if(tmp != j)
				cout << "Offset mismatch in index node"
				     << endl;
		} else {
			if(leafNode.searchForElement(vals[j], NULL))
				cout << "Found deleted element in leaf node!"
				     << endl;
			if(indexNode.searchForElement(vals[j], NULL))
				cout << "Found deleted element in index node!"
				     << endl;
		}
	}

	cout << "BTree Node test done." << endl << endl;

}

void BTreeTest() {
	BTree btree(10, 5);
    const int max = 100;
	int tot = 0;
	int offsets[max];
	int offset;
	char keys[max][100];
	char key[100];
	int k, i, tmp;
    int deleted[max];

	btree.open("test.idx");

    // Test inserts
	for(i = 0; i < max; i++) {
        rndstr(key, 10);
        offset = rand() % 1000;

		btree.insertRecord(key, offset);
		strcpy(keys[i], key);
        offsets[i] = offset;
	}
	for(i = 0; i < max; i++) {
		if(!btree.findRecord(keys[i], &offset))
			cout << "Could not find key " << keys[i] << endl;
		if(offset != offsets[i])
			cout << "Offsets did not match." << endl;
	}

    // Everything starts out deleted
    memset(deleted, 1, sizeof(deleted));

    btree.close();
  	btree.open("test.idx");

    // Test both inserts and deletes
    for(i = 0; i < max; i++) {
        switch(rand() % 3) {
        case 0:
        case 1:
			if(tot < max) {
        		for(k = 0; k < max; k++)
	        		if(deleted[k]) break;
                rndstr(keys[k], 10);

                offsets[k] = rand();
                if(btree.insertRecord(keys[k], offsets[k])) {
                    tot++;
    		        deleted[k] = 0;
                }
            }
            break;
        case 2:
			tmp = rand() % max;
			if(tot > 0 && !deleted[tmp]) {
				if(!btree.deleteRecord(keys[tmp]))
					cout << "Error deleting" << keys[tmp]
                         << endl;
				deleted[tmp] = 1;
				tot--;
			} else {
				if(btree.deleteRecord(keys[tmp]))
					cout << "Should not successfully delete"
                         << endl;
			}
            break;
        }
    }

	for(i = 0; i < max; i++) {
        if(!deleted[i]) {
		    if(!btree.findRecord(keys[i], &offset))
			    cout << "Could not find key " << keys[i] << endl;
		    if(offset != offsets[i])
			    cout << "Offsets did not match." << endl;
        } else {
             if(btree.findRecord(keys[i], &offset))
                 cout << "Should not find key in btree." << endl;
        }
	}

    cout << "BTree test done." << endl << endl;
}

void BTreeDBTest() {
    BTreeDB db(10, 100);

    char *keys[10000];
    char *vals[10000];
    int deleted[10000];
    char val[101];
    int i, k, tot = 0, tmp;

    for(i = 0; i < 10000; i++) {
        keys[i] = new char[11];
        vals[i] = new char[101];
    }

    db.open("dbtest.db", "dbtest.idx");

    // Everything starts out deleted
    memset(deleted, 1, sizeof(deleted));

    // Test inserts and deletes
    for(i = 0; i < 1000; i++) {
        switch(rand() % 3) {
        case 0:
        case 1:
			if(tot < 10000) {
        		for(k = 0; k < 10000; k++)
	        		if(deleted[k]) break;
                rndstr(keys[k], 10);
                rndstr(vals[k], 100);
		        deleted[k] = 0;

                db.insertRecord(keys[k], vals[k]);
                tot++;
            }
            break;
        case 2:
			tmp = rand() % 10000;
			if(tot > 0 && !deleted[tmp]) {
				if(!db.deleteRecord(keys[tmp]))
					cout << "Error deleting" << keys[tmp]
                         << endl;
				deleted[tmp] = 1;
				tot--;
			} else {
				if(db.deleteRecord(keys[tmp]))
					cout << "Should not successfully delete"
                         << endl;
			}
            break;
        }
    }

	for(i = 0; i < 10000; i++) {
        if(!deleted[i]) {
		    if(!db.findRecord(keys[i], val))
			    cout << "Could not find key " << keys[i] << endl;
		    if(strncmp(val, vals[i], 100))
			    cout << "Records did not match." << endl;
        } else {
             if(db.findRecord(keys[i], NULL))
                 cout << "Should not find key in btree." << endl;
        }
	}

    for(i = 0; i < 10000; i++) {
        delete[] keys[i];
        delete[] vals[i];
    }

    cout << "BTree DB test done." << endl << endl;

}

void SecondaryIndexTest() {
    SecondaryIndex secidx(10, 10);

    char skeys[100][11];
    char pkeys[100][11];
    int deleted[100];
    int i, k, tot = 0, tmp;

    secidx.open("skey.db", "skey.idx");

    // Everything starts out deleted
    memset(deleted, 1, sizeof(deleted));

    // initialize keys
    for(i = 0; i < 100; i++) {
        memset(skeys[i], 0, 11);
        memset(pkeys[i], 0, 11);
    }

    // test inserts
    for(i = 0; i < 100; i++) {
        rndstr(pkeys[i], 10);
        memset(skeys[i], rand() % 26 + 'A', 10);
        skeys[i][10] = 0;

        secidx.insertRecord(skeys[i], pkeys[i]);
        tot++;
    }

   	for(i = 0; i < 100; i++) {
	    if(!secidx.findRecord(skeys[i], pkeys[i]))
		    cout << "Inserted key " << skeys[i] << " "
                 << pkeys[i] << " missing." << endl;
	}

    secidx.close();
    secidx.open("skey.db", "skey.idx");

    for(i = 0; i < 100; i++) {
        switch(rand() % 3) {
        case 0:
        case 1:
			if(tot < 100) {
        		for(k = 0; k < 100; k++)
	        		if(deleted[k]) break;
                rndstr(pkeys[k], 10);
                memset(skeys[k], rand() % 26 + 'A', 10);
                skeys[k][10] = 0;

		        deleted[k] = 0;

                secidx.insertRecord(skeys[k], pkeys[k]);
                tot++;
            }
            break;
        case 2:
			tmp = rand() % 100;
			if(tot > 0 && !deleted[tmp]) {
				if(!secidx.deleteRecord(skeys[tmp], pkeys[tmp]))
					cout << "Error deleting " << skeys[tmp] << " "
                         << pkeys[tmp] << endl;
				deleted[tmp] = 1;
				tot--;
			} else {
				if(secidx.deleteRecord(skeys[tmp], pkeys[tmp]))
					cout << "Should not successfully delete"
                         << endl;
			}
            break;
        }
    }

	for(i = 0; i < 100; i++) {
        if(!deleted[i]) {
		    if(!secidx.findRecord(skeys[i], pkeys[i]))
			    cout << "Could not find key " << skeys[i] << " "
                     << pkeys[i] << endl;
        } else {
             if(secidx.findRecord(skeys[i], pkeys[i]))
                 cout << "Should not find key in index." << endl;
        }
	}
}

main() {
    srand(100);
    cout << "Note: if no error messages are printed, then everything "
            "is working correctly." << endl << endl;
	DBFileTest(1);	    	// check for writing records to a new file
	DBFileTest(10);	    	// check for writing over old records
	BTreeNodeTest();    	// Test the BTree node
	BTreeTest();	    	// Test the BTree
    BTreeDBTest();          // Test the entire database
    SecondaryIndexTest();   // Test secondary keys

    return 0;
}
