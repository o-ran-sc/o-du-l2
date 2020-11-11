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

/************************************************************************
 
     Name:     LTE-MAC layer
  
     Type:     C source file
  
     Desc:     C source code to provide software part number, version and
               revision. 
  
     File:     rg_id.c 
  
**********************************************************************/

/** @file rg_id.c
@brief This file contains the definition of this software's part number, 
       version and revision. Also provides an API to access the same.
*/

/* header include files (.h) */
#include "common_def.h"
#include "lrg.h"              /* layer manager */
   
/* header/extern include files (.x) */
#include "cm_lib.x"           /* common library */


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
Void rgGetSId ARGS((SystemId *s));
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* defines */
#define RGSWMV 3             /* LTE MAC- main version */
#define RGSWMR 1             /* LTE MAC - main revision */
#define RGSWBV 0             /* LTE MAC - branch version */
/* Updated the branch revision Number */
#define RGSWBR 2             /* LTE MAC - branch revision */
#define RGSWPN "1000372"     /* LTE MAC - part number */

/* system id */
static const SystemId sId ={
   RGSWMV,              /* LTE MAC - main version */
   RGSWMR,              /* LTE MAC - main revision */
   RGSWBV,              /* LTE MAC - branch version */
   RGSWBR,              /* LTE MAC - branch revision */
   (Txt *)RGSWPN               /* LTE MAC - part number */
};


/**
 * @brief Retrieve system id. 
 *
 * @details
 *
 *     Function : rgGetSId 
 *    
 *     Get system id consisting of part number, main version and
 *     revision and branch version and branch.
 *     
 *     
 *  @param[out] SystemId *s  
 *  @return  void
 **/
/*
*
*       Fun:   get system id
*
*       Desc:  Get system id consisting of part number, main version and
*              revision and branch version and branch.
*
*       Ret:   TRUE      - ok
*
*       Notes: None
*
*       File:  rg_id.c
*
*/
 
Void rgGetSId
(
SystemId *s                 /* system id */
)
{
   s->mVer = sId.mVer;
   s->mRev = sId.mRev;
   s->bVer = sId.bVer;
   s->bRev = sId.bRev;
   memcpy(s->ptNmb, sId.ptNmb, LRG_MAX_PT_NUM_SIZE); 

   /* Stack Crash Problem for TRACE5 Changes. Added the return below */
  return; 
  
} /* end of rgGetSid */



/**********************************************************************
 
         End of file
**********************************************************************/
