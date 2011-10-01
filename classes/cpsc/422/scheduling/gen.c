/* gen.c				mark smotherman, 27 jan. 1998
 *
 * compiled by:  gcc gen.c -o gen
 *
 * number of jobs and parameters to the distributions are passed on the
 * command line:
 * gen <#jobs> <avg. service time> <avg. interarrival time> <bulk prob.>
 *
 * example use:  gen 6000 10.0 25.0 0.0
 *
 * gen.c generates service and arrival times according to the geometric
 * distribution, p(1-p)**(i-1) (i=1,2,...) and a bulk arrival parameter
 * for clustering arrivals
 *
 * average times must be 1.0 or greater; bulk probability must be strictly
 * less than 1.0
 *
 * for avg. time t, the parameter p for the geometric distribution is
 * p = 1/t, and the coefficient of variation is sqrt(1-p); thus,
 *
 *    t = 1     p = 1       1-p = 0       Cx = 0       0 => uniform
 *        1.5       0.667         0.333        0.577
 *        2         0.5           0.5          0.707
 *        5         0.2           0.8          0.894
 *       10         0.1           0.9          0.949
 *      100         0.01          0.99         0.995   1 => "random"
 *
 * the bulk arrival parameter is the fraction of arrivals with zero
 * interarrival time (thus they will arrive "in bulk").  this allows
 * you to cluster the arrivals and increase the coefficient of
 * variation for interarrival times.  the mean interarrival time for
 * non-bulk jobs is adjusted by multiplying it by the reciprocal of
 * (1 - bulk arrival parameter), so that the overall mean interarrival
 * time remains the same as specified in the third command parameter.
 * I.e., mean_iat = bulk*0 + (1-bulk)*[ mean_iat/(1-bulk) ]
 */

#include<stdio.h>

#define MINJOBS 1

int geom(p)
double p;
{
  int n;
  double u;
  n = 0;
  u = ((double)random())/((double)0x7fffffff);
  while(u>p){
    n++;
    u = ((double)random())/((double)0x7fffffff);
  }
  return(n+1);
}


main(argc,argv)
  int argc;
  char *argv[];
{
  int
    i,
    time,
    jobs;
  double
    u,
    serv_parm,
    arrv_parm,
    bulk_parm;

  if(argc<5){
    fprintf(stderr,"gen usage:\n");
    fprintf(stderr,"  gen <#jobs> <service time> <arrival time> <bulk prob>\n");
    exit(-1);
  }

  sscanf(argv[1],"%d",&jobs);
  sscanf(argv[2],"%lf",&serv_parm);
  sscanf(argv[3],"%lf",&arrv_parm);
  sscanf(argv[4],"%lf",&bulk_parm);

  if(jobs<MINJOBS){fprintf(stderr,"too few jobs!\n"); exit(-1);}
  if(serv_parm<1.0){fprintf(stderr,"service time too low!\n"); exit(-1);}
  if(arrv_parm<1.0){fprintf(stderr,"arrival time too low!\n"); exit(-1);}
  if(arrv_parm<serv_parm){
    fprintf(stderr,"warning: avg. interarrival time < avg. service time!\n");
    fprintf(stderr,"         queue is unstable and will grow without bound.\n");
  }

  if(bulk_parm<0.0){fprintf(stderr,"bulk arrival prob too low!\n"); exit(-1);}
  if(bulk_parm>=1.0){fprintf(stderr,"bulk arrival prob too high!\n"); exit(-1);}

  serv_parm = 1.0/serv_parm;
  arrv_parm = 1.0/arrv_parm;
  if(bulk_parm>0.0) arrv_parm /= (1.0-bulk_parm);

  time = 0;
  for(i=0;i<jobs;i++){
    printf("%6d %5d\n",time,geom(serv_parm));
    u = ((double)random())/((double)0x7fffffff);
    if(u>=bulk_parm){
      time += geom(arrv_parm);
    }
  }
}
