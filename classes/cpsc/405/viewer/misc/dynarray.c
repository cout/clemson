#include <glib.h>
#include "dynarray.h"

void da_init(Dynarray *a, int size, int initial_size) {
        a->count = 0;
        a->max = initial_size;
        a->data_size = size;
        a->data = g_malloc(size * initial_size);
}

void da_add(Dynarray *a, void *data) {
        if(a->count == a->max) {
		a->max *= 2;
                a->data = g_realloc(a->data, a->max*a->data_size);
	}
        memcpy(((char *)a->data) + (a->data_size * a->count), data,
                a->data_size);
	a->count++;
}

void da_free(Dynarray *a) {
	g_free(a->data);
}

