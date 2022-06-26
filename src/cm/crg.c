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

#endif
/**********************************************************************
         End of file
**********************************************************************/
