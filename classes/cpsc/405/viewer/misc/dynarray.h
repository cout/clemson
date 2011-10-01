#ifndef DYNARRAY_H
#define DYNARRAY_H

typedef struct {
	int count;
	int max;
	void *data;
	int data_size;
} Dynarray;

#define DYNARRAY(t) \
	struct {\
		int count;\
		int max;\
		t *data;\
		int *data_size;\
	}

void da_init(Dynarray *a, int size, int initial_size);
void da_add(Dynarray *a, void *data);
void da_free(Dynarray *a);

#define DA_INIT(a, size, i) da_init((Dynarray*)&a, size, i)
#define DA_ADD(a, data) da_add((Dynarray*)&a, data)
#define DA_FREE(a) da_free((Dynarray*)&a)

#endif

