
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
          insert(item);
     }
}


void *consumer(void *parm){    /* consume ten items */
     int tnum,k,item;
     tnum = *((int *) parm);
     for(k=0;k<10;k++){
          item = remove();
          sleep(3);
          printf("consumer %d got item %d\n",tnum,item);
     }
}
