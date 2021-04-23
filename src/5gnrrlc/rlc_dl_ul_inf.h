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
  
        Name:    UDX  Control Interface
    
        Type:    C include file
  
        Desc:    This file Contains the Data structures for and prototypes
                  UDX Interface
 
        File:    rlc_dl_ul_inf.h
  
**********************************************************************/

#ifndef __RLC_DL_UL_INF_H__
#define __RLC_DL_UL_INF_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** @file rlc_dl_ul_inf.h
    @brief UDX Interface File (ckw.h)
*/
 
/* UDX Interface Hash Defines */

/* Loose Coupling define */
#define UDX_SEL_LC            0     /*!< Loose Coupling Option */
#define UDX_SEL_LWLC          1
#define RLC_MAX_UDX 3


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

typedef struct rlcNackInfo
{
   uint8_t nackRange;
   uint8_t isSegment;
   uint32_t sn;      /*!< Nack Sn */
   uint16_t soStart; /*!< Segment offset start */
   uint16_t soEnd;   /*!< Segment offset End */
}RlcNackInfo;

typedef struct rlcUdxStaPdu
{
   uint32_t ackSn;
   /* TODO : RLC_MAX_NACK_CNT should set to MAx SR delay 
      It is tradeoff to consider array based instead of dynamic
      as there are chances where we might loose Status information
      on spill over*/
   uint8_t nackCnt;         /* Cnt of Nacks in the array*/
   RlcNackInfo nackInfo[RLC_MAX_NACK_CNT];
}RlcUdxStaPdu;

typedef struct rlcUdxBufLst
{
   CmLListCp rlsTxLst;      /*!< Stores to be released Tx PDUs */
   CmLListCp rlsRetxLst;    /*!< Stores to be released Retx PDUs */
   CmLListCp rlsSduLst;     /*!< Stores to be released SDUs*/
}RlcUdxBufLst;

typedef struct rlcStatusPdu
{
   uint32_t     ackSn;
   RlcNackInfo  nackInfo[RLC_MAX_NACK_CNT];
   uint16_t     nackCount;
   uint16_t     controlBo;
}RlcUdxDlStaPdu;
/* ### Umapathi*/

/**
  * @brief
  * UDX APIs
  */
typedef S16 (*UdxBndCfm) ARGS((Pst* pst, SuId suId,uint8_t  status ));

typedef S16 (*UdxBndReq) ARGS((Pst* pst, SuId suId,SpId  spId ));

typedef S16 (*UdxUbndReq) ARGS((Pst* pst, SuId suId,Reason reason));

typedef S16 (*UdxCfgReq) ARGS((Pst *pst, SpId spId, RlcCfgInfo *cfgInfo));

typedef S16 (*UdxCfgCfm) ARGS((Pst* pst,SuId suId,RlcCfgCfmInfo *cfmInfo));

typedef S16 (*UdxStaUpdReq) ARGS((Pst* pst,
                                  SpId spId,
                                  CmLteRlcId *rlcId,
                                  RlcUdxStaPdu *pStaPdu ));

typedef S16 (*UdxUeIdChgReq) ARGS((Pst *pst, 
                                   SpId spId, 
                                   uint32_t transId, 
                                   CkwUeInfo *ueInfo, 
                                   CkwUeInfo *newUeInfo));

typedef S16 (*UdxUeIdChgCfm) ARGS((Pst *pst, 
                                   SuId suId, 
                                   uint32_t transId, 
                                   CmStatus status));

typedef S16 (*UdxStaUpdCfm) ARGS((Pst* pst,
                                  SuId  suId,
                                  CmLteRlcId *rlcId,
                                  RlcUdxBufLst *pBufLst));

typedef S16 (*UdxStaPduReq) ARGS((Pst* pst,
                                  SpId  spId, 
                                  CmLteRlcId *rlcId,
                                  RlcUdxDlStaPdu *pStaPdu));

typedef S16 (*UdxStaProhTmrStart) ARGS((Pst* pst,
                                         SuId  suId,
                                         CmLteRlcId  *rlcId));

#ifdef LTE_L2_MEAS
typedef S16 (*UdxL2MeasReq) ARGS((Pst* pst,
                                RlcL2MeasReqEvt *measReqEvt));

typedef S16 (*UdxL2MeasSendReq) ARGS((Pst* pst,
                                    uint8_t measType));

typedef S16 (*UdxL2MeasStopReq) ARGS((Pst* pst,
                                    uint8_t measType));
#endif

#ifdef LCUDX
S16 cmPkUdxBndCfm ARGS((Pst* pst, SuId suId,uint8_t  status ));
S16 cmPkUdxBndReq ARGS((Pst* pst, SuId suId,SpId  spId ));
S16 cmPkUdxUbndReq ARGS((Pst* pst, SuId suId,Reason reason));
S16 cmPkUdxCfgReq ARGS((Pst *pst, SpId spId, RlcCfgInfo *cfgInfo));
S16 cmPkUdxCfgCfm ARGS(( Pst* pst,SuId suId,RlcCfgCfmInfo *cfmInfo));
S16 cmPkUdxStaUpdReq ARGS((Pst* pst,SpId spId,CmLteRlcId *rlcId,
                               RlcUdxStaPdu         *pStaPdu ));
S16 cmPkUdxUeIdChgReq ARGS((Pst *pst, SpId spId, uint32_t transId, 
                                  CkwUeInfo *ueInfo, CkwUeInfo *newUeInfo));
S16 cmPkUdxUeIdChgCfm ARGS((Pst *pst, SuId suId, uint32_t transId, 
                                   CmStatus status));


S16 cmPkUdxStaUpdCfm ARGS((Pst* pst,SuId  suId,CmLteRlcId  *rlcId,
                             RlcUdxBufLst  *pBufLst));

S16 cmPkUdxStaPduReq ARGS(( Pst* pst,SpId  spId, CmLteRlcId *rlcId,
                             RlcUdxDlStaPdu  *pStaPdu ));
S16 cmPkUdxStaProhTmrStart ARGS((Pst *pst, 
                                        SpId spId, 
                                        CmLteRlcId *rlcId));

#ifdef LTE_L2_MEAS
S16 cmPkUdxL2MeasReq ARGS((Pst *pst, RlcL2MeasReqEvt *measReqEvt)); 
S16 cmPkUdxL2MeasSendReq ARGS((Pst *pst, uint8_t measType)); 
S16 cmPkUdxL2MeasStopReq ARGS((Pst *pst, uint8_t measType)); 
#endif

S16 cmUnpkUdxBndCfm ARGS((UdxBndCfm func,Pst* pst, Buffer *mBuf ));
S16 cmUnpkUdxBndReq ARGS((UdxBndReq func,Pst* pst, Buffer *mBuf ));
S16 cmUnpkUdxUbndReq ARGS((UdxUbndReq func,Pst* pst, Buffer *mBuf));
S16 cmUnpkUdxCfgReq ARGS((UdxCfgReq func,Pst *pst, Buffer *mBuf));
S16 cmUnpkUdxCfgCfm ARGS((UdxCfgCfm func , Pst* pst,Buffer *mBuf));
S16 cmUnpkUdxStaUpdReq ARGS((UdxStaUpdReq func,Pst* pst,Buffer *mBuf));
S16 cmUnpkUdxUeIdChgReq ARGS((UdxUeIdChgReq func,Pst *pst, Buffer *mBuf)); 
S16 cmUnpkUdxUeIdChgCfm ARGS((UdxUeIdChgCfm func,Pst *pst, Buffer *mBuf)); 

S16 cmUnpkUdxStaUpdCfm ARGS((UdxStaUpdCfm func,Pst* pst,Buffer *mBuf));

S16 cmUnpkUdxStaPduReq ARGS((UdxStaPduReq func, Pst* pst, Buffer *mBuf));

S16 cmUnpkUdxStaProhTmrStart ARGS((UdxStaProhTmrStart func,
                                          Pst* pst,
                                          Buffer *mBuf));

#ifdef LTE_L2_MEAS
S16 cmUnpkUdxL2MeasReq ARGS((UdxL2MeasReq func, Pst* pst, Buffer *mBuf));
S16 cmUnpkUdxL2MeasSendReq ARGS((UdxL2MeasSendReq func, Pst* pst, Buffer *mBuf));
S16 cmUnpkUdxL2MeasStopReq ARGS((UdxL2MeasStopReq func, Pst* pst, Buffer *mBuf));
#endif
#endif

#ifdef KW
S16 rlcUlUdxBndReq ARGS((Pst* pst, SuId suId, SpId spId));

S16 rlcDlUdxBndReq ARGS((Pst* pst, SuId suId, SpId spId));

S16 rlcDlUdxBndCfm ARGS((Pst* pst, SuId suId, uint8_t status));

S16 rlcUlUdxBndCfm ARGS((Pst* pst, SuId suId, uint8_t status));

S16 rlcDlUdxCfgCfm ARGS((Pst* pst, SuId suId, RlcCfgCfmInfo *cfmInfo));

S16 rlcUlUdxCfgCfm ARGS((Pst* pst, SuId suId, RlcCfgCfmInfo *cfmInfo));

S16 rlcDlUdxUeIdChgCfm ARGS((Pst *pst, 
                                   SuId suId, 
                                   uint32_t transId,  
                                   CmStatus status));

S16 rlcUlUdxUeIdChgCfm ARGS((Pst *pst, 
                                   SuId suId, 
                                   uint32_t transId, 
                                   CmStatus status));

S16 rlcUlUdxUbndReq ARGS((Pst* pst, SuId suId, Reason reason));

S16 rlcDlUdxUbndReq ARGS((Pst* pst, SuId suId, Reason reason));

S16 rlcUlUdxUeIdChgReq ARGS((Pst *pst, 
                                   SpId spId, 
                                   uint32_t transId, 
                                   CkwUeInfo *ueInfo, 
                                   CkwUeInfo *newUeInfo));

S16 rlcUlUdxCfgReq ARGS((Pst *pst, SpId spId, RlcCfgInfo *cfgInfo));

S16 rlcDlUdxCfgReq ARGS((Pst *pst, SpId spId, RlcCfgInfo *cfgInfo));

uint8_t rlcUlUdxStaUpdReq ARGS((Pst* pst,
                                  SpId spId,
                                  CmLteRlcId *rlcId,
                                  RlcUdxStaPdu *pStaPdu));

S16 rlcDlUdxStaUpdReq ARGS((Pst* pst,
                                  SpId spId,
                                  CmLteRlcId *rlcId,
                                  RlcUdxStaPdu *pStaPdu));

S16 rlcDlUdxStaUpdCfm ARGS((Pst* pst,
                                  SuId suId,
                                  CmLteRlcId *rlcId,
                                  RlcUdxBufLst *pBufLst));

S16 rlcUlUdxStaUpdCfm ARGS((Pst* pst,
                                  SuId suId,
                                  CmLteRlcId *rlcId,
                                  RlcUdxBufLst *pBufLst));

uint8_t rlcUlUdxStaPduReq ARGS((Pst *pst,
                                  SpId spId,
                                  CmLteRlcId *rlcId,
                                  RlcUdxDlStaPdu *pStaPdu ));

S16 rlcDlUdxStaPduReq ARGS((Pst *pst,
                                  SpId spId,
                                  CmLteRlcId *rlcId,
                                  RlcUdxDlStaPdu *pStaPdu));

S16 rlcDlUdxUeIdChgReq ARGS((Pst *pst, 
                                   SpId spId, 
                                   uint32_t transId, 
                                   CkwUeInfo *ueInfo, 
                                   CkwUeInfo *newUeInfo));

S16 rlcDlUdxCfgReq ARGS((Pst *pst, SpId spId, RlcCfgInfo *cfgInfo));


S16 rlcUlUdxStaUpdCfm ARGS((Pst* pst,
                                  SuId suId,
                                  CmLteRlcId *rlcId,
                                  RlcUdxBufLst *pBufLst));

S16 rlcDlUdxStaPduReq ARGS((Pst *pst,
                                  SpId spId,
                                  CmLteRlcId *rlcId,
                                  RlcUdxDlStaPdu *pStaPdu));

S16 rlcUlUdxStaProhTmrStart ARGS((Pst *pst,
                                        SpId spId,
                                        CmLteRlcId *rlcId ));

uint8_t rlcDlUdxStaProhTmrStart ARGS((Pst *pst,
                                        SpId spId,
                                        CmLteRlcId *rlcId ));

#ifdef LTE_L2_MEAS
S16 rlcDlUdxL2MeasReq ARGS((Pst *pst, RlcL2MeasReqEvt *measReqEvt)); 

S16 rlcDlUdxL2MeasSendReq ARGS((Pst *pst, uint8_t status)); 

S16 rlcDlUdxL2MeasStopReq ARGS((Pst *pst, uint8_t status)); 

S16 rlcUlUdxL2MeasReq ARGS((Pst* pst, RlcL2MeasReqEvt *measReqEvt));

S16 rlcUlUdxL2MeasSendReq ARGS((Pst *pst, uint8_t status));

S16 rlcUlUdxL2MeasStopReq ARGS((Pst* pst, uint8_t measType));
#endif

#endif




#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

  
/********************************************************************30**
         End of file
**********************************************************************/
