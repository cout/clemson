#ifndef T_ALGO_KERNEL
#define T_ALGO_KERNEL

#include "ide.h"

#ifndef T_ALGO
#define T_ALGO
#endif

#ifdef __KERNEL__
struct request *get_greedy( struct request *first, struct request *second, struct  request *last);
int get_20000_times(struct t_algo_stats_s **disktime);
int start_20000_times(int);
int print_20000_times(void);
struct request* get_algorithmT(struct request* first, struct request* second,
		        struct request* last, ide_drive_t* drive);
int algorithmT(struct request* request_q, ide_drive_t *drive);
#endif

#endif
