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
   U8 nackRange;
   U8 isSegment;
   U32 sn;      /*!< Nack Sn */
   U16 soStart; /*!< Segment offset start */
   U16 soEnd;   /*!< Segment offset End */
}RlcNackInfo;

typedef struct rlcUdxStaPdu
{
   U32 ackSn;
   /* TODO : RLC_MAX_NACK_CNT should set to MAx SR delay 
      It is tradeoff to consider array based instead of dynamic
      as there are chances where we might loose Status information
      on spill over*/
   U8 nackCnt;         /* Cnt of Nacks in the array*/
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
   U32         ackSn;
   RlcNackInfo  nackInfo[RLC_MAX_NACK_CNT];
   U16         nackCount;
   U16         controlBo;
}RlcUdxDlStaPdu;
/* ### Umapathi*/

/**
  * @brief
  * UDX APIs
  */
typedef S16 (*UdxBndCfm) ARGS((Pst* pst, SuId suId,U8  status ));

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
                                   U32 transId, 
                                   CkwUeInfo *ueInfo, 
                                   CkwUeInfo *newUeInfo));

typedef S16 (*UdxUeIdChgCfm) ARGS((Pst *pst, 
                                   SuId suId, 
                                   U32 transId, 
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
                                rlcL2MeasReqEvt *measReqEvt));

typedef S16 (*UdxL2MeasSendReq) ARGS((Pst* pst,
                                    U8 measType));

typedef S16 (*UdxL2MeasStopReq) ARGS((Pst* pst,
                                    U8 measType));
#endif

#ifdef LCUDX
EXTERN S16 cmPkUdxBndCfm ARGS((Pst* pst, SuId suId,U8  status ));
EXTERN S16 cmPkUdxBndReq ARGS((Pst* pst, SuId suId,SpId  spId ));
EXTERN S16 cmPkUdxUbndReq ARGS((Pst* pst, SuId suId,Reason reason));
EXTERN S16 cmPkUdxCfgReq ARGS((Pst *pst, SpId spId, RlcCfgInfo *cfgInfo));
EXTERN S16 cmPkUdxCfgCfm ARGS(( Pst* pst,SuId suId,RlcCfgCfmInfo *cfmInfo));
EXTERN S16 cmPkUdxStaUpdReq ARGS((Pst* pst,SpId spId,CmLteRlcId *rlcId,
                               RlcUdxStaPdu         *pStaPdu ));
EXTERN S16 cmPkUdxUeIdChgReq ARGS((Pst *pst, SpId spId, U32 transId, 
                                  CkwUeInfo *ueInfo, CkwUeInfo *newUeInfo));
EXTERN S16 cmPkUdxUeIdChgCfm ARGS((Pst *pst, SuId suId, U32 transId, 
                                   CmStatus status));


EXTERN S16 cmPkUdxStaUpdCfm ARGS((Pst* pst,SuId  suId,CmLteRlcId  *rlcId,
                             RlcUdxBufLst  *pBufLst));

EXTERN S16 cmPkUdxStaPduReq ARGS(( Pst* pst,SpId  spId, CmLteRlcId *rlcId,
                             RlcUdxDlStaPdu  *pStaPdu ));
EXTERN S16 cmPkUdxStaProhTmrStart ARGS((Pst *pst, 
                                        SpId spId, 
                                        CmLteRlcId *rlcId));

#ifdef LTE_L2_MEAS
EXTERN S16 cmPkUdxL2MeasReq ARGS((Pst *pst, rlcL2MeasReqEvt *measReqEvt)); 
EXTERN S16 cmPkUdxL2MeasSendReq ARGS((Pst *pst, U8 measType)); 
EXTERN S16 cmPkUdxL2MeasStopReq ARGS((Pst *pst, U8 measType)); 
#endif

EXTERN S16 cmUnpkUdxBndCfm ARGS((UdxBndCfm func,Pst* pst, Buffer *mBuf ));
EXTERN S16 cmUnpkUdxBndReq ARGS((UdxBndReq func,Pst* pst, Buffer *mBuf ));
EXTERN S16 cmUnpkUdxUbndReq ARGS((UdxUbndReq func,Pst* pst, Buffer *mBuf));
EXTERN S16 cmUnpkUdxCfgReq ARGS((UdxCfgReq func,Pst *pst, Buffer *mBuf));
EXTERN S16 cmUnpkUdxCfgCfm ARGS((UdxCfgCfm func , Pst* pst,Buffer *mBuf));
EXTERN S16 cmUnpkUdxStaUpdReq ARGS((UdxStaUpdReq func,Pst* pst,Buffer *mBuf));
EXTERN S16 cmUnpkUdxUeIdChgReq ARGS((UdxUeIdChgReq func,Pst *pst, Buffer *mBuf)); 
EXTERN S16 cmUnpkUdxUeIdChgCfm ARGS((UdxUeIdChgCfm func,Pst *pst, Buffer *mBuf)); 

EXTERN S16 cmUnpkUdxStaUpdCfm ARGS((UdxStaUpdCfm func,Pst* pst,Buffer *mBuf));

EXTERN S16 cmUnpkUdxStaPduReq ARGS((UdxStaPduReq func, Pst* pst, Buffer *mBuf));

EXTERN S16 cmUnpkUdxStaProhTmrStart ARGS((UdxStaProhTmrStart func,
                                          Pst* pst,
                                          Buffer *mBuf));

#ifdef LTE_L2_MEAS
EXTERN S16 cmUnpkUdxL2MeasReq ARGS((UdxL2MeasReq func, Pst* pst, Buffer *mBuf));
EXTERN S16 cmUnpkUdxL2MeasSendReq ARGS((UdxL2MeasSendReq func, Pst* pst, Buffer *mBuf));
EXTERN S16 cmUnpkUdxL2MeasStopReq ARGS((UdxL2MeasStopReq func, Pst* pst, Buffer *mBuf));
#endif
#endif

#ifdef KW
EXTERN S16 rlcUlUdxBndReq ARGS((Pst* pst, SuId suId, SpId spId));

EXTERN S16 rlcDlUdxBndReq ARGS((Pst* pst, SuId suId, SpId spId));

EXTERN S16 rlcDlUdxBndCfm ARGS((Pst* pst, SuId suId, U8 status));

EXTERN S16 rlcUlUdxBndCfm ARGS((Pst* pst, SuId suId, U8 status));

EXTERN S16 rlcDlUdxCfgCfm ARGS((Pst* pst, SuId suId, RlcCfgCfmInfo *cfmInfo));

EXTERN S16 rlcUlUdxCfgCfm ARGS((Pst* pst, SuId suId, RlcCfgCfmInfo *cfmInfo));

EXTERN S16 rlcDlUdxUeIdChgCfm ARGS((Pst *pst, 
                                   SuId suId, 
                                   U32 transId,  
                                   CmStatus status));

EXTERN S16 rlcUlUdxUeIdChgCfm ARGS((Pst *pst, 
                                   SuId suId, 
                                   U32 transId, 
                                   CmStatus status));

EXTERN S16 rlcUlUdxUbndReq ARGS((Pst* pst, SuId suId, Reason reason));

EXTERN S16 rlcDlUdxUbndReq ARGS((Pst* pst, SuId suId, Reason reason));

EXTERN S16 rlcUlUdxUeIdChgReq ARGS((Pst *pst, 
                                   SpId spId, 
                                   U32 transId, 
                                   CkwUeInfo *ueInfo, 
                                   CkwUeInfo *newUeInfo));

EXTERN S16 rlcUlUdxCfgReq ARGS((Pst *pst, SpId spId, RlcCfgInfo *cfgInfo));

EXTERN S16 rlcDlUdxCfgReq ARGS((Pst *pst, SpId spId, RlcCfgInfo *cfgInfo));

EXTERN S16 rlcUlUdxStaUpdReq ARGS((Pst* pst,
                                  SpId spId,
                                  CmLteRlcId *rlcId,
                                  RlcUdxStaPdu *pStaPdu));

EXTERN S16 rlcDlUdxStaUpdReq ARGS((Pst* pst,
                                  SpId spId,
                                  CmLteRlcId *rlcId,
                                  RlcUdxStaPdu *pStaPdu));

EXTERN S16 rlcDlUdxStaUpdCfm ARGS((Pst* pst,
                                  SuId suId,
                                  CmLteRlcId *rlcId,
                                  RlcUdxBufLst *pBufLst));

EXTERN S16 rlcUlUdxStaUpdCfm ARGS((Pst* pst,
                                  SuId suId,
                                  CmLteRlcId *rlcId,
                                  RlcUdxBufLst *pBufLst));

EXTERN S16 rlcUlUdxStaPduReq ARGS((Pst *pst,
                                  SpId spId,
                                  CmLteRlcId *rlcId,
                                  RlcUdxDlStaPdu *pStaPdu ));

EXTERN S16 rlcDlUdxStaPduReq ARGS((Pst *pst,
                                  SpId spId,
                                  CmLteRlcId *rlcId,
                                  RlcUdxDlStaPdu *pStaPdu));

EXTERN S16 rlcDlUdxUeIdChgReq ARGS((Pst *pst, 
                                   SpId spId, 
                                   U32 transId, 
                                   CkwUeInfo *ueInfo, 
                                   CkwUeInfo *newUeInfo));

EXTERN S16 rlcDlUdxCfgReq ARGS((Pst *pst, SpId spId, RlcCfgInfo *cfgInfo));


EXTERN S16 rlcUlUdxStaUpdCfm ARGS((Pst* pst,
                                  SuId suId,
                                  CmLteRlcId *rlcId,
                                  RlcUdxBufLst *pBufLst));

EXTERN S16 rlcDlUdxStaPduReq ARGS((Pst *pst,
                                  SpId spId,
                                  CmLteRlcId *rlcId,
                                  RlcUdxDlStaPdu *pStaPdu));

EXTERN S16 rlcUlUdxStaProhTmrStart ARGS((Pst *pst,
                                        SpId spId,
                                        CmLteRlcId *rlcId ));

EXTERN S16 rlcDlUdxStaProhTmrStart ARGS((Pst *pst,
                                        SpId spId,
                                        CmLteRlcId *rlcId ));

#ifdef LTE_L2_MEAS
EXTERN S16 rlcDlUdxL2MeasReq ARGS((Pst *pst, rlcL2MeasReqEvt *measReqEvt)); 

EXTERN S16 rlcDlUdxL2MeasSendReq ARGS((Pst *pst, U8 status)); 

EXTERN S16 rlcDlUdxL2MeasStopReq ARGS((Pst *pst, U8 status)); 

EXTERN S16 rlcUlUdxL2MeasReq ARGS((Pst* pst, rlcL2MeasReqEvt *measReqEvt));

EXTERN S16 rlcUlUdxL2MeasSendReq ARGS((Pst *pst, U8 status));

EXTERN S16 rlcUlUdxL2MeasStopReq ARGS((Pst* pst, U8 measType));
#endif

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __UDX_X__ */


/**********************************************************************
         End of file
**********************************************************************/
