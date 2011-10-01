/* CPSC 422/622 threads program for dining philosophers */

#include <thread.h>
#include <synch.h>
#include <errno.h>

void *philosopher(void *);

sema_t s;
sema_t ch[5];

int main(){
   thread_t t[5];
   int i,num[5];
   printf("key:\n");
   printf("  Ad - philosopher d arrives restaurant\n");
   printf("  Ld - philosopher d gets chopstick on left\n");
   printf("  Rd - philosopher d gets chopstick on right and can now eat\n");
   printf("  Td - philosopher d puts chopsticks down and thinks\n");
   printf("  Xd - philosopher d has solved the world's problems and goes home\n");
   sema_init(&s, 4, USYNC_THREAD, NULL);
   for(i=0;i<5;i++)
      sema_init(&ch[i], 1, USYNC_THREAD, NULL);
   for(i=0;i<5;i++){
      num[i] = i;
      thr_create(NULL, 0, philosopher, (void *) &(num[i]), (long) 0, &t[i]);
   }
   while(thr_join(NULL,NULL,NULL)==0);
}

void *philosopher(void *ptr){
   int i,j;
   char pad[] = "                                                               ";
   i = *((int *)ptr);
   pad[4*i] = '\0';
   printf("%sA%d\n",pad,i);
   for(j=0;j<4;j++){

/* This is a bit confusing, but it works.  If we are waiting on the left
 * chopstick, then we block.  But if we get the left chopstick, then we
 * don't want to hold the left chopstick while waiting on the right
 * chopstick, so we check to see if the right chopstick is available.
 * If it is not, then we release the left chopstick, and block on the
 * right chopstick, and repeat the above as necessary.
 */
wait1:
        sema_wait(&ch[(i+4)%5]);
        if(sema_trywait(&ch[i])) {
           sema_post(&ch[(i+4)%5]);
           goto wait2;
        }
        goto done;
wait2:
        sema_wait(&ch[i]);
        if(sema_trywait(&ch[(i+4)%5])) {
           sema_post(&ch[i]);
           goto wait1;
        }
done:

     printf("%sL%d\n",pad,i);
     printf("%sR%d\n",pad,i);
     sleep(i+2);
     printf("%sT%d\n",pad,i);
     sema_post(&ch[(i+4)%5]);
     sema_post(&ch[i]);
     sleep(i+2);
   }
   printf("%sX%d\n",pad,i);
}
