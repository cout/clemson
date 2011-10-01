/******************************************************************************
 *
 * int.h: pvm header file for numerical integration problem
 *
 * authors: Roy Pargas        - pargas@cs.clemson.edu
 *          John N. Underwood - junderw@cs.clemson.edu
 *
 ******************************************************************************/

#include "pvm3.h"

/* constants */
#define ARCH              "SUN4"   /* ARCHitecture type           */
#define MAX_PROC             16    /* maximum number of processes */
#define CLOCKS_PER_SEC  1000000L   /* for use with clock()        */

/* message types */
#define DATA                 10
#define RESULTS              20

/* macros */
#define pvm_err(cmd, errno) { if(errno < PvmOk) perror(cmd); }
