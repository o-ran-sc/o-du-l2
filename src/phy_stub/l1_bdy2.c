/* This file handles TTI genertion */

#include <stdio.h>
#include <pthread.h>
#include "lphy_stub.h"

void *GenerateTicks(void *arg)
{
   uint8_t counter = 1;
   while(counter)
   {
      sleep(1);
      printf("\nTTI indication");
      /* Send TTI indication to CL */
      //duSendEgtpTTIInd();
      buildAndSendSlotIndication();
		counter--;
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


