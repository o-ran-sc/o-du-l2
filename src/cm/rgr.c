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
  
     Desc:     C source code for pack/unpack of RGR interface primitives. 
  
     File:     rgr.c 
 
**********************************************************************/

/** @file rgr.c
@brief This file contains the packing/unpacking code for the RGR interface 
       primitives.
*/
/* LTE_ADV_FLAG_REMOVED_END */
#include "common_def.h"
#include "rgr.h"           /* RGR Interface defines */
/* LTE_ADV_FLAG_REMOVED_END */
#include "rgr.x"           /* RGR Interface includes */

#if defined(LCRGR)


/***********************************************************
*
*     Func : cmPkRgrBndReq
*
*
*     Desc : Request from RRM to MAC to bind the interface saps
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrBndReq
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
         (ErrVal)ERGR001, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR002, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR003, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   pst->event = (Event) EVTRGRBNDREQ;
   return (SPstTsk(pst,mBuf));
}


/***********************************************************
*
*     Func : cmUnpkRgrBndReq
*
*
*     Desc : Request from RRM to MAC to bind the interface saps
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrBndReq
(
RgrBndReq func,
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
         (ErrVal)ERGR004, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (SUnpkS16(&spId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR005, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, suId, spId));
}


/***********************************************************
*
*     Func : cmPkRgrBndCfm
*
*
*     Desc : Confirmation from MAC to RRM for the bind/Unbind 
 * request for the interface saps
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrBndCfm
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
         (ErrVal)ERGR006, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (oduUnpackUInt8(status, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR007, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR008, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   pst->event = (Event) EVTRGRBNDCFM;
   return (SPstTsk(pst,mBuf));
}


/***********************************************************
*
*     Func : cmUnpkRgrBndCfm
*
*
*     Desc : Confirmation from MAC to RRM for the bind/Unbind 
 * request for the interface saps
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrBndCfm
(
RgrBndCfm func,
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
         (ErrVal)ERGR009, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (oduPackUInt8(&status, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR010, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, suId, status));
}


/***********************************************************
*
*     Func : cmPkRgrUbndReq
*
*
*     Desc : Request from RRM to MAC to Unbind the interface saps
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUbndReq
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
         (ErrVal)ERGR011, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (SPkS16(reason, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR012, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR013, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   pst->event = (Event) EVTRGRUBNDREQ;
   return (SPstTsk(pst,mBuf));
}


/***********************************************************
*
*     Func : cmUnpkRgrUbndReq
*
*
*     Desc : Request from RRM to MAC to Unbind the interface saps
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUbndReq
(
RgrUbndReq func,
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
         (ErrVal)ERGR014, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (SUnpkS16(&reason, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR015, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, spId, reason));
}


/***********************************************************
*
*     Func : cmUnpkRgrCfgReq
*
*
*     Desc : Configuration Request from RRM to MAC for 
 * configuring Cell/Ue/Lc
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrCfgReq
(
RgrCfgReq func,
Pst *pst,
Buffer *mBuf
)
{
   RgrCfgTransId transId;
   RgrCfgReqInfo *cfgReqInfo;
   
#if 0
   if (SUnpkS16(&spId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR021, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
#endif

   if (cmUnpkRgrCfgTransId(&transId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR022, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if ((SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&cfgReqInfo,sizeof(RgrCfgReqInfo))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGR023, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   memset(cfgReqInfo, 0, sizeof(RgrCfgReqInfo));
   if (pst->selector == ODU_SELECTOR_LC) 
      if (cmUnpkRgrCfgReqInfo(cfgReqInfo, mBuf) != ROK) {
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgReqInfo, sizeof(RgrCfgReqInfo));
         SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ERGR024, (ErrVal)0, "Packing failed");
#endif
         return RFAILED;
      }
   SPutMsg(mBuf);
   return ((*func)(pst, transId, cfgReqInfo));
}

/* rgr_c_001.main_3: Added TTI indication from MAC to RGR user */

/***********************************************************
*
*     Func : cmPkRgrTtiIndInfo
*
*
*     Desc : Packs the TTI indication information.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrTtiIndInfo
(
RgrTtiIndInfo *param,
Buffer        *mBuf
)
{
   CMCHKPK(oduUnpackUInt16, param->sfn, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}

/***********************************************************
*
*     Func : cmUnpkRgrTtiIndInfo
*
*
*     Desc : Unpacking for RgrTtiIndInfo
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrTtiIndInfo
(
RgrTtiIndInfo *param,
Buffer *mBuf
)
{

   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->sfn, mBuf);
   return ROK;
}


/***********************************************************
*
*     Func : cmPkRgrTtiInd
*
*
*     Desc : TTI indication  from MAC to RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrTtiInd
(
Pst* pst,
SuId suId,
RgrTtiIndInfo *ttiInd
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR025, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (cmPkRgrTtiIndInfo(ttiInd, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR026, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ttiInd, sizeof(RgrTtiIndInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR027, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ttiInd, sizeof(RgrTtiIndInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }
   SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ttiInd, sizeof(RgrTtiIndInfo));
   pst->event = (Event) EVTRGRTTIIND;
   return (SPstTsk(pst,mBuf));
}


/***********************************************************
*
*     Func : cmUnpkRgrTtiInd
*
*
*     Desc : TTI indication from MAC to RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrTtiInd
(
RgrTtiInd func,
Pst *pst,
Buffer *mBuf
)
{
   SuId suId;
   RgrTtiIndInfo *ttiInd;
   

   if (SUnpkS16(&suId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR028, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if ((SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&ttiInd, sizeof(RgrTtiIndInfo))) != ROK) 
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGR029, (ErrVal)0, "UnPacking failed");
#endif      
      SPutMsg(mBuf);
      return RFAILED;
   }
 
   if (cmUnpkRgrTtiIndInfo(ttiInd, mBuf) != ROK) {
      /*ccpu00114888- Memory Leak issue- Start*/
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ttiInd, 
            sizeof(RgrTtiIndInfo));
      /*ccpu00114888- Memory Leak issue- End*/
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR030, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, suId, ttiInd));
}


/***********************************************************
*
*     Func : cmPkRgrCfgCfm
*
*
*     Desc : Configuration Confirm from MAC to RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrCfgCfm
(
Pst* pst,
RgrCfgTransId transId,
uint8_t status
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR031, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (oduUnpackUInt8(status, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR032, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (cmPkRgrCfgTransId(&transId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR033, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   pst->event = (Event) EVTMACSCHCFGCFM;
   return (SPstTsk(pst,mBuf));
}


/***********************************************************
*
*     Func : cmUnpkRgrCfgCfm
*
*
*     Desc : Configuration Confirm from MAC to RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrCfgCfm
(
RgrCfgCfm func,
Pst *pst,
Buffer *mBuf
)
{
   RgrCfgTransId transId;
   uint8_t status;
   

   if (cmUnpkRgrCfgTransId(&transId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR036, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (oduPackUInt8(&status, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR037, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, transId, status));
}

/* rgr_c_001.main_4: ADD-Added for SI Enhancement. */
#ifdef RGR_SI_SCH
/***********************************************************
*
*     Func : cmPkRgrSiCfgCfm
*
*
*     Desc : SI Configuration Confirm from MAC to RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrSiCfgCfm
(
Pst* pst,
SuId suId,
RgrCfgTransId transId,
uint8_t status
)
{
   Buffer *mBuf = NULLP;


   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR038, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (oduUnpackUInt8(status, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR039, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (cmPkRgrCfgTransId(&transId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR040, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR041, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTRGRSICFGCFM;
   return (SPstTsk(pst,mBuf));
}


/***********************************************************
*
*     Func : cmUnpkRgrSiCfgCfm
*
*
*     Desc : Configuration Confirm from MAC to RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrSiCfgCfm
(
RgrSiCfgCfm func,
Pst *pst,
Buffer *mBuf
)
{
   SuId suId;
   RgrCfgTransId transId;
   uint8_t status;
   

   if (SUnpkS16(&suId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR042, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (cmUnpkRgrCfgTransId(&transId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR043, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (oduPackUInt8(&status, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR044, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, suId, transId, status));
}
#endif/*RGR_SI_SCH*/


/***********************************************************
*
*     Func : cmPkRgrCfgTransId
*
*
*     Desc : Transaction ID between MAC and RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrCfgTransId
(
RgrCfgTransId *param,
Buffer *mBuf
)
{

   S32 i;

   for (i=RGR_CFG_TRANSID_SIZE-1; i >= 0; i--) {
      CMCHKPK(oduUnpackUInt8, param->trans[i], mBuf);
   }
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrCfgTransId
*
*
*     Desc : Transaction ID between MAC and RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrCfgTransId
(
RgrCfgTransId *param,
Buffer *mBuf
)
{

   S32 i;

   for (i=0; i<RGR_CFG_TRANSID_SIZE; i++) {
      CMCHKUNPK(oduPackUInt8, &param->trans[i], mBuf);
   }
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrDlHqCfg
*
*
*     Desc :  Downlink HARQ configuration per Cell
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrDlHqCfg
(
RgrDlHqCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt8, param->maxMsg4HqTx, mBuf);
   CMCHKPK(oduUnpackUInt8, param->maxDlHqTx, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrDlHqCfg
*
*
*     Desc :  Downlink HARQ configuration per Cell
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrDlHqCfg
(
RgrDlHqCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->maxDlHqTx, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->maxMsg4HqTx, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrRntiCfg
*
*
*     Desc : Range of RNTIs managed by MAC
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrRntiCfg
(
RgrRntiCfg *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt16, param->size, mBuf);
   CMCHKPK(cmPkLteRnti, param->startRnti, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrRntiCfg
*
*
*     Desc : Range of RNTIs managed by MAC
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrRntiCfg
(
RgrRntiCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(cmUnpkLteRnti, &param->startRnti, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->size, mBuf);
   return ROK;
}

#ifdef EMTC_ENABLE
/***********************************************************
*
*     Func : cmPkRgrEmtcRntiCfg
*
*
*     Desc : Range of Emtc RNTIs managed by MAC
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrEmtcRntiCfg
(
RgrEmtcRntiCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt16, param->size, mBuf);
   CMCHKPK(cmPkLteRnti, param->rntiCeModeBRange, mBuf);
   CMCHKPK(cmPkLteRnti, param->rntiCeModeBStart, mBuf);
   CMCHKPK(cmPkLteRnti, param->rntiCeModeARange, mBuf);
   CMCHKPK(cmPkLteRnti, param->rntiCeModeAStart, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrEmtcRntiCfg
*
*
*     Desc : Range of Emtc RNTIs managed by MAC
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrEmtcRntiCfg
(
RgrEmtcRntiCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(cmUnpkLteRnti, &param->rntiCeModeAStart, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->rntiCeModeARange, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->rntiCeModeBStart, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->rntiCeModeBRange, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->size, mBuf);

   return ROK;
}
#endif


/***********************************************************
*
*     Func : cmPkRgrDlCmnCodeRateCfg
*
*
*     Desc : Downlink common channel code rate configuration per cell
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrDlCmnCodeRateCfg
(
RgrDlCmnCodeRateCfg *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt8, param->ccchCqi, mBuf);
   CMCHKPK(oduUnpackUInt16, param->pdcchCodeRate, mBuf);
   CMCHKPK(oduUnpackUInt16, param->bcchPchRaCodeRate, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrDlCmnCodeRateCfg
*
*
*     Desc : Downlink common channel code rate configuration per cell
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrDlCmnCodeRateCfg
(
RgrDlCmnCodeRateCfg *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt16, &param->bcchPchRaCodeRate, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->pdcchCodeRate, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->ccchCqi, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrCfiCfg
*
*
*     Desc : Control Format Indicator (CFI) configuration per cell
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrCfiCfg
(
RgrCfiCfg *param,
Buffer *mBuf
)
{
   CMCHKPK(oduUnpackUInt8, param->cfi, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrCfiCfg
*
*
*     Desc : Control Format Indicator (CFI) configuration per cell
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrCfiCfg
(
RgrCfiCfg *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt8, &param->cfi, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrPuschSubBandCfg
*
*
*     Desc : PUSCH sub-band configuration per cell
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrPuschSubBandCfg
(
RgrPuschSubBandCfg *param,
Buffer *mBuf
)
{

   S32 i;

   for (i=param->numSubbands-1; i >= 0; i--) {
      CMCHKPK(oduUnpackUInt8, param->dmrs[i], mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->size, mBuf);
   CMCHKPK(oduUnpackUInt8, param->numSubbands, mBuf);
   CMCHKPK(oduUnpackUInt8, param->subbandStart, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrPuschSubBandCfg
*
*
*     Desc : PUSCH sub-band configuration per cell
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrPuschSubBandCfg
(
RgrPuschSubBandCfg *param,
Buffer *mBuf
)
{

   S32 i;
   CMCHKUNPK(oduPackUInt8, &param->subbandStart, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->numSubbands, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->size, mBuf);
   for (i=0; i<param->numSubbands; i++) {
      CMCHKUNPK(oduPackUInt8, &param->dmrs[i], mBuf);
   }
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrUlCmnCodeRateCfg
*
*
*     Desc : Uplink common channel code rate configuration per cell
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUlCmnCodeRateCfg
(
RgrUlCmnCodeRateCfg *param,
Buffer *mBuf
)
{
   CMCHKPK(oduUnpackUInt8, param->ccchCqi, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUlCmnCodeRateCfg
*
*
*     Desc : Uplink common channel code rate configuration per cell
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUlCmnCodeRateCfg
(
RgrUlCmnCodeRateCfg *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt8, &param->ccchCqi, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrUlTrgCqiCfg
*
*
*     Desc : Target Uplink CQI to achieve through group power control configured per cell
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUlTrgCqiCfg
(
RgrUlTrgCqiCfg *param,
Buffer *mBuf
)
{
   CMCHKPK(oduUnpackUInt8, param->trgCqi, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUlTrgCqiCfg
*
*
*     Desc : Target Uplink CQI to achieve through group power control configured per cell
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUlTrgCqiCfg
(
RgrUlTrgCqiCfg *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt8, &param->trgCqi, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrBwCfg
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
S16 cmPkRgrBwCfg
(
RgrBwCfg *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt8, param->ulTotalBw, mBuf);
   CMCHKPK(oduUnpackUInt8, param->dlTotalBw, mBuf);
   return ROK;
}


#ifdef EMTC_ENABLE 
/* EMTC related pack changes start*/

S16 cmPkRgrEmtcSiCfg
(
RgrEmtcSiCfg *param,
Buffer *mBuf
)
{

   S32 i;
   	
   CMCHKPK(oduUnpackUInt8, param->siHoppingEnable, mBuf);
   CMCHKPK(oduUnpackUInt32, param->modPrd, mBuf);
   CMCHKPK(oduUnpackUInt8, param->siWinSizeBr, mBuf);
   CMCHKPK(oduUnpackUInt8, param->sib1Repetition, mBuf);
   CMCHKPK(oduUnpackUInt8, param->siRepetition, mBuf);
   
   CMCHKPK(oduUnpackUInt16, param->startSymbolLc, mBuf);
   //CMCHKPK(oduUnpackUInt8, param->fddDLOrTddSfBitmapLC.sfnPtnChoice, mBuf);
   /*Changes by SIMRAN*/
    if(!(param->fddDLOrTddSfBitmapLC.sfnPtnChoice))
    {
      CMCHKPK(oduUnpackUInt16, param->fddDLOrTddSfBitmapLC.u.ptn10, mBuf);
    }
    else
   {
      //CMCHKPK(SPkU64,param->fddDLOrTddSfBitmapLC.u.ptn40, mBuf);
      for (i=0; i<2; i++) {
         CMCHKPK(oduUnpackUInt32, param->fddDLOrTddSfBitmapLC.u.ptn40[i], mBuf);
      }

   }
   CMCHKPK(oduUnpackUInt8, param->fddDLOrTddSfBitmapLC.sfnPtnChoice, mBuf);
   //CMCHKPK(oduUnpackUInt16, param->fddDlOrTddSfBitmapBR, mBuf);
   
     for (i= (param->numSi-1); i >= 0; i--) {
   		 CMCHKPK(oduUnpackUInt8, param->schdInfo[i].emtcSiNarrowBand, mBuf);
   		 CMCHKPK(oduUnpackUInt16, param->schdInfo[i].emtcSiTbs, mBuf);
     }

     for (i= (param->numSi-1); i >= 0; i--) {
   	  CMCHKPK(oduUnpackUInt32, param->siPeriodicity[i], mBuf);
   	}
   
     CMCHKPK(oduUnpackUInt8, param->numSi, mBuf);  

 return ROK;
}




S16 cmPkRgrEmtcRachCfg
(
RgrEmtcRachCfg *param,
Buffer *mBuf
)
{

   S32 i;
   	
   CMCHKPK(oduUnpackUInt8, param->emtcCeLvlSupported, mBuf);

   for (i=RGR_MAX_CE_LEVEL-1; i >= 0; i--) {
   	  CMCHKPK(oduUnpackUInt8, param->ceLevelInfo[i].emtcRarHopping, mBuf);
	  CMCHKPK(oduUnpackUInt16, param->ceLevelInfo[i].raEmtcWinSize, mBuf);
	  CMCHKPK(oduUnpackUInt16, param->ceLevelInfo[i].raEmtcContResTmr, mBuf);
   	  CMCHKPK(oduUnpackUInt16, param->ceLevelInfo[i].emtcPreambleTransMax, mBuf);
	  CMCHKPK(oduUnpackUInt8, param->ceLevelInfo[i].emtcPreambleMap.firstPreamble, mBuf);
   	  CMCHKPK(oduUnpackUInt8, param->ceLevelInfo[i].emtcPreambleMap.lastPreamble, mBuf);
   	}
   
 return ROK;
}


S16 cmPkRgrEmtcPdschCfg
(
RgrEmtcPdschCfg*param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt16, param->emtcMaxRepCeModeA, mBuf);
   CMCHKPK(oduUnpackUInt16, param->emtcMaxRepCeModeB, mBuf);
   
 return ROK;
}

S16 cmPkRgrEmtcPuschCfg
(
RgrEmtcPuschCfg*param,
Buffer *mBuf
)
{
   CMCHKPK(oduUnpackUInt16, param->emtcMaxRepCeModeA, mBuf);
   CMCHKPK(oduUnpackUInt16, param->emtcMaxRepCeModeB, mBuf);
   CMCHKPK(oduUnpackUInt8,  param->emtcHoppingOffset, mBuf);
   return ROK;
}



S16 cmPkRgrEmtcPucchCfg
(
RgrEmtcPucchCfg *param,
Buffer *mBuf
)
{

   S32 i;
   for (i=RGR_MAX_CE_LEVEL-1; i >= 0; i--) 
   {
   	  CMCHKPK(oduUnpackUInt16, param->emtcN1pucchAnInfoLst[i], mBuf);
   }

 	CMCHKPK(oduUnpackUInt8, param->emtcPucchNumRepCEMsg4Lvl0, mBuf);
 	CMCHKPK(oduUnpackUInt8, param->emtcPucchNumRepCEMsg4Lvl1, mBuf);
 	CMCHKPK(oduUnpackUInt8, param->emtcPucchNumRepCEMsg4Lvl2, mBuf);
 	CMCHKPK(oduUnpackUInt8, param->emtcPucchNumRepCEMsg4Lvl3, mBuf);	
   
 return ROK;
}


S16 cmPkRgrEmtcPrachCeParamCfg
(
RgrEmtcPrachCEParamLst *param,
Buffer *mBuf
)
{
   CMCHKPK(oduUnpackUInt8, param->emtcPrachCfgIdx, mBuf);
   CMCHKPK(oduUnpackUInt8, param->emtcPrachFreqOffset, mBuf);
   CMCHKPK(oduUnpackUInt16,  param->emtcPrachStartSubFrame, mBuf);
   CMCHKPK(oduUnpackUInt8, param->emtcMaxPremAttemptCE, mBuf);
   CMCHKPK(oduUnpackUInt8, param->emtcNumRepPerPreambleAtt, mBuf);
   CMCHKPK(oduUnpackUInt8,  param->emtcNumMpdcchNBtoMonitor, mBuf);
   CMCHKPK(oduUnpackUInt8, param->emtcMpdcchNBtoMonitor[0], mBuf);
   CMCHKPK(oduUnpackUInt8,  param->emtcMpdcchNBtoMonitor[1], mBuf);
   CMCHKPK(oduUnpackUInt16, param->emtcMpdcchNumRep, mBuf);
   CMCHKPK(oduUnpackUInt8,  param->emtcPrachHoppingCfg, mBuf);   
   return ROK;
}



S16 cmPkRgrEmtcPrachCfg
(
RgrEmtcPrachCfg *param,
Buffer *mBuf
)
{

   S32 i;
 	CMCHKPK(oduUnpackUInt8, param->emtcMpdcchStartSFCssRaFdd, mBuf);
 	CMCHKPK(oduUnpackUInt8, param->emtcPrachHopingOffset, mBuf);
 	CMCHKPK(oduUnpackUInt8, param->emtcInitialCElevel, mBuf);
   for (i=RGR_MAX_CE_LEVEL-1; i >= 0; i--) {   	
 	CMCHKPK(cmPkRgrEmtcPrachCeParamCfg, &param->emtcPrachCEparmLst[i], mBuf);
   	}
   
 return ROK;
}




S16 cmPkRgrEmtcCfg
(
RgrEmtcCellCfg *param,
Buffer *mBuf
)
{
   CMCHKPK(oduUnpackUInt16, param->pci, mBuf);
   CMCHKPK(oduUnpackUInt32, param->emtcT300Tmr, mBuf);
   CMCHKPK(oduUnpackUInt32, param->emtcT301Tmr, mBuf);
   CMCHKPK(cmPkRgrEmtcSiCfg, &param->emtcSiCfg, mBuf);
   CMCHKPK(cmPkRgrEmtcRachCfg, &param->emtcRachCfg, mBuf);
   CMCHKPK(cmPkRgrEmtcPdschCfg, &param->emtcPdschCfg, mBuf);
   CMCHKPK(cmPkRgrEmtcPuschCfg, &param->emtcPuschCfg, mBuf);
   CMCHKPK(cmPkRgrEmtcPrachCfg, &param->emtcPrachCfg, mBuf);
   CMCHKPK(cmPkRgrEmtcPucchCfg, &param->emtcPucchCfg, mBuf);
   CMCHKPK(cmPkRgrEmtcRntiCfg, &param->emtcMacRnti, mBuf);
   CMCHKPK(oduUnpackUInt8, param->emtcPdschNbIdx, mBuf);
   CMCHKPK(oduUnpackUInt8, param->emtcMpdcchNbIdx, mBuf);
   CMCHKPK(oduUnpackUInt8, param->emtcPuschNbIdx, mBuf);
   
   return ROK;
}

/* EMTC related pack changes end*/

/* EMTC related unpack changes start*/

S16 cmUnpkRgrEmtcSiCfg
(
RgrEmtcSiCfg *param,
Buffer *mBuf
)
{

     S32 i;
     uint32_t tmpEnum; 
     CMCHKUNPK(oduPackUInt8, &param->numSi, mBuf); 

     for (i=0; i < param->numSi ; i++) 
     {
     	 CMCHKUNPK(oduPackUInt32,(uint32_t*)&tmpEnum,mBuf);
	 param->siPeriodicity[i] = (RgrSiPeriodicity) tmpEnum;
     }   

     for (i=0; i < param->numSi ; i++)
     {
   	 CMCHKUNPK(oduPackUInt16, &param->schdInfo[i].emtcSiTbs, mBuf);
   	 CMCHKUNPK(oduPackUInt8, &param->schdInfo[i].emtcSiNarrowBand, mBuf);
     }
     //CMCHKUNPK(oduPackUInt16, &param->fddDlOrTddSfBitmapBR, mBuf);
     CMCHKUNPK(oduPackUInt8, &param->fddDLOrTddSfBitmapLC.sfnPtnChoice, mBuf);
     /*Changes by SIMRAN*/
       if(!(param->fddDLOrTddSfBitmapLC.sfnPtnChoice))
       {
        CMCHKUNPK(oduPackUInt16, &param->fddDLOrTddSfBitmapLC.u.ptn10, mBuf);
       }
       else
       {
          for (i=1; i>=0; i--) {
            CMCHKUNPK(oduPackUInt32, &param->fddDLOrTddSfBitmapLC.u.ptn40[i], mBuf);
            }
     }

     CMCHKUNPK(oduPackUInt16, &param->startSymbolLc, mBuf);
     CMCHKUNPK(oduPackUInt8, &param->siRepetition, mBuf);
     CMCHKUNPK(oduPackUInt8, &param->sib1Repetition, mBuf);
     CMCHKUNPK(oduPackUInt8, &param->siWinSizeBr, mBuf);
     CMCHKUNPK(oduPackUInt32, (uint32_t*)&tmpEnum, mBuf);
     param->modPrd = (RgrModPeriodicity) tmpEnum;
     CMCHKUNPK(oduPackUInt8, &param->siHoppingEnable, mBuf);

 	  return ROK;
}




S16 cmUnpkRgrEmtcRachCfg
(
RgrEmtcRachCfg *param,
Buffer *mBuf
)
{

   S32 i;
   for (i=0; i < RGR_MAX_CE_LEVEL; i++) {
   	  CMCHKUNPK(oduPackUInt8, &(param->ceLevelInfo[i].emtcPreambleMap.lastPreamble), mBuf);
	  CMCHKUNPK(oduPackUInt8, &(param->ceLevelInfo[i].emtcPreambleMap.firstPreamble), mBuf);
   	  CMCHKUNPK(oduPackUInt16, &(param->ceLevelInfo[i].emtcPreambleTransMax), mBuf);
	  CMCHKUNPK(oduPackUInt16, &(param->ceLevelInfo[i].raEmtcContResTmr), mBuf);
	  CMCHKUNPK(oduPackUInt16, &(param->ceLevelInfo[i].raEmtcWinSize), mBuf);
   	  CMCHKUNPK(oduPackUInt8,&(param->ceLevelInfo[i].emtcRarHopping), mBuf);
   	}
    CMCHKUNPK(oduPackUInt8, &param->emtcCeLvlSupported, mBuf);   
 return ROK;
}


S16 cmUnpkRgrEmtcPdschCfg
(
RgrEmtcPdschCfg*param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt16, &param->emtcMaxRepCeModeB, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->emtcMaxRepCeModeA, mBuf);
 return ROK;
}

S16 cmUnpkRgrEmtcPuschCfg
(
RgrEmtcPuschCfg*param,
Buffer *mBuf
)
{
   CMCHKUNPK(oduPackUInt8,  &param->emtcHoppingOffset, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->emtcMaxRepCeModeB, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->emtcMaxRepCeModeA, mBuf);
   return ROK;
}



S16 cmUnpkRgrEmtcPucchCfg
(
RgrEmtcPucchCfg *param,
Buffer *mBuf
)
{

   S32 i;


 	CMCHKUNPK(oduPackUInt8, &param->emtcPucchNumRepCEMsg4Lvl3, mBuf);
 	CMCHKUNPK(oduPackUInt8, &param->emtcPucchNumRepCEMsg4Lvl2, mBuf);
 	CMCHKUNPK(oduPackUInt8, &param->emtcPucchNumRepCEMsg4Lvl1, mBuf);
 	CMCHKUNPK(oduPackUInt8, &param->emtcPucchNumRepCEMsg4Lvl0, mBuf);	
 
   for (i=0; i <  RGR_MAX_CE_LEVEL ; i++) {
   	  CMCHKUNPK(oduPackUInt16, &param->emtcN1pucchAnInfoLst[i], mBuf);
  	}   
 return ROK;
}


S16 cmUnpkRgrEmtcPrachCeParamCfg
(
RgrEmtcPrachCEParamLst *param,
Buffer *mBuf
)
{
   CMCHKUNPK(oduPackUInt8,  &param->emtcPrachHoppingCfg, mBuf);  
   CMCHKUNPK(oduPackUInt16, &param->emtcMpdcchNumRep, mBuf);
   CMCHKUNPK(oduPackUInt8,  &param->emtcMpdcchNBtoMonitor[1], mBuf);
   CMCHKUNPK(oduPackUInt8, &param->emtcMpdcchNBtoMonitor[0], mBuf);
   CMCHKUNPK(oduPackUInt8,  &param->emtcNumMpdcchNBtoMonitor, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->emtcNumRepPerPreambleAtt, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->emtcMaxPremAttemptCE, mBuf);
   CMCHKUNPK(oduPackUInt16,  &param->emtcPrachStartSubFrame, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->emtcPrachFreqOffset, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->emtcPrachCfgIdx, mBuf); 
   return ROK;
}



S16 cmUnpkRgrEmtcPrachCfg
(
RgrEmtcPrachCfg *param,
Buffer *mBuf
)
{

   S32 i;
   for (i=0; i < RGR_MAX_CE_LEVEL; i++) {   	
 	CMCHKUNPK(cmUnpkRgrEmtcPrachCeParamCfg, &param->emtcPrachCEparmLst[i], mBuf);
   	}
 	CMCHKUNPK(oduPackUInt8, &param->emtcInitialCElevel, mBuf);
 	CMCHKUNPK(oduPackUInt8, &param->emtcPrachHopingOffset, mBuf);
 	CMCHKUNPK(oduPackUInt8, &param->emtcMpdcchStartSFCssRaFdd, mBuf);

 return ROK;
}

S16 cmUnpkRgrEmtcCfg
(
RgrEmtcCellCfg *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt8, &param->emtcPuschNbIdx, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->emtcMpdcchNbIdx, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->emtcPdschNbIdx, mBuf);
   CMCHKUNPK(cmUnpkRgrEmtcRntiCfg, &param->emtcMacRnti, mBuf);
   CMCHKUNPK(cmUnpkRgrEmtcPucchCfg, &param->emtcPucchCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrEmtcPrachCfg, &param->emtcPrachCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrEmtcPuschCfg, &param->emtcPuschCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrEmtcPdschCfg, &param->emtcPdschCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrEmtcRachCfg, &param->emtcRachCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrEmtcSiCfg, &param->emtcSiCfg, mBuf);
   CMCHKUNPK(oduPackUInt32,&param->emtcT301Tmr, mBuf);
   CMCHKUNPK(oduPackUInt32, &param->emtcT300Tmr, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->pci, mBuf);





   return ROK;
}

/* EMTC related unpack changes end*/


#endif


/***********************************************************
*
*     Func : cmUnpkRgrBwCfg
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
S16 cmUnpkRgrBwCfg
(
RgrBwCfg *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt8, &param->dlTotalBw, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->ulTotalBw, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrPhichCfg
*
*
*     Desc : PHICH configuration per cell
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrPhichCfg
(
RgrPhichCfg *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt8, param->isDurExtend, mBuf);
   CMCHKPK(oduUnpackUInt32, param->ngEnum, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrPhichCfg
*
*
*     Desc : PHICH configuration per cell
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrPhichCfg
(
RgrPhichCfg *param,
Buffer *mBuf
)
{
   uint32_t tmpEnum;


   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->ngEnum = (RgrPhichNg) tmpEnum;
   CMCHKUNPK(oduPackUInt8, &param->isDurExtend, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrPucchCfg
*
*
*     Desc : PUCCH configuration per cell
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrPucchCfg
(
RgrPucchCfg *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt8, param->maxPucchRb, mBuf);
   CMCHKPK(oduUnpackUInt8, param->cyclicShift, mBuf);
   CMCHKPK(oduUnpackUInt8, param->deltaShift, mBuf);
   CMCHKPK(oduUnpackUInt16, param->n1PucchAn, mBuf);
   CMCHKPK(oduUnpackUInt8, param->resourceSize, mBuf);

   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrPucchCfg
*
*
*     Desc : PUCCH configuration per cell
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrPucchCfg
(
RgrPucchCfg *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt8, &param->resourceSize, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->n1PucchAn, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->deltaShift, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->cyclicShift, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->maxPucchRb, mBuf);

   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrSrsCfg
*
*
*     Desc : SRS configuration per cell
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrSrsCfg
(
RgrSrsCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt8, param->srsSubFrameCfg, mBuf);
   CMCHKPK(oduUnpackUInt32, param->srsBwEnum, mBuf);
   CMCHKPK(oduUnpackUInt32, param->srsCfgPrdEnum, mBuf);
   CMCHKPK(oduUnpackUInt8, param->isSrsCfgSetup, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrSrsCfg
*
*
*     Desc : SRS configuration per cell
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrSrsCfg
(
RgrSrsCfg *param,
Buffer *mBuf
)
{
   uint32_t tmpEnum;

   CMCHKUNPK(oduPackUInt8, &param->isSrsCfgSetup, mBuf);
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->srsCfgPrdEnum = (RgrSrsCfgPrd) tmpEnum;
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->srsBwEnum = (RgrSrsBwCfg) tmpEnum;
   CMCHKUNPK(oduPackUInt8, &param->srsSubFrameCfg, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrRachCfg
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
S16 cmPkRgrRachCfg
(
RgrRachCfg *param,
Buffer *mBuf
)
{

   S32 i;

   CMCHKPK(oduUnpackUInt8, param->prachResource, mBuf);
   CMCHKPK(oduUnpackUInt16, param->msgSizeGrpA, mBuf);
   CMCHKPK(oduUnpackUInt8, param->sizeRaPreambleGrpA, mBuf);
   CMCHKPK(oduUnpackUInt8, param->numRaPreamble, mBuf);
   CMCHKPK(oduUnpackUInt8, param->maxMsg3Tx, mBuf);
      for (i=param->raOccasion.size-1; i >= 0; i--) {
         CMCHKPK(oduUnpackUInt8, param->raOccasion.subFrameNum[i], mBuf);
      }
      CMCHKPK(oduUnpackUInt32, param->raOccasion.sfnEnum, mBuf);
      CMCHKPK(oduUnpackUInt8, param->raOccasion.size, mBuf);
   CMCHKPK(oduUnpackUInt8, param->raWinSize, mBuf);
   CMCHKPK(oduUnpackUInt8, param->preambleFormat, mBuf);
#ifdef RGR_V1
   /* rgr_c_001.main_5: cccpu00112372: Added contention resolution 
    * timer configuration */
   CMCHKPK(oduUnpackUInt8, param->contResTmr, mBuf);
#endif
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrRachCfg
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
S16 cmUnpkRgrRachCfg
(
RgrRachCfg *param,
Buffer *mBuf
)
{
   uint32_t tmpEnum;

   S32 i;
#ifdef RGR_V1 
   /* rgr_c_001.main_5: cccpu00112372: Added contention resolution 
    * timer configuration */
   CMCHKUNPK(oduPackUInt8, &param->contResTmr, mBuf);
#endif
   CMCHKUNPK(oduPackUInt8, &param->preambleFormat, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->raWinSize, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->raOccasion.size, mBuf);
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->raOccasion.sfnEnum = (RgrRaSfn) tmpEnum;
      for (i=0; i<param->raOccasion.size; i++) {
         CMCHKUNPK(oduPackUInt8, &param->raOccasion.subFrameNum[i], mBuf);
      }
   CMCHKUNPK(oduPackUInt8, &param->maxMsg3Tx, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->numRaPreamble, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->sizeRaPreambleGrpA, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->msgSizeGrpA, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->prachResource, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrSiCfg
*
*
*     Desc : SI Configuration per cell
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrSiCfg
(
RgrSiCfg *param,
Buffer *mBuf
)
{
   /* rgr_c_001.main_4:MOD-Modified for SI Enhancement. */
#ifdef RGR_SI_SCH
   S16 idx;
#endif/*RGR_SI_SCH*/


#ifdef RGR_SI_SCH
   if(param->numSi > sizeof(param->siPeriodicity)/
                       sizeof(param->siPeriodicity[0]))
      param->numSi = sizeof(param->siPeriodicity)/
                       sizeof(param->siPeriodicity[0]);

   for (idx=param->numSi-1; idx >= 0; idx--) {
   /* Enum to be packed/unpacked as uint32_t instead of S32 */
      CMCHKPK(oduUnpackUInt32, param->siPeriodicity[idx], mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->numSi, mBuf);
   /*rgr_c_001.main_9 ccpu00115364 MOD changed modPrd to enum*/
   CMCHKPK(oduUnpackUInt32, (uint32_t)param->modPrd, mBuf);
#endif/*RGR_SI_SCH*/
   CMCHKPK(oduUnpackUInt8, param->retxCnt, mBuf);
   CMCHKPK(oduUnpackUInt8, param->siWinSize, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrSiCfg
*
*
*     Desc : SI Configuration per cell
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrSiCfg
(
RgrSiCfg *param,
Buffer *mBuf
)
{
   uint32_t tmpEnum;
   /* rgr_c_001.main_4:MOD-Modified for SI Enhancement. */
#ifdef RGR_SI_SCH
   S16 idx;
#endif/*RGR_SI_SCH*/


   CMCHKUNPK(oduPackUInt8, &param->siWinSize, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->retxCnt, mBuf);
#ifdef RGR_SI_SCH
   /*rgr_c_001.main_9 ccpu00115364 MOD changed modPrd to enum*/
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->modPrd = (RgrModPeriodicity) tmpEnum;
   CMCHKUNPK(oduPackUInt8, &param->numSi, mBuf);

   if(param->numSi > sizeof(param->siPeriodicity)/
                       sizeof(param->siPeriodicity[0]))
      param->numSi = sizeof(param->siPeriodicity)/
                       sizeof(param->siPeriodicity[0]);

   /* Enum to be packed/unpacked as uint32_t instead of S32 */
   for (idx=0; idx < param->numSi; idx++) {
      CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
      param->siPeriodicity[idx] = (RgrSiPeriodicity) tmpEnum;
   }
#endif/*RGR_SI_SCH*/

   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrTpcRntiCfg
*
*
*     Desc : TPC RNTI Range
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrTpcRntiCfg
(
RgrTpcRntiCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt16, param->size, mBuf);
   CMCHKPK(cmPkLteRnti, param->startTpcRnti, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrTpcRntiCfg
*
*
*     Desc : TPC RNTI Range
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrTpcRntiCfg
(
RgrTpcRntiCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(cmUnpkLteRnti, &param->startTpcRnti, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->size, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrUlPwrCfg
*
*
*     Desc : Cell specific power configuration
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUlPwrCfg
(
RgrUlPwrCfg *param,
Buffer *mBuf
)
{

   CMCHKPK(cmPkRgrTpcRntiCfg, &param->puschPwrFmt3a, mBuf);
   CMCHKPK(cmPkRgrTpcRntiCfg, &param->puschPwrFmt3, mBuf);
   CMCHKPK(cmPkRgrTpcRntiCfg, &param->pucchPwrFmt3a, mBuf);
   CMCHKPK(cmPkRgrTpcRntiCfg, &param->pucchPwrFmt3, mBuf);
   CMCHKPK(SPkS8, param->deltaPreambleMsg3, mBuf);
   CMCHKPK(SPkS8, param->p0NominalPucch, mBuf);
   CMCHKPK(oduUnpackUInt32, param->alpha, mBuf);
   CMCHKPK(SPkS8, param->p0NominalPusch, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUlPwrCfg
*
*
*     Desc : Cell specific power configuration
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUlPwrCfg
(
RgrUlPwrCfg *param,
Buffer *mBuf
)
{
   uint32_t tmpEnum;


   CMCHKUNPK(SUnpkS8, &param->p0NominalPusch, mBuf);
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->alpha = (RgrPwrAlpha) tmpEnum;
   CMCHKUNPK(SUnpkS8, &param->p0NominalPucch, mBuf);
   CMCHKUNPK(SUnpkS8, &param->deltaPreambleMsg3, mBuf);
   CMCHKUNPK(cmUnpkRgrTpcRntiCfg, &param->pucchPwrFmt3, mBuf);
   CMCHKUNPK(cmUnpkRgrTpcRntiCfg, &param->pucchPwrFmt3a, mBuf);
   CMCHKUNPK(cmUnpkRgrTpcRntiCfg, &param->puschPwrFmt3, mBuf);
   CMCHKUNPK(cmUnpkRgrTpcRntiCfg, &param->puschPwrFmt3a, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrPuschCfg
*
*
*     Desc : brief cell specific hopping configuration
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrPuschCfg
(
RgrPuschCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt8, param->hopOffst, mBuf);
   CMCHKPK(oduUnpackUInt8, param->isIntraHop, mBuf);
   CMCHKPK(oduUnpackUInt8, param->numSubBands, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrPuschCfg
*
*
*     Desc : brief cell specific hopping configuration
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrPuschCfg
(
RgrPuschCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->numSubBands, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->isIntraHop, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->hopOffst, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrCodeBookRstCfg
*
*
*     Desc : Number of bits in code book for different transmission modes
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrCodeBookRstCfg
(
RgrCodeBookRstCfg *param,
Buffer *mBuf
)
{

   S32 i;

   for (i=1-1; i >= 0; i--) {
      CMCHKPK(oduUnpackUInt32, param->pmiBitMap[i], mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->pres, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrCodeBookRstCfg
*
*
*     Desc : Number of bits in code book for different transmission modes
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrCodeBookRstCfg
(
RgrCodeBookRstCfg *param,
Buffer *mBuf
)
{

   S32 i;

   CMCHKUNPK(oduPackUInt8, &param->pres, mBuf);
   for (i=0; i<1; i++) {
      CMCHKUNPK(oduPackUInt32, &param->pmiBitMap[i], mBuf);
   }
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrPreambleSetCfg
*
*
*     Desc : Range of PDCCH Order Preamble Set managed by MAC
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrPreambleSetCfg
(
RgrPreambleSetCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt8, param->size, mBuf);
   CMCHKPK(oduUnpackUInt8, param->start, mBuf);
   CMCHKPK(oduUnpackUInt8, param->pres, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrPreambleSetCfg
*
*
*     Desc : Range of PDCCH Order Preamble Set managed by MAC
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrPreambleSetCfg
(
RgrPreambleSetCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->pres, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->start, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->size, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrCmnLchCfg
*
*
*     Desc : Logical channel configuration info for common channels
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrCmnLchCfg
(
RgrCmnLchCfg *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt8, param->ulTrchType, mBuf);
   CMCHKPK(oduUnpackUInt8, param->dlTrchType, mBuf);
   CMCHKPK(oduUnpackUInt8, param->dir, mBuf);
   CMCHKPK(cmPkLteLcType, param->lcType, mBuf);
   CMCHKPK(cmPkLteLcId, param->lcId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrCmnLchCfg
*
*
*     Desc : Logical channel configuration info for common channels
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrCmnLchCfg
(
RgrCmnLchCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(cmUnpkLteLcId, &param->lcId, mBuf);
   CMCHKUNPK(cmUnpkLteLcType, &param->lcType, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->dir, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->dlTrchType, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->ulTrchType, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrDlfsCfg
*
*
*     Desc : RGR configuration for DLFS scheduler
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrDlfsCfg
(
RgrDlfsCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt8, param->thresholdCqi, mBuf);
   CMCHKPK(oduUnpackUInt8, param->isDlFreqSel, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrDlfsCfg
*
*
*     Desc : RGR configuration for DLFS scheduler
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrDlfsCfg
(
RgrDlfsCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->isDlFreqSel, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->thresholdCqi, mBuf);
   return ROK;
}


#ifdef LTE_TDD

#ifdef LTE_TDD


/***********************************************************
*
*     Func : cmPkRgrTddPrachInfo
*
*
*     Desc : PRACH resource information for TDD
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrTddPrachInfo
(
RgrTddPrachInfo *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt8, param->ulStartSfIdx, mBuf);
   CMCHKPK(oduUnpackUInt8, param->halfFrm, mBuf);
   CMCHKPK(oduUnpackUInt32, param->sfn, mBuf);
   CMCHKPK(oduUnpackUInt8, param->freqIdx, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrTddPrachInfo
*
*
*     Desc : PRACH resource information for TDD
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrTddPrachInfo
(
RgrTddPrachInfo *param,
Buffer *mBuf
)
{
   uint32_t tmpEnum;


   CMCHKUNPK(oduPackUInt8, &param->freqIdx, mBuf);
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->sfn = tmpEnum;
   CMCHKUNPK(oduPackUInt8, &param->halfFrm, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->ulStartSfIdx, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrTddPrachRscInfo
*
*
*     Desc : Set of PRACH Information for TDD
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrTddPrachRscInfo
(
RgrTddPrachRscInfo *param,
Buffer *mBuf
)
{

   S32 i;

   for (i=param->numRsc-1; i >= 0; i--) {
      CMCHKPK(cmPkRgrTddPrachInfo, &param->prachInfo[i], mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->numRsc, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrTddPrachRscInfo
*
*
*     Desc : Set of PRACH Information for TDD
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrTddPrachRscInfo
(
RgrTddPrachRscInfo *param,
Buffer *mBuf
)
{

   S32 i;

   CMCHKUNPK(oduPackUInt8, &param->numRsc, mBuf);
   for (i=0; i<param->numRsc; i++) {
      CMCHKUNPK(cmUnpkRgrTddPrachInfo, &param->prachInfo[i], mBuf);
   }
   return ROK;
}


#endif

#endif


/***********************************************************
*
*     Func : cmPkRgrEnbPfs
*
*
*     Desc : PFS Configuration
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrEnbPfs
(
RgrEnbPfs  *param,
Buffer    *mBuf
)
{
   S32 idx;
   for(idx = RGR_MAX_NUM_QCI-1; idx >= 0; idx--)
   {
      CMCHKPK(oduUnpackUInt32, param->qciWgt[idx], mBuf);
   }   
   CMCHKPK(oduUnpackUInt8, param->fairCoeffi, mBuf);
   CMCHKPK(oduUnpackUInt8, param->tptCoeffi, mBuf);

   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrEnbPfs
*
*
*     Desc : PFS Configuration
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrEnbPfs
(
RgrEnbPfs *param,
Buffer   *mBuf
)
{
   S32 idx;

   CMCHKUNPK(oduPackUInt8, &param->tptCoeffi, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->fairCoeffi, mBuf);
   for(idx = 0; idx < RGR_MAX_NUM_QCI; idx++)
   {
      CMCHKUNPK(oduPackUInt32, &param->qciWgt[idx], mBuf);
   }   
   return ROK;
}

/*rgr_c_001.main_7 - Added support for SPS*/

/***********************************************************
*
*     Func : cmPkRgrSpsCellCfg
*
*
*     Desc : DL SPS configuration parameters per UE 
TODO: Check if this is to be added to re-configuration as well
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrSpsCellCfg
(
RgrSpsCellCfg *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt16, param->maxSpsUePerUlSf, mBuf);
   CMCHKPK(oduUnpackUInt16, param->maxSpsUePerDlSf, mBuf);
   CMCHKPK(oduUnpackUInt8, param->maxSpsDlBw, mBuf);

   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrSpsDlCellCfg
*
*
*     Desc : DL SPS configuration parameters per UE 
TODO: Check if this is to be added to re-configuration as well
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrSpsDlCellCfg
(
RgrSpsCellCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->maxSpsDlBw, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->maxSpsUePerDlSf, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->maxSpsUePerUlSf, mBuf);
   
   return ROK;
}

#ifdef RG_5GTF
S16 cmPkRgr5gtfCellCfg
(
Rgr5gtfCellCfg   *param,
Buffer           *mBuf
)
{
   S8 idx = 0;      
   for (idx = MAX_5GTF_SUBFRAME_INFO - 1; idx >= 0 ;--idx)
   {

      CMCHKPK(oduUnpackUInt32, param->dynConfig[(uint8_t)idx], mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->uePerGrp, mBuf);
   CMCHKPK(oduUnpackUInt8, param->ueGrpPerTti, mBuf);
   CMCHKPK(oduUnpackUInt8, param->numUes, mBuf);
   CMCHKPK(oduUnpackUInt8, param->numOfCC, mBuf);
   CMCHKPK(oduUnpackUInt8, param->bwPerCC, mBuf);
   CMCHKPK(oduUnpackUInt8, param->cfi, mBuf);
   return ROK;
}

S16 cmUnPkRgr5gtfCellCfg
(
Rgr5gtfCellCfg   *param,
Buffer           *mBuf
)
{
   S8 idx = 0;      
   
   CMCHKUNPK(oduPackUInt8, &param->cfi, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->bwPerCC, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->numOfCC, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->numUes, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->ueGrpPerTti, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->uePerGrp, mBuf);
   for (idx = 0; idx < MAX_5GTF_SUBFRAME_INFO ; ++idx)
   {

      CMCHKUNPK(oduPackUInt32, &param->dynConfig[(uint8_t)idx], mBuf);
   }
   return ROK;
}
#endif


/*LAA : functions to pack and unpack LAA params*/
/***********************************************************
*
*     Func : cmPkRgrLteUCellCfg
*
*
*     Desc : LAA configuration for the Cell 
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
static S16 cmPkRgrLteUCellCfg
(
RgrLteUCfg *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt8, param->isLaaCell, mBuf);

   return ROK;
}

/***********************************************************
*
*     Func : cmUnpkRgrLteUCellCfg
*
*
*     Desc : LAA configuration for the cell 
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
static S16 cmUnpkRgrLteUCellCfg
(
RgrLteUCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->isLaaCell, mBuf);
   
   return ROK;
}

/* LTE_ADV_FLAG_REMOVED_START */
/***********************************************************
 *
 *     Func : cmPkRgrLteAdvancedUeConfig
 *
 *
 *     Desc : PAcks LteAdvancedUeConfig
 *
 *
 *     Ret  : S16
 *
 *     Notes:
 *
 *     File  : 
 *
 **********************************************************/
S16 cmPkRgrLteAdvancedUeConfig
(
RgrLteAdvancedUeConfig *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt8, param->isUeCellEdge, mBuf);
   CMCHKPK(oduUnpackUInt8, param->isAbsUe, mBuf);
   CMCHKPK(oduUnpackUInt32, param->pres, mBuf);
   return ROK;
}

/***********************************************************
 *
 *     Func : cmUnpkRgrLteAdvancedUeConfig
 *
 *
 *     Desc : unpacks LteAdvancedUeConfig
 *
 *
 *     Ret  : S16
 *
 *     Notes:
 *
 *     File  : 
 *
 **********************************************************/
S16 cmUnpkRgrLteAdvancedUeConfig
(
RgrLteAdvancedUeConfig *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt32, &param->pres, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->isAbsUe, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->isUeCellEdge, mBuf);
   return ROK;
}

/***********************************************************
 *
 *     Func : cmPkRgrAbsConfig
 *
 *
 *     Desc : Packs RgrAbsConfig
 *
 *
 *     Ret  : S16
 *
 *     Notes:
 *
 *     File  : 
 *
 **********************************************************/
S16 cmPkRgrAbsConfig
(
RgrAbsConfig *param,
Buffer *mBuf
)
{
   S8   indx = 0;

   CMCHKPK(oduUnpackUInt32, (uint32_t)param->status, mBuf);
   for (indx = RGR_ABS_PATTERN_LEN-1; indx >= 0; indx--) 
   {
      CMCHKPK(oduUnpackUInt8, param->absPattern[(uint8_t)indx], mBuf);
   }

   CMCHKPK(oduUnpackUInt32, param->absPatternType, mBuf);
   CMCHKPK(oduUnpackUInt32, param->absLoadPeriodicity, mBuf);

   return ROK;

}
/***********************************************************
 *
 *     Func : cmPkRgrSfrConfig
 *
 *
 *     Desc : Packs RgrSfrConfig
 *
 *
 *     Ret  : S16
 *
 *     Notes:
 *
 *     File  : 
 *
 **********************************************************/
S16 cmPkRgrSfrConfig
(
RgrSfrConfig *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt32, (uint32_t)param->status, mBuf);
#ifdef TFU_UPGRADE   
   CMCHKPK(oduUnpackUInt32, param->pwrThreshold.pHigh, mBuf);
   CMCHKPK(oduUnpackUInt32, param->pwrThreshold.pLow, mBuf);
#endif   
   CMCHKPK(oduUnpackUInt8, param->cellEdgeRbRange.endRb, mBuf);
   CMCHKPK(oduUnpackUInt8, param->cellEdgeRbRange.startRb, mBuf);

   return ROK;

}


/***********************************************************
 *
 *     Func : cmPkRgrDsfrConfig
 *
 *
 *     Desc : Packs RgrDsfrConfig
 *
 *
 *     Ret  : S16
 *
 *     Notes:  AIRSPAN_LTE_ADV_DSFR
 *
 *     File  : 
 *
 **********************************************************/
S16 cmPkRgrDsfrConfig
(
RgrDsfrConfig *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt32, (uint32_t)param->status, mBuf);

   return ROK;

}


/***********************************************************
 *
 *     Func : cmUnpkRgrDsfrConfig
 *
 *
 *     Desc : UnPacks RgrDsfrConfig
 *
 *
 *     Ret  : S16
 *
 *     Notes:  AIRSPAN_LTE_ADV_DSFR
 *
 *     File  : 
 *
 **********************************************************/
S16 cmUnpkRgrDsfrConfig
(
RgrDsfrConfig *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt32, (uint32_t *)&param->status, mBuf);
   return ROK;

}


/***********************************************************
 *
 *     Func : cmPkRgrCellLteAdvancedFeatureCfg
 *
 *
 *     Desc : Cell LteAdvancedFeatureCfg
 *
 *
 *     Ret  : S16
 *
 *     Notes:
 *
 *     File  : 
 *
 **********************************************************/

S16 cmPkRgrCellLteAdvancedFeatureCfg
(
RgrLteAdvancedCellConfig *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt32, param->pres, mBuf);
   CMCHKPK(cmPkRgrDsfrConfig, &param->dsfrCfg, mBuf); /*AIRSPAN_LTE_ADV_DSFR*/
   CMCHKPK(cmPkRgrSfrConfig,  &param->sfrCfg, mBuf);
   CMCHKPK(cmPkRgrAbsConfig,  &param->absCfg, mBuf);

   return ROK;

}   


/***********************************************************
 *
 *     Func : cmUnpkRgrAbsConfig
 *
 *
 *     Desc : Unpacks AbsConfig
 *
 *
 *     Ret  : S16
 *
 *     Notes:
 *
 *     File  : 
 *
 **********************************************************/
S16 cmUnpkRgrAbsConfig
(
RgrAbsConfig *param,
Buffer *mBuf
)
{

   S8 indx = 0;

   CMCHKUNPK(oduPackUInt32, &param->absLoadPeriodicity, mBuf);
   CMCHKUNPK(oduPackUInt32, &param->absPatternType, mBuf);
   for (indx = 0; indx <RGR_ABS_PATTERN_LEN; indx++) 
   {
      CMCHKUNPK(oduPackUInt8, &param->absPattern[(uint8_t)indx], mBuf);
   }
   CMCHKUNPK(oduPackUInt32, (uint32_t*)&param->status, mBuf);

   return ROK;

}

/***********************************************************
 *
 *     Func : cmUnpkRgrSfrConfig
 *
 *
 *     Desc : Unpacks SfrConfig
 *
 *
 *     Ret  : S16
 *
 *     Notes:
 *
 *     File  : 
 *
 **********************************************************/
S16 cmUnpkRgrSfrConfig
(
RgrSfrConfig *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->cellEdgeRbRange.startRb, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->cellEdgeRbRange.endRb, mBuf);
#ifdef TFU_UPGRADE   
   CMCHKUNPK(oduPackUInt32, (uint32_t*)&param->pwrThreshold.pLow, mBuf);
   CMCHKUNPK(oduPackUInt32, (uint32_t*)&param->pwrThreshold.pHigh, mBuf);
#endif   
   CMCHKUNPK(oduPackUInt32, (uint32_t*)&param->status, mBuf);

   return ROK;
}

/***********************************************************
 *
 *     Func : cmUnpkRgrCellLteAdvancedFeatureCfg
 *
 *
 *     Desc : unpacks LteAdvancedFeatureCfg per cell
 *
 *
 *     Ret  : S16
 *
 *     Notes:
 *
 *     File  : 
 *
 **********************************************************/
S16 cmUnpkRgrCellLteAdvancedFeatureCfg
(
RgrLteAdvancedCellConfig *param,
Buffer *mBuf
)
{

   CMCHKUNPK(cmUnpkRgrAbsConfig, &param->absCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrSfrConfig, &param->sfrCfg, mBuf);   
   CMCHKUNPK(cmUnpkRgrDsfrConfig, &param->dsfrCfg, mBuf); /*AIRSPAN_LTE_ADV_DSFR*/
   CMCHKUNPK(oduPackUInt32, &param->pres, mBuf);

   return ROK;
}

/* LTE_ADV_FLAG_REMOVED_END */

/***********************************************************
*
*     Func : cmPkMacSchedGnbCfg
*
*     Desc : SCH GNB Configurations 
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkMacSchedGnbCfg
(
MacSchedGnbCfg *param,
Buffer         *mBuf
)
{

   CMCHKPK(oduUnpackUInt8, param->maxDlUePerTti, mBuf);
   CMCHKPK(oduUnpackUInt8, param->maxUlUePerTti, mBuf);
   CMCHKPK(oduUnpackUInt8, param->numCells, mBuf);
   CMCHKPK(oduUnpackUInt8, param->dlSchdType, mBuf);
   CMCHKPK(oduUnpackUInt8, param->ulSchdType, mBuf);
   CMCHKPK(oduUnpackUInt8, param->numTxAntPorts, mBuf);
   return ROK;
} /* cmPkRgrSchedEnbCfg */

/***********************************************************
*
*     Func : cmUnpkMacSchedGnbCfg
*
*
*     Desc : SCH Gnodeb Configuration to SCH
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkMacSchedGnbCfg
(
MacSchedGnbCfg *param,
Buffer         *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->numTxAntPorts, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->ulSchdType, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->dlSchdType, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->numCells, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->maxUlUePerTti, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->maxDlUePerTti, mBuf);
   return ROK;
} /* cmUnpkMacSchedGnbCfg */


/***********************************************************
*
*     Func : cmPkRgrCellCfg
*
*
*     Desc : Cell Configuration at RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrCellCfg
(
RgrCellCfg *param,
Buffer *mBuf
)
{

   S32 i;

#ifdef EMTC_ENABLE 
/* EMTC related changes start*/

     
/* EMTC related changes ends*/
#endif

#ifdef RG_5GTF
   CMCHKPK(cmPkRgr5gtfCellCfg, &param->Cell5gtfCfg, mBuf);
#endif
#ifdef LTE_ADV
   CMCHKPK(oduUnpackUInt8, param->isPucchFormat3Sptd, mBuf);
#endif
/*LAA: Pack LAA params*/
   CMCHKPK(cmPkRgrLteUCellCfg, &param->lteUCfg, mBuf);
   CMCHKPK(oduUnpackUInt32, param->msg4pAVal, mBuf);
   CMCHKPK(oduUnpackUInt8, param->isAutoCfgModeEnb, mBuf);
   CMCHKPK(oduUnpackUInt8, param->isDynCfiEnb, mBuf);
   CMCHKPK(oduUnpackUInt16, param->phichTxPwrOffset, mBuf);
   CMCHKPK(oduUnpackUInt16, param->rarTxPwrOffset, mBuf);
   CMCHKPK(oduUnpackUInt16, param->pcchTxPwrOffset, mBuf);
   CMCHKPK(oduUnpackUInt16, param->bcchTxPwrOffset, mBuf);

   CMCHKPK(oduUnpackUInt16, param->t300TmrVal, mBuf);
/*rgr_c_001.main_7 - Added support for SPS*/
   CMCHKPK(cmPkRgrSpsCellCfg, &param->spsCfg, mBuf);

/* rgr_c_001.main_3: Added TTI indication from MAC to RGR user */
   CMCHKPK(oduUnpackUInt8, param->rrmTtiIndPrd, mBuf);
#ifdef LTE_TDD
   CMCHKPK(cmPkRgrTddPrachRscInfo, &param->prachRscInfo, mBuf);
   CMCHKPK(oduUnpackUInt8, param->spclSfCfgIdx, mBuf);
   CMCHKPK(oduUnpackUInt8, param->ulDlCfgIdx, mBuf);

#endif
   CMCHKPK(cmPkRgrCellCsgParamCfg, &param->csgParamCfg, mBuf);
   for (i=param->numCmnLcs-1; i >= 0; i--) {
      CMCHKPK(cmPkRgrCmnLchCfg, &param->cmnLcCfg[i], mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->numCmnLcs, mBuf);
   CMCHKPK(cmPkRgrPreambleSetCfg, &param->macPreambleSet, mBuf);
   CMCHKPK(cmPkRgrPuschCfg, &param->puschCfg, mBuf);
   CMCHKPK(cmPkRgrUlPwrCfg, &param->pwrCfg, mBuf);
   CMCHKPK(cmPkRgrSiCfg, &param->siCfg, mBuf);
   CMCHKPK(cmPkRgrRachCfg, &param->rachCfg, mBuf);
   CMCHKPK(cmPkRgrSrsCfg, &param->srsCfg, mBuf);
   CMCHKPK(cmPkRgrPucchCfg, &param->pucchCfg, mBuf);
   CMCHKPK(cmPkRgrPhichCfg, &param->phichCfg, mBuf);
   /* LTE_ADV_FLAG_REMOVED_START */
   CMCHKPK(cmPkRgrCellLteAdvancedFeatureCfg, &param->rgrLteAdvCfg, mBuf);
   /* LTE_ADV_FLAG_REMOVED_END */
   CMCHKPK(cmPkRgrBwCfg, &param->bwCfg, mBuf);
   CMCHKPK(cmPkRgrDlfsCfg, &param->dlfsCfg, mBuf);
   CMCHKPK(cmPkRgrUlCmnCodeRateCfg, &param->ulCmnCodeRate, mBuf);
   CMCHKPK(cmPkRgrPuschSubBandCfg, &param->puschSubBand, mBuf);
   CMCHKPK(cmPkRgrDlCmnCodeRateCfg, &param->dlCmnCodeRate, mBuf);
   CMCHKPK(cmPkRgrUlTrgCqiCfg, &param->trgUlCqi, mBuf);
   CMCHKPK(cmPkRgrCfiCfg, &param->cfiCfg, mBuf);
   CMCHKPK(cmPkRgrRntiCfg, &param->macRnti, mBuf);
   CMCHKPK(cmPkRgrDlHqCfg, &param->dlHqCfg, mBuf);
   CMCHKPK(oduUnpackUInt8, param->dlfsSchdType, mBuf);

   CMCHKPK(SPkS8, param->pMax, mBuf);
   CMCHKPK(oduUnpackUInt8, param->cellModSchm, mBuf);
   CMCHKPK(oduUnpackUInt8, param->isCpDlExtend, mBuf);
   CMCHKPK(oduUnpackUInt8, param->isCpUlExtend, mBuf);
   CMCHKPK(oduUnpackUInt8, param->maxUlUeNewTxPerTti, mBuf);
   CMCHKPK(oduUnpackUInt8, param->maxDlUeNewTxPerTti, mBuf);
   CMCHKPK(oduUnpackUInt8, param->maxDlRetxBw, mBuf);
   CMCHKPK(oduUnpackUInt8, param->maxDlBwPerUe, mBuf);
   CMCHKPK(oduUnpackUInt8, param->maxUlBwPerUe, mBuf);
   CMCHKPK(oduUnpackUInt8, param->maxCcchPerDlSf, mBuf);
   CMCHKPK(oduUnpackUInt8, param->maxUePerDlSf, mBuf);
   CMCHKPK(oduUnpackUInt8, param->maxUePerUlSf, mBuf);
#ifdef RGR_V1
   /* rgr_x_001.main_7: [ccpu00112789] Added configuration for maximum number
      of  MSG3s */
   CMCHKPK(oduUnpackUInt8, param->maxMsg3PerUlSf, mBuf);
#endif
   CMCHKPK(oduUnpackUInt8, param->macInst, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);

#ifdef EMTC_ENABLE 
/* EMTC related changes start*/
   CMCHKPK(oduUnpackUInt8, param->emtcEnable, mBuf);
   CMCHKPK(cmPkRgrEmtcCfg, &param->emtcCellCfg, mBuf);
/* EMTC related changes ends*/
#endif

   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrCellCfg
*
*
*     Desc : Cell Configuration at RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrCellCfg
(
RgrCellCfg *param,
Buffer *mBuf
)
{

   S32 i;

#ifdef EMTC_ENABLE   	
/* EMTC related changes start*/
   CMCHKUNPK(cmUnpkRgrEmtcCfg, &param->emtcCellCfg, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->emtcEnable, mBuf);
/* EMTC related changes ends*/
#endif

   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->macInst, mBuf);
#ifdef RGR_V1
   /* rgr_x_001.main_7: [ccpu00112789] Added configuration for maximum number
      of  MSG3s */
   CMCHKUNPK(oduPackUInt8, &param->maxMsg3PerUlSf, mBuf);
#endif
   CMCHKUNPK(oduPackUInt8, &param->maxUePerUlSf, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->maxUePerDlSf, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->maxCcchPerDlSf, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->maxUlBwPerUe, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->maxDlBwPerUe, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->maxDlRetxBw, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->maxDlUeNewTxPerTti, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->maxUlUeNewTxPerTti, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->isCpUlExtend, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->isCpDlExtend, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->cellModSchm, mBuf);
   CMCHKUNPK(SUnpkS8, &param->pMax, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->dlfsSchdType, mBuf);
   CMCHKUNPK(cmUnpkRgrDlHqCfg, &param->dlHqCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrRntiCfg, &param->macRnti, mBuf);
   CMCHKUNPK(cmUnpkRgrCfiCfg, &param->cfiCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrUlTrgCqiCfg, &param->trgUlCqi, mBuf);
   CMCHKUNPK(cmUnpkRgrDlCmnCodeRateCfg, &param->dlCmnCodeRate, mBuf);
   CMCHKUNPK(cmUnpkRgrPuschSubBandCfg, &param->puschSubBand, mBuf);
   CMCHKUNPK(cmUnpkRgrUlCmnCodeRateCfg, &param->ulCmnCodeRate, mBuf);
   CMCHKUNPK(cmUnpkRgrDlfsCfg, &param->dlfsCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrBwCfg, &param->bwCfg, mBuf);
   /* LTE_ADV_FLAG_REMOVED_START */
   CMCHKUNPK(cmUnpkRgrCellLteAdvancedFeatureCfg, &param->rgrLteAdvCfg, mBuf);
   /* LTE_ADV_FLAG_REMOVED_END */
   CMCHKUNPK(cmUnpkRgrPhichCfg, &param->phichCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrPucchCfg, &param->pucchCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrSrsCfg, &param->srsCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrRachCfg, &param->rachCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrSiCfg, &param->siCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrUlPwrCfg, &param->pwrCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrPuschCfg, &param->puschCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrPreambleSetCfg, &param->macPreambleSet, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->numCmnLcs, mBuf);
   for (i=0; i<param->numCmnLcs; i++) {
      CMCHKUNPK(cmUnpkRgrCmnLchCfg, &param->cmnLcCfg[i], mBuf);
   }
   CMCHKUNPK(cmUnpkRgrCellCsgParamCfg, &param->csgParamCfg, mBuf);

#ifdef LTE_TDD
   CMCHKUNPK(oduPackUInt8, &param->ulDlCfgIdx, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->spclSfCfgIdx, mBuf);
   CMCHKUNPK(cmUnpkRgrTddPrachRscInfo, &param->prachRscInfo, mBuf);

#endif
/* rgr_c_001.main_3: Added TTI indication from MAC to RGR user */
   CMCHKUNPK(oduPackUInt8, &param->rrmTtiIndPrd, mBuf);
   /*rgr_c_001.main_7 - Added support for SPS*/
   CMCHKUNPK(cmUnpkRgrSpsDlCellCfg, &param->spsCfg, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->t300TmrVal, mBuf);
   CMCHKPK(oduPackUInt16, &param->bcchTxPwrOffset, mBuf);
   CMCHKPK(oduPackUInt16, &param->pcchTxPwrOffset, mBuf);
   CMCHKPK(oduPackUInt16, &param->rarTxPwrOffset, mBuf);
   CMCHKPK(oduPackUInt16, &param->phichTxPwrOffset, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->isDynCfiEnb, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->isAutoCfgModeEnb, mBuf);
   CMCHKPK(oduPackUInt32, (uint32_t*)&param->msg4pAVal, mBuf);
   /*LAA: Unpack LAA Cell params*/
   CMCHKUNPK(cmUnpkRgrLteUCellCfg, &param->lteUCfg, mBuf);
   #ifdef LTE_ADV
   CMCHKUNPK(oduPackUInt8, &param->isPucchFormat3Sptd, mBuf);
#endif
#ifdef RG_5GTF
   CMCHKUNPK(cmUnPkRgr5gtfCellCfg, &param->Cell5gtfCfg, mBuf);
#endif
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrUeAprdDlCqiCfg
*
*
*     Desc : Downlink Aperiodic CQI reporting related configuration per UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeAprdDlCqiCfg
(
RgrUeAprdDlCqiCfg *param,
Buffer *mBuf
)
{

   /*Pack Aperiodic Trigger List only for Pcell */
#ifdef LTE_ADV
   CMCHKPK(oduUnpackUInt8, param->triggerSet2, mBuf);
   CMCHKPK(oduUnpackUInt8, param->triggerSet1, mBuf);
#endif

   CMCHKPK(oduUnpackUInt32, param->aprdModeEnum, mBuf);
   CMCHKPK(oduUnpackUInt8, param->pres, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUeAprdDlCqiCfg
*
*
*     Desc : Downlink Aperiodic CQI reporting related configuration per UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUeAprdDlCqiCfg
(
RgrUeAprdDlCqiCfg *param,
Buffer *mBuf
)
{
   uint32_t tmpEnum;


   CMCHKUNPK(oduPackUInt8, &param->pres, mBuf);
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->aprdModeEnum = (RgrAprdCqiMode) tmpEnum;

#ifdef LTE_ADV
   CMCHKUNPK(oduPackUInt8, &param->triggerSet1, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->triggerSet2, mBuf);
#endif

   return ROK;
}


/* rgr_c_001.main_8. Added changes of TFU_UPGRADE */
#ifndef TFU_UPGRADE


/***********************************************************
*
*     Func : cmPkRgrUePrdDlCqiCfg
*
*
*     Desc : Downlink Periodic CQI reporting related configuration per UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUePrdDlCqiCfg
(
RgrUePrdDlCqiCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt16, param->cqiPmiCfgIdx, mBuf);
   CMCHKPK(oduUnpackUInt8, param->k, mBuf);
   CMCHKPK(SPkS8, param->cqiOffst, mBuf);
   CMCHKPK(oduUnpackUInt8, param->subframeOffst, mBuf);
   CMCHKPK(oduUnpackUInt32, param->prdicityEnum, mBuf);
   CMCHKPK(oduUnpackUInt32, param->prdModeEnum, mBuf);
   CMCHKPK(oduUnpackUInt8, param->pres, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUePrdDlCqiCfg
*
*
*     Desc : Downlink Periodic CQI reporting related configuration per UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUePrdDlCqiCfg
(
RgrUePrdDlCqiCfg *param,
Buffer *mBuf
)
{
   uint32_t tmpEnum;


   CMCHKUNPK(oduPackUInt8, &param->pres, mBuf);
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->prdModeEnum = tmpEnum;
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->prdicityEnum = tmpEnum;
   CMCHKUNPK(oduPackUInt8, &param->subframeOffst, mBuf);
   CMCHKUNPK(SUnpkS8, &param->cqiOffst, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->k, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->cqiPmiCfgIdx, mBuf);
   return ROK;
}


#endif

/* rgr_c_001.main_8. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
/***********************************************************
*
*     Func : cmPkRgrUeDlPCqiSetup
*
*
*     Desc : Periodic CQI Setup configuration parameters information
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeDlPCqiSetup
(
RgrUeDlPCqiSetup *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt32, param->prdModeEnum, mBuf);
   CMCHKPK(oduUnpackUInt8, param->sANCQI, mBuf);
   /*rgr_c_001.main_9 DEL removed unwanted comments*/
   CMCHKPK(oduUnpackUInt16, param->riCfgIdx, mBuf);
   CMCHKPK(oduUnpackUInt8, param->riEna, mBuf);
   CMCHKPK(oduUnpackUInt8, param->k, mBuf);
   CMCHKPK(oduUnpackUInt8, param->cqiRepType, mBuf);
   CMCHKPK(oduUnpackUInt16, param->cqiPCfgIdx, mBuf);
   CMCHKPK(oduUnpackUInt16, param->cqiPResIdx, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUeDlPCqiSetup
*
*
*     Desc : Periodic CQI Setup configuration parameters information
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUeDlPCqiSetup
(
RgrUeDlPCqiSetup *param,
Buffer *mBuf
)
{
   uint32_t tmpEnum;


   CMCHKUNPK(oduPackUInt16, &param->cqiPResIdx, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->cqiPCfgIdx, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->cqiRepType, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->k, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->riEna, mBuf);
   /*rgr_c_001.main_9 DEL removed unwanted comments*/
   CMCHKUNPK(oduPackUInt16, &param->riCfgIdx, mBuf); 
   CMCHKUNPK(oduPackUInt8, &param->sANCQI, mBuf);
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->prdModeEnum = (RgrPrdCqiMode) tmpEnum;
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrUePrdDlCqiCfg
*
*
*     Desc : Periodic CQI/PMI/RI configuration parameters information
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUePrdDlCqiCfg
(
RgrUePrdDlCqiCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(cmPkRgrUeDlPCqiSetup, &param->cqiSetup, mBuf);
   CMCHKPK(oduUnpackUInt8, param->type, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUePrdDlCqiCfg
*
*
*     Desc : Periodic CQI/PMI/RI configuration parameters information
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUePrdDlCqiCfg
(
RgrUePrdDlCqiCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->type, mBuf);
   CMCHKUNPK(cmUnpkRgrUeDlPCqiSetup, &param->cqiSetup, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrUeUlSrsSetupCfg
*
*
*     Desc : SRS configuration setup parameters information. 
   Reference 36.313 SoundingRS-UL-Config
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeUlSrsSetupCfg
(
RgrUeUlSrsSetupCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt8, param->fDomPosi, mBuf);
   CMCHKPK(oduUnpackUInt8, param->txComb, mBuf);
   CMCHKPK(oduUnpackUInt8, param->sANSrs, mBuf);
   CMCHKPK(oduUnpackUInt8, param->duration, mBuf);
   CMCHKPK(oduUnpackUInt32, param->cycShift, mBuf);
   CMCHKPK(oduUnpackUInt32, param->srsHopBw, mBuf);
   CMCHKPK(oduUnpackUInt32, param->srsBw, mBuf);
   /*rgr_c_001.main_9 DEL removed unwanted comments*/
   CMCHKPK(oduUnpackUInt16, param->srsCfgIdx, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUeUlSrsSetupCfg
*
*
*     Desc : SRS configuration setup parameters information. 
   Reference 36.313 SoundingRS-UL-Config
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUeUlSrsSetupCfg
(
RgrUeUlSrsSetupCfg *param,
Buffer *mBuf
)
{
   uint32_t tmpEnum;


   /*rgr_c_001.main_9 DEL removed unwanted comments*/
   CMCHKUNPK(oduPackUInt16, &param->srsCfgIdx, mBuf); 
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->srsBw = (RgrUlSrsBwInfo) tmpEnum;
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->srsHopBw = (RgrUlSrsHoBwInfo) tmpEnum;
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->cycShift = (RgrUlSrsCycShiftInfo) tmpEnum;
   CMCHKUNPK(oduPackUInt8, &param->duration, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->sANSrs, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->txComb, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->fDomPosi, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrUeSrSetupCfg
*
*
*     Desc : SR Setup configuration parameters information
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeSrSetupCfg
(
RgrUeSrSetupCfg *param,
Buffer *mBuf
)
{


   /* ccpu00131601:DEL - dTMax Packing removed since this param will not 
    * be required by Scheduler*/
   CMCHKPK(oduUnpackUInt8, param->srCfgIdx, mBuf);
   CMCHKPK(oduUnpackUInt16, param->srResIdx, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUeSrSetupCfg
*
*
*     Desc : SR Setup configuration parameters information
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUeSrSetupCfg
(
RgrUeSrSetupCfg *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt16, &param->srResIdx, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->srCfgIdx, mBuf);
   /* ccpu00131601:DEL - dTMax UnPacking removed since this param will not 
    * be required by Scheduler*/
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrUeSrCfg
*
*
*     Desc : SR configuration parameters information
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeSrCfg
(
RgrUeSrCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(cmPkRgrUeSrSetupCfg, &param->srSetup, mBuf);
   CMCHKPK(oduUnpackUInt8, param->type, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUeSrCfg
*
*
*     Desc : SR configuration parameters information
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUeSrCfg
(
RgrUeSrCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->type, mBuf);
   CMCHKUNPK(cmUnpkRgrUeSrSetupCfg, &param->srSetup, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrUeUlSrsCfg
*
*
*     Desc : SRS configuration parameters information.  
  Reference 36.313 SoundingRS-UL-Config
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeUlSrsCfg
(
RgrUeUlSrsCfg *param,
Buffer *mBuf
)
{


   /*rgr_c_001.main_9 DEL removed unwanted comments*/
   CMCHKPK(cmPkRgrUeUlSrsSetupCfg, &param->srsSetup, mBuf);  
   CMCHKPK(oduUnpackUInt8, param->type, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUeUlSrsCfg
*
*
*     Desc : SRS configuration parameters information.  
  Reference 36.313 SoundingRS-UL-Config
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUeUlSrsCfg
(
RgrUeUlSrsCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->type, mBuf);
   /*rgr_c_001.main_9 DEL removed unwanted comments*/
   CMCHKUNPK(cmUnpkRgrUeUlSrsSetupCfg, &param->srsSetup, mBuf);  
   return ROK;
}


#endif /*TFU_UPGRADE */


/***********************************************************
*
*     Func : cmPkRgrUeDlCqiCfg
*
*
*     Desc : Downlink CQI reporting related configuration per UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeDlCqiCfg
(
RgrUeDlCqiCfg *param,
Buffer *mBuf
)
{



/* rgr_c_001.main_8. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
   CMCHKPK(cmPkRgrUePrdDlCqiCfg, &param->prdCqiCfg, mBuf);
#endif

/* rgr_c_001.main_8. Added changes of TFU_UPGRADE */
#ifndef TFU_UPGRADE
   CMCHKPK(cmPkRgrUePrdDlCqiCfg, &param->prdCqiCfg, mBuf);
#endif
   CMCHKPK(cmPkRgrUeAprdDlCqiCfg, &param->aprdCqiCfg, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUeDlCqiCfg
*
*
*     Desc : Downlink CQI reporting related configuration per UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUeDlCqiCfg
(
RgrUeDlCqiCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(cmUnpkRgrUeAprdDlCqiCfg, &param->aprdCqiCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrUePrdDlCqiCfg, &param->prdCqiCfg, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrUeMeasGapCfg
*
*
*     Desc : Measurement gap configuration for UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeMeasGapCfg
(
RgrUeMeasGapCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt8, param->gapOffst, mBuf);
   CMCHKPK(oduUnpackUInt8, param->gapPrd, mBuf);
   CMCHKPK(oduUnpackUInt8, param->isMesGapEnabled, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUeMeasGapCfg
*
*
*     Desc : Measurement gap configuration for UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUeMeasGapCfg
(
RgrUeMeasGapCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->isMesGapEnabled, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->gapPrd, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->gapOffst, mBuf);
   return ROK;
}

/*rgr_c_001.main_9 ADD DRX functionality under flag*/

/***********************************************************
*
*     Func : cmPkRgrDrxLongCycleOffst
*
*
*     Desc : DRX Long Cycle Offset
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrDrxLongCycleOffst
(
RgrDrxLongCycleOffst *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt16, param->drxStartOffst, mBuf);
   CMCHKPK(oduUnpackUInt16, param->longDrxCycle, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrDrxLongCycleOffst
*
*
*     Desc : DRX Long Cycle Offset
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrDrxLongCycleOffst
(
RgrDrxLongCycleOffst *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt16, &param->longDrxCycle, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->drxStartOffst, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrDrxShortDrx
*
*
*     Desc : DRX Short Cycle Offset
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrDrxShortDrx
(
RgrDrxShortDrx *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt8, param->drxShortCycleTmr, mBuf);
   CMCHKPK(oduUnpackUInt16, param->shortDrxCycle, mBuf);
   CMCHKPK(oduUnpackUInt8, param->pres, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrDrxShortDrx
*
*
*     Desc : DRX Short Cycle Offset
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrDrxShortDrx
(
RgrDrxShortDrx *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->pres, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->shortDrxCycle, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->drxShortCycleTmr, mBuf);
   return ROK;
}

/***********************************************************
*
*     Func : cmPkRgrUeDrxCfg
*
*
*     Desc : DRX configuration for UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeDrxCfg
(
RgrUeDrxCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(cmPkRgrDrxShortDrx, &param->drxShortDrx, mBuf);
   CMCHKPK(cmPkRgrDrxLongCycleOffst, &param->drxLongCycleOffst, mBuf);
   CMCHKPK(oduUnpackUInt16, param->drxRetxTmr, mBuf);
#ifdef EMTC_ENABLE
   CMCHKPK(oduUnpackUInt8,  param->drxRetxTmrR13Pres, mBuf);
   CMCHKPK(oduUnpackUInt8,  param->drxOnDurTmrR13Pres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->emtcDrxUlRetxTmr, mBuf);
   CMCHKPK(oduUnpackUInt8,  param->isEmtcUe, mBuf); 
#endif
   CMCHKPK(oduUnpackUInt16, param->drxInactvTmr, mBuf);
   CMCHKPK(oduUnpackUInt16, param->drxOnDurTmr, mBuf);
/*rgr_c_001.main_9 ADD added changes for R9*/
#ifdef LTEMAC_R9
   CMCHKPK(cmPkTknS32, &param->cqiMask, mBuf);
#endif
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUeDrxCfg
*
*
*     Desc : DRX configuration for UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUeDrxCfg
(
RgrUeDrxCfg *param,
Buffer *mBuf
)
{


/*rgr_c_001.main_9 ADD added changes for R9*/
#ifdef LTEMAC_R9
   CMCHKUNPK(cmUnpkTknS32, &param->cqiMask, mBuf);
#endif
   CMCHKUNPK(oduPackUInt16, &param->drxOnDurTmr, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->drxInactvTmr, mBuf);
#ifdef EMTC_ENABLE
   CMCHKUNPK(oduPackUInt8, &param->isEmtcUe, mBuf); 
   CMCHKUNPK(oduPackUInt16,&param->emtcDrxUlRetxTmr, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->drxOnDurTmrR13Pres, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->drxRetxTmrR13Pres, mBuf);
#endif
   CMCHKUNPK(oduPackUInt16, &param->drxRetxTmr, mBuf);
   CMCHKUNPK(cmUnpkRgrDrxLongCycleOffst, &param->drxLongCycleOffst, mBuf);
   CMCHKUNPK(cmUnpkRgrDrxShortDrx, &param->drxShortDrx, mBuf);
   return ROK;
}

/*rgr_c_001.main_9 ADD added DRX changes under DRX*/

/***********************************************************
*
*     Func : cmPkRgrUeCapCfg
*
*
*     Desc : RgrUeCapCfg
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeCapCfg
(
RgrUeCapCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt8, param->txAntSel, mBuf);
   CMCHKPK(oduUnpackUInt8, param->simCqiAckNack, mBuf);
   CMCHKPK(oduUnpackUInt8, param->resAloocType1, mBuf);
   CMCHKPK(oduUnpackUInt8, param->intraSfFeqHop, mBuf);
   CMCHKPK(oduUnpackUInt8, param->pwrClass, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUeCapCfg
*
*
*     Desc : RgrUeCapCfg
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUeCapCfg
(
RgrUeCapCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->pwrClass, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->intraSfFeqHop, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->resAloocType1, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->simCqiAckNack, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->txAntSel, mBuf);
   
   return ROK;
}


/***********************************************************
*
*     Func : cmPkRgrUeAckNackRepCfg
*
*
*     Desc : rgrUeAckNackRepCfg
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeAckNackRepCfg
(
RgrUeAckNackRepCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt32, param->ackNackRepFactor, mBuf);
   CMCHKPK(oduUnpackUInt16, param->pucchAckNackRep, mBuf);
   CMCHKPK(oduUnpackUInt8, param->isAckNackEnabled, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUeAckNackRepCfg
*
*
*     Desc : rgrUeAckNackRepCfg
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUeAckNackRepCfg
(
RgrUeAckNackRepCfg *param,
Buffer *mBuf
)
{
   uint32_t tmpEnum;


   CMCHKUNPK(oduPackUInt8, &param->isAckNackEnabled, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->pucchAckNackRep, mBuf);
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->ackNackRepFactor = (RgrAckNackRepFactor) tmpEnum;
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrUeTxModeCfg
*
*
*     Desc : Transmission mode configuration per UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeTxModeCfg
(
RgrUeTxModeCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt32, param->txModeEnum, mBuf);
   CMCHKPK(oduUnpackUInt32, param->tmTrnstnState, mBuf);
   CMCHKPK(oduUnpackUInt8, param->pres, mBuf);
   
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUeTxModeCfg
*
*
*     Desc : Transmission mode configuration per UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUeTxModeCfg
(
RgrUeTxModeCfg *param,
Buffer *mBuf
)
{
   uint32_t tmpEnum;


   CMCHKUNPK(oduPackUInt8, &param->pres, mBuf);
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->tmTrnstnState = (RgrTxModeTrnstn) tmpEnum;

   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->txModeEnum = (RgrTxMode) tmpEnum;
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrUeUlHqCfg
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
S16 cmPkRgrUeUlHqCfg
(
RgrUeUlHqCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt8, param->deltaHqOffst, mBuf);
   CMCHKPK(oduUnpackUInt8, param->maxUlHqTx, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUeUlHqCfg
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
S16 cmUnpkRgrUeUlHqCfg
(
RgrUeUlHqCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->maxUlHqTx, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->deltaHqOffst, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrUeGrpPwrCfg
*
*
*     Desc : Group power configuration per UE for PUCCH and PUSCH group power control
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeGrpPwrCfg
(
RgrUeGrpPwrCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt8, param->idx, mBuf);
   CMCHKPK(cmPkLteRnti, param->tpcRnti, mBuf);
   CMCHKPK(oduUnpackUInt8, param->pres, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUeGrpPwrCfg
*
*
*     Desc : Group power configuration per UE for PUCCH and PUSCH group power control
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUeGrpPwrCfg
(
RgrUeGrpPwrCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->pres, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->tpcRnti, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->idx, mBuf);
   return ROK;
}

/*rgr_c_001.main_7 - Added support for SPS*/


/***********************************************************
*
*     Func : cmPkRgrUeSpsDlCfg
*
*
*     Desc : DL SPS configuration parameters per UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeSpsDlCfg
(
RgrUeSpsDlCfg *param,
Buffer *mBuf
)
{

   S32 i;
   CMCHKPK(oduUnpackUInt16, param->explicitRelCnt, mBuf);
   CMCHKPK(oduUnpackUInt32, param->dlSpsPrdctyEnum, mBuf);
   for (i=param->numPucchVal-1; i >= 0; i--) {
      CMCHKPK(oduUnpackUInt32, param->n1PucchVal[i], mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->numPucchVal, mBuf);
   CMCHKPK(oduUnpackUInt8, param->numSpsHqProc, mBuf);
   CMCHKPK(oduUnpackUInt8, param->isDlSpsEnabled, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUeSpsDlCfg
*
*
*     Desc : DL SPS configuration parameters per UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUeSpsDlCfg
(
RgrUeSpsDlCfg *param,
Buffer *mBuf
)
{
   uint32_t tmpEnum;

   S32 i;

   CMCHKUNPK(oduPackUInt8, &param->isDlSpsEnabled, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->numSpsHqProc, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->numPucchVal, mBuf);
   for (i=0; i<param->numPucchVal; i++) {
      CMCHKUNPK(oduPackUInt32, &param->n1PucchVal[i], mBuf);
   }
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->dlSpsPrdctyEnum = tmpEnum;
   CMCHKUNPK(oduPackUInt16, &param->explicitRelCnt, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrUeSpsUlCfg
*
*
*     Desc : UL SPS configuration parameters per UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeSpsUlCfg
(
RgrUeSpsUlCfg *param,
Buffer *mBuf
)
{

   S32 i;

   CMCHKPK(oduUnpackUInt8, param->isLcSRMaskEnab, mBuf);
   for (i=param->lcCnt-1; i >= 0; i--) {
      /* SPS Changes starts */
      CMCHKPK(oduUnpackUInt8, param->spsLcInfo[i].isSpsEnabled, mBuf);
      CMCHKPK(oduUnpackUInt8, param->spsLcInfo[i].lcId, mBuf);
      /* SPS Changes ends */
   }
   CMCHKPK(oduUnpackUInt8, param->lcCnt, mBuf);
   CMCHKPK(oduUnpackUInt32, param->ulSpsPrdctyEnum, mBuf);
      CMCHKPK(SPkS8, param->pwrCfg.p0UePuschVal, mBuf);
      CMCHKPK(SPkS8, param->pwrCfg.p0NominalPuschVal, mBuf);
   CMCHKPK(oduUnpackUInt8, param->pwrCfgPres, mBuf);

#ifdef LTE_TDD
   CMCHKPK(oduUnpackUInt8, param->twoIntervalCfg, mBuf);

#endif
   CMCHKPK(oduUnpackUInt32, param->implicitRelCnt, mBuf);
   CMCHKPK(oduUnpackUInt8, param->isUlSpsEnabled, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUeSpsUlCfg
*
*
*     Desc : UL SPS configuration parameters per UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUeSpsUlCfg
(
RgrUeSpsUlCfg *param,
Buffer *mBuf
)
{
   uint32_t tmpEnum;

   S32 i;

   CMCHKUNPK(oduPackUInt8, &param->isUlSpsEnabled, mBuf);
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->implicitRelCnt = tmpEnum;

#ifdef LTE_TDD
   CMCHKUNPK(oduPackUInt8, &param->twoIntervalCfg, mBuf);

#endif
   CMCHKUNPK(oduPackUInt8, &param->pwrCfgPres, mBuf);
      CMCHKUNPK(SUnpkS8, &param->pwrCfg.p0NominalPuschVal, mBuf);
      CMCHKUNPK(SUnpkS8, &param->pwrCfg.p0UePuschVal, mBuf);
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->ulSpsPrdctyEnum = tmpEnum;
   CMCHKUNPK(oduPackUInt8, &param->lcCnt, mBuf); 
   for (i=0; i<param->lcCnt; i++) {
      CMCHKUNPK(oduPackUInt8, &param->spsLcInfo[i].lcId, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->spsLcInfo[i].isSpsEnabled, mBuf);
   }
   CMCHKUNPK(oduPackUInt8, &param->isLcSRMaskEnab, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrUeSpsCfg
*
*
*     Desc : SPS configuration parameters per UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeSpsCfg
(
RgrUeSpsCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(cmPkRgrUeSpsUlCfg, &param->ulSpsCfg, mBuf);
   CMCHKPK(cmPkRgrUeSpsDlCfg, &param->dlSpsCfg, mBuf);
   CMCHKPK(cmPkLteRnti, param->spsRnti, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUeSpsCfg
*
*
*     Desc : SPS configuration parameters per UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUeSpsCfg
(
RgrUeSpsCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(cmUnpkLteRnti, &param->spsRnti, mBuf);
   CMCHKUNPK(cmUnpkRgrUeSpsDlCfg, &param->dlSpsCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrUeSpsUlCfg, &param->ulSpsCfg, mBuf);
   return ROK;
}


/***********************************************************
*
*     Func : cmPkRgrUeUlPwrCfg
*
*
*     Desc : Uplink power configuration per UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeUlPwrCfg
(
RgrUeUlPwrCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt8, param->trgCqi, mBuf);
   CMCHKPK(oduUnpackUInt8, param->pSRSOffset, mBuf);
   CMCHKPK(SPkS8, param->p0UePucch, mBuf);
   CMCHKPK(SPkS8, param->p0UePusch, mBuf);
   CMCHKPK(oduUnpackUInt8, param->isDeltaMCSEnabled, mBuf);
   CMCHKPK(oduUnpackUInt8, param->isAccumulated, mBuf);
   CMCHKPK(cmPkRgrUeGrpPwrCfg, &param->uePucchPwr, mBuf);
   CMCHKPK(cmPkRgrUeGrpPwrCfg, &param->uePuschPwr, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUeUlPwrCfg
*
*
*     Desc : Uplink power configuration per UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUeUlPwrCfg
(
RgrUeUlPwrCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(cmUnpkRgrUeGrpPwrCfg, &param->uePuschPwr, mBuf);
   CMCHKUNPK(cmUnpkRgrUeGrpPwrCfg, &param->uePucchPwr, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->isAccumulated, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->isDeltaMCSEnabled, mBuf);
   CMCHKUNPK(SUnpkS8, &param->p0UePusch, mBuf);
   CMCHKUNPK(SUnpkS8, &param->p0UePucch, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->pSRSOffset, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->trgCqi, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrUeQosCfg
*
*
*     Desc : Downlink/Uplink QoS configuration per UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeQosCfg
(
RgrUeQosCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt32, param->ueBr, mBuf);
   CMCHKPK(oduUnpackUInt32, param->dlAmbr, mBuf);
   CMCHKPK(oduUnpackUInt8, param->ambrPres, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUeQosCfg
*
*
*     Desc : Downlink/Uplink QoS configuration per UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUeQosCfg
(
RgrUeQosCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->ambrPres, mBuf);
   CMCHKUNPK(oduPackUInt32, &param->dlAmbr, mBuf);
   CMCHKUNPK(oduPackUInt32, &param->ueBr, mBuf);
   return ROK;
}

/*********************************************************** 
* 
*     Func : cmPkRgrUePuschDedCfg 
* 
* 
*     Desc : PUSCH Config Dedication for UE 
* 
* 
*     Ret  : S16 
* 
*     Notes: 
* 
*     File  :  
* 
**********************************************************/ 
S16 cmPkRgrUePuschDedCfg 
( 
RgrUePuschDedCfg *param, 
Buffer *mBuf 
) 
{ 
 
 
   CMCHKPK(oduUnpackUInt8, param->bCQIIdx, mBuf);
   CMCHKPK(oduUnpackUInt8, param->bRIIdx, mBuf);
   CMCHKPK(oduUnpackUInt8, param->bACKIdx, mBuf);
   CMCHKPK(oduUnpackUInt8, param->pres, mBuf);
   return ROK;
}


/***********************************************************
*
*     Func : cmUnpkRgrUePuschDedCfg
*
*
*     Desc : PUSCH Config Dedication for UE
* 
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUePuschDedCfg
(
RgrUePuschDedCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->pres, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->bACKIdx, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->bRIIdx, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->bCQIIdx, mBuf);
   return ROK;
}

/***********************************************************
*
*     Func : cmPkRgrUeTxAntSelCfg
*
*
*     Desc : UE Transmit Antenna selection related configuration
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeTxAntSelCfg
(
RgrUeTxAntSelCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt32, param->selType, mBuf);
   CMCHKPK(oduUnpackUInt8, param->pres, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUeTxAntSelCfg
*
*
*     Desc : UE Transmit Antenna selection related configuration
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUeTxAntSelCfg
(
RgrUeTxAntSelCfg *param,
Buffer *mBuf
)
{
   uint32_t tmpEnum;


   CMCHKUNPK(oduPackUInt8, &param->pres, mBuf);
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->selType = (RgrUeTxAntSelType) tmpEnum;
   return ROK;
}


/***********************************************************
*
*     Func : cmPkRgrUeTaTmrCfg
*
*
*     Desc : Time Alignment timer configuration per UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeTaTmrCfg
(
RgrUeTaTmrCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt16, param->taTmr, mBuf);
   CMCHKPK(oduUnpackUInt8, param->pres, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUeTaTmrCfg
*
*
*     Desc : Time Alignment timer configuration per UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUeTaTmrCfg
(
RgrUeTaTmrCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->pres, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->taTmr, mBuf);
   return ROK;
}


#ifdef EMTC_ENABLE
S16 cmUnpkRgrEmtcUeCfg
(
RgrUeEmtcCfg *param,
Buffer *mBuf
)
{
   S32 indx=0;
   S32 idx=0;
   RgrEpdcchConfigRel11 *emtcEpdcchCfg = &(param->emtcEpdcchCfg);
   RgrPucchRepCfgRel13 *emtcPucchRepCfg = &(param->emtcPucchRepCfg);
   RgrEpdcchAddModLst *epdcchAddModLst;

   CMCHKUNPK(oduPackUInt8,&param->pdschReptLevModeA,mBuf);
   /*UnPacking PUCCH Rep */
   CMCHKUNPK(oduPackUInt8,&(emtcPucchRepCfg->modeANumPucchRepFormat2),mBuf);
   CMCHKUNPK(oduPackUInt8,&(emtcPucchRepCfg->modeANumPucchRepFormat1),mBuf);
   CMCHKUNPK(oduPackUInt8,&(emtcPucchRepCfg->isPucchRepPres),mBuf);
   
   
   
   for(indx = 0; indx < RGR_MAX_EPDCCH_SET; indx++)
   {
      epdcchAddModLst =  &(emtcEpdcchCfg->epdcchAddModLst[indx]);
      CMCHKUNPK(oduPackUInt32,&(epdcchAddModLst->extaddgrp2.mpddchNB),mBuf);
      CMCHKUNPK(oduPackUInt16,&(epdcchAddModLst->extaddgrp2.mpdcchNumRep),mBuf);
      CMCHKUNPK(oduPackUInt8,&(epdcchAddModLst->extaddgrp2.mpdcchStartUESSFDD),mBuf);
      CMCHKUNPK(oduPackUInt8,&(epdcchAddModLst->extaddgrp2.mpddchPdschHop),mBuf);
      CMCHKUNPK(oduPackUInt8,&(epdcchAddModLst->extaddgrp2.csiNumRep),mBuf);
      CMCHKUNPK(oduPackUInt8,&(epdcchAddModLst->extaddgrp2.pres),mBuf);
      CMCHKUNPK(cmUnpkTknUInt8,&(epdcchAddModLst->mpdcchNumPRBpair),mBuf);
      CMCHKUNPK(cmUnpkTknUInt32,&(epdcchAddModLst->pdschRemapQLcfgId),mBuf);
      CMCHKUNPK(oduPackUInt32,&(epdcchAddModLst->pucchResStartoffset),mBuf);
      CMCHKUNPK(oduPackUInt32,&(epdcchAddModLst->dmrsScrambSeq),mBuf);
      for(idx = 0; idx < 5; idx++)
      {
         CMCHKUNPK(oduPackUInt8,&(epdcchAddModLst->resBlkAssignment.rbAssignment[idx]),mBuf);
      }

      CMCHKUNPK(oduPackUInt8,&(epdcchAddModLst->resBlkAssignment.numPRBpairs),mBuf);
      CMCHKUNPK(oduPackUInt8,&(epdcchAddModLst->transmissionType),mBuf);
      CMCHKUNPK(oduPackUInt8,&(epdcchAddModLst->setConfigId),mBuf);

   }
   CMCHKUNPK(cmUnpkTknUInt32,&(emtcEpdcchCfg->startSymbolr11),mBuf);
   for(indx = 0; indx < 5; indx++)
   {
      CMCHKUNPK(oduPackUInt8,&(emtcEpdcchCfg->sfPtn.measSfPatFDD[indx]),mBuf);
   }
   CMCHKUNPK(oduPackUInt8, &(emtcEpdcchCfg->sfPtn.pres), mBuf);
   CMCHKUNPK(oduPackUInt8, &(param->pres), mBuf);
   CMCHKUNPK(oduPackUInt8, &(param->isHdFddEnbld), mBuf);

#ifdef EMTC_DEBUG_CONSOLE_PRINTS 
   printf("\n EMTC RGR UE unpack\n");
   printf("\nemtcEpdcchCfg->sfPtn.pres %d ",emtcEpdcchCfg->sfPtn.pres);

   for(indx = 0; indx < 5; indx++)
   {
      printf("\nemtcEpdcchCfg->sfPtn.measSfPatFDD[%d] %d ",indx,emtcEpdcchCfg->sfPtn.measSfPatFDD[indx]);
   }
   printf("\nemtcEpdcchCfg->startSymbolr11 %d ",emtcEpdcchCfg->startSymbolr11.pres);
   printf("\nemtcEpdcchCfg->startSymbolr11 %d ",emtcEpdcchCfg->startSymbolr11.val);

   for(indx = 0 ; indx < RGR_MAX_EPDCCH_SET; indx++)
   {
      epdcchAddModLst =  &(emtcEpdcchCfg->epdcchAddModLst[indx]);
      printf("\nemtcEpdcchCfg->epdcchRelLst[indx].setCfgId %d %d ", indx,emtcEpdcchCfg->epdcchRelLst[indx].setCfgId.pres);
      printf("\nemtcEpdcchCfg->epdcchRelLst[indx].setCfgId %d %d ", indx,emtcEpdcchCfg->epdcchRelLst[indx].setCfgId.val);
      printf("\nepdcchAddModLst->setConfigId %d ", epdcchAddModLst->setConfigId);
      printf("\nepdcchAddModLst->transmissionType %d ",epdcchAddModLst->transmissionType);
      printf("\nepdcchAddModLst->resBlkAssignment.numPRBpairs %d ",epdcchAddModLst->resBlkAssignment.numPRBpairs);
      for(idx = 0; idx < 5 ; idx++)
      {
         printf("\nepdcchAddModLst->resBlkAssignment.rbAssignment[idx] %d ",epdcchAddModLst->resBlkAssignment.rbAssignment[idx]);
      }
      printf("\nepdcchAddModLst->dmrsScrambSeq %d ",epdcchAddModLst->dmrsScrambSeq);
      printf("\nepdcchAddModLst->pucchResStartoffset %d ",epdcchAddModLst->pucchResStartoffset);
      printf("\nepdcchAddModLst->pdschRemapQLcfgId.pres %d ",epdcchAddModLst->pdschRemapQLcfgId.pres);
      printf("\nepdcchAddModLst->pdschRemapQLcfgId.val %d ", epdcchAddModLst->pdschRemapQLcfgId.val);
      printf("\nepdcchAddModLst->mpdcchNumPRBpair.pres %d ",  epdcchAddModLst->mpdcchNumPRBpair.pres);
      printf("\nepdcchAddModLst->mpdcchNumPRBpair.val %d ",  epdcchAddModLst->mpdcchNumPRBpair.val);
      printf("\nepdcchAddModLst->extaddgrp2.pres %d ",       epdcchAddModLst->extaddgrp2.pres);
      printf("\nepdcchAddModLst->extaddgrp2.csiNumRep %d ",   epdcchAddModLst->extaddgrp2.csiNumRep);
      printf("\nepdcchAddModLst->extaddgrp2.mpddchPdschHop %d ", epdcchAddModLst->extaddgrp2.mpddchPdschHop);
      printf("\nepdcchAddModLst->extaddgrp2.mpdcchStartUESSFDD %d ",epdcchAddModLst->extaddgrp2.mpdcchStartUESSFDD);
      printf("\nepdcchAddModLst->extaddgrp2.mpdcchNumRep %d ", epdcchAddModLst->extaddgrp2.mpdcchNumRep);
      printf("\nepdcchAddModLst->extaddgrp2.mpddchNB %d ",  epdcchAddModLst->extaddgrp2.mpddchNB);
   }

#endif

   return ROK;
}



S16 cmPkRgrEmtcUeCfg
(
 RgrUeEmtcCfg *param,
 Buffer *mBuf
 )
{
   S32 indx=0;
   S32 idx=0;
   RgrEpdcchConfigRel11 *emtcEpdcchCfg = &(param->emtcEpdcchCfg);
   RgrPucchRepCfgRel13 *emtcPucchRepCfg = &(param->emtcPucchRepCfg);
   RgrEpdcchAddModLst *epdcchAddModLst;

   CMCHKPK(oduUnpackUInt8, param->isHdFddEnbld, mBuf);
   CMCHKPK(oduUnpackUInt8, param->pres, mBuf);
   CMCHKPK(oduUnpackUInt8, emtcEpdcchCfg->sfPtn.pres, mBuf);
   for(indx = 4; indx >=0; indx--)
   {
      CMCHKPK(oduUnpackUInt8,emtcEpdcchCfg->sfPtn.measSfPatFDD[indx],mBuf);
   }
   CMCHKPK(cmPkTknUInt32,&emtcEpdcchCfg->startSymbolr11,mBuf);
   for(indx = RGR_MAX_EPDCCH_SET-1 ; indx >= 0; indx--)
   {
      epdcchAddModLst =  &(emtcEpdcchCfg->epdcchAddModLst[indx]);

      CMCHKPK(oduUnpackUInt8,epdcchAddModLst->setConfigId,mBuf);
      CMCHKPK(oduUnpackUInt8,epdcchAddModLst->transmissionType,mBuf);
      CMCHKPK(oduUnpackUInt8,epdcchAddModLst->resBlkAssignment.numPRBpairs,mBuf);
      for(idx = 4; idx >= 0; idx--)
      {
         CMCHKPK(oduUnpackUInt8,epdcchAddModLst->resBlkAssignment.rbAssignment[idx],mBuf);
      }
      CMCHKPK(oduUnpackUInt32,epdcchAddModLst->dmrsScrambSeq,mBuf);
      CMCHKPK(oduUnpackUInt32,epdcchAddModLst->pucchResStartoffset,mBuf);
      CMCHKPK(cmPkTknUInt32,&(epdcchAddModLst->pdschRemapQLcfgId),mBuf);
      CMCHKPK(cmPkTknUInt8,&(epdcchAddModLst->mpdcchNumPRBpair),mBuf);
      CMCHKPK(oduUnpackUInt8,epdcchAddModLst->extaddgrp2.pres,mBuf);
      CMCHKPK(oduUnpackUInt8,epdcchAddModLst->extaddgrp2.csiNumRep,mBuf);
      CMCHKPK(oduUnpackUInt8,epdcchAddModLst->extaddgrp2.mpddchPdschHop,mBuf);
      CMCHKPK(oduUnpackUInt8,epdcchAddModLst->extaddgrp2.mpdcchStartUESSFDD,mBuf);
      CMCHKPK(oduUnpackUInt16,epdcchAddModLst->extaddgrp2.mpdcchNumRep,mBuf);
      CMCHKPK(oduUnpackUInt32,epdcchAddModLst->extaddgrp2.mpddchNB,mBuf);
   }
   
/*Packing PUCCH Rep */
   CMCHKPK(oduUnpackUInt8,emtcPucchRepCfg->isPucchRepPres, mBuf);
   CMCHKPK(oduUnpackUInt8,emtcPucchRepCfg->modeANumPucchRepFormat1,mBuf);
   CMCHKPK(oduUnpackUInt8,emtcPucchRepCfg->modeANumPucchRepFormat2,mBuf);
   CMCHKPK(oduUnpackUInt8,param->pdschReptLevModeA,mBuf);
   
#ifdef EMTC_DEBUG_CONSOLE_PRINTS 
   printf("\n EMTC RGR UE pack\n");
   printf("\nemtcEpdcchCfg->sfPtn.pres %d ",emtcEpdcchCfg->sfPtn.pres);

   for(indx = 0; indx < 5; indx++)
   {
      printf("\nemtcEpdcchCfg->sfPtn.measSfPatFDD[%d] %d ",indx,emtcEpdcchCfg->sfPtn.measSfPatFDD[indx]);
   }
   printf("\nemtcEpdcchCfg->startSymbolr11 %d ",emtcEpdcchCfg->startSymbolr11.pres);
   printf("\nemtcEpdcchCfg->startSymbolr11 %d ",emtcEpdcchCfg->startSymbolr11.val);

   for(indx = 0 ; indx < RGR_MAX_EPDCCH_SET; indx++)
   {
      epdcchAddModLst =  &(emtcEpdcchCfg->epdcchAddModLst[indx]);
     /* printf("\nemtcEpdcchCfg->epdcchRelLst[indx].setCfgId %d %d ", indx,emtcEpdcchCfg->epdcchRelLst[indx].setCfgId.pres);
      printf("\nemtcEpdcchCfg->epdcchRelLst[indx].setCfgId %d %d ", indx,emtcEpdcchCfg->epdcchRelLst[indx].setCfgId.val);*/
      printf("\nepdcchAddModLst->setConfigId %d ", epdcchAddModLst->setConfigId);
      printf("\nepdcchAddModLst->transmissionType %d ",epdcchAddModLst->transmissionType);
      printf("\nepdcchAddModLst->resBlkAssignment.numPRBpairs %d ",epdcchAddModLst->resBlkAssignment.numPRBpairs);
      for(idx = 0; idx < 5 ; idx++)
      {
         printf("\nepdcchAddModLst->resBlkAssignment.rbAssignment[idx] %d ",epdcchAddModLst->resBlkAssignment.rbAssignment[idx]);
      }
      printf("\nepdcchAddModLst->dmrsScrambSeq %d ",epdcchAddModLst->dmrsScrambSeq);
      printf("\nepdcchAddModLst->pucchResStartoffset %d ",epdcchAddModLst->pucchResStartoffset);
      printf("\nepdcchAddModLst->pdschRemapQLcfgId.pres %d ",epdcchAddModLst->pdschRemapQLcfgId.pres);
      printf("\nepdcchAddModLst->pdschRemapQLcfgId.val %d ", epdcchAddModLst->pdschRemapQLcfgId.val);
      printf("\nepdcchAddModLst->mpdcchNumPRBpair.pres %d ",  epdcchAddModLst->mpdcchNumPRBpair.pres);
      printf("\nepdcchAddModLst->mpdcchNumPRBpair.val %d ",  epdcchAddModLst->mpdcchNumPRBpair.val);
      printf("\nepdcchAddModLst->extaddgrp2.pres %d ",       epdcchAddModLst->extaddgrp2.pres);
      printf("\nepdcchAddModLst->extaddgrp2.csiNumRep %d ",   epdcchAddModLst->extaddgrp2.csiNumRep);
      printf("\nepdcchAddModLst->extaddgrp2.mpddchPdschHop %d ", epdcchAddModLst->extaddgrp2.mpddchPdschHop);
      printf("\nepdcchAddModLst->extaddgrp2.mpdcchStartUESSFDD %d ",epdcchAddModLst->extaddgrp2.mpdcchStartUESSFDD);
      printf("\nepdcchAddModLst->extaddgrp2.mpdcchNumRep %d ", epdcchAddModLst->extaddgrp2.mpdcchNumRep);
      printf("\nepdcchAddModLst->extaddgrp2.mpddchNB %d ",  epdcchAddModLst->extaddgrp2.mpddchNB);
   }

#endif



   return ROK;
}
#endif

#ifdef RG_5GTF
S16 cmUnpkRgr5gtfUeCfg
(
 RgrUe5gtfCfg *param,
 Buffer       *mBuf
)
{
   CMCHKUNPK(oduPackUInt8, &param->grpId, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->BeamId, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->numCC, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->mcs, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->maxPrb, mBuf);
   return ROK;
}

S16 cmPkRgr5gtfUeCfg
(
 RgrUe5gtfCfg *param,
 Buffer       *mBuf
)
{
   CMCHKPK(oduUnpackUInt8, param->maxPrb, mBuf);
   CMCHKPK(oduUnpackUInt8, param->mcs, mBuf);
   CMCHKPK(oduUnpackUInt8, param->numCC, mBuf);
   CMCHKPK(oduUnpackUInt8, param->BeamId, mBuf);
   CMCHKPK(oduUnpackUInt8, param->grpId, mBuf);
   return ROK;
}
#endif


/***********************************************************
*
*     Func : cmPkRgrUeCfg
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
S16 cmPkRgrUeCfg
(
RgrUeCfg *param,
Buffer *mBuf
)
{

#ifdef RG_5GTF
   CMCHKPK(cmPkRgr5gtfUeCfg, &param->ue5gtfCfg, mBuf);
#endif
#ifdef EMTC_ENABLE
 CMCHKPK(cmPkRgrEmtcUeCfg, &param->emtcUeCfg, mBuf);
#endif
   CMCHKPK(oduUnpackUInt8, param->csgMmbrSta, mBuf);

   CMCHKPK(oduUnpackUInt32, param->accessStratumRls, mBuf);
   /* LTE_ADV_FLAG_REMOVED_START */
   CMCHKPK(cmPkRgrLteAdvancedUeConfig, &param->ueLteAdvCfg, mBuf);
   /* LTE_ADV_FLAG_REMOVED_END */
#ifdef TFU_UPGRADE
/*rgr_c_001.main_9 ADD PDSCH ded cfg*/
   CMCHKPK(cmPkRgrUePdschDedCfg, &param->uePdschDedCfg, mBuf);
#endif
   /*rgr_c_001.main_9 ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
   CMCHKPK(cmPkRgrUeCqiReptCfg, &param->ueCqiReptCfg, mBuf);
#endif
   CMCHKPK(cmPkRgrUeTxAntSelCfg, &param->ulTxAntSel, mBuf);

/* rgr_c_001.main_8. Added changes of TFU_UPGRADE */
   CMCHKPK(cmPkRgrUePuschDedCfg, &param->puschDedCfg, mBuf);

/*rgr_c_001.main_9 ADD added changes for HDFDD*/
#ifdef LTEMAC_HDFDD
   CMCHKPK(oduUnpackUInt8, param->isHdFddEnbld, mBuf);
#endif

#ifdef TFU_UPGRADE
   CMCHKPK(cmPkRgrUeSrCfg, &param->srCfg, mBuf);
   CMCHKPK(cmPkRgrUeUlSrsCfg, &param->srsCfg, mBuf);
#endif



   CMCHKPK(cmPkRgrUeSpsCfg, &param->ueSpsCfg, mBuf);

#ifdef LTE_TDD
   CMCHKPK(oduUnpackUInt32, param->ackNackModeEnum, mBuf);

#endif
   CMCHKPK(cmPkTknUInt8, &param->dedPreambleId, mBuf);
   CMCHKPK(cmPkRgrCodeBookRstCfg, &param->ueCodeBookRstCfg, mBuf);
   CMCHKPK(cmPkRgrUeCapCfg, &param->ueCapCfg, mBuf);
   CMCHKPK(cmPkRgrUeMeasGapCfg, &param->ueMesGapCfg, mBuf);
   CMCHKPK(cmPkRgrUeAckNackRepCfg, &param->ueAckNackCfg, mBuf);
   CMCHKPK(oduUnpackUInt8, param->isTtiBundlEnabled, mBuf);
/*rgr_c_001.main_9 ADD added changes for DRX*/
   
   /* Anshika - Pack only if DRX is enabled */
   if(TRUE == param->ueDrxCfg.isDrxEnabled)
   {   
    CMCHKPK(cmPkRgrUeDrxCfg, &param->ueDrxCfg, mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->ueDrxCfg.isDrxEnabled, mBuf); /* This has to be packed even if
                                                 DRX is disabled */
   CMCHKPK(oduUnpackUInt32, param->ueCatEnum, mBuf);
#ifdef RGR_V1
   /* rgr_c_001.main_5: ccpu00112398: Added periodicBSR-timer and
    * retxBSR-timer */
   CMCHKPK(cmPkRgrUeBsrTmrCfg, &param->ueBsrTmrCfg, mBuf);
#endif
   CMCHKPK(cmPkRgrUeTaTmrCfg, &param->ueTaTmrCfg, mBuf);
   CMCHKPK(cmPkRgrUeQosCfg, &param->ueQosCfg, mBuf);
   CMCHKPK(cmPkRgrUeUlPwrCfg, &param->ueUlPwrCfg, mBuf);
   CMCHKPK(cmPkRgrUeUlHqCfg, &param->ueUlHqCfg, mBuf);
   CMCHKPK(cmPkRgrUeDlCqiCfg, &param->ueDlCqiCfg, mBuf);
   CMCHKPK(cmPkRgrUeTxModeCfg, &param->txMode, mBuf);
   CMCHKPK(cmPkLteRnti, param->crnti, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUeCfg
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
S16 cmUnpkRgrUeCfg
(
RgrUeCfg *param,
Buffer *mBuf
)
{
   uint32_t tmpEnum;


   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->crnti, mBuf);
   CMCHKUNPK(cmUnpkRgrUeTxModeCfg, &param->txMode, mBuf);
   CMCHKUNPK(cmUnpkRgrUeDlCqiCfg, &param->ueDlCqiCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrUeUlHqCfg, &param->ueUlHqCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrUeUlPwrCfg, &param->ueUlPwrCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrUeQosCfg, &param->ueQosCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrUeTaTmrCfg, &param->ueTaTmrCfg, mBuf);
#ifdef RGR_V1
   /* rgr_c_001.main_5: ccpu00112398: Added periodicBSR-timer and
    * retxBSR-timer */
   CMCHKUNPK(cmUnpkRgrUeBsrTmrCfg, &param->ueBsrTmrCfg, mBuf);
#endif
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->ueCatEnum = (CmLteUeCategory) tmpEnum;
/*rgr_c_001.main_9 ADD added changes for DRX*/
   
   /* Anshika - Unpack only if DRX is enabled */
   CMCHKUNPK(oduPackUInt8, &param->ueDrxCfg.isDrxEnabled, mBuf);
   if(TRUE == param->ueDrxCfg.isDrxEnabled)
   {
      CMCHKUNPK(cmUnpkRgrUeDrxCfg, &param->ueDrxCfg, mBuf);
   }
   CMCHKUNPK(oduPackUInt8, &param->isTtiBundlEnabled, mBuf);
   CMCHKUNPK(cmUnpkRgrUeAckNackRepCfg, &param->ueAckNackCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrUeMeasGapCfg, &param->ueMesGapCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrUeCapCfg, &param->ueCapCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrCodeBookRstCfg, &param->ueCodeBookRstCfg, mBuf);
   CMCHKUNPK(cmUnpkTknUInt8, &param->dedPreambleId, mBuf);

#ifdef LTE_TDD
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->ackNackModeEnum = tmpEnum;

#endif
   /*rgr_c_001.main_7 - Added support for SPS*/
   CMCHKUNPK(cmUnpkRgrUeSpsCfg, &param->ueSpsCfg, mBuf);

/* rgr_c_001.main_8. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
   CMCHKUNPK(cmUnpkRgrUeUlSrsCfg, &param->srsCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrUeSrCfg, &param->srCfg, mBuf);
#endif
/*rgr_c_001.main_9 ADD added changes for HDFDD*/
#ifdef LTEMAC_HDFDD
   CMCHKUNPK(oduPackUInt8, &param->isHdFddEnbld, mBuf);
#endif
   CMCHKUNPK(cmUnpkRgrUePuschDedCfg, &param->puschDedCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrUeTxAntSelCfg, &param->ulTxAntSel, mBuf);
/* rgr_c_001.main_9 ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
   CMCHKUNPK(cmUnpkRgrUeCqiReptCfg, &param->ueCqiReptCfg, mBuf);
#endif
#ifdef TFU_UPGRADE
   CMCHKUNPK(cmUnpkRgrUePdschDedCfg, &param->uePdschDedCfg, mBuf);
#endif
   /* LTE_ADV_FLAG_REMOVED_START */
   CMCHKUNPK(cmUnpkRgrLteAdvancedUeConfig, &param->ueLteAdvCfg, mBuf);
   /* LTE_ADV_FLAG_REMOVED_END */
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&param->accessStratumRls, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->csgMmbrSta, mBuf);

#ifdef EMTC_ENABLE
 CMCHKUNPK(cmUnpkRgrEmtcUeCfg, &param->emtcUeCfg, mBuf);
#endif

#ifdef RG_5GTF
 CMCHKUNPK(cmUnpkRgr5gtfUeCfg, &param->ue5gtfCfg, mBuf);
#endif

   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrLchQosCfg
*
*
*     Desc : QCI, GBR and MBR configuration for dedicated logical channels
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrLchQosCfg
(
RgrLchQosCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt32, param->mbr, mBuf);
   CMCHKPK(oduUnpackUInt32, param->gbr, mBuf);
   CMCHKPK(oduUnpackUInt8, param->qci, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrLchQosCfg
*
*
*     Desc : QCI, GBR and MBR configuration for dedicated logical channels
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrLchQosCfg
(
RgrLchQosCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->qci, mBuf);
   CMCHKUNPK(oduPackUInt32, &param->gbr, mBuf);
   CMCHKUNPK(oduPackUInt32, &param->mbr, mBuf);
   return ROK;
}
/*rgr_c_001.main_7 - Added support for SPS*/


/***********************************************************
*
*     Func : cmPkRgrLchSpsCfg
*
*
*     Desc : SPS related configuration for logical channels
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrLchSpsCfg
(
RgrLchSpsCfg *param,
Buffer *mBuf
)
{


   /* SPS_DEV */
   CMCHKPK(oduUnpackUInt8, param->isSpsEnabled, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrLchSpsCfg
*
*
*     Desc : SPS related configuration for logical channels
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrLchSpsCfg
(
RgrLchSpsCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->isSpsEnabled, mBuf);
   
   return ROK;
}


/***********************************************************
*
*     Func : cmPkRgrDlLchCfg
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
S16 cmPkRgrDlLchCfg
(
RgrDlLchCfg *param,
Buffer *mBuf
)
{

/*rgr_c_001.main_7 - Added support for SPS*/

   CMCHKPK(oduUnpackUInt8, param->rlcReorderTmr, mBuf);
   CMCHKPK(cmPkRgrLchSpsCfg, &param->dlSpsCfg, mBuf);
   CMCHKPK(cmPkRgrLchQosCfg, &param->dlQos, mBuf);
   CMCHKPK(oduUnpackUInt8, param->dlTrchType, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrDlLchCfg
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
S16 cmUnpkRgrDlLchCfg
(
RgrDlLchCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->dlTrchType, mBuf);
   CMCHKUNPK(cmUnpkRgrLchQosCfg, &param->dlQos, mBuf);
   /*rgr_c_001.main_7 - Added support for SPs*/
   CMCHKUNPK(cmUnpkRgrLchSpsCfg, &param->dlSpsCfg, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->rlcReorderTmr, mBuf);

   return ROK;
}


/*rgr_c_001.main_9 ADD added changes for L2 measurements*/
#ifdef LTE_L2_MEAS


/***********************************************************
*
*     Func : cmPkRgrUlLchCfg
*
*
*     Desc : Logical channel configuration information for uplink logical channels
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUlLchCfg
(
RgrUlLchCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt8, param->qci, mBuf);
   CMCHKPK(cmPkLteLcId, param->lcId, mBuf);
   return ROK;
}



/*rgr_c_001.main_9 ADD added changes for L2 measurements*/
/***********************************************************
*
*     Func : cmUnpkRgrUlLchCfg
*
*
*     Desc : Logical channel configuration information for uplink logical channels
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUlLchCfg
(
RgrUlLchCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(cmUnpkLteLcId, &param->lcId, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->qci, mBuf);
   return ROK;
}


#endif

/***********************************************************
*
*     Func : cmPkRgrUlLcgCfg
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
S16 cmPkRgrUlLcgCfg
(
RgrUlLcgCfg *param,
Buffer *mBuf
)
{

/*rgr_c_001.main_9 ADD added changes for L2 measurements*/
#ifdef LTE_L2_MEAS
   S32 i;
#endif
   CMCHKPK(oduUnpackUInt32, param->mbr, mBuf);
   CMCHKPK(oduUnpackUInt32, param->gbr, mBuf);

/*rgr_c_001.main_9 ADD added changes for L2 measurements*/
#ifdef LTE_L2_MEAS
   for (i = param->numLch - 1; i >= 0; i--)
   {
      CMCHKPK(cmPkRgrUlLchCfg, &param->lchUlCfg[i], mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->numLch, mBuf);
#endif
   CMCHKPK(oduUnpackUInt8, param->lcgId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUlLcgCfg
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
S16 cmUnpkRgrUlLcgCfg
(
RgrUlLcgCfg *param,
Buffer *mBuf
)
{

/*rgr_c_001.main_9 ADD added changes for L2 measurements*/
#ifdef LTE_L2_MEAS
   S32 i;
#endif

   CMCHKUNPK(oduPackUInt8, &param->lcgId, mBuf);
/*rgr_c_001.main_9 ADD added changes for L2 measurements*/
#ifdef LTE_L2_MEAS
   CMCHKUNPK(oduPackUInt8, &param->numLch, mBuf);
   for (i = 0; i< param->numLch; i++)
   {
      CMCHKUNPK(cmUnpkRgrUlLchCfg, &param->lchUlCfg[i], mBuf);
   }

#endif
   CMCHKUNPK(oduPackUInt32, &param->gbr, mBuf);
   CMCHKUNPK(oduPackUInt32, &param->mbr, mBuf);
   return ROK;
}

S16 cmPkRgrUlLchQciCfg
(
RgrUlLchQciCfg *param,
Buffer *mBuf
)
{
   CMCHKPK(oduUnpackUInt8, param->lcId, mBuf);
   CMCHKPK(oduUnpackUInt8, param->qci, mBuf);
   CMCHKPK(oduUnpackUInt8, param->lcgId, mBuf);
   return ROK;
}
S16 cmUnpkRgrUlLchQciCfg
(
RgrUlLchQciCfg *param,
Buffer *mBuf
)
{
   CMCHKUNPK(oduPackUInt8, &param->lcgId, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->qci, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->lcId, mBuf);
   return ROK;
}


/***********************************************************
*
*     Func : cmPkRgrLchCfg
*
*
*     Desc : Logical channel configuration info for dedicated channels
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrLchCfg
(
RgrLchCfg *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt8, param->lcgId, mBuf);
   CMCHKPK(cmPkRgrUlLchQciCfg, &param->ulLchQciInfo, mBuf);
   CMCHKPK(cmPkRgrDlLchCfg, &param->dlInfo, mBuf);
   CMCHKPK(cmPkLteLcType, param->lcType, mBuf);
   CMCHKPK(cmPkLteLcId, param->lcId, mBuf);
   CMCHKPK(cmPkLteRnti, param->crnti, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrLchCfg
*
*
*     Desc : Logical channel configuration info for dedicated channels
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrLchCfg
(
RgrLchCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->crnti, mBuf);
   CMCHKUNPK(cmUnpkLteLcId, &param->lcId, mBuf);
   CMCHKUNPK(cmUnpkLteLcType, &param->lcType, mBuf);
   CMCHKUNPK(cmUnpkRgrDlLchCfg, &param->dlInfo, mBuf);
   CMCHKUNPK(cmUnpkRgrUlLchQciCfg, &param->ulLchQciInfo, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->lcgId, mBuf);
   return ROK;
}


S16 cmPkRgrLcgCfg
(
RgrLcgCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(cmPkRgrUlLcgCfg, &param->ulInfo, mBuf);
   CMCHKPK(cmPkLteRnti, param->crnti, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}


S16 cmUnpkRgrLcgCfg
(
RgrLcgCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->crnti, mBuf);
   CMCHKUNPK(cmUnpkRgrUlLcgCfg, &param->ulInfo, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrCfg
*
*
*     Desc : Basic Configuration structure at RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrCfg
(
RgrCfg *param,
Buffer *mBuf
)
{


      switch(param->cfgType) {
         case RGR_LCG_CFG:
            CMCHKPK(cmPkRgrLcgCfg, &param->u.lcgCfg, mBuf);
            break;
         case RGR_LCH_CFG:
            CMCHKPK(cmPkRgrLchCfg, &param->u.lchCfg, mBuf);
            break;
         case RGR_UE_CFG:
            CMCHKPK(cmPkRgrUeCfg, &param->u.ueCfg, mBuf);
            break;
         case RGR_CELL_CFG:
            CMCHKPK(cmPkRgrCellCfg, &param->u.cellCfg, mBuf);
            break;
         case MAC_GNB_CFG:
            CMCHKPK(cmPkMacSchedGnbCfg, &param->u.schedGnbCfg, mBuf);
            break;
         default :
            return RFAILED;
      }
   CMCHKPK(oduUnpackUInt8, param->cfgType, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrCfg
*
*
*     Desc : Basic Configuration structure at RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrCfg
(
RgrCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->cfgType, mBuf);
      switch(param->cfgType) {
         case MAC_GNB_CFG:
            CMCHKUNPK(cmUnpkMacSchedGnbCfg, &param->u.schedGnbCfg, mBuf);
            break;
         case RGR_CELL_CFG:
            CMCHKUNPK(cmUnpkRgrCellCfg, &param->u.cellCfg, mBuf);
            break;
         case RGR_UE_CFG:
            CMCHKUNPK(cmUnpkRgrUeCfg, &param->u.ueCfg, mBuf);
            break;
         case RGR_LCH_CFG:
            CMCHKUNPK(cmUnpkRgrLchCfg, &param->u.lchCfg, mBuf);
            break;
         case RGR_LCG_CFG:
            CMCHKUNPK(cmUnpkRgrLcgCfg, &param->u.lcgCfg, mBuf);
            break;
         default :
            return RFAILED;
      }
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrActvTime
*
*
*     Desc : Activation time information
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrActvTime
(
RgrActvTime *param,
Buffer *mBuf
)
{


   CMCHKPK(cmPkLteTimingInfo, &param->actvTime, mBuf);
   CMCHKPK(oduUnpackUInt8, param->pres, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrActvTime
*
*
*     Desc : Activation time information
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrActvTime
(
RgrActvTime *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->pres, mBuf);
   CMCHKUNPK(cmUnpkLteTimingInfo, &param->actvTime, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrCellRecfg
*
*
*     Desc : Cell reconfiguration structure at RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrCellRecfg
(
RgrCellRecfg *param,
Buffer *mBuf
)
{

   CMCHKPK(cmPkRgrCellCntrlCmdCfg, &param->cntrlCmdCfg, mBuf);
   CMCHKPK(cmPkRgrCellCsgParamCfg, &param->csgParamCfg, mBuf);
   CMCHKPK(oduUnpackUInt8, param->isDynCfiEnb, mBuf);
   CMCHKPK(oduUnpackUInt8, param->isAutoCfgModeEnb, mBuf);
   /* LTE_ADV_FLAG_REMOVED_START */
   CMCHKPK(cmPkRgrCellLteAdvancedFeatureCfg, &param->rgrLteAdvCfg, mBuf);
   /* LTE_ADV_FLAG_REMOVED_END */
   CMCHKPK(oduUnpackUInt16, param->t300TmrVal, mBuf);
   /* rgr_c_001.main_4:MOD-Modified for SI Enhancement. */
#ifdef RGR_SI_SCH
   CMCHKPK(cmPkRgrSiCfg, &param->siReCfg, mBuf);
#endif /* RGR_SI_SCH */
   CMCHKPK(cmPkRgrDlfsCfg, &param->dlfsRecfg, mBuf);
   CMCHKPK(cmPkRgrRachCfg, &param->rachRecfg, mBuf);
   CMCHKPK(cmPkRgrSrsCfg, &param->srsRecfg, mBuf);
   CMCHKPK(cmPkRgrPucchCfg, &param->pucchRecfg, mBuf);
   CMCHKPK(cmPkRgrUlCmnCodeRateCfg, &param->ulCmnCodeRate, mBuf);
   CMCHKPK(cmPkRgrPuschSubBandCfg, &param->puschSubBand, mBuf);
   CMCHKPK(cmPkRgrDlCmnCodeRateCfg, &param->dlCmnCodeRate, mBuf);
   CMCHKPK(cmPkRgrUlTrgCqiCfg, &param->trgUlCqi, mBuf);
   CMCHKPK(cmPkRgrCfiCfg, &param->cfiRecfg, mBuf);
   CMCHKPK(cmPkRgrDlHqCfg, &param->dlHqRecfg, mBuf);
   CMCHKPK(cmPkRgrActvTime, &param->recfgActvTime, mBuf);
   CMCHKPK(oduUnpackUInt32, param->recfgTypes, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrCellRecfg
*
*
*     Desc : Cell reconfiguration structure at RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrCellRecfg
(
RgrCellRecfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(oduPackUInt32, &param->recfgTypes, mBuf);
   CMCHKUNPK(cmUnpkRgrActvTime, &param->recfgActvTime, mBuf);
   CMCHKUNPK(cmUnpkRgrDlHqCfg, &param->dlHqRecfg, mBuf);
   CMCHKUNPK(cmUnpkRgrCfiCfg, &param->cfiRecfg, mBuf);
   CMCHKUNPK(cmUnpkRgrUlTrgCqiCfg, &param->trgUlCqi, mBuf);
   CMCHKUNPK(cmUnpkRgrDlCmnCodeRateCfg, &param->dlCmnCodeRate, mBuf);
   CMCHKUNPK(cmUnpkRgrPuschSubBandCfg, &param->puschSubBand, mBuf);
   CMCHKUNPK(cmUnpkRgrUlCmnCodeRateCfg, &param->ulCmnCodeRate, mBuf);
   CMCHKUNPK(cmUnpkRgrPucchCfg, &param->pucchRecfg, mBuf);
   CMCHKUNPK(cmUnpkRgrSrsCfg, &param->srsRecfg, mBuf);
   CMCHKUNPK(cmUnpkRgrRachCfg, &param->rachRecfg, mBuf);
   CMCHKUNPK(cmUnpkRgrDlfsCfg, &param->dlfsRecfg, mBuf);
   /* rgr_c_001.main_4-MOD-Modified for SI Enhancement. */
#ifdef RGR_SI_SCH
   CMCHKUNPK(cmUnpkRgrSiCfg, &param->siReCfg, mBuf);
#endif /* RGR_SI_SCH */
   CMCHKUNPK(oduPackUInt16, &param->t300TmrVal, mBuf);
   /* LTE_ADV_FLAG_REMOVED_START */
   CMCHKUNPK(cmUnpkRgrCellLteAdvancedFeatureCfg, &param->rgrLteAdvCfg, mBuf);
   /* LTE_ADV_FLAG_REMOVED_END */
   CMCHKUNPK(oduPackUInt8, &param->isAutoCfgModeEnb, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->isDynCfiEnb, mBuf);
   CMCHKUNPK(cmUnpkRgrCellCsgParamCfg, &param->csgParamCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrCellCntrlCmdCfg, &param->cntrlCmdCfg, mBuf);

   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrUeRecfg
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
S16 cmPkRgrUeRecfg
(
RgrUeRecfg *param,
Buffer *mBuf
)
{

#ifdef EMTC_ENABLE
   /* Note: As we add more members to emtcUeRecfg, 
      create a new function for pkUnpk */
   CMCHKPK(oduUnpackUInt8, param->emtcUeRecfg.isHdFddEnbld, mBuf);
#endif
   CMCHKPK(oduUnpackUInt8, param->csgMmbrSta, mBuf);
  
   CMCHKPK(oduUnpackUInt32, param->accessStratumRls, mBuf);
#ifdef LTE_ADV 
   CMCHKPK(oduUnpackUInt8, param->simulAckNackCQIFormat3, mBuf);
   CMCHKPK(cmPkRgrUeSCellAckPucchCfg, &param->sCellAckN1ResCfg,mBuf);
   CMCHKPK(cmPkRgrUeSecCellInfo, &param->ueSCellCfgInfo,mBuf);
#endif
   /* LTE_ADV_FLAG_REMOVED_START */
   CMCHKPK(oduUnpackUInt8, param->ueLteAdvCfg.isUeCellEdge, mBuf);
   CMCHKPK(oduUnpackUInt8, param->ueLteAdvCfg.isAbsUe, mBuf);
   CMCHKPK(oduUnpackUInt32, param->ueLteAdvCfg.pres, mBuf);
   /* LTE_ADV_FLAG_REMOVED_END */
   /*ccpu00130497: Removed the duplicate packing*/
#ifdef TFU_UPGRADE
   CMCHKPK(cmPkRgrUePdschDedCfg, &param->uePdschDedCfg, mBuf);
#endif
   /* rgr_c_001.main_9 ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
   CMCHKPK(cmPkRgrUeCqiReptCfg, &param->ueCqiReptCfg, mBuf);
#endif

/* rgr_c_001.main_8. Added changes of TFU_UPGRADE */
   CMCHKPK(cmPkRgrUePuschDedCfg, &param->puschDedCfg, mBuf);

/*rgr_c_001.main_9 ADD added changes for HDFDD*/
#ifdef LTEMAC_HDFDD
   CMCHKPK(oduUnpackUInt8, param->isHdFddEnbld, mBuf);
#endif

#ifdef TFU_UPGRADE
   CMCHKPK(cmPkRgrUeSrCfg, &param->srCfg, mBuf);
   CMCHKPK(cmPkRgrUeUlSrsCfg, &param->srsCfg, mBuf);
   CMCHKPK(cmPkRgrUePrdDlCqiCfg, &param->cqiCfg, mBuf);
#endif 

   /*rgr_c_001.main_7 - Added support for SPS*/
   CMCHKPK(cmPkRgrUeSpsCfg, &param->ueSpsRecfg, mBuf);
   CMCHKPK(cmPkRgrUeTxAntSelCfg, &param->ulTxAntSel, mBuf);

   /* rgr_c_001.main_7 - Changes for UE category Reconfiguration */
   CMCHKPK(oduUnpackUInt32, param->ueCatEnum, mBuf);

   CMCHKPK(cmPkRgrCodeBookRstCfg, &param->ueCodeBookRstRecfg, mBuf);
   CMCHKPK(cmPkRgrUeMeasGapCfg, &param->ueMeasGapRecfg, mBuf);
   CMCHKPK(cmPkRgrUeCapCfg, &param->ueCapRecfg, mBuf);
   CMCHKPK(cmPkRgrUeAckNackRepCfg, &param->ueAckNackRecfg, mBuf);
   CMCHKPK(oduUnpackUInt8, param->isTtiBundlEnabled, mBuf);
/*rgr_c_001.main_9 ADD added changes for DRX*/
   
   /* Anshika - Pack only if DRX is enabled */
   if(TRUE == param->ueDrxRecfg.isDrxEnabled)
   {
      CMCHKPK(cmPkRgrUeDrxCfg, &param->ueDrxRecfg, mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->ueDrxRecfg.isDrxEnabled, mBuf); /* This has to be packed even if
                                                 DRX is disabled */
   CMCHKPK(cmPkRgrUeUlPwrCfg, &param->ueUlPwrRecfg, mBuf);
#ifdef RGR_V1
   /* rgr_c_001.main_5: ccpu00112398: Added periodicBSR-timer and
    * retxBSR-timer */
   CMCHKPK(cmPkRgrUeBsrTmrCfg, &param->ueBsrTmrRecfg, mBuf);
#endif
   CMCHKPK(cmPkRgrUeTaTmrCfg, &param->ueTaTmrRecfg, mBuf);
   CMCHKPK(cmPkRgrUeQosCfg, &param->ueQosRecfg, mBuf);
   CMCHKPK(cmPkRgrUeUlHqCfg, &param->ueUlHqRecfg, mBuf);

#ifndef TFU_UPGRADE
   CMCHKPK(cmPkRgrUePrdDlCqiCfg, &param->prdDlCqiRecfg, mBuf);
#endif

   CMCHKPK(cmPkRgrUeAprdDlCqiCfg, &param->aprdDlCqiRecfg, mBuf);
   CMCHKPK(cmPkRgrUeTxModeCfg, &param->txMode, mBuf);
   /*rgr_c_001.main_9: changing ueRecfgTypes to uint32_t */
   /* LTE_ADV_FLAG_REMOVED_START */
   /* KW fix for LTE_ADV */
   CMCHKPK(oduUnpackUInt32, param->ueRecfgTypes, mBuf);
   /* LTE_ADV_FLAG_REMOVED_END */
   CMCHKPK(cmPkLteRnti, param->newCrnti, mBuf);
   CMCHKPK(cmPkLteRnti, param->oldCrnti, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrUeRecfg
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
S16 cmUnpkRgrUeRecfg
(
RgrUeRecfg *param,
Buffer *mBuf
)
{
   uint32_t tmpEnum;


   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->oldCrnti, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->newCrnti, mBuf);
   /* LTE_ADV_FLAG_REMOVED_START */
   /*rgr_c_001.main_9: changing ueRecfgTypes to uint32_t */
   /* KW fix for LTE_ADV */
   CMCHKUNPK(oduPackUInt32, &param->ueRecfgTypes, mBuf);
   /* LTE_ADV_FLAG_REMOVED_END */
   CMCHKUNPK(cmUnpkRgrUeTxModeCfg, &param->txMode, mBuf);

   CMCHKUNPK(cmUnpkRgrUeAprdDlCqiCfg, &param->aprdDlCqiRecfg, mBuf);

#ifndef TFU_UPGRADE
   CMCHKUNPK(cmUnpkRgrUePrdDlCqiCfg, &param->prdDlCqiRecfg, mBuf);
#endif

   CMCHKUNPK(cmUnpkRgrUeUlHqCfg, &param->ueUlHqRecfg, mBuf);
   CMCHKUNPK(cmUnpkRgrUeQosCfg, &param->ueQosRecfg, mBuf);
   CMCHKUNPK(cmUnpkRgrUeTaTmrCfg, &param->ueTaTmrRecfg, mBuf);
#ifdef RGR_V1
   /* rgr_c_001.main_5: ccpu00112398: Added periodicBSR-timer and
    * retxBSR-timer */
   CMCHKUNPK(cmUnpkRgrUeBsrTmrCfg, &param->ueBsrTmrRecfg, mBuf);
#endif
   CMCHKUNPK(cmUnpkRgrUeUlPwrCfg, &param->ueUlPwrRecfg, mBuf);
/*rgr_c_001.main_9 ADD added changes for DRX*/
   
   /* Anshika - Unpack only if DRX is enabled */
   CMCHKUNPK(oduPackUInt8, &param->ueDrxRecfg.isDrxEnabled, mBuf);
   if(TRUE == param->ueDrxRecfg.isDrxEnabled)
   {
      CMCHKUNPK(cmUnpkRgrUeDrxCfg, &param->ueDrxRecfg, mBuf);
   }
   CMCHKUNPK(oduPackUInt8, &param->isTtiBundlEnabled, mBuf);
   CMCHKUNPK(cmUnpkRgrUeAckNackRepCfg, &param->ueAckNackRecfg, mBuf);
   CMCHKUNPK(cmUnpkRgrUeCapCfg, &param->ueCapRecfg, mBuf);
   CMCHKUNPK(cmUnpkRgrUeMeasGapCfg, &param->ueMeasGapRecfg, mBuf);
   CMCHKUNPK(cmUnpkRgrCodeBookRstCfg, &param->ueCodeBookRstRecfg, mBuf);
/* rgr_c_001.main_7 - Changes for UE category Reconfiguration */
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->ueCatEnum = (CmLteUeCategory) tmpEnum;
   CMCHKUNPK(cmUnpkRgrUeTxAntSelCfg, &param->ulTxAntSel, mBuf);
   /*rgr_c_001.main_7 - Added support for SPS*/
   CMCHKUNPK(cmUnpkRgrUeSpsCfg, &param->ueSpsRecfg, mBuf);

/* rgr_c_001.main_8. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
   CMCHKUNPK(cmUnpkRgrUePrdDlCqiCfg, &param->cqiCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrUeUlSrsCfg, &param->srsCfg, mBuf);
   CMCHKUNPK(cmUnpkRgrUeSrCfg, &param->srCfg, mBuf);
#endif

/*rgr_c_001.main_9 ADD added changes for HDFDD*/
#ifdef LTEMAC_HDFDD
   CMCHKUNPK(oduPackUInt8, &param->isHdFddEnbld, mBuf);
#endif

   CMCHKUNPK(cmUnpkRgrUePuschDedCfg, &param->puschDedCfg, mBuf);

/* rgr_c_001.main_9 ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
   CMCHKUNPK(cmUnpkRgrUeCqiReptCfg, &param->ueCqiReptCfg, mBuf);
#endif
#ifdef TFU_UPGRADE
   CMCHKUNPK(cmUnpkRgrUePdschDedCfg, &param->uePdschDedCfg, mBuf);
#endif

   /* LTE_ADV_FLAG_REMOVED_START */
   CMCHKUNPK(oduPackUInt32, &param->ueLteAdvCfg.pres, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->ueLteAdvCfg.isAbsUe, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->ueLteAdvCfg.isUeCellEdge, mBuf);
   /* LTE_ADV_FLAG_REMOVED_END */
#ifdef LTE_ADV 
   CMCHKUNPK(cmUnpkRgrUeSecCellInfo, &param->ueSCellCfgInfo,mBuf);
   CMCHKUNPK(cmUnpkRgrUeSCellAckPucchCfg, &param->sCellAckN1ResCfg,mBuf);
   CMCHKUNPK(oduPackUInt8, &param->simulAckNackCQIFormat3, mBuf);
#endif
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&param->accessStratumRls, mBuf);

   CMCHKUNPK(oduPackUInt8, &param->csgMmbrSta, mBuf);
#ifdef EMTC_ENABLE
   CMCHKPK(oduPackUInt8, &param->emtcUeRecfg.isHdFddEnbld, mBuf);
#endif

   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrLchRecfg
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
S16 cmPkRgrLchRecfg
(
RgrLchRecfg *param,
Buffer *mBuf
)
{

/*rgr_c_001.main_7 - Added support for SPS*/
   CMCHKPK(oduUnpackUInt8, param->lcgId, mBuf);
   
   CMCHKPK(cmPkRgrUlLchQciCfg, &param->ulLchQciInfo, mBuf);
   CMCHKPK(cmPkRgrLchSpsCfg, &param->dlRecfg.dlSpsRecfg, mBuf);
   
   CMCHKPK(cmPkRgrLchQosCfg, &param->dlRecfg.dlQos, mBuf);
   CMCHKPK(cmPkLteLcId, param->lcId, mBuf);
   CMCHKPK(cmPkLteRnti, param->crnti, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrLchRecfg
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
S16 cmUnpkRgrLchRecfg
(
RgrLchRecfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->crnti, mBuf);
   CMCHKUNPK(cmUnpkLteLcId, &param->lcId, mBuf);
   CMCHKUNPK(cmUnpkRgrLchQosCfg, &param->dlRecfg.dlQos, mBuf);
      /*rgr_c_001.main_7 - Added support for SPs*/
   CMCHKUNPK(cmUnpkRgrLchSpsCfg, &param->dlRecfg.dlSpsRecfg, mBuf);

   CMCHKUNPK(cmUnpkRgrUlLchQciCfg, &param->ulLchQciInfo, mBuf);   
   CMCHKUNPK(oduPackUInt8, &param->lcgId, mBuf);
   return ROK;
}


S16 cmPkRgrLcgRecfg
(
RgrLcgRecfg *param,
Buffer *mBuf
)
{
#ifdef RG_UNUSED
   S32 i;
#endif

   CMCHKPK(oduUnpackUInt32, param->ulRecfg.mbr, mBuf);
   CMCHKPK(oduUnpackUInt32, param->ulRecfg.gbr, mBuf);
#ifdef RG_UNUSED
   for (i = param->ulRecfg.numLch - 1; i >= 0; i--)
   {
      CMCHKPK(cmPkRgrUlLchCfg, &param->ulRecfg.lchUlCfg[i], mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->ulRecfg.numLch, mBuf);
#endif
   CMCHKPK(oduUnpackUInt8, param->ulRecfg.lcgId, mBuf);
   CMCHKPK(cmPkLteRnti, param->crnti, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}


S16 cmUnpkRgrLcgRecfg
(
RgrLcgRecfg *param,
Buffer *mBuf
)
{
#ifdef RG_UNUSED
   S32 i;
#endif

   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->crnti, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->ulRecfg.lcgId, mBuf);

#ifdef RG_UNUSED
   CMCHKUNPK(oduPackUInt8, &param->ulRecfg.numLch, mBuf);
   for (i = 0; i < param->ulRecfg.numLch; i++)
   {
      CMCHKUNPK(cmUnpkRgrUlLchCfg, &param->ulRecfg.lchUlCfg[i], mBuf);
   }
#endif
   CMCHKUNPK(oduPackUInt32, &param->ulRecfg.gbr, mBuf);
   CMCHKUNPK(oduPackUInt32, &param->ulRecfg.mbr, mBuf);

   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrRecfg
*
*
*     Desc : Basic reconfiguration structure at RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrRecfg
(
RgrRecfg *param,
Buffer *mBuf
)
{


      switch(param->recfgType) {
         case RGR_LCG_CFG:
            CMCHKPK(cmPkRgrLcgRecfg, &param->u.lcgRecfg, mBuf);
            break;
         case RGR_LCH_CFG:
            CMCHKPK(cmPkRgrLchRecfg, &param->u.lchRecfg, mBuf);
            break;
         case RGR_UE_CFG:
            CMCHKPK(cmPkRgrUeRecfg, &param->u.ueRecfg, mBuf);
            break;
         case RGR_CELL_CFG:
            CMCHKPK(cmPkRgrCellRecfg, &param->u.cellRecfg, mBuf);
            break;
         default :
            return RFAILED;
      }
   CMCHKPK(oduUnpackUInt8, param->recfgType, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrRecfg
*
*
*     Desc : Basic reconfiguration structure at RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrRecfg
(
RgrRecfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->recfgType, mBuf);
      switch(param->recfgType) {
         case RGR_CELL_CFG:
            CMCHKUNPK(cmUnpkRgrCellRecfg, &param->u.cellRecfg, mBuf);
            break;
         case RGR_UE_CFG:
            CMCHKUNPK(cmUnpkRgrUeRecfg, &param->u.ueRecfg, mBuf);
            break;
         case RGR_LCH_CFG:
            CMCHKUNPK(cmUnpkRgrLchRecfg, &param->u.lchRecfg, mBuf);
            break;
         case RGR_LCG_CFG:
            CMCHKUNPK(cmUnpkRgrLcgRecfg, &param->u.lcgRecfg, mBuf);
            break;
         default :
            return RFAILED;
      }
   return ROK;
}

#ifdef LTE_ADV
/***********************************************************
*
*     Func : cmPkRgrUeDlSecCellRelInfo
*
*
*     Desc : DL Secondary Cell information of the UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeDlSecCellRelInfo
(
RgrUeDlSecCellRelInfo *param,
Buffer *mBuf
)
{
   CMCHKPK(oduUnpackUInt16, param->sCellId, mBuf);
   CMCHKPK(oduUnpackUInt8, param->sCellIdx, mBuf);
   return ROK;
}

/***********************************************************
*
*     Func : cmPkRgrUeSecCellRelInfo
*
*
*     Desc : Secondary Cell Delete information of the UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeSecCellRelInfo
(
RgrUeSecCellRelInfo *param,
Buffer *mBuf
)
{
   S8 idx;
   for(idx = param->numSCells - 1; idx >= 0; idx--)
   {
      CMCHKPK(cmPkRgrUeDlSecCellRelInfo, &param->ueSCellRelDedCfg[(uint8_t)idx],mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->numSCells, mBuf);

   return ROK;
}
#endif


/***********************************************************
*
*     Func : cmPkRgrDel
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
S16 cmPkRgrDel
(
RgrDel *param,
Buffer *mBuf
)
{


      switch(param->delType) {
         case RGR_LCG_CFG:
            CMCHKPK(oduUnpackUInt8, param->u.lcgDel.lcgId, mBuf);
            CMCHKPK(cmPkLteRnti, param->u.lcgDel.crnti, mBuf);
            CMCHKPK(cmPkLteCellId, param->u.lcgDel.cellId, mBuf);
         break;
         case RGR_LCH_CFG:
            CMCHKPK(oduUnpackUInt8, param->u.lchDel.lcgId, mBuf);
            CMCHKPK(cmPkLteLcId, param->u.lchDel.lcId, mBuf);
            CMCHKPK(cmPkLteRnti, param->u.lchDel.crnti, mBuf);
            CMCHKPK(cmPkLteCellId, param->u.lchDel.cellId, mBuf);
         break;
         case RGR_UE_CFG:
            CMCHKPK(cmPkLteRnti, param->u.ueDel.crnti, mBuf);
            CMCHKPK(cmPkLteCellId, param->u.ueDel.cellId, mBuf);
         break;
         case RGR_CELL_CFG:
            CMCHKPK(cmPkLteCellId, param->u.cellDel.cellId, mBuf);
         break;
#ifdef LTE_ADV
         case RGR_SCELL_UE_CFG:
            CMCHKPK(cmPkRgrUeSecCellRelInfo, &param->u.ueScellRel.ueSCellRelCfgInfo, mBuf);
            CMCHKPK(oduUnpackUInt32, param->u.ueScellRel.ueDelTypes, mBuf);
            CMCHKPK(cmPkLteRnti, param->u.ueScellRel.crnti, mBuf);
            CMCHKPK(cmPkLteCellId, param->u.ueScellRel.cellId, mBuf);
         break;
#endif
         default :
            return RFAILED;
      }
   CMCHKPK(oduUnpackUInt8, param->delType, mBuf);
   return ROK;
}


#ifdef LTE_ADV

/***********************************************************
*
*     Func : cmUnpkRgrUeDlSecCellRelInfo
*
*
*     Desc : DL Secondary Cell information of the UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUeDlSecCellRelInfo
(
RgrUeDlSecCellRelInfo *param,
Buffer *mBuf
)
{
   CMCHKUNPK(oduPackUInt8, &param->sCellIdx, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->sCellId, mBuf);
   return ROK;
}


/***********************************************************
*
*     Func : cmUnpkRgrUeSecCellRelInfo
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
S16 cmUnpkRgrUeSecCellRelInfo
(
RgrUeSecCellRelInfo *param,
Buffer *mBuf
)
{
   uint8_t idx;

   CMCHKUNPK(oduPackUInt8, &param->numSCells, mBuf);

   for(idx = 0; idx < param->numSCells;idx++)
   {
      CMCHKUNPK(cmUnpkRgrUeDlSecCellRelInfo, &param->ueSCellRelDedCfg[idx],mBuf);
   }
   return ROK;
}
#endif

/***********************************************************
*
*     Func : cmUnpkRgrDel
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
S16 cmUnpkRgrDel
(
RgrDel *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->delType, mBuf);
      switch(param->delType) {
         case RGR_CELL_CFG:
            CMCHKUNPK(cmUnpkLteCellId, &param->u.cellDel.cellId, mBuf);
         break;
         case RGR_UE_CFG:
            CMCHKUNPK(cmUnpkLteCellId, &param->u.ueDel.cellId, mBuf);
            CMCHKUNPK(cmUnpkLteRnti, &param->u.ueDel.crnti, mBuf);
         break;
         case RGR_LCH_CFG:
            CMCHKUNPK(cmUnpkLteCellId, &param->u.lchDel.cellId, mBuf);
            CMCHKUNPK(cmUnpkLteRnti, &param->u.lchDel.crnti, mBuf);
            CMCHKUNPK(cmUnpkLteLcId, &param->u.lchDel.lcId, mBuf);
            CMCHKUNPK(oduPackUInt8, &param->u.lchDel.lcgId, mBuf);
         break;
         case RGR_LCG_CFG:
            CMCHKUNPK(cmUnpkLteCellId, &param->u.lcgDel.cellId, mBuf);
            CMCHKUNPK(cmUnpkLteRnti, &param->u.lcgDel.crnti, mBuf);
            CMCHKUNPK(oduPackUInt8, &param->u.lcgDel.lcgId, mBuf);
         break;
#ifdef LTE_ADV
         case RGR_SCELL_UE_CFG:
            CMCHKUNPK(cmUnpkLteCellId, &param->u.ueScellRel.cellId, mBuf);
            CMCHKUNPK(cmUnpkLteRnti, &param->u.ueScellRel.crnti, mBuf);
            CMCHKUNPK(oduPackUInt32, &param->u.ueScellRel.ueDelTypes, mBuf);
            CMCHKUNPK(cmUnpkRgrUeSecCellRelInfo, &param->u.ueScellRel.ueSCellRelCfgInfo, mBuf);
         break;
#endif
         default :
            return RFAILED;
      }
   return ROK;
}
/***********************************************************
*
*     Func : cmPkRgrSonPrbCfg
*
*
*     Desc : SON PRB Configuration info for MAC
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  :
*
**********************************************************/
S16 cmPkRgrSonPrbCfg
(
RgrPrbCfg *param,
Buffer *mBuf
)
{
   S32 count;
   for(count = RGR_SCH_MAX_PA_PER_PRB - 1; count >= 0; count --)
   {
      CMCHKPK(oduUnpackUInt32, param->paLevelPerPrb[count], mBuf);
   }
   for(count = param->numCellCentreUEs - 1; count >= 0; count --)
   {
      CMCHKPK(cmPkLteRnti, param->cellCentreUe[count], mBuf);
   } 
   for(count = param->numCellEdgeUEs - 1; count >= 0; count --)
   {
      CMCHKPK(cmPkLteRnti, param->cellEdgeUe[count], mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->numCellCentreUEs, mBuf);
   CMCHKPK(oduUnpackUInt8, param->numCellEdgeUEs, mBuf);
   CMCHKPK(oduUnpackUInt8, param->isSonIcicEnable, mBuf);
   return ROK;
}

/***********************************************************
*
*     Func : cmUnpkRgrSon
*
*
*     Desc : SON configuration info for MAC
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  :
*
**********************************************************/
S16 cmUnpkRgrSonPrbCfg
(
RgrPrbCfg   *param,
Buffer      *mBuf
)
{
   S32   count;
   CMCHKUNPK(oduPackUInt8, &param->isSonIcicEnable, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->numCellEdgeUEs, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->numCellCentreUEs, mBuf);
   for (count = 0; count < param->numCellEdgeUEs; count++)
   {
      CMCHKUNPK(cmUnpkLteRnti, &param->cellEdgeUe[count], mBuf);
   }
   for (count = 0; count < param->numCellCentreUEs; count++)
   {
      CMCHKUNPK(cmUnpkLteRnti, &param->cellCentreUe[count], mBuf);
   }
   for(count = 0; count < RGR_SCH_MAX_PA_PER_PRB; count ++)
   {
      CMCHKUNPK(oduPackUInt32,(uint32_t *) &param->paLevelPerPrb[count], mBuf);
   }
   return ROK;
}
/***********************************************************
*
*     Func : cmPkRgrSonCfg
*
*
*     Desc : SON Configuration info for MAC
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  :
*
**********************************************************/
S16 cmPkRgrSonCfg
(
RgrSonCfg *param,
Buffer *mBuf
)
{

   switch(param->cfgType)
   {
      case RGR_SON_PRB_CFG :
      CMCHKPK(cmPkRgrSonPrbCfg, &param->u.prbCfg, mBuf);
      break;
      default :
      return RFAILED;
   }
   CMCHKPK(oduUnpackUInt32, param->cfgType, mBuf);
   return ROK;
}

/***********************************************************
*
*     Func : cmUnpkRgrSon
*
*
*     Desc : SON configuration info for MAC
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  :
*
**********************************************************/
S16 cmUnpkRgrSonCfg
(
RgrSonCfg   *param,
Buffer      *mBuf
)
{

   CMCHKUNPK(oduPackUInt32, (uint32_t *)&param->cfgType, mBuf);
   switch(param->cfgType)
   {
      case RGR_SON_PRB_CFG :
      {
         CMCHKUNPK(cmUnpkRgrSonPrbCfg, &param->u.prbCfg, mBuf);
         break;
         default :
         return RFAILED;
      }
   }
   return ROK;
}


/***********************************************************
*
*     Func : cmPkRgrRst
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
S16 cmPkRgrRst
(
RgrRst *param,
Buffer *mBuf
)
{


   CMCHKPK(cmPkLteRnti, param->crnti, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrRst
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
S16 cmUnpkRgrRst
(
RgrRst *param,
Buffer *mBuf
)
{


   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->crnti, mBuf);
   return ROK;
}
#ifdef LTE_ADV
/***********************************************************
*
*     Func : cmUnPkRgrSCellActDeactInfo
*
*
*     Desc : SCell Activation information of the UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
static S16 cmUnPkRgrSCellActDeactInfo
(
RgrSCellActDeactInfo *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt8, &param->sCellIdx, mBuf);
   return ROK;
}

/***********************************************************
*
*     Func : cmUnPkRgrSCellActDeactEvnt
*
*
*     Desc : SCell Activation Configuration structure to SCH
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnPkRgrSCellActDeactEvnt
(
RgrSCellActDeactEvnt *param,
Buffer *mBuf
)
{

   uint8_t idx;

   CMCHKUNPK(cmUnpkLteRnti, &param->crnti, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->numOfSCells, mBuf);

   for(idx = 0; idx < param->numOfSCells;idx++)
   {
      CMCHKUNPK(cmUnPkRgrSCellActDeactInfo, &param->sCellActDeactInfo[idx],mBuf);
   }

   return ROK;
}


/***********************************************************
*
*     Func : cmPkRgrSCellActInfo
*
*
*     Desc : SCell Activation information of the UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
static S16 cmPkRgrSCellActDeactInfo
(
RgrSCellActDeactInfo *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt8, param->sCellIdx, mBuf);

   return ROK;
}

/***********************************************************
*
*     Func : cmPkRgrSCellActDeactEvnt
*
*
*     Desc : Basic Configuration structure at SCH
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrSCellActDeactEvnt
(
RgrSCellActDeactEvnt *param,
Buffer *mBuf
)
{


   S8 idx;
   for(idx = param->numOfSCells - 1; idx >= 0; idx--)
   {
      CMCHKPK(cmPkRgrSCellActDeactInfo, &param->sCellActDeactInfo[(uint8_t)idx], mBuf);
   }

   CMCHKPK(oduUnpackUInt8, param->numOfSCells, mBuf);
   CMCHKPK(cmPkLteRnti, param->crnti, mBuf);

   return ROK;
}


#endif /* LTE_ADV */

/***********************************************************
*
*     Func : cmPkRgrCfgReqInfo
*
*
*     Desc : Basic RGR configuration/reconfiguration info at RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrCfgReqInfo
(
RgrCfgReqInfo *param,
Buffer *mBuf
)
{


      switch(param->action) {
         case RGR_SON_CFG:
            CMCHKPK(cmPkRgrSonCfg, &param->u.sonCfg, mBuf);
            break;
         case RGR_RESET:
            CMCHKPK(cmPkRgrRst, &param->u.rstInfo, mBuf);
            break;
         case RGR_DELETE:
            CMCHKPK(cmPkRgrDel, &param->u.delInfo, mBuf);
            break;
         case RGR_RECONFIG:
            CMCHKPK(cmPkRgrRecfg, &param->u.recfgInfo, mBuf);
            break;
         case SCH_CONFIG:
            CMCHKPK(cmPkRgrCfg, &param->u.cfgInfo, mBuf);
            break;
#ifdef LTE_ADV
         case RGR_SCELL_ACT:
         case RGR_SCELL_DEACT:
         case RGR_SCELL_READY:
            CMCHKPK(cmPkRgrSCellActDeactEvnt, &param->u.sCellActDeactEvnt, mBuf);
            break;
#endif
         default :
            return RFAILED;
      }
   CMCHKPK(oduUnpackUInt8, param->action, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgrCfgReqInfo
*
*
*     Desc : Basic RGR configuration/reconfiguration info at RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrCfgReqInfo
(
RgrCfgReqInfo *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->action, mBuf);
      switch(param->action) {
         case SCH_CONFIG:
            CMCHKUNPK(cmUnpkRgrCfg, &param->u.cfgInfo, mBuf);
            break;
         case RGR_RECONFIG:
            CMCHKUNPK(cmUnpkRgrRecfg, &param->u.recfgInfo, mBuf);
            break;
         case RGR_DELETE:
            CMCHKUNPK(cmUnpkRgrDel, &param->u.delInfo, mBuf);
            break;
         case RGR_RESET:
            CMCHKUNPK(cmUnpkRgrRst, &param->u.rstInfo, mBuf);
            break;
         case RGR_SON_CFG:
            CMCHKUNPK(cmUnpkRgrSonCfg, &param->u.sonCfg, mBuf);
            break;
#ifdef LTE_ADV
         case RGR_SCELL_ACT:
         case RGR_SCELL_DEACT:
         case RGR_SCELL_READY:
            CMCHKUNPK(cmUnPkRgrSCellActDeactEvnt, &param->u.sCellActDeactEvnt, mBuf);
            break;
#endif /* LTE_ADV */
         default :
            return RFAILED;
      }
   return ROK;
}
#ifdef RGR_V1
/* rgr_c_001.main_5: ccpu00112398: Added periodicBSR-timer and
 * retxBSR-timer */
/***********************************************************
*
*     Func : cmPkRgrUeBsrTmrCfg
*
*
*     Desc : BSR timer configuration per UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeBsrTmrCfg
(
RgrUeBsrTmrCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt16, param->prdBsrTmr, mBuf);
   CMCHKPK(oduUnpackUInt16, param->retxBsrTmr, mBuf);
   CMCHKPK(oduUnpackUInt8, param->isPrdBsrTmrPres, mBuf);
   return ROK;
}

/***********************************************************
*
*     Func : cmUnpkRgrUeBsrTmrCfg
*
*
*     Desc : BSR timer configuration per UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUeBsrTmrCfg
(
RgrUeBsrTmrCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->isPrdBsrTmrPres, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->retxBsrTmr, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->prdBsrTmr, mBuf);
   return ROK;
}
#endif /* RGR_V1 */
/* rgr_c_001.main_4-ADD-Added for SI Enhancement. */
#ifdef RGR_SI_SCH
/***********************************************************
*
*     Func : cmPkRgrSiCfgReq
*
*
*     Desc : SI Configuration Request from RRM to MAC for 
*            configuring SI
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrSiCfgReq
(
Pst* pst,
SpId spId,
RgrCfgTransId transId,
RgrSiCfgReqInfo * cfgReqInfo
)
{
   Buffer *mBuf = NULLP;


   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR045, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgReqInfo, 
                 sizeof(RgrSiCfgReqInfo));
      return RFAILED;
   }
   if (cmPkRgrSiCfgReqInfo(cfgReqInfo, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGR046, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgReqInfo, 
                sizeof(RgrSiCfgReqInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (cmPkRgrCfgTransId(&transId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR047, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgReqInfo, 
               sizeof(RgrSiCfgReqInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR048, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgReqInfo, 
               sizeof(RgrSiCfgReqInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgReqInfo, 
               sizeof(RgrSiCfgReqInfo)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGR049, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTRGRSICFGREQ;
   return (SPstTsk(pst,mBuf));
}

/***********************************************************
*
*     Func : cmUnpkRgrSiCfgReq
*
*
*     Desc : SI Configuration Request from RRM to MAC for 
*     configuring SI
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrSiCfgReq
(
RgrSiCfgReq func,
Pst *pst,
Buffer *mBuf
)
{
   SpId spId;
   RgrCfgTransId transId;
   RgrSiCfgReqInfo *cfgReqInfo;
   

   if (SUnpkS16(&spId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR050, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (cmUnpkRgrCfgTransId(&transId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR051, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if ((SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&cfgReqInfo,
            sizeof(RgrSiCfgReqInfo))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGR052, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   memset(cfgReqInfo, 0, sizeof(RgrSiCfgReqInfo));
   if (pst->selector == ODU_SELECTOR_LC) 
      if (cmUnpkRgrSiCfgReqInfo(cfgReqInfo, mBuf) != ROK) {
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgReqInfo, 
                          sizeof(RgrSiCfgReqInfo));
         SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ERGR053, (ErrVal)0, "Packing failed");
#endif
         return RFAILED;
      }
   SPutMsg(mBuf);

   return ((*func)(pst, spId, transId, cfgReqInfo));
}

/***********************************************************
*
*     Func : cmPkRgrSiCfgReqInfo
*
*
*     Desc : Basic SI configuration/reconfiguration info at RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrSiCfgReqInfo
(
RgrSiCfgReqInfo *param,
Buffer *mBuf
)
{
  /* ccpu00111328: S16 is renamed as MsgLen */
   MsgLen msgLen;


   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(SPkS32, param->cfgType, mBuf);
   CMCHKPK(oduUnpackUInt8, param->siId, mBuf);

   if(NULLP != param->pdu)
   {
      if (SFndLenMsg(param->pdu, &msgLen) != ROK)
         return RFAILED;
      if (SCatMsg(mBuf, param->pdu, M1M2) != ROK)
         return RFAILED;
      SPutMsg(param->pdu);
      CMCHKPK(cmPkMsgLen, msgLen, mBuf);
   }
   /*ccpu00140789*/
   else
   {
      CMCHKPK(cmPkMsgLen, 0, mBuf);
   }

   return ROK;
}

/***********************************************************
*
*     Func : cmUnpkRgrSiCfgReqInfo
*
*
*     Desc : Basic SI configuration/reconfiguration info at RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrSiCfgReqInfo
(
RgrSiCfgReqInfo *param,
Buffer *mBuf
)
{
   MsgLen  msgLen, totalMsgLen;
   /*Merge from Mohit Changes*/
   S32    cfgType;


   SFndLenMsg(mBuf, &msgLen);
   if(msgLen > 0)
   {
      CMCHKUNPK(cmUnpkMsgLen, &msgLen, mBuf);
      /*ccpu00140789*/
      if(msgLen > 0)
      {
         if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
            return RFAILED;
         if (SSegMsg(mBuf, totalMsgLen-msgLen, &param->pdu) != ROK)
            return RFAILED;
      }
      else
      {
         param->pdu = NULLP;
      }
   }

   CMCHKUNPK(oduPackUInt8, &param->siId, mBuf);
   /*CMCHKUNPK(SUnpkS32, (S32 *)&param->cfgType, mBuf);*/
   /*Merge from Mohit Changes*/
   CMCHKUNPK(SUnpkS32,(S32 *)&cfgType, mBuf);
   param->cfgType = (RgrSiCfgType) cfgType;
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);

   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrWarningSiCfgReq
*
*
*     Desc : SI Configuration Request from RRM to MAC for 
*            configuring warning SI
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrWarningSiCfgReq
(
Pst* pst,
SpId spId,
RgrCfgTransId transId,
RgrWarningSiCfgReqInfo * warningSiCfgReqInfo
)
{
   Buffer *mBuf = NULLP;


   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR061, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)warningSiCfgReqInfo, 
                 sizeof(RgrWarningSiCfgReqInfo));
      return RFAILED;
   }

   if (cmPkRgrWarningSiCfgReqInfo(pst, warningSiCfgReqInfo, mBuf) != ROK) 
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGR062, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)warningSiCfgReqInfo, 
                sizeof(RgrWarningSiCfgReqInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }

   if (cmPkRgrCfgTransId(&transId, mBuf) != ROK) 
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR063, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)warningSiCfgReqInfo, 
               sizeof(RgrWarningSiCfgReqInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }

   if (SPkS16(spId, mBuf) != ROK) 
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR064, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)warningSiCfgReqInfo, 
               sizeof(RgrWarningSiCfgReqInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }

   /* if the application wants to retain this structure, the below 
    * code should be removed */
   if (SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)warningSiCfgReqInfo, 
               sizeof(RgrWarningSiCfgReqInfo)) != ROK) 
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGR065, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTRGRWARNINGSICFGREQ;
   return (SPstTsk(pst,mBuf));
}/*cmPkRgrWarningSiCfgReq*/


/***********************************************************
*
*     Func : cmUnpkRgrWarningSiCfgReq
*
*
*     Desc : SI Configuration Request from RRM to MAC for  
*            configuring warning SI
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrWarningSiCfgReq
(
RgrWarningSiCfgReq func,
Pst *pst,
Buffer *mBuf
)
{
   SpId spId;
   RgrCfgTransId transId;
   RgrWarningSiCfgReqInfo *warningSiCfgReqInfo;
   

   if (SUnpkS16(&spId, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR066, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }

   if (cmUnpkRgrCfgTransId(&transId, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR067, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }

   if ((SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&warningSiCfgReqInfo,
            sizeof(RgrWarningSiCfgReqInfo))) != ROK) 
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGR068, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }


   if (pst->selector == ODU_SELECTOR_LC)
   {
      if (cmUnpkRgrWarningSiCfgReqInfo(pst, warningSiCfgReqInfo, mBuf) != ROK) 
      {
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)warningSiCfgReqInfo, 
                          sizeof(RgrWarningSiCfgReqInfo));
         SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ERGR069, (ErrVal)0, "Packing failed");
#endif
         return RFAILED;
      }
   }
   SPutMsg(mBuf);

   return ((*func)(pst, spId, transId, warningSiCfgReqInfo));
} /* cmUnpkRgrWarningSiCfgReq */


/***********************************************************
*
*     Func : cmPkRgrWarningSiCfgReqInfo
*
*
*     Desc : Basic warning SI configuration/reconfiguration 
*            info at RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrWarningSiCfgReqInfo
(
Pst *pst,
RgrWarningSiCfgReqInfo *param,
Buffer *mBuf
)
{
   MsgLen   msgLen;
   uint32_t      count;
   CmLList  *node, *prevNode;
   RgrSegmentInfo   *pdu;
 

   CMCHKPK(oduUnpackUInt8, param->emtcEnable, mBuf);

   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(oduUnpackUInt8, param->siId, mBuf);

   count = param->siPduLst.count;
   node = param->siPduLst.last;
   while (node) 
   {
      pdu = (RgrSegmentInfo *)node->node;
      prevNode= node->prev;
      if(NULLP != pdu)
      {
         if (SFndLenMsg(pdu->pdu, &msgLen) != ROK)
            return RFAILED;
         if (SCatMsg(mBuf, pdu->pdu, M1M2) != ROK)
            return RFAILED;
         /* The PDU is not Released. This has to be done by 
          * the caller of the packing function  */
         CMCHKPK(cmPkMsgLen, msgLen, mBuf);
         SPutMsg(pdu->pdu);
      }
      cmLListDelFrm(&param->siPduLst, node);
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)node, sizeof(RgrSegmentInfo));
      node= prevNode;
   }
   CMCHKPK(oduUnpackUInt32, count, mBuf);
   return ROK;
}


/***********************************************************
*
*     Func : cmUnpkRgrWarningSiCfgReqInfo
*
*
*     Desc : Unpack warning SI configuration info at SCH 
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrWarningSiCfgReqInfo
(
Pst *pst,
RgrWarningSiCfgReqInfo *param,
Buffer *mBuf
)
{
   MsgLen   msgLen, totalMsgLen;
   uint32_t      count, loopCnt;
   Buffer   *pdu;
   CmLList  *node;


   cmLListInit(&param->siPduLst);
   CMCHKUNPK(oduPackUInt32, &count, mBuf);
   for (loopCnt=0; loopCnt<count; loopCnt++) 
   {
      if(SFndLenMsg(mBuf, &msgLen) != ROK)
      {
         return RFAILED;
      } 

      if(msgLen > 0)
      { 
         CMCHKUNPK(cmUnpkMsgLen, &msgLen, mBuf);
         if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
         { 
            return RFAILED;
         }   
         if (SSegMsg(mBuf, totalMsgLen-msgLen, &pdu) != ROK)
         {		 
            return RFAILED;
         }   
      }
      else
      {
         return RFAILED;
      }
      if ((SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&node,
                  sizeof(CmLList))) != ROK)
         return RFAILED;
      node->node = (PTR)pdu;
      cmLListAdd2Tail(&param->siPduLst, node);
   }

   CMCHKUNPK(oduPackUInt8, &param->siId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKPK(oduPackUInt8, &param->emtcEnable, mBuf);

   return ROK;
}


/***********************************************************
*
*    Func : cmPkRgrWarningSiStopReq
*
*    Desc : To stop the broadcast for SIB10, SIB11 and SIB12.
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
**********************************************************/
S16 cmPkRgrWarningSiStopReq
(
Pst           *pst,
SpId          spId,
RgrCfgTransId transId,
uint8_t            siId
)
{
   Buffer *mBuf = NULLP;


   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGR070, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }

   CMCHKPK(oduUnpackUInt8, siId, mBuf);

 
   if (cmPkRgrCfgTransId(&transId, mBuf) != ROK) 
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR082, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }


   if (SPkS16(spId, mBuf) != ROK) 
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGR071, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTRGRWARNINGSISTOPREQ;
   return (SPstTsk(pst,mBuf));
}/*cmPkRgrWarningSiStopReq */


/***********************************************************
*
*    Func : cmUnpkRgrWarningSiStopReq
*
*    Desc : To stop the broadcast for SIB10, SIB11 and SIB12.
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
**********************************************************/
S16 cmUnpkRgrWarningSiStopReq
(
RgrWarningSiStopReq func,
Pst *pst,
Buffer *mBuf
)
{
   SpId          spId;
   uint8_t            siId;
   RgrCfgTransId transId;


  if (SUnpkS16(&spId, mBuf) != ROK)
  {
     SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
     SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
           __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
           (ErrVal)ERGR072, (ErrVal)0, "Packing failed");
#endif
     return RFAILED;
  }

    if (cmUnpkRgrCfgTransId(&transId, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR083, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
 
   CMCHKUNPK(oduPackUInt8, &siId, mBuf);

   SPutMsg(mBuf);

   return ((*func)(pst, spId, transId, siId));
} /*cmUnpkRgrWarningSiStopReq */


/***********************************************************
*
*     Func : cmPkRgrWarningSiCfgCfm
*
*
*     Desc : Warning SI Configuration Confirm from MAC to RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrWarningSiCfgCfm
(
Pst*          pst,
SuId          suId,
RgrCfgTransId transId,
uint8_t            siId,
uint8_t            status
)
{
   Buffer *mBuf = NULLP;


   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR073, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }

   if (oduUnpackUInt8(status, mBuf) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR074, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }

   if (oduUnpackUInt8(siId, mBuf) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR075, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }

   if (cmPkRgrCfgTransId(&transId, mBuf) != ROK) 
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR076, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }

   if (SPkS16(suId, mBuf) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR077, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTRGRWARNINGSICFGCFM;
   return (SPstTsk(pst,mBuf));
} /* cmPkRgrWarningSiCfgCfm */


/***********************************************************
*
*     Func : cmUnpkRgrwarningSiCfgCfm
*
*
*     Desc : Warning SI Configuration Confirm from MAC to RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrWarningSiCfgCfm
(
RgrWarningSiCfgCfm func,
Pst *pst,
Buffer *mBuf
)
{
   SuId          suId;
   uint8_t            siId;
   RgrCfgTransId transId;
   uint8_t            status;


   if (SUnpkS16(&suId, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGR078, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }

   if (cmUnpkRgrCfgTransId(&transId, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGR079, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }

   if (oduPackUInt8(&siId, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGR080, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }

   if (oduPackUInt8(&status, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGR081, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, suId, transId, siId, status));
}/* cmUnpkRgrwarningSiCfgCfm */
#endif/*RGR_SI_SCH*/

/* LTE_ADV_FLAG_REMOVED_START */
/***********************************************************
*
*     Func : cmPkRgrLoadInfReq
*
*
*     Desc : LOAD INF Configuration Request from RRM to MAC for
*            configuring RNTP, ABS etc
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  :
*
**********************************************************/
S16 cmPkRgrLoadInfReq
(
Pst* pst,
SpId spId,
RgrCfgTransId transId,
RgrLoadInfReqInfo * loadInfReq
)
{
   Buffer *mBuf = NULLP;


   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR045, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)loadInfReq, 
                 sizeof(RgrLoadInfReqInfo));
      return RFAILED;
   }
   if (cmPkRgrLoadInfReqInfo(loadInfReq, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGR046, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)loadInfReq, 
                sizeof(RgrLoadInfReqInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (cmPkRgrCfgTransId(&transId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR047, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)loadInfReq, 
               sizeof(RgrLoadInfReqInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR048, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)loadInfReq, 
               sizeof(RgrLoadInfReqInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)loadInfReq, 
               sizeof(RgrLoadInfReqInfo)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGR049, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTRGRLOADINFREQ;
   return (SPstTsk(pst,mBuf));
}

/***********************************************************
*
*     Func : cmUnpkRgrLoadInfReq
*
*
*     Desc : LOAD INF Configuration Request from RRM to MAC for 
*     configuring RNTP, ABS etc
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrLoadInfReq
(
RgrLoadInfReq func,
Pst *pst,
Buffer *mBuf
)
{
   SpId spId;
   RgrCfgTransId transId;
   RgrLoadInfReqInfo *loadInfReq;
   

   if (SUnpkS16(&spId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR050, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }

   if (cmUnpkRgrCfgTransId(&transId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR051, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }

   if ((SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&loadInfReq,
            sizeof(RgrLoadInfReqInfo))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGR052, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }

   memset(loadInfReq, 0, sizeof(RgrLoadInfReqInfo));

   if (pst->selector == ODU_SELECTOR_LC) 
      if (cmUnpkRgrLoadInfReqInfo(loadInfReq, mBuf) != ROK) {
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)loadInfReq, 
                          sizeof(RgrLoadInfReqInfo));
         SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ERGR053, (ErrVal)0, "Packing failed");
#endif
         return RFAILED;
      }
   SPutMsg(mBuf);

   return ((*func)(pst, spId, transId, loadInfReq));
}

/***********************************************************
*
*     Func : cmPkRgrLoadInfReqInfo
*
*
*     Desc : Basic LOAD INF configuration/reconfiguration info at RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrLoadInfReqInfo
(
RgrLoadInfReqInfo *param,
Buffer *mBuf
)
{

   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(oduUnpackUInt8, param->rgrCcPHighStartRb, mBuf);
   CMCHKPK(oduUnpackUInt8, param->rgrCcPHighEndRb, mBuf);

   return ROK;
}

/***********************************************************
*
*     Func : cmUnpkRgrLoadInfReqInfo
*
*
*     Desc : Basic LOAD INF configuration/reconfiguration info at RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrLoadInfReqInfo
(
RgrLoadInfReqInfo *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt8, &param->rgrCcPHighEndRb, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->rgrCcPHighStartRb, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);

   return ROK;
}
/* LTE_ADV_FLAG_REMOVED_END */

/*rgr_c_001.main_9 ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT

/***********************************************************
*
*     Func : cmPkRgrUeCqiReptCfg
*
*
*     Desc : Transaction ID between MAC and RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeCqiReptCfg
(
RgrUeCqiReptCfg *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt8, param->numColltdCqiRept, mBuf);

   return ROK;
}


/*rgr_c_001.main_9 ADD added changes for CQI management*/
/***********************************************************
*
*     Func : cmUnpkRgrUeCqiReptCfg
*
*
*     Desc : Transaction ID between MAC and RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUeCqiReptCfg
(
RgrUeCqiReptCfg *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt8, &param->numColltdCqiRept, mBuf);

   return ROK;
}


/*rgr_c_001.main_9 ADD added changes for CQI management*/
/***********************************************************
*
*     Func : cmPkRgrStaInd
*
*
*     Desc : Packing Sta Indication sent to RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrStaInd
(
Pst* pst,
SuId suId,
RgrStaIndInfo* staInd
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR054, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)staInd, 
               sizeof(RgrStaIndInfo));
      return RFAILED;
   }

   if (cmPkRgrStaIndInfo(staInd, mBuf) != ROK) 
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGR055, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)staInd, sizeof(RgrStaIndInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }
   
   if (SPkS16(suId, mBuf) != ROK) 
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR056, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)staInd, 
                  sizeof(RgrStaIndInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }

   if (SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)staInd, 
                  sizeof(RgrStaIndInfo)) != ROK) 
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR057, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTRGRSTAIND;
   return (SPstTsk(pst,mBuf));
}

/*rgr_c_001.main_9 ADD added changes for CQI management*/
/***********************************************************
*
*     Func : cmUnpkRgrStaInd
*
*
*     Desc : Unpacking Sta Indication sent to RRM
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrStaInd
(
RgrStaInd func,
Pst *pst,
Buffer *mBuf
)
{
   SuId suId;
   RgrStaIndInfo *staInd;


   if (SUnpkS16(&suId, mBuf) != ROK)
   {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR058, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }

   if ((SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&staInd,
                 sizeof(RgrStaIndInfo))) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGR059, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
   }

   if (cmUnpkRgrStaIndInfo(staInd, mBuf) != ROK)
   {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)staInd,
             sizeof(RgrStaIndInfo));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR060, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }

   SPutMsg(mBuf);
   return ((*func)(pst, suId, staInd));
}

/*rgr_c_001.main_9 ADD added changes for CQI management*/
/***********************************************************
*
*     Func : cmPkRgrStaIndInfo
*
*
*     Desc : Packing of StaIndInfo structure 
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrStaIndInfo
(
RgrStaIndInfo *param,
Buffer *mBuf
)
{


   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->crnti, mBuf);
   CMCHKPK(cmPkRgrUeCqiInfo, &param->ueCqiInfo, mBuf);

   return ROK;
}
/*rgr_c_001.main_9 ADD added changes for CQI management*/
/***********************************************************
*
*     Func : cmUnpkRgrStaIndInfo
*
*
*     Desc : Unpacking of StaIndInfo structure 
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrStaIndInfo
(
RgrStaIndInfo *param,
Buffer *mBuf
)
{


   CMCHKUNPK(cmUnpkRgrUeCqiInfo, &param->ueCqiInfo, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->crnti, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);

   return ROK;
}
/*rgr_c_001.main_9 ADD added changes for CQI management*/
/***********************************************************
*
*     Func : cmPkRgrUeCqiInfo
*
*
*     Desc : Packing of StaIndInfo structure 
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeCqiInfo
(
RgrUeCqiInfo *param,
Buffer *mBuf
)
{

   S16 idx;

   for (idx = param->numCqiRept - 1; idx >= 0; idx--)
   {
      CMCHKPK(cmPkRgrUeCqiRept, &param->cqiRept[idx], mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->numCqiRept, mBuf);

   return ROK;
} /* cmPkRgrUeCqiInfo */

/*rgr_c_001.main_9 ADD added changes for CQI management*/
/***********************************************************
*
*     Func : cmUnpkRgrUeCqiInfo
*
*
*     Desc : Packing of StaIndInfo structure 
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUeCqiInfo
(
RgrUeCqiInfo *param,
Buffer *mBuf
)
{

   uint8_t  idx;

   CMCHKUNPK(oduPackUInt8, &param->numCqiRept, mBuf);
   for (idx = 0; idx < param->numCqiRept; idx++)
   {
      CMCHKUNPK(cmUnpkRgrUeCqiRept, &param->cqiRept[idx], mBuf);
   }

   return ROK;
} /* cmUnpkRgrUeCqiInfo */

/***********************************************************
*
*     Func : cmPkRgrSubBandCqiInfo 
*
*
*     Desc : Packing ofcmPkRgrSubBandCqiInfo structure 
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrSubBandCqiInfo 
(
RgrSubBandCqiInfo *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt8, param->cqi[0], mBuf);
   CMCHKPK(oduUnpackUInt8, param->cqi[1], mBuf);
   CMCHKPK(oduUnpackUInt8, param->subBandIdx, mBuf);

   return ROK;
} /* cmPkRgrSubBandCqiInfo */

/*rgr_c_001.main_9 ADD added changes for CQI management*/
/***********************************************************
*
*     Func : cmPkRgrUeCqiRept
*
*
*     Desc : Packing of StaIndInfo structure 
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeCqiRept
(
RgrUeCqiRept *param,
Buffer *mBuf
)
{
   S8 idx;
   uint8_t count;


   CMCHKPK(oduUnpackUInt8, param->cqi[0], mBuf);
   CMCHKPK(oduUnpackUInt8, param->cqi[1], mBuf);
   CMCHKPK(oduUnpackUInt8, param->cqiMode, mBuf);
   for (idx = param->numSubBand - 1; idx >= 0; idx--)
   {
      count = idx;
      CMCHKPK(cmPkRgrSubBandCqiInfo, &param->sbCqiInfo[count], mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->numSubBand, mBuf);
   return ROK;
} /* cmPkRgrUeCqiRept */

/***********************************************************
*
*     Func :cmUnpkRgrSubBandCqiInfo 
*
*
*     Desc : Packing of cmUnpkRgrSubBandCqiInfo structure 
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrSubBandCqiInfo 
(
RgrSubBandCqiInfo *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->subBandIdx, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->cqi[1], mBuf);
   CMCHKUNPK(oduPackUInt8, &param->cqi[0], mBuf);

   return ROK;
} /* cmUnpkRgrSubBandCqiInfo*/

/*rgr_c_001.main_9 ADD added changes for CQI management*/
/***********************************************************
*
*     Func : cmUnpkRgrUeCqiRept
*
*
*     Desc : Packing of StaIndInfo structure 
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUeCqiRept
(
RgrUeCqiRept *param,
Buffer *mBuf
)
{
   uint8_t idx;

   CMCHKUNPK(oduPackUInt8, &param->numSubBand, mBuf);
   for (idx = 0; idx < param->numSubBand; idx++)
   {
      CMCHKUNPK(cmUnpkRgrSubBandCqiInfo, &param->sbCqiInfo[idx], mBuf);
   }
   CMCHKUNPK(oduPackUInt8, &param->cqiMode, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->cqi[1], mBuf);
   CMCHKUNPK(oduPackUInt8, &param->cqi[0], mBuf);

   return ROK;
} /* cmUnpkRgrUeCqiRept */
#endif /* End of ifdef RGR_CQI_REPT */

/* LTE_ADV_FLAG_REMOVED_START */
/***********************************************************
*
*     Func : cmPkRgrLoadInfInd
*
*
*     Desc : Packing LOAD INFORMATION Indication sent to RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  :
*
**********************************************************/
S16 cmPkRgrLoadInfInd
(
Pst* pst,
SuId suId,
RgrLoadInfIndInfo* loadInfInd
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGR054, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)loadInfInd,
            sizeof(RgrLoadInfIndInfo));
      return RFAILED;
   }

   if (cmPkRgrLoadInfIndInfo(loadInfInd, mBuf) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGR055, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)loadInfInd,
            sizeof(RgrLoadInfIndInfo));
      SPutMsg(mBuf);

      return RFAILED;
   }

   if (SPkS16(suId, mBuf) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGR056, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)loadInfInd,
            sizeof(RgrLoadInfIndInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }

   if (SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)loadInfInd,
                  sizeof(RgrLoadInfIndInfo)) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGR057, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTRGRLOADINFIND;
   return (SPstTsk(pst,mBuf));
}

/***********************************************************
*
*     Func : cmUnpkRgrLoadInfInd
*
*
*     Desc : Unpacking LOAD INFORMATION Indication sent to RRM
*
*     Ret  : S16
*
*     Notes:
*
*     File  :
*
**********************************************************/
S16 cmUnpkRgrLoadInfInd
(
RgrLoadInfInd func,
Pst *pst,
Buffer *mBuf
)
{
   SuId suId;
   RgrLoadInfIndInfo *loadInfInd = NULLP; /* dsfr_pal_fixes ** 21-March-2013 ** SKS */


   if (SUnpkS16(&suId, mBuf) != ROK)
   {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR058, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }

   if ((SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&loadInfInd,
                 sizeof(RgrLoadInfIndInfo))) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGR059, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
   }

   /* dsfr_pal_fixes ** 22-March-2013 ** SKS */
   if (cmUnpkRgrLoadInfIndInfo(loadInfInd, pst, mBuf) != ROK)
   {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)loadInfInd,
             sizeof(RgrLoadInfIndInfo));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR060, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }

   SPutMsg(mBuf);
   return ((*func)(pst, suId, loadInfInd));
}


/***********************************************************
*
*     Func : cmPkRgrLoadInfIndInfo
*
*
*     Desc : Packing of LoadInfIndInfo structure
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrLoadInfIndInfo
(
RgrLoadInfIndInfo *param,
Buffer *mBuf
)
{
   uint8_t                     idx;

   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(oduUnpackUInt16, param->bw, mBuf);
   if(RGR_ABS == param->type)
   {
      for(idx = 0; idx < RGR_ABS_PATTERN_LEN; idx++)
      {
         CMCHKPK(oduUnpackUInt32, param->u.absLoadInfo[idx], mBuf);
      }
   }
   else
   {
      CMCHKPK(cmPkTknStrOSXL, &param->u.rntpInfo, mBuf);
   }

   CMCHKPK(oduUnpackUInt32, param->type, mBuf);

   return ROK;
}

/***********************************************************
*
*     Func : cmUnpkRgrLoadInfIndInfo
*
*
*     Desc : Unpacking of LoadInfIndInfo structure
*
*     Ret  : S16
*
*     Notes:
*
*     File  :
*
**********************************************************/
S16 cmUnpkRgrLoadInfIndInfo
(
RgrLoadInfIndInfo *param,
Pst *pst,
Buffer *mBuf
)
{
   S8          idx;

   /* dsfr_pal_fixes ** 21-March-2013 ** SKS */
   TknStrOSXL  *tknStr;
   uint16_t         ndx;


   CMCHKUNPK(oduPackUInt32, &param->type, mBuf);
   if(RGR_ABS == param->type)
   {
      for(idx = RGR_ABS_PATTERN_LEN -1 ; idx >= 0; idx--)
      {
         CMCHKUNPK(oduPackUInt32, &param->u.absLoadInfo[(uint8_t)idx], mBuf);
      }
   }
   else
   {

      /* dsfr_pal_fixes ** 21-March-2013 ** SKS ** Start */
      tknStr = &param->u.rntpInfo;

      CMCHKUNPK(oduPackUInt8, &tknStr->pres, mBuf);

      if(tknStr->pres)
      {
         /* Length */
         CMCHKUNPK(oduPackUInt16, &tknStr->len, mBuf);

         if ((SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&tknStr->val,tknStr->len)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
            SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ERGR052, (ErrVal)0, "Packing failed");
#endif
            SPutMsg(mBuf);
            return RFAILED;
         }

         /* Value */
         for (ndx = 1; ndx <= tknStr->len; ndx++)
         {
            CMCHKUNPK(oduPackUInt8, &tknStr->val[tknStr->len - ndx], mBuf);
         }
      }
   }
   /* dsfr_pal_fixes ** 21-March-2013 ** SKS ** End */

   /* dsfr_pal_fixes ** 22-March-2013 ** SKS */
   CMCHKUNPK(oduPackUInt16, &param->bw, mBuf);

   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);

   return ROK;
}
/* LTE_ADV_FLAG_REMOVED_END */

#ifdef TFU_UPGRADE 
/*rgr_c_001.main_9 ADD added changes for CQI management*/
/***********************************************************
*
*     Func : cmPkRgrUepACfg
*
*
*     Desc : Transaction ID between MAC and RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUepACfg
(
RgrUepACfg *param,
Buffer *mBuf
)
{

   if(param->pAPrsnt)
   {
      CMCHKPK(oduUnpackUInt32, param->pA, mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->pAPrsnt, mBuf);

   return ROK;
}


/*rgr_c_001.main_9 ADD added changes for CQI management*/
/***********************************************************
*
*     Func : cmUnpkRgrUepACfg
*
*
*     Desc : Transaction ID between MAC and RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUepACfg
(
RgrUepACfg *param,
Buffer *mBuf
)
{
   uint32_t tmpEnum;

   CMCHKUNPK(oduPackUInt8, (uint8_t *)&param->pAPrsnt, mBuf);
   if (param->pAPrsnt)
   {
      CMCHKUNPK(oduPackUInt32, (uint32_t*)&tmpEnum, mBuf);
      param->pA = (RgrUeDlPwrCntrlPaCfg) tmpEnum;
   }

   return ROK;
}

/*rgr_c_001.main_9 ADD PDSCH ded cfg*/
/***********************************************************
*
*     Func : cmPkRgrUePdschDedCfg
*
*
*     Desc : Transaction ID between MAC and RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUePdschDedCfg
(
RgrUePdschDedCfg *param,
Buffer *mBuf
)
{

   CMCHKPK(cmPkRgrUepACfg, &param->uepACfg, mBuf);

   return ROK;
}


/*rgr_c_001.main_9 ADD PDSCH ded cfg*/
/***********************************************************
*
*     Func : cmUnpkRgrUePdschDedCfg
*
*
*     Desc : Transaction ID between MAC and RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUePdschDedCfg
(
RgrUePdschDedCfg *param,
Buffer *mBuf
)
{

   CMCHKUNPK(cmUnpkRgrUepACfg, &param->uepACfg, mBuf);

   return ROK;
}
#endif


#ifdef LTE_ADV 

/***********************************************************
*
*     Func : cmPkRgrUeUlPwrDedSCellCfg
*
*
*     Desc : DL Secondary Cell UL PC Info
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
static S16 cmPkRgrUeUlPwrDedSCellCfg
(
RgrUeUlPwrDedSCellCfg *param,
Buffer *mBuf
)
{
   CMCHKPK(oduUnpackUInt8, param->pSRSOffset, mBuf);
   CMCHKPK(SPkS8, param->p0UePusch, mBuf);
   CMCHKPK(oduUnpackUInt8, param->isDeltaMCSEnabled, mBuf);
   CMCHKPK(oduUnpackUInt8, param->isAccumulated, mBuf);

   return ROK;
}

/***********************************************************
*
*     Func : cmUnpkRgrUeUlPwrDedSCellCfg
*
*
*     Desc : Uplink power configuration per UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
static S16 cmUnpkRgrUeUlPwrDedSCellCfg
(
RgrUeUlPwrDedSCellCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->isAccumulated, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->isDeltaMCSEnabled, mBuf);
   CMCHKUNPK(SUnpkS8, &param->p0UePusch, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->pSRSOffset, mBuf);
   return ROK;
}




/***********************************************************
*
*     Func : cmPkRgrUeSecCellCfg
*
*
*     Desc : DL Secondary Cell information of the UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
static S16 cmPkRgrUeSecCellCfg
(
RgrUeSecCellCfg *param,
Buffer *mBuf
)
{
   CMCHKPK(cmPkRgrUeUlPwrDedSCellCfg,&param->ueSCellUlDedPwrCfg, mBuf);
   CMCHKPK(oduUnpackUInt8, param->isUlCaEnabled, mBuf);
   CMCHKPK(cmPkRgrUeTxModeCfg, &param->txMode, mBuf);
#ifdef TFU_UPGRADE
/*rgr_c_001.main_9 ADD PDSCH ded cfg*/
   CMCHKPK(cmPkRgrUePdschDedCfg, &param->uePdschDedCfg, mBuf);
#endif
   CMCHKPK(cmPkRgrUeDlCqiCfg,&param->ueSCellDlCqiCfg, mBuf);
   CMCHKPK(cmPkTknUInt32, &param->sCellDeActTmr, mBuf);
   CMCHKPK(oduUnpackUInt16, param->sCellId, mBuf);
   CMCHKPK(oduUnpackUInt8, param->sCellIdx, mBuf);

   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgrUeSecCellInfo
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
S16 cmPkRgrUeSecCellInfo
(
RgrUeSecCellInfo *param,
Buffer *mBuf
)
{
   S8 idx;
   for(idx = param->numSCells - 1; idx >= 0; idx--)
   {
      CMCHKPK(cmPkRgrUeSecCellCfg, &param->ueSCellDedCfg[(uint8_t)idx],mBuf);
   }

   CMCHKPK(oduUnpackUInt8, param->numSCells, mBuf);

   CMCHKPK(oduUnpackUInt8, param->useExtBSRSizes, mBuf);

   return ROK;
}


/***********************************************************
*
*     Func : cmUnpkRgrUeSecCellCfg
*
*
*     Desc : DL Secondary Cell information of the UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
static S16 cmUnpkRgrUeSecCellCfg
(
RgrUeSecCellCfg *param,
Buffer *mBuf
)
{
   CMCHKUNPK(oduPackUInt8, &param->sCellIdx, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->sCellId, mBuf);
   CMCHKUNPK(cmUnpkTknUInt32, &param->sCellDeActTmr, mBuf);
   CMCHKUNPK(cmUnpkRgrUeDlCqiCfg, &param->ueSCellDlCqiCfg, mBuf);
#ifdef TFU_UPGRADE
   CMCHKUNPK(cmUnpkRgrUePdschDedCfg, &param->uePdschDedCfg, mBuf);
#endif
   CMCHKUNPK(cmUnpkRgrUeTxModeCfg, &param->txMode, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->isUlCaEnabled, mBuf);
   CMCHKUNPK(cmUnpkRgrUeUlPwrDedSCellCfg, &param->ueSCellUlDedPwrCfg, mBuf);

   return ROK;
}




/***********************************************************
*
*     Func : cmUnpkRgrUeSecCellInfo
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
S16 cmUnpkRgrUeSecCellInfo
(
RgrUeSecCellInfo *param,
Buffer *mBuf
)
{
   uint8_t idx;

   CMCHKUNPK(oduPackUInt8, &param->useExtBSRSizes, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->numSCells, mBuf);

   for(idx = 0; idx < param->numSCells;idx++)
   {
      CMCHKUNPK(cmUnpkRgrUeSecCellCfg, &param->ueSCellDedCfg[idx],mBuf);
   }

   return ROK;
}
/* Sprint 3 */
/***********************************************************
*
*     Func : cmPkRgrUeSCellAckPucchCfg
*
*
*     Desc : PUCCH Configuration information of the UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeSCellAckPucchCfg
(
RgrUeSCellAckPucchCfg *param,
Buffer *mBuf
)
{
   S8 idx;

   if (param->pucchFormatType == RG_SCH_UCI_FORMAT1B_CS || param->pucchFormatType == RG_SCH_UCI_FORMAT1A_1B)
   { 
      for(idx = param->u.format1Bcs.sCellAckN1ResTb2Count - 1; idx >=0 ;idx--)
      {
         CMCHKPK(oduUnpackUInt16, param->u.format1Bcs.sCellAckN1ResTb2[(uint8_t)idx], mBuf);
      }

      CMCHKPK(oduUnpackUInt8, param->u.format1Bcs.sCellAckN1ResTb2Count, mBuf);

      for(idx = param->u.format1Bcs.sCellAckN1ResTb1Count - 1; idx >=0 ;idx--)
      {
         CMCHKPK(oduUnpackUInt16, param->u.format1Bcs.sCellAckN1ResTb1[(uint8_t)idx], mBuf);
      }

      CMCHKPK(oduUnpackUInt8, param->u.format1Bcs.sCellAckN1ResTb1Count, mBuf);
   }
   else
   {
      for(idx = param->u.format3.sCellAckN3ResAntP1Count - 1; idx >=0 ;idx--)
      {
         CMCHKPK(oduUnpackUInt16, param->u.format3.sCellAckN3ResAntP1[(uint8_t)idx], mBuf);
      }

      CMCHKPK(oduUnpackUInt8, param->u.format3.sCellAckN3ResAntP1Count, mBuf);

      for(idx = param->u.format3.sCellAckN3ResAntP0Count- 1; idx >=0 ;idx--)
      {
         CMCHKPK(oduUnpackUInt16, param->u.format3.sCellAckN3ResAntP0[(uint8_t)idx], mBuf);
      }

      CMCHKPK(oduUnpackUInt8, param->u.format3.sCellAckN3ResAntP0Count, mBuf);
   }
   CMCHKPK(oduUnpackUInt32, param->pucchFormatType, mBuf);

   return ROK;
}

/***********************************************************
*
*     Func : cmUnpkRgrUeSCellAckPucchCfg
*
*
*     Desc : PUCCH Configuration information of the UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUeSCellAckPucchCfg
(
RgrUeSCellAckPucchCfg *param,
Buffer *mBuf
)
{
   uint8_t idx;


   CMCHKUNPK(oduPackUInt32, (uint32_t*)&param->pucchFormatType, mBuf);
   //if (param->pucchFormatType == RG_SCH_UCI_FORMAT1B_CS)
   if (param->pucchFormatType == RG_SCH_UCI_FORMAT1B_CS || param->pucchFormatType == RG_SCH_UCI_FORMAT1A_1B)
   {
      CMCHKUNPK(oduPackUInt8, &param->u.format1Bcs.sCellAckN1ResTb1Count, mBuf);

      for(idx = 0; idx < param->u.format1Bcs.sCellAckN1ResTb1Count; idx++)
      {
         CMCHKUNPK(oduPackUInt16, &param->u.format1Bcs.sCellAckN1ResTb1[idx], mBuf);
      }

      CMCHKUNPK(oduPackUInt8, &param->u.format1Bcs.sCellAckN1ResTb2Count, mBuf);

      for(idx = 0; idx < param->u.format1Bcs.sCellAckN1ResTb2Count; idx++)
      {
         CMCHKUNPK(oduPackUInt16, &param->u.format1Bcs.sCellAckN1ResTb2[idx], mBuf);
      }
   }
   else
   {
      CMCHKUNPK(oduPackUInt8, &param->u.format3.sCellAckN3ResAntP0Count, mBuf);

      for(idx = 0; idx < param->u.format3.sCellAckN3ResAntP0Count; idx++)
      {
         CMCHKUNPK(oduPackUInt16, &param->u.format3.sCellAckN3ResAntP0[idx], mBuf);
      }

      CMCHKUNPK(oduPackUInt8, &param->u.format3.sCellAckN3ResAntP1Count, mBuf);

      for(idx = 0; idx < param->u.format3.sCellAckN3ResAntP1Count; idx++)
      {
         CMCHKUNPK(oduPackUInt16, &param->u.format3.sCellAckN3ResAntP1[idx], mBuf);
      }

   }
   return ROK;
}




#endif


/***********************************************************
*
*     Func : cmPkRgrUeStaInd
*
*
*     Desc : Packing Ue Sta Indication sent to RRM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeStaInd
(
Pst* pst,
SuId suId,
RgrUeStaIndInfo* ueStaInd
)
{
   Buffer *mBuf = NULLP;
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR054, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ueStaInd, 
               sizeof(RgrUeStaIndInfo));
      return RFAILED;
   }

   if (cmPkRgrUeStaIndInfo(ueStaInd, mBuf) != ROK) 
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGR055, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ueStaInd, 
               sizeof(RgrUeStaIndInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }
   
   if (SPkS16(suId, mBuf) != ROK) 
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR056, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ueStaInd, 
                  sizeof(RgrUeStaIndInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }

   if (SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ueStaInd, 
                  sizeof(RgrUeStaIndInfo)) != ROK) 
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR057, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTRGRUESTAIND;
   return (SPstTsk(pst,mBuf));
}

/*rgr_c_001.main_9 ADD added changes for CQI management*/
/***********************************************************
*
*     Func : cmUnpkRgrUeStaInd
*
*
*     Desc : Unpacking Sta Indication sent to RRM
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUeStaInd
(
RgrUeStaInd  func,
Pst *pst,
Buffer *mBuf
)
{
   SuId suId;
   RgrUeStaIndInfo *ueStaInd;


   if (SUnpkS16(&suId, mBuf) != ROK)
   {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR058, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }

   if ((SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&ueStaInd,
                 sizeof(RgrUeStaIndInfo))) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGR059, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
   }

   if (cmUnpkRgrUeStaIndInfo(ueStaInd, mBuf) != ROK)
   {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ueStaInd,
             sizeof(RgrUeStaIndInfo));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGR060, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }

   SPutMsg(mBuf);
   return ((*func)(pst, suId, ueStaInd));
}

/***********************************************************
*
*     Func : cmPkRgrUeStaIndInfo
*
*
*     Desc : Packing of UeStaIndInfo structure 
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrUeStaIndInfo
(
RgrUeStaIndInfo *param,
Buffer *mBuf
)
{


   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->crnti, mBuf);
   CMCHKPK(oduUnpackUInt8, param->status, mBuf);

   return ROK;
}
/***********************************************************
*
*     Func : cmUnpkRgrUeStaIndInfo
*
*
*     Desc : Unpacking of UeStaIndInfo structure 
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrUeStaIndInfo
(
RgrUeStaIndInfo *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->status, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->crnti, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);

   return ROK;
}


/***********************************************************
*
*     Func : cmPkRgrCellCsgParamCfg 
*
*
*     Desc : CSG Parameter configuration
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrCellCsgParamCfg 
(
RgrCellCsgParamCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt8, param->minUlResNonCsg, mBuf);
   CMCHKPK(oduUnpackUInt8, param->minDlResNonCsg, mBuf);

   return ROK;
}

/***********************************************************
*
*     Func : cmUnpkRgrCellCsgParamCfg
*
*
*     Desc : Unpacking of RgrCellCsgParamCfg structure 
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrCellCsgParamCfg
(
RgrCellCsgParamCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->minDlResNonCsg, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->minUlResNonCsg, mBuf);

   return ROK;
}

/***********************************************************
*
*     Func : cmPkRgrCellCntrlCmdCfg
*
*
*     Desc : Packing of RgrCellCntrlCmdCfg structure 
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgrCellCntrlCmdCfg
(
RgrCellCntrlCmdCfg *param,
Buffer *mBuf
)
{

   switch(param->cmdType)
   {
      case RGR_CNTRL_CMD_RACH_OVRLD:
         CMCHKUNPK(oduUnpackUInt8, param->cmdDesc.rachOvrLd.backOffVal, mBuf);
         CMCHKUNPK(oduUnpackUInt8, param->cmdDesc.rachOvrLd.backOffEnb, mBuf);
         break;
      case RGR_CNTRL_CMD_CPU_OVRLD:
         CMCHKUNPK(oduUnpackUInt8, param->cmdDesc.cpuOvrLd.instruction, mBuf);
         break;
      default:
         break;
   }
   CMCHKUNPK(oduUnpackUInt32, param->cmdType, mBuf);

   return ROK;
}
/***********************************************************
*
*     Func : cmUnpkRgrCellCntrlCmdCfg
*
*
*     Desc : Unpacking of RgrCellCntrlCmdCfg structure 
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgrCellCntrlCmdCfg
(
RgrCellCntrlCmdCfg *param,
Buffer *mBuf
)
{
   uint32_t tmpEnum;


   CMCHKUNPK(oduPackUInt32, &tmpEnum, mBuf);
   param->cmdType = (RgrCellCntrlCmdType) tmpEnum;
   switch(param->cmdType)
   {
      case RGR_CNTRL_CMD_RACH_OVRLD:
         CMCHKUNPK(oduPackUInt8, &param->cmdDesc.rachOvrLd.backOffEnb, mBuf);
         CMCHKUNPK(oduPackUInt8, &param->cmdDesc.rachOvrLd.backOffVal, mBuf);
         break;
      case RGR_CNTRL_CMD_CPU_OVRLD:
         CMCHKUNPK(oduPackUInt8, &param->cmdDesc.cpuOvrLd.instruction, mBuf);
         break;
      default:
         break;
   }

   return ROK;
}
#endif /* End of if defined(LCRGR) */

/**********************************************************************
         End of file
**********************************************************************/
