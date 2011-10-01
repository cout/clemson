#include <stdio.h>

#define TIME_SLICE 4

int			/* various counters */
  arrival_time,
  service_time,
  current_time,
  departures,
  job_counter,
  time_to_swap;

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
  time_to_swap = 0;

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
      time_to_swap = TIME_SLICE;
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
      time_to_swap--;
      if(time_to_swap <= 0) {
        time_to_swap = 0;
        current_job = current_job->next;
        if(current_job == NULL) current_job = head;
      }
    }

  /* increment the clock and test for termination */
    current_time++;
    if(all_arrived&&(current_job==NULL)&&(head==NULL)) no_more_jobs = 1;
  }
}
