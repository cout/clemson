#ifndef T_ALGO_STRUCT
#define T_ALGO_STRUCT

/* This file really shouldn't be in include/linux, but no time to fix this */

#define T_ALGO

#include <linux/time.h>
#include <linux/t_algo_defs.h>

struct t_algo_stats_s
{
  struct timeval start_of_wait, start_of_service, service_comp;
  unsigned int pid;
} t_algo_stats_t;

#endif
