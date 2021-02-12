/*******************************************************************************
################################################################################
#   Copyright (c) [2017-2019] [Radisys]                                        #
#                                                                              #
#   Licensed under the Apache License, Version 2.0 (the "License");            #
#   you may not use this file except in compliance with the License.           #
#   You may obtain a copy of the License at                                    #
#                                                                              #
#       http://www.apache.org/licenses/LICENSE-2.0                             #
#                                                                              #
#   Unless required by applicable law or agreed to in writing, software        #
#   distributed under the License is distributed on an "AS IS" BASIS,          #
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   #
#   See the License for the specific language governing permissions and        #
#   limitations under the License.                                             #
################################################################################
*******************************************************************************/

/*
  Steps to Add a new Ring Buffer

1. Add Buffer Id to enum in ss_rbuf.h
2. Update Static SsRngCfgTbl below for ring and element size. Add a macro in ss_rbuf.h
3. typedef a structure for element type if not available in ss_rbuf.h
4. Call SCreateSRngBuf for new Buffer id from bcInitRingBuffers() in bc_cpu_init.c
5. Call SAttachSRngBuf from producer and consumer ActvInit
6. Call SEnqSRngBuf and SDeqSRngBuf from producer and consumer respectively
 
*/
#include "envopt.h"        /* environment options     */
#include "envdep.h"        /* environment dependent   */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general                 */
#include "ssi.h"           /* system services         */
#include "cm_err.h"       
#include <stdlib.h>
#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */
#include "ss_rbuf.h"
#include "ss_rbuf.x"

uint32_t ssRngBufStatus = 0;

/* Global Ring Loop Up Table */
SsRngBufTbl SsRngInfoTbl[SS_RNG_BUF_MAX];


Void SsRngBufEnable(Void)
{
  ssRngBufStatus = TRUE;
}


Void SsRngBufDisable(Void)
{
  ssRngBufStatus = FALSE;

}
/*
Func: SCreateSRngBuf
Desc: Creates Ring Buffer for the given Id.
      Ring Structure is allocated from given 
      Region and Pool
*/
S16 SCreateSRngBuf
(
uint32_t id,  
Region region,
Pool pool,
uint32_t elmSize,
uint32_t rngSize
)
{
   SsRngBuf* ring;

   /* Validate Id */
   if(id >= SS_RNG_BUF_MAX)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESSXXX, id, "Invalid RBUF ID");
#endif
      return RFAILED;
   }
   if(SsRngInfoTbl[id].r_addr != 0)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESSXXX, id, 
            "Failed to Create Ring Buffer Id Ring already exist");
#endif
      return RFAILED;
   }
   /* Get Element Size */
   
   /* Get Ring Size    */
   /* Allocate memory for Ring structure */
   ring = (SsRngBuf* )malloc(sizeof(SsRngBuf));
   if(ring == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_INT_PAR, ESSXXX, ERRZERO, "Allocating Ring  Failed!!!")
#endif
      return RFAILED;
   }
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESSXXX, ring, 
            "Failed to Create Ring Buffer Id Ring already exist");
#endif

   ring->size  = rngSize; /* No empty elem */
   ring->read  = 0;
   ring->write = 0;
   ring->type  = elmSize;

   /* Allocate elements memory */
   ring->elem  = calloc(ring->size, ring->type);
   if(ring->elem == NULLP)
    {
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_INT_PAR, ESSXXX, ERRZERO, "Allocating Ring  Failed!!!")
#endif
      free(ring);
      return RFAILED;
    }
    /* Update Buffer Id Table */
    SsRngInfoTbl[id].r_addr   = ring;
    SsRngInfoTbl[id].rngState = SS_RNG_CREATED;
    SsRngInfoTbl[id].n_write  = 0;
    SsRngInfoTbl[id].n_read   = 0;

#ifndef ALIGN_64BIT
    printf("\nRing Buffer Created with id =%ld rSize:%ld eSize:%ld %lx\n",id,ring->size,ring->type,(PTR)ring);
#else
    printf("\nRing Buffer Created with id =%d rSize:%d eSize:%d %lx\n",id,ring->size,ring->type,(PTR)ring);
#endif
    return ROK;
}

/*
Func: SAttachSRngBuf
Desc: Attach the calling Entity to a ring buffer 
      as consumer(Rx) or producer (Tx)
*/
S16 SAttachSRngBuf
(
uint32_t id,  
uint32_t ent,
uint32_t txRx
)
{
    /* Retrive Buffer from Global Info Table */
    if( id >= SS_RNG_BUF_MAX)
    {
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESSXXX, id, "Invalid RBUF ID");
#endif
       return RFAILED;
    }
    if(SsRngInfoTbl[id].rngState < SS_RNG_CREATED)
    {
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESSXXX, id, 
            "Attach Request in Invalid Ring ID");
#endif
#ifndef ALIGN_64BIT
       printf("\nAttach Request in Invalid Ring State %ld id%ld \n",
         SsRngInfoTbl[id].rngState,id);
#else
       printf("\nAttach Request in Invalid Ring State %d id%d \n",
         SsRngInfoTbl[id].rngState,id);
#endif
       return RFAILED;
    }
    if(txRx == SS_RNG_TX)
    {
        SsRngInfoTbl[id].txEnt = ent;
        SsRngInfoTbl[id].rngState = SS_RNG_TX_ATTACHED;
    }
    else if(txRx == SS_RNG_RX)
    {
        SsRngInfoTbl[id].rxEnt = ent;
        SsRngInfoTbl[id].rngState = SS_RNG_RX_ATTACHED;
    }
    return ROK;
}
/* 
Func: SConnectSRngBuf
Desc: Establish a pipe between producer and consumer

*/
S16 SConnectSRngBuf
(
uint32_t id,  
uint32_t rxEnt
)
{
   /* Send to Reciever ENT*/ 
   return ROK; 
}

/*
Func: IsFull
Desc: Checks if Ring is full
*/
inline static S16 IsFull(SsRngBuf* rBuf)
{
#if 1
   if((rBuf->write+1) == rBuf->read)
   {
     return 1;
   }
   if((rBuf->write+1) == (rBuf->read + rBuf->size))
   {
     return 1;
   }
   if((rBuf->write + 1) > rBuf->size )
   {
     return 1;
   }
   return 0;
#endif
} 
/*
Func: IsEmpty
Desc: Checks if ring is empty
*/
inline static S16 IsEmpty(SsRngBuf* rBuf)
{
   /* write == read implies Buffer is empty*/
   return (rBuf->write == rBuf->read); 
}

S16 isRngEmpty(uint32_t id)
{
   return (IsEmpty(SsRngInfoTbl[id].r_addr));
}
/*
Func: SEnqSRngBuf
Desc: Perform Queue operation on Ring bufer
*/
S16 SEnqSRngBuf 
(
uint32_t id, 
Void* elem
)
{
   uint8_t* w_ptr;
   uint8_t i=0;
   uint8_t *element = (uint8_t *)elem;
   uint32_t wrIndex;
   /* TBD To replace id with ring addr when SAttachSRngBuf is used*/
   /* Retrive Buffer from Id */
   SsRngBuf* ring = SsRngInfoTbl[id].r_addr;
   if (IsFull(ring))
   {
        SsRngInfoTbl[id].nWriteFail++;
        return RFAILED;
   }
   /* TBD Avoid multiplication for optimisation */
   w_ptr = (uint8_t*)ring->elem + (ring->write * ring->type);
   /* TBD Avoid for loop - use memcpy */
   for( i=0; i < ring->type; i++)
   {
      *(uint8_t*)w_ptr = *(uint8_t*)element;
      w_ptr++;
      (uint8_t*)element++;
   }
   /* Increment write index */
    wrIndex = ring->write + 1 ;
    ring->write = (wrIndex == ring->size)?0: wrIndex;
   /* Update Statistics */
   SsRngInfoTbl[id].n_write++;
   return ROK;
}

S16 SGetNumElemInRng(uint32_t id)
{

   S16 freeDist = (SsRngInfoTbl[id].n_write- SsRngInfoTbl[id].n_read);

	return (freeDist);
}
/*
Func: SDeqSRngBuf
Desc: Perform DeQueue operation on Ring bufer
*/
S16 SDeqSRngBuf(uint32_t id,Void *elem)
{
   uint8_t* r_ptr;
   uint8_t i=0;
   uint8_t *element = (uint8_t *)elem;
   uint32_t rdIndex;
   /* Retrive Buffer from Id*/
   SsRngBuf* ring  = SsRngInfoTbl[id].r_addr;
   if(IsEmpty(ring))
   {  
       SsRngInfoTbl[id].nReadFail++;
       return RFAILED;
   }
   r_ptr = (uint8_t*)ring->elem + (ring->read * ring->type);
   for(i=0; i<ring->type; i++)
   {
      *(uint8_t*)element = *r_ptr;
      (uint8_t*)element++;
      r_ptr++;
   }
   // Avoiding % operation for wrap around
   rdIndex= ring->read + 1;
   ring->read = (rdIndex == ring->size)?0:rdIndex;
   SsRngInfoTbl[id].n_read++;
   return ROK;
}

S16 SDestroySRngBuf 
(
uint32_t id,
Region region,
Pool pool
)
{
   /* Retrive Buffer from Id */
   SsRngBuf* ring = SsRngInfoTbl[id].r_addr;
   if(ring)
   {   
      //SPutSBuf(region, pool, (Data *)ring->elem, (ring->size * ring->type));
      //SPutSBuf(region, pool, (Data *)ring, sizeof(SsRngBuf));
      
      free(ring->elem); /* OK if null */ 
      free(ring); /* OK if null */ 

      /* Update Global Info  table */
      SsRngInfoTbl[id].rngState = SS_RNG_DESTROYED;
      SsRngInfoTbl[id].r_addr = 0;
   }
   return ROK;
}

S16 SPrintSRngStats(Void)
{
uint32_t i; 

   Txt   prntBuf[100];

#ifdef RGL_SPECIFIC_CHANGES
   return ROK;
#endif
   for(i=0; i< SS_RNG_BUF_MAX;i++)
   {
      if(SsRngInfoTbl[i].r_addr != 0 )
      {
#ifndef ALIGN_64BIT
         sprintf(prntBuf,"\n=======Ring %ld Stats========\n",i);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"r_addr      = %lx\n", (PTR)SsRngInfoTbl[i].r_addr);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"txEnt       = %lx\n", (PTR)SsRngInfoTbl[i].txEnt);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"rxEnt       = %lx\n", (PTR)SsRngInfoTbl[i].rxEnt);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"n_write     = %lu\n", SsRngInfoTbl[i].n_write);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"n_read      = %lu\n", SsRngInfoTbl[i].n_read);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"nWriteFail  = %lu\n", SsRngInfoTbl[i].nWriteFail);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"nReadFail   = %lu\n", SsRngInfoTbl[i].nReadFail);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"pktDrop     = %lu\n", SsRngInfoTbl[i].pktDrop);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"State       = %lu\n\n", SsRngInfoTbl[i].rngState);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"nPktProc    = %lu\n\n", SsRngInfoTbl[i].nPktProc);
         SDisplay(0, prntBuf);
#else
         sprintf(prntBuf,"\n=======Ring %d Stats========\n",i);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"r_addr      = %lx\n", (PTR)SsRngInfoTbl[i].r_addr);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"txEnt       = %lx\n", (PTR)SsRngInfoTbl[i].txEnt);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"rxEnt       = %lx\n", (PTR)SsRngInfoTbl[i].rxEnt);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"n_write     = %u\n", SsRngInfoTbl[i].n_write);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"n_read      = %u\n", SsRngInfoTbl[i].n_read);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"nWriteFail  = %u\n", SsRngInfoTbl[i].nWriteFail);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"nReadFail   = %u\n", SsRngInfoTbl[i].nReadFail);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"pktDrop     = %u\n", SsRngInfoTbl[i].pktDrop);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"State       = %u\n\n", SsRngInfoTbl[i].rngState);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"nPktProc    = %u\n\n", SsRngInfoTbl[i].nPktProc);
         SDisplay(0, prntBuf);

#endif
      }
   }
   return ROK; 
}

Void* SRngGetWIndx(uint32_t rngId)
{
   /* Retrive Buffer from Id*/
   SsRngBuf* ring  = SsRngInfoTbl[rngId].r_addr;
   if (IsFull(ring))
   {
      SsRngInfoTbl[rngId].nWriteFail++;
      return (NULLP);
   }
   else
   {
      return (((uint8_t *)ring->elem) + (ring->type * ring->write));
   }
}

Void* SRngGetRIndx(uint32_t rngId)
{
   /* Retrive Buffer from Id*/
   SsRngBuf* ring  = SsRngInfoTbl[rngId].r_addr;
   if(IsEmpty(ring))
   {
      SsRngInfoTbl[rngId].nReadFail++;
      return (NULLP);
   }
   else
   {
      return (((uint8_t *)ring->elem) + (ring->type * ring->read));
   }
}

Void SRngIncrWIndx(uint32_t rngId)
{
   uint32_t wrIndex;
   /* Retrive Buffer from Id*/
   SsRngBuf* ring  = SsRngInfoTbl[rngId].r_addr;
   wrIndex = ring->write + 1;
   ring->write = (wrIndex == ring->size)?0:wrIndex;
   /* Update Statistics */
   SsRngInfoTbl[rngId].n_write++;
}

Void SRngIncrRIndx(uint32_t rngId)
{
   uint32_t rdIndex;
   /* Retrive Buffer from Id*/
   SsRngBuf* ring  = SsRngInfoTbl[rngId].r_addr;
   rdIndex = ring->read + 1;
   ring->read = (rdIndex == ring->size)?0:rdIndex;
   /* Update Statistics */
   SsRngInfoTbl[rngId].n_read++;
}
#ifdef XEON_SPECIFIC_CHANGES
#if (defined (MAC_FREE_RING_BUF) || defined (RLC_FREE_RING_BUF))
S16 mtAddBufToRing(SsRngBufId ringId,void *bufPtr,uint8_t freeType)
{
   S16 ret1 = ROK;
   
   Void *elem = NULLP;

   RgKwFreeInfo *bufFreeInfo = NULLP;
   elem = SRngGetWIndx(ringId);

   if (NULLP != elem)
   {
      bufFreeInfo = (RgKwFreeInfo *) elem;

      bufFreeInfo->bufToFree = bufPtr;
      bufFreeInfo->freeType  = freeType;

      SRngIncrWIndx(ringId);

      SsRngInfoTbl[ringId].pktRate++;
   }   
   else
   {
      printf("\nFree Ring FULL id %d!!! \n",ringId);
      SsRngInfoTbl[ringId].pktDrop++;
      ret1 = RFAILED;
   }
   return (ret1);
}
#endif
#endif



/**********************************************************************
         End of file
**********************************************************************/
