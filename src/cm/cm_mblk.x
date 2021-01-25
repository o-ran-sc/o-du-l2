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
  
     Desc:     Structures, variables and typedefs required by the
               memory library routines 
 
     File:     cm_mblk.x
 
*********************************************************************21*/
  
#ifndef __CMMBLKX__
#define __CMMBLKX__

#ifdef __cplusplus
extern "C" {
#endif

/************************************************
 Memory Management Control Block 
************************************************/

typedef struct cmMemCb
{
   Size      maxSize;      /* Size of memory chunk */ 
   Mem       sMem;         /* Static memory region and pool */ 
   uint32_t       memAllocated; /* amount of memory already allocated */
   PTR       initPtr;      /* Initial pointer */
   PTR       runPtr;       /* Start of avaliable memory chunk */ 
} CmMemCb;

/************************************************
 Memory Link List Node Structure
************************************************/
typedef struct  cmMemList CmMemList;

struct cmMemList
{
   CmMemList *next;        /* next */
   CmMemList *prev;        /* previous */
   /* cm_mblk_x_001.101: added new element "size" */
   Size       size;        /* block size */
};

/************************************************
 Memory Link List Control Point 
************************************************/

typedef struct cmMemListCp
{
   CmMemList *first;       /* first entry in list */
   CmMemList *last;        /* last entry in list */
   uint32_t       count;        /* number of entries */
   CmMemCb   memCb;        /* Memory Control Block */
}CmMemListCp;

/***********************************************
 Memory Status structure 
***********************************************/
typedef struct cmMemStatus
{
   Mem       sMem;          /* Static Memory region,pool */
   uint32_t       memBlkCnt;     /* Memory Blocks Count */ 
   Size      maxBlkSize;    /* Size of memory Block */
   Size      memAllocated;  /* Memory allocated off chunk */
} CmMemStatus;


/***********************************************
 Extern Declarations
***********************************************/

#ifdef TFU_ALLOC_EVENT_NO_INIT
 S16 cmAllocEvntNoInit ARGS((
                                   Size evntSize,
                                   Size maxBlkSize,
                                   Mem  *sMem, 
                                   Ptr  *ptr));
#endif
#ifdef TFU_ALLOC_EVENT_NO_INIT
 S16 cmGetMemNoInit ARGS((
                               Ptr  memPtr,
                               Size size, 
                               Ptr  *allocPtr));
#endif
 S16 cmAllocEvnt ARGS((
                                   Size evntSize,
                                   Size maxBlkSize,
                                   Mem  *sMem, 
                                   Ptr  *ptr));

 Void cmInitMemCp ARGS((
                                   CmMemListCp *memCp,
                                   Size  maxBlkSize,
                                   Mem   *sMem )); 

#ifdef TFU_ALLOC_EVENT_NO_INIT
 S16 cmGetMemNoInit ARGS((
                               Ptr  memPtr,
                               Size size, 
                               Ptr  *allocPtr));
#endif
 S16 cmGetMem ARGS((
                               Ptr  memPtr,
                               Size size, 
                               Ptr  *allocPtr));

 Void cmFreeMem ARGS((
                                Ptr memPtr));

 Void cmGetMemStatus ARGS((
                                  Ptr         memPtr,
                                  CmMemStatus *status));

#ifdef __cplusplus
}
#endif

#endif /* __CMMBLKX__ */

 
/********************************************************************30**
  
         End of file
**********************************************************************/
