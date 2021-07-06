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
 
     Name:     CTF Interface
 
     Type:     C file
 
     Desc:     This file contains the packing/unpacking functions
               for control plane primitives on CTF interface.

     File:     ctf.c

*********************************************************************21*/

/* header include files (.h) */
#include "common_def.h"
#include "ctf.h"           /* CTF interface defines           */

/* header/extern include files (.x) */
#ifdef TENB_AS_SECURITY
#include "cm_mblk.x"
#endif
#include "ctf.x"          

#ifdef TENB_AS_SECURITY
#define cmPkCtfCrnti oduUnpackUInt16
#define cmPkCtfCellId oduUnpackUInt16
#define cmUnpkCtfCellId oduPackUInt16
#define cmUnpkCtfCrnti oduPackUInt16
#endif


#if (defined(LCCTF) || defined(LWLCCTF))


/**
* @brief Request from RRC to PHY to bind the CTF interface SAP.
*
* @details
*
*     Function: cmPkCtfBndReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   SpId  spId
*  @return   S16
*      -# ROK
**/
S16 cmPkCtfBndReq
(
Pst* pst,
SuId suId,
SpId spId
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF001, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF002, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF003, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   pst->event = (Event) EVTCTFBNDREQ;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief Request from RRC to PHY to bind the CTF interface SAP.
*
* @details
*
*     Function: cmUnpkCtfBndReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   SpId  spId
*  @return   S16
*      -# ROK
**/
S16 cmUnpkCtfBndReq
(
CtfBndReq func,
Pst *pst,
Buffer *mBuf
)
{
   SuId suId = 0;
   SpId spId = 0;
   

   if (SUnpkS16(&suId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF004, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (SUnpkS16(&spId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF005, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, suId, spId));
}


/**
* @brief Confirmation from PHY to RRC for the bind 
 * request for the CTF interface SAP.
*
* @details
*
*     Function: cmPkCtfBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   uint8_t  status
*  @return   S16
*      -# ROK
**/
S16 cmPkCtfBndCfm
(
Pst* pst,
SuId suId,
uint8_t status
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF006, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (oduUnpackUInt8(status, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF007, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF008, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   pst->event = (Event) EVTCTFBNDCFM;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief Confirmation from PHY to RRC for the bind 
 * request for the CTF interface SAP.
*
* @details
*
*     Function: cmUnpkCtfBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   uint8_t  status
*  @return   S16
*      -# ROK
**/
S16 cmUnpkCtfBndCfm
(
CtfBndCfm func,
Pst *pst,
Buffer *mBuf
)
{
   SuId suId = 0;
   uint8_t status;
   

   if (SUnpkS16(&suId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF009, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (oduPackUInt8(&status, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF010, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, suId, status));
}


/**
* @brief Request from RRC to PHY to Unbind the CTF interface SAP.
*
* @details
*
*     Function: cmPkCtfUbndReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   Reason  reason
*  @return   S16
*      -# ROK
**/
S16 cmPkCtfUbndReq
(
Pst* pst,
SpId spId,
Reason reason
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF011, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (SPkS16(reason, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF012, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF013, (ErrVal)0,(Txt*)&"Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   pst->event = (Event) EVTCTFUBNDREQ;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief Request from RRC to PHY to Unbind the CTF interface SAP.
*
* @details
*
*     Function: cmUnpkCtfUbndReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   Reason  reason
*  @return   S16
*      -# ROK
**/
S16 cmUnpkCtfUbndReq
(
CtfUbndReq func,
Pst *pst,
Buffer *mBuf
)
{
   SpId spId = 0;
   Reason reason = 0;
   

   if (SUnpkS16(&spId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF014, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (SUnpkS16(&reason, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF015, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, spId, reason));
}


/***********************************************************
*
*     Func: cmRelCtfCfgReq
*
*
*     Desc:  This fucntion will release the memory used for ctf cfg req
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
static S16 cmRelCtfCfgReq
(
Pst  *pst,
CtfCfgReqInfo *cfgReqInfo
)
{
   /* ctf_c_001.main_4: Added support for vendor specific parameters */
#ifdef CTF_VER3
   if(cfgReqInfo->vendorParams.paramBuffer != NULLP) {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, 
                   (Data *)cfgReqInfo->vendorParams.paramBuffer,
                   cfgReqInfo->vendorParams.buffLen);
   }
#endif
   SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgReqInfo, sizeof(CtfCfgReqInfo)); 
   return ROK;
}

#ifdef TENB_AS_SECURITY
/***********************************************************
*
*     Func: cmPkCtfNhKeyInfo
*
*
*     Desc:   Pack keNB* input parameter information
 * @details This is one of the main structure in the KDF request primitive.
 *    -# This structure contains the configuration information as given by 
 *       the Application to trigger key derivation at PHY layer for generation of kENB*.
 *    -# It take Next Hop key, DL Frequency and Physical cell Id as input.
 *    -# The result of configuration is indicated in KdfCfgCfm.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfNhKeyInfo
(
CtfNhKeyInfo *param,
Buffer *mBuf
)
{

   S16  idx;

         
   for(idx=31;idx>=0;idx--)
   {
      CMCHKPK(oduUnpackUInt8,param->secKey[idx],mBuf);
   }
   CMCHKPK(oduUnpackUInt16, param->physCellId, mBuf);
   CMCHKPK(oduUnpackUInt16, param->dlCarrFreq, mBuf);

   return ROK;
}

/***********************************************************
*
*     Func: cmPkCtfKenbStarInfo
*
*
*     Desc:   Pack keNB* input parameter information
 * @details This is one of the main structure in the KDF request primitive.
 *    -# This structure contains the configuration information as given by 
 *       the Application to trigger key derivation at PHY layer for generation of kENB*.
 *    -# It take existing kENB, DL Frequency and Physical cell Id as input.
 *    -# The result of configuration is indicated in KdfCfgCfm.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfKenbStarInfo
(
CtfKenbStarInfo *param,
Buffer *mBuf
)
{

   S16  idx;

         
   for(idx=31;idx>=0;idx--)
   {
      CMCHKPK(oduUnpackUInt8,param->secKey[idx],mBuf);
   }
   CMCHKPK(oduUnpackUInt16, param->physCellId, mBuf);
   CMCHKPK(oduUnpackUInt16, param->dlCarrFreq, mBuf);
   
   return ROK;
}


/***********************************************************
*
*     Func: cmPkCtfAskeyInfo
*
*
*     Desc:   Pack AS key generation input parameter information.
 * @details This is one of the main structure in the KDF request primitive.
 *    -# This structure contains the configuration information as given by 
 *       the Application to trigger key derivation at PHY layer for generation of AS Security keys.
 *    -# It take existing kENB, and integrity/cipherig Algorithm as input.
 *    -# The result of configuration is indicated in KdfCfgCfm.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfAskeyInfo
(
CtfAsKeyInfo *param,
Buffer *mBuf
)
{
   S16  idx;
         
   for(idx=31;idx>=0;idx--)
   {
      CMCHKPK(oduUnpackUInt8,param->secKey[idx],mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->ciphAlgoType, mBuf);
   CMCHKPK(oduUnpackUInt8, param->intgAlgoType, mBuf);

   return ROK;
}

/***********************************************************
*
*     Func: cmPkCtfKdfReqInfo
*
*
*     Desc:   Pack one of the 3 Key Derivation types.
 * @details This is the encompassing structure in the KDF request primitive.
 *    -# This structure contains the configuration information as given by 
 *       the Application to trigger key derivation at PHY layer for generation of keys.
 *    -# It takes key derivation type as input.
 *    -# The result of configuration is indicated in KdfCfgCfm.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfKdfReqInfo
(
CtfKdfReqInfo *param,
Buffer *mBuf
)
{


   switch (param->keyDerType)
    {
         case CTF_KDF_TYPE1:
             CMCHKPK(cmPkCtfAskeyInfo, &param->u.kEnbInf, mBuf);
             break;
         case CTF_KDF_TYPE2:
            CMCHKPK(cmPkCtfKenbStarInfo, &param->u.kEnbStarInf, mBuf);
            break;
         case CTF_KDF_TYPE3:
            CMCHKPK(cmPkCtfNhKeyInfo , &param->u.nhKeyInf, mBuf);
             break;
         default :
            return RFAILED;
    }
    CMCHKPK(oduUnpackUInt8, param->keyDerType, mBuf);

   return ROK;
}

/**
* @brief Primitive Request from Application to PHY for 
 * security key derivation.
*
* @details
*
*     Function: cmPkCtfKdfReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   CtfCfgTransId  transId
*  @param[in]   CtfCfgReqInfo*  cfgReqInfo
*  @return   S16
*      -# ROK
**/
S16 cmPkCtfKdfReq
(
Pst* pst,
SpId spId,
CtfCfgTransId transId,
CtfKdfReqInfo* kdfReqInfo
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF016, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)kdfReqInfo, sizeof(CtfKdfReqInfo));
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LC) {
      if (cmPkCtfKdfReqInfo(kdfReqInfo, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF017, (ErrVal)0, (Txt*)&"Packing failed");
#endif
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)kdfReqInfo, sizeof(CtfKdfReqInfo));
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   else if (pst->selector == ODU_SELECTOR_LWLC) {
   }
   if (cmPkCtfCfgTransId(&transId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF019, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)kdfReqInfo, sizeof(CtfKdfReqInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF020, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)kdfReqInfo, sizeof(CtfKdfReqInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (pst->selector != ODU_SELECTOR_LWLC) {
      if (SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)kdfReqInfo, sizeof(CtfKdfReqInfo)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF021, (ErrVal)0, (Txt*)&"Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   pst->event = (Event) EVTCTFKDFREQ;
   return (SPstTsk(pst,mBuf));
}
#endif

/********************************************************************************************************************************/
                              /*START OF CNM PACK & UNPACK FUNCTIONS*/
/********************************************************************************************************************************/
S16 cmPkCtfCnmVendorSpecificInfo
(
CtfCnmVendorSpecificInfo *param,
Buffer *mBuf
)
{
   S32 i;
   if(param->numOfVendorSpecifcParam <= CTF_CNM_MAX_VENDOR_PARAMS)
   {
      for(i= param->numOfVendorSpecifcParam-1; i >= 0; i--)
      {
         CMCHKPK(oduUnpackUInt32,(uint32_t)param->vendorSpecifcParam[i], mBuf);
      }
   }
   CMCHKPK(oduUnpackUInt16, param->numOfVendorSpecifcParam, mBuf);
   return ROK;
}

S16 cmPkCtfCtfCnmCellSyncReq
(
CtfCnmCellSyncReq *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt16, param->nbrEarfcn, mBuf);
   CMCHKPK(oduUnpackUInt16, param->nbrPCellId, mBuf);
   CMCHKPK(oduUnpackUInt8, param->nbrTxAntCount, mBuf);
   CMCHKPK(oduUnpackUInt8, param->nbrCellCp, mBuf);
   CMCHKPK(oduUnpackUInt8, param->nbrCellNRb, mBuf);
   CMCHKPK(oduUnpackUInt8, param->nbrSpecSfCfg, mBuf);
   CMCHKPK(cmPkCtfCnmVendorSpecificInfo, &param->ctfCnmVendorSpecificInfo, mBuf);
   return ROK;

}

S16 cmPkCtfcfgReqInfo
(
CtfCnmInitSyncReq *param,
Buffer *mBuf
)
{
   S32 i;

   CMCHKPK(oduUnpackUInt8, param->searchType, mBuf);
   CMCHKPK(oduUnpackUInt8, param->mibRequest, mBuf);
   CMCHKPK(oduUnpackUInt16, param->earfcn, mBuf);
   CMCHKPK(oduUnpackUInt8, param->measBandWidth, mBuf);
   if(param->numOfPciList <= CTF_CNM_MAX_CELL_SEARCH)
   { 
      for(i= param->numOfPciList-1; i >= 0; i--)
      {
         CMCHKPK(cmPkCtfCtfCnmCellSyncReq, &param->pciList[i], mBuf);
      }
   }
   CMCHKPK(oduUnpackUInt16, param->numOfPciList, mBuf);
   return ROK;

}
S16 cmUnpkCtfCnmVendorSpecificInfo
(
CtfCnmVendorSpecificInfo *param,
Buffer *mBuf
)
{
   S32 i;

   CMCHKUNPK(oduPackUInt16, &param->numOfVendorSpecifcParam, mBuf);
   for(i=0; (i<param->numOfVendorSpecifcParam)&&(i < CTF_CNM_MAX_VENDOR_PARAMS); i++)
   {
      CMCHKUNPK(oduPackUInt32, &param->vendorSpecifcParam[i], mBuf);
   }
   return ROK;
}

S16 cmUnpkCtfCtfCnmCellSyncReq
(
CtfCnmCellSyncReq *param,
Buffer *mBuf
)
{

   CMCHKUNPK(cmUnpkCtfCnmVendorSpecificInfo, &param->ctfCnmVendorSpecificInfo, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->nbrSpecSfCfg, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->nbrCellNRb, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->nbrCellCp, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->nbrTxAntCount, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->nbrPCellId, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->nbrEarfcn, mBuf);
   return ROK;

}

S16 cmUnpkCtfsyncReqInfo
(
CtfCnmInitSyncReq *param,
Buffer *mBuf
)
{
   S32 i;

   CMCHKUNPK(oduPackUInt16, &param->numOfPciList, mBuf);
   for(i=0; (i < param->numOfPciList)&&(i < CTF_CNM_MAX_CELL_SEARCH); i++)
   {
      CMCHKUNPK(cmUnpkCtfCtfCnmCellSyncReq, &param->pciList[i], mBuf);
   }

   CMCHKUNPK(oduPackUInt8, &param->measBandWidth, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->earfcn, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->mibRequest, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->searchType, mBuf);
   return ROK;
}
/**
* @brief Configuration Request from App to CL for
 * cnm Cell Sync request.
*
* @details
*
*     Function: cmPkCtfCnmCellSyncReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   CtfCfgTransId  * transId
*  @param[in]   CtfCnmCellSyncReq* ctfcnmCellSyncReq
*  @return   S16
*      -# ROK
**/
S16 cmPkCtfCnmCellSyncReq
(
Pst* pst,
SpId spId,
CtfCfgTransId *transId,
CtfCnmCellSyncReq * cfgReqInfo
)
{

   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF016, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgReqInfo, sizeof(CtfCnmCellSyncReq));
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LC) {
      if (cmPkCtfCtfCnmCellSyncReq(cfgReqInfo, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF017, (ErrVal)0, (Txt*)&"Packing failed");
#endif
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgReqInfo, sizeof(CtfCnmCellSyncReq));
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   else if (pst->selector == ODU_SELECTOR_LWLC) {

   if (oduPackPointer((PTR)cfgReqInfo, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF018, (ErrVal)0, "Packing failed");
#endif
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgReqInfo, sizeof(CtfCnmCellSyncReq));
         SPutMsg(mBuf);
         return RFAILED;
      }

   }
   if(cmPkCtfCfgTransId(transId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF019, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgReqInfo, sizeof(CtfCnmCellSyncReq));
      SPutMsg(mBuf);
      return RFAILED;
   }
 if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF020, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgReqInfo, sizeof(CtfCnmCellSyncReq));
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (pst->selector != ODU_SELECTOR_LWLC) {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgReqInfo, sizeof(CtfCnmCellSyncReq));
   }
   pst->event = (Event)EVTCTFCNMSYNCREQ;
   return (SPstTsk(pst,mBuf));

}
/**
* @brief Configuration Request from RRC to PHY for
 * cell or dedicated configuration.
*
* @details
*
*     Function: cmUnpkCtfCnmCellSyncReq
*
*  @param[in]   CtfCnmCellSyncReqMsg func
*  @param[in]   Pst*  pst
*  @param[in]   Buffer *mBuf
*  @return   S16
*      -# ROK
**/
S16 cmUnpkCtfCnmCellSyncReq
(
CtfCnmCellSyncReqMsg func,
Pst *pst,
Buffer *mBuf
)
{

   SpId spId = 0;
   CtfCfgTransId transId;
   CtfCnmCellSyncReq *SyncReqMsg = NULLP;

   if (SUnpkS16(&spId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF022, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (cmUnpkCtfCfgTransId(&transId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF023, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (pst->selector != ODU_SELECTOR_LWLC) {
      if ((SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&SyncReqMsg, sizeof(CtfCnmCellSyncReq))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF024, (ErrVal)0, (Txt*)&"Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   if (pst->selector == ODU_SELECTOR_LWLC) {
      if (oduUnpackPointer((PTR *)&SyncReqMsg, mBuf) != ROK) {
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
  else if (pst->selector == ODU_SELECTOR_LC)
      if (cmUnpkCtfCtfCnmCellSyncReq(SyncReqMsg, mBuf) != ROK) {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)SyncReqMsg, sizeof(CtfCnmCellSyncReq));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF025, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, spId, &transId, SyncReqMsg));

}
/*-----------------------------------------------------*/


/**
* @brief Configuration Request from App to CL for
 * cnm Init Sync request.
*
* @details
*
*     Function: cmPkCtfCnmInitSyncReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   CtfCfgTransId  * transId
*  @param[in]   CtfCnmInitSyncReq* ctfcnmInitSyncReq
*  @return   S16
*      -# ROK
**/
S16 cmPkCtfCnmInitSyncReq
(
Pst* pst,
SpId spId,
CtfCfgTransId *transId,
CtfCnmInitSyncReq * cfgReqInfo
)
{
   Buffer *mBuf = NULLP;


   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF016, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgReqInfo, sizeof(CtfCnmInitSyncReq));
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LC) {
      if (cmPkCtfcfgReqInfo(cfgReqInfo, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF017, (ErrVal)0, (Txt*)&"Packing failed");
#endif
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgReqInfo, sizeof(CtfCnmInitSyncReq));
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   else if (pst->selector == ODU_SELECTOR_LWLC) {

   if (oduPackPointer((PTR)cfgReqInfo, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF018, (ErrVal)0, "Packing failed");
#endif
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgReqInfo, sizeof(CtfCnmInitSyncReq));
         SPutMsg(mBuf);
         return RFAILED;
      }

   }
   if(cmPkCtfCfgTransId(transId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF019, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgReqInfo, sizeof(CtfCnmInitSyncReq));
      SPutMsg(mBuf);
      return RFAILED;
   }
 if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF020, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgReqInfo, sizeof(CtfCnmInitSyncReq));
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (pst->selector != ODU_SELECTOR_LWLC) {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgReqInfo, sizeof(CtfCnmInitSyncReq));
   }
   pst->event = (Event)EVTCTFCNMINITSYNCREQ;
   return (SPstTsk(pst,mBuf));

}
/*-----------------------------------------------------------------*/

/**
* @brief Configuration Request from RRC to PHY for
 * cell or dedicated configuration.
*
* @details
*
*     Function: cmUnpkCtfCnmInitSyncReq
*
*  @param[in]   CtfCnmInitSyncReqMsg func
*  @param[in]   Pst*  pst
*  @param[in]   Buffer *mBuf
*  @return   S16
*      -# ROK
**/
S16 cmUnpkCtfCnmInitSyncReq
(
CtfCnmInitSyncReqMsg func,
Pst *pst,
Buffer *mBuf
)
{

   SpId spId = 0;
   CtfCfgTransId transId;
   CtfCnmInitSyncReq *syncReqInfo = NULLP;

   if (SUnpkS16(&spId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF022, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (cmUnpkCtfCfgTransId(&transId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF023, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (pst->selector != ODU_SELECTOR_LWLC) {
      if ((SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&syncReqInfo, sizeof(CtfCnmInitSyncReq))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF024, (ErrVal)0, (Txt*)&"Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   if (pst->selector == ODU_SELECTOR_LWLC) {
      if (oduUnpackPointer((PTR *)&syncReqInfo, mBuf) != ROK) {
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
  else if (pst->selector == ODU_SELECTOR_LC)
      if (cmUnpkCtfsyncReqInfo(syncReqInfo, mBuf) != ROK) {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)syncReqInfo, sizeof(CtfCnmInitSyncReq));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF025, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, spId, &transId, syncReqInfo));
}
/*-----------------------------------------------------------------------------*/

S16 cmPkCtfctfCnmMibInfo
(
CtfCnmMibInfo *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt16, param->sfn, mBuf);
   CMCHKPK(oduUnpackUInt8, param->numTxAntennas, mBuf);
   CMCHKPK(oduUnpackUInt8, param->phichDuration, mBuf);
   CMCHKPK(oduUnpackUInt8, param->phichResource, mBuf);
   CMCHKPK(oduUnpackUInt8, param->dlSystemBandWidht, mBuf);
   return ROK;

}

S16 cmPkCtfctfCnmInitSyncInfo
(
CtfCnmInitSyncInfo *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt16, param->pci, mBuf);
   CMCHKPK(oduUnpackUInt8, param->rsrp, mBuf);
   CMCHKPK(oduUnpackUInt8, param->mibValidFlag, mBuf);
   CMCHKPK(oduUnpackUInt16, param->localSfn, mBuf);
   CMCHKPK(cmPkCtfctfCnmMibInfo, &param->ctfCnmMibInfo, mBuf);
   CMCHKPK(cmPkCtfCnmVendorSpecificInfo, &param->ctfCnmVendorSpecificInfo, mBuf);
   return ROK;

}

S16 cmPkCtfcfgRspInfo
(
CtfCnmInitSyncRsp *param,
Buffer *mBuf
)
{
   S32 i;

   CMCHKPK(oduUnpackUInt8, param->status, mBuf);
   CMCHKPK(oduUnpackUInt16, param->earfcn, mBuf);
   if(param->numOfNeighbourInfo <= CTF_CNM_MAX_CELL_SEARCH)
   {
      for(i= param->numOfNeighbourInfo-1; i >= 0; i--)
      {
         CMCHKPK(cmPkCtfctfCnmInitSyncInfo, &param->ctfCnmInitSyncInfo[i], mBuf);
      }
   }
   CMCHKPK(oduUnpackUInt8, param->numOfNeighbourInfo, mBuf);
   return ROK;

}

/**
* @brief Configuration Request from App to CL for
 * cnm Init Sync responce
*
* @details
*
*     Function: cmPkCtfCnmInitSyncRsp
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   CtfCfgTransId  * transId
*  @param[in]   CtfCnmInitSyncRsp* ctfcnmInitSyncRsp
*  @return   S16
*      -# ROK
**/
S16 cmPkCtfCnmInitSyncRsp
(
Pst* pst,
SpId spId,
CtfCfgTransId *transId,
CtfCnmInitSyncRsp * cfgRspInfo
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF016, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgRspInfo, sizeof(CtfCnmInitSyncRsp));
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LC) {
      if (cmPkCtfcfgRspInfo(cfgRspInfo, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF017, (ErrVal)0, (Txt*)&"Packing failed");
#endif
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgRspInfo, sizeof(CtfCnmInitSyncRsp));
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   else if (pst->selector == ODU_SELECTOR_LWLC) {

   if (oduPackPointer((PTR)cfgRspInfo, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF018, (ErrVal)0, "Packing failed");
#endif
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgRspInfo, sizeof(CtfCnmInitSyncRsp));
         SPutMsg(mBuf);
         return RFAILED;
      }

   }
 if(cmPkCtfCfgTransId(transId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF019, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgRspInfo, sizeof(CtfCnmInitSyncRsp));
      SPutMsg(mBuf);
      return RFAILED;
   }
 if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF020, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgRspInfo, sizeof(CtfCnmInitSyncRsp));
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (pst->selector != ODU_SELECTOR_LWLC) {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgRspInfo, sizeof(CtfCnmInitSyncRsp));
   }
   pst->event = (Event)EVTCTFCNMINITSYNCRSP;
   return (SPstTsk(pst,mBuf));

}
/*-----------------------------------------------------------------------------*/

S16 cmUnpkCtfctfCnmMibInfo
(
CtfCnmMibInfo *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt8, &param->dlSystemBandWidht, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->phichResource, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->phichDuration, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->numTxAntennas, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->sfn, mBuf);
   return ROK;

}

S16 cmUnpkctfCnmInitSyncInfo
(
CtfCnmInitSyncInfo *param,
Buffer *mBuf
)
{

   CMCHKUNPK(cmUnpkCtfCnmVendorSpecificInfo, &param->ctfCnmVendorSpecificInfo, mBuf);
   CMCHKUNPK(cmUnpkCtfctfCnmMibInfo, &param->ctfCnmMibInfo, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->localSfn, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->mibValidFlag, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->rsrp, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->pci, mBuf);
   return ROK;

}

S16 cmUnpkCtfCnmInitSyncRspMsg
(
CtfCnmInitSyncRsp *param,
Buffer *mBuf
)
{
    S32 i;

    CMCHKUNPK(oduPackUInt8, &param->numOfNeighbourInfo, mBuf);
    for(i=0; (i < param->numOfNeighbourInfo)&&(i < CTF_CNM_MAX_CELL_SEARCH); i++)
    {
         CMCHKUNPK(cmUnpkctfCnmInitSyncInfo, &param->ctfCnmInitSyncInfo[i], mBuf);
    }
    CMCHKUNPK(oduPackUInt16, &param->earfcn, mBuf);
    CMCHKUNPK(oduPackUInt8, &param->status, mBuf);
    return ROK;

}
/**
* @brief Configuration Request from RRC to PHY for
 * cell or dedicated configuration.
*
* @details
*
*     Function: cmUnpkCtfCnmInitSyncRsp
*
*  @param[in]   CtfCnmInitSyncRspMsg func
*  @param[in]   Pst*  pst
*  @param[in]   Buffer *mBuf
*  @return   S16
*      -# ROK
**/
S16 cmUnpkCtfCnmInitSyncRsp
(
CtfCnmInitSyncRspMsg func,
Pst *pst,
Buffer *mBuf
)
{

   SpId spId = 0;
   CtfCfgTransId transId;
   CtfCnmInitSyncRsp *syncRspInfo = NULLP;

   if (SUnpkS16(&spId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF022, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (cmUnpkCtfCfgTransId(&transId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF023, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (pst->selector != ODU_SELECTOR_LWLC) {
      if((SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&syncRspInfo, sizeof(CtfCnmInitSyncRsp))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF024, (ErrVal)0, (Txt*)&"Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   if (pst->selector == ODU_SELECTOR_LWLC) {
      if(oduUnpackPointer((PTR *)&syncRspInfo, mBuf) != ROK) {
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
  else if (pst->selector == ODU_SELECTOR_LC)
      if(cmUnpkCtfCnmInitSyncRspMsg(syncRspInfo, mBuf) != ROK) {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)syncRspInfo, sizeof(CtfCnmInitSyncRsp));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF025, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, spId, &transId, syncRspInfo));

}
/*-----------------------------------------------------------------------*/

S16 cmPkCtfCellSyncRsp 
(
CtfCnmCellSyncRsp *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt8, param->status, mBuf);
   return ROK;

}
/**
* @brief Configuration Request from App to CL for
* cnm Init Sync responce.
*
* @details
*
*     Function: cmPkCtfCnmCellSyncRsp
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   CtfCfgTransId  * transId
*  @param[in]   CtfCnmCellSyncRsp* ctfcnmCellSyncRsp
*  @return   S16
*      -# ROK
**/
S16 cmPkCtfCnmCellSyncRsp
(
Pst* pst,
SpId spId,
CtfCfgTransId *transId,
CtfCnmCellSyncRsp * cfgRspInfo
)
{

   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF016, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgRspInfo, sizeof(CtfCnmCellSyncRsp));
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LC) {
      if (cmPkCtfCellSyncRsp(cfgRspInfo, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF017, (ErrVal)0, (Txt*)&"Packing failed");
#endif
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgRspInfo, sizeof(CtfCnmCellSyncRsp));
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   else if (pst->selector == ODU_SELECTOR_LWLC) {

   if (oduPackPointer((PTR)cfgRspInfo, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF018, (ErrVal)0, "Packing failed");
#endif
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgRspInfo, sizeof(CtfCnmCellSyncRsp));
         SPutMsg(mBuf);
         return RFAILED;
      }

   }
 if(cmPkCtfCfgTransId(transId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF019, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgRspInfo, sizeof(CtfCnmCellSyncRsp));
      SPutMsg(mBuf);
      return RFAILED;
   }
 if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF020, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgRspInfo, sizeof(CtfCnmCellSyncRsp));
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (pst->selector != ODU_SELECTOR_LWLC) {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgRspInfo, sizeof(CtfCnmCellSyncRsp));
   }
   pst->event = (Event)EVTCTFCNMSYNCRSP;
   return (SPstTsk(pst,mBuf));


}
/*------------------------------------------------------------*/

S16 cmUnpkCtfCnmSyncRspMsg
(
CtfCnmCellSyncRsp *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt8, &param->status, mBuf);
   return ROK;

}
/**
* @brief Configuration Request from RRC to PHY for
 * cell or dedicated configuration.
*
* @details
*
*     Function: cmUnpkCtfCnmCellSyncRsp
*
*  @param[in]   CtfCnmCellSyncRspMsg func
*  @param[in]   Pst*  pst
*  @param[in]   Buffer *mBuf
*  @return   S16
*      -# ROK
**/
S16 cmUnpkCtfCnmCellSyncRsp
(
CtfCnmCellSyncRspMsg func,
Pst *pst,
Buffer *mBuf
)
{

   SpId spId = 0;
   CtfCfgTransId transId;
   CtfCnmCellSyncRsp *syncRspMsg = NULLP;

   if (SUnpkS16(&spId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF022, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (cmUnpkCtfCfgTransId(&transId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF023, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (pst->selector != ODU_SELECTOR_LWLC) {
      if ((SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&syncRspMsg, sizeof(CtfCnmCellSyncRsp))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF024, (ErrVal)0, (Txt*)&"Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   if (pst->selector == ODU_SELECTOR_LWLC) {
      if (oduUnpackPointer((PTR *)&syncRspMsg, mBuf) != ROK) {
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
  else if (pst->selector == ODU_SELECTOR_LC)
      if (cmUnpkCtfCnmSyncRspMsg(syncRspMsg, mBuf) != ROK) {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)syncRspMsg, sizeof(CtfCnmCellSyncRsp));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF025, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, spId, &transId, syncRspMsg));

}
/*---------------------------------------------------------------*/

S16 cmPkCtfcfgIndInfo
(
CtfCnmCellSyncInd *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt8, param->status, mBuf);
   CMCHKPK(cmPkCtfCnmVendorSpecificInfo, &param->ctfCnmVendorSpecificInfo, mBuf);
   return ROK;

}
/**
* @brief Configuration Request from App to CL for
* cnm Init Sync responce.
*
* @details
*
*     Function: cmPkCtfCnmCellSyncInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   CtfCfgTransId  * transId
*  @param[in]   CtfCnmCellSyncInd* ctfcnmCellSyncInd
*  @return   S16
*      -# ROK
**/
S16 cmPkCtfCnmCellSyncInd
(
Pst* pst,
SpId spId,
CtfCfgTransId *transId,
CtfCnmCellSyncInd * cfgIndInfo
)
{
   
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF016, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgIndInfo, sizeof(CtfCnmCellSyncInd));
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LC) {
      if (cmPkCtfcfgIndInfo(cfgIndInfo, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF017, (ErrVal)0, (Txt*)&"Packing failed");
#endif
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgIndInfo, sizeof(CtfCnmCellSyncInd));
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   else if (pst->selector == ODU_SELECTOR_LWLC) {

   if (oduPackPointer((PTR)cfgIndInfo, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF018, (ErrVal)0, "Packing failed");
#endif
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgIndInfo, sizeof(CtfCnmCellSyncInd));
         SPutMsg(mBuf);
         return RFAILED;
      }

   }
 if(cmPkCtfCfgTransId(transId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF019, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgIndInfo, sizeof(CtfCnmCellSyncInd));
      SPutMsg(mBuf);
      return RFAILED;
   }
 if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF020, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgIndInfo, sizeof(CtfCnmCellSyncInd));
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (pst->selector != ODU_SELECTOR_LWLC) {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)cfgIndInfo, sizeof(CtfCnmCellSyncInd));
   }
   pst->event = (Event)EVTCTFCNMSYNCIND;
   return (SPstTsk(pst,mBuf));

}

/*---------------------------------------------------------------*/

S16 cmUnpkCtfCnmSyncIndMsg
(
CtfCnmCellSyncInd *param,
Buffer *mBuf
)
{

   CMCHKUNPK(cmUnpkCtfCnmVendorSpecificInfo, &param->ctfCnmVendorSpecificInfo, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->status, mBuf);
   return ROK;

}
/**
* @brief Configuration Request from RRC to PHY for
 * cell or dedicated configuration.
*
* @details
*
*     Function: cmUnpkCtfCnmCellSyncInd
*
*  @param[in]   CtfCnmCellSyncIndMsg func
*  @param[in]   Pst*  pst
*  @param[in]   Buffer *mBuf
*  @return   S16
*      -# ROK
**/
S16 cmUnpkCtfCnmCellSyncInd
(
CtfCnmCellSyncIndMsg func,
Pst *pst,
Buffer *mBuf
)
{

   SpId spId = 0;
   CtfCfgTransId transId;
   CtfCnmCellSyncInd *syncIndMsg = NULLP;

   if (SUnpkS16(&spId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF022, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (cmUnpkCtfCfgTransId(&transId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF023, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (pst->selector != ODU_SELECTOR_LWLC) {
      if ((SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&syncIndMsg, sizeof(CtfCnmCellSyncInd))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF024, (ErrVal)0, (Txt*)&"Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   if (pst->selector == ODU_SELECTOR_LWLC) {
      if (oduUnpackPointer((PTR *)&syncIndMsg, mBuf) != ROK) {
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
  else if (pst->selector == ODU_SELECTOR_LC)
      if (cmUnpkCtfCnmSyncIndMsg(syncIndMsg, mBuf) != ROK) {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)syncIndMsg, sizeof(CtfCnmCellSyncInd));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF025, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, spId, &transId, syncIndMsg));

}

/********************************************************************************************************************************/
                              /*END OF CNM PACK & UNPACK FUNCTIONS*/
/********************************************************************************************************************************/




/**
* @brief Configuration Request from RRC to PHY for 
 * cell or dedicated configuration.
*
* @details
*
*     Function: cmPkCtfCfgReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   CtfCfgTransId  transId
*  @param[in]   CtfCfgReqInfo*  cfgReqInfo
*  @return   S16
*      -# ROK
**/
S16 cmPkCtfCfgReq
(
Pst* pst,
SpId spId,
CtfCfgTransId transId,
CtfCfgReqInfo* cfgReqInfo
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF016, (ErrVal)0, "Packing failed");
#endif
      cmRelCtfCfgReq(pst, cfgReqInfo);     
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LC) {
      if (cmPkCtfCfgReqInfo(cfgReqInfo, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF017, (ErrVal)0, "Packing failed");
#endif
         cmRelCtfCfgReq(pst, cfgReqInfo);     
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   else if (pst->selector == ODU_SELECTOR_LWLC) {
      if (oduPackPointer((PTR)cfgReqInfo, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF018, (ErrVal)0, "Packing failed");
#endif
         cmRelCtfCfgReq(pst, cfgReqInfo);     
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   if (cmPkCtfCfgTransId(&transId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF019, (ErrVal)0, "Packing failed");
#endif
      cmRelCtfCfgReq(pst, cfgReqInfo);     
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF020, (ErrVal)0, "Packing failed");
#endif
      cmRelCtfCfgReq(pst, cfgReqInfo);     
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (pst->selector != ODU_SELECTOR_LWLC) {
         cmRelCtfCfgReq(pst, cfgReqInfo); 
   }
   pst->event = (Event) EVTCTFCFGREQ;
   return (SPstTsk(pst,mBuf));
}

#ifdef TENB_AS_SECURITY
/***********************************************************
*
*     Func: cmPkCtfKenbCfgInfo
*
*
*     Desc:   Key derivation funtion Input Information
 * @details This funtion packs the the CtfKenbCfgInfo structure
 *           which contains the eNodeB key and Algorithm Type for
 *           deriving AS security keys.
 *
 *
 *     Ret : S16
 *
 *     Notes:
 *
 *     File:
 *
 **********************************************************/
S16 cmUnpkCtfNhKeyInfo
(
CtfNhKeyInfo *param,
Buffer *mBuf
)
{

   uint8_t  idx;
        
   CMCHKUNPK(oduPackUInt16, &param->dlCarrFreq, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->physCellId, mBuf);
   for(idx=0;idx<32;idx++)
   {
      CMCHKUNPK(oduPackUInt8,&param->secKey[idx],mBuf);
   }
   return ROK;
}

/***********************************************************
*
*     Func: cmPkCtfKenbCfgInfo
*
*
*     Desc:   Key derivation funtion Input Information
 * @details This funtion packs the the CtfKenbCfgInfo structure
 *           which contains the eNodeB key and Algorithm Type for
 *           deriving AS security keys.
 *
 *
 *     Ret : S16
 *
 *     Notes:
 *
 *     File:
 *
 **********************************************************/
S16 cmUnpkCtfKenbStarInfo
(
CtfKenbStarInfo *param,
Buffer *mBuf
)
{

   uint8_t  idx;

   CMCHKUNPK(oduPackUInt16, &param->dlCarrFreq, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->physCellId, mBuf);
   for(idx=0;idx<32;idx++)
   {
      CMCHKUNPK(oduPackUInt8,&param->secKey[idx],mBuf);
   }
   return ROK;
}


/***********************************************************
*
*     Func: cmPkCtfKenbCfgInfo
*
*
*     Desc:   Key derivation funtion Input Information
 * @details This funtion packs the the CtfKenbCfgInfo structure
 *           which contains the eNodeB key and Algorithm Type for
 *           deriving AS security keys.
 *
 *
 *     Ret : S16
 *
 *     Notes:
 *
 *     File:
 *
 **********************************************************/
S16 cmUnpkCtfAskeyInfo
(
CtfAsKeyInfo *param,
Buffer *mBuf
)
{
   uint8_t  idx;

   CMCHKUNPK(oduPackUInt8, &param->intgAlgoType, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->ciphAlgoType, mBuf);
   for(idx=0;idx<32;idx++)
   {
      CMCHKUNPK(oduPackUInt8,&param->secKey[idx],mBuf);
   }
   return ROK;
}

/***********************************************************
 *
 *    Func: cmPkCtfKenbCfgInfo
 *
 *
 *    Desc:   Key derivation funtion Input Information
 * @details This funtion packs the the CtfKenbCfgInfo structure
 *           which contains the eNodeB key and Algorithm Type for
 *           deriving AS security keys.
 *
 *
 *     Ret : S16
 *
 *     Notes:
 *
 *     File:
 *
 *  **********************************************************/
S16 cmUnpkCtfKdfReqInfo
(
CtfKdfReqInfo *param,
Buffer *mBuf
)
{

    CMCHKUNPK(oduPackUInt8, &param->keyDerType, mBuf);
     switch (param->keyDerType)
    {
       case CTF_KDF_TYPE1:
           CMCHKUNPK(cmUnpkCtfAskeyInfo, &param->u.kEnbInf, mBuf);
           break;
       case CTF_KDF_TYPE2:
          CMCHKUNPK(cmUnpkCtfKenbStarInfo, &param->u.kEnbStarInf, mBuf);
          break;
       case CTF_KDF_TYPE3:
          CMCHKUNPK(cmUnpkCtfNhKeyInfo, &param->u.nhKeyInf, mBuf);
           break;
       default :
          return RFAILED;
    }
   return ROK;
}

/**
* @brief Configuration Request from RRC to PHY for 
 * cell or dedicated configuration.
*
* @details
*
*     Function: cmUnpkCtfCfgReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   CtfCfgTransId  transId
*  @param[in]   CtfCfgReqInfo*  cfgReqInfo
*  @return   S16
*      -# ROK
**/
S16 cmUnpkCtfKdfReq
(
CtfKdfReq func,
Pst *pst,
Buffer *mBuf
)
{
   SpId spId = 0;
   CtfCfgTransId transId;
   CtfKdfReqInfo *kdfReqInfo = NULLP;
   

   if (SUnpkS16(&spId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF022, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   
   if (cmUnpkCtfCfgTransId(&transId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF023, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   
   if (pst->selector != ODU_SELECTOR_LWLC) {
      if ((SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&kdfReqInfo, sizeof(CtfKdfReqInfo))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF024, (ErrVal)0, (Txt*)&"Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
      }
	  
   }
   
   if (pst->selector == ODU_SELECTOR_LWLC) {
      /*if (oduUnpackPointer((PTR *)&cfgReqInfo, mBuf) != ROK) {
         SPutMsg(mBuf);
         return RFAILED;
      }*/
   }
   else if (pst->selector == ODU_SELECTOR_LC) 
      if (cmUnpkCtfKdfReqInfo(kdfReqInfo, mBuf) != ROK) {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)kdfReqInfo, sizeof(CtfKdfReqInfo));
      SPutMsg(mBuf);
	  #if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF025, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
	   SPutMsg(mBuf);
   return ((*func)(pst, spId, transId, kdfReqInfo));
}
#endif



/**
* @brief Configuration Request from RRC to PHY for 
 * cell or dedicated configuration.
*
* @details
*
*     Function: cmUnpkCtfCfgReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   CtfCfgTransId  transId
*  @param[in]   CtfCfgReqInfo*  cfgReqInfo
*  @return   S16
*      -# ROK
**/
S16 cmUnpkCtfCfgReq
(
CtfCfgReq func,
Pst *pst,
Buffer *mBuf
)
{
   SpId spId = 0;
   CtfCfgTransId transId;
   CtfCfgReqInfo *cfgReqInfo = NULLP;
   

   if (SUnpkS16(&spId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF022, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (cmUnpkCtfCfgTransId(&transId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF023, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (pst->selector != ODU_SELECTOR_LWLC) {
      if ((SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&cfgReqInfo, sizeof(CtfCfgReqInfo))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF024, (ErrVal)0, (Txt*)&"Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   if (pst->selector == ODU_SELECTOR_LWLC) {
      if (oduUnpackPointer((PTR *)&cfgReqInfo, mBuf) != ROK) {
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   else if (pst->selector == ODU_SELECTOR_LC) 
      if (cmUnpkCtfCfgReqInfo(pst,cfgReqInfo, mBuf) != ROK) {
      cmRelCtfCfgReq(pst, cfgReqInfo);
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF025, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, spId, transId, cfgReqInfo));
}

#ifdef TENB_AS_SECURITY
/***********************************************************
*
*     Func: cmPkCtfKenbCfgInfo
*
*
*     Desc:   Key derivation funtion Input Information
 * @details This funtion packs the the CtfKenbCfgInfo structure
 *           which contains the eNodeB key and Algorithm Type for
 *           deriving AS security keys.
 *
 *
 *     Ret : S16
 *
 *     Notes:
 *
 *     File:
 *
 **********************************************************/
S16 cmPkCtfNhKeyCfmInfo
(
CtfNhKeyCfmInfo *param,
Buffer *mBuf
)
{

   S16  idx;
         
   for(idx=31;idx>=0;idx--)
   {
      CMCHKPK(oduUnpackUInt8,param->secKey[idx],mBuf);
   }
   return ROK;
}

/***********************************************************
*
*     Func: cmPkCtfKenbCfgInfo
*
*
*     Desc:   Key derivation funtion Input Information
 * @details This funtion packs the the CtfKenbCfgInfo structure
 *           which contains the eNodeB key and Algorithm Type for
 *           deriving AS security keys.
 *
 *
 *     Ret : S16
 *
 *     Notes:
 *
 *     File:
 *
 **********************************************************/
S16 cmPkCtfKenbStarCfmInfo
(
CtfKenbStarCfmInfo *param,
Buffer *mBuf
)
{

   S16  idx;

         
   for(idx=31;idx>=0;idx--)
   {
      CMCHKPK(oduUnpackUInt8,param->secKey[idx],mBuf);
   }
   return ROK;
}


/***********************************************************
*
*     Func: cmPkCtfKenbCfgInfo
*
*
*     Desc:   Key derivation funtion Input Information
 * @details This funtion packs the the CtfKenbCfgInfo structure
 *           which contains the eNodeB key and Algorithm Type for
 *           deriving AS security keys.
 *
 *
 *     Ret : S16
 *
 *     Notes:
 *
 *     File:
 *
 **********************************************************/
S16 cmPkCtfAskeyCfmInfo
(
CtfAskeyCfmInfo *param,
Buffer *mBuf
)
{

   S16  idx;

   for(idx=15;idx>=0;idx--)
   {
      CMCHKPK(oduUnpackUInt8,param->cpCiphKey[idx],mBuf);
   }
   for(idx=15;idx>=0;idx--)
   {
      CMCHKPK(oduUnpackUInt8,param->upCiphKey[idx],mBuf);
   }
   for(idx=15;idx>=0;idx--)
   {
      CMCHKPK(oduUnpackUInt8,param->intKey[idx],mBuf);
   }
   return ROK;
}

/***********************************************************
 *
 *    Func: cmPkCtfKenbCfgInfo
 *
 *
 *    Desc:   Key derivation funtion Input Information
 * @details This funtion packs the the CtfKenbCfgInfo structure
 *           which contains the eNodeB key and Algorithm Type for
 *           deriving AS security keys.
 *
 *
 *     Ret : S16
 *
 *     Notes:
 *
 *     File:
 *
 *  **********************************************************/
S16 cmPkCtfKdfCfmInfo
(
CtfKdfCfmInfo *param,
Buffer *mBuf
)
{

   switch (param->keyDerType)
   {
         case CTF_KDF_TYPE1:
             CMCHKPK(cmPkCtfAskeyCfmInfo, &param->u.kEnbInf, mBuf);
             break;
         case CTF_KDF_TYPE2:
            CMCHKPK(cmPkCtfKenbStarCfmInfo, &param->u.kEnbStarInf, mBuf);
            break;
         case CTF_KDF_TYPE3:
            CMCHKPK(cmPkCtfNhKeyCfmInfo, &param->u.nhKeyInf, mBuf);
             break;
         default :
            return RFAILED;
    }
    CMCHKPK(oduUnpackUInt8, param->keyDerType, mBuf);
   return ROK;
}

/**
* @brief Configuration Confirm from PHY to RRC.
*
* @details
*
*     Function: cmPkCtfCfgCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   CtfCfgTransId  transId
*  @param[in]   uint8_t  status
*  @return   S16
*      -# ROK
**/
S16 cmPkCtfKdfCfm
(
Pst* pst,
SuId suId,
CtfCfgTransId transId,
CtfKdfCfmInfo *kdfCfmInfo,
uint8_t status
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF026, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)kdfCfmInfo, sizeof(CtfKdfCfmInfo));
      return RFAILED;
   }

   if (oduUnpackUInt8(status, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF027, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)kdfCfmInfo, sizeof(CtfKdfCfmInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }

   if (cmPkCtfKdfCfmInfo(kdfCfmInfo, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF028, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)kdfCfmInfo, sizeof(CtfKdfCfmInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }

   if (cmPkCtfCfgTransId(&transId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF028, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)kdfCfmInfo, sizeof(CtfKdfCfmInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }

   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF029, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)kdfCfmInfo, sizeof(CtfKdfCfmInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }

   if (SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)kdfCfmInfo, sizeof(CtfKdfCfmInfo)) != ROK) 
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
   /* ctf_c_001.main_3 Compilation warning fix with g++ */
   SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
      (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
      (ErrVal)ECTF021, (ErrVal)0, (Txt*)&"Packing failed");
#endif
     SPutMsg(mBuf);
     return RFAILED;
   }

   pst->event = (Event) EVTCTFKDFCFM;
   return (SPstTsk(pst,mBuf));
}
#endif

/**
* @brief Configuration Confirm from PHY to RRC.
*
* @details
*
*     Function: cmPkCtfCfgCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   CtfCfgTransId  transId
*  @param[in]   uint8_t  status
*  @return   S16
*      -# ROK
**/
S16 cmPkCtfCfgCfm
(
Pst* pst,
SuId suId,
CtfCfgTransId transId,
uint8_t status
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF026, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (oduUnpackUInt8(status, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF027, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (cmPkCtfCfgTransId(&transId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF028, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF029, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   pst->event = (Event) EVTCTFCFGCFM;
   return (SPstTsk(pst,mBuf));
}

/**
* @brief ENB STOP Indication from PHY to ENB-APP.
*
* @details
*
*     Function: cmPkCtfEnbStopInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   CtfCfgTransId  transId
*  @return   S16
*      -# ROK
**/
S16 cmPkCtfEnbStopInd
(
Pst* pst,
SuId suId,
CtfCfgTransId transId
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF026, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (cmPkCtfCfgTransId(&transId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF028, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF029, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   pst->event = (Event) EVTCTFENBSTOPIND;
   return (SPstTsk(pst,mBuf));
}

#ifdef TENB_AS_SECURITY
/***********************************************************
*
*     Func: cmPkCtfKenbCfgInfo
*
*
*     Desc:   Key derivation funtion Input Information
 * @details This funtion packs the the CtfKenbCfgInfo structure
 *           which contains the eNodeB key and Algorithm Type for
 *           deriving AS security keys.
 *
 *
 *     Ret : S16
 *
 *     Notes:
 *
 *     File:
 *
 **********************************************************/
S16 cmUnpkCtfNhKeyCfmInfo
(
CtfNhKeyCfmInfo *param,
Buffer *mBuf
)
{

   uint8_t  idx;

   for(idx=0;idx<32;idx++)
   {
      CMCHKUNPK(oduPackUInt8,&param->secKey[idx],mBuf);
   }
   return ROK;
}

/***********************************************************
*
*     Func: cmPkCtfKenbCfgInfo
*
*
*     Desc:   Key derivation funtion Input Information
 * @details This funtion packs the the CtfKenbCfgInfo structure
 *           which contains the eNodeB key and Algorithm Type for
 *           deriving AS security keys.
 *
 *
 *     Ret : S16
 *
 *     Notes:
 *
 *     File:
 *
 **********************************************************/
S16 cmUnpkCtfKenbStarCfmInfo
(
CtfKenbStarCfmInfo *param,
Buffer *mBuf
)
{

   uint8_t  idx;
         
   for(idx=0;idx<32;idx++)
   {
      CMCHKUNPK(oduPackUInt8,&param->secKey[idx],mBuf);
   }
   return ROK;
}


/***********************************************************
*
*     Func: cmPkCtfKenbCfgInfo
*
*
*     Desc:   Key derivation funtion Input Information
 * @details This funtion packs the the CtfKenbCfgInfo structure
 *           which contains the eNodeB key and Algorithm Type for
 *           deriving AS security keys.
 *
 *
 *     Ret : S16
 *
 *     Notes:
 *
 *     File:
 *
 **********************************************************/
S16 cmUnpkCtfAskeyCfmInfo
(
CtfAskeyCfmInfo *param,
Buffer *mBuf
)
{

   uint8_t  idx;

         
   for(idx=0;idx<16;idx++)
   {
      CMCHKUNPK(oduPackUInt8,&param->intKey[idx],mBuf);
   }
   for(idx=0;idx<16;idx++)
   {
      CMCHKUNPK(oduPackUInt8,&param->upCiphKey[idx],mBuf);
   }
   for(idx=0;idx<16;idx++)
   {
      CMCHKUNPK(oduPackUInt8,&param->cpCiphKey[idx],mBuf);
   }
   return ROK;
}

/***********************************************************
 *
 *    Func: cmPkCtfKenbCfgInfo
 *
 *
 *    Desc:   Key derivation funtion Input Information
 * @details This funtion packs the the CtfKenbCfgInfo structure
 *           which contains the eNodeB key and Algorithm Type for
 *           deriving AS security keys.
 *
 *
 *     Ret : S16
 *
 *     Notes:
 *
 *     File:
 *
 *  **********************************************************/
S16 cmUnpkCtfKdfCfmInfo
(
CtfKdfCfmInfo *param,
Buffer *mBuf
)
{


    CMCHKUNPK(oduPackUInt8, &param->keyDerType, mBuf);
   switch (param->keyDerType)
    {
         case CTF_KDF_TYPE1:
             CMCHKUNPK(cmUnpkCtfAskeyCfmInfo, &param->u.kEnbInf, mBuf);
             break;
         case CTF_KDF_TYPE2:
            CMCHKUNPK(cmUnpkCtfKenbStarCfmInfo, &param->u.kEnbStarInf, mBuf);
            break;
         case CTF_KDF_TYPE3:
            CMCHKUNPK(cmUnpkCtfNhKeyCfmInfo, &param->u.nhKeyInf, mBuf);
             break;
         default :
            return RFAILED;
    }
   return ROK;
}

/**
* @brief Configuration Confirm from PHY to RRC.
*
* @details
*
*     Function: cmUnpkCtfCfgCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   CtfCfgTransId  transId
*  @param[in]   uint8_t  status
*  @return   S16
*      -# ROK
**/
S16 cmUnpkCtfKdfCfm
(
CtfKdfCfm func,
Pst *pst,
Buffer *mBuf
)
{
   SuId suId = 0;
   CtfCfgTransId transId;
   uint8_t status;
   CtfKdfCfmInfo kdfCfmInfo;
   

   if (SUnpkS16(&suId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF030, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (cmUnpkCtfCfgTransId(&transId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF031, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (cmUnpkCtfKdfCfmInfo(&kdfCfmInfo, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF031, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }

   if (oduPackUInt8(&status, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF032, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, suId, transId,&kdfCfmInfo, status));
}
#endif


/**
* @brief Configuration Confirm from PHY to RRC.
*
* @details
*
*     Function: cmUnpkCtfCfgCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   CtfCfgTransId  transId
*  @param[in]   uint8_t  status
*  @return   S16
*      -# ROK
**/
S16 cmUnpkCtfCfgCfm
(
CtfCfgCfm func,
Pst *pst,
Buffer *mBuf
)
{
   SuId suId = 0;
   CtfCfgTransId transId;
   uint8_t status;
   

   if (SUnpkS16(&suId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF030, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (cmUnpkCtfCfgTransId(&transId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF031, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (oduPackUInt8(&status, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF032, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, suId, transId, status));
}

/**
* @brief ENB STOP Indication from PHY to ENB-APP
*
* @details
*
*     Function: cmUnpkCtfEnbStopInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   CtfCfgTransId  transId
*  @return   S16
*      -# ROK
**/
S16 cmUnpkCtfEnbStopInd
(
CtfEnbStopInd func,
Pst *pst,
Buffer *mBuf
)
{
   SuId suId = 0;
   CtfCfgTransId transId;

   if (SUnpkS16(&suId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF030, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (cmUnpkCtfCfgTransId(&transId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF031, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, suId, transId));
}


/**
* @brief UE ID Change Request from RRC to PHY.
*
* @details
*
*     Function: cmPkCtfUeIdChgReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   CtfCfgTransId  transId
*  @param[in]   CtfUeInfo *  ueInfo
*  @param[in]   CtfUeInfo *  newUeInfo
*  @return   S16
*      -# ROK
**/
S16 cmPkCtfUeIdChgReq
(
Pst* pst,
SpId spId,
CtfCfgTransId transId,
CtfUeInfo * ueInfo,
CtfUeInfo * newUeInfo
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF033, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ueInfo, sizeof(CtfUeInfo));
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)newUeInfo, sizeof(CtfUeInfo));
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LC) {
      if (cmPkCtfUeInfo(newUeInfo, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF034, (ErrVal)0, (Txt*)&"Packing failed");
#endif
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ueInfo, sizeof(CtfUeInfo));
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)newUeInfo, sizeof(CtfUeInfo));
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   else if (pst->selector == ODU_SELECTOR_LWLC) {
      if (oduPackPointer((PTR)newUeInfo, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF035, (ErrVal)0, (Txt*)&"Packing failed");
#endif
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ueInfo, sizeof(CtfUeInfo));
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)newUeInfo, sizeof(CtfUeInfo));
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   if (pst->selector == ODU_SELECTOR_LC) {
      if (cmPkCtfUeInfo(ueInfo, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF036, (ErrVal)0, (Txt*)&"Packing failed");
#endif
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ueInfo, sizeof(CtfUeInfo));
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)newUeInfo, sizeof(CtfUeInfo));
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   else if (pst->selector == ODU_SELECTOR_LWLC) {
      if (oduPackPointer((PTR)ueInfo, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF037, (ErrVal)0, (Txt*)&"Packing failed");
#endif
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ueInfo, sizeof(CtfUeInfo));
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)newUeInfo, sizeof(CtfUeInfo));
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   if (cmPkCtfCfgTransId(&transId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF038, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ueInfo, sizeof(CtfUeInfo));
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)newUeInfo, sizeof(CtfUeInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF039, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ueInfo, sizeof(CtfUeInfo));
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)newUeInfo, sizeof(CtfUeInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (pst->selector != ODU_SELECTOR_LWLC) {
      if (SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ueInfo, sizeof(CtfUeInfo)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF040, (ErrVal)0, (Txt*)&"Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
      }
      if (SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)newUeInfo, sizeof(CtfUeInfo)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF041, (ErrVal)0, (Txt*)&"Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   pst->event = (Event) EVTCTFUEIDCHGREQ;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief UE ID Change Request from RRC to PHY.
*
* @details
*
*     Function: cmUnpkCtfUeIdChgReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   CtfCfgTransId  transId
*  @param[in]   CtfUeInfo *  ueInfo
*  @param[in]   CtfUeInfo *  newUeInfo
*  @return   S16
*      -# ROK
**/
S16 cmUnpkCtfUeIdChgReq
(
CtfUeIdChgReq func,
Pst *pst,
Buffer *mBuf
)
{
   SpId spId = 0;
   CtfCfgTransId transId;
   CtfUeInfo *ueInfo = NULLP;
   CtfUeInfo *newUeInfo = NULLP;
   

   if (SUnpkS16(&spId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
        (Txt*)& __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF042, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (cmUnpkCtfCfgTransId(&transId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF043, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (pst->selector != ODU_SELECTOR_LWLC) {
      if ((SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&ueInfo, sizeof(CtfUeInfo))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF044, (ErrVal)0, (Txt*)&"Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   if (pst->selector == ODU_SELECTOR_LWLC) {
      if (oduUnpackPointer((PTR *)&ueInfo, mBuf) != ROK) {
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   else if (pst->selector == ODU_SELECTOR_LC) 
      if (cmUnpkCtfUeInfo(ueInfo, mBuf) != ROK) {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ueInfo, sizeof(CtfUeInfo));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF045, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (pst->selector != ODU_SELECTOR_LWLC) {
      if ((SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&newUeInfo, sizeof(CtfUeInfo))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF046, (ErrVal)0, (Txt*)&"Packing failed");
#endif
         SPutMsg(mBuf);
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ueInfo, sizeof(CtfUeInfo));
         return RFAILED;
      }
   }
   if (pst->selector == ODU_SELECTOR_LWLC) {
      if (oduUnpackPointer((PTR *)&newUeInfo, mBuf) != ROK) {
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   else if (pst->selector == ODU_SELECTOR_LC) 
      if (cmUnpkCtfUeInfo(newUeInfo, mBuf) != ROK) {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ueInfo, sizeof(CtfUeInfo));
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)newUeInfo, sizeof(CtfUeInfo));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF047, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, spId, transId, ueInfo, newUeInfo));
}


/**
* @brief UE ID Change Confirm from PHY to RRC.
*
* @details
*
*     Function: cmPkCtfUeIdChgCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   CtfCfgTransId  transId
*  @param[in]   CtfUeInfo *  ueInfo
*  @param[in]   uint8_t  status
*  @return   S16
*      -# ROK
**/
S16 cmPkCtfUeIdChgCfm
(
Pst* pst,
SuId suId,
CtfCfgTransId transId,
CtfUeInfo * ueInfo,
CmStatus status
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF048, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ueInfo, sizeof(CtfUeInfo));
      return RFAILED;
   }
   if (cmPkCmStatus(&status, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF049, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ueInfo, sizeof(CtfUeInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LC) {
      if (cmPkCtfUeInfo(ueInfo, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF050, (ErrVal)0, (Txt*)&"Packing failed");
#endif
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ueInfo, sizeof(CtfUeInfo));
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   else if (pst->selector == ODU_SELECTOR_LWLC) {
      if (oduPackPointer((PTR)ueInfo, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF051, (ErrVal)0, (Txt*)&"Packing failed");
#endif
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ueInfo, sizeof(CtfUeInfo));
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   if (cmPkCtfCfgTransId(&transId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF052, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ueInfo, sizeof(CtfUeInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF053, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ueInfo, sizeof(CtfUeInfo));
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (pst->selector != ODU_SELECTOR_LWLC) {
      if (SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ueInfo, sizeof(CtfUeInfo)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF054, (ErrVal)0, (Txt*)&"Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   pst->event = (Event) EVTCTFUEIDCHGCFM;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief UE ID Change Confirm from PHY to RRC.
*
* @details
*
*     Function: cmUnpkCtfUeIdChgCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   CtfCfgTransId  transId
*  @param[in]   CtfUeInfo *  ueInfo
*  @param[in]   uint8_t  status
*  @return   S16
*      -# ROK
**/
S16 cmUnpkCtfUeIdChgCfm
(
CtfUeIdChgCfm func,
Pst *pst,
Buffer *mBuf
)
{
   SuId suId = 0;
   CtfCfgTransId transId;
   CtfUeInfo *ueInfo = NULLP;
   CmStatus status;
   

   if (SUnpkS16(&suId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF055, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (cmUnpkCtfCfgTransId(&transId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF056, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (pst->selector != ODU_SELECTOR_LWLC) {
      if ((SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&ueInfo, sizeof(CtfUeInfo))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF057, (ErrVal)0, (Txt*)&"Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   if (pst->selector == ODU_SELECTOR_LWLC) {
      if (oduUnpackPointer((PTR *)&ueInfo, mBuf) != ROK) {
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   else if (pst->selector == ODU_SELECTOR_LC) 
      if (cmUnpkCtfUeInfo(ueInfo, mBuf) != ROK) {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ueInfo, sizeof(CtfUeInfo));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF058, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (cmUnpkCmStatus(&status, mBuf) != ROK) {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)ueInfo, sizeof(CtfUeInfo));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF059, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, suId, transId, ueInfo, status));
}


/***********************************************************
*
*     Func: cmPkCtfCfgTransId
*
*
*     Desc: Transaction ID between CTF and RRC.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfCfgTransId
(
CtfCfgTransId *param,
Buffer *mBuf
)
{

   S32 i;
   for (i=CTF_CFG_TRANSID_SIZE-1; i >= 0; i--) {
      CMCHKPK(oduUnpackUInt8, param->trans[i], mBuf);
   }
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfCfgTransId
*
*
*     Desc: Transaction ID between CTF and RRC.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfCfgTransId
(
CtfCfgTransId *param,
Buffer *mBuf
)
{

   S32 i;

   for (i=0; i<CTF_CFG_TRANSID_SIZE; i++) {
      CMCHKUNPK(oduPackUInt8, &param->trans[i], mBuf);
   }
   return ROK;
}



/***********************************************************
*
*     Func: cmPkCtfUeInfo
*
*
*     Desc: *  UE ID information
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfUeInfo
(
CtfUeInfo *param,
Buffer *mBuf
)
{

   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfUeInfo
*
*
*     Desc: *  UE ID information
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfUeInfo
(
CtfUeInfo *param,
Buffer *mBuf
)
{

   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmPkCtfBwCfgInfo
*
*
*     Desc:   Bandwidth Configuration
 * @details This structure contains the uplink and downlink bandwidth 
 *          information for configuration at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfBwCfgInfo
(
CtfBwCfgInfo *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt8, param->eUtraBand, mBuf);
   CMCHKPK(oduUnpackUInt32, param->ulBw, mBuf);
   CMCHKPK(oduUnpackUInt32, param->dlBw, mBuf);
   /* ctf_c_001.main_4: Removed packing of param->pres */
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfBwCfgInfo
*
*
*     Desc:   Bandwidth Configuration
 * @details This structure contains the uplink and downlink bandwidth 
 *          information for configuration at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfBwCfgInfo
(
CtfBwCfgInfo *param,
Buffer *mBuf
)
{

   uint32_t tmpEnum;

   /* ctf_c_001.main_4: Removed UnPacking of param->pres */
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->dlBw = (CtfBandwidth)tmpEnum;
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->ulBw = (CtfBandwidth)tmpEnum;
   CMCHKUNPK(oduPackUInt8, &param->eUtraBand, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmPkCtfTxSchemeCfg
*
*
*     Desc:   Transmission Scheme Configuration
 * @details This structure contains the transmission scheme related information.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfTxSchemeCfg
(
CtfTxSchemeCfg *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt32, param->cycPfx, mBuf);
   CMCHKPK(oduUnpackUInt32, param->scSpacing, mBuf);
   CMCHKPK(oduUnpackUInt32, param->duplexMode, mBuf);
   /* ctf_c_001.main_4: Removed packing of param->pres */
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfTxSchemeCfg
*
*
*     Desc:   Transmission Scheme Configuration
 * @details This structure contains the transmission scheme related information.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfTxSchemeCfg
(
CtfTxSchemeCfg *param,
Buffer *mBuf
)
{
   uint32_t tmpEnum;


   /* ctf_c_001.main_4: Removed UnPacking of param->pres */
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->duplexMode = (CtfDuplexMode)tmpEnum;
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->scSpacing = (CtfSCSpacing)tmpEnum;
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->cycPfx = (CtfCPCfg)tmpEnum;
   return ROK;
}



/***********************************************************
*
*     Func: cmPkCtfAntennaCfgInfo
*
*
*     Desc:   Antenna Ports configuration
 * @details This structure contains the antenna configuration information for 
 *          configuration at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfAntennaCfgInfo
(
CtfAntennaCfgInfo *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt32, param->antPortsCnt, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfAntennaCfgInfo
*
*
*     Desc:   Antenna Ports configuration
 * @details This structure contains the antenna configuration information for 
 *          configuration at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfAntennaCfgInfo
(
CtfAntennaCfgInfo *param,
Buffer *mBuf
)
{
   uint32_t tmpEnum;

   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->antPortsCnt = (CtfAntennaPortsCount)tmpEnum;
   return ROK;
}



/***********************************************************
*
*     Func: cmPkCtfPrachCfgInfo
*
*
*     Desc:   PRACH configuration
 * @details This structure contains the configuration information for PRACH at PHY.
 *          -# PRACH preamble sequences are generated by PHY using Zadoff-Chu 
 *             sequences.[Ref: 36.211, 5.7.2]
 *          -# PRACH preamble format is derived from PRACH Configuration Index.
 *             [Ref: 36.211, Table 5.7.1-2]
 *          -# PrachFreqOffset is the first physical resource block allocated to 
 *             the PRACH opportunity considered for preamble format 0, 1, 2, and 3.
 *             [Ref: 36.211, 5.7.1]
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfPrachCfgInfo
(
CtfPrachCfgInfo *param,
Buffer *mBuf
)
{

   /*ctf_c_001.main_1*/
   if(param->pres != FALSE)
   {
      CMCHKPK(oduUnpackUInt8, param->prachFreqOffset, mBuf);
      CMCHKPK(oduUnpackUInt8, param->highSpeedFlag, mBuf);
      CMCHKPK(oduUnpackUInt8, param->zeroCorrelationZoneCfg, mBuf);
      CMCHKPK(oduUnpackUInt8, param->prachCfgIndex, mBuf);
      CMCHKPK(oduUnpackUInt16, param->rootSequenceIndex, mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->pres, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfPrachCfgInfo
*
*
*     Desc:   PRACH configuration
 * @details This structure contains the configuration information for PRACH at PHY.
 *          -# PRACH preamble sequences are generated by PHY using Zadoff-Chu 
 *             sequences.[Ref: 36.211, 5.7.2]
 *          -# PRACH preamble format is derived from PRACH Configuration Index.
 *             [Ref: 36.211, Table 5.7.1-2]
 *          -# PrachFreqOffset is the first physical resource block allocated to 
 *             the PRACH opportunity considered for preamble format 0, 1, 2, and 3.
 *             [Ref: 36.211, 5.7.1]
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfPrachCfgInfo
(
CtfPrachCfgInfo *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt8, &param->pres, mBuf);
/*ctf_c_001.main_1*/
   if(param->pres != FALSE)
   {
      CMCHKUNPK(oduPackUInt16, &param->rootSequenceIndex, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->prachCfgIndex, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->zeroCorrelationZoneCfg, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->highSpeedFlag, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->prachFreqOffset, mBuf);
   }
   return ROK;
}



/***********************************************************
*
*     Func: cmPkCtfPdschCfgInfo
*
*
*     Desc:   PDSCH configuration
 * @details This structure contains the PDSCH configuration information for 
 *          configuration at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfPdschCfgInfo
(
CtfPdschCfgInfo *param,
Buffer *mBuf
)
{

/*ctf_c_001.main_1*/
   if(param->pres != FALSE)
   {
      CMCHKPK(oduUnpackUInt8, param->p_b, mBuf);
      CMCHKPK(SPkS16, param->refSigPwr, mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->pres, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfPdschCfgInfo
*
*
*     Desc:   PDSCH configuration
 * @details This structure contains the PDSCH configuration information for 
 *          configuration at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfPdschCfgInfo
(
CtfPdschCfgInfo *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt8, &param->pres, mBuf);
/*ctf_c_001.main_1*/
   if(param->pres != FALSE)
   {
     CMCHKUNPK(SUnpkS16, &param->refSigPwr, mBuf);
     CMCHKUNPK(oduPackUInt8, &param->p_b, mBuf);
   }
   return ROK;
}



/***********************************************************
*
*     Func: cmPkCtfPuschCfgBasic
*
*
*     Desc:   Basic PUSCH configuration
 * @details This structure contains the basic PUSCH configuration information for 
 *          configuration at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfPuschCfgBasic
(
CtfPuschCfgBasic *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt8, param->enable64QAM, mBuf);
   CMCHKPK(oduUnpackUInt8, param->hoppingOffset, mBuf);
   CMCHKPK(oduUnpackUInt32, param->hoppingMode, mBuf);
   CMCHKPK(oduUnpackUInt8, param->noOfsubBands, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfPuschCfgBasic
*
*
*     Desc:   Basic PUSCH configuration
 * @details This structure contains the basic PUSCH configuration information for 
 *          configuration at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfPuschCfgBasic
(
CtfPuschCfgBasic *param,
Buffer *mBuf
)
{
   uint32_t tmpEnum;

   CMCHKUNPK(oduPackUInt8, &param->noOfsubBands, mBuf);
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->hoppingMode = (CtfPuschHoppingMode)tmpEnum;
   CMCHKUNPK(oduPackUInt8, &param->hoppingOffset, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->enable64QAM, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmPkCtfPuschUlRS
*
*
*     Desc:   PUSCH Uplink Reference Signals configuration
 * @details This structure contains the PUSCH configuration information for 
 *          uplink reference signals configuration at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfPuschUlRS
(
CtfPuschUlRS *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt8, param->cycShift, mBuf);
   CMCHKPK(oduUnpackUInt8, param->grpNum, mBuf);
   CMCHKPK(oduUnpackUInt8, param->seqHopEnabled, mBuf);
   CMCHKPK(oduUnpackUInt8, param->grpHopEnabled, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfPuschUlRS
*
*
*     Desc:   PUSCH Uplink Reference Signals configuration
 * @details This structure contains the PUSCH configuration information for 
 *          uplink reference signals configuration at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfPuschUlRS
(
CtfPuschUlRS *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->grpHopEnabled, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->seqHopEnabled, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->grpNum, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->cycShift, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmPkCtfPuschCfgInfo
*
*
*     Desc:   PUSCH configuration
 * @details This structure contains the information for PUSCH 
 *          configuration at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfPuschCfgInfo
(
CtfPuschCfgInfo *param,
Buffer *mBuf
)
{

/*ctf_c_001.main_1*/
   if(param->pres != FALSE)
   {
     CMCHKPK(cmPkCtfPuschUlRS, &param->puschUlRS, mBuf);
     CMCHKPK(cmPkCtfPuschCfgBasic, &param->puschBasicCfg, mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->pres, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfPuschCfgInfo
*
*
*     Desc:   PUSCH configuration
 * @details This structure contains the information for PUSCH 
 *          configuration at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfPuschCfgInfo
(
CtfPuschCfgInfo *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt8, &param->pres, mBuf);
/*ctf_c_001.main_1*/
   if(param->pres != FALSE)
   {
     CMCHKUNPK(cmUnpkCtfPuschCfgBasic, &param->puschBasicCfg, mBuf);
     CMCHKUNPK(cmUnpkCtfPuschUlRS, &param->puschUlRS, mBuf);
   }
   return ROK;
}



/***********************************************************
*
*     Func: cmPkCtfPhichCfgInfo
*
*
*     Desc:   PHICH configuration
 * @details This structure contains the duration and resource information for
 *          PHICH configuration at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfPhichCfgInfo
(
CtfPhichCfgInfo *param,
Buffer *mBuf
)
{

/*ctf_c_001.main_1*/
   if(param->pres != FALSE)
   {
      CMCHKPK(oduUnpackUInt32, param->resource, mBuf);
      CMCHKPK(oduUnpackUInt32, param->duration, mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->pres, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfPhichCfgInfo
*
*
*     Desc:   PHICH configuration
 * @details This structure contains the duration and resource information for
 *          PHICH configuration at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfPhichCfgInfo
(
CtfPhichCfgInfo *param,
Buffer *mBuf
)
{
   uint32_t tmpEnum;

   CMCHKUNPK(oduPackUInt8, &param->pres, mBuf);
/*ctf_c_001.main_1*/
   if(param->pres != FALSE)
   {
     CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
     param->duration = (CtfPhichDuration)tmpEnum;
     CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
     param->resource = (CtfPhichResource)tmpEnum;
   }
   return ROK;
}



/***********************************************************
*
*     Func: cmPkCtfPucchCfgInfo
*
*
*     Desc:   PUCCH configuration
 * @details This structure contains the information for PUCCH
 *          configuration at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfPucchCfgInfo
(
CtfPucchCfgInfo *param,
Buffer *mBuf
)
{


/*ctf_c_001.main_1*/
   if(param->pres != FALSE)
   {
      CMCHKPK(oduUnpackUInt32, param->deltaShift, mBuf);
      CMCHKPK(oduUnpackUInt16, param->n1PUCCH, mBuf);
      CMCHKPK(oduUnpackUInt8, param->nCS, mBuf);
      CMCHKPK(oduUnpackUInt8, param->nRB, mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->pres, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfPucchCfgInfo
*
*
*     Desc:   PUCCH configuration
 * @details This structure contains the information for PUCCH
 *          configuration at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfPucchCfgInfo
(
CtfPucchCfgInfo *param,
Buffer *mBuf
)
{
   uint32_t tmpEnum;

   CMCHKUNPK(oduPackUInt8, &param->pres, mBuf);
/*ctf_c_001.main_1*/
   if(param->pres != FALSE)
   {
      CMCHKUNPK(oduPackUInt8, &param->nRB, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->nCS, mBuf);
      CMCHKUNPK(oduPackUInt16, &param->n1PUCCH, mBuf);
      CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
      param->deltaShift = (CtfPucchDeltaShift)tmpEnum;
   }
   return ROK;
}



/***********************************************************
*
*     Func: cmPkCtfSrsUlCfgInfo
*
*
*     Desc:   SRS uplink configuration
 * @details This structure contains the information for setting-up/release
 *          of uplink SRS configuration at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfSrsUlCfgInfo
(
CtfSrsUlCfgInfo *param,
Buffer *mBuf
)
{

/*ctf_c_001.main_1*/
   if(param->pres != FALSE)
   {
      CMCHKPK(oduUnpackUInt8, param->srsSetup.srsMaxUpPts, mBuf);
      CMCHKPK(oduUnpackUInt8, param->srsSetup.srsANSimultTx, mBuf);
      CMCHKPK(oduUnpackUInt8, param->srsSetup.sfCfg, mBuf);
      CMCHKPK(oduUnpackUInt8, param->srsSetup.srsBw, mBuf);
      CMCHKPK(oduUnpackUInt8, param->srsCfgType, mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->pres, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfSrsUlCfgInfo
*
*
*     Desc:   SRS uplink configuration
 * @details This structure contains the information for setting-up/release
 *          of uplink SRS configuration at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfSrsUlCfgInfo
(
CtfSrsUlCfgInfo *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt8, &param->pres, mBuf);
/*ctf_c_001.main_1*/
   if(param->pres != FALSE)
   {
      CMCHKUNPK(oduPackUInt8, &param->srsCfgType, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->srsSetup.srsBw, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->srsSetup.sfCfg, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->srsSetup.srsANSimultTx, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->srsSetup.srsMaxUpPts, mBuf);
   }
   return ROK;
}



/***********************************************************
*
*     Func: cmPkCtfTddSfCfgInfo
*
*
*     Desc:   Subframe Configuration for TDD mode.
 * @details This structure contains the information for setting-up
 *          the subframe for TDD mode at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfTddSfCfgInfo
(
CtfTddSfCfgInfo *param,
Buffer *mBuf
)
{

/*ctf_c_001.main_1*/
   if(param->pres != FALSE)
   {
      CMCHKPK(oduUnpackUInt8, param->spclSfPatterns, mBuf);
      CMCHKPK(oduUnpackUInt8, param->sfAssignment, mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->pres, mBuf);
   return ROK;
}

/* ctf_c_001.main_4: Added support for vendor specific parameters */
#ifdef CTF_VER3
/***********************************************************
*
*     Func: cmPkCtfVendorSpecParams
*
*
*     Desc: Vendor Specific Parameter configuration 
*   @details This structure contains the Parameters which is spaciftc to
*            Vendor.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfVendorSpecParams
(
CtfVendorSpecific *param,
Buffer *mBuf
)
{

   if( param->buffLen != 0) {
     SAddPreMsgMultInOrder(param->paramBuffer,param->buffLen,mBuf);   
   }
   CMCHKPK(oduUnpackUInt16,param->buffLen,mBuf);

   return ROK;
}

/***********************************************************
*
*     Func: cmUnpkCtfVendorSpecParams
*
*
*     Desc: Vendor Specific Parameter configuration  
* @details  This structure contains the Parameters which is spaciftc to
*           Vendor.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfVendorSpecParams
(
Pst *pst,
CtfVendorSpecific *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt16, &param->buffLen, mBuf);

   if(param->buffLen != 0) {
      if ((SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&param->paramBuffer, param->buffLen)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ECTF060, (ErrVal)0, (Txt*)&"Unpacking failed");
#endif
         return RFAILED ;
      }
      SRemPreMsgMult(param->paramBuffer, param->buffLen,mBuf) ;   
   }
   else {
      param->paramBuffer = NULLP ;
   }

   return ROK;
}
#endif

/***********************************************************
*
*     Func: cmUnpkCtfTddSfCfgInfo
*
*
*     Desc:   Subframe Configuration for TDD mode.
 * @details This structure contains the information for setting-up
 *          the subframe for TDD mode at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfTddSfCfgInfo
(
CtfTddSfCfgInfo *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt8, &param->pres, mBuf);
/*ctf_c_001.main_1*/
   if(param->pres != FALSE)
   {
      CMCHKUNPK(oduPackUInt8, &param->sfAssignment, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->spclSfPatterns, mBuf);
   }
   return ROK;
}
/***********************************************************
*
*     Func: cmPkCtfUnLicnsdCellParam
*
*
*     Desc:   Packing unlicensed SDL cell specific parameter
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
static S16 cmPkCtfUnLicnsdCellParam
(
CtfUnLicnsdCfgInfo *param,
Buffer *mBuf
)
{
   uint8_t  idx;


   CMCHKPK(SPkS32, param->txPowerLimit, mBuf);
   CMCHKPK(oduUnpackUInt8, param->coExistMethod, mBuf);
   CMCHKPK(oduUnpackUInt8, param->lteOnPeriod, mBuf);
   CMCHKPK(oduUnpackUInt8, param->ccaMethod, mBuf);
   CMCHKPK(oduUnpackUInt8, param->adaptiveTx, mBuf);
   CMCHKPK(oduUnpackUInt16, param->transPeriod, mBuf);
   CMCHKPK(oduUnpackUInt16, param->listenPrd, mBuf);
   CMCHKPK(SPkS16, param->energyTh, mBuf);
   CMCHKPK(oduUnpackUInt16, param->scanTimePrd, mBuf);
   CMCHKPK(oduUnpackUInt16, param->activityTh, mBuf);

   for(idx = 0; idx < param->numFreq; idx++)
   {
      CMCHKPK(oduUnpackUInt32, param->earfcn[idx], mBuf);
   }   

   CMCHKPK(oduUnpackUInt8, param->numFreq, mBuf);

   return ROK;
} 

/***********************************************************
*
*     Func: cmPkCtfCellCfgInfo
*
*
*     Desc:   Cell Configuration Information
 * @details This structure contains the information for setting-up
 *          of a cell and its associated resources at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfCellCfgInfo
(
CtfCellCfgInfo *param,
Buffer *mBuf
)
{
#ifdef EMTC_ENABLE
   S32 i;
#endif

#ifdef EMTC_ENABLE
   for (i = CTF_MAX_CE_LEVEL-1; i >= 0; i--) {
      CMCHKPK(oduUnpackUInt8, param->ceLevelInfo[i].emtcNumRepPerPreambleAtt, mBuf);
      CMCHKPK(oduUnpackUInt8, param->ceLevelInfo[i].lastPreamble, mBuf);
      CMCHKPK(oduUnpackUInt8, param->ceLevelInfo[i].firstPreamble, mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->mPdcchStart, mBuf);
   CMCHKPK(oduUnpackUInt8, param->catMenabled, mBuf);
#endif
   if(param->unLicnsdCfgPres)
   {   
      DU_LOG("\nDEBUG  -->  pverma param->unLicnsdCfgPres cm/ctf -->%d\n",param->unLicnsdCfgPres);
      CMCHKPK(cmPkCtfUnLicnsdCellParam, &param->lteUCfg, mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->unLicnsdCfgPres, mBuf);
   /*Added for rgu dynamic delta*/
   CMCHKPK(oduUnpackUInt8, param->numDlUePerTti, mBuf);
   CMCHKPK(SPkS16, param->pilotSigPwr, mBuf);
   CMCHKPK(oduUnpackUInt16, param->ulEarfcn, mBuf);
   CMCHKPK(oduUnpackUInt16, param->dlEarfcn, mBuf);
   CMCHKPK(SPkS16, param->priSigPwr, mBuf);
   CMCHKPK(SPkS16, param->secSigPwr, mBuf);
   /* Packing PHY configuration parameters */
   CMCHKPK(oduUnpackUInt32, param->period, mBuf);
   CMCHKPK(oduUnpackUInt32, param->counter, mBuf);
   CMCHKPK(oduUnpackUInt16, param->opMode, mBuf);

   /* ctf_c_001.main_3: Added the pack for newly added fileds inside the CFG structure*/
   CMCHKPK(oduUnpackUInt8, param->rachSrRepModePres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->rachSrRepMode, mBuf);

   CMCHKPK(oduUnpackUInt8, param->dataRepModePres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->dataRepMode, mBuf);

   CMCHKPK(oduUnpackUInt8, param->phySyncModePres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->phySyncMode, mBuf);

   CMCHKPK(oduUnpackUInt8, param->rxAntennaPortsPres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->rxAntennaPorts, mBuf);

   CMCHKPK(oduUnpackUInt8, param->txAntennaPortsPres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->txAntennaPorts, mBuf);

   CMCHKPK(oduUnpackUInt8, param->pucchProbDtxAckPres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->pucchProbDtxAck, mBuf);

   CMCHKPK(oduUnpackUInt8, param->puschProbDtxAckPres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->puschProbDtxAck, mBuf);
  
   CMCHKPK(oduUnpackUInt8, param->srsDopEstFactorPres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->srsDopEstFactor, mBuf);
  
   CMCHKPK(oduUnpackUInt8, param->prachPkRatio0Pres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->prachPkRatio0, mBuf);
  
   CMCHKPK(oduUnpackUInt8, param->prachPkRatio4Pres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->prachPkRatio4, mBuf);
  
   CMCHKPK(oduUnpackUInt8, param->pucchNoiseGammaPres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->pucchNoiseGamma, mBuf);
  
   CMCHKPK(oduUnpackUInt8, param->extWinMarginPres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->extWinMargin, mBuf);
  
   CMCHKPK(oduUnpackUInt8, param->dciPowOsPres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->dciPowOs, mBuf);
  
   CMCHKPK(oduUnpackUInt8, param->cfiPowOsPres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->cfiPowOs, mBuf);
  
   CMCHKPK(oduUnpackUInt8, param->syncSigPowOsPres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->syncSigPowOs, mBuf);
 
   CMCHKPK(cmPkCtfTddSfCfgInfo, &param->tddSfCfg, mBuf);
   CMCHKPK(cmPkCtfSrsUlCfgInfo, &param->srsUlCfg, mBuf);
   CMCHKPK(cmPkCtfPucchCfgInfo, &param->pucchCfg, mBuf);
   CMCHKPK(cmPkCtfPhichCfgInfo, &param->phichCfg, mBuf);
   CMCHKPK(cmPkCtfPuschCfgInfo, &param->puschCfg, mBuf);
   CMCHKPK(cmPkCtfPdschCfgInfo, &param->pdschCfg, mBuf);
   CMCHKPK(cmPkCtfPrachCfgInfo, &param->prachCfg, mBuf);
   CMCHKPK(cmPkCtfAntennaCfgInfo, &param->antennaCfg, mBuf);
   CMCHKPK(cmPkCtfTxSchemeCfg, &param->txCfg, mBuf);
   CMCHKPK(cmPkCtfBwCfgInfo, &param->bwCfg, mBuf);
   CMCHKPK(oduUnpackUInt8, param->physCellId, mBuf);
   CMCHKPK(oduUnpackUInt8, param->physCellIdPres, mBuf);
   
   CMCHKPK(SPkS16, param->macSapId, mBuf);
   CMCHKPK(SPkS16, param->schSapId, mBuf);

   CMCHKPK(oduUnpackUInt8, param->cellIdGrpId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}
/***********************************************************
*
*     Func: cmUnpkCtfUnLicnsdCellParam
*
*
*     Desc:   Unpacking unLicensed SDL cell specific parameter
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
static S16 cmUnpkCtfUnLicnsdCellParam
(
CtfUnLicnsdCfgInfo *param,
Buffer *mBuf
)
{
   uint8_t  idx;

   CMCHKUNPK(oduPackUInt8, &param->numFreq, mBuf);

   for(idx = 0; idx < param->numFreq; idx++)
   {
      CMCHKUNPK(oduPackUInt32, &param->earfcn[idx], mBuf);
   }  

   CMCHKUNPK(oduPackUInt16, &param->activityTh, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->scanTimePrd, mBuf);
   CMCHKUNPK(SUnpkS16, &param->energyTh, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->listenPrd, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->transPeriod, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->adaptiveTx, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->ccaMethod, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->lteOnPeriod, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->coExistMethod, mBuf);
   CMCHKUNPK(SUnpkS32, &param->txPowerLimit, mBuf);

   return ROK;
}   


/***********************************************************
*
*     Func: cmUnpkCtfCellCfgInfo
*
*
*     Desc:   Cell Configuration Information
 * @details This structure contains the information for setting-up
 *          of a cell and its associated resources at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfCellCfgInfo
(
CtfCellCfgInfo *param,
Buffer *mBuf
)
{
#ifdef EMTC_ENABLE
   S32 i;
#endif

   /* ctf_c_001.main_3: Added the unpack for newly added fileds inside the CFG structure*/
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->cellIdGrpId, mBuf);

   CMCHKUNPK(SUnpkS16, &param->schSapId, mBuf);
   CMCHKUNPK(SUnpkS16, &param->macSapId, mBuf);

   CMCHKUNPK(oduPackUInt8, &param->physCellIdPres, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->physCellId, mBuf);
   CMCHKUNPK(cmUnpkCtfBwCfgInfo, &param->bwCfg, mBuf);
   CMCHKUNPK(cmUnpkCtfTxSchemeCfg, &param->txCfg, mBuf);
   CMCHKUNPK(cmUnpkCtfAntennaCfgInfo, &param->antennaCfg, mBuf);
   CMCHKUNPK(cmUnpkCtfPrachCfgInfo, &param->prachCfg, mBuf);
   CMCHKUNPK(cmUnpkCtfPdschCfgInfo, &param->pdschCfg, mBuf);
   CMCHKUNPK(cmUnpkCtfPuschCfgInfo, &param->puschCfg, mBuf);
   CMCHKUNPK(cmUnpkCtfPhichCfgInfo, &param->phichCfg, mBuf);
   CMCHKUNPK(cmUnpkCtfPucchCfgInfo, &param->pucchCfg, mBuf);
   CMCHKUNPK(cmUnpkCtfSrsUlCfgInfo, &param->srsUlCfg, mBuf);
   CMCHKUNPK(cmUnpkCtfTddSfCfgInfo, &param->tddSfCfg, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->syncSigPowOs, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->syncSigPowOsPres, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->cfiPowOs, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->cfiPowOsPres, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->dciPowOs, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->dciPowOsPres, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->extWinMargin, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->extWinMarginPres, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->pucchNoiseGamma, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->pucchNoiseGammaPres, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->prachPkRatio4, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->prachPkRatio4Pres, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->prachPkRatio0, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->prachPkRatio0Pres, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->srsDopEstFactor, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->srsDopEstFactorPres, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->puschProbDtxAck, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->puschProbDtxAckPres, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->pucchProbDtxAck, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->pucchProbDtxAckPres, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->txAntennaPorts, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->txAntennaPortsPres, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->rxAntennaPorts, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->rxAntennaPortsPres, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->phySyncMode, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->phySyncModePres, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->dataRepMode, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->dataRepModePres, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->rachSrRepMode, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->rachSrRepModePres, mBuf);

   /* UnPacking PHY configuration parameters */
   CMCHKUNPK(oduPackUInt16, &param->opMode, mBuf);
   CMCHKUNPK(oduPackUInt32, &param->counter, mBuf);
   CMCHKUNPK(oduPackUInt32, &param->period, mBuf);
   CMCHKUNPK(SUnpkS16, &param->priSigPwr, mBuf);
   CMCHKUNPK(SUnpkS16, &param->secSigPwr, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->dlEarfcn, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->ulEarfcn, mBuf);
   CMCHKUNPK(SUnpkS16, &param->pilotSigPwr, mBuf);
   /* Added for rgu dynamic delta change*/
   CMCHKUNPK(oduPackUInt8, &param->numDlUePerTti, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->unLicnsdCfgPres, mBuf);

   if(param->unLicnsdCfgPres)
   {
      DU_LOG("\nDEBUG  -->  pverma param->unLicnsdCfgPres 2 -->%d \n",param->unLicnsdCfgPres);
      CMCHKUNPK(cmUnpkCtfUnLicnsdCellParam, &param->lteUCfg, mBuf);
   }   
#ifdef EMTC_ENABLE
   CMCHKUNPK(oduPackUInt8, &param->catMenabled, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->mPdcchStart, mBuf);
   
   for (i = 0; i < CTF_MAX_CE_LEVEL; i++) {
      CMCHKUNPK(oduPackUInt8, &param->ceLevelInfo[i].firstPreamble, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->ceLevelInfo[i].lastPreamble, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->ceLevelInfo[i].emtcNumRepPerPreambleAtt, mBuf);
   }
#endif 
   return ROK;
}



/* ctf_c_001.main_4: Modified cmPkCtfCellRecfgInfo() to Add support for
 * vendor specific parameters */
/***********************************************************
*
*     Func: cmPkCtfCellRecfgInfo
*
*
*     Desc:   Cell Re-configuration Information
 * @details This structure contains the information for reconfiguration
 *          of a cell and its associated resources at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfCellRecfgInfo
(
CtfCellRecfgInfo *param,
Buffer *mBuf
)
{


#ifdef ENABLE_API_LOG
   CMCHKPK(oduUnpackUInt8, param->enableAPILog, mBuf);
#endif /* ENABLE_API_LOG */
   /* ctf_c_001.main_3: Added the pack for newly added fileds inside the CFG structure*/
   CMCHKPK(SPkS16, param->secSigPwr, mBuf);
   CMCHKPK(SPkS16, param->priSigPwr, mBuf);
   CMCHKPK(SPkS16, param->pilotSigPwr, mBuf);
   CMCHKPK(oduUnpackUInt8, param->rachSrRepModePres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->rachSrRepMode, mBuf);

   CMCHKPK(oduUnpackUInt8, param->dataRepModePres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->dataRepMode, mBuf);

   CMCHKPK(oduUnpackUInt8, param->phySyncModePres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->phySyncMode, mBuf);

   CMCHKPK(oduUnpackUInt8, param->rxAntennaPortsPres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->rxAntennaPorts, mBuf);

   CMCHKPK(oduUnpackUInt8, param->txAntennaPortsPres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->txAntennaPorts, mBuf);

   CMCHKPK(oduUnpackUInt8, param->pucchProbDtxAckPres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->pucchProbDtxAck, mBuf);


   CMCHKPK(oduUnpackUInt8, param->puschProbDtxAckPres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->puschProbDtxAck, mBuf);

   CMCHKPK(oduUnpackUInt8, param->srsDopEstFactorPres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->srsDopEstFactor, mBuf);

   CMCHKPK(oduUnpackUInt8, param->prachPkRatio0Pres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->prachPkRatio0, mBuf);

   CMCHKPK(oduUnpackUInt8, param->prachPkRatio4Pres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->prachPkRatio4, mBuf);

   CMCHKPK(oduUnpackUInt8, param->pucchNoiseGammaPres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->pucchNoiseGamma, mBuf);

   CMCHKPK(oduUnpackUInt8, param->extWinMarginPres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->extWinMargin, mBuf);

   CMCHKPK(oduUnpackUInt8, param->dciPowOsPres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->dciPowOs, mBuf);

   CMCHKPK(oduUnpackUInt8, param->cfiPowOsPres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->cfiPowOs, mBuf);

   CMCHKPK(oduUnpackUInt8, param->syncSigPowOsPres, mBuf);
   CMCHKPK(oduUnpackUInt16, param->syncSigPowOs, mBuf);

   CMCHKPK(cmPkCtfTddSfCfgInfo, &param->tddSfCfg, mBuf);
   CMCHKPK(cmPkCtfSrsUlCfgInfo, &param->srsUlCfg, mBuf);
   CMCHKPK(cmPkCtfPucchCfgInfo, &param->pucchCfg, mBuf);
   CMCHKPK(cmPkCtfPhichCfgInfo, &param->phichCfg, mBuf);
   CMCHKPK(cmPkCtfPuschCfgInfo, &param->puschCfg, mBuf);
   CMCHKPK(cmPkCtfPdschCfgInfo, &param->pdschCfg, mBuf);
   CMCHKPK(cmPkCtfPrachCfgInfo, &param->prachCfg, mBuf);
   CMCHKPK(cmPkLteTimingInfo, &param->actvnTime, mBuf);
   
   CMCHKPK(cmPkCtfAntennaCfgInfo, &param->antennaCfg, mBuf);
   CMCHKPK(cmPkCtfTxSchemeCfg, &param->txCfg, mBuf);
   CMCHKPK(cmPkCtfBwCfgInfo, &param->bwCfg, mBuf);
   CMCHKPK(oduUnpackUInt8, param->physCellId, mBuf);
   CMCHKPK(oduUnpackUInt8, param->physCellIdPres, mBuf);
   CMCHKPK(oduUnpackUInt8, param->cellIdGrpId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(oduUnpackUInt32, param->ctfReCfgType, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfCellRecfgInfo
*
*
*     Desc:   Cell Re-configuration Information
 * @details This structure contains the information for reconfiguration
 *          of a cell and its associated resources at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfCellRecfgInfo
(
CtfCellRecfgInfo *param,
Buffer *mBuf
)
{

   uint32_t tmpEnum;
   
   /* ctf_c_001.main_3: Added the unpack for newly added fileds inside the CFG structure*/
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
   param->ctfReCfgType = (CtfReCfgType)tmpEnum;
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->cellIdGrpId, mBuf);   
   CMCHKUNPK(oduPackUInt8, &param->physCellIdPres, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->physCellId, mBuf);
   CMCHKUNPK(cmUnpkCtfBwCfgInfo, &param->bwCfg, mBuf);
   CMCHKUNPK(cmUnpkCtfTxSchemeCfg, &param->txCfg, mBuf);
   CMCHKUNPK(cmUnpkCtfAntennaCfgInfo, &param->antennaCfg, mBuf);

   CMCHKUNPK(cmUnpkLteTimingInfo, &param->actvnTime, mBuf);
   CMCHKUNPK(cmUnpkCtfPrachCfgInfo, &param->prachCfg, mBuf);
   CMCHKUNPK(cmUnpkCtfPdschCfgInfo, &param->pdschCfg, mBuf);
   CMCHKUNPK(cmUnpkCtfPuschCfgInfo, &param->puschCfg, mBuf);
   CMCHKUNPK(cmUnpkCtfPhichCfgInfo, &param->phichCfg, mBuf);
   CMCHKUNPK(cmUnpkCtfPucchCfgInfo, &param->pucchCfg, mBuf);
   CMCHKUNPK(cmUnpkCtfSrsUlCfgInfo, &param->srsUlCfg, mBuf);
   CMCHKUNPK(cmUnpkCtfTddSfCfgInfo, &param->tddSfCfg, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->syncSigPowOs, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->syncSigPowOsPres, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->cfiPowOs, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->cfiPowOsPres, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->dciPowOs, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->dciPowOsPres, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->extWinMargin, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->extWinMarginPres, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->pucchNoiseGamma, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->pucchNoiseGammaPres, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->prachPkRatio4, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->prachPkRatio4Pres, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->prachPkRatio0, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->prachPkRatio0Pres, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->srsDopEstFactor, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->srsDopEstFactorPres, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->puschProbDtxAck, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->puschProbDtxAckPres, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->pucchProbDtxAck, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->pucchProbDtxAckPres, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->txAntennaPorts, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->txAntennaPortsPres, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->rxAntennaPorts, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->rxAntennaPortsPres, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->phySyncMode, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->phySyncModePres, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->dataRepMode, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->dataRepModePres, mBuf);

   CMCHKUNPK(oduPackUInt16, &param->rachSrRepMode, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->rachSrRepModePres, mBuf);

   CMCHKUNPK(SUnpkS16, &param->pilotSigPwr, mBuf);
   CMCHKUNPK(SUnpkS16, &param->priSigPwr, mBuf);
   CMCHKUNPK(SUnpkS16, &param->secSigPwr, mBuf);
   DU_LOG("\nDEBUG  -->  param->pilotSigPwr = %d\nparam->priSigPwr = %d\n param->secSigPwr = %d\n",param->pilotSigPwr,param->priSigPwr,param->secSigPwr); 
#ifdef ENABLE_API_LOG
   CMCHKUNPK(oduPackUInt8, &param->enableAPILog, mBuf);
#endif /* ENABLE_API_LOG */
   return ROK;
}



/***********************************************************
*
*     Func: cmPkCtfDedPdschCfgInfo
*
*
*     Desc:   Dedicated PDSCH Configuration
 * @details This structure contains the information for PDSCH configuration
 *          specific to an UE.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfDedPdschCfgInfo
(
CtfDedPdschCfgInfo *param,
Buffer *mBuf
)
{

/*ctf_c_001.main_1*/
   if(param->pres != FALSE)
      CMCHKPK(oduUnpackUInt32, param->pA, mBuf);
   CMCHKPK(oduUnpackUInt8, param->pres, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfDedPdschCfgInfo
*
*
*     Desc:   Dedicated PDSCH Configuration
 * @details This structure contains the information for PDSCH configuration
 *          specific to an UE.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfDedPdschCfgInfo
(
CtfDedPdschCfgInfo *param,
Buffer *mBuf
)
{
   uint32_t tmpEnum;

   CMCHKUNPK(oduPackUInt8, &param->pres, mBuf);
/*ctf_c_001.main_1*/
   if(param->pres != FALSE)
   {
      CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
      param->pA = (CtfPdschPaParam)tmpEnum;
   }
   return ROK;
}



/***********************************************************
*
*     Func: cmPkCtfDedPucchCfgInfo
*
*
*     Desc:   Dedicated PUCCH Configuration
 * @details This structure contains the information for setting-up/release 
 *          PUCCH configuration specific to an UE.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfDedPucchCfgInfo
(
CtfDedPucchCfgInfo *param,
Buffer *mBuf
)
{


/*ctf_c_001.main_1*/
  if(param->pres != FALSE)
  {
#ifdef CTF_VER3
     /*ctf_c_001.main_4:115549: packing tddAckNackFBMode and tddAckNackCfgPres */
       if (param->tddAckNackCfgPres != FALSE)
       {
          CMCHKPK(oduUnpackUInt32, param->tddAckNackFBMode, mBuf);
       }
 
       CMCHKPK(oduUnpackUInt8, param->tddAckNackCfgPres, mBuf);
#endif /* CTF_VER3 */
      CMCHKPK(oduUnpackUInt16, param->pucchSetup.n1PUCCHRep, mBuf);
#ifdef CTF_VER3
      /*ctf_c_001.main_4:115549: packing pucchSetup.repFactPres */
       if (param->pucchSetup.repFactPres)
       {
          CMCHKPK(oduUnpackUInt32, param->pucchSetup.repFact, mBuf);
       }
 
       CMCHKPK(oduUnpackUInt8, param->pucchSetup.repFactPres, mBuf);
#else
        CMCHKPK(oduUnpackUInt32, param->pucchSetup.repFact, mBuf);
#endif /* CTF_VER3 */
      CMCHKPK(oduUnpackUInt8, param->dedPucchCfgType, mBuf);
  }
  CMCHKPK(oduUnpackUInt8, param->pres, mBuf);
  return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfDedPucchCfgInfo
*
*
*     Desc:   Dedicated PUCCH Configuration
 * @details This structure contains the information for setting-up/release 
 *          PUCCH configuration specific to an UE.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfDedPucchCfgInfo
(
CtfDedPucchCfgInfo *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->pres, mBuf);
/*ctf_c_001.main_1*/
   if(param->pres != FALSE)
   {
      CMCHKUNPK(oduPackUInt8, &param->dedPucchCfgType, mBuf);
#ifdef CTF_VER3
      /* ctf_c_001.main_4:115549: unpacking pucchSetup.repFactPres */
       CMCHKUNPK(oduPackUInt8, &param->pucchSetup.repFactPres, mBuf);
 
       if (param->pucchSetup.repFactPres)
       {
          CMCHKUNPK(oduPackUInt32, (uint32_t *)&param->pucchSetup.repFact, mBuf);
       }
#else
        CMCHKUNPK(oduPackUInt32, (uint32_t *)&param->pucchSetup.repFact, mBuf);
#endif /*CTF_VER3 */
      CMCHKUNPK(oduPackUInt16, &param->pucchSetup.n1PUCCHRep, mBuf);
#ifdef CTF_VER3
      /*ctf_c_001.main_4:115549: unpacking tddAckNackCfgPres and tddAckNackFBMode */
      CMCHKUNPK(oduPackUInt8, &param->tddAckNackCfgPres, mBuf);

      if (param->tddAckNackCfgPres != FALSE)
      {
         CMCHKUNPK(oduPackUInt32, (uint32_t *)&param->tddAckNackFBMode, mBuf);
      }
#endif /* CTF_VER3 */
   }
   return ROK;
}



/***********************************************************
*
*     Func: cmPkCtfDedPuschCfgInfo
*
*
*     Desc:   Dedicated PUSCH Configuration
 * @details This structure contains the information for PUCCH 
 *          configuration specific to an UE.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfDedPuschCfgInfo
(
CtfDedPuschCfgInfo *param,
Buffer *mBuf
)
{

/*ctf_c_001.main_1*/
   if(param->pres != FALSE)
   {
      CMCHKPK(oduUnpackUInt8, param->betaOffsetCqiIdx, mBuf);
      CMCHKPK(oduUnpackUInt8, param->betaOffsetRiIdx, mBuf);
      CMCHKPK(oduUnpackUInt8, param->betaOffsetAckIdx, mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->pres, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfDedPuschCfgInfo
*
*
*     Desc:   Dedicated PUSCH Configuration
 * @details This structure contains the information for PUCCH 
 *          configuration specific to an UE.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfDedPuschCfgInfo
(
CtfDedPuschCfgInfo *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt8, &param->pres, mBuf);
/*ctf_c_001.main_1*/
   if(param->pres != FALSE)
   {
      CMCHKUNPK(oduPackUInt8, &param->betaOffsetAckIdx, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->betaOffsetRiIdx, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->betaOffsetCqiIdx, mBuf);
   }
   return ROK;
}


#ifndef TFU_UPGRADE

/***********************************************************
*
*     Func: cmPkCtfCqiRptModePeriodic
*
*
*     Desc:   Periodic CQI Report Configuration
 * @details This structure contains the information for periodic CQI report
 *          related configuration specific to an UE.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfCqiRptModePeriodic
(
CtfCqiRptModePeriodic *param,
Buffer *mBuf
)
{

/* ctf_c_001.main_2: added packning for cqiMask*/
#ifdef CTF_VER1
      CMCHKPK(oduUnpackUInt32, (uint32_t )param->cqiSetup.cqiMask.cqiMaskSetup, mBuf);
      CMCHKPK(oduUnpackUInt8, param->cqiSetup.cqiMask.pres, mBuf);
#endif /* CTF_VER1 */
      CMCHKPK(oduUnpackUInt8, param->cqiSetup.simultaneousAckNack, mBuf);
      CMCHKPK(oduUnpackUInt16, param->cqiSetup.riCfgIndx, mBuf);
      CMCHKPK(oduUnpackUInt8, param->cqiSetup.riCfgPres, mBuf);
      CMCHKPK(oduUnpackUInt8, param->cqiSetup.subbandCqi.k, mBuf);
      CMCHKPK(oduUnpackUInt8, param->cqiSetup.formatIndicator, mBuf);
      CMCHKPK(oduUnpackUInt16, param->cqiSetup.cqiPmiCfgIndx, mBuf);
      CMCHKPK(oduUnpackUInt16, param->cqiSetup.cqiPUCCHRsrcIndx, mBuf);
   CMCHKPK(oduUnpackUInt8, param->cqiPeriodicCfgType, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfCqiRptModePeriodic
*
*
*     Desc:   Periodic CQI Report Configuration
 * @details This structure contains the information for periodic CQI report
 *          related configuration specific to an UE.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfCqiRptModePeriodic
(
CtfCqiRptModePeriodic *param,
Buffer *mBuf
)
{
#ifdef CTF_VER1    
   uint32_t tmpEnum;
#endif   

   CMCHKUNPK(oduPackUInt8, &param->cqiPeriodicCfgType, mBuf);
      CMCHKUNPK(oduPackUInt16, &param->cqiSetup.cqiPUCCHRsrcIndx, mBuf);
      CMCHKUNPK(oduPackUInt16, &param->cqiSetup.cqiPmiCfgIndx, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->cqiSetup.formatIndicator, mBuf);
         CMCHKUNPK(oduPackUInt8, &param->cqiSetup.subbandCqi.k, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->cqiSetup.riCfgPres, mBuf);
      CMCHKUNPK(oduPackUInt16, &param->cqiSetup.riCfgIndx, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->cqiSetup.simultaneousAckNack, mBuf);
/* ctf_c_001.main_2: added Unpackning for cqiMask*/
#ifdef CTF_VER1
      CMCHKUNPK(oduPackUInt8, &param->cqiSetup.cqiMask.pres, mBuf);
      CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
      param->cqiSetup.cqiMask.cqiMaskSetup = tmpEnum;
#endif /* CTF_VER1 */
   return ROK;
}



/***********************************************************
*
*     Func: cmPkCtfCqiReportCfgInfo
*
*
*     Desc:   CQI Report Configuration
 * @details This structure contains the information for CQI report
 *          related configuration specific to an UE.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfCqiReportCfgInfo
(
CtfCqiReportCfgInfo *param,
Buffer *mBuf
)
{

/*ctf_c_001.main_1*/
   if(param->pres != FALSE)
   {
#ifdef CTF_VER2
      /* 
       * ctf_c_001.main_2: Added provision to send Periodic and aPeriodic CQI
       *report configuration in the same message
       */
       if(param->reportMode.aPeriodicRpt.pres != FALSE)
       {
           CMCHKPK(oduUnpackUInt32, param->reportMode.aPeriodicRpt.aPeriodicRptMode, mBuf);
       }
       CMCHKPK(oduUnpackUInt8, param->reportMode.aPeriodicRpt.pres, mBuf);

       if(param->reportMode.periodicRpt.pres != FALSE)
       {
           CMCHKPK(cmPkCtfCqiRptModePeriodic, &param->reportMode.periodicRpt, mBuf);
       }
       CMCHKPK(oduUnpackUInt8, param->reportMode.periodicRpt.pres, mBuf);

#else /* CTF_VER2 */

      switch(param->reportingMode) {
         case CTF_CQI_RPTMODE_PRDIOC:
            CMCHKPK(cmPkCtfCqiRptModePeriodic, &param->reportMode.periodicRpt, mBuf);
            break;
         case CTF_CQI_RPTMODE_APRDIOC:
            CMCHKPK(oduUnpackUInt32, param->reportMode.aPeriodicRpt, mBuf);
            break;
         default :
            return RFAILED;
      }
      CMCHKPK(oduUnpackUInt8, param->reportingMode, mBuf);
#endif /* CTF_VER2 */

/* ctf_c_001.main_2: added packning for pmiRiRpt*/
#ifdef CTF_VER1
       CMCHKPK(oduUnpackUInt32, param->pmiRiRpt.pmiRiRptSetup, mBuf);
       CMCHKPK(oduUnpackUInt8, param->pmiRiRpt.pres, mBuf);
#endif /* CTF_VER1 */
   }

   CMCHKPK(oduUnpackUInt8, param->pres, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfCqiReportCfgInfo
*
*
*     Desc:   CQI Report Configuration
 * @details This structure contains the information for CQI report
 *          related configuration specific to an UE.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfCqiReportCfgInfo
(
CtfCqiReportCfgInfo *param,
Buffer *mBuf
)
{
#if defined (CTF_VER1) || defined (CTF_VER2)
   uint32_t tmpEnum;
#endif

   CMCHKUNPK(oduPackUInt8, &param->pres, mBuf);
/*ctf_c_001.main_1*/
   if(param->pres != FALSE)
   {
/* ctf_c_001.main_2: added Unpackning for cqiMask*/
#ifdef CTF_VER1
      CMCHKUNPK(oduPackUInt8, &param->pmiRiRpt.pres, mBuf);
      CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
      param->pmiRiRpt.pmiRiRptSetup = tmpEnum;
#endif /* CTF_VER1 */

#ifdef CTF_VER2
      /* 
       *ctf_c_001.main_2: Added provision to send Periodic and aPeriodic CQI
       *report configuration in the same message
       */
      CMCHKUNPK(oduPackUInt8, &param->reportMode.periodicRpt.pres, mBuf)
      if(param->reportMode.periodicRpt.pres != FALSE)
      {
         CMCHKUNPK(cmUnpkCtfCqiRptModePeriodic, &param->reportMode.periodicRpt, mBuf);
      }
       
      CMCHKUNPK(oduPackUInt8, &param->reportMode.aPeriodicRpt.pres, mBuf)
      if(param->reportMode.aPeriodicRpt.pres != FALSE)
      {
	 CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
         param->reportMode.aPeriodicRpt.aPeriodicRptMode =
            (CtfCqiRptModeAperiodic)tmpEnum;
      }

#else /* CTF_VER2 */

      CMCHKUNPK(oduPackUInt8, &param->reportingMode, mBuf);
      switch(param->reportingMode) {
         case CTF_CQI_RPTMODE_APRDIOC:
            CMCHKUNPK(oduPackUInt32, (uint32_t *)&param->reportMode.aPeriodicRpt, mBuf);
            break;
         case CTF_CQI_RPTMODE_PRDIOC:
            CMCHKUNPK(cmUnpkCtfCqiRptModePeriodic, &param->reportMode.periodicRpt, mBuf);
            break;
         default :
            return RFAILED;
      }
#endif /* CTF_VER2 */
   }
   return ROK;
}



/***********************************************************
*
*     Func: cmPkCtfDedSrsUlCfgInfo
*
*
*     Desc:   Dedicated Uplink SRS Configuration
 * @details This structure contains the information for setting-up/release
 *          UL SRS configuration specific to an UE.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfDedSrsUlCfgInfo
(
CtfDedSrsUlCfgInfo *param,
Buffer *mBuf
)
{

/*ctf_c_001.main_1*/
   if(param->pres != FALSE)
   {
      CMCHKPK(oduUnpackUInt8, param->dedSrsSetup.cyclicShift, mBuf);
      CMCHKPK(oduUnpackUInt8, param->dedSrsSetup.txComb, mBuf);
      CMCHKPK(oduUnpackUInt16, param->dedSrsSetup.srsCfgIdx, mBuf);
      CMCHKPK(oduUnpackUInt8, param->dedSrsSetup.duration, mBuf);
      CMCHKPK(oduUnpackUInt8, param->dedSrsSetup.freqDmnPos, mBuf);
      CMCHKPK(oduUnpackUInt8, param->dedSrsSetup.srsHopngBw, mBuf);
      CMCHKPK(oduUnpackUInt8, param->dedSrsSetup.srsBw, mBuf);
      CMCHKPK(oduUnpackUInt8, param->dedSrsUlCfgType, mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->pres, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfDedSrsUlCfgInfo
*
*
*     Desc:   Dedicated Uplink SRS Configuration
 * @details This structure contains the information for setting-up/release
 *          UL SRS configuration specific to an UE.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfDedSrsUlCfgInfo
(
CtfDedSrsUlCfgInfo *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt8, &param->pres, mBuf);
/*ctf_c_001.main_1*/
   if(param->pres != FALSE)
   {
      CMCHKUNPK(oduPackUInt8, &param->dedSrsUlCfgType, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->dedSrsSetup.srsBw, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->dedSrsSetup.srsHopngBw, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->dedSrsSetup.freqDmnPos, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->dedSrsSetup.duration, mBuf);
      CMCHKUNPK(oduPackUInt16, &param->dedSrsSetup.srsCfgIdx, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->dedSrsSetup.txComb, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->dedSrsSetup.cyclicShift, mBuf);
   }
   return ROK;
}



/***********************************************************
*
*     Func: cmPkCtfDedSRCfgInfo
*
*
*     Desc:   Dedicated SR Configuration
 * @details This structure contains the information for setting-up/release
 *          SR configuration specific to an UE at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfDedSRCfgInfo
(
CtfDedSRCfgInfo *param,
Buffer *mBuf
)
{

/*ctf_c_001.main_1*/
  if(param->pres != FALSE)
  {
     CMCHKPK(oduUnpackUInt8, param->dedSrSetup.srCfgIdx, mBuf);
     CMCHKPK(oduUnpackUInt16, param->dedSrSetup.srPUCCHRi, mBuf);
     CMCHKPK(oduUnpackUInt8, param->dedSRCfgType, mBuf);
  }
  CMCHKPK(oduUnpackUInt8, param->pres, mBuf);
  return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfDedSRCfgInfo
*
*
*     Desc:   Dedicated SR Configuration
 * @details This structure contains the information for setting-up/release
 *          SR configuration specific to an UE at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfDedSRCfgInfo
(
CtfDedSRCfgInfo *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->pres, mBuf);
/*ctf_c_001.main_1*/
   if(param->pres != FALSE)
   {
      CMCHKUNPK(oduPackUInt8, &param->dedSRCfgType, mBuf);
      CMCHKUNPK(oduPackUInt16, &param->dedSrSetup.srPUCCHRi, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->dedSrSetup.srCfgIdx, mBuf);
   }
   return ROK;
}

#endif /* TFU_UPGRADE */


/***********************************************************
*
*     Func: cmPkCtfDedAntCfgInfo
*
*
*     Desc:   Dedicated Antenna Configuration
 * @details This structure contains the information for antenna related
 *          configuration specific to an UE at PHY.
*
*
*     Ret : S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfDedAntCfgInfo
(
CtfDedAntCfgInfo *param,
Buffer *mBuf
)
{
   
#ifdef CTF_VER2
   uint8_t  idx;
#endif  /* CTF_VER2 */


/*ctf_c_001.main_1*/
   if(param->pres != FALSE)
   {
      CMCHKPK(oduUnpackUInt32, param->ueTxAntSelection.txAntSelect, mBuf);
      CMCHKPK(oduUnpackUInt8, param->ueTxAntSelection.cfgType, mBuf);
     /* ctf_c_001.main_3 txMode is of type enum and is 32bit length */
      CMCHKPK(oduUnpackUInt32, (uint32_t)param->txMode, mBuf);
#ifdef CTF_VER2
      /* 
       *ctf_c_001.main_2: Added Packing of CodeBookSubsetRestriction parameter
       */
      switch(param->codeBookSubsetRes.ctfCdBkRestType)
      {
          case N2TX_ANT_TM3:
              CMCHKPK(oduUnpackUInt8, param->codeBookSubsetRes.codeBookSubsetRestn.
			     n2TxAntTm3, mBuf);
	      break;

          case N4TX_ANT_TM3:
              CMCHKPK(oduUnpackUInt8, param->codeBookSubsetRes.codeBookSubsetRestn.
			     n4TxAntTm3, mBuf);
	      break;

          case N2TX_ANT_TM4:
              CMCHKPK(oduUnpackUInt8, param->codeBookSubsetRes.codeBookSubsetRestn.
			     n2TxAntTm4, mBuf);
	      break;

          case N4TX_ANT_TM4:
              for(idx = 0; idx < 8; idx++)
                  CMCHKPK(oduUnpackUInt8, param->codeBookSubsetRes.codeBookSubsetRestn.
				 n4TxAntTm4[idx], mBuf);
	      break;

          case N2TX_ANT_TM5:
              CMCHKPK(oduUnpackUInt8, param->codeBookSubsetRes.codeBookSubsetRestn.
			     n2TxAntTm5, mBuf);
	      break;

          case N4TX_ANT_TM5:
              CMCHKPK(oduUnpackUInt16, param->codeBookSubsetRes.codeBookSubsetRestn.
			      n4TxAntTm5, mBuf);
	      break;

          case N2TX_ANT_TM6:
              CMCHKPK(oduUnpackUInt8, param->codeBookSubsetRes.codeBookSubsetRestn.
			     n2TxAntTm6, mBuf);
	      break;

          case N4TX_ANT_TM6:
              CMCHKPK(oduUnpackUInt16, param->codeBookSubsetRes.codeBookSubsetRestn.
			      n4TxAntTm6, mBuf);
	      break;
	  default :
              break;

      }
      CMCHKPK(oduUnpackUInt32, param->codeBookSubsetRes.ctfCdBkRestType, mBuf);
#endif /* CTF_VER2 */
   }
   CMCHKPK(oduUnpackUInt8, param->pres, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfDedAntCfgInfo
*
*
*     Desc:   Dedicated Antenna Configuration
 * @details This structure contains the information for antenna related
 *          configuration specific to an UE at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfDedAntCfgInfo
(
CtfDedAntCfgInfo *param,
Buffer *mBuf
)
{
#ifdef CTF_VER2
   S16  idx;
#endif /* CTF_VER2 */
   
   uint32_t tmpEnum;


   CMCHKUNPK(oduPackUInt8, &param->pres, mBuf);
/*ctf_c_001.main_1*/
   if(param->pres != FALSE)
   {
#ifdef CTF_VER2
      /* 
       * Added Unpacking for CodeBookSubsetRestriction parameter
       */
      CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
      param->codeBookSubsetRes.ctfCdBkRestType = (CtfCdBkSubsetRestnType)tmpEnum;
      switch(param->codeBookSubsetRes.ctfCdBkRestType)
      {
          case N2TX_ANT_TM3:
              CMCHKUNPK(oduPackUInt8, &param->codeBookSubsetRes.codeBookSubsetRestn.
			     n2TxAntTm3, mBuf);
	      break;

          case N4TX_ANT_TM3:
              CMCHKUNPK(oduPackUInt8, &param->codeBookSubsetRes.codeBookSubsetRestn.
			     n4TxAntTm3, mBuf);
	      break;

          case N2TX_ANT_TM4:
              CMCHKUNPK(oduPackUInt8, &param->codeBookSubsetRes.codeBookSubsetRestn.
			     n2TxAntTm4, mBuf);
	      break;

          case N4TX_ANT_TM4:
              for(idx = 7; idx >= 0; idx--)
              {
                 CMCHKUNPK(oduPackUInt8, &(param->codeBookSubsetRes.codeBookSubsetRestn.
                       n4TxAntTm4[idx]), mBuf);
              }
	      break;

          case N2TX_ANT_TM5:
              CMCHKUNPK(oduPackUInt8, &param->codeBookSubsetRes.codeBookSubsetRestn.
			     n2TxAntTm5, mBuf);
	      break;

          case N4TX_ANT_TM5:
              CMCHKUNPK(oduPackUInt16, &param->codeBookSubsetRes.codeBookSubsetRestn.
			      n4TxAntTm5, mBuf);
	      break;

          case N2TX_ANT_TM6:
              CMCHKUNPK(oduPackUInt8, &param->codeBookSubsetRes.codeBookSubsetRestn.
			     n2TxAntTm6, mBuf);
	      break;

          case N4TX_ANT_TM6:
              CMCHKUNPK(oduPackUInt16, &param->codeBookSubsetRes.codeBookSubsetRestn.
			      n4TxAntTm6, mBuf);
	      break;
	  default :
              break;
      }
#endif /* CTF_VER2 */
      /* ctf_c_001.main_3 Enum is 32bit length */
      CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
      param->txMode = (CtfUeTxAntMode)tmpEnum;
      CMCHKUNPK(oduPackUInt8, &param->ueTxAntSelection.cfgType, mBuf);
      CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpEnum, mBuf);
      param->ueTxAntSelection.txAntSelect = (CtfUeTxAntSelectType)tmpEnum;
   }
   return ROK;
}


/***********************************************************
*
*     Func: cmPkCtfUeSecCellCfgInfo
*
*
*     Desc:   Secondary Cell Configuration for a UE
 * @details This structure contains the information for 
*           secondary cells for the UE
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfUeSecCellCfgInfo
(
CtfUeSecCellCfgInfo *param,
Buffer *mBuf
)
{
   CMCHKPK(oduUnpackUInt16, param->sCellId, mBuf);
   CMCHKPK(oduUnpackUInt8, param->sCellIdx, mBuf);

   return ROK;
}
/***********************************************************
*
*     Func: cmPkCtfSecCellCfgInfo
*
*
*     Desc:   Secondary Cell Configuration for a UE
 * @details This structure contains the information for 
*           secondary cells for the UE
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfSecCellCfgInfo
(
CtfSecCellCfgInfo *param,
Buffer *mBuf
)
{
   S8 idx;

   for(idx = param->numSCells - 1; idx >= 0; idx--)
   {
      CMCHKPK(cmPkCtfUeSecCellCfgInfo, &param->ueSCellDedCfg[(uint8_t)idx], mBuf);
   }
   CMCHKPK(oduUnpackUInt32, param->sCellAction, mBuf);
   CMCHKPK(oduUnpackUInt8, param->numSCells, mBuf);

   return ROK;
}

/***********************************************************
*
*     Func: cmUnpkCtfUeSecCellCfgInfo
*
*
*     Desc:   Secondary Cell Configuration for a UE
 * @details This structure contains the information for 
*           secondary cells for the UE
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfUeSecCellCfgInfo
(
CtfUeSecCellCfgInfo *param,
Buffer *mBuf
)
{
   CMCHKUNPK(oduPackUInt8, &param->sCellIdx, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->sCellId, mBuf);

   return ROK;
}

/***********************************************************
*
*     Func: cmUnpkCtfSecCellCfgInfo
*
*
*     Desc:   Secondary Cell Configuration for a UE
 * @details This structure contains the information for 
*           secondary cells for the UE
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/

S16 cmUnpkCtfSecCellCfgInfo
(
CtfSecCellCfgInfo *param,
Buffer *mBuf
)
{
   uint8_t idx;

   CMCHKUNPK(oduPackUInt8, &param->numSCells, mBuf);
   CMCHKUNPK(oduPackUInt32, (uint32_t*)&param->sCellAction, mBuf);
   for(idx = 0 ; idx < param->numSCells; idx++)
   {
      CMCHKUNPK(cmUnpkCtfUeSecCellCfgInfo, &param->ueSCellDedCfg[idx], mBuf);
   }

   return ROK;
}


/***********************************************************
*
*     Func: cmPkCtfUeCatCfgInfo
*
*
*     Desc:   UE Category Configuration
 * @details This structure contains the information for configuring
 *          the UE category at PHY.
*
*
*     Ret : S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfUeCatCfgInfo
(
CtfUeCatCfgInfo *param,
Buffer *mBuf
)
{


/*ctf_c_001.main_1*/
   if(param->pres != FALSE)
      CMCHKPK(oduUnpackUInt32, param->ueCategory, mBuf);
   CMCHKPK(oduUnpackUInt8, param->pres, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfUeCatCfgInfo
*
*
*     Desc:   UE Category Configuration
 * @details This structure contains the information for configuring
 *          the UE category at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfUeCatCfgInfo
(
CtfUeCatCfgInfo *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->pres, mBuf);
/*ctf_c_001.main_1*/
   if(param->pres != FALSE)
      CMCHKUNPK(oduPackUInt32, (uint32_t *)&param->ueCategory, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmPkCtfDedCfgInfo
*
*
*     Desc:   Dedicated Configuration Information
 * @details This structure contains the information for configuration 
 *          of UE-specific physical channel resources at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfDedCfgInfo
(
CtfDedCfgInfo *param,
Buffer *mBuf
)
{

#ifdef EMTC_ENABLE
   CMCHKPK(oduUnpackUInt8, param->isEmtcUe, mBuf);
#endif
   CMCHKPK(cmPkCtfDedAntCfgInfo, &param->antInfo, mBuf);
#ifndef TFU_UPGRADE
   CMCHKPK(cmPkCtfDedSRCfgInfo, &param->dedSRCfg, mBuf);
   CMCHKPK(cmPkCtfDedSrsUlCfgInfo, &param->srsUlCfg, mBuf);
   CMCHKPK(cmPkCtfCqiReportCfgInfo, &param->cqiRptCfg, mBuf);
#endif /* TFU_UPGRADE */
   CMCHKPK(cmPkCtfDedPuschCfgInfo, &param->puschCfg, mBuf);
   CMCHKPK(cmPkCtfDedPucchCfgInfo, &param->pucchCfg, mBuf);
   CMCHKPK(cmPkCtfDedPdschCfgInfo, &param->pdschCfg, mBuf);
   CMCHKPK(cmPkCtfUeCatCfgInfo, &param->ueCatCfg, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfDedCfgInfo
*
*
*     Desc:   Dedicated Configuration Information
 * @details This structure contains the information for configuration 
 *          of UE-specific physical channel resources at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfDedCfgInfo
(
CtfDedCfgInfo *param,
Buffer *mBuf
)
{

   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkCtfUeCatCfgInfo, &param->ueCatCfg, mBuf);
   CMCHKUNPK(cmUnpkCtfDedPdschCfgInfo, &param->pdschCfg, mBuf);
   CMCHKUNPK(cmUnpkCtfDedPucchCfgInfo, &param->pucchCfg, mBuf);
   CMCHKUNPK(cmUnpkCtfDedPuschCfgInfo, &param->puschCfg, mBuf);
#ifndef TFU_UPGRADE
   CMCHKUNPK(cmUnpkCtfCqiReportCfgInfo, &param->cqiRptCfg, mBuf);
   CMCHKUNPK(cmUnpkCtfDedSrsUlCfgInfo, &param->srsUlCfg, mBuf);
   CMCHKUNPK(cmUnpkCtfDedSRCfgInfo, &param->dedSRCfg, mBuf);
#endif /* TFU_UPGRADE */
   CMCHKUNPK(cmUnpkCtfDedAntCfgInfo, &param->antInfo, mBuf);
#ifdef EMTC_ENABLE
   CMCHKUNPK(oduPackUInt8, &param->isEmtcUe, mBuf);
#endif
   return ROK;
}



/***********************************************************
*
*     Func: cmPkCtfDedRecfgInfo
*
*
*     Desc:   Dedicated Re-configuration Information
 * @details This structure contains the information for re-configuration 
 *          of UE-specific physical channel resources at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfDedRecfgInfo
(
CtfDedRecfgInfo *param,
Buffer *mBuf
)
{

   CMCHKPK(cmPkCtfSecCellCfgInfo, &param->sCellInfo,mBuf);
   /*ctf_c_001.main_1*/
   CMCHKPK(cmPkCtfDedAntCfgInfo, &param->antInfo, mBuf);
#ifndef TFU_UPGRADE
   CMCHKPK(cmPkCtfCqiReportCfgInfo, &param->cqiRptCfg, mBuf);
   CMCHKPK(cmPkCtfDedSRCfgInfo, &param->dedSRCfg, mBuf);
   CMCHKPK(cmPkCtfDedSrsUlCfgInfo, &param->srsUlCfg, mBuf);
#endif /* TFU_UPGRADE */
   CMCHKPK(cmPkCtfDedPuschCfgInfo, &param->puschCfg, mBuf);
   CMCHKPK(cmPkCtfDedPucchCfgInfo, &param->pucchCfg, mBuf);
   CMCHKPK(cmPkCtfDedPdschCfgInfo, &param->pdschCfg, mBuf);
   CMCHKPK(cmPkCtfUeCatCfgInfo, &param->ueCatCfg, mBuf);
   /* ctf_c_001.main_4: ReEst Fix */
#ifdef CTF_VER3
   CMCHKPK(cmPkLteRnti, param->newUeId, mBuf);
#else
   CMCHKPK(cmPkLteRnti, param->newUeId, mBuf);
#endif
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfDedRecfgInfo
*
*
*     Desc:   Dedicated Re-configuration Information
 * @details This structure contains the information for re-configuration 
 *          of UE-specific physical channel resources at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfDedRecfgInfo
(
CtfDedRecfgInfo *param,
Buffer *mBuf
)
{

   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   /* ctf_c_001.main_4: ReEst Fix */
#ifdef CTF_VER3
   CMCHKUNPK(cmUnpkLteRnti, &param->newUeId, mBuf);
#else
   CMCHKUNPK(cmUnpkLteRnti, &param->newUeId, mBuf);
#endif
   CMCHKUNPK(cmUnpkCtfUeCatCfgInfo, &param->ueCatCfg, mBuf);
   CMCHKUNPK(cmUnpkCtfDedPdschCfgInfo, &param->pdschCfg, mBuf);
   CMCHKUNPK(cmUnpkCtfDedPucchCfgInfo, &param->pucchCfg, mBuf);
   CMCHKUNPK(cmUnpkCtfDedPuschCfgInfo, &param->puschCfg, mBuf);
#ifndef TFU_UPGRADE
   CMCHKUNPK(cmUnpkCtfDedSrsUlCfgInfo, &param->srsUlCfg, mBuf);
   CMCHKUNPK(cmUnpkCtfDedSRCfgInfo, &param->dedSRCfg, mBuf);
   /*ctf_c_001.main_1*/
   CMCHKUNPK(cmUnpkCtfCqiReportCfgInfo, &param->cqiRptCfg, mBuf);
#endif /* TFU_UPGRADE */
   CMCHKUNPK(cmUnpkCtfDedAntCfgInfo, &param->antInfo, mBuf);

   CMCHKUNPK(cmUnpkCtfSecCellCfgInfo, &param->sCellInfo,mBuf);

   return ROK;
}



/***********************************************************
*
*     Func: cmPkCtfCellReleaseInfo
*
*
*     Desc:   Cell Release Information
 * @details This structure contains the information for release/removal 
 *          of a cell and its assocated resources at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfCellReleaseInfo
(
CtfCellReleaseInfo *param,
Buffer *mBuf
)
{

   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfCellReleaseInfo
*
*
*     Desc:   Cell Release Information
 * @details This structure contains the information for release/removal 
 *          of a cell and its assocated resources at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfCellReleaseInfo
(
CtfCellReleaseInfo *param,
Buffer *mBuf
)
{

   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmPkCtfDedReleaseInfo
*
*
*     Desc:   Dedicated Release Information
 * @details This structure contains the information for release/removal 
 *          of dedicated/UE-specific configuration at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfDedReleaseInfo
(
CtfDedReleaseInfo *param,
Buffer *mBuf
)
{

   CMCHKPK(cmPkCtfSecCellCfgInfo, &param->sCellInfo,mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfDedReleaseInfo
*
*
*     Desc:   Dedicated Release Information
 * @details This structure contains the information for release/removal 
 *          of dedicated/UE-specific configuration at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfDedReleaseInfo
(
CtfDedReleaseInfo *param,
Buffer *mBuf
)
{

   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkCtfSecCellCfgInfo, &param->sCellInfo,mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmPkCtfCfgInfo
*
*
*     Desc:   Configuration Information
 * @details This structure contains the information for setting-up 
 *          cell or UE-specific configuration at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfCfgInfo
(
CtfCfgInfo *param,
Buffer *mBuf
)
{

      switch(param->cfgElem) {
         case CTF_UE_CFG:
            CMCHKPK(cmPkCtfDedCfgInfo, &param->u.dedCfg, mBuf);
            break;
         case CTF_CELL_CFG:
            CMCHKPK(cmPkCtfCellCfgInfo, &param->u.cellCfg, mBuf);
            break;
         default :
            return RFAILED;
      }
   CMCHKPK(oduUnpackUInt8, param->cfgElem, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfCfgInfo
*
*
*     Desc:   Configuration Information
 * @details This structure contains the information for setting-up 
 *          cell or UE-specific configuration at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfCfgInfo
(
CtfCfgInfo *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->cfgElem, mBuf);
      switch(param->cfgElem) {
         case CTF_CELL_CFG:
            CMCHKUNPK(cmUnpkCtfCellCfgInfo, &param->u.cellCfg, mBuf);
            break;
         case CTF_UE_CFG:
            CMCHKUNPK(cmUnpkCtfDedCfgInfo, &param->u.dedCfg, mBuf);
            break;
         default :
            return RFAILED;
      }
   return ROK;
}



/***********************************************************
*
*     Func: cmPkCtfReCfgInfo
*
*
*     Desc:   Re-configuration Information
 * @details This structure contains the information for re-configuring 
 *          cell or UE-specific configuration at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfReCfgInfo
(
CtfReCfgInfo *param,
Buffer *mBuf
)
{


      switch(param->cfgElem) {
         case CTF_UE_CFG:
            CMCHKPK(cmPkCtfDedRecfgInfo, &param->u.dedRecfg, mBuf);
            break;
         case CTF_CELL_CFG:
            CMCHKPK(cmPkCtfCellRecfgInfo, &param->u.cellRecfg, mBuf);
            break;
         /* Starts - Fix for CR ccpu00123185 */
         case CTF_TX_PWR_CFG:
            CMCHKPK(cmPkCtfCellPwrcfgInfo, &param->u.cellRecfg, mBuf);
            break;
         /* ends - Fix for CR ccpu00123185 */
         case CTF_CELL_STOP:
            CMCHKPK(cmPkCtfCellStop, &param->u.cellRecfg, mBuf);
            break;
             
         default :
            return RFAILED;
      }
   CMCHKPK(oduUnpackUInt8, param->cfgElem, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfReCfgInfo
*
*
*     Desc:   Re-configuration Information
 * @details This structure contains the information for re-configuring 
 *          cell or UE-specific configuration at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfReCfgInfo
(
CtfReCfgInfo *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->cfgElem, mBuf);
      switch(param->cfgElem) {
         case CTF_CELL_CFG:
            CMCHKUNPK(cmUnpkCtfCellRecfgInfo, &param->u.cellRecfg, mBuf);
            break;
         case CTF_UE_CFG:
            CMCHKUNPK(cmUnpkCtfDedRecfgInfo, &param->u.dedRecfg, mBuf);
            break;
         /* Starts - Fix for CR ccpu00123185 */
         case CTF_TX_PWR_CFG:
            CMCHKUNPK(cmUnpkCtfCellPwrcfgInfo, &param->u.cellRecfg, mBuf);
            break;
         /* ends - Fix for CR ccpu00123185 */
         case CTF_CELL_STOP:
            CMCHKUNPK(cmUnpkCtfCellStop, &param->u.cellRecfg, mBuf);
            break;
         default :
            return RFAILED;
      }
   return ROK;
}



/***********************************************************
*
*     Func: cmPkCtfReleaseInfo
*
*
*     Desc:   Release configuration Information
 * @details This structure contains the information for releasing 
 *          cell or UE-specific configuration at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfReleaseInfo
(
CtfReleaseInfo *param,
Buffer *mBuf
)
{


      switch(param->cfgElem) {
         case CTF_UE_CFG:
            CMCHKPK(cmPkCtfDedReleaseInfo, &param->u.dedRel, mBuf);
            break;
         case CTF_CELL_CFG:
            CMCHKPK(cmPkCtfCellReleaseInfo, &param->u.cellRel, mBuf);
            break;
         default :
            return RFAILED;
      }
   CMCHKPK(oduUnpackUInt8, param->cfgElem, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfReleaseInfo
*
*
*     Desc:   Release configuration Information
 * @details This structure contains the information for releasing 
 *          cell or UE-specific configuration at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfReleaseInfo
(
CtfReleaseInfo *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->cfgElem, mBuf);
      switch(param->cfgElem) {
         case CTF_CELL_CFG:
            CMCHKUNPK(cmUnpkCtfCellReleaseInfo, &param->u.cellRel, mBuf);
            break;
         case CTF_UE_CFG:
            CMCHKUNPK(cmUnpkCtfDedReleaseInfo, &param->u.dedRel, mBuf);
            break;
         default :
            return RFAILED;
      }
   return ROK;
}



/***********************************************************
*
*     Func: cmPkCtfCfgReqInfo
*
*
*     Desc:   Configuration Request information
 * @details This is the main structure in the Configuration request primitive.
 *    -# This structure contains the configuration information as given by 
 *       the RRC to configure PHY layer for common/dedicated resources.
 *    -# It can contain Config/Reconfig/Release.
 *    -# The result of configuration is indicated in TfUiCtfCfgCfm.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfCfgReqInfo
(
CtfCfgReqInfo *param,
Buffer *mBuf
)
{

   /* ctf_c_001.main_4: Added support for vendor specific parameters */
#ifdef CTF_VER3
   CMCHKPK(cmPkCtfVendorSpecParams, &param->vendorParams, mBuf);
#endif    
      switch(param->cfgType) {
         case CTF_DELETE:
            CMCHKPK(cmPkCtfReleaseInfo, &param->u.release, mBuf);
            break;
         case CTF_RECONFIG:
            CMCHKPK(cmPkCtfReCfgInfo, &param->u.reCfg, mBuf);
            break;
         case CTF_CONFIG:
            CMCHKPK(cmPkCtfCfgInfo, &param->u.cfg, mBuf);
            break;
         default :
            return RFAILED;
      }
   CMCHKPK(oduUnpackUInt8, param->cfgType, mBuf);
   return ROK;
}



   /* ctf_c_001.main_4: Modified cmUnpkCtfCfgReqInfo() to add support
    * for vendor specific parameters */
/***********************************************************
*
*     Func: cmUnpkCtfCfgReqInfo
*
*
*     Desc:   Configuration Request information
 * @details This is the main structure in the Configuration request primitive.
 *    -# This structure contains the configuration information as given by 
 *       the RRC to configure PHY layer for common/dedicated resources.
 *    -# It can contain Config/Reconfig/Release.
 *    -# The result of configuration is indicated in TfUiCtfCfgCfm.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfCfgReqInfo
(
Pst  *pst,
CtfCfgReqInfo *param,
Buffer *mBuf
)
{

#ifdef CTF_VER3
   S16 retVal;
#endif


   CMCHKUNPK(oduPackUInt8, &param->cfgType, mBuf);
      switch(param->cfgType) {
         case CTF_CONFIG:
            CMCHKUNPK(cmUnpkCtfCfgInfo, &param->u.cfg, mBuf);
            break;
         case CTF_RECONFIG:
            CMCHKUNPK(cmUnpkCtfReCfgInfo, &param->u.reCfg, mBuf);
            break;
         case CTF_DELETE:
            CMCHKUNPK(cmUnpkCtfReleaseInfo, &param->u.release, mBuf);
            break;
         default :
            return RFAILED;
      }
#ifdef CTF_VER3
    if((retVal = cmUnpkCtfVendorSpecParams(pst, &param->vendorParams, mBuf)) != ROK)
        return (retVal);
#endif
   return ROK;
}

         /* Starts - Fix for CR ccpu00123185 */
/***********************************************************
*
*     Func: cmPkCtfCellPwrcfgInfo
*
*
*     Desc:   Cell Re-configuration Information
 * @details This structure contains the information for reconfiguration of Reference Signal Power
 *          of a cell and its associated resources at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfCellPwrcfgInfo
(
CtfCellRecfgInfo *param,
Buffer *mBuf
)
{

   /* Added the pack for newly added reference signal Power*/
   CMCHKPK(oduUnpackUInt32, (uint32_t)param->ctfCellStatus, mBuf);
   CMCHKPK(SPkS16, param->pilotSigPwr, mBuf);
   CMCHKPK(SPkS16, param->priSigPwr, mBuf);
   CMCHKPK(SPkS16, param->secSigPwr, mBuf);
   
   CMCHKPK(SPkS16, param->pdschCfg.refSigPwr, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func: cmUnpkCtfCellPwrcfgInfo
*
*
*     Desc:   Cell Re-configuration Information
 * @details This structure contains the information for reconfiguration
 *          of a cell and its associated resources at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfCellPwrcfgInfo
(
CtfCellRecfgInfo *param,
Buffer *mBuf
)
{
   
   /* ctf_c_001.main_3: Added the unpack for newly added fileds inside the CFG structure*/
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);

   CMCHKUNPK(SUnpkS16, &param->pdschCfg.refSigPwr, mBuf);
   CMCHKUNPK(SUnpkS16, &param->secSigPwr, mBuf);
   CMCHKUNPK(SUnpkS16, &param->priSigPwr, mBuf);
   CMCHKUNPK(SUnpkS16, &param->pilotSigPwr, mBuf);
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&param->ctfCellStatus, mBuf);

   return ROK;
}

/***********************************************************
*
*     Func: cmPkCtfCellStop
*
*
*     Desc:   Cell Re-configuration Information
 * @details This structure contains the information for reconfiguration of Reference Signal Power
 *          of a cell and its associated resources at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfCellStop
(
CtfCellRecfgInfo *param,
Buffer *mBuf
)
{

   /* Added the pack for newly added reference signal Power*/
   CMCHKPK(oduUnpackUInt32, (uint32_t)param->ctfCellStatus, mBuf);
   CMCHKPK(SPkS16, param->pilotSigPwr, mBuf);
   CMCHKPK(SPkS16, param->priSigPwr, mBuf);
   CMCHKPK(SPkS16, param->secSigPwr, mBuf);
   
   CMCHKPK(SPkS16, param->pdschCfg.refSigPwr, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}

/***********************************************************
*
*     Func: cmUnpkCtfCellStop
*
*
*     Desc:   Cell Re-configuration Information
 * @details This structure contains the information for reconfiguration
 *          of a cell and its associated resources at PHY.
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmUnpkCtfCellStop
(
CtfCellRecfgInfo *param,
Buffer *mBuf
)
{
   
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);

   CMCHKUNPK(SUnpkS16, &param->pdschCfg.refSigPwr, mBuf);
   CMCHKUNPK(SUnpkS16, &param->secSigPwr, mBuf);
   CMCHKUNPK(SUnpkS16, &param->priSigPwr, mBuf);
   CMCHKUNPK(SUnpkS16, &param->pilotSigPwr, mBuf);
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&param->ctfCellStatus, mBuf);

   return ROK;
}

/* Periodic REM Scan for TPM */
/***********************************************************
*
*     Func: cmPkPerRemScanCellInfo
*
*
*     Desc: Periodic REM cell search response
*     @details This Message contains cell search results. 
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkPerRemScanCellInfo
(
CtfPeriodicRemCellInfo *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt16,param->pci, mBuf);
   CMCHKPK(oduUnpackUInt8, param->rsrp, mBuf);
   CMCHKPK(oduUnpackUInt8, param->dlBw, mBuf);
   CMCHKPK(oduUnpackUInt8, param->numTx, mBuf);
   return ROK;
}


/***********************************************************
*
*     Func: cmPkCtfPeriodicCellSearchRsp
*
*
*     Desc: Periodic REM cell search response
*     @details This Message contains Periodic REM scan results. 
*
*
*     Ret: S16
*
*     Notes:
*
*     File: 
*
**********************************************************/
S16 cmPkCtfPeriodicCellSearchRsp
(
CtfPeriodicRemCellSearchRsp *param,
Buffer *mBuf
)
{
   S16 idx;

   for(idx = param->numCells - 1; idx >=0; idx--)
   {
      CMCHKPK(cmPkPerRemScanCellInfo, &param->cellInfo[idx], mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->numCells, mBuf);
  
   return ROK;
}

/**
* @brief Configuration Request from App to CL for
 * cnm Init Sync responce
*
* @details
*
*     Function: cmPkCtfPeriodicRemCellSearchRsp
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   CtfCfgTransId  * transId
*  @param[in]   CtfCnmInitSyncRsp* ctfcnmInitSyncRsp
*  @return   S16
*      -# ROK
**/
S16 cmPkCtfPeriodicRemCellSearchRsp
(
Pst* pst,
SpId spId,
CtfCfgTransId *transId,
CtfPeriodicRemCellSearchRsp *CellCfgRsp
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF016, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)CellCfgRsp,
            sizeof(CtfPeriodicRemCellSearchRsp));
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LC) {
      if (cmPkCtfPeriodicCellSearchRsp(CellCfgRsp, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF017, (ErrVal)0, (Txt*)&"Packing failed");
#endif
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)CellCfgRsp,
               sizeof(CtfPeriodicRemCellSearchRsp));
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   else if (pst->selector == ODU_SELECTOR_LWLC) {

   if (oduPackPointer((PTR)CellCfgRsp, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF018, (ErrVal)0, "Packing failed");
#endif
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)CellCfgRsp,
               sizeof(CtfPeriodicRemCellSearchRsp));
         SPutMsg(mBuf);
         return RFAILED;
      }

   }
 if(cmPkCtfCfgTransId(transId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF019, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)CellCfgRsp,
            sizeof(CtfPeriodicRemCellSearchRsp));
      SPutMsg(mBuf);
      return RFAILED;
   }
 if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF020, (ErrVal)0, "Packing failed");
#endif
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)CellCfgRsp,
            sizeof(CtfPeriodicRemCellSearchRsp));
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (pst->selector != ODU_SELECTOR_LWLC) {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)CellCfgRsp,
            sizeof(CtfPeriodicRemCellSearchRsp));
   }
   pst->event = (Event)EVTCTFPREMCELLSRCHRSP;
   return (SPstTsk(pst,mBuf));
}

S16 cmUnpkCtfPerRemScanCellInfo
(
CtfPeriodicRemCellInfo *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt8, &param->numTx, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->dlBw, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->rsrp, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->pci, mBuf);
   return ROK;
}

S16 cmUnpkCtfPerRemCellSrch
(
CtfPeriodicRemCellSearchRsp *param,
Buffer *mBuf
)
{
   S16 idx;

   CMCHKUNPK(oduPackUInt8, &param->numCells, mBuf);
   for(idx = 0; idx < (param->numCells); idx++)
   {
      CMCHKUNPK(cmUnpkCtfPerRemScanCellInfo, &param->cellInfo[idx], mBuf);
   }

   return ROK;
}

/**
* @brief ENB STOP Indication from PHY to ENB-APP
*
* @details
*
*     Function: cmUnpkPerRemCellSrchRsp
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   CtfCfgTransId  transId
*  @return   S16
*      -# ROK
**/
S16 cmUnpkPerRemCellSrchRsp
(
CtfPerRemCellSrchRsp func,
Pst *pst,
Buffer *mBuf
)
{
   SuId suId = 0;
   CtfCfgTransId transId;
   CtfPeriodicRemCellSearchRsp *CellSrchRsp = NULLP;  

   if (SUnpkS16(&suId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF030, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (cmUnpkCtfCfgTransId(&transId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF031, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }
   if (pst->selector != ODU_SELECTOR_LWLC) {
      if ((SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&CellSrchRsp,
                  sizeof(CtfPeriodicRemCellSearchRsp))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECTF024, (ErrVal)0, (Txt*)&"Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   if (pst->selector == ODU_SELECTOR_LWLC) {
      if (oduUnpackPointer((PTR *)&CellSrchRsp, mBuf) != ROK) {
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
  else if (pst->selector == ODU_SELECTOR_LC)
      if (cmUnpkCtfPerRemCellSrch(CellSrchRsp, mBuf) != ROK) {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)CellSrchRsp,
            sizeof(CtfPeriodicRemCellSearchRsp));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      /* ctf_c_001.main_3 Compilation warning fix with g++ */
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         (Txt*)&__FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECTF025, (ErrVal)0, (Txt*)&"Packing failed");
#endif
      return RFAILED;
   }

   SPutMsg(mBuf);
   return ((*func)(pst, suId, &transId, CellSrchRsp));
}
/* Periodic REM Scan for TPM End */

/* ends - Fix for CR ccpu00123185 */
#endif

/**********************************************************************
         End of file
**********************************************************************/
