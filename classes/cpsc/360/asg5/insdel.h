#ifndef __INSDEL_H

#define __INSDEL_H

#include <sys/types.h>
#include "buf.h"

struct db_node_t {
	int size;
	int offset;
	struct db_node_t *size_next, *size_prev;
	struct db_node_t *offset_next, *offset_prev;
};

typedef struct db_node_t db_node;
typedef struct db_node_t db_list;

#define BEST_FIT 0
#define WORST_FIT 1

typedef struct {
	int fd;
	db_list *freelist;
	db_list *reclist;
	int placement_type;
	int db_end;
} DB;

db_list *init_list();
db_node *insert_node(db_list *l, int size, int offset);
void delete_node(db_list *l, db_node *n);
db_node *merge_nodes(db_list *l, db_node *n1, db_node *n2);

int insert_rec(DB *db, record_type rec);
db_node *delete_rec(DB *db, int recnum);

DB *db_open(const char *pathname, int flags, mode_t mode);
void db_close(DB *db);

int add_rec_sizes(DB *db);
int add_free_sizes(DB *db);

#endif
