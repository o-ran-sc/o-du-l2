#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _p{
char *func;
int size;
char *add;
}Alloc;

int allocline;
int freeline;
Alloc alloc[1000000];
Alloc freemem[1000000];

char allocStr1[] = "ALLOC";
char allocStr2[] = "ALLOC_WC";
char freeStr[] = "FREE";
char shrbAllocStr1[] = "ALLOC_SHRABL_BUF";
char shrbAllocStr2[] = "ALLOC_SHRABL_BUF_WC";
char shrbAllocStr3[] = "SHRABL_STATIC_BUF_ALLOC";
char shrbFreeStr1[] = "FREE_SHRABL_BUF";
char shrbFreeStr2[] = "SHRABL_STATIC_BUF_FREE";

int main()
{
   char buffer[10000];
   int i=0;

   /* Open input file pointer */
   FILE *allocfp=fopen("alloc.txt","r");
   FILE *freefp=fopen("free.txt","r");

   /* Open output file pointer */
   FILE *allocop=fopen("allocoutput.txt","w");
   FILE *freeop=fopen("freeoutput.txt","w");

   /* If alloc.txt is open, scan line by line and store "memory size" and
    * "memory address" in alloc[] structure */
   if (!allocfp)
   {
      return 0;
   }
   while(fgets(buffer, 10000, allocfp))
   {
      alloc[i].func=malloc(20);
      alloc[i].add=malloc(20);
      sscanf(buffer,"%s %d %s", (alloc[i].func), &(alloc[i].size), (alloc[i].add));
      //printf("%d %s\n",alloc[i].size, alloc[i].add);
      i++;
   }
   allocline=i;

   /* If free.txt is open, scan line by line and store "memory size" and
    * "memory address" in freemem[] structure */
   i=0;
   if (!freefp)
   {
      return 0;
   }
   while(fgets(buffer, 10000, freefp))
   {
      freemem[i].func= malloc(20);
      freemem[i].add=malloc(20);
      sscanf(buffer,"%s %d %s", (freemem[i].func), &(freemem[i].size), (freemem[i].add));
      //printf("%d %s\n",freemem[i].size, freemem[i].add);
      i++;
   }
   freeline=i;

   /* Scan through all entries in alloc[] and freemem[].
    * If an entry is found in both array, with same size and address, remove this
    * entry from both */
   int count1=0;
   int count2=0;
   for(count1; count1<allocline; count1++)
   {
      for(count2=0; count2<freeline; count2++)
      {
         if(( alloc[count1].size == freemem[count2].size ) &&
               ( !(strcmp(alloc[count1].add,freemem[count2].add)))
           )
         {
            if(
                  (!(strcmp(alloc[count1].func, allocStr1) && strcmp(alloc[count1].func, allocStr2)) && !(strcmp(freemem[count2].func, freeStr))) ||
                  (!(strcmp(alloc[count1].func, shrbAllocStr1) && strcmp(alloc[count1].func, shrbAllocStr2) && strcmp(alloc[count1].func, shrbAllocStr3))\
                  && !(strcmp(freemem[count2].func, shrbFreeStr1) && strcmp(freemem[count2].func, shrbFreeStr2)))
              )
            {
               //printf("%d %s\n",alloc[count1].size, alloc[count1].add);
               freemem[count2].size=-1;
               free(freemem[count2].func);
               freemem[count2].func = 0;
               free(freemem[count2].add);
               freemem[count2].add=0;

               alloc[count1].size=-1;
               free(alloc[count1].func);
               alloc[count1].func=0;
               free(alloc[count1].add);
               alloc[count1].add=0;
               break;
            }
         }
         else
         {
         }
      }
      if (count2 == freeline)
      {
         //printf("%d %s\n",alloc[count1].size, alloc[count1].add);
      }
   }

   /* Add the remaining entries of array[] in allocoutput.txt. These are memory
    * addresses allocated but not freed */
   printf("=========== %d %d\n",allocline,freeline);
   for(count1=0; count1<allocline; count1++)
   {
      if( (alloc[count1].size!=-1) && (alloc[count1].add !=0) )
      {
         fprintf(allocop,"%d %s\n",alloc[count1].size, alloc[count1].add);
      }
   }

   /* Add the remaining entries of freemem[] in freeoutput.txt. These are memory
    * addresses freed but not allocated */
   for(count2=0; count2<freeline; count2++)
   {
      if( (freemem[count2].size!=-1) && (freemem[count2].add !=0) )
      {
         fprintf(freeop,"%d %s\n",freemem[count2].size, freemem[count2].add);
      }
   }
   return 0;
}
