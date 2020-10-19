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
 
     Name:     MTSS -- implementation specific definitions
 
     Type:     C include file
 
     Desc:     Data structure definitions demanded by systems services.
               The contents of these are for the MTSS implementation.
 
     File:     mt_ss.x
 
*********************************************************************21*/


#ifndef __MTSSWLX__
#define __MTSSWLX__

typedef struct mtDynBktCfg
{
   uint16_t          blkSetRelThreshold;       /* threshold value for releasing memory blocks */
   uint16_t          blkSetAcquireThreshold;   /* threshold for requesting additional memory blocks */
} MtDynBktCfg;

/* The number of blocks in the buckets and size of bloks must be 
   same as the configured in mtGlobMemCfg. */
typedef struct mtDynRegCfg
{
   Region          regionId;                  /* Region Id */
   uint16_t             numBkts;                   /* Number of buckets */
   MtDynBktCfg     bkt[SS_MAX_POOLS_PER_REG]; /* Threshold value configuration for each buckets */
} MtDynRegCfg;

/* This structure is used to store the initial values of the configuraiton
 * of the individual system taks regions. The number of buckets is same
 * as of the one configured in the gloabl region */
typedef struct mtDynMemCfg
{
   uint8_t              numRegions;           /* Number of regions */
   MtDynRegCfg     region[SS_MAX_REGS];  /* Configuration details of each region */
} MtDynMemCfg;

/* Global bucket configuration, the number of bucket, block size and the memory
 * block set size for each bucket */
typedef struct mtGlobBktCfg
{
   Size         blkSize;          /* bucket quantum size */
   uint32_t          numBlks;          /* the total blocks in the bucket */
   uint16_t          bucketSetSize;    /* Size of each bucket set */
} MtGlobBktCfg;

typedef struct mtGlobMemCfg
{
   uint16_t              numBkts;
#ifdef  XEON_SPECIFIC_CHANGES
   Size             heapSize;
#endif   
   MtGlobBktCfg     bkt[SS_MAX_POOLS_PER_REG];
}MtGlobMemCfg;

#endif

/**********************************************************************
         End of file
**********************************************************************/
