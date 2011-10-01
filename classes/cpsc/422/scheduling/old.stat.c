/* stat.c				mark smotherman, 28 jan. 98
 *
 * compiled by:  cc stat.c -lm -o stat
 *
 * example use:  gen 6000 10.0 25.0 0.0 | fcfs | stat | sort | rel
 *
 *
 * stat.c calculates sample mean and sample standard deviation for batch
 * means of waiting times.  stat.c requires at least 25 batches (=5000)
 * and assumes that the policy program discards the first five batches
 * (=1000) to account for any initial transient behavior.
 *
 * stat.c acts as a filter and passes through its input on stdout.  the
 * mean (and any warning messages) is printed on stderr.
 */

#include <stdio.h>
#include <math.h>

#define MAXOBS 25
#define BATCH 200

main(){
  int
    st,				/* service time		*/
    wt,				/* wait time		*/
    count,			/* input count		*/
    samples,			/* total samples	*/
    i,				/* index		*/
    ascending,			/* # ascending means	*/
    dummy;			/* place holder		*/
  double
    x,				/* temporary variable	*/
    m_wait_time,		/* mean for a BATCH	*/
    observations[MAXOBS],	/* array of samples	*/
    sample_mean,
    sample_var,
    sample_stdev;

/* read samples from stdin */
  samples = 0;
  count = 0;
  m_wait_time = 0.0;
  while(scanf("%d %d",&st,&wt)!=EOF){
    printf("%06d %06d\n",st,wt);
    m_wait_time += (double)wt;
    count++;
    if(count==BATCH){
      m_wait_time = m_wait_time/((double)BATCH);
      if(samples<MAXOBS) observations[samples] = m_wait_time;
      samples++;
      m_wait_time = 0.0;
      count = 0;
    }
  }

  if(samples<25){fprintf(stderr,"*** need more samples!\n"); exit(0);}
  if(samples>MAXOBS){fprintf(stderr,"*** samples past %d discarded\n",MAXOBS);}

  sample_mean = 0.0;
  for(i=0;i<MAXOBS;i++) sample_mean += observations[i];
  sample_mean /= (double)samples;

  ascending = 0;
  sample_var = 0.0;
  for(i=0;i<MAXOBS;i++){
    x = observations[i] - sample_mean;
    sample_var += x*x;
    if((i>3)&&(observations[i]>
      ((observations[i-3]+observations[i-2]+observations[i-1])/3.0)))
      ascending++;
  }
  sample_var /= (double)(samples - 1);
  sample_stdev = sqrt(sample_var);

/* print final statistics -- with a warning when indicated */
  if(ascending>=(4*MAXOBS/5)){
    fprintf(stderr,
      "--warning-- batch means are generally increasing => unstable queue\n");
    for(i=0;i<samples;i++){
      fprintf(stderr,
        "    batch %d wait time %lf\n",i+1,observations[i]);
    }
  }
  fprintf(stderr,
    "mean wait time is %f (w/ std dev %f)\n",sample_mean,sample_stdev);
}
