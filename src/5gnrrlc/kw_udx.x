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
 
    Name:   LTE RLC layer
 
    Type:   C include file
 
    Desc:   Defines required by LTE MAC
 
    File:   kw_udx.x
 
 
**********************************************************************/
/** @file kw_udx.x
@brief This file contains basic data structures for the Uplink Downlink 
interface.
*/
 
#ifndef __UDX_X__
#define __UDX_X__

#include "ckw.h"
#include "ckw.x"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

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
   uint32_t         ackSn;
   RlcNackInfo  nackInfo[RLC_MAX_NACK_CNT];
   uint16_t         nackCount;
   uint16_t         controlBo;
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
#endif /* __UDX_X__ */


/**********************************************************************
         End of file
**********************************************************************/
