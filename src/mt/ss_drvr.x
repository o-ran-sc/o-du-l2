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
 
     Name:     System Services -- Driver tasks
 
     Type:     C include file
 
     Desc:     Data structure definitions required for driver tasks.
 
     File:     ss_drvr.x
 
*********************************************************************21*/


#ifndef __SSDRVRX__
#define __SSDRVRX__

#ifdef __cplusplus
extern "C" {
#endif


#ifdef SS_DRVR_SUPPORT

/* individual entry in the table of driver tasks */
typedef struct ssDrvrTskEntry
{
   /* Any implementation specific content */
   SsdDrvrTskEntry dep;


   /* Common content */
   Bool         used;                   /* entry is used or not */
   Inst         channel;                /* channel ID */
   ActvTsk      actvTsk;                /* activation function */
   ISTsk        isTsk;                  /* interrupt service task */
   ProcId       low;                    /* processor ID -- low */
   ProcId       high;                   /* processor ID -- high */

   SLockId      lock;                   /* to serialize calls to the
                                           activation function */

} SsDrvrTskEntry;

#endif  /* SS_DRVR_SUPPORT */


#ifdef __cplusplus
}
#endif

#endif  /*  __SSDRVRX__  */


  
/********************************************************************30**

         End of file
**********************************************************************/
