/****************************************************************************
 *
 * intmaster: pvm master program for numerical integration problem
 *            Calculates the area under x^2 from lower to upper 
 *            using upper sum approximation 
 *
 * authors: Roy Pargas        - pargas@cs.clemson.edu
 *          John N. Underwood - junderw@cs.clemson.edu
 *
 * date: 11 November 1993 (last modified for pvm3.2)
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "int.h"

/* global variables */
int    i, tids[MAX_PROC], msgtype, mytid, nproc, partitions;
double actual_result, approx_result=0.0, delta_x, 
       lower, upper, x, elapse, result, avg_time;

/* function prototypes */
double F();


void main(argc, argv)
   int  argc;
   char *argv[];
{

/* usage of executable: 'master n l u p' */
   if(argc < 4) {
      printf("\nUsage: %s <n> <l> <u> <p>\n", argv[0]);
      printf("       n - number of processors\n");
      printf("       l - lower range of function x**2\n");
      printf("       u - upper range of function x**2\n");
      printf("       p - number of partitions\n\n");
      exit(1);
   }

/* process command line arguments */
   nproc = atoi(argv[1]);
   lower = atof(argv[2]);
   upper = atof(argv[3]);
   partitions = atoi(argv[4]);

   if((nproc<0) || (nproc>MAX_PROC)) 
   {
      printf("n must be between 0 and %d\n", MAX_PROC);
      exit(1);
   }

/* enroll in pvm */
   mytid=pvm_mytid(); 
   /* pvm_catchout(stderr); */

/* start up slave tasks */
   pvm_spawn("intslave", (char **)0, PvmTaskDefault, "", nproc, tids);

/* calculate result, using integration formula, and number of partitions */
   actual_result = F(upper) - F(lower);
   delta_x = (upper-lower) / (double)partitions;
   printf("\nSize of each partition: %6.8f\n\n", delta_x);

/* send data */
   pvm_initsend(PvmDataDefault);
   pvm_pkint(&nproc, 1, 1);
   pvm_pkdouble(&lower, 1, 1);
   pvm_pkdouble(&upper, 1, 1);
   pvm_pkdouble(&delta_x, 1, 1);
   pvm_pkint(&partitions, 1, 1);
   pvm_pkint(tids, nproc, 1);
   pvm_mcast(tids, nproc, DATA);

/* receive results */
   for(i=0; i<nproc; i++) 
   {
      pvm_recv(tids[i], RESULTS);
      pvm_upkdouble(&avg_time, 1, 1);
      pvm_upkdouble(&elapse, 1, 1);
      pvm_upkdouble(&result, 1, 1);
      approx_result += result;
      printf("result[%2d]: %6.3f sec, %15.6f (%6.3f avg time per partition)\n", i, elapse, result, avg_time);
   }

/* print approximated result */
   printf("\nApproximation:   %f\n", approx_result);
   printf("Correct result:  %f\n\n", actual_result);

/* exit PVM */
   /* pvm_catchout(0); */
   pvm_exit();

}   /* main */


/* --- F --- integrated function */
double F(x)
   double x;
{
   return((x*x*x)/3.0);
}   /* F */

