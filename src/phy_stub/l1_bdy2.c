/* This file handles TTI genertion */

#include <stdio.h>
#include <pthread.h>
#include "lphy_stub.h"

void *GenerateTicks(void *arg)
{
   while(1)
   {
      sleep(10);
      printf("\nTTI indication");
      /* Send TTI indication to CL */
      duSendEgtpTTIInd();
   }
}

void duStartTtiThread()
{
   pthread_t ttiThread;
   int ret;

   ret = pthread_create(&ttiThread, NULL, GenerateTicks, NULL);
   if(ret)
   {
      printf("\nUnable to create thread");
   }
}


