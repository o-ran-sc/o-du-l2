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
  
     Desc:     C source code for packing/unpacking of RGU interface
               primitives.
  
     File:     rgu.c 
  
**********************************************************************/

/** @file rgu.c
@brief This file contains the packing/unpacking code for the RGU interface 
       primitives.
*/

/* header include files (.h) */
#include "common_def.h"
#include "rgu.h"           /* RGU Interface defines */

/* header/extern include files (.x) */
#include "rgu.x"           /* RGU Interface includes */

#ifdef SS_RBUF
#include "ss_rbuf.h"
#include "ss_rbuf.x"
#endif

#ifdef LCRGU


/**
* @brief Request from RLC to MAC to bind the interface saps
*
* @details
*
*     Function : cmPkRguBndReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   SpId  spId
*  @return   S16
*      -# ROK
**/
S16 cmPkRguBndReq
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
          (ErrVal)ERGU001, (ErrVal)0, "Packing failed");
#endif      
      return RFAILED;
   }
   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU002, (ErrVal)0, "Packing failed");
#endif      
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU003, (ErrVal)0, "Packing failed");
#endif      
      SPutMsg(mBuf);
      return RFAILED;
   }
   pst->event = (Event) EVTRGUBNDREQ;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief Request from RLC to MAC to bind the interface saps
*
* @details
*
*     Function : cmUnpkRguBndReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   SpId  spId
*  @return   S16
*      -# ROK
**/
S16 cmUnpkRguBndReq
(
RguBndReq func,
Pst *pst,
Buffer *mBuf
)
{
   SuId suId;
   SpId spId;

   if (SUnpkS16(&suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU004, (ErrVal)0, "UnPacking failed");
#endif      
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SUnpkS16(&spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU005, (ErrVal)0, "UnPacking failed");
#endif      
      SPutMsg(mBuf);
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, suId, spId));
}


/**
* @brief Request from RLC to MAC to Unbind the interface saps
*
* @details
*
*     Function : cmPkRguUbndReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   Reason  reason
*  @return   S16
*      -# ROK
**/
S16 cmPkRguUbndReq
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
          (ErrVal)ERGU006, (ErrVal)0, "Packing failed");
#endif      
      return RFAILED;
   }
   if (SPkS16(reason, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU007, (ErrVal)0, "Packing failed");
#endif      
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU008, (ErrVal)0, "Packing failed");
#endif      
      SPutMsg(mBuf);
      return RFAILED;
   }
   pst->event = (Event) EVTRGUUBNDREQ;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief Request from RLC to MAC to Unbind the interface saps
*
* @details
*
*     Function : cmUnpkRguUbndReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   Reason  reason
*  @return   S16
*      -# ROK
**/
S16 cmUnpkRguUbndReq
(
RguUbndReq func,
Pst *pst,
Buffer *mBuf
)
{
   SpId spId;
   Reason reason;
   

   if (SUnpkS16(&spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU009, (ErrVal)0, "UnPacking failed");
#endif      
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SUnpkS16(&reason, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU010, (ErrVal)0, "UnPacking failed");
#endif      
      SPutMsg(mBuf);
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, spId, reason));
}


/**
* @brief Confirmation from MAC to RLC for the bind/Unbind 
 * request for the interface saps
*
* @details
*
*     Function : cmPkRguBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   uint8_t  status
*  @return   S16
*      -# ROK
**/
S16 cmPkRguBndCfm
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
          (ErrVal)ERGU011, (ErrVal)0, "Packing failed");
#endif      
      return RFAILED;
   }
   if (oduUnpackUInt8(status, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU012, (ErrVal)0, "Packing failed");
#endif      
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU013, (ErrVal)0, "Packing failed");
#endif      
      SPutMsg(mBuf);
      return RFAILED;
   }
   pst->event = (Event) EVTRGUBNDCFM;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief Confirmation from MAC to RLC for the bind/Unbind 
 * request for the interface saps
*
* @details
*
*     Function : cmUnpkRguBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   uint8_t  status
*  @return   S16
*      -# ROK
**/
S16 cmUnpkRguBndCfm
(
RguBndCfm func,
Pst *pst,
Buffer *mBuf
)
{
   SuId suId;
   uint8_t status;

   if (SUnpkS16(&suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU014, (ErrVal)0, "UnPacking failed");
#endif      
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (oduPackUInt8(&status, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU015, (ErrVal)0, "UnPacking failed");
#endif      
      SPutMsg(mBuf);
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, suId, status));
}

/*rgu_c_001.main_5 - ADD - L2M & R9 Support */
#ifdef LTE_L2_MEAS

/***********************************************************
*
*     Func :cmPkRguL2MUlThrpMeasReqInfo 
*
*
* Status Response from RLC to MAC on UL dedicated logical channel for Uplink
* Scheduled throughput measurement
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRguL2MUlThrpMeasReqInfo 
(
RguL2MUlThrpMeasReqInfo* param,
Buffer *mBuf
)
{
   S32 loop;

   CMCHKPK(oduUnpackUInt8, param->enbMeas, mBuf);
   for (loop=param->numLcId-1; loop >= 0; loop--)
   {
      CMCHKPK(oduUnpackUInt8, param->lcId[loop], mBuf);
   }
   CMCHKPK(cmPkLteLcId, param->numLcId, mBuf);
   CMCHKPK(cmPkLteRnti, param->rnti, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func :cmUnpkRguL2MUlThrpMeasReqInfo 
*
*
* Status Response from RLC to MAC on UL dedicated logical channel for Uplink
* Scheduled throughput measurement
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRguL2MUlThrpMeasReqInfo 
(
RguL2MUlThrpMeasReqInfo *param,
Buffer *mBuf
)
{
   uint8_t loop;

   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->rnti, mBuf);
   CMCHKUNPK(cmUnpkLteLcId, &param->numLcId, mBuf);
   for (loop=0; loop<param->numLcId; loop++)
   {
      CMCHKUNPK(oduPackUInt8, &param->lcId[loop], mBuf);
   }
   CMCHKUNPK(oduPackUInt8, &param->enbMeas, mBuf);
   return ROK;
}

/**
* @brief Primitive invoked from RLC to MAC to 
* inform the On/Off status for Scheduled UL throughput 
* measurment for dedicated channels
*
* @details
*
*     Function :cmPkRguL2MUlThrpMeasReq 
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   RguL2MUlThrpMeasReqInfo*  measReq
*  @return   S16
*      -# ROK
**/
S16 cmPkRguL2MUlThrpMeasReq 
(
Pst* pst,
SpId spId,
RguL2MUlThrpMeasReqInfo* measReq
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU049, (ErrVal)0, "Packing failed");
#endif      
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)measReq, sizeof(RguL2MUlThrpMeasReqInfo));
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LWLC)
   {
      CMCHKPK(oduPackPointer,(PTR) measReq, mBuf);
   }
   else
   {
      if (cmPkRguL2MUlThrpMeasReqInfo(measReq, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
             (ErrVal)ERGU050, (ErrVal)0, "Packing failed");
#endif      
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)measReq, 
                                    sizeof(RguL2MUlThrpMeasReqInfo));
         SPutMsg(mBuf);
         return RFAILED;
      }
      if (SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)measReq, 
                           sizeof(RguL2MUlThrpMeasReqInfo)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
             (ErrVal)ERGU052, (ErrVal)0, "Packing failed");
#endif      
         SPutMsg(mBuf);
         return RFAILED;
      }
      measReq = NULLP;
   }

   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU051, (ErrVal)0, "Packing failed");
#endif      
      if (measReq != NULLP)
      {
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)measReq,
                             sizeof(RguL2MUlThrpMeasReqInfo));
      }
      SPutMsg(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTRGUL2MULTHRPMEASREQ;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief Primitive invoked from RLC to MAC to 
* inform the On/Off status for Scheduled UL throughput 
* measurment for dedicated channels
*
* @details
*
*     Function :cmUnpkRguL2MUlThrpMeasReq 
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   RguL2MUlThrpMeasReqInfo  * measReq 
*  @return   S16
*      -# ROK
**/
S16 cmUnpkRguL2MUlThrpMeasReq 
(
RguL2MUlThrpMeasReq func,
Pst *pst,
Buffer *mBuf
)
{
   SpId spId;
   RguL2MUlThrpMeasReqInfo* measReq;
   

   if (SUnpkS16(&spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU053, (ErrVal)0, "UnPacking failed");
#endif      
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LWLC)
   {
      CMCHKUNPK(oduUnpackPointer,(PTR *) &measReq, mBuf);
   }
   else 
   {
      if ((SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&measReq,
                              sizeof(RguL2MUlThrpMeasReqInfo))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
             (ErrVal)ERGU054, (ErrVal)0, "UnPacking failed");
#endif      
         SPutMsg(mBuf);
         return RFAILED;
      }
      if (cmUnpkRguL2MUlThrpMeasReqInfo(measReq, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
             (ErrVal)ERGU055, (ErrVal)0, "UnPacking failed");
#endif      
         SPutMsg(mBuf);
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)measReq, 
                                sizeof(RguL2MUlThrpMeasReqInfo));
         return RFAILED;
      }
   }
   SPutMsg(mBuf);
   return ((*func)(pst, spId, measReq));
}

#endif


   /*rgu_c_001.main_5 - ADD - L2M Support */
#ifdef LTE_L2_MEAS

/**
* @brief Harq Status Indication from MAC to RLC  
*
* @details
*
*     Function : cmPkRguHqStaInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RguHarqStatusInd  *harqStatusInd
*  @return   S16
*      -# ROK
**/
S16 cmPkRguHqStaInd
(
Pst* pst,
SuId suId,
RguHarqStatusInd  *harqStatusInd
)
{

   Buffer   *mBuf;
   uint8_t  idx;
   RguHarqStatusInd *harqStaInd = NULL;

#ifdef XEON_SPECIFIC_CHANGES
   if (SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&harqStaInd, sizeof(RguHarqStatusInd)) != ROK)
#else      
   if ((SGetStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, 
               (Data **)&harqStaInd, sizeof(RguHarqStatusInd),0)) != ROK)
#endif      
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGU070, (ErrVal)0, "Packing failed");
#endif      
      return RFAILED;
   }

   memcpy(harqStaInd, harqStatusInd, sizeof(RguHarqStatusInd));

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGU070, (ErrVal)0, "Packing failed");
#endif
#ifdef XEON_SPECIFIC_CHANGES
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)harqStaInd, sizeof(RguHarqStatusInd));
#else      
      SPutStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)harqStaInd, sizeof(RguHarqStatusInd), 0);
#endif      
      return RFAILED;
   }

   if (pst->selector == ODU_SELECTOR_LWLC)
   {
      CMCHKPK(oduPackPointer,(PTR) harqStaInd, mBuf); 
   }
   else
   {
      for(idx = 0; idx < harqStaInd->numTbs; idx++)
      {
         CMCHKPK(oduUnpackUInt16, harqStaInd->status[idx], mBuf);
      }
      for(idx = 0; idx < harqStaInd->numTbs; idx++)
      {
         CMCHKPK(oduUnpackUInt32, harqStaInd->tbId[idx], mBuf);
      }
      CMCHKPK(oduUnpackUInt8, harqStaInd->numTbs, mBuf);
      CMCHKPK(cmPkLteRnti, harqStaInd->ueId, mBuf);
      CMCHKPK(cmPkLteCellId, harqStaInd->cellId, mBuf);
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU071, (ErrVal)0, "Packing failed");
#endif
#ifdef XEON_SPECIFIC_CHANGES
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)harqStaInd,
              sizeof(RguHarqStatusInd));
#else      
      SPutStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)harqStaInd, sizeof(RguHarqStatusInd), 0);
#endif      
      SPutMsg(mBuf);
      return RFAILED;
   }
#ifdef XEON_SPECIFIC_CHANGES
   if (SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)harqStaInd,
            sizeof(RguHarqStatusInd)) != ROK) {
#else   
   if (pst->selector != ODU_SELECTOR_LWLC)
   {
      if(SPutStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)harqStaInd, 
               sizeof(RguHarqStatusInd), 0) != ROK)
      {
#endif         
#if (ERRCLASS & ERRCLS_ADD_RES)      
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ERGU072, (ErrVal)0, "Packing failed");
#endif      
         SPutMsg(mBuf);
         return RFAILED;
      }
#ifndef XEON_SPECIFIC_CHANGES      
   }
#endif   

   pst->event = (Event) EVTRGUHQSTAIND;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief Harq Status Indication from MAC to RLC  
*
* @details
*
*     Function : cmUnpkRguHqStaInd
*
*  @param[in]  RguHqStaInd   func
*  @param[in]  Pst           *pst
*  @param[in]  Buffer        *mBuf
*  @return   S16
*      -# ROK
**/
S16 cmUnpkRguHqStaInd
(
RguHqStaInd  func,
Pst *pst,
Buffer *mBuf
)
{
   RguHarqStatusInd    *hqStaInd;  
   SuId    suId;
   uint8_t idx;

   if (SUnpkS16(&suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU073, (ErrVal)0, "UnPacking failed");
#endif      
      SPutMsg(mBuf);
      return RFAILED;
   }
#ifdef XEON_SPECIFIC_CHANGES
   if ((SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&hqStaInd, 
        sizeof(RguHarqStatusInd))) != ROK) {
#else   
   if (pst->selector == ODU_SELECTOR_LWLC)
   {
      CMCHKUNPK(oduUnpackPointer,(PTR *) &hqStaInd, mBuf);
   }  
   else
   {   
      if ((SGetStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, 
                  (Data **)&hqStaInd, sizeof(RguHarqStatusInd),0)) != ROK)
      {
#endif         
#if (ERRCLASS & ERRCLS_ADD_RES)      
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ERGU074, (ErrVal)0, "UnPacking failed");
#endif      
         SPutMsg(mBuf);
         return RFAILED;
      }
      CMCHKUNPK(cmUnpkLteCellId, &hqStaInd->cellId, mBuf);
      CMCHKUNPK(cmUnpkLteRnti, &hqStaInd->ueId, mBuf);
      CMCHKUNPK(oduPackUInt8, &hqStaInd->numTbs, mBuf);
      for(idx = hqStaInd->numTbs; idx > 0; idx--)
      {
         CMCHKUNPK(oduPackUInt32, &hqStaInd->tbId[idx - 1], mBuf);
      }
      for(idx = hqStaInd->numTbs; idx > 0; idx--)
      {
         CMCHKUNPK(oduPackUInt16, &hqStaInd->status[idx - 1], mBuf);
      }
#ifndef XEON_SPECIFIC_CHANGES      
   }
#endif   
   SPutMsg(mBuf);
   (*func)(pst, suId, hqStaInd);
#ifdef XEON_SPECIFIC_CHANGES
   SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)hqStaInd, sizeof(RguHarqStatusInd));
#else   
   SPutStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)hqStaInd, sizeof(RguHarqStatusInd), 0);
#endif   
   return ROK;
}
#endif /* LTE_L2_MEAS */

S16 cmPkRguLcFlowCntrlInfo
(
RguLcFlowCntrlInfo *param,
Buffer             *mBuf
)
{
  CMCHKPK(oduUnpackUInt32, param->maxBo4FlowCtrl, mBuf);
  CMCHKPK(oduUnpackUInt32, param->pktAdmitCnt, mBuf);
  CMCHKPK(cmPkLteLcId, param->lcId, mBuf);

  return ROK;
}

S16 cmPkRguUeFlowCntrlInfo
(
RguUeFlowCntrlInfo *param,
Buffer             *mBuf
)
{
  uint32_t idx;


  for(idx=(param->numLcs - 1); idx >= 0; idx--)
  {
    cmPkRguLcFlowCntrlInfo(&param->lcInfo[idx],mBuf);
  }
  CMCHKPK(oduUnpackUInt32, param->numLcs, mBuf);
  CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
  return ROK;
}

S16 cmPkRguFlowCntrlInfo
(
RguFlowCntrlInd *param, 
Buffer          *mBuf
)
{
  uint32_t idx;

  for (idx=(param->numUes - 1); idx >= 0; idx--)
  {
    cmPkRguUeFlowCntrlInfo(&param->ueFlowCntrlInfo[idx],mBuf);
  }
  CMCHKPK(oduUnpackUInt32, param->numUes, mBuf);
  CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
  return ROK;
}

/**
* @brief Flow Cntrl Indication from MAC to RLC  
 * Informs RLC of the LCs on which flow control
 * to be performed.
*
* @details
*
*     Function : cmPkRguFlowCntrlInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RguFlowCntrlInd *staInd
*  @return   S16
*      -# ROK
**/
S16 cmPkRguFlowCntrlInd
(
Pst* pst,
SuId suId,
RguFlowCntrlInd  *flowCntrlInd
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU076, (ErrVal)0, "Packing failed");
#endif      
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LWLC)
   {
      CMCHKPK(oduPackPointer,(PTR) flowCntrlInd, mBuf);
   }
   else
   {
      if (cmPkRguFlowCntrlInfo(flowCntrlInd, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
           SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ERGU077, (ErrVal)0, "Packing failed");
#endif      
         SPutMsg(mBuf);
         return RFAILED;
      } 
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU078, (ErrVal)0, "Packing failed");
#endif      
      SPutMsg(mBuf);
      return RFAILED;
   }
   
   pst->event = (Event) EVTRGUFLOWCNTRLIND;
   return (SPstTsk(pst,mBuf));
}

S16 cmUnpkRguLcFlowCntrlInfo
(
RguLcFlowCntrlInfo *param,
Buffer           *mBuf
)
{

  CMCHKUNPK(cmUnpkLteLcId, &param->lcId, mBuf);
  CMCHKUNPK(oduPackUInt32, &param->pktAdmitCnt, mBuf);
  CMCHKUNPK(oduPackUInt32, &param->maxBo4FlowCtrl, mBuf);
  
  return ROK;
}
S16 cmUnpkRguUeFlowCntrlInfo
(
RguUeFlowCntrlInfo *param,
Buffer           *mBuf
)
{
  uint32_t idx;
  CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
  CMCHKUNPK(oduPackUInt32, &param->numLcs, mBuf);
  for(idx=0; idx < param->numLcs; idx++)
  {
    cmUnpkRguLcFlowCntrlInfo(&param->lcInfo[idx],mBuf);
  }
  return ROK;
}

S16 cmUnpkRguFlowCntrlInfo
(
RguFlowCntrlInd *param, 
Buffer           *mBuf
)
{
  uint32_t idx; 
  

  CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
  CMCHKUNPK(oduPackUInt32, &param->numUes, mBuf);
  for (idx=0; idx < param->numUes; idx++)
  {
    cmUnpkRguUeFlowCntrlInfo(&param->ueFlowCntrlInfo[idx],mBuf);
  }
  return ROK;
}

/**
* @brief Flow control indication from MAC to RLC  
* Informs RLC of the LCs on which flow control to  
* be started
*
* @details
*
*     Function : cmUnpkRguFlowCntrlInd
*
*  @param[in]   RguFlowCntrlInd func
*  @param[in]   Pst    *pst
*  @param[in]   Buffer *mBuf
*  @return   S16
*      -# ROK
**/
S16 cmUnpkRguFlowCntrlInd
(
RguFlowCntrlIndInfo func,
Pst *pst,
Buffer *mBuf
)
{
   SuId suId;
   RguFlowCntrlInd *flowCntrlInd;

   if (SUnpkS16(&suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU080, (ErrVal)0, "UnPacking failed");
#endif      
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LWLC)
   {
      CMCHKUNPK(oduUnpackPointer,(PTR *) &flowCntrlInd, mBuf);
   }
   else 
   {
      if ((SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&flowCntrlInd, sizeof(RguFlowCntrlInd))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGU081, (ErrVal)0, "UnPacking failed");
#endif      
         SPutMsg(mBuf);
         return RFAILED;
      }
      if (cmUnpkRguFlowCntrlInfo(flowCntrlInd, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
             (ErrVal)ERGU082, (ErrVal)0, "UnPacking failed");
#endif      
         SPutMsg(mBuf);
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)flowCntrlInd, sizeof(RguFlowCntrlInd));
         return RFAILED;
      }
   }
   SPutMsg(mBuf);
   return ((*func)(pst, suId, flowCntrlInd));
}

/***********************************************************
*
*     Func : cmPkRguPduInfo
*
*
*     Desc : RguPduInfo
 * PDU information given on a logical channel
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRguPduInfo
(
RguPduInfo *param,
Buffer *mBuf
)
{
   S32 i;

   for (i=param->numPdu-1; i >= 0; i--) {
     /* rgu_c_001.main_3: ccpu00111328: S16 is renamed as MsgLen */
      MsgLen msgLen = 0;
      if (SFndLenMsg(param->mBuf[i], &msgLen) != ROK)
         return RFAILED;
      if (SCatMsg(mBuf, param->mBuf[i], M1M2) != ROK)
         return RFAILED;
      SPutMsg(param->mBuf[i]);
      CMCHKPK(cmPkMsgLen, msgLen, mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->numPdu, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRguPduInfo
*
*
*     Desc : RguPduInfo
 * PDU information given on a logical channel
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRguPduInfo
(
RguPduInfo *param,
Buffer *mBuf
)
{
   S32 i;


   CMCHKUNPK(oduPackUInt8, &param->numPdu, mBuf);
   for (i=0; i<param->numPdu; i++) {
      MsgLen msgLen, totalMsgLen;
      CMCHKUNPK(cmUnpkMsgLen, &msgLen, mBuf);
      if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
         return RFAILED;
      if (SSegMsg(mBuf, totalMsgLen-msgLen, &param->mBuf[i]) != ROK)
         return RFAILED;
   }
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRguDBoReport
*
*
*     Desc : RguDBoReport
 * Buffer Occupancy Report for dedicated logical channel
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRguDBoReport
(
RguDBoReport *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt32, param->oldestSduArrTime, mBuf);
   CMCHKPK(oduUnpackUInt32, param->staPduBo, mBuf);
#ifdef CCPU_OPT
   CMCHKPK(oduUnpackUInt8, param->staPduPrsnt, mBuf);
   CMCHKPK(oduUnpackUInt16, param->estRlcHdrSz, mBuf);
#endif
   CMCHKPK(SPkS32, param->bo, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRguDBoReport
*
*
*     Desc : RguDBoReport
 * Buffer Occupancy Report for dedicated logical channel
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRguDBoReport
(
RguDBoReport *param,
Buffer *mBuf
)
{


   CMCHKUNPK(SUnpkS32, &param->bo, mBuf);
#ifdef CCPU_OPT
   CMCHKUNPK(oduPackUInt16, &param->estRlcHdrSz, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->staPduPrsnt, mBuf);
#endif
   
   CMCHKUNPK(oduPackUInt32, &param->staPduBo, mBuf);
   CMCHKUNPK(oduPackUInt32, &param->oldestSduArrTime, mBuf);
   return ROK;
}


/***********************************************************
*
*     Func : cmPkRguCDatReqInfo
*
*
*     Desc : RguCDatReqInfo
 * DatReq from RLC to MAC for Common Channels(BCCH, PCCH and CCCH)
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRguCDatReqInfo
(
RguCDatReqInfo *param,
Buffer *mBuf
)
{
   MsgLen msgLen;

   if (param->pdu != NULLP)
   {
      if (SFndLenMsg(param->pdu, &msgLen) != ROK)
         return RFAILED;
      if (SCatMsg(mBuf, param->pdu, M1M2) != ROK)
         return RFAILED;
      SPutMsg(param->pdu);
      CMCHKPK(cmPkMsgLen, msgLen, mBuf);
   }
   switch(param->lcType) {
      case CM_LTE_LCH_CCCH:
         CMCHKPK(cmPkLteRnti, param->u.rnti, mBuf);
         break;
      case CM_LTE_LCH_BCCH:
      case CM_LTE_LCH_PCCH:
         CMCHKPK(cmPkLteTimingInfo, &param->u.timeToTx, mBuf);
#ifdef EMTC_ENABLE
        CMCHKPK(oduUnpackUInt8,param->pnb,mBuf);
#endif
         break;
      default :
         return RFAILED;
   }
   CMCHKPK(oduUnpackUInt32, param->transId, mBuf);
   CMCHKPK(cmPkLteLcType, param->lcType, mBuf);
   CMCHKPK(cmPkLteLcId, param->lcId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRguCDatReqInfo
*
*
*     Desc : RguCDatReqInfo
 * DatReq from RLC to MAC for Common Channels(BCCH, PCCH and CCCH)
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRguCDatReqInfo
(
RguCDatReqInfo *param,
Buffer *mBuf
)
{
   MsgLen msgLen, totalMsgLen;


   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteLcId, &param->lcId, mBuf);
   CMCHKUNPK(cmUnpkLteLcType, &param->lcType, mBuf);
   CMCHKUNPK(oduPackUInt32, &param->transId, mBuf);
   switch(param->lcType) {
      case CM_LTE_LCH_BCCH:
      case CM_LTE_LCH_PCCH:
#ifdef EMTC_ENABLE
         CMCHKUNPK(oduPackUInt8,&param->pnb, mBuf);
#endif
         CMCHKUNPK(cmUnpkLteTimingInfo, &param->u.timeToTx, mBuf);
         break;
      case CM_LTE_LCH_CCCH:
         CMCHKUNPK(cmUnpkLteRnti, &param->u.rnti, mBuf);
         break;
      default :
         return RFAILED;
   }
   SFndLenMsg(mBuf, &msgLen);
   if (msgLen > 0)
   {
      CMCHKUNPK(cmUnpkMsgLen, &msgLen, mBuf);
      if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
         return RFAILED;
      if (SSegMsg(mBuf, totalMsgLen-msgLen, &param->pdu) != ROK)
         return RFAILED;
   }
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRguLchDatReq
*
*
*     Desc : RguLchDatReq
 * DatReq Information of a logical channel
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRguLchDatReq
(
RguLchDatReq *param,
Buffer *mBuf
)
{

#ifdef L2_OPTMZ
   CMCHKPK(oduUnpackUInt8, param->freeBuff, mBuf);
#endif
   CMCHKPK(oduUnpackUInt8, param->setMaxUlPrio, mBuf);
   CMCHKPK(cmPkRguPduInfo, &param->pdu, mBuf);
   CMCHKPK(cmPkRguDBoReport, &param->boReport, mBuf);
   CMCHKPK(cmPkLteLcId, param->lcId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRguLchDatReq
*
*
*     Desc : RguLchDatReq
 * DatReq Information of a logical channel
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRguLchDatReq
(
RguLchDatReq *param,
Buffer *mBuf
)
{

   CMCHKUNPK(cmUnpkLteLcId, &param->lcId, mBuf);
   CMCHKUNPK(cmUnpkRguDBoReport, &param->boReport, mBuf);
   CMCHKUNPK(cmUnpkRguPduInfo, &param->pdu, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->setMaxUlPrio, mBuf);
#ifdef L2_OPTMZ
   CMCHKUNPK(oduPackUInt8, &param->freeBuff, mBuf);
#endif
   return ROK;
}


/***********************************************************
*
*     Func : cmPkRguDatReqTb
*
*
*     Desc : RguDatReqTb
* DatReq from RLC to MAC for dedicated channels of a UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRguDatReqTb
(
RguDatReqTb *param,
Buffer *mBuf
)
{
   S32 i;

   /*rgu_c_001.main_5 - ADD - L2M Support */
#ifdef LTE_L2_MEAS
   CMCHKPK(oduUnpackUInt32, param->tbId, mBuf);
#endif
   for (i=param->nmbLch-1; i >= 0; i--) {
      CMCHKPK(cmPkRguLchDatReq, &param->lchData[i], mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->nmbLch, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRguDatReqTb
*
*
*     Desc : RguDatReqTb
 * DatReq from RLC to MAC for dedicated channels of a UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef LTE_L2_MEAS
S16 cmUnpkRguDatReqTb
(
Pst         *pst, 
RguDatReqTb *param,
Buffer *mBuf
)
#else
S16 cmUnpkRguDatReqTb
(
RguDatReqTb *param,
Buffer *mBuf
)
#endif
{
   S32 i;


   CMCHKUNPK(oduPackUInt8, &param->nmbLch, mBuf);
   for (i=0; i<param->nmbLch; i++) {
      CMCHKUNPK(cmUnpkRguLchDatReq, &param->lchData[i], mBuf);
   }
   /*rgu_c_001.main_5 - ADD - L2M Support */
#ifdef LTE_L2_MEAS
   CMCHKUNPK(oduPackUInt32, &param->tbId, mBuf);
#endif
   return ROK;
}


/***********************************************************
*
*     Func : cmPkRguDDatReqInfo
*
*
*     Desc : RguDDatReqInfo
 * DatReq from RLC to MAC for dedicated channels of a UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
   /*rgu_c_001.main_5 - ADD - L2M Support */
#ifdef LTE_L2_MEAS
S16 cmPkRguDDatReqInfo
(
Pst            *pst,
RguDDatReqInfo *param,
Buffer *mBuf
)
#else
S16 cmPkRguDDatReqInfo
(
RguDDatReqInfo *param,
Buffer *mBuf
)
#endif
{
   S32 i;
   S32 idx;


   for(idx = (param->nmbOfUeGrantPerTti-1); idx >= 0; idx--)
   {
      RguDDatReqPerUe *datReq = &param->datReq[idx];
      for (i=RGU_MAX_TB-1; i >= 0; i--) 
      {
         CMCHKPK(cmPkRguDatReqTb, &datReq->datReqTb[i], mBuf);
      }
      CMCHKPK(oduUnpackUInt8, datReq->nmbOfTbs, mBuf);
      CMCHKPK(oduUnpackUInt32, datReq->transId, mBuf);
      CMCHKPK(cmPkLteRnti, datReq->rnti, mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->nmbOfUeGrantPerTti, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRguDDatReqInfo
*
*
*     Desc : RguDDatReqInfo
 * DatReq from RLC to MAC for dedicated channels of a UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef LTE_L2_MEAS
S16 cmUnpkRguDDatReqInfo
(
Pst            *pst, 
RguDDatReqInfo *param,
Buffer *mBuf
)
#else
S16 cmUnpkRguDDatReqInfo
(
RguDDatReqInfo *param,
Buffer *mBuf
)
#endif
{
   S32 i;
  /* After Merging from 2.1 to 2.2 */
   /*rgu_c_001.main_5 - ADD - L2M Support */
#ifdef LTE_L2_MEAS
   S16 retVal;
#endif
   S32 idx;


  /* After Merging from 2.1 to 2.2 */
   /*rgu_c_001.main_5 - ADD - L2M Support */
#ifdef LTE_L2_MEAS
   retVal = ROK;
#endif
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->nmbOfUeGrantPerTti, mBuf);
   for(idx = 0; idx < param->nmbOfUeGrantPerTti; idx++)
   { 
      RguDDatReqPerUe *datReq = &param->datReq[idx];
      CMCHKUNPK(cmUnpkLteRnti, &datReq->rnti, mBuf);
      CMCHKUNPK(oduPackUInt32, &datReq->transId, mBuf);
      CMCHKUNPK(oduPackUInt8, &datReq->nmbOfTbs, mBuf);
      /* rgu_c_001.main_4 - Changes for MIMO feature addition */
      for (i=0; i<RGU_MAX_TB; i++) 
      {
         /*rgu_c_001.main_5 - ADD - L2M Support */
#ifdef LTE_L2_MEAS
         /* After Merging from 2.1 to 2.2 */
         retVal = cmUnpkRguDatReqTb(pst, &datReq->datReqTb[i], mBuf);
         if(retVal != ROK)
         {
            return RFAILED;
         }
#else
         CMCHKUNPK(cmUnpkRguDatReqTb, &datReq->datReqTb[i], mBuf);
#endif
      }
   }
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRguCDatIndInfo
*
*
*     Desc : RguCDatIndInfo
 * DatInd from MAC to RLC on CCCH
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRguCDatIndInfo
(
RguCDatIndInfo *param,
Buffer *mBuf
)
{
   MsgLen msgLen;

   if (SFndLenMsg(param->pdu, &msgLen) != ROK)
      return RFAILED;
   if (SCatMsg(mBuf, param->pdu, M1M2) != ROK)
      return RFAILED;
   SPutMsg(param->pdu);
   CMCHKPK(cmPkMsgLen, msgLen, mBuf);
   CMCHKPK(cmPkLteLcId, param->lcId, mBuf);
   CMCHKPK(cmPkLteRnti, param->rnti, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRguCDatIndInfo
*
*
*     Desc : RguCDatIndInfo
 * DatInd from MAC to RLC on CCCH
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRguCDatIndInfo
(
RguCDatIndInfo *param,
Buffer *mBuf
)
{
   MsgLen msgLen, totalMsgLen;


   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->rnti, mBuf);
   CMCHKUNPK(cmUnpkLteLcId, &param->lcId, mBuf);
   CMCHKUNPK(cmUnpkMsgLen, &msgLen, mBuf);
   if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
      return RFAILED;
   if (SSegMsg(mBuf, totalMsgLen-msgLen, &param->pdu) != ROK)
      return RFAILED;
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRguLchDatInd
*
*
*     Desc : RguLchDatInd
 * DatInd Information for a logical channel
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRguLchDatInd
(
RguLchDatInd *param,
Buffer *mBuf
)
{

   CMCHKPK(cmPkRguPduInfo, &param->pdu, mBuf);
   CMCHKPK(cmPkLteLcId, param->lcId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRguLchDatInd
*
*
*     Desc : RguLchDatInd
 * DatInd Information for a logical channel
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRguLchDatInd
(
RguLchDatInd *param,
Buffer *mBuf
)
{

   CMCHKUNPK(cmUnpkLteLcId, &param->lcId, mBuf);
   CMCHKUNPK(cmUnpkRguPduInfo, &param->pdu, mBuf);
   return ROK;
}


/***********************************************************
*
*     Func : cmPkRguCStaRspInfo
*
*
*     Desc : RguCStaRspInfo
 * Status Response from RLC to MAC  for common logical channel
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRguCStaRspInfo
(
RguCStaRspInfo *param,
Buffer *mBuf
)
{


   switch(param->lcType) {
      case CM_LTE_LCH_CCCH:
         CMCHKPK(cmPkLteRnti, param->u.rnti, mBuf);
         break;
      case CM_LTE_LCH_BCCH:
      case CM_LTE_LCH_PCCH:
         CMCHKPK(cmPkLteTimingInfo, &param->u.timeToTx, mBuf);
#ifdef EMTC_ENABLE
         CMCHKPK(oduUnpackUInt8,param->pnb,mBuf);
         CMCHKPK(oduUnpackUInt8,param->emtcDiReason,mBuf);
#endif
         break;
      default :
         return RFAILED;
   }
   CMCHKPK(SPkS32, param->bo, mBuf);
   CMCHKPK(cmPkLteLcType, param->lcType, mBuf);
   CMCHKPK(cmPkLteLcId, param->lcId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRguCStaRspInfo
*
*
*     Desc : RguCStaRspInfo
 * Status Response from RLC to MAC  for common logical channel
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRguCStaRspInfo
(
RguCStaRspInfo *param,
Buffer *mBuf
)
{


   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteLcId, &param->lcId, mBuf);
   CMCHKUNPK(cmUnpkLteLcType, &param->lcType, mBuf);
   CMCHKUNPK(SUnpkS32, &param->bo, mBuf);
   switch(param->lcType) {
      case CM_LTE_LCH_BCCH:
      case CM_LTE_LCH_PCCH:
#ifdef EMTC_ENABLE
         CMCHKUNPK(oduPackUInt8,&param->emtcDiReason , mBuf);
         CMCHKUNPK(oduPackUInt8,&param->pnb , mBuf);
#endif
         CMCHKUNPK(cmUnpkLteTimingInfo, &param->u.timeToTx, mBuf);
         break;
      case CM_LTE_LCH_CCCH:
         CMCHKUNPK(cmUnpkLteRnti, &param->u.rnti, mBuf);
         break;
      default :
         return RFAILED;
   }
   return ROK;
}


/***********************************************************
*
*     Func : cmPkRguLchStaInd
*
*
*     Desc : RguLchStaInd
 * StaInd info for each logical channel of a UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRguLchStaInd
(
RguLchStaInd *param,
Buffer *mBuf
)
{


   CMCHKPK(SPkS16, param->totBufSize, mBuf);
   CMCHKPK(cmPkLteLcId, param->lcId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRguLchStaInd
*
*
*     Desc : RguLchStaInd
 * StaInd info for each logical channel of a UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRguLchStaInd
(
RguLchStaInd *param,
Buffer *mBuf
)
{

   CMCHKUNPK(cmUnpkLteLcId, &param->lcId, mBuf);
   CMCHKUNPK(SUnpkS16, &param->totBufSize, mBuf);
   return ROK;
}


/***********************************************************
*
*     Func : cmPkRguStaIndTb
*
*
*     Desc : RguStaIndTb
* StaInd from RLC to MAC for dedicated channels of a UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRguStaIndTb
(
RguStaIndTb *param,
Buffer *mBuf
)
{
   S32 i;


#ifdef LTE_L2_MEAS
   CMCHKPK(oduUnpackUInt16, param->status, mBuf);
   CMCHKPK(oduUnpackUInt32, param->tbId, mBuf);
#endif   
   for (i=param->nmbLch-1; i >= 0; i--) {
      CMCHKPK(cmPkRguLchStaInd, &param->lchStaInd[i], mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->nmbLch, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRguStaIndTb
*
*
*     Desc : RguStaIndTb
* StaInd from RLC to MAC for dedicated channels of a UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRguStaIndTb
(
RguStaIndTb *param,
Buffer *mBuf
)
{
   S32 i;

   CMCHKUNPK(oduPackUInt8, &param->nmbLch, mBuf);
   for (i=0; i<param->nmbLch; i++) {
      CMCHKUNPK(cmUnpkRguLchStaInd, &param->lchStaInd[i], mBuf);
   }
#ifdef LTE_L2_MEAS
   CMCHKUNPK(oduPackUInt32, &param->tbId, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->status, mBuf);
#endif   
   return ROK;
}

#endif

/**********************************************************************
         End of file
**********************************************************************/
