/* rel.c				mark smotherman, 27 jan. 1998
 *
 * compiled by:  gcc rel.c -o rel
 *
 * example use:  gen 6000 10.0 25.0 0.0 | fcfs | sort | rel
 *
 * rel.c reads 5000 triples of service, arrival, and wait times, assuming 
 * that they are sorted according to ascending service times, and then
 * prints out the mean wait times for each 10-percentile
 */

#include <stdio.h>

int w[5000][2];
double pctm[10],overallm;

main(){
  int i,j,arrv;
  double m,maxm,scale;
  i = 0;
  while(scanf("%d %d %d",&w[i][0],&arrv,&w[i][1])!=EOF){
    i++;
    if(i>5000){ printf("too many triples!\n"); exit(0);}
  }
  if(i!=5000){ printf("too few triples!\n"); exit(0);}
  maxm = 0.0;
  overallm = 0.0;
  for(i=0;i<10;i++){
    m = 0.0;
    for(j=0;j<500;j++) m += w[i*500+j][1];
    m /= 500.0;
    pctm[i] = m;
    overallm += m;
    if(m>maxm) maxm = m;
  }
  scale = maxm/20.0;
  for(i=0;i<10;i++){
    printf("%2d-th 10-percentile avg. wait is %10.3f  |",i+1,pctm[i]);
    for(j=0;j<(int)pctm[i]/scale;j++) printf("*");
    printf("\n");
  }
  printf("                                             +--------------------\n");
  printf("                                              scaled to max value\n");
  overallm /= 10.0;
  printf("                                              overall avg. %10.3f\n",
    overallm);
}
