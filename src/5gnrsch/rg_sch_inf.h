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
     Name:    Scheduler interface - RG_SCH_INF

     Type:    C header file

     Desc:    Constants needed for interface

     File:    rg_sch_inf.h

*********************************************************************21*/

#ifndef __GKSCH_H__ 
#define __GKSCH_H__

/* Operation type for Harq Rls Req */

#define RGINF_RLS_HQ_NO_ACTION 0x00
#define RGINF_RLS_HQ_SAVE_TB   0x01
#define RGINF_RLS_HQ_DEL_TB    0x02

/*5g_NR RGU_MAX_LC has been reduced to 10 to 4*/
#define RGINF_MAX_NUM_DED_LC         4
#define RGINF_MAX_TB_PER_UE          2
#define RGINF_MAX_NUM_UE_PER_TTI     1
#define RGINF_MAX_LCG_PER_UE   4
/* RRM_SP1_START */
#define RGINF_MAX_GBR_QCI_REPORTS  4
/* RRM_SP1_END */
#define RGINF_BCH_INFO   (1<<0)
#define RGINF_BCCH_INFO  (1<<1)
#define RGINF_PCCH_INFO  (1<<2) 
#ifdef EMTC_ENABLE
#define RGINF_EMTC_BCCH_INFO  (1<<3)
#define RGINF_EMTC_PCCH_INFO  (1<<4)
#endif
/* Event corresponding to each primitive at this interface */
/* SCH interface events values startes from 1 and max up to 49 because 50
   onwards is used between MAC-MAC interface*/
#define EVTINFCELLREGREQ          1
#define EVTINFSFALLOCREQ          2
#define EVTINFRLSHQREQ            3
#define EVTINFRLSRNTIREQ          4
#define EVTINFDEDBOUPDTREQ        5
#define EVTINFCMNBOUPDTREQ        6
#define EVTINFSFRECPIND           7
/* Added support for SPS*/
#ifdef LTEMAC_SPS
#define EVTINFSPSLCREG               8
#define EVTINFSPSLCDEREG             9
#define EVTINFSPSRELIND              10
#define EVTINFSPSRESET               18
#endif /* LTEMAC_SPS */

#ifdef LTE_L2_MEAS
#define EVTINFL2MEASREQ              11
#define EVTINFL2MEASCFM              12
/*Added for radisys oam*/
#define EVTINFL2MEASSENDREQ          14
#define EVTINFL2MEASSTOPREQ          15
#define EVTINFL2MEASSTOPCFM          16
#endif
/*Fix: Inform UE delete to scheduler*/
#define EVTINFUEDELIND               13

#define EVTINFLCGREG                 17

#ifdef LTE_ADV
#define EVTINFHQENTRESET             19
#endif

#define RGSCHINF_FREE_MSG(_buf)\
{\
   if (NULLP != (_buf)) \
   { \
      ODU_PUT_MSG_BUF((_buf)); \
      _buf = NULLP; \
   } \
}
#endif /* __GKSCH_H__ */

/********************************************************************30**

         End of file
**********************************************************************/
