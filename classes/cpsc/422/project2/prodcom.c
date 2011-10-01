
/* example multithreaded program using Solaris threads (see "man threads")
 *
 * this version demonstrates producer/consumer synchronization with a
 * bounded circular buffer
 *
 * compile this using 'gcc <file name> -lthread'
 */


#include <thread.h>
#include <synch.h>
#include <errno.h>


void *producer(void *), *consumer(void *);

#define PRODUCER(i) {int k; k=i; thr_create(NULL,0,producer,(void *)&k,(long)0,&t[k]);}
#define CONSUMER(i) {int k; k=i; thr_create(NULL,0,consumer,(void *)&k,(long)0,&t[k]);}


sema_t s;  /* semaphore */
sema_t n;  /* semaphore */
sema_t e;  /* semaphore */

int in,out;
int buffer[4];


void insert(int val){
  buffer[in] = val;
  in = (in+1)&0x3;    /* modulo 4 */
}


int remove(void){
  int val;
  val = buffer[out];
  out = (out+1)&0x3;  /* modulo 4 */
  return(val);
}


int main(void){
     thread_t t[10];

     sema_init(&s, 1, USYNC_THREAD, NULL);
     sema_init(&n, 0, USYNC_THREAD, NULL);
     sema_init(&e, 4, USYNC_THREAD, NULL);

     in = out = 0;

     PRODUCER(0);
     PRODUCER(1);
     PRODUCER(2);
     CONSUMER(3);
     CONSUMER(4);
     CONSUMER(5);

     while(thr_join(NULL,NULL,NULL)==0);
     printf("all done\n");
}


void *producer(void *parm){    /* produce ten items, numbered */
     int tnum,item;            /*   0 to 9, or 10 to 19, ...  */
     tnum = *((int *) parm);
     for(item=(tnum*10);item<(tnum*10)+10;item++){
          sleep(2);
          sema_wait(&e);
          sema_wait(&s);
               insert(item);
          sema_post(&s);
          sema_post(&n);
     }
}


void *consumer(void *parm){    /* consume ten items */
     int tnum,k,item;
     tnum = *((int *) parm);
     for(k=0;k<10;k++){
          sema_wait(&n);
          sema_wait(&s);
               item = remove();
          sema_post(&s);
          sleep(3);
          printf("consumer %d got item %d\n",tnum,item);
          sema_post(&e);
     }
}
