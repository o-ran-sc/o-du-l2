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
  
        Name:    UDX  Control Interface
    
        Type:    C include file
  
        Desc:    This file Contains the Data structures for and prototypes
                  UDX Interface
 
        File:    kw_udx.h
  
*********************************************************************21*/

#ifndef __KW_UDX_H__
#define __KW_UDX_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** @file kw_udx.h
    @brief UDX Interface File (ckw.h)
*/
 
/* UDX Interface Hash Defines */

/* Loose Coupling define */
#define UDX_SEL_LC            0     /*!< Loose Coupling Option */
#define UDX_SEL_LWLC          1
#define KW_MAX_UDX 3


/* CKW Interface defines */
#define UDX_MAX_ENT_CFG       24    /*!< Maximum number of entities to configure */

#define   ERRUDX 0
#define   EUDXXXX 0
/* ckw_h_001.main_3 added support for L2 Measurement */   
#ifdef LTE_L2_MEAS
#define UDX_MAX_QCI           10
#endif
/* CKW Interface configuration type */
#define UDX_CFG_ADD           1     /*!< Add RLC Entity */
#define UDX_CFG_MODIFY        2     /*!< Modify RLC Entity */
#define UDX_CFG_DELETE        3     /*!< Delete RLC entity */
#define UDX_CFG_REESTABLISH   4     /*!< Re-establish RLC entity */
#define UDX_CFG_DELETE_UE     5     /*!< Release RLC entities per UE */
#define UDX_CFG_DELETE_CELL   6     /*!< Release RLC entities per Cell */

/* CKW RLC entity direction configuration */
#define UDX_CFG_DIR_UL        1     /*!< Unlink direction */
#define UDX_CFG_DIR_DL        2     /*!< Downlink direction */
#define UDX_CFG_DIR_BOTH      3     /*!< Both Downlink and Unlink */

/* CKW Configuration confirmations */
#define UDX_CFG_CFM_OK        1     /*!< Configuration confirmation success */
#define UDX_CFG_CFM_NOK       2     /*!< Configuration confirmation failed */

/***********************************************************************
        Defines for  CKW Interface Events
 ***********************************************************************/
#define UDX_EVT_BND_REQ              0x01   /*!< Bind Request */
#define UDX_EVT_BND_CFM              0x02   /*!< Bind Confirm */
#define UDX_EVT_UBND_REQ             0x03   /*!< Unbind Request */
#define UDX_EVT_CFG_REQ              0x04   /*!< Config Request */
#define UDX_EVT_CFG_CFM              0x05   /*!< Config Confirm */
#define UDX_EVT_UEIDCHG_REQ          0x06   /*!< UE Id Change Request */
#define UDX_EVT_UEIDCHG_CFM          0x07   /*!< UE Id Change Confirm */
#define UDX_EVT_STA_UPD_REQ          0x08
#define UDX_EVT_STA_UPD_CFM          0x09
#define UDX_EVT_STA_PDU_REQ          0x0A
#define UDX_EVT_STA_PHBT_TMR_START   0x0B
#define UDX_EVT_DL_CLEANUP_MEM       0x10   /*!< To cleanup memory in DL inst */

/***********************************************************************
        Defines for Measurements 
 ***********************************************************************/
#define UDX_EVT_L2MEAS_REQ           0x11 
#define UDX_EVT_L2MEAS_SEND_REQ      0x12 
#define UDX_EVT_L2MEAS_STOP_REQ      0x13 

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

  
/********************************************************************30**
         End of file
**********************************************************************/
