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
     Name:    Upper Layer Interface - RLC

     Type:    C header file

     Desc:    Structures, variables and typedefs required by 
              RGM interface

     File:    rgm.h

*********************************************************************21*/


#ifndef __RGMH__
#define __RGMH__
#ifdef __cplusplus
extern "C" {
#endif

/** 
  @file rgm.h 
  @brief Defines for RGM interface.
 */


/***********************************************************************
                         Macro Definitions 
 ***********************************************************************/

/* Event corresponding to each primitive at this interface */
#define EVTRGMBASEEVT      0     /*!< Bind Request */
#define EVTRGMBNDREQ       1     /*!< Bind Request */
#define EVTRGMUBNDREQ      2     /*!< Un-Bind Request */
#define EVTRGMBNDCFM       3     /*!< Bind Confirm */
#define EVTRGMCFGPRBRPRT   4     /*!< PRB Report Configuration */
#define EVTRGMPRBRPRTIND   5     /*!< PRB report Indication */
#define EVTRGMTRANSMODEIND 6     /*!< Trans Mode Chg Indication */
#define EVTRGMMAX          7


/* selector(coupling) values */
#define RGM_SEL_LC              0
#define RGM_SEL_LWLC            1
#define RGM_SEL_TC              2

#define RGM_UBNDREQ_MNGMT       1    /*!< TFU sap unbind reason */
/* RRM_SP1_START */
/* Rgm Prb Usage Info */
#define RGM_PRB_USAGE_UL        1    /* Only UL - Mode TDD */ 
#define RGM_PRB_USAGE_DL        2    /* Only DL - Mode TDD */
/* RRM_SP1_END */


#ifdef __cplusplus
}
#endif
#endif /* __RGMH__ */

/********************************************************************30**

         End of file
**********************************************************************/
