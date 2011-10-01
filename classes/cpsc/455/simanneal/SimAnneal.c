#line 58 "SimAnneal.nw"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#line 279 "SimAnneal.nw"
  
#line 295 "SimAnneal.nw"
  int ntemps, maxtemps;
  int nsucc, ntrials, maxsucc, maxtrials;
  double tfactor,temperature;

#line 280 "SimAnneal.nw"
  
#line 319 "SimAnneal.nw"
  
#line 305 "SimAnneal.nw"
typedef struct {
  double x,y;
} Point;
#line 320 "SimAnneal.nw"
#define gsize 1024
  Point c[gsize];
  int imax;

  double deltax,deltay;
  double dx,dy;
  double time,dtime;

  int i,j,k;
  double segmentprob;
  double timer(Point[],int);
  double dtimer(Point[],int,double,double);
  double dsegtimer(Point[],int,int,double,double);
  void line(Point[],int);
  void save(Point[],int);

#line 85 "SimAnneal.nw"
  
#line 422 "SimAnneal.nw"
void line(Point c[],int imax) {

  int i;

  double xo,yo;
  double xf,yf;

  double xincrement,xloc;
  double m,b;

  float tmp1,tmp2;

  printf("\n\nWhat is the initial position of the ball (x,y)?");
  scanf("%f %f",&tmp1,&tmp2);
  xo=(double)tmp1;
  yo=(double)tmp2;
  printf("\n(%f,%f)",xo,yo);

  printf("\n\nWhat is the final position of the ball (x,y)?");
  scanf("%f %f",&tmp1,&tmp2);
  xf=(double)tmp1;
  yf=(double)tmp2;
  printf("\n(%f,%f)",xf,yf);

  xincrement=(xf-xo)/(double)imax;

  xloc=xo;

  m=(yo-yf)/(xo-xf);

  b=(xo*yf-xf*yo)/(xo-xf);

  for(i=0;i<=imax;i++) {

    c[i].x=xloc;
    c[i].y=m*xloc+b;
    xloc+=xincrement;

  }

}
#line 86 "SimAnneal.nw"
  
#line 480 "SimAnneal.nw"
void save(Point c[],int imax) {
  FILE *fpt;
  int i;
  fpt=fopen("graph.dat","w+");
  for (i=0;i<=imax;i++) {
    fprintf(fpt,"%1.16f   %1.16f\n",c[i].x,c[i].y);
  }
  fclose(fpt);
}
#line 87 "SimAnneal.nw"
  
#line 196 "SimAnneal.nw"
double timer(Point c[], int imax) {

  double time,d,v,g;

  int i;
  time = 0.0;
  g = 9.8;

  for (i=0;i<=imax-1;i++){

    if (c[i+1].y>=c[0].y) return 1e100;
    d=pow(2.0*(pow(c[i].x-c[i+1].x,2.0)+pow(c[i].y-c[i+1].y,2.0)),0.5);
    v=pow(g*(c[0].y-c[i].y),0.5)+pow(g*(c[0].y-c[i+1].y),0.5);
    time+=d/v;

  }

  return time;

}
#line 88 "SimAnneal.nw"
  
#line 222 "SimAnneal.nw"
double dsegtimer(Point c[],int i,int j,double dx,double dy) {
  double dtime,d,v;
  double g=9.8;
  int k;
  dtime=0.0;
  d=pow(2.0*(pow(c[i-1].x-c[i].x,2.0)+pow(c[i-1].y-c[i].y,2.0)),0.5);
  v=pow(g*(c[0].y-c[i-1].y),0.5)+pow(g*(c[0].y-c[i].y),0.5);
  dtime-=d/v;
  d=pow(2.0*(pow(c[j].x-c[j+1].x,2.0)+pow(c[j].y-c[j+1].y,2.0)),0.5);
  v=pow(g*(c[0].y-c[j].y),0.5)+pow(g*(c[0].y-c[j+1].y),0.5);
  dtime-=d/v;
  if (c[i].y+dy>=c[0].y) return 1e100;
  d=pow(2.0*(pow(c[i-1].x-c[i].x-dx,2.0)+pow(c[i-1].y-c[i].y-dy,2.0)),0.5);
  v=pow(g*(c[0].y-c[i-1].y),0.5)+pow(g*(c[0].y-c[i].y-dy),0.5);
  dtime+=d/v;
  if (c[j].y+dy>=c[0].y) return 1e100;
  d=pow(2.0*(pow(c[j].x+dx-c[j+1].x,2.0)+pow(c[j].y+dy-c[j+1].y,2.0)),0.5);
  v=pow(g*(c[0].y-c[j].y-dy),0.5)+pow(g*(c[0].y-c[j+1].y),0.5);
  dtime+=d/v;
  for (k=i;k<=j-1;k++) {
    d=pow(2.0*(pow(c[k].x-c[k+1].x,2.0)+pow(c[k].y-c[k+1].y,2.0)),0.5);
    v=pow(g*(c[0].y-c[k].y),0.5)+pow(g*(c[0].y-c[k+1].y),0.5);
    dtime-=d/v;
    if (c[k+1].y+dy>=c[0].y) return 1e100;
    v=pow(g*(c[0].y-c[k].y-dy),0.5)+pow(g*(c[0].y-c[k+1].y-dy),0.5);
    dtime+=d/v;
  }
  return dtime;
}

#line 89 "SimAnneal.nw"
  
#line 255 "SimAnneal.nw"
double dtimer(Point c[],int i,double dx,double dy) {
  double dtime,dm,vm,dp,vp;
  double g=9.8;
  if ((c[i].y+dy)>=c[0].y) return 1e100;
  dm=pow(2.0*(pow(c[i-1].x-(c[i].x+dx),2.0)+pow(c[i-1].y-(c[i].y+dy),2.0)),0.5);
  vm=pow(g*(c[0].y-c[i-1].y),0.5)+pow(g*(c[0].y-(c[i].y+dy)),0.5);
  dp=pow(2.0*(pow((c[i].x+dx)-c[i+1].x,2.0)+pow((c[i].y+dy)-c[i+1].y,2.0)),0.5);
  vp=pow(g*(c[0].y-(c[i].y+dy)),0.5)+pow(g*(c[0].y-c[i+1].y),0.5);
  dtime=dm/vm+dp/vp;
  dm=pow(2.0*(pow(c[i-1].x-c[i].x,2.0)+pow(c[i-1].y-c[i].y,2.0)),0.5);
  vm=pow(g*(c[0].y-c[i-1].y),0.5)+pow(g*(c[0].y-c[i].y),0.5);
  dp=pow(2.0*(pow(c[i].x-c[i+1].x,2.0)+pow(c[i].y-c[i+1].y,2.0)),0.5);
  vp=pow(g*(c[0].y-c[i].y),0.5)+pow(g*(c[0].y-c[i+1].y),0.5);
  dtime=dtime-dm/vm-dp/vp;
  return dtime;
}
#line 67 "SimAnneal.nw"
main() {
  
#line 342 "SimAnneal.nw"
  
#line 350 "SimAnneal.nw"
{
  float tmp;

  printf("\n\nAfter how many temperature steps will the program cease?");
  scanf("%d",&maxtemps);
  printf("\n%d",maxtemps);

  printf("\n\nWhat is the maximum number of trials per temperature?");
  scanf("%d",&maxtrials);
  printf("\n%d",maxtrials);

  printf("\n\nWhat is the maximum number of successes per temperature?");
  scanf("%d",&maxsucc);
  printf("\n%d",maxsucc);

  printf("\n\nBy which factor are successive temperatures \n");
  printf("related in the geometric series?");
  scanf("%f",&tmp);
  tfactor=(double)tmp;
  printf("\n%f",tfactor);

  printf("\n\nWhat is the initial temperature?");
  scanf("%f",&tmp);
  temperature=(double)tmp;
  printf("\n%f",temperature);

  printf("\n\nWhat is the probability for the movement \n");
  printf("of an entire segment of points?");
  scanf("%f",&tmp);
  segmentprob=(double)tmp;
  printf("\n%f",segmentprob);

  printf("\n\nHow far can a point wander in the \n");
  printf("x direction in a single Monte Carlo move?");
  scanf("%f",&tmp);
  deltax=(double)tmp;
  printf("\n%f",deltax);

  printf("\n\nHow far can a point wander in the \n");
  printf("y direction in a single Monte Carlo move?");
  scanf("%f",&tmp);
  deltay=(double)tmp;
  printf("\n%f",deltay);
}
#line 343 "SimAnneal.nw"
  
#line 405 "SimAnneal.nw"
  printf("\n\nHow many points compose the path of the ball?");
  scanf("%d",&imax);
  printf("\n%d",imax);
  imax--;

  line(c,imax);
  time=timer(c,imax);

#line 344 "SimAnneal.nw"
  printf("---End Of Input---\n");
  
#line 466 "SimAnneal.nw"
  printf("\n\n\nAnnealing started timer at time = %1.16e\n\n",time);
  printf("  step              temperature                     ");
  printf("time  ntrials   nsteps\n\n");
#line 68 "SimAnneal.nw"
                    ;
  
#line 101 "SimAnneal.nw"
  for(ntemps=1;ntemps<=maxtemps;ntemps++) {
    nsucc=0;
    ntrials=0;
    
#line 115 "SimAnneal.nw"
for (ntrials=1;ntrials<=maxtrials-1;ntrials++) {
  
#line 129 "SimAnneal.nw"
  {
    dx=deltax*(0.5-drand48());
    dy=deltay*(0.5-drand48());
    if (drand48()<segmentprob)
      
#line 175 "SimAnneal.nw"
 {
      do {
        i=1+(int)(drand48()*(imax-1));
        j=1+(int)(drand48()*(imax-1));
      } while (j<=i);
      dtime=dsegtimer(c,i,j,dx,dy);
      if (dtime<0.0 || drand48()<exp(-dtime/temperature)) {
        nsucc+=1;
        for(k=i;k<=j;k++) {
          c[k].x+=dx;
          c[k].y+=dy;
        }
        time+=dtime;
      }
    }

#line 134 "SimAnneal.nw"
    else
      
#line 152 "SimAnneal.nw"
 {
      i=1+(int)(drand48()*(imax-1));
      dtime=dtimer(c,i,dx,dy);
      if (dtime<0.0 || drand48()<exp(-dtime/temperature)) {
        nsucc+=1;
        c[i].x+=dx;
        c[i].y+=dy;
        time+=dtime;
      }
    }

#line 136 "SimAnneal.nw"
  }
#line 117 "SimAnneal.nw"
  if(nsucc>=maxsucc) break;
}

#line 105 "SimAnneal.nw"
    printf("%6d    %1.15e    %1.15e %8d %8d\n"
                    ,ntemps,temperature,time,ntrials,nsucc);
    if(nsucc==0) break;
    temperature*=tfactor;
  }
#line 69 "SimAnneal.nw"
               ;
  
#line 473 "SimAnneal.nw"
  printf("\nAnnealing done!\n");
  printf("Final time at %2.15f\n\n",timer(c,imax));

  save(c,imax);
#line 70 "SimAnneal.nw"
                 ;
} 

