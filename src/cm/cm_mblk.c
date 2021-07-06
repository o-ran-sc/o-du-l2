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

/********************************************************************20**
  
     Name:     common memory allocation library 
  
     Type:     C include file
  
     Desc:     memory library routines 
 
     File:     cm_mblk.c
  
*********************************************************************21*/


/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm_mblk.h"       /* Header file */

/* header/extern include files (.x) */
#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */
#include "cm_lib.x"        /* Common library */
#include "cm_mblk.x"       /* Typedef file */

#ifdef SS_LOCKLESS_MEMORY
pthread_t tmpRegTidMap[20];
#define CM_MEM_GET_REGION(_region)                        \
{                                                         \
   uint8_t  _regCnt;                                           \
   _region = 0xFF;                                        \
                                                          \
   for(_regCnt = 0; _regCnt < 12; _regCnt++)              \
   {                                                      \
      if(tmpRegTidMap[_regCnt] == pthread_self())         \
      {                                                   \
         _region = _regCnt;                               \
         break;                                           \
      }                                                   \
   }                                                      \
}
#endif



static Void cmAddMemNode ARGS((CmMemListCp *lCp,CmMemList *node)); 


/*
*
*       Fun:   cmAllocEvnt
*
*       Desc:  This function allocates the first memory chunk,
*              which contains CmMemListCp structure at the top,
*              parcels the requested event structure out of this
*              chunk and return to the user.
*
*       Ret:   ROK 
*
*       Notes: None 
*
*       File:  cm_mblk.c
*
*/
  
S16 cmAllocEvnt
(
Size            evntSize,    /* Size of the Event structure */
Size            maxBlkSize,  /* Chunk Memory size */
Mem             *sMem,       /* Static memory region and pool */
Ptr             *ptr         /* Location to place allocated event ptr */
)
{

  Data          *allocPtr;  /* Allocated Memory Pointer */
  CmMemList     *node;      /* Memory Link List Node */
  CmMemListCp   *memCp;     /* memory Link list control point */
  CmMemCb       *cb;        /* Allocated Memory Control Block */


  /* Validation check */
#ifndef LTE_ENB_PAL
  if( evntSize > maxBlkSize)
  {
     DU_LOG("\nERROR  -->  Not Allocating memory for Event\n");
#ifdef ALIGN_64BIT
     DU_LOG("\nERROR  -->  eventSize [%d] greater than maxBlkSize [%d]\n",
           evntSize, maxBlkSize);
#else
     DU_LOG("\nERROR  -->  eventSize [%ld] greater than maxBlkSize [%ld]\n",
           evntSize, maxBlkSize);
#endif
     return RFAILED;
  }
#endif
 
  /* Allocate memory for the first Memory Chunk */
  /* Allocated memory should be maxBlkSize + sizeof(CmMemList) */
#ifdef SS_LOCKLESS_MEMORY
   if(SGetStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,sMem->region, sMem->pool, (Data **)&allocPtr,
                       (Size)(maxBlkSize + sizeof(CmMemList)), 0) != ROK)
#else
  if (SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,sMem->region, sMem->pool, 
               (Data **)&allocPtr, 
               (Size)(maxBlkSize + sizeof(CmMemList))) != ROK)
#endif /* SS_LOCKLESS_MEMORY */

     return RFAILED;

  /* Reset the contents */
  memset(allocPtr, 0, 
           (maxBlkSize + sizeof(CmMemList)) );

  /* The above allocated memory chunk is structured as follows 

  +-------------------+
  |  CmMemList        |
  +-------------------+   <---- Event Structure begins here
  |  CmMemListCp      |      ^
  +-------------------+      |
  |  Event data part  |      |  evntSize
  +-------------------+      |
       ....                  |
       ....                  \/ 
  +-------------------+    
  
  */

  /* Overlay CmMemList structure on top of this allocated chunk */
  node = (CmMemList *)allocPtr;
  /* cm_mblk_c_001.101: update size value */
  node->size = maxBlkSize;

  /* Get to CmMemListCp start */
  memCp = (CmMemListCp *) ((PTR)allocPtr + sizeof(CmMemList));
  
  /* Initialise memListCp */
  CM_INIT_MEMCP( memCp,maxBlkSize,sMem);

  /* Add link list node to link list */
  cmAddMemNode(memCp, node); 

  /* Get pointer to memCb inside MemListCp */
  cb = (CmMemCb *)&(memCp->memCb);

  /* Align evntSize, if necessary */
  CM_ALIGN_SIZE(evntSize);

  cb->memAllocated = (evntSize + sizeof(CmMemList) );
  cb->initPtr = (PTR)allocPtr;
  *ptr = (Ptr) ((PTR)allocPtr + sizeof(CmMemList));
  cb->runPtr = ((PTR)(*ptr) + evntSize);

  return ROK;

} /* End of cmAllocEvnt */


/*
*
*       Fun:   cmInitMemCp
*
*       Desc:  This function intialises Memory Link List
*              Control point  
*
*       Ret:   ROK 
*
*       Notes: None 
*
*       File:  cm_mblk.c
*
*/
  
Void cmInitMemCp
(
CmMemListCp     *memCp,     /* Memory control pointer */
Size            maxBlkSize, /* Chunk Memory size */
Mem             *sMem       /* Static memory region and pool */
)
{

  /* Intialise Memory Control Point */
  CM_INIT_MEMCP(memCp,maxBlkSize,sMem);

  return;

} /* End of cmInitMemCp */ 


/*
*
*       Fun:   cmGetMem
*
*       Desc:  This function parcels memory from memory chunks 
*              It allocated big chunk as and when required
*
*       Ret:   ROK 
*
*       Notes: None 
*
*       File:  cm_mblk.c
*
*/
  
S16 cmGetMem
(
Ptr           memPtr,    /* Pointer to memCp */
Size          size,      /* Memory size requested */
Ptr           *allocPtr  /* location to place pointer */
)
{
 
  CmMemCb   *cb;       /* Pointer to Memory Control Block */
  CmMemList *node;     /* Memory List node */
  CmMemListCp *memCp;  /* Memory Control Point */
  /* cm_mblk_c_001.101: added local variable */
  Size       blkSize;  /* required block size */


  memCp = (CmMemListCp *)memPtr;
  cb = &memCp->memCb; 

  /* Make requested size aligned, if necessary */
  CM_ALIGN_SIZE(size);

  /* cm_mblk_c_001.101 : If the requested size is greater than the configured
   * max size, then allocate a new block with the requested size. This will
   * enable us to handle large allocation requests in H.323 stack for certain
   * information elements. This will not impact any normal allocation request
   * as we fall back to the configured size */
   if( size > cb->maxSize)
     blkSize = size;
   else
     blkSize = cb->maxSize;
 
  if( cb->initPtr)
  { 
    /* if a chunk is already there */
    if( (cb->memAllocated + size) <= 
           (uint32_t)(cb->maxSize + sizeof(CmMemList)) )
    {
      /* Requested memory is available in present chunk */
      *allocPtr = (Ptr) cb->runPtr;
      cb->memAllocated += size;
      cb->runPtr += size;
      return ROK;
    }
  }
  
  /* For all other cases, We need to allocate a new memory chunk */
  /* Allocate buffer */
  /* cm_mblk_c_001.101: use blkSize instead of cb->maxSize */
#ifdef SS_LOCKLESS_MEMORY
   if(SGetStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,cb->sMem.region, cb->sMem.pool, 
                       (Data **)&(cb->initPtr),
                       (Size)(blkSize + sizeof(CmMemList)), 0) != ROK)
#else
  if (SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,cb->sMem.region, cb->sMem.pool, 
              (Data **)&(cb->initPtr),
              (Size)(blkSize + sizeof(CmMemList)) )  != ROK)
#endif /* SS_LOCKLESS_MEMORY */
     return RFAILED;

  /* Reset the contents */
  /* Initialise above allocated structure */
  /* cm_mblk_c_001.101: use blkSize instead of cb->maxSize */
  memset(&(cb->initPtr), 0, 
           (blkSize + sizeof(CmMemList) ));

  /* The above allocated memory chunk is structured as follows 

  +-------------------+
  |  CmMemList        |
  +-------------------+
      Data Portion
       ....
       ....
  +-------------------+    */

  /* Overlay CmMemList structure on top of this allocated chunk */
  node = (CmMemList *)cb->initPtr;
  /* cm_mblk_c_001.101: update size */
  node->size = blkSize;

  /* Add link list node to link list */
  cmAddMemNode(memCp, node); 

  cb->memAllocated = (size + sizeof(CmMemList) );
  *allocPtr = (Ptr) ((PTR)cb->initPtr + sizeof(CmMemList));
  cb->runPtr = ((PTR)(*allocPtr) + size);

  return ROK;   

} /* End of cmGetMem */


#ifdef TFU_ALLOC_EVENT_NO_INIT

/*
*
*       Fun:   cmAllocEvntNoInit
*
*       Desc:  This function allocates the first memory chunk,
*              which contains CmMemListCp structure at the top,
*              parcels the requested event structure out of this
*              chunk and return to the user.
*
*       Ret:   ROK 
*
*       Notes: None 
*
*       File:  cm_mblk.c
*
*/
  
S16 cmAllocEvntNoInit
(
Size            evntSize,    /* Size of the Event structure */
Size            maxBlkSize,  /* Chunk Memory size */
Mem             *sMem,       /* Static memory region and pool */
Ptr             *ptr         /* Location to place allocated event ptr */
)
{

  Data          *allocPtr;  /* Allocated Memory Pointer */
  CmMemList     *node;      /* Memory Link List Node */
  CmMemListCp   *memCp;     /* memory Link list control point */
  CmMemCb       *cb;        /* Allocated Memory Control Block */


  /* Validation check */
  if( evntSize > maxBlkSize)
     return RFAILED;
 
  /* Allocate memory for the first Memory Chunk */
  /* Allocated memory should be maxBlkSize + sizeof(CmMemList) */
#ifdef SS_LOCKLESS_MEMORY
   if(SGetStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,sMem->region, sMem->pool, (Data **)&allocPtr,
                       (Size)(maxBlkSize + sizeof(CmMemList)), 0) != ROK)
#else
  if (SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,sMem->region, sMem->pool, 
               (Data **)&allocPtr, 
               (Size)(maxBlkSize + sizeof(CmMemList))) != ROK)
#endif /* SS_LOCKLESS_MEMORY */

     return RFAILED;

  /* Reset the contents */
  memset(allocPtr, 0, 
           (sizeof(CmMemList)) );

  /* The above allocated memory chunk is structured as follows 

  +-------------------+
  |  CmMemList        |
  +-------------------+   <---- Event Structure begins here
  |  CmMemListCp      |      ^
  +-------------------+      |
  |  Event data part  |      |  evntSize
  +-------------------+      |
       ....                  |
       ....                  \/ 
  +-------------------+    
  
  */

  /* Overlay CmMemList structure on top of this allocated chunk */
  node = (CmMemList *)allocPtr;
  /* cm_mblk_c_001.101: update size value */
  node->size = maxBlkSize;

  /* Get to CmMemListCp start */
  memCp = (CmMemListCp *) ((PTR)allocPtr + sizeof(CmMemList));
  
  /* Initialise memListCp */
  CM_INIT_MEMCP( memCp,maxBlkSize,sMem);

  /* Add link list node to link list */
  cmAddMemNode(memCp, node); 

  /* Get pointer to memCb inside MemListCp */
  cb = (CmMemCb *)&(memCp->memCb);

  /* Align evntSize, if necessary */
  CM_ALIGN_SIZE(evntSize);

  cb->memAllocated = (evntSize + sizeof(CmMemList) );
  cb->initPtr = (PTR)allocPtr;
  *ptr = (Ptr) ((PTR)allocPtr + sizeof(CmMemList));
  cb->runPtr = ((PTR)(*ptr) + evntSize);

  return ROK;

} /* End of cmAllocEvntNoInit */

/*
*
*       Fun:   cmGetMemNoInit
*
*       Desc:  This function parcels memory from memory chunks 
*              It allocated big chunk as and when required
*
*       Ret:   ROK 
*
*       Notes: None 
*
*       File:  cm_mblk.c
*
*/
  
S16 cmGetMemNoInit
(
Ptr           memPtr,    /* Pointer to memCp */
Size          size,      /* Memory size requested */
Ptr           *allocPtr  /* location to place pointer */
)
{
 
  CmMemCb   *cb;       /* Pointer to Memory Control Block */
  CmMemList *node;     /* Memory List node */
  CmMemListCp *memCp;  /* Memory Control Point */
  /* cm_mblk_c_001.101: added local variable */
  Size       blkSize;  /* required block size */


  memCp = (CmMemListCp *)memPtr;
  cb = &memCp->memCb; 

  /* Make requested size aligned, if necessary */
  CM_ALIGN_SIZE(size);

  /* cm_mblk_c_001.101 : If the requested size is greater than the configured
   * max size, then allocate a new block with the requested size. This will
   * enable us to handle large allocation requests in H.323 stack for certain
   * information elements. This will not impact any normal allocation request
   * as we fall back to the configured size */
   if( size > cb->maxSize)
     blkSize = size;
   else
     blkSize = cb->maxSize;
 
  if( cb->initPtr)
  { 
    /* if a chunk is already there */
    if( (cb->memAllocated + size) <= 
           (uint32_t)(cb->maxSize + sizeof(CmMemList)) )
    {
      /* Requested memory is available in present chunk */
      *allocPtr = (Ptr) cb->runPtr;
      //memset(*allocPtr, (uint8_t )0, 
        //   (PTR)(size) );
      cb->memAllocated += size;
      cb->runPtr += size;
      return ROK;
    }
  }
  
  /* For all other cases, We need to allocate a new memory chunk */
  /* Allocate buffer */
  /* cm_mblk_c_001.101: use blkSize instead of cb->maxSize */
#ifdef SS_LOCKLESS_MEMORY
   if(SGetStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,cb->sMem.region, cb->sMem.pool, 
                       (Data **)&(cb->initPtr),
                       (Size)(blkSize + sizeof(CmMemList)), 0) != ROK)
#else
  if (SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,cb->sMem.region, cb->sMem.pool, 
              (Data **)&(cb->initPtr),
              (Size)(blkSize + sizeof(CmMemList)) )  != ROK)
#endif /* SS_LOCKLESS_MEMORY */
     return RFAILED;

  /* Reset the contents */
  /* Initialise above allocated structure */
  /* cm_mblk_c_001.101: use blkSize instead of cb->maxSize */
  memset(&(cb->initPtr), 0, 
           (sizeof(CmMemList)));
  /* The above allocated memory chunk is structured as follows 

  +-------------------+
  |  CmMemList        |
  +-------------------+
      Data Portion
       ....
       ....
  +-------------------+    */

  /* Overlay CmMemList structure on top of this allocated chunk */
  node = (CmMemList *)cb->initPtr;
  /* cm_mblk_c_001.101: update size */
  node->size = blkSize;

  /* Add link list node to link list */
  cmAddMemNode(memCp, node); 

  cb->memAllocated = (size + sizeof(CmMemList) );
  *allocPtr = (Ptr) ((PTR)cb->initPtr + sizeof(CmMemList));
  cb->runPtr = ((PTR)(*allocPtr) + size);

  return ROK;   

} /* End of cmGetMemNoInit */




#endif

/*
*
*       Fun:   cmFreeMem
*
*       Desc:  This function frees memory chunks after
*              traversing link list
*
*       Ret:   ROK 
*
*       Notes: None 
*
*       File:  cm_mblk.c
*
*/
  
Void cmFreeMem
(
Ptr    memPtr      /* Link List CP */
)
{
  Mem         sMem;      /* Static Memory region and pool */
  S32         count;     /* Count of linked blocks */ 
  /* cm_mblk_c_001.101: removed local variable maxSize */
  CmMemList   *node;     /* Pointer to link list node */
  CmMemList   *prevNode; /* Pointer to previous node */
  CmMemListCp *lcp;      /* Memory Link List */


  lcp = (CmMemListCp *)memPtr;

  sMem.region = lcp->memCb.sMem.region;
  sMem.pool   = lcp->memCb.sMem.pool;
  count       = lcp->count; 
  /* cm_mblk_c_001.101: removed getting maxSize value */

  /* Free Memory by traversing Back to Front */
  node = lcp->last;

  /* Initialise memCp running variables */
  /* User may want to reuse same memCp for further */
  /* allocations, if memCp was not a part of */
  /* event structure */
  CM_INIT_MEMCPVAR(lcp);

  while(count && node)
  {
    prevNode = node->prev;

    /* Release complete memory for present chunk */
    /* cm_mblk_c_001.101: use node->size instead of maxSize */
    if( node )
    {
#ifdef SS_LOCKLESS_MEMORY
       SPutStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,sMem.region, sMem.pool,
                        (Data *)node, (node->size + sizeof(CmMemList)), 0);
#else
       SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,sMem.region,sMem.pool, 
                (Data *)node, (node->size + sizeof(CmMemList)));
#endif /* SS_LOCKLESS_MEMORY */
    }
    node = prevNode;
    count--;
  }

  return;   

} /* End of cmFreeMem */


/*
*
*       Fun:   cmAddMemNode
*
*       Desc:  adds node to Memory linked list after last.
*
*       Ret:   ROK   - ok
*
*       Notes: None
*
*       File:  cm_mblk.c
*
*/
static Void cmAddMemNode
(
CmMemListCp *lCp,               /* list control point */
CmMemList   *node               /* node to be added */
)
{

   lCp->count++;

   node->prev = lCp->last;
   node->next = NULLP;
   lCp->last = node;
   
   if (!node->prev)
   {
      lCp->first = node;
      return;
   }
   
   node->prev->next = node;
   return;

} /* end of cmAddMemNode */



/*
*
*       Fun:   cmGetMemStatus
*
*       Desc:  This function returns the static memory status with
*              parameters such as memory  region and pool etc
*
*       Ret:   ROK 
*
*       Notes: None 
*
*       File:  cm_mblk.c
*
*/
  
Void cmGetMemStatus
(
Ptr             memPtr,    /* Memory control pointer */
CmMemStatus     *status    /* memory region,pool and status */
)
{

  CmMemListCp *memCp;    /* Memory Link List */


  memCp = (CmMemListCp *)memPtr;

  /* Copy relevant parameters */
  status->sMem.region  = memCp->memCb.sMem.region;
  status->sMem.pool    = memCp->memCb.sMem.pool;
  status->memBlkCnt    = memCp->count;
  status->maxBlkSize   = memCp->memCb.maxSize;
  status->memAllocated = memCp->memCb.memAllocated;

  return;

} /* End of cmGetMemStatus */ 

/**********************************************************************
         End of file
**********************************************************************/
