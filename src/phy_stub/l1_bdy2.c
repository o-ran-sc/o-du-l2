/* This file handles TTI genertion */

#include <stdio.h>
#include <pthread.h>
#include "lphy_stub.h"

void *GenerateTicks(void *arg)
{
   while(1)
   {
      sleep(1);
      printf("\nPHY STUB : Slot indication");

      /* Send Slot indication indication to lower mac */
      //buildAndSendSlotIndication();
      duSendEgtpSlotInd();
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


