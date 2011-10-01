#ifndef T_ALGO_DEFS
#define T_ALGO_DEFS

#define T_NUM_OF_REQS_MSRD      20000
#define T_NUM_OF_REQS_IGND      500
#define T_ALGO_SIZE             128

/* Diagonal lookup macros */
#define DLOOK(i,j) ((j)*((j)+1)/2+(i))
#define DSIZE(i) ((i)*((i)+1)/2+(i)+1)*sizeof(unsigned long)
#define DISVALID(i,j) ((i)<=(j))

#endif
