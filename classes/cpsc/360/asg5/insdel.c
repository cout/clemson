#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "insdel.h"

/* Initialize a database list.
 * A database list consists of two fields -- size and offset -- and is
 * sorted by both.  The first node contains pointers to the other nodes.
 * x_prev points to the last node in list x, and x_next points to the first
 * node in list x.  Determine that there are no elements in the list if
 * x_next is NULL.
 */
db_list *init_list() {
	db_list *l = malloc(sizeof(db_list));
	if(l==NULL) return NULL;
	l->size_prev = l->offset_prev = l;
	l->size_next = l->offset_next = 0;
	l->size = l->offset = 0;
	return l;
}

/* Allocate memory for an insert a node into a database list */
db_node *insert_node(db_list *l, int size, int offset) {
	db_node *n, *tmp;

	/* Allocate memory for the node */
	if((n = (db_node *)malloc(sizeof(db_node))) == NULL) return NULL;

	/* Set the proper values for the node */
	n->size = size;
	n->offset = offset;

	/* Special case for an empty list */
	if(l->size_next == NULL) {
		n->size_prev = l;
		n->size_next = NULL;
		n->offset_prev = l;
		n->offset_next = NULL;
		l->size_prev = n;
		l->size_next = n;
		l->offset_prev = n;
		l->offset_next = n;
		return n;
	}

	/* Find the position in the size list */
	for(tmp = l; tmp != NULL && tmp->size <= size; tmp = tmp->size_next);

	if(tmp == NULL) {
		/* At the end of the list... */
		n->size_prev = l->size_prev;
		n->size_next = NULL;
		l->size_prev->size_next = n;
		l->size_prev = n;
	} else {
		/* In the middle of the list */
		n->size_prev = tmp->size_prev;
		n->size_next = tmp;
		tmp->size_prev->size_next = n;
		tmp->size_prev = n;
	}

	/* Find the position in the offset list */
	for(tmp = l; tmp != NULL && tmp->offset <= offset;
	    tmp = tmp->offset_next);

	if(tmp == NULL) {
		/* At the end of the list... */
		n->offset_prev = l->offset_prev;
		n->offset_next = NULL;
		l->offset_prev->offset_next = n;
		l->offset_prev = n;
	} else {
		/* In the middle of the list */
		n->offset_prev = tmp->offset_prev;
		n->offset_next = tmp;
		tmp->offset_prev->offset_next = n;
		tmp->offset_prev = n;
	}

	return n;
}

/* Delete a node from a database list */
void delete_node(db_list *l, db_node *n) {
	/* The previous node should never be NULL */
	n->size_prev->size_next = n->size_next;
	n->offset_prev->offset_next = n->offset_next;

	if(n->size_next != NULL) {
		/* Delete from the end of the list */
		n->size_next->size_prev = n->size_prev;
	} else {
		/* Delete from the middle of the list */
		l->size_prev = n->size_prev;
	}
	if(n->offset_next != NULL) {
		/* Delete from the end of the list */
		n->offset_next->offset_prev = n->offset_prev;
	} else {
		/* Delete from the middle of the list */
		l->offset_prev = n->offset_prev;
	}
	/* Free the memory that was allocated */
	free(n);
}

/* Remove a node from the list and put it back in its proper position */
db_node *replace_node(db_list *l, db_node *n) {
	if(n->size != 0) {
		int tmpsize = n->size;
		int tmpoffset = n->offset;
		delete_node(l, n);
		n = insert_node(l, tmpsize, tmpoffset);
	} else {
		delete_node(l, n);
		n = NULL;
	}
	return n;
}
	
/* Merge two nodes together.  n1 must not equal n2, and neither must equal
 * the start of the list (l).
 */
db_node *merge_nodes(db_list *l, db_node *n1, db_node *n2) {
	/* Determine which node comes first and act accordingly */
	if(n1->offset < n2->offset) {
		n1->size += n2->size;
		delete_node(l, n2);
		return n1;
	} else {
		n2->size += n1->size;
		delete_node(l, n1);
		return n2;
	}
}

/* Insert a record into the database */
int insert_rec(DB *db, record_type rec) {
	int size = record_size(rec);
	int offset;
	db_node *tmp;

	switch(db->placement_type) {
		case BEST_FIT:
			/* Find the location in the list */
			for(tmp = db->freelist->size_next; tmp != NULL &&
			    tmp->size < size; tmp = tmp->size_next);
			if(tmp == NULL) {
				/* If there are no suitable free blocks */
				lseek(db->fd, 0, SEEK_END);
				offset = db->db_end;
				db->db_end += size;
			} else {
				/* Place the node in the best block */
				offset = tmp->offset;
				lseek(db->fd, offset, SEEK_SET);
				tmp->size -= size;
				tmp->offset += size;

				/* Reorganize the free list */
				replace_node(db->freelist, tmp);
			}
			break;
		case WORST_FIT:
			/* Check the last block, since it is the largest */
			if(db->freelist->size_next == NULL ||
			   db->freelist->size_prev->size <= size) {
				/* If it is too small */
				lseek(db->fd, 0, SEEK_END);
				offset = db->db_end;
				db->db_end += size;
			} else {
				/* If it is large enough */
				tmp = db->freelist->size_prev;
				offset = tmp->offset;
				lseek(db->fd, offset, SEEK_SET);
				tmp->size -= size;
				tmp->offset += size;

				/* Reorganize the free list */
				replace_node(db->freelist, tmp);
			}
			break;
		default:
			return -1;
	}

	/* Insert a node into the record list and write the record to disk */
	if(insert_node(db->reclist, size, offset) == FALSE) return -1;
	return(write_rec(db->fd, rec));
}

/* Delete a record from the database.  Note that the database does not shrink
 * when the last record is deleted.
 */
db_node *delete_rec(DB *db, int recnum) {
	db_node *tmp = db->reclist, *n;
	int j;

	/* Find the node.  If recnum is out of range, return an error. */
	for(j = 0; j <= recnum; j++) {
		if(tmp == NULL) return NULL;
		tmp = tmp->offset_next;
	}

	/* Add the record to the free list */
	if((n = insert_node(db->freelist, tmp->size, tmp->offset)) == NULL)
		return NULL;

	/* And delete it from the record list */
	delete_node(db->reclist, tmp);

	/* Check to see if we need to merge adjacent nodes.  Don't merge with
	 * a NULL node and don't merge with the head node.
	 */
	if(n->offset_prev != db->freelist) {
		if(n->offset_prev->offset + n->offset_prev->size >= n->offset)
			n = merge_nodes(db->freelist, n, n->offset_prev);
	}
	if(n->offset_next != NULL) {
		if(n->offset + n->size >= n->offset_next->offset)
			n = merge_nodes(db->freelist, n, n->offset_next);
	}
	replace_node(db->freelist, n);

	return(n);
}

/* Open a database file and allocate an appropriate data structure */
DB *db_open(const char *pathname, int flags, mode_t mode) {
	DB *db = (DB *)malloc(sizeof(DB));

	if(db == NULL) return NULL;

	if((db->fd = open(pathname, flags, mode)) == -1) {
		free(db);
		return NULL;
	}

	db->freelist = init_list();
	db->reclist = init_list();
	db->placement_type = BEST_FIT;
	db->db_end = 0;

	return db;
}

/* Close a database file and free the DB struct */
void db_close(DB *db) {
	close(db->fd);
	free(db);
}

/* Sum up sizes for all records in the database */
int add_rec_sizes(DB *db) {
	db_node *tmp;
	int total = 0;
	for(tmp = db->reclist; tmp != NULL; tmp=tmp->size_next)
		total += tmp->size;
	return total;
}

/* Sum up all free space in the database */
int add_free_sizes(DB *db) {
	db_node *tmp;
	int total = 0;
	for(tmp = db->freelist; tmp != NULL; tmp=tmp->size_next)
		total += tmp->size;
	return total;
}
