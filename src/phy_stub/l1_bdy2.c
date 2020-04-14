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
   }
}

void duStartSlotIndicaion()
{
   pthread_t thread;
   int ret;

   ret = pthread_create(&thread, NULL, GenerateTicks, NULL);
   if(ret)
   {
      printf("\nUnable to create thread");
   }
}


