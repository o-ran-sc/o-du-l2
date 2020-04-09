/* This file handles TTI genertion */
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "lphy_stub.h"

void *GenerateTicks(void *arg)
{
   uint8_t counter = 2;
   while(counter)
   {
      sleep(1);
      printf("\nPHY_STUB: SLOT indication");
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
      printf("\nPHY_STUB: Unable to create thread");
   }
}


