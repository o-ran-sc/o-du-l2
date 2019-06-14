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
  
     Name:    Ring Buffer 
  
     Type:     C include file
  
     Desc:     This file implements the funcitons required to isolate
               freeing of packer buffers from Main stack processing. This will be 
               usefull in a hyper threaded environment where the freeing can be
               done from low priority thread
  
     File:    ss_rbuf.h 
  
*********************************************************************21*/
#ifndef __SS_RBUF_H__
#define __SS_RBUF_H__

#ifdef __cplusplus
extern "C" {
#endif


#define SS_RNG_TX                    1
#define SS_RNG_RX                    0
#ifdef XEON_SPECIFIC_CHANGES
#define SS_RNG_MAX_DLPDCP_TO_DLRLC_DQ_CNT 80 /* Max Buffer Read from Ring Buffer from PDCP to RLC in DL Direction */
#define SS_RNG_MAX_ULMAC_TO_ULRLC_DQ_CNT  200
#define SS_RNG_ULRLC_TO_ULPDCP_SIZE      256
#define SS_RNG_ULPDCP_TO_ULDAM_SIZE      2048
#define SS_RNG_MAX_ULPDCP_TO_ULDAM_DAT_IND_DQ_CNT 512
#define SS_RNG_MAX_ULRLC_TO_ULPDCP_DAT_IND_DQ_CNT 10
#else   
#define SS_RNG_MAX_DLPDCP_TO_DLRLC_DQ_CNT 40 /* Max Buffer Read from Ring Buffer from PDCP to RLC in DL Direction */
#define SS_RNG_MAX_ULMAC_TO_ULRLC_DQ_CNT  25
#endif   

/* ccpu00143253: Changing SS_RNG_DLPDCP_TO_DLRLC_SIZE from 128 to 512, 
  * as the forwarded data packet drop observed during Handover */
#ifdef L2_L3_SPLIT
#define SS_RNG_DLPDCP_TO_DLRLC_SIZE     5120
#else
#define SS_RNG_DLPDCP_TO_DLRLC_SIZE     5120
#endif

/* Ring Size Defines, powers of 2 only  */
//#define SS_RNG_ICPU_TO_DLPDCP_SIZE      128
#define SS_RNG_ICPU_TO_DLPDCP_SIZE      512
#define SS_RNG_ICPU_TO_DLPDCP_NONPRIO_SIZE      20480
//#define SS_RNG_DLPDCP_TO_DLRLC_SIZE     1024
//#define SS_RNG_L2_RT_TO_FREE_MGR_SIZE   512 
//#define SS_RNG_L2_NRT_TO_FREE_MGR_SIZE  640
/* Increasing from 512 to 1024
   Increasing from 1024 to 2048 to handle free after re-est */ 
#define SS_RNG_L2_RT_TO_FREE_MGR_SIZE   2048
#define SS_RNG_L2_NRT_TO_FREE_MGR_SIZE  2048 
#define SS_RNG_L2_DLRLC_TO_FREE_MGR_SIZE  2048 
#define SS_RNG_PRC_L1D_TO_CL_SIZE       32
#define SS_RNG_PRC_FREE_TO_CL_SIZE      256
#define SS_RNG_ICPU_TO_DAM_SIZE         1024
#define SS_RNG_PDCP_TO_CIPH_SIZE        2048
#define SS_RNG_CIPH_TO_PDCP_SIZE        1024
#ifdef XEON_SPECIFIC_CHANGES
#define SS_RNG_ULMAC_TO_ULRLC_SIZE      1024
#else   
#define SS_RNG_ULMAC_TO_ULRLC_SIZE      128
#endif   
#define SS_RNG_ICCRX_TO_DLRLC_SIZE      640
#define SS_RNG_DL_SMSG_REUSE_SIZE       384
#define SS_RNG_DLRLC_TO_DLMAC_SIZE      512
#define SS_RNG_BUF_MAC_HARQ_SIZE        256
#ifndef L2_L3_SPLIT
#ifdef XEON_SPECIFIC_CHANGES
#define SS_FREE_MGR_MAX_FREE            1024
#else   
#define SS_FREE_MGR_MAX_FREE            32
#endif   
#else
#define SS_FREE_MGR_MAX_FREE            96
#endif

#ifdef MAC_FREE_RING_BUF
#define SS_RNG_MAC_FREE_RING_SIZE      8096
#endif
#ifdef RLC_FREE_RING_BUF
#define SS_RNG_RLC_FREE_RING_SIZE      8096
#endif
#ifdef LC_EGTP_THREAD
#define SS_RNG_EGTP_FREE_RING_SIZE      1024
#endif
/* Ring Element Size Defines */
#define SS_RNG_BUF_ELEM           sizeof(SsRngBufElem)

/*Defines for Packet Prio */
#define SS_QCI_BASED_PRIO_UNUSED      0
#define SS_QCI_BASED_PRIO_HIGH        1
#define SS_QCI_BASED_PRIO_LOW         2
#define SS_QCI_BASED_PRIO_OTHER       3

#ifdef XEON_SPECIFIC_CHANGES
#define SS_RNG_MAC_TO_RLC_HARQ_IND_SIZE 512
#define SS_RNG_DLRLC_TO_DLMAC_STA_RSP_SIZE 2048
#define SS_RNG_DLRLC_TO_DLMAC_DAT_REQ_SIZE 256
#define SS_RNG_MAX_DLRLC_TO_DLMAC_STA_RSP_DQ_CNT 320
#define SS_RNG_MAX_DLRLC_TO_DLMAC_DAT_REQ_DQ_CNT 40
#define SS_RNG_MAX_DLMAC_TO_DLRLC_HARQ_STA_DQ_CNT 80  
#else   
#define SS_RNG_MAC_TO_RLC_HARQ_IND_SIZE 64
#define SS_RNG_DLRLC_TO_DLMAC_STA_RSP_SIZE 512
#define SS_RNG_DLRLC_TO_DLMAC_DAT_REQ_SIZE 64
#define SS_RNG_MAX_DLRLC_TO_DLMAC_STA_RSP_DQ_CNT 80
#define SS_RNG_MAX_DLRLC_TO_DLMAC_DAT_REQ_DQ_CNT 10
#define SS_RNG_MAX_DLMAC_TO_DLRLC_HARQ_STA_DQ_CNT 20 
#endif   
#ifdef __cplusplus
}
#endif

#endif
/**************************************************************************
          End of file
**************************************************************************/

