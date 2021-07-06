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
  
     Desc:     C source code for pack/unpack of CRG interface primitives. 
  
     File:     crg.c 
  
**********************************************************************/

/** @file crg.c
@brief This file contains the packing/unpacking code for the CRG interface 
       primitives.
*/
/* header include files (.h) */
#include "common_def.h"
#include "crg.h"           /* CRG Interface defines */

/* header/extern include files (.x) */
#include "crg.x"           /* CRG Interface includes */

#ifdef LCCRG

/**
* @brief Request from RRC to MAC to bind the interface saps
*
* @details
*
*     Function : cmPkCrgBndReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   SpId  spId
*  @return   S16
*      -# ROK
**/
S16 cmPkCrgBndReq
(
Pst* pst,
SuId suId,
SpId spId
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG001, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG002, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG003, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   pst->event = (Event) EVTCRGBNDREQ;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief Request from RRC to MAC to bind the interface saps
*
* @details
*
*     Function : cmUnpkCrgBndReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   SpId  spId
*  @return   S16
*      -# ROK
**/
S16 cmUnpkCrgBndReq
(
CrgBndReq func,
Pst *pst,
Buffer *mBuf
)
{
   SuId suId;
   SpId spId;
   

   if (SUnpkS16(&suId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG004, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (SUnpkS16(&spId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG005, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, suId, spId));
}


/**
* @brief Confirmation from MAC to RRC for the bind/Unbind 
 * request for the interface saps
*
* @details
*
*     Function : cmPkCrgBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   uint8_t  status
*  @return   S16
*      -# ROK
**/
S16 cmPkCrgBndCfm
(
Pst* pst,
SuId suId,
uint8_t status
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG006, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (oduUnpackUInt8(status, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG007, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG008, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   pst->event = (Event) EVTCRGBNDCFM;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief Confirmation from MAC to RRC for the bind/Unbind 
 * request for the interface saps
*
* @details
*
*     Function : cmUnpkCrgBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   uint8_t  status
*  @return   S16
*      -# ROK
**/
S16 cmUnpkCrgBndCfm
(
CrgBndCfm func,
Pst *pst,
Buffer *mBuf
)
{
   SuId suId;
   uint8_t status;
   

   if (SUnpkS16(&suId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG009, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (oduPackUInt8(&status, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG010, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, suId, status));
}


/**
* @brief Request from RRC to MAC to Unbind the interface saps
*
* @details
*
*     Function : cmPkCrgUbndReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   Reason  reason
*  @return   S16
*      -# ROK
**/
S16 cmPkCrgUbndReq
(
Pst* pst,
SpId spId,
Reason reason
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG011, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (SPkS16(reason, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG012, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG013, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   pst->event = (Event) EVTCRGUBNDREQ;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief Request from RRC to MAC to Unbind the interface saps
*
* @details
*
*     Function : cmUnpkCrgUbndReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   Reason  reason
*  @return   S16
*      -# ROK
**/
S16 cmUnpkCrgUbndReq
(
CrgUbndReq func,
Pst *pst,
Buffer *mBuf
)
{
   SpId spId;
   Reason reason;
   

   if (SUnpkS16(&spId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG014, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (SUnpkS16(&reason, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG015, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, spId, reason));
}


/**
* @brief Configuration Request from RRC to MAC for 
 * configuring Cell/Ue/Lc
*
* @details
*
*     Function : cmPkCrgCfgReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   CrgCfgTransId  transId
*  @param[in]   CrgCfgReqInfo  *  cfgReqInfo
*  @return   S16
*      -# ROK
**/
S16 cmPkCrgCfgReq
(
Pst* pst,
SpId spId,
CrgCfgTransId transId,
CrgCfgReqInfo  * cfgReqInfo
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG016, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgReqInfo, sizeof(CrgCfgReqInfo));
      return RFAILED;
   }
   if (cmPkCrgCfgReqInfo(cfgReqInfo, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG017, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgReqInfo, sizeof(CrgCfgReqInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }
/* crg_c_001.main_5: MOD - Updating ERR code */
   if (cmPkCrgCfgTransId(&transId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG018, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgReqInfo, sizeof(CrgCfgReqInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG019, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgReqInfo, sizeof(CrgCfgReqInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgReqInfo, sizeof(CrgCfgReqInfo)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG020, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   pst->event = (Event) EVTCRGCFGREQ;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief Configuration Request from RRC to MAC for 
 * configuring Cell/Ue/Lc
*
* @details
*
*     Function : cmUnpkCrgCfgReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   CrgCfgTransId  transId
*  @param[in]   CrgCfgReqInfo  *  cfgReqInfo
*  @return   S16
*      -# ROK
**/
S16 cmUnpkCrgCfgReq
(
CrgCfgReq func,
Pst *pst,
Buffer *mBuf
)
{
   SpId spId;
   CrgCfgTransId transId;
   CrgCfgReqInfo *cfgReqInfo;
   

   if (SUnpkS16(&spId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG021, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (cmUnpkCrgCfgTransId(&transId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG022, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if ((SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&cfgReqInfo, sizeof(CrgCfgReqInfo))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG023, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
    
   if (cmUnpkCrgCfgReqInfo(cfgReqInfo, mBuf) != ROK) {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgReqInfo, sizeof(CrgCfgReqInfo));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG024, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, spId, transId, cfgReqInfo));
}


/**
* @brief Configuration Confirm from MAC to RRC
*
* @details
*
*     Function : cmPkCrgCfgCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   CrgCfgTransId  transId
*  @param[in]   uint8_t  status
*  @return   S16
*      -# ROK
**/
S16 cmPkCrgCfgCfm
(
Pst* pst,
SuId suId,
CrgCfgTransId transId,
uint8_t status
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG025, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
/* crg_c_001.main_5 - ADD - Added the packing for status. */
   if (oduUnpackUInt8(status, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG026, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (cmPkCrgCfgTransId(&transId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG027, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG028, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   pst->event = (Event) EVTCRGCFGCFM;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief Configuration Confirm from MAC to RRC
*
* @details
*
*     Function : cmUnpkCrgCfgCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   CrgCfgTransId  transId
*  @param[in]   uint8_t  status
*  @return   S16
*      -# ROK
**/
S16 cmUnpkCrgCfgCfm
(
CrgCfgCfm func,
Pst *pst,
Buffer *mBuf
)
{
   SuId suId;
   CrgCfgTransId transId;
   uint8_t status;
   

   if (SUnpkS16(&suId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG029, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (cmUnpkCrgCfgTransId(&transId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG030, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (oduPackUInt8(&status, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECRG031, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, suId, transId, status));
}


/***********************************************************
*
*     Func : cmPkCrgCfgTransId
*
*
*     Desc : Transaction ID between MAC and RRC
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkCrgCfgTransId
(
CrgCfgTransId *param,
Buffer *mBuf
)
{

   S32 i;

   for (i=CRG_CFG_TRANSID_SIZE-1; i >= 0; i--) {
      CMCHKPK(oduUnpackUInt8, param->trans[i], mBuf);
   }
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkCrgCfgTransId
*
*
*     Desc : Transaction ID between MAC and RRC
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkCrgCfgTransId
(
CrgCfgTransId *param,
Buffer *mBuf
)
{

   S32 i;

   for (i=0; i<CRG_CFG_TRANSID_SIZE; i++) {
      CMCHKUNPK(oduPackUInt8, &param->trans[i], mBuf);
   }
   return ROK;
}



/***********************************************************
*
*     Func : cmPkCrgBwCfg
*
*
*     Desc : Bandwidth configuration per cell
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkCrgBwCfg
(
CrgBwCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt8, param->ulTotalBw, mBuf);
   CMCHKPK(oduUnpackUInt8, param->dlTotalBw, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkCrgBwCfg
*
*
*     Desc : Bandwidth configuration per cell
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkCrgBwCfg
(
CrgBwCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->dlTotalBw, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->ulTotalBw, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkCrgRachCfg
*
*
*     Desc : RACH configuration per cell
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkCrgRachCfg
(
CrgRachCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt8, param->maxMsg3Tx, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkCrgRachCfg
*
*
*     Desc : RACH configuration per cell
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkCrgRachCfg
(
CrgRachCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->maxMsg3Tx, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkCrgCellCfg
*
*
*     Desc : Cell configuration
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkCrgCellCfg
(
CrgCellCfg *param,
Buffer *mBuf
)
{

#ifdef EMTC_ENABLE
   CMCHKPK(oduUnpackUInt8, param->emtcEnable, mBuf);
#endif
#ifdef TENB_MULT_CELL_SUPPRT
   CMCHKPK(SPkS16, param->rguDlSapId, mBuf);
   CMCHKPK(SPkS16, param->rguUlSapId, mBuf);
#endif
   CMCHKPK(cmPkCrgBwCfg, &param->bwCfg, mBuf);
   CMCHKPK(cmPkCrgRachCfg, &param->rachCfg, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkCrgCellCfg
*
*
*     Desc : Cell configuration
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkCrgCellCfg
(
CrgCellCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkCrgRachCfg, &param->rachCfg, mBuf);
   CMCHKUNPK(cmUnpkCrgBwCfg, &param->bwCfg, mBuf);
#ifdef TENB_MULT_CELL_SUPPRT
   CMCHKUNPK(SUnpkS16, &param->rguUlSapId, mBuf);
   CMCHKUNPK(SUnpkS16, &param->rguDlSapId, mBuf);
#endif
#ifdef EMTC_ENABLE
   CMCHKUNPK(oduPackUInt8, &param->emtcEnable, mBuf);
#endif
   return ROK;
}



/***********************************************************
*
*     Func : cmPkCrgUeUlHqCfg
*
*
*     Desc : Uplink HARQ configuration per UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkCrgUeUlHqCfg
(
CrgUeUlHqCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt8, param->maxUlHqTx, mBuf);
   return ROK;
}

#ifdef LTE_ADV
/***********************************************************
*
*     Func : cmPkCrgUeSCellCfg
*
*
*     Desc : Secondary Cell information of the UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
static S16 cmPkCrgUeSCellCfg
(
CrgUeSCellInfo *param,
Buffer *mBuf
)
{
   CMCHKPK(SPkS16, param->rguDlSapId, mBuf);
   CMCHKPK(SPkS16, param->rguUlSapId, mBuf);
   CMCHKPK(oduUnpackUInt8, param->macInst, mBuf);
   CMCHKPK(oduUnpackUInt16, param->sCellId, mBuf);

   return ROK;
}
/***********************************************************
*
*     Func : cmUnpkCrgUeSCellCfg
*
*
*     Desc : Secondary Cell information of the UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
static S16 cmUnpkCrgUeSCellCfg
(
CrgUeSCellInfo *param,
Buffer *mBuf
)
{
   CMCHKUNPK(oduPackUInt16, &param->sCellId, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->macInst, mBuf);
   CMCHKUNPK(SUnpkS16, &param->rguUlSapId, mBuf);
   CMCHKUNPK(SUnpkS16, &param->rguDlSapId, mBuf);
   return ROK;
}
/***********************************************************
*
*     Func : cmPkCrgUeSecCellInfo
*
*
*     Desc : Secondary Cell information of the UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkCrgUeSecCellInfo
(
CrgUeSecCellInfo *param,
Buffer *mBuf
)
{
   S8 idx;
   for(idx = param->numSCells - 1; idx >= 0; idx--)
   {
      CMCHKPK(cmPkCrgUeSCellCfg, &param->ueSCellCfg[(uint8_t)idx], mBuf);
   }

   CMCHKPK(oduUnpackUInt8, param->numSCells, mBuf);
   CMCHKPK(oduUnpackUInt8, param->isSCellCfgPres, mBuf);

   return ROK;
}

/***********************************************************
*
*     Func : cmUnpkCrgUeSecCellInfo
*
*
*     Desc : Secondary Cell information of the UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkCrgUeSecCellInfo
(
CrgUeSecCellInfo *param,
Buffer *mBuf
)
{
   uint8_t idx;

   CMCHKUNPK(oduPackUInt8, &param->isSCellCfgPres, mBuf);
   if(TRUE == param->isSCellCfgPres)
   {
      CMCHKUNPK(oduPackUInt8, &param->numSCells, mBuf);

      for(idx = 0; idx < param->numSCells; idx++)
      {
         CMCHKUNPK(cmUnpkCrgUeSCellCfg, &param->ueSCellCfg[idx],mBuf);
      }
   }

   return ROK;
}

#endif /* LTE_ADV */


/***********************************************************
*
*     Func : cmUnpkCrgUeUlHqCfg
*
*
*     Desc : Uplink HARQ configuration per UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkCrgUeUlHqCfg
(
CrgUeUlHqCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->maxUlHqTx, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkCrgUeCfg
*
*
*     Desc : UE configuration
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkCrgUeCfg
(
CrgUeCfg *param,
Buffer *mBuf
)
{

#ifdef TENB_MULT_CELL_SUPPRT
   CMCHKPK(SPkS16, param->rguDlSapId, mBuf);
   CMCHKPK(SPkS16, param->rguUlSapId, mBuf);
#endif
   CMCHKPK(oduUnpackUInt32, param->txMode.tm, mBuf);
   CMCHKPK(oduUnpackUInt8, param->txMode.pres, mBuf);
   CMCHKPK(cmPkCrgUeUlHqCfg, &param->ueUlHqCfg, mBuf);
   CMCHKPK(cmPkLteRnti, param->crnti, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkCrgUeCfg
*
*
*     Desc : UE configuration
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkCrgUeCfg
(
CrgUeCfg *param,
Buffer *mBuf
)
{
   uint32_t tmpEnum;
   

   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->crnti, mBuf);
   CMCHKUNPK(cmUnpkCrgUeUlHqCfg, &param->ueUlHqCfg, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->txMode.pres, mBuf);
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->txMode.tm = (CrgTxMode) tmpEnum;
#ifdef TENB_MULT_CELL_SUPPRT
   CMCHKUNPK(SUnpkS16, &param->rguUlSapId, mBuf);
   CMCHKUNPK(SUnpkS16, &param->rguDlSapId, mBuf);
#endif
   return ROK;
}



/***********************************************************
*
*     Func : cmPkCrgDlLchCfg
*
*
*     Desc : Logical channel configuration info for downlink logical channels
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkCrgDlLchCfg
(
CrgDlLchCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt8, param->dlTrchType, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkCrgDlLchCfg
*
*
*     Desc : Logical channel configuration info for downlink logical channels
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkCrgDlLchCfg
(
CrgDlLchCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->dlTrchType, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkCrgUlLchCfg
*
*
*     Desc : Logical channel configuration info for uplink logical channels
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkCrgUlLchCfg
(
CrgUlLchCfg *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt8, param->lcgId, mBuf);
   CMCHKPK(oduUnpackUInt8, param->ulTrchType, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkCrgUlLchCfg
*
*
*     Desc : Logical channel configuration info for uplink logical channels
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkCrgUlLchCfg
(
CrgUlLchCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->ulTrchType, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->lcgId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkCrgLchCfg
*
*
*     Desc : Logical channel configuration info for common and dedicated channels
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkCrgLchCfg
(
CrgLchCfg *param,
Buffer *mBuf
)
{

/* crg_c_001.main_5 - ADD - Added the packing for LTE_L2_MEAS. */
#ifdef LTE_L2_MEAS
   CMCHKPK(oduUnpackUInt8, param->qci, mBuf);
#endif
   CMCHKPK(cmPkCrgUlLchCfg, &param->ulInfo, mBuf);
   CMCHKPK(cmPkCrgDlLchCfg, &param->dlInfo, mBuf);
   CMCHKPK(oduUnpackUInt8, param->dir, mBuf);
   CMCHKPK(cmPkLteLcType, param->lcType, mBuf);
   CMCHKPK(cmPkLteLcId, param->lcId, mBuf);
   CMCHKPK(cmPkLteRnti, param->crnti, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkCrgLchCfg
*
*
*     Desc : Logical channel configuration info for common and dedicated channels
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkCrgLchCfg
(
CrgLchCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->crnti, mBuf);
   CMCHKUNPK(cmUnpkLteLcId, &param->lcId, mBuf);
   CMCHKUNPK(cmUnpkLteLcType, &param->lcType, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->dir, mBuf);
   CMCHKUNPK(cmUnpkCrgDlLchCfg, &param->dlInfo, mBuf);
   CMCHKUNPK(cmUnpkCrgUlLchCfg, &param->ulInfo, mBuf);
/* crg_c_001.main_5 - ADD - Added the unpacking for LTE_L2_MEAS. */
#ifdef LTE_L2_MEAS
   CMCHKUNPK(oduPackUInt8, &param->qci, mBuf);
#endif
   return ROK;
}



/***********************************************************
*
*     Func : cmPkCrgCfg
*
*
*     Desc : Basic configuration info for MAC
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkCrgCfg
(
CrgCfg *param,
Buffer *mBuf
)
{


      switch(param->cfgType) {
         case CRG_LCH_CFG:
            CMCHKPK(cmPkCrgLchCfg, &param->u.lchCfg, mBuf);
            break;
         case CRG_UE_CFG:
            CMCHKPK(cmPkCrgUeCfg, &param->u.ueCfg, mBuf);
            break;
         case CRG_CELL_CFG:
            CMCHKPK(cmPkCrgCellCfg, &param->u.cellCfg, mBuf);
            break;
         default :
            return RFAILED;
      }
   CMCHKPK(oduUnpackUInt8, param->cfgType, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkCrgCfg
*
*
*     Desc : Basic configuration info for MAC
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkCrgCfg
(
CrgCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->cfgType, mBuf);
      switch(param->cfgType) {
         case CRG_CELL_CFG:
            CMCHKUNPK(cmUnpkCrgCellCfg, &param->u.cellCfg, mBuf);
            break;
         case CRG_UE_CFG:
            CMCHKUNPK(cmUnpkCrgUeCfg, &param->u.ueCfg, mBuf);
            break;
         case CRG_LCH_CFG:
            CMCHKUNPK(cmUnpkCrgLchCfg, &param->u.lchCfg, mBuf);
            break;
         default :
            return RFAILED;
      }
   return ROK;
}



/***********************************************************
*
*     Func : cmPkCrgCellRecfg
*
*
*     Desc : Cell reconfiguration info
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkCrgCellRecfg
(
CrgCellRecfg *param,
Buffer *mBuf
)
{


   CMCHKPK(cmPkCrgRachCfg, &param->rachRecfg, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkCrgCellRecfg
*
*
*     Desc : Cell reconfiguration info
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkCrgCellRecfg
(
CrgCellRecfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkCrgRachCfg, &param->rachRecfg, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkCrgUeRecfg
*
*
*     Desc : UE reconfiguration info
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkCrgUeRecfg
(
CrgUeRecfg *param,
Buffer *mBuf
)
{

#ifdef LTE_ADV
   if(TRUE == param->crgSCellCfg.isSCellCfgPres)
   {
      CMCHKPK(cmPkCrgUeSecCellInfo, &param->crgSCellCfg, mBuf);
   }
   else
   {
      CMCHKPK(oduUnpackUInt8, param->crgSCellCfg.isSCellCfgPres, mBuf);
   }
#endif /* LTE_ADV */
   CMCHKPK(oduUnpackUInt32, param->txMode.tm, mBuf);
   CMCHKPK(oduUnpackUInt8, param->txMode.pres, mBuf);
   CMCHKPK(cmPkCrgUeUlHqCfg, &param->ueUlHqRecfg, mBuf);
   CMCHKPK(cmPkLteRnti, param->newCrnti, mBuf);
   CMCHKPK(cmPkLteRnti, param->oldCrnti, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkCrgUeRecfg
*
*
*     Desc : UE reconfiguration info
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkCrgUeRecfg
(
CrgUeRecfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->oldCrnti, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->newCrnti, mBuf);
   CMCHKUNPK(cmUnpkCrgUeUlHqCfg, &param->ueUlHqRecfg, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->txMode.pres, mBuf);
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&param->txMode.tm, mBuf);
#ifdef LTE_ADV 
      CMCHKUNPK(cmUnpkCrgUeSecCellInfo, &param->crgSCellCfg, mBuf);
#endif /* LTE_ADV */
   return ROK;
}



/***********************************************************
*
*     Func : cmPkCrgLchRecfg
*
*
*     Desc : Logical channel reconfiguration info for dedicated channels only
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkCrgLchRecfg
(
CrgLchRecfg *param,
Buffer *mBuf
)
{


      CMCHKPK(oduUnpackUInt8, param->ulRecfg.lcgId, mBuf);
   CMCHKPK(cmPkLteLcId, param->lcId, mBuf);
   CMCHKPK(cmPkLteRnti, param->crnti, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkCrgLchRecfg
*
*
*     Desc : Logical channel reconfiguration info for dedicated channels only
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkCrgLchRecfg
(
CrgLchRecfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->crnti, mBuf);
   CMCHKUNPK(cmUnpkLteLcId, &param->lcId, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->ulRecfg.lcgId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkCrgRecfg
*
*
*     Desc : Basic reconfiguration info for MAC
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkCrgRecfg
(
CrgRecfg *param,
Buffer *mBuf
)
{


      switch(param->recfgType) {
         case CRG_LCH_CFG:
            CMCHKPK(cmPkCrgLchRecfg, &param->u.lchRecfg, mBuf);
            break;
         case CRG_UE_CFG:
            CMCHKPK(cmPkCrgUeRecfg, &param->u.ueRecfg, mBuf);
            break;
         case CRG_CELL_CFG:
            CMCHKPK(cmPkCrgCellRecfg, &param->u.cellRecfg, mBuf);
            break;
         default :
            return RFAILED;
      }
   CMCHKPK(oduUnpackUInt8, param->recfgType, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkCrgRecfg
*
*
*     Desc : Basic reconfiguration info for MAC
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkCrgRecfg
(
CrgRecfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->recfgType, mBuf);
      switch(param->recfgType) {
         case CRG_CELL_CFG:
            CMCHKUNPK(cmUnpkCrgCellRecfg, &param->u.cellRecfg, mBuf);
            break;
         case CRG_UE_CFG:
            CMCHKUNPK(cmUnpkCrgUeRecfg, &param->u.ueRecfg, mBuf);
            break;
         case CRG_LCH_CFG:
            CMCHKUNPK(cmUnpkCrgLchRecfg, &param->u.lchRecfg, mBuf);
            break;
         default :
            return RFAILED;
      }
   return ROK;
}



/***********************************************************
*
*     Func : cmPkCrgDel
*
*
*     Desc : Basic Delete info for MAC
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkCrgDel
(
CrgDel *param,
Buffer *mBuf
)
{


      switch(param->delType) {
         case CRG_LCH_CFG:
            CMCHKPK(oduUnpackUInt8, param->u.lchDel.dir, mBuf);
            CMCHKPK(cmPkLteLcId, param->u.lchDel.lcId, mBuf);
            CMCHKPK(cmPkLteRnti, param->u.lchDel.crnti, mBuf);
            CMCHKPK(cmPkLteCellId, param->u.lchDel.cellId, mBuf);
         break;
         case CRG_UE_CFG:
            CMCHKPK(cmPkLteRnti, param->u.ueDel.crnti, mBuf);
            CMCHKPK(cmPkLteCellId, param->u.ueDel.cellId, mBuf);
         break;
         case CRG_CELL_CFG:
            CMCHKPK(cmPkLteCellId, param->u.cellDel.cellId, mBuf);
         break;
         default :
            return RFAILED;
      }
   CMCHKPK(oduUnpackUInt8, param->delType, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkCrgDel
*
*
*     Desc : Basic Delete info for MAC
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkCrgDel
(
CrgDel *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->delType, mBuf);
      switch(param->delType) {
         case CRG_CELL_CFG:
            CMCHKUNPK(cmUnpkLteCellId, &param->u.cellDel.cellId, mBuf);
         break;
         case CRG_UE_CFG:
            CMCHKUNPK(cmUnpkLteCellId, &param->u.ueDel.cellId, mBuf);
            CMCHKUNPK(cmUnpkLteRnti, &param->u.ueDel.crnti, mBuf);
         break;
         case CRG_LCH_CFG:
            CMCHKUNPK(cmUnpkLteCellId, &param->u.lchDel.cellId, mBuf);
            CMCHKUNPK(cmUnpkLteRnti, &param->u.lchDel.crnti, mBuf);
            CMCHKUNPK(cmUnpkLteLcId, &param->u.lchDel.lcId, mBuf);
            CMCHKUNPK(oduPackUInt8, &param->u.lchDel.dir, mBuf);
         break;
         default :
            return RFAILED;
      }
   return ROK;
}



/***********************************************************
*
*     Func : cmPkCrgRst
*
*
*     Desc : UE RESET info for MAC
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkCrgRst
(
CrgRst *param,
Buffer *mBuf
)
{


   CMCHKPK(cmPkLteRnti, param->crnti, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkCrgRst
*
*
*     Desc : UE RESET info for MAC
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkCrgRst
(
CrgRst *param,
Buffer *mBuf
)
{


   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->crnti, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkCrgCfgReqInfo
*
*
*     Desc : Config/Reconfig/Delete info for MAC
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkCrgCfgReqInfo
(
CrgCfgReqInfo *param,
Buffer *mBuf
)
{


      switch(param->action) {
         case CRG_RESET:
            CMCHKPK(cmPkCrgRst, &param->u.rstInfo, mBuf);
            break;
         case CRG_DELETE:
            CMCHKPK(cmPkCrgDel, &param->u.delInfo, mBuf);
            break;
         case CRG_RECONFIG:
            CMCHKPK(cmPkCrgRecfg, &param->u.recfgInfo, mBuf);
            break;
         case CRG_CONFIG:
            CMCHKPK(cmPkCrgCfg, &param->u.cfgInfo, mBuf);
            break;
         default :
            return RFAILED;
      }
   CMCHKPK(oduUnpackUInt8, param->action, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkCrgCfgReqInfo
*
*
*     Desc : Config/Reconfig/Delete info for MAC
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkCrgCfgReqInfo
(
CrgCfgReqInfo *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->action, mBuf);
      switch(param->action) {
         case CRG_CONFIG:
            CMCHKUNPK(cmUnpkCrgCfg, &param->u.cfgInfo, mBuf);
            break;
         case CRG_RECONFIG:
            CMCHKUNPK(cmUnpkCrgRecfg, &param->u.recfgInfo, mBuf);
            break;
         case CRG_DELETE:
            CMCHKUNPK(cmUnpkCrgDel, &param->u.delInfo, mBuf);
            break;
         case CRG_RESET:
            CMCHKUNPK(cmUnpkCrgRst, &param->u.rstInfo, mBuf);
            break;
         default :
            return RFAILED;
      }
   return ROK;
}
#endif

/**********************************************************************
         End of file
**********************************************************************/
