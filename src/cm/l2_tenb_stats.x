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

/**********************************************************************
 
    Name:  
 
    Type:  
 
    Desc:  
 
    File:  l2_tenb_stats.x 
 
**********************************************************************/
 
/** @file l2_tenb_stats.x
*/

#ifdef TENB_STATS
#ifndef __TENBSTATSL2X__
#define __TENBSTATSL2X__

#include "cm_tenb_stats.x"
#include "l2_tenb_stats.h"
 
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef TSInfL2CellStats TSL2CellStatsCb;

typedef struct tSL2UeStatsCb
{
   CmLList         lnk;
   uint32_t             inUse;

   TSInfL2UeStats  stats;
}TSL2UeStatsCb;

Void TSL2AllocStatsMem ARGS((
  Inst inst
));
TSL2UeStatsCb* TSL2AllocUeStatsBlk ARGS((
 uint16_t   rnti
));
Void TSL2DeallocUeStatsBlk ARGS((
 uint16_t         rnti,
 TSL2UeStatsCb   *statsCb
));
TSL2CellStatsCb* TSL2AllocCellStatsBlk ARGS((
 uint32_t cellId
));
Void TSL2DeallocCellStatsBlk ARGS((
 uint32_t cellId
));
Void TSL2SendStatsToApp ARGS((
 Pst    *pst,
 SuId   suId
));

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __TENBSTATSL2X__ */
#endif /* TENB_STATS */

/**********************************************************************
         End of file
**********************************************************************/
