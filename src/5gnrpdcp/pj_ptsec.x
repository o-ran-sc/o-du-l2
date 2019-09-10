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
  
        Name:    LTE PDCP-PAL Interface file 
    
        Type:    C include file
  
        Desc:    This file contains all the data structures and 
                 prototypes for LTE PDCP.
 
        File:    pj_ptsec.x
  
*********************************************************************21*/
/** @file pj_ptsec.x
@brief PDCP Product Structures, prototypes
*/

#ifndef __PJPTSECPALX__
#define __PJPTSECPALX__
 
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

EXTERN S16 pjUtlChekTxEnqReq ARGS ((PjCb *gCb, PjDlRbCb *pjRbCb, PjTxEnt *txEnt));
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PJPTSECPALX__ */

  
/********************************************************************30**
  
         End of file
**********************************************************************/
