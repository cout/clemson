/* fcfs.c				mark smotherman, 27 jan. 98
 *
 * ---- to show queues each clock ----
 * compiled by:  gcc -DSHOW fcfs.c -o show.fcfs
 * example use:  gen 10 10.0 25.0 0.0 | show.fcfs
 *
 * ---- to do simulation ----
 * compiled by:  gcc fcfs.c -o fcfs
 * example use:  gen 6000 10.0 25.0 0.0 | fcfs | sort | rel
 *
 *
 * fcfs.c simulates the first-come, first-served scheduling policy.
 * for each job beyond the first 1000, it will print out the service
 * time, arrival time, and wait time.  (the first 1000 are ignored to
 * avoid any startup transient in simulation.)
 *
 * input from the gen.c program consists of two numbers per line:
 * an arrival time and service time (arrival times are expected to
 * be non-decreasing)
 */

#include <stdio.h>

int			/* various counters */
  arrival_time,
  service_time,
  current_time,
  departures,
  job_counter;

struct job{		/* job node, will be enqueued */
  int
    job_id,
    arrival_time,
    service_time,
    remaining_time;
  struct job *next;
};

struct job		/* various pointers */
  *head,			/* ready queue */
  *tail,			/* ready queue */
  *current_job,
  *new_job;


/* enqueue job node on rear of ready queue */
void insert_job_rear(ptr)
struct job *ptr;
{
  if(tail==NULL){
    head = tail = ptr;
  }else{
    tail->next = ptr;
    tail = ptr;
  }
}


/* allocate job node and add to queue */
void add_job(){
  new_job = (struct job *) malloc(sizeof(struct job));
  if(new_job==NULL){ printf("*** unable to alloc new job!\n"); exit(0);}
  new_job->job_id         = job_counter++;
  new_job->arrival_time   = arrival_time;
  new_job->service_time   = service_time;
  new_job->remaining_time = service_time;
  new_job->next           = NULL;
  insert_job_rear(new_job);
}


/* obtain first job on ready queue */
void get_job(){
  if(head==NULL) return;
  current_job = head;
  head = head->next;
  if(head==NULL) tail = NULL;
  current_job->next = NULL;
}


/* job terminates - print service time and wait time
 */
void end_job(){
  if(current_job==NULL){ printf("*** no current job!\n"); exit (0);}
  departures++;
  if(departures>1000)
    printf("%06d %06d %06d\n",
      current_job->service_time,
      current_job->arrival_time,
      current_time - current_job->arrival_time + 1 - current_job->service_time
    );
  free(current_job);
  current_job = NULL;
}


/* print current job id and its remaining time as well as id and time
 *   for each job on the ready queue
 */
void printqs(){
  struct job *ptr;

  printf("time %d",current_time);
  if(current_job!=NULL){
    printf("  current: %d/%d ",
      current_job->job_id,current_job->remaining_time);
  }else{
    printf("               ");
  }
  if(head!=NULL){
    printf("ready queue: %d/%d",head->job_id,head->remaining_time);
    ptr = head->next;
    while(ptr!=NULL){
      printf(", %d/%d",ptr->job_id,ptr->remaining_time);
      ptr = ptr->next;
    }
  }else{
    printf("empty ready queue");
  }
  printf("\n");
}


/* main program does a clock-tick by clock-tick simulation until all
 *   jobs are removed from the system
 */
main(){
  int			/* various logical flags */
    no_more_jobs,
    all_arrived;

/* initialization */
  head = tail = NULL;
  current_job = NULL;
  job_counter = 0;
  departures  = 0;
  no_more_jobs = 0;
  all_arrived = 0;
  current_time = 0;

/* obtain first job parameters */
  if(scanf("%d %d",&arrival_time,&service_time)==EOF) all_arrived = 1;

/* continue until jobs are gone */
  while(!no_more_jobs){

  /* add all jobs that arrive at this time to rear of ready queue */
    while((!all_arrived)&&(arrival_time==current_time)){
      add_job();
      if(scanf("%d %d",&arrival_time,&service_time)==EOF) all_arrived = 1;
    }

  /* if no job being serviced, then get one */
    if((current_job==NULL)&&(head!=NULL)){
      get_job();
    }

#ifdef SHOW
    printqs();
#endif

  /* record one tick of service, jobs ends if no remaing service left */
    if(current_job!=NULL){
      current_job->remaining_time--;
      if(current_job->remaining_time==0){
        end_job();
      }
    }

  /* increment the clock and test for termination */
    current_time++;
    if(all_arrived&&(current_job==NULL)&&(head==NULL)) no_more_jobs = 1;
  }
}
