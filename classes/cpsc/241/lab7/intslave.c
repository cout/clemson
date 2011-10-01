/****************************************************************************
 *
 * intslave: pvm slave program for numerical integration problem
 *
 * authors: Roy Pargas        - pargas@cs.clemson.edu
 *          John N. Underwood - junderw@cs.clemson.edu
 *
 * date: 11 November 1993 (last modified for pvm3.2)
 *
 ****************************************************************************/

#include <stdio.h>
#include <time.h>
#include "int.h"

/* global variables */
clock_t start;
double  approx_result=0.0, delta_x, first, last, lower, upper, x,
        elapse, clocks_per_sec=(double)CLOCKS_PER_SEC, avg_time;
int     master, msgtype, mytid, nproc, mynode, tids[MAX_PROC],
        partitions, partitions_per_slave, i;

/* function prototypes */
double f();


void main()
{

/* enroll in pvm */
   mytid = pvm_mytid();
   master = pvm_parent();

/* receive data from master */
   pvm_recv(master, DATA);
   pvm_upkint(&nproc, 1, 1); 
   pvm_upkdouble(&lower, 1, 1); 
   pvm_upkdouble(&upper, 1, 1); 
   pvm_upkdouble(&delta_x, 1, 1); 
   pvm_upkint(&partitions, 1, 1); 
   pvm_upkint(tids, nproc, 1); 

/* determine which slave I am (0..nproc-1) */
   for(i=0; i<nproc; i++) 
      if(mytid==tids[i]) { mynode = i; break; }

/* start timiming */
   start = clock();

/* calculate approximation */
   partitions_per_slave = partitions / nproc;
   first = lower + mynode * ((upper-lower)/nproc);
   last = first + partitions_per_slave * delta_x;

   x = first + (delta_x/2.0);
   while(x<last) 
   {
      approx_result += f(x) * delta_x;
      x += delta_x;
   }

/* end timing */
   elapse = (clock() - start) / clocks_per_sec;
   avg_time = elapse / partitions;

/* send the results back to the master program */
   pvm_initsend(PvmDataDefault);
   pvm_pkdouble(&elapse, 1, 1); 
   pvm_pkdouble(&approx_result, 1, 1); 
   pvm_pkdouble(&avg_time, 1, 1);
   pvm_send(master, RESULTS);

/* exit pvm */
   pvm_exit(); 
}


/* --- f --- the function */
double f(x)
   double x;
{
   return(x*x);
}   /* f */
