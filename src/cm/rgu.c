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
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm_tkns.h"       /* Common Token Defines */
#include "cm_llist.h"      /* Common Link List Defines */
#include "cm_hash.h"       /* Common Hash List Defines */
#include "cm_lte.h"        /* Common LTE Defines */
#include "rgu.h"           /* RGU Interface defines */

/* header/extern include files (.x) */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_lte.x"        /* Common LTE Defines */
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
#ifdef ANSI
PUBLIC S16 cmPkRguBndReq
(
Pst* pst,
SuId suId,
SpId spId
)
#else
PUBLIC S16 cmPkRguBndReq(pst, suId, spId)
Pst* pst;
SuId suId;
SpId spId;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkRguBndReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU001, (ErrVal)0, "Packing failed");
#endif      
      RETVALUE(RFAILED);
   }
   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU002, (ErrVal)0, "Packing failed");
#endif      
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU003, (ErrVal)0, "Packing failed");
#endif      
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
   pst->event = (Event) EVTRGUBNDREQ;
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
PUBLIC S16 cmUnpkRguBndReq
(
RguBndReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkRguBndReq(func, pst, mBuf)
RguBndReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   SuId suId;
   SpId spId;
   
   TRC3(cmUnpkRguBndReq)

   if (SUnpkS16(&suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU004, (ErrVal)0, "UnPacking failed");
#endif      
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
   if (SUnpkS16(&spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU005, (ErrVal)0, "UnPacking failed");
#endif      
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, suId, spId));
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
#ifdef ANSI
PUBLIC S16 cmPkRguUbndReq
(
Pst* pst,
SpId spId,
Reason reason
)
#else
PUBLIC S16 cmPkRguUbndReq(pst, spId, reason)
Pst* pst;
SpId spId;
Reason reason;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkRguUbndReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU006, (ErrVal)0, "Packing failed");
#endif      
      RETVALUE(RFAILED);
   }
   if (SPkS16(reason, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU007, (ErrVal)0, "Packing failed");
#endif      
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU008, (ErrVal)0, "Packing failed");
#endif      
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
   pst->event = (Event) EVTRGUUBNDREQ;
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
PUBLIC S16 cmUnpkRguUbndReq
(
RguUbndReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkRguUbndReq(func, pst, mBuf)
RguUbndReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   SpId spId;
   Reason reason;
   
   TRC3(cmUnpkRguUbndReq)

   if (SUnpkS16(&spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU009, (ErrVal)0, "UnPacking failed");
#endif      
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
   if (SUnpkS16(&reason, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU010, (ErrVal)0, "UnPacking failed");
#endif      
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, spId, reason));
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
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 cmPkRguBndCfm
(
Pst* pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 cmPkRguBndCfm(pst, suId, status)
Pst* pst;
SuId suId;
U8 status;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkRguBndCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU011, (ErrVal)0, "Packing failed");
#endif      
      RETVALUE(RFAILED);
   }
   if (SPkU8(status, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU012, (ErrVal)0, "Packing failed");
#endif      
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU013, (ErrVal)0, "Packing failed");
#endif      
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
   pst->event = (Event) EVTRGUBNDCFM;
   RETVALUE(SPstTsk(pst,mBuf));
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
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 cmUnpkRguBndCfm
(
RguBndCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkRguBndCfm(func, pst, mBuf)
RguBndCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   SuId suId;
   U8 status;
   
   TRC3(cmUnpkRguBndCfm)

   if (SUnpkS16(&suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU014, (ErrVal)0, "UnPacking failed");
#endif      
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
   if (SUnpkU8(&status, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU015, (ErrVal)0, "UnPacking failed");
#endif      
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, suId, status));
}


/**
* @brief Request from RLC to MAC for forwarding SDUs on 
 * dedicated channel for transmission
*
* @details
*
*     Function : packDlData
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   RguDDatReqInfo  *  datReq
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 packDlData
(
Pst* pst,
SpId spId,
RlcMacData  *dlData
)
#else
PUBLIC S16 packDlData(pst, spId, dlData)
Pst* pst;
SpId spId;
RlcMacData  dlData;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(packDlData)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU022, (ErrVal)0, "Packing failed");
#endif      
      SPutSBuf(pst->region, pst->pool, (Data *)dlData, sizeof(RlcMacData));
      RETVALUE(RFAILED);
   }

   if (pst->selector == RGU_SEL_LWLC)
   {
      CMCHKPK(cmPkPtr,(PTR) dlData, mBuf);
   }
   else
   {
      /*rgu_c_001.main_5 - ADD - L2M Support */
#ifdef LTE_L2_MEAS
      if (packRlcMacDataInfo(pst, dlData, mBuf) != ROK)
#else
      if (packRlcMacDataInfo(dlData, mBuf) != ROK) 
#endif
      {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU023, (ErrVal)0, "Packing failed");
#endif      
         SPutSBuf(pst->region, pst->pool, (Data *)dlData,
                                       sizeof(RlcMacData));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }

      if (SPutSBuf(pst->region, pst->pool, 
                   (Data *)dlData, sizeof(RlcMacData)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
              __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
             (ErrVal)ERGU025, (ErrVal)0, "Packing failed");
#endif      
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
      dlData = NULLP;
   }
   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU024, (ErrVal)0, "Packing failed");
#endif      
      if (dlData != NULLP);
      {
         SPutSBuf(pst->region, pst->pool, 
                  (Data *)dlData, sizeof(RlcMacData));
      }
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
   pst->event = (Event) EVTRLCDLDAT;
   RETVALUE(SPstTsk(pst,mBuf));
}


/**
* @brief Request from RLC to MAC for forwarding SDUs on 
 * dedicated channel for transmission
*
* @details
*
*     Function : unpackDlData
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   RguDDatReqInfo  *  datReq
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 unpackDlData
(
RguDDatReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 unpackDlData(func, pst, mBuf)
RguDDatReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   SpId spId;
//   RguDDatReqInfo *datReq;
   RlcMacData *dlData;
   
   TRC3(unpackDlData)

   if (SUnpkS16(&spId, mBuf) != ROK) {
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }

   if (pst->selector == RGU_SEL_LWLC)
   {
      CMCHKUNPK(cmUnpkPtr,(PTR *) &dlData, mBuf);
   }
   else 
   {
      if ((SGetSBuf(pst->region, pst->pool, 
                     (Data **)&dlData, sizeof(RlcMacData))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
             (ErrVal)ERGU026, (ErrVal)0, "UnPacking failed");
#endif      
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
      cmMemset((U8*)dlData, (U8)0, sizeof(RlcMacData));
  /*rgu_c_001.main_5 - ADD - L2M Support */
#ifdef LTE_L2_MEAS
      if (unpackRlcMacDataInfo(pst,dlData, mBuf) != ROK)
#else
      if (unpackRlcMacDataInfo(dlData, mBuf) != ROK) 
#endif
      {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
             (ErrVal)ERGU027, (ErrVal)0, "UnPacking failed");
#endif      
         SPutMsg(mBuf);
         SPutSBuf(pst->region, pst->pool, (Data *)dlData,
                                     sizeof(RlcMacData));
         RETVALUE(RFAILED);
      }
   }
   SPutMsg(mBuf);

   /* TODO : change function call to send RlcMacData as below: */
   RETVALUE((*func)(pst, spId, dlData));
   
   //RETVALUE((*func)(pst, spId, datReq));
}


/**
* @brief Data Indication from MAC to RLC to 
 * forward the data received for dedicated channels
*
* @details
*
*     Function : packRcvdUlData
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RlcMacData *ulData
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 packRcvdUlData
(
Pst* pst,
SuId suId,
RlcMacData  *ulData
)
#else
PUBLIC S16 packRcvdUlData(pst, suId, ulData)
Pst* pst;
SuId suId;
RlcMacData  *ulData;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(packRcvdUlData)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU035, (ErrVal)0, "Packing failed");
#endif      
      SPutStaticBuffer(pst->region, pst->pool, (Data *)ulData, sizeof(RlcMacData),0);
      RETVALUE(RFAILED);
   }

   if (pst->selector == RGU_SEL_LWLC)
   {
      CMCHKPK(cmPkPtr,(PTR)ulData, mBuf);
   }
   else
   {
      if (packRlcMacDataInfo(ulData, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
            SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                (ErrVal)ERGU036, (ErrVal)0, "Packing failed");
#endif            
         SPutStaticBuffer(pst->region, pst->pool, (Data *)ulData, sizeof(RlcMacData),0);
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }

      if (SPutStaticBuffer(pst->region, pst->pool, 
                      (Data *)ulData, sizeof(RlcMacData),0) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
             (ErrVal)ERGU038, (ErrVal)0, "Packing failed");
#endif      
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
      ulData = NULLP;
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU037, (ErrVal)0, "Packing failed");
#endif      
      SPutStaticBuffer(pst->region, pst->pool, (Data *)ulData, sizeof(RlcMacData),0);
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTRLCULDAT;
   RETVALUE(SPstTsk(pst,mBuf));
}


/**
* @brief Data Indication from MAC to RLC to 
 * forward the data received for dedicated channels
*
* @details
*
*     Function : unpackRcvdUlData
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RlcMacData  *ulData
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 unpackRcvdUlData
(
RguDDatInd func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 unpackRcvdUlData(func, pst, mBuf)
RguDDatInd func;
Pst *pst;
Buffer *mBuf;
#endif
{
   SuId suId;
   RlcMacData *ulData;
   
   TRC3(unpackRcvdUlData)

   if (SUnpkS16(&suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU039, (ErrVal)0, "UnPacking failed");
#endif      
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }

   if (pst->selector == RGU_SEL_LWLC)
   {
      CMCHKUNPK(cmUnpkPtr,(PTR *) &ulData, mBuf);
   }
   else 
   {
       if ((SGetStaticBuffer(pst->region, pst->pool, 
                    (Data **)&ulData, sizeof(RlcMacData),0)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
             (ErrVal)ERGU040, (ErrVal)0, "UnPacking failed");
#endif      
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
      if (unpackRlcMacDataInfo(ulData, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
             (ErrVal)ERGU041, (ErrVal)0, "UnPacking failed");
#endif      
         SPutMsg(mBuf);
         SPutStaticBuffer(pst->region, pst->pool, 
                    (Data *)ulData, sizeof(RlcMacData),0);
         RETVALUE(RFAILED);
      }
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, suId, ulData));
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
#ifdef ANSI
PUBLIC S16 cmPkRguL2MUlThrpMeasReqInfo 
(
RguL2MUlThrpMeasReqInfo* param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkRguL2MUlThrpMeasReqInfo(param, mBuf)
RguL2MUlThrpMeasReqInfo* param;
Buffer *mBuf;
#endif
{
   S32 loop;
   TRC3(cmPkRguL2MUlThrpMeasReqInfo);

   CMCHKPK(SPkU8, param->enbMeas, mBuf);
   for (loop=param->numLcId-1; loop >= 0; loop--)
   {
      CMCHKPK(SPkU8, param->lcId[loop], mBuf);
   }
   CMCHKPK(cmPkLteLcId, param->numLcId, mBuf);
   CMCHKPK(cmPkLteRnti, param->rnti, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 cmUnpkRguL2MUlThrpMeasReqInfo 
(
RguL2MUlThrpMeasReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkRguL2MUlThrpMeasReqInfo(param, mBuf)
RguL2MUlThrpMeasReqInfo *param;
Buffer *mBuf;
#endif
{
   U8 loop;
   TRC3(cmUnpkRguL2MUlThrpMeasReqInfo);

   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->rnti, mBuf);
   CMCHKUNPK(cmUnpkLteLcId, &param->numLcId, mBuf);
   for (loop=0; loop<param->numLcId; loop++)
   {
      CMCHKUNPK(SUnpkU8, &param->lcId[loop], mBuf);
   }
   CMCHKUNPK(SUnpkU8, &param->enbMeas, mBuf);
   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 cmPkRguL2MUlThrpMeasReq 
(
Pst* pst,
SpId spId,
RguL2MUlThrpMeasReqInfo* measReq
)
#else
PUBLIC S16 cmPkRguL2MUlThrpMeasReq(pst, spId, measReq)
Pst* pst;
SpId spId;
RguL2MUlThrpMeasReqInfo* measReq;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkRguL2MUlThrpMeasReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU049, (ErrVal)0, "Packing failed");
#endif      
      SPutSBuf(pst->region, pst->pool, (Data *)measReq, sizeof(RguL2MUlThrpMeasReqInfo));
      RETVALUE(RFAILED);
   }
   if (pst->selector == RGU_SEL_LWLC)
   {
      CMCHKPK(cmPkPtr,(PTR) measReq, mBuf);
   }
   else
   {
      if (cmPkRguL2MUlThrpMeasReqInfo(measReq, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
             (ErrVal)ERGU050, (ErrVal)0, "Packing failed");
#endif      
         SPutSBuf(pst->region, pst->pool, (Data *)measReq, 
                                    sizeof(RguL2MUlThrpMeasReqInfo));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
      if (SPutSBuf(pst->region, pst->pool, (Data *)measReq, 
                           sizeof(RguL2MUlThrpMeasReqInfo)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
             (ErrVal)ERGU052, (ErrVal)0, "Packing failed");
#endif      
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
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
         SPutSBuf(pst->region, pst->pool, (Data *)measReq,
                             sizeof(RguL2MUlThrpMeasReqInfo));
      }
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTRGUL2MULTHRPMEASREQ;
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
PUBLIC S16 cmUnpkRguL2MUlThrpMeasReq 
(
RguL2MUlThrpMeasReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkRguL2MUlThrpMeasReq(func, pst, mBuf)
RguL2MUlThrpMeasReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   SpId spId;
   RguL2MUlThrpMeasReqInfo* measReq;
   
   TRC3(cmUnpkRguL2MUlThrpMeasReq)

   if (SUnpkS16(&spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU053, (ErrVal)0, "UnPacking failed");
#endif      
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
   if (pst->selector == RGU_SEL_LWLC)
   {
      CMCHKUNPK(cmUnpkPtr,(PTR *) &measReq, mBuf);
   }
   else 
   {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&measReq,
                              sizeof(RguL2MUlThrpMeasReqInfo))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
             (ErrVal)ERGU054, (ErrVal)0, "UnPacking failed");
#endif      
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
      if (cmUnpkRguL2MUlThrpMeasReqInfo(measReq, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
             (ErrVal)ERGU055, (ErrVal)0, "UnPacking failed");
#endif      
         SPutMsg(mBuf);
         SPutSBuf(pst->region, pst->pool, (Data *)measReq, 
                                sizeof(RguL2MUlThrpMeasReqInfo));
         RETVALUE(RFAILED);
      }
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, spId, measReq));
}

#endif




/**
* @brief Primitive invoked from RLC to MAC to 
 * inform the BO report for dedicated channels
*
* @details
*
*     Function : packBOStatus,
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   RlcMacBOStatus  *  staRsp
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 packBOStatus
(
Pst* pst,
SpId spId,
RlcMacBOStatus  *boStatus
)
#else
PUBLIC S16 packBOStatus(pst, spId, staRsp)
Pst* pst;
SpId spId;
RlcMacBOStatus  *boStatus;
#endif
{
   RlcMacBOStatus  *boStaInfo = NULL;
   Buffer *mBuf = NULLP;

   if(SGetSBuf(pst->region, pst->pool, (Data **)&boStaInfo, sizeof(RlcMacBOStatus)) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGU056, (ErrVal)0, "Packing failed");
#endif      
      RETVALUE(RFAILED);
   }
#ifdef ERRCLS_KW
   /* boStaInfo cant be NULL here */
   if (boStaInfo == NULLP)
   {
      RETVALUE(RFAILED);
   }
#endif
   cmMemcpy((U8 *)boStaInfo, (U8 *)boStatus, sizeof(RlcMacBOStatus)); 
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU056, (ErrVal)0, "Packing failed");
#endif      
         SPutSBuf(pst->region, pst->pool, (Data *)boStaInfo, sizeof(RlcMacBOStatus));

      RETVALUE(RFAILED);
   }
   if (pst->selector == RGU_SEL_LWLC)
   {
      CMCHKPK(cmPkPtr,(PTR) boStaInfo, mBuf);
   }
   else
   {
      if (packBOStatusInfo(boStatus, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGU057, (ErrVal)0, "Packing failed");
#endif      
        SPutMsg(mBuf);
        RETVALUE(RFAILED);
     }
   }
   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU058, (ErrVal)0, "Packing failed");
#endif      
      if (boStaInfo != NULLP)
      {
         SPutSBuf(pst->region, pst->pool, (Data *)boStaInfo, sizeof(RlcMacBOStatus));
      }
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event)EVTRLCBOSTA;
   RETVALUE(SPstTsk(pst,mBuf));
   SPutMsg(mBuf);
}


/**
* @brief Primitive invoked from RLC to MAC to 
* inform the BO report for dedicated channels
*
* @details
*
*     Function : unpackBOStatus
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   RlcMacBOStatus *  staRsp
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 unpackBOStatus
(
RguDStaRsp func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 unpackBOStatus(func, pst, mBuf)
RguDStaRsp func;
Pst *pst;
Buffer *mBuf;
#endif
{
   SpId spId;
   RlcMacBOStatus *boSta;

   TRC3(unpackBOStatus)

   if (SUnpkS16(&spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU060, (ErrVal)0, "UnPacking failed");
#endif      
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }

   if (pst->selector == RGU_SEL_LWLC)
   {
      CMCHKUNPK(cmUnpkPtr,(PTR *) &boSta, mBuf);
   }
   else
   {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&boSta, 
                                 sizeof(RlcMacBOStatus))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
             (ErrVal)ERGU061, (ErrVal)0, "UnPacking failed");
#endif      
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
      if (unpackBOStatusInfo(boSta, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
             (ErrVal)ERGU062, (ErrVal)0, "UnPacking failed");
#endif      
         SPutMsg(mBuf);
         SPutSBuf(pst->region, pst->pool, (Data *)boSta, sizeof(RlcMacBOStatus));
         RETVALUE(RFAILED);
      }
   }
   SPutMsg(mBuf);
  // (*func)(pst, spId, boSta);
   SPutSBuf(pst->region, pst->pool, (Data *)boSta, sizeof(RlcMacBOStatus));
   RETVALUE(ROK);
}


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
#ifdef ANSI
PUBLIC S16 cmPkRguHqStaInd
(
Pst* pst,
SuId suId,
RguHarqStatusInd  *harqStatusInd
)
#else
PUBLIC S16 cmPkRguHqStaInd(pst, suId, harqStatusInd)
Pst* pst;
SuId suId;
RguHarqStatusInd  *harqStatusInd;
#endif
{

   Buffer   *mBuf;
   U8       idx;
   RguHarqStatusInd *harqStaInd = NULL;

   TRC3(cmPkRguHqStaInd)
#ifdef XEON_SPECIFIC_CHANGES
   if (SGetSBuf(pst->region, pst->pool, (Data **)&harqStaInd, sizeof(RguHarqStatusInd)) != ROK)
#else      
   if ((SGetStaticBuffer(pst->region, pst->pool, 
               (Data **)&harqStaInd, sizeof(RguHarqStatusInd),0)) != ROK)
#endif      
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGU070, (ErrVal)0, "Packing failed");
#endif      
      RETVALUE(RFAILED);
   }

   cmMemcpy((U8 *)harqStaInd, (U8 *)harqStatusInd, sizeof(RguHarqStatusInd));

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERGU070, (ErrVal)0, "Packing failed");
#endif
#ifdef XEON_SPECIFIC_CHANGES
      SPutSBuf(pst->region, pst->pool, (Data *)harqStaInd, sizeof(RguHarqStatusInd));
#else      
      SPutStaticBuffer(pst->region, pst->pool, (Data *)harqStaInd, sizeof(RguHarqStatusInd), 0);
#endif      
      RETVALUE(RFAILED);
   }

   if (pst->selector == RGU_SEL_LWLC)
   {
      CMCHKPK(cmPkPtr,(PTR) harqStaInd, mBuf); 
   }
   else
   {
      for(idx = 0; idx < harqStaInd->numTbs; idx++)
      {
         CMCHKPK(SPkU16, harqStaInd->status[idx], mBuf);
      }
      for(idx = 0; idx < harqStaInd->numTbs; idx++)
      {
         CMCHKPK(SPkU32, harqStaInd->tbId[idx], mBuf);
      }
      CMCHKPK(SPkU8, harqStaInd->numTbs, mBuf);
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
      SPutSBuf(pst->region, pst->pool, (Data *)harqStaInd,
              sizeof(RguHarqStatusInd));
#else      
      SPutStaticBuffer(pst->region, pst->pool, (Data *)harqStaInd, sizeof(RguHarqStatusInd), 0);
#endif      
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
#ifdef XEON_SPECIFIC_CHANGES
   if (SPutSBuf(pst->region, pst->pool, (Data *)harqStaInd,
            sizeof(RguHarqStatusInd)) != ROK) {
#else   
   if (pst->selector != RGU_SEL_LWLC)
   {
      if(SPutStaticBuffer(pst->region, pst->pool, (Data *)harqStaInd, 
               sizeof(RguHarqStatusInd), 0) != ROK)
      {
#endif         
#if (ERRCLASS & ERRCLS_ADD_RES)      
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ERGU072, (ErrVal)0, "Packing failed");
#endif      
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
#ifndef XEON_SPECIFIC_CHANGES      
   }
#endif   

   pst->event = (Event) EVTRGUHQSTAIND;
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
PUBLIC S16 cmUnpkRguHqStaInd
(
RguHqStaInd  func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkRguHqStaInd(func, pst, mBuf)
RguHqStaInd   func;
Pst *pst;
Buffer *mBuf;
#endif
{
   RguHarqStatusInd    *hqStaInd;  
   SuId                suId;
   U8                  idx;

   TRC3(cmUnpkRguHqStaInd)

   if (SUnpkS16(&suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU073, (ErrVal)0, "UnPacking failed");
#endif      
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
#ifdef XEON_SPECIFIC_CHANGES
   if ((SGetSBuf(pst->region, pst->pool, (Data **)&hqStaInd, 
        sizeof(RguHarqStatusInd))) != ROK) {
#else   
   if (pst->selector == RGU_SEL_LWLC)
   {
      CMCHKUNPK(cmUnpkPtr,(PTR *) &hqStaInd, mBuf);
   }  
   else
   {   
      if ((SGetStaticBuffer(pst->region, pst->pool, 
                  (Data **)&hqStaInd, sizeof(RguHarqStatusInd),0)) != ROK)
      {
#endif         
#if (ERRCLASS & ERRCLS_ADD_RES)      
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ERGU074, (ErrVal)0, "UnPacking failed");
#endif      
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
      CMCHKUNPK(cmUnpkLteCellId, &hqStaInd->cellId, mBuf);
      CMCHKUNPK(cmUnpkLteRnti, &hqStaInd->ueId, mBuf);
      CMCHKUNPK(SUnpkU8, &hqStaInd->numTbs, mBuf);
      for(idx = hqStaInd->numTbs; idx > 0; idx--)
      {
         CMCHKUNPK(SUnpkU32, &hqStaInd->tbId[idx - 1], mBuf);
      }
      for(idx = hqStaInd->numTbs; idx > 0; idx--)
      {
         CMCHKUNPK(SUnpkU16, &hqStaInd->status[idx - 1], mBuf);
      }
#ifndef XEON_SPECIFIC_CHANGES      
   }
#endif   
   SPutMsg(mBuf);
   (*func)(pst, suId, hqStaInd);
#ifdef XEON_SPECIFIC_CHANGES
   SPutSBuf(pst->region, pst->pool, (Data *)hqStaInd, sizeof(RguHarqStatusInd));
#else   
   SPutStaticBuffer(pst->region, pst->pool, (Data *)hqStaInd, sizeof(RguHarqStatusInd), 0);
#endif   
   RETVALUE(ROK);
}
#endif /* LTE_L2_MEAS */


/**
* @brief Status Indication from MAC to RLC  
 * as a response to the staRsp primitive from RLC.
 * Informs RLC of the totalBufferSize and Timing Info 
 * for the transmission on dedicated channels.
*
* @details
*
*     Function : packSchedRep
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RlcMacSchedRep*  schRep
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 packSchedRep
(
Pst* pst,
SuId suId,
RlcMacSchedRep  * schRep
)
#else
PUBLIC S16 packSchedRep(pst, suId, staInd)
Pst* pst;
SuId suId;
RlcMacSchedRep  * schRep;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(packSchedRep)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU076, (ErrVal)0, "Packing failed");
#endif      
      SPutSBuf(pst->region, pst->pool, (Data *)schRep, sizeof(RlcMacSchedRep));
      RETVALUE(RFAILED);
   }
   if (pst->selector == RGU_SEL_LWLC)
   {
      CMCHKPK(cmPkPtr,(PTR) schRep, mBuf);
   }
   else
   {
      if (packSchedRepInfo(schRep, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
           SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ERGU077, (ErrVal)0, "Packing failed");
#endif      
        SPutSBuf(pst->region, pst->pool, (Data *)schRep, 
                                         sizeof(RlcMacSchedRep));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      } 
      if (SPutSBuf(pst->region, pst->pool, (Data *)schRep,
                                  sizeof(RlcMacSchedRep)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
             (ErrVal)ERGU079, (ErrVal)0, "Packing failed");
#endif      
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
      schRep= NULLP;
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU078, (ErrVal)0, "Packing failed");
#endif      
      if (schRep != NULLP)
      {
         SPutSBuf(pst->region, pst->pool, (Data *)schRep, sizeof(RlcMacSchedRep));
      }
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTSCHREP;
   RETVALUE(SPstTsk(pst,mBuf));
}


/**
* @brief Status Indication from MAC to RLC  
 * as a response to the staRsp primitive from RLC.
 * Informs RLC of the totalBufferSize and Timing Info 
 * for the transmission on dedicated channels.
*
* @details
*
*     Function : unpackSchedRep
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RguDStaIndInfo  *  staInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 unpackSchedRep
(
RguDStaInd func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 unpackSchedRep(func, pst, mBuf)
RguDStaInd func;
Pst *pst;
Buffer *mBuf;
#endif
{
   SuId suId;
//   RguDStaIndInfo *staInd;
   RlcMacSchedRep *schRep;
   
   TRC3(unpackSchedRep)

   if (SUnpkS16(&suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU080, (ErrVal)0, "UnPacking failed");
#endif      
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
   if (pst->selector == RGU_SEL_LWLC)
   {
      CMCHKUNPK(cmUnpkPtr,(PTR *) &schRep, mBuf);
   }
   else 
   {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&schRep, sizeof(RlcMacSchedRep))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGU081, (ErrVal)0, "UnPacking failed");
#endif      
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
      if (unpackSchedRepInfo(schRep, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
             (ErrVal)ERGU082, (ErrVal)0, "UnPacking failed");
#endif      
         SPutMsg(mBuf);
         SPutSBuf(pst->region, pst->pool, (Data *)schRep, sizeof(RlcMacSchedRep));
         RETVALUE(RFAILED);
      }
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, suId, schRep));
}

#ifdef ANSI
PUBLIC S16 cmPkRguLcFlowCntrlInfo
(
RguLcFlowCntrlInfo *param,
Buffer             *mBuf
)
#else
PUBLIC S16 cmPkRguLcFlowCntrlInfo (param,mBuf)
RguLcFlowCntrlInfo *param;
Buffer             *mBuf;
#endif
{
  TRC3(cmPkRguLcFlowCntrlInfo);
  CMCHKPK(SPkU32, param->maxBo4FlowCtrl, mBuf);
  CMCHKPK(SPkU32, param->pktAdmitCnt, mBuf);
  CMCHKPK(cmPkLteLcId, param->lcId, mBuf);

  RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 cmPkRguUeFlowCntrlInfo
(
RguUeFlowCntrlInfo *param,
Buffer             *mBuf
)
#else
PUBLIC S16 cmPkRguUeFlowCntrlInfo (param,mBuf)
RguUeFlowCntrlInfo *param;
Buffer             *mBuf;
#endif
{
  U32 idx;

  TRC3(cmPkRguUeFlowCntrlInfo);

  for(idx=(param->numLcs - 1); idx >= 0; idx--)
  {
    cmPkRguLcFlowCntrlInfo(&param->lcInfo[idx],mBuf);
  }
  CMCHKPK(SPkU32, param->numLcs, mBuf);
  CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
  RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 cmPkRguFlowCntrlInfo
(
RguFlowCntrlInd *param, 
Buffer          *mBuf
)
#else
PUBLIC S16 cmPkRguFlowCntrlInfo(flowCntrlInd, mBuf)
RguFlowCntrlInd *param;
Buffer          *mBuf;
#endif
{
  U32 idx;

  TRC3(cmPkRguFlowCntrlInfo);

  for (idx=(param->numUes - 1); idx >= 0; idx--)
  {
    cmPkRguUeFlowCntrlInfo(&param->ueFlowCntrlInfo[idx],mBuf);
  }
  CMCHKPK(SPkU32, param->numUes, mBuf);
  CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
  RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 cmPkRguFlowCntrlInd
(
Pst* pst,
SuId suId,
RguFlowCntrlInd  *flowCntrlInd
)
#else
PUBLIC S16 cmPkRguFlowCntrlInd(pst, suId, flowCntrlInd)
Pst* pst;
SuId suId;
RguFlowCntrlInd   *flowCntrlInd;
#endif
{
   Buffer *mBuf = NULLP;

   TRC3(cmPkRguFlowCntrlInd);
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU076, (ErrVal)0, "Packing failed");
#endif      
      RETVALUE(RFAILED);
   }
   if (pst->selector == RGU_SEL_LWLC)
   {
      CMCHKPK(cmPkPtr,(PTR) flowCntrlInd, mBuf);
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
         RETVALUE(RFAILED);
      } 
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU078, (ErrVal)0, "Packing failed");
#endif      
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
   
   pst->event = (Event) EVTRGUFLOWCNTRLIND;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkRguLcFlowCntrlInfo
(
RguLcFlowCntrlInfo *param,
Buffer           *mBuf
)
#else
PUBLIC S16 cmUnpkRguLcFlowCntrlInfo (param,mBuf)
RguLcFlowCntrlInfo *param;
Buffer           *mBuf;
#endif
{
  TRC3(cmUnpkRguLcFlowCntrlInfo);

  CMCHKUNPK(cmUnpkLteLcId, &param->lcId, mBuf);
  CMCHKUNPK(SUnpkU32, &param->pktAdmitCnt, mBuf);
  CMCHKUNPK(SUnpkU32, &param->maxBo4FlowCtrl, mBuf);
  
  RETVALUE(ROK);
}
#ifdef ANSI
PUBLIC S16 cmUnpkRguUeFlowCntrlInfo
(
RguUeFlowCntrlInfo *param,
Buffer           *mBuf
)
#else
PUBLIC S16 cmUnpkRguUeFlowCntrlInfo (param,mBuf)
RguUeFlowCntrlInfo *param;
Buffer           *mBuf;
#endif
{
  U32 idx;
  TRC3(cmUnpkRguUeFlowCntrlInfo);
  CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
  CMCHKUNPK(SUnpkU32, &param->numLcs, mBuf);
  for(idx=0; idx < param->numLcs; idx++)
  {
    cmUnpkRguLcFlowCntrlInfo(&param->lcInfo[idx],mBuf);
  }
  RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 cmUnpkRguFlowCntrlInfo
(
RguFlowCntrlInd *param, 
Buffer           *mBuf
)
#else
PUBLIC S16 cmUnpkRguFlowCntrlInfo(flowCntrlInfo, mBuf)
RguFlowCntrlInd *param;
Buffer           *mBuf;
#endif
{
  U32 idx; 
  
  TRC3(cmUnpkRguFlowCntrlInfo);

  CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
  CMCHKUNPK(SUnpkU32, &param->numUes, mBuf);
  for (idx=0; idx < param->numUes; idx++)
  {
    cmUnpkRguUeFlowCntrlInfo(&param->ueFlowCntrlInfo[idx],mBuf);
  }
  RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 cmUnpkRguFlowCntrlInd
(
RguFlowCntrlIndInfo func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkRguFlowCntrlInd(func, pst, mBuf)
RguFlowCntrlIndInfo func;
Pst *pst;
Buffer *mBuf;
#endif
{
   SuId suId;
   RguFlowCntrlInd *flowCntrlInd;
   
   TRC3(cmUnpkRguFlowCntrlInd)

   if (SUnpkS16(&suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
          (ErrVal)ERGU080, (ErrVal)0, "UnPacking failed");
#endif      
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
   if (pst->selector == RGU_SEL_LWLC)
   {
      CMCHKUNPK(cmUnpkPtr,(PTR *) &flowCntrlInd, mBuf);
   }
   else 
   {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&flowCntrlInd, sizeof(RguFlowCntrlInd))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERGU081, (ErrVal)0, "UnPacking failed");
#endif      
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
      if (cmUnpkRguFlowCntrlInfo(flowCntrlInd, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
             (ErrVal)ERGU082, (ErrVal)0, "UnPacking failed");
#endif      
         SPutMsg(mBuf);
         SPutSBuf(pst->region, pst->pool, (Data *)flowCntrlInd, sizeof(RguFlowCntrlInd));
         RETVALUE(RFAILED);
      }
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, suId, flowCntrlInd));
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
#ifdef ANSI
PUBLIC S16 cmPkRguPduInfo
(
RguPduInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkRguPduInfo(param, mBuf)
RguPduInfo *param;
Buffer *mBuf;
#endif
{
   S32 i;

   TRC3(cmPkRguPduInfo);

   for (i=param->numPdu-1; i >= 0; i--) {
     /* rgu_c_001.main_3: ccpu00111328: S16 is renamed as MsgLen */
      MsgLen msgLen = 0;
      if (SFndLenMsg(param->mBuf[i], &msgLen) != ROK)
         RETVALUE(RFAILED);
      if (SCatMsg(mBuf, param->mBuf[i], M1M2) != ROK)
         RETVALUE(RFAILED);
      SPutMsg(param->mBuf[i]);
      CMCHKPK(cmPkMsgLen, msgLen, mBuf);
   }
   CMCHKPK(SPkU8, param->numPdu, mBuf);
   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 cmUnpkRguPduInfo
(
RguPduInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkRguPduInfo(param, mBuf)
RguPduInfo *param;
Buffer *mBuf;
#endif
{
   S32 i;

   TRC3(cmUnpkRguPduInfo);

   CMCHKUNPK(SUnpkU8, &param->numPdu, mBuf);
   for (i=0; i<param->numPdu; i++) {
      MsgLen msgLen, totalMsgLen;
      CMCHKUNPK(cmUnpkMsgLen, &msgLen, mBuf);
      if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
         RETVALUE(RFAILED);
      if (SSegMsg(mBuf, totalMsgLen-msgLen, &param->mBuf[i]) != ROK)
         RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 cmPkRguDBoReport
(
RguDBoReport *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkRguDBoReport(param, mBuf)
RguDBoReport *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkRguDBoReport);

   CMCHKPK(SPkU32, param->oldestSduArrTime, mBuf);
   CMCHKPK(SPkU32, param->staPduBo, mBuf);
#ifdef CCPU_OPT
   CMCHKPK(SPkU8, param->staPduPrsnt, mBuf);
   CMCHKPK(SPkU16, param->estRlcHdrSz, mBuf);
#endif
   CMCHKPK(SPkS32, param->bo, mBuf);
   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 cmUnpkRguDBoReport
(
RguDBoReport *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkRguDBoReport(param, mBuf)
RguDBoReport *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkRguDBoReport);

   CMCHKUNPK(SUnpkS32, &param->bo, mBuf);
#ifdef CCPU_OPT
   CMCHKUNPK(SUnpkU16, &param->estRlcHdrSz, mBuf);
   CMCHKUNPK(SUnpkU8, &param->staPduPrsnt, mBuf);
#endif
   
   CMCHKUNPK(SUnpkU32, &param->staPduBo, mBuf);
   CMCHKUNPK(SUnpkU32, &param->oldestSduArrTime, mBuf);
   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 cmPkRguCDatReqInfo
(
RguCDatReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkRguCDatReqInfo(param, mBuf)
RguCDatReqInfo *param;
Buffer *mBuf;
#endif
{
   MsgLen msgLen;

   TRC3(cmPkRguCDatReqInfo);

   if (param->pdu != NULLP)
   {
      if (SFndLenMsg(param->pdu, &msgLen) != ROK)
         RETVALUE(RFAILED);
      if (SCatMsg(mBuf, param->pdu, M1M2) != ROK)
         RETVALUE(RFAILED);
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
        CMCHKPK(SPkU8,param->pnb,mBuf);
#endif
         break;
      default :
         RETVALUE(RFAILED);
   }
   CMCHKPK(SPkU32, param->transId, mBuf);
   CMCHKPK(cmPkLteLcType, param->lcType, mBuf);
   CMCHKPK(cmPkLteLcId, param->lcId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 cmUnpkRguCDatReqInfo
(
RguCDatReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkRguCDatReqInfo(param, mBuf)
RguCDatReqInfo *param;
Buffer *mBuf;
#endif
{
   MsgLen msgLen, totalMsgLen;

   TRC3(cmUnpkRguCDatReqInfo);

   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteLcId, &param->lcId, mBuf);
   CMCHKUNPK(cmUnpkLteLcType, &param->lcType, mBuf);
   CMCHKUNPK(SUnpkU32, &param->transId, mBuf);
   switch(param->lcType) {
      case CM_LTE_LCH_BCCH:
      case CM_LTE_LCH_PCCH:
#ifdef EMTC_ENABLE
         CMCHKUNPK(SUnpkU8,&param->pnb, mBuf);
#endif
         CMCHKUNPK(cmUnpkLteTimingInfo, &param->u.timeToTx, mBuf);
         break;
      case CM_LTE_LCH_CCCH:
         CMCHKUNPK(cmUnpkLteRnti, &param->u.rnti, mBuf);
         break;
      default :
         RETVALUE(RFAILED);
   }
   SFndLenMsg(mBuf, &msgLen);
   if (msgLen > 0)
   {
      CMCHKUNPK(cmUnpkMsgLen, &msgLen, mBuf);
      if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
         RETVALUE(RFAILED);
      if (SSegMsg(mBuf, totalMsgLen-msgLen, &param->pdu) != ROK)
         RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 cmPkRguLchDatReq
(
RguLchDatReq *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkRguLchDatReq(param, mBuf)
RguLchDatReq *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkRguLchDatReq);

#ifdef L2_OPTMZ
   CMCHKPK(SPkU8, param->freeBuff, mBuf);
#endif
   CMCHKPK(SPkU8, param->setMaxUlPrio, mBuf);
   CMCHKPK(cmPkRguPduInfo, &param->pdu, mBuf);
   CMCHKPK(cmPkRguDBoReport, &param->boReport, mBuf);
   CMCHKPK(cmPkLteLcId, param->lcId, mBuf);
   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 cmUnpkRguLchDatReq
(
RguLchDatReq *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkRguLchDatReq(param, mBuf)
RguLchDatReq *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkRguLchDatReq);

   CMCHKUNPK(cmUnpkLteLcId, &param->lcId, mBuf);
   CMCHKUNPK(cmUnpkRguDBoReport, &param->boReport, mBuf);
   CMCHKUNPK(cmUnpkRguPduInfo, &param->pdu, mBuf);
   CMCHKUNPK(SUnpkU8, &param->setMaxUlPrio, mBuf);
#ifdef L2_OPTMZ
   CMCHKUNPK(SUnpkU8, &param->freeBuff, mBuf);
#endif
   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 cmPkRguDatReqTb
(
RguDatReqTb *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkRguDatReqTb(param, mBuf)
RguDatReqTb *param;
Buffer *mBuf;
#endif
{
   S32 i;

   TRC3(cmPkRguDatReqTb);
   /*rgu_c_001.main_5 - ADD - L2M Support */
#ifdef LTE_L2_MEAS
   CMCHKPK(SPkU32, param->tbId, mBuf);
#endif
   for (i=param->nmbLch-1; i >= 0; i--) {
      CMCHKPK(cmPkRguLchDatReq, &param->lchData[i], mBuf);
   }
   CMCHKPK(SPkU8, param->nmbLch, mBuf);
   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 cmUnpkRguDatReqTb
(
Pst         *pst, 
RguDatReqTb *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkRguDatReqTb(pst, param, mBuf)
Pst         *pst;   
RguDatReqTb *param;
Buffer *mBuf;
#endif
#else
#ifdef ANSI
PUBLIC S16 cmUnpkRguDatReqTb
(
RguDatReqTb *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkRguDatReqTb(param, mBuf)
RguDatReqTb *param;
Buffer *mBuf;
#endif
#endif
{
   S32 i;

   TRC3(cmUnpkRguDatReqTb);

   CMCHKUNPK(SUnpkU8, &param->nmbLch, mBuf);
   for (i=0; i<param->nmbLch; i++) {
      CMCHKUNPK(cmUnpkRguLchDatReq, &param->lchData[i], mBuf);
   }
   /*rgu_c_001.main_5 - ADD - L2M Support */
#ifdef LTE_L2_MEAS
   CMCHKUNPK(SUnpkU32, &param->tbId, mBuf);
#endif
   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 cmPkRguDDatReqInfo
(
Pst            *pst,
RguDDatReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkRguDDatReqInfo(pst, param, mBuf)
Pst            *pst;
RguDDatReqInfo *param;
Buffer *mBuf;
#endif
#else
#ifdef ANSI
PUBLIC S16 cmPkRguDDatReqInfo
(
RguDDatReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkRguDDatReqInfo(param, mBuf)
RguDDatReqInfo *param;
Buffer *mBuf;
#endif
#endif
{
   S32 i;
   S32 idx;

   TRC3(cmPkRguDDatReqInfo);

   for(idx = (param->nmbOfUeGrantPerTti-1); idx >= 0; idx--)
   {
      RguDDatReqPerUe *datReq = &param->datReq[idx];
      for (i=RGU_MAX_TB-1; i >= 0; i--) 
      {
         CMCHKPK(cmPkRguDatReqTb, &datReq->datReqTb[i], mBuf);
      }
      CMCHKPK(SPkU8, datReq->nmbOfTbs, mBuf);
      CMCHKPK(SPkU32, datReq->transId, mBuf);
      CMCHKPK(cmPkLteRnti, datReq->rnti, mBuf);
   }
   CMCHKPK(SPkU8, param->nmbOfUeGrantPerTti, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 cmUnpkRguDDatReqInfo
(
Pst            *pst, 
RguDDatReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkRguDDatReqInfo(pst, param, mBuf)
Pst            *pst;
RguDDatReqInfo *param;
Buffer *mBuf;
#endif
#else
#ifdef ANSI
PUBLIC S16 cmUnpkRguDDatReqInfo
(
RguDDatReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkRguDDatReqInfo(param, mBuf)
RguDDatReqInfo *param;
Buffer *mBuf;
#endif
#endif
{
   S32 i;
  /* After Merging from 2.1 to 2.2 */
   /*rgu_c_001.main_5 - ADD - L2M Support */
#ifdef LTE_L2_MEAS
   S16 retVal;
#endif
   S32 idx;

   TRC3(cmUnpkRguDDatReqInfo);

  /* After Merging from 2.1 to 2.2 */
   /*rgu_c_001.main_5 - ADD - L2M Support */
#ifdef LTE_L2_MEAS
   retVal = ROK;
#endif
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(SUnpkU8, &param->nmbOfUeGrantPerTti, mBuf);
   for(idx = 0; idx < param->nmbOfUeGrantPerTti; idx++)
   { 
      RguDDatReqPerUe *datReq = &param->datReq[idx];
      CMCHKUNPK(cmUnpkLteRnti, &datReq->rnti, mBuf);
      CMCHKUNPK(SUnpkU32, &datReq->transId, mBuf);
      CMCHKUNPK(SUnpkU8, &datReq->nmbOfTbs, mBuf);
      /* rgu_c_001.main_4 - Changes for MIMO feature addition */
      for (i=0; i<RGU_MAX_TB; i++) 
      {
         /*rgu_c_001.main_5 - ADD - L2M Support */
#ifdef LTE_L2_MEAS
         /* After Merging from 2.1 to 2.2 */
         retVal = cmUnpkRguDatReqTb(pst, &datReq->datReqTb[i], mBuf);
         if(retVal != ROK)
         {
            RETVALUE(RFAILED);
         }
#else
         CMCHKUNPK(cmUnpkRguDatReqTb, &datReq->datReqTb[i], mBuf);
#endif
      }
   }
   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 cmPkRguCDatIndInfo
(
RguCDatIndInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkRguCDatIndInfo(param, mBuf)
RguCDatIndInfo *param;
Buffer *mBuf;
#endif
{
   MsgLen msgLen;

   TRC3(cmPkRguCDatIndInfo);

   if (SFndLenMsg(param->pdu, &msgLen) != ROK)
      RETVALUE(RFAILED);
   if (SCatMsg(mBuf, param->pdu, M1M2) != ROK)
      RETVALUE(RFAILED);
   SPutMsg(param->pdu);
   CMCHKPK(cmPkMsgLen, msgLen, mBuf);
   CMCHKPK(cmPkLteLcId, param->lcId, mBuf);
   CMCHKPK(cmPkLteRnti, param->rnti, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 cmUnpkRguCDatIndInfo
(
RguCDatIndInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkRguCDatIndInfo(param, mBuf)
RguCDatIndInfo *param;
Buffer *mBuf;
#endif
{
   MsgLen msgLen, totalMsgLen;

   TRC3(cmUnpkRguCDatIndInfo);

   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->rnti, mBuf);
   CMCHKUNPK(cmUnpkLteLcId, &param->lcId, mBuf);
   CMCHKUNPK(cmUnpkMsgLen, &msgLen, mBuf);
   if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
      RETVALUE(RFAILED);
   if (SSegMsg(mBuf, totalMsgLen-msgLen, &param->pdu) != ROK)
      RETVALUE(RFAILED);
   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 cmPkRguLchDatInd
(
RguLchDatInd *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkRguLchDatInd(param, mBuf)
RguLchDatInd *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkRguLchDatInd);

      CMCHKPK(cmPkRguPduInfo, &param->pdu, mBuf);
   CMCHKPK(cmPkLteLcId, param->lcId, mBuf);
   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 cmUnpkRguLchDatInd
(
RguLchDatInd *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkRguLchDatInd(param, mBuf)
RguLchDatInd *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkRguLchDatInd);

   CMCHKUNPK(cmUnpkLteLcId, &param->lcId, mBuf);
   CMCHKUNPK(cmUnpkRguPduInfo, &param->pdu, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : packRlcMacDataInfo
*
*
*     Desc : RlcMacData
 * Data Indication from MAC to RLC for dedicated channels of a UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 packRlcMacDataInfo
(
RlcMacData *param,
Buffer *mBuf
)
#else
PUBLIC S16 packRlcMacDataInfo(param, mBuf)
RlcMacData *param;
Buffer *mBuf;
#endif
{
   S32 i;
   MsgLen msgLen;

   TRC3(packRlcMacDataInfo);

   for (i=param->nmbPdu-1; i >= 0; i--)
   {
      MsgLen msgLen = 0;
      if (SFndLenMsg(param->pduInfo[i].pduBuf, &msgLen) != ROK)
         RETVALUE(RFAILED);
      if (SCatMsg(mBuf, param->pduInfo[i].pduBuf, M1M2) != ROK)
          RETVALUE(RFAILED);      
      CMCHKPK(cmPkMsgLen, msgLen, mBuf);
      CMCHKPK(cmPkLteLcId, param->pduInfo[i].lcId, mBuf);
      CMCHKPK(cmPkBool, param->pduInfo[i].commCh, mBuf);
   }
   CMCHKPK(SPkU8, param->nmbPdu, mBuf);
   CMCHKPK(cmPkLteRnti, param->rnti, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteTimingInfo, &param->timeToTx, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : unpackRlcMacDataInfo
*
*
*     Desc : RlcMacData
 * Data Indication from MAC to RLC for dedicated channels of a UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 unpackRlcMacDataInfo
(
RlcMacData *param,
Buffer *mBuf
)
#else
PUBLIC S16 unpackRlcMacDataInfo(param, mBuf)
RlcMacData *param;
Buffer *mBuf;
#endif
{
   S32 i;

   TRC3(unpackRlcMacDataInfo);
   
   CMCHKUNPK(cmUnpkLteTimingInfo, &param->timeToTx, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->rnti, mBuf);
   CMCHKUNPK(SUnpkU8, &param->nmbPdu, mBuf);
   for (i=0; i<param->nmbPdu; i++) 
   {
      MsgLen totalMsgLen;

      CMCHKUNPK(cmUnpkBool, &param->pduInfo[i].commCh, mBuf);
      CMCHKUNPK(cmUnpkLteLcId, &param->pduInfo[i].lcId, mBuf);
      CMCHKUNPK(cmPkMsgLen, &param->pduInfo[i].pduLen, mBuf);
      if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
         RETVALUE(RFAILED);
      if (SSegMsg(mBuf, totalMsgLen-param->pduInfo[i].pduLen, &param->pduInfo[i].pduBuf) != ROK)
         RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 cmPkRguCStaRspInfo
(
RguCStaRspInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkRguCStaRspInfo(param, mBuf)
RguCStaRspInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkRguCStaRspInfo);

   switch(param->lcType) {
      case CM_LTE_LCH_CCCH:
         CMCHKPK(cmPkLteRnti, param->u.rnti, mBuf);
         break;
      case CM_LTE_LCH_BCCH:
      case CM_LTE_LCH_PCCH:
         CMCHKPK(cmPkLteTimingInfo, &param->u.timeToTx, mBuf);
#ifdef EMTC_ENABLE
         CMCHKPK(SPkU8,param->pnb,mBuf);
         CMCHKPK(SPkU8,param->emtcDiReason,mBuf);
#endif
         break;
      default :
         RETVALUE(RFAILED);
   }
   CMCHKPK(SPkS32, param->bo, mBuf);
   CMCHKPK(cmPkLteLcType, param->lcType, mBuf);
   CMCHKPK(cmPkLteLcId, param->lcId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 cmUnpkRguCStaRspInfo
(
RguCStaRspInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkRguCStaRspInfo(param, mBuf)
RguCStaRspInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkRguCStaRspInfo);

   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteLcId, &param->lcId, mBuf);
   CMCHKUNPK(cmUnpkLteLcType, &param->lcType, mBuf);
   CMCHKUNPK(SUnpkS32, &param->bo, mBuf);
   switch(param->lcType) {
      case CM_LTE_LCH_BCCH:
      case CM_LTE_LCH_PCCH:
#ifdef EMTC_ENABLE
         CMCHKUNPK(SUnpkU8,&param->emtcDiReason , mBuf);
         CMCHKUNPK(SUnpkU8,&param->pnb , mBuf);
#endif
         CMCHKUNPK(cmUnpkLteTimingInfo, &param->u.timeToTx, mBuf);
         break;
      case CM_LTE_LCH_CCCH:
         CMCHKUNPK(cmUnpkLteRnti, &param->u.rnti, mBuf);
         break;
      default :
         RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : packBOStatusInfo
*
*
*     Desc : RlcMacBOStatus
 * Status Response from RLC to MAC  for dedicated logical channel
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 packBOStatusInfo
(
RlcMacBOStatus *param,
Buffer *mBuf
)
#else
PUBLIC S16 packBOStatusInfo(param, mBuf)
RlcMacBOStatus *param;
Buffer *mBuf;
#endif
{

   TRC3(packBOStatusInfo);

   CMCHKPK(SPkS32, param->bo, mBuf);
   CMCHKPK(cmPkLteLcId, param->lcId, mBuf);
   CMCHKPK(cmPkBool, param->commCh, mBuf);
   CMCHKPK(cmPkLteRnti, param->rnti, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   RETVALUE(ROK);
} /* End of packBOStatusInfo */



/***********************************************************
*
*     Func : unpackBOStatusInfo
*
*
*     Desc : RlcMacBOStatus
 * Status Response from RLC to MAC  for dedicated logical channel
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 unpackBOStatusInfo
(
RlcMacBOStatus *param,
Buffer *mBuf
)
#else
PUBLIC S16 unpackBOStatusInfo(param, mBuf)
RlcMacBOStatus *param;
Buffer *mBuf;
#endif
{

   TRC3(unpackBOStatusInfo);

   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->rnti, mBuf);
   CMCHKUNPK(cmUnpkBool, &param->commCh, mBuf);
   CMCHKUNPK(cmUnpkLteLcId, &param->lcId, mBuf);
   CMCHKUNPK(SUnpkS32, &param->bo, mBuf);
   RETVALUE(ROK);
} /* End of unpackBOStatusInfo */


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
#ifdef ANSI
PUBLIC S16 cmPkRguLchStaInd
(
RguLchStaInd *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkRguLchStaInd(param, mBuf)
RguLchStaInd *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkRguLchStaInd);

   CMCHKPK(SPkS16, param->totBufSize, mBuf);
   CMCHKPK(cmPkLteLcId, param->lcId, mBuf);
   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 cmUnpkRguLchStaInd
(
RguLchStaInd *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkRguLchStaInd(param, mBuf)
RguLchStaInd *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkRguLchStaInd);

   CMCHKUNPK(cmUnpkLteLcId, &param->lcId, mBuf);
   CMCHKUNPK(SUnpkS16, &param->totBufSize, mBuf);
   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 cmPkRguStaIndTb
(
RguStaIndTb *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkRguStaIndTb(param, mBuf)
RguStaIndTb *param;
Buffer *mBuf;
#endif
{
   S32 i;

   TRC3(cmPkRguStaIndTb);

#ifdef LTE_L2_MEAS
   CMCHKPK(SPkU16, param->status, mBuf);
   CMCHKPK(SPkU32, param->tbId, mBuf);
#endif   
   for (i=param->nmbLch-1; i >= 0; i--) {
      CMCHKPK(cmPkRguLchStaInd, &param->lchStaInd[i], mBuf);
   }
   CMCHKPK(SPkU8, param->nmbLch, mBuf);
   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 cmUnpkRguStaIndTb
(
RguStaIndTb *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkRguStaIndTb(param, mBuf)
RguStaIndTb *param;
Buffer *mBuf;
#endif
{
   S32 i;

   TRC3(cmUnpkRguStaIndTb);

   CMCHKUNPK(SUnpkU8, &param->nmbLch, mBuf);
   for (i=0; i<param->nmbLch; i++) {
      CMCHKUNPK(cmUnpkRguLchStaInd, &param->lchStaInd[i], mBuf);
   }
#ifdef LTE_L2_MEAS
   CMCHKUNPK(SUnpkU32, &param->tbId, mBuf);
   CMCHKUNPK(SUnpkU16, &param->status, mBuf);
#endif   
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : packSchedRepInfo
*
*
*     Desc : RlcMacSchedRep
 * StaInd from MAC to RLC for dedicated logical channels of a UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 packSchedRepInfo
(
RlcMacSchedRep *param,
Buffer *mBuf
)
#else
PUBLIC S16 packSchedRepInfo(param, mBuf)
RlcMacSchedRep *param;
Buffer *mBuf;
#endif
{
   S32 i;
   S32 idx;

   TRC3(packSchedRepInfo);

   for(idx = (param->nmbLch-1); idx >= 0; idx--)
   {
      CMCHKPK(cmPkRguLchStaInd, &param->lchSta[idx].lchStaInd, mBuf);
      CMCHKPK(cmPkBool, param->lchSta[idx].commCh, mBuf);
   }
   CMCHKPK(SPkU8, param->nmbLch, mBuf);
   CMCHKPK(cmPkLteRnti, param->rnti, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteTimingInfo, &param->timeToTx, mBuf);

   RETVALUE(ROK);
} /* End of packSchedRepInfo */



/***********************************************************
*
*     Func : unpackSchedRepInfo
*
*
*     Desc : RlcMacSchedRep
 * StaInd from MAC to RLC for dedicated logical channels of a UE
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 unpackSchedRepInfo
(
RlcMacSchedRep *param,
Buffer *mBuf
)
#else
PUBLIC S16 unpackSchedRepInfo(param, mBuf)
RlcMacSchedRep *param;
Buffer *mBuf;
#endif
{
   S32 i;
   S32 idx;

   TRC3(unpackSchedRepInfo);

   CMCHKUNPK(cmUnpkLteTimingInfo, &param->timeToTx, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->rnti, mBuf);
   CMCHKUNPK(SUnpkU8, &param->nmbLch, mBuf);
   for(idx = 0; idx < param->nmbLch; idx++)
   {
      CMCHKUNPK(cmUnpkBool, &param->lchSta[idx].commCh, mBuf);
      CMCHKUNPK(cmUnpkRguLchStaInd, &param->lchSta[idx].lchStaInd, mBuf);
   }

   RETVALUE(ROK);
} /* End of unpackSchedRepInfo */

#endif

/**********************************************************************
         End of file
**********************************************************************/
