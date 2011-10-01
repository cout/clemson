/* CPSC 422/622 threads program for dining philosophers */

#include <thread.h>
#include <synch.h>
#include <errno.h>

void *philosopher(void *);

int main(){
   thread_t t[5];
   int i,num[5];
   printf("key:\n");
   printf("  Ad - philosopher d arrives restaurant\n");
   printf("  Ld - philosopher d gets chopstick on left\n");
   printf("  Rd - philosopher d gets chopstick on right and can now eat\n");
   printf("  Td - philosopher d puts chopsticks down and thinks\n");
   printf("  Xd - philosopher d has solved the world's problems and goes home\n");
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
     printf("%sL%d\n",pad,i);
     printf("%sR%d\n",pad,i);
     sleep(i+2);
     printf("%sT%d\n",pad,i);
     sleep(i+2);
   }
   printf("%sX%d\n",pad,i);
}
