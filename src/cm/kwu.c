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
  
        Name:    KWU Interface
    
        Type:    C file
  
        Desc:    This file Contains the packing and unpacking functions for
                 KWU Interface
 
        File:    kwu.c
  
*********************************************************************21*/

/* header include files (.h) */
#include "common_def.h"
#ifdef TENB_SPLIT_ARCH
#include "ss_queue.h"
#include "ss_task.h"
#include "ss_msg.h"
#endif
#include "kwu.h"           /* KWU defines */

/* extern (.x) include files */
#ifdef TENB_SPLIT_ARCH
#include "ss_queue.x"
#include "ss_task.x"
#include "ss_msg.x"
#endif
#ifdef SS_RBUF
#include "ss_rbuf.h"
#include "ss_rbuf.x"
#endif /*SS_RBUF*/
#include "kwu.x"           /* KWU */

#ifdef LCKWU

/****************************************************************************
 *                      Packing Functions
 ***************************************************************************/
S16 cmPkKwuBndReq(Pst * pst,SuId suId,SpId spId)
{
   S16    ret1;
   Buffer *mBuf;

   mBuf = NULLP;

   if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      if(ret1 != ROK)
      {
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EKWU001, (ErrVal)0, "SGetMsg() failed");
      }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      return (ret1);
   }
   CMCHKPKLOG(SPkS16, spId, mBuf, EKWU002, pst);
   CMCHKPKLOG(SPkS16, suId, mBuf, EKWU003, pst);
   pst->event = (Event) KWU_EVT_BND_REQ;
   return (SPstTsk(pst,mBuf));
}


S16 cmPkKwuBndCfm(Pst * pst,SuId suId,uint8_t status)
{
   S16    ret1;
   Buffer *mBuf;


   mBuf = NULLP;

   if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      if(ret1 != ROK)
      {
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EKWU004, (ErrVal)0, "SGetMsg() failed");
      }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      return (ret1);
   }

   CMCHKPKLOG(oduUnpackUInt8, status, mBuf, EKWU005, pst);
   CMCHKPKLOG(SPkS16, suId, mBuf, EKWU006, pst);
   pst->event = (Event) KWU_EVT_BND_CFM;
   return (SPstTsk(pst,mBuf));
}


S16 cmPkKwuUbndReq(Pst * pst,SuId suId,Reason reason)
{
   S16 ret1;
   Buffer *mBuf;


   mBuf = NULLP;

   if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      if(ret1 != ROK)
      {
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EKWU007, (ErrVal)0, "SGetMsg() failed");
      }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      return (ret1);
   }
   CMCHKPKLOG(SPkS16, reason, mBuf, EKWU008, pst);
   CMCHKPKLOG(SPkS16, suId, mBuf, EKWU009, pst);

   pst->event = (Event) KWU_EVT_UBND_REQ;
   return (SPstTsk(pst,mBuf));
} /* cmPkKwuUbndReq */

uint8_t cmPkRlcDatReqInfo(RlcDatReqInfo *param,Buffer *mBuf)
{
#ifdef CCPU_OPT
   switch(param->lcType) {
      case CM_LTE_LCH_CCCH:
         CMCHKPK(cmPkLteRnti, param->tm.rnti, mBuf);
         break;
      case CM_LTE_LCH_BCCH:
      case  CM_LTE_LCH_PCCH:
         CMCHKPK(cmPkLteTimingInfo, &param->tm.tmg, mBuf);
#ifdef EMTC_ENABLE
         CMCHKPK(oduUnpackUInt8, param->emtcDiReason,mBuf);
         CMCHKPK(oduUnpackUInt8, param->pnb,mBuf);
#endif
         break;
      case CM_LTE_LCH_DTCH:
      case CM_LTE_LCH_DCCH:
         break;
      default :
         return RFAILED;
   }
   CMCHKPK(cmPkLteLcType, param->lcType, mBuf);

#endif
   CMCHKPK(oduUnpackUInt32, param->sduId, mBuf);
   CMCHKPK(cmPkLteRlcId, &param->rlcId, mBuf);
   return ROK;
} /* cmPkRlcDatReqInfo */


uint8_t cmPkRlcDatReq(Pst * pst,RlcDatReqInfo* datReq,Buffer * mBuf)
{
#ifdef LCKWU
#if (ERRCLASS & ERRCLS_ADD_RES)
#endif
#endif /* LCKWU */
   S16 ret1 = ROK;
#ifndef SS_RBUF
   RlcDatReqInfo* datReqInfo;
#endif
#ifndef SS_RBUF
   switch(pst->selector)
   {
     case ODU_SELECTOR_LWLC:
        {
           if(pst->srcEnt == ENTDUAPP)
           {
             /* When the Selector is LWLC, we need to allocate memory, copy
              * the contents and pass the pointer of the allocated memory. The
              * subsequent free would be done during the Unpack function of the
              * primitive. */
            if((ret1 = SGetStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&datReqInfo,
                        sizeof(RlcDatReqInfo),SS_SHARABLE_MEMORY)) != ROK)
            {
#if (ERRCLASS & ERRCLS_ADD_RES)
               if(ret1 != ROK)
               {
                  SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                        (ErrVal)EKWU028, (ErrVal)0, "SGetMsg() failed");
               }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
               return (ret1);
            }
            memcpy(datReqInfo,datReq,sizeof(RlcDatReqInfo));
            CMCHKPK(oduPackPointer,(PTR)datReqInfo, mBuf);
           }
           else
           {
            CMCHKPK(oduPackPointer,(PTR)datReq, mBuf);
           }
        }
        break;
     case ODU_SELECTOR_LC:
        {
#if (ERRCLASS & ERRCLS_ADD_RES)
           ret1 = cmPkRlcDatReqInfo( (datReq), mBuf);
           if(ret1 != ROK)
           {
              SPutMsg(mBuf);
              SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                    __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                    (ErrVal)EKWU010, (ErrVal)ret1, "Packing failure");
              return ( ret1 );
           }
#else
           cmPkRlcDatReqInfo( (datReq), mBuf);
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
           if(pst->srcEnt == ENTNH)
           {
              if (SPutStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)datReq,
                       sizeof(RlcDatReqInfo),SS_SHARABLE_MEMORY) != ROK)
              {
                 SPutMsg(mBuf);
                 return RFAILED;
              }
           }
        }
        break;
     default:
          SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ERRKWU, (ErrVal)ret1, "pst->selector is invalid\n");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
          return RFAILED;
   }

#ifdef L2_L3_SPLIT
    if(datReq->rlcId.rbType == CM_LTE_SRB)
    {
       pst->event = (Event) KWU_EVT_CPLANE_DAT_REQ;
    }
    else
    {
       pst->event = (Event) KWU_EVT_UPLANE_DAT_REQ;
    }
#else
    pst->event = (Event) KWU_EVT_DAT_REQ;
#endif
    return (SPstTsk(pst,mBuf));
#else
   if (pst->srcEnt == ENTPJ)
   {
      Void *elem = NULLP;
      KwuDatReqDetl *kwuDatReqDetl = NULLP;
      elem = SRngGetWIndx(SS_RNG_BUF_DLPDCP_TO_DLRLC);
      if (NULLP != elem)
      {
         kwuDatReqDetl = (KwuDatReqDetl *) elem;
         kwuDatReqDetl->spId = spId;
         kwuDatReqDetl->lcType = datReq->lcType;
         kwuDatReqDetl->sduId = datReq->sduId;
         kwuDatReqDetl->rlcId = datReq->rlcId;
         kwuDatReqDetl->mBuf = mBuf;
         SRngIncrWIndx(SS_RNG_BUF_DLPDCP_TO_DLRLC);
         SsRngInfoTbl[SS_RNG_BUF_DLPDCP_TO_DLRLC].pktRate++;
         ret1 = ROK;
      }
      else
      {
         SsRngInfoTbl[SS_RNG_BUF_DLPDCP_TO_DLRLC].pktDrop++;
         SPutMsg(mBuf);
         ret1 = RFAILED;
      }
   }
   else
   {
      switch(pst->selector)
      {
#ifdef LCKWU
         case ODU_SELECTOR_LC:
            {
               ret1 = cmPkRlcDatReqInfo( (datReq), mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
               if(ret1 != ROK)
               {
                  SPutMsg(mBuf);
                  SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                        (ErrVal)EKWU010, (ErrVal)ret1, "Packing failure");
                  return ( ret1 );
               }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
            }
            break;
         case ODU_SELECTOR_LWLC:
            {
               CMCHKPK(oduPackPointer,(PTR)datReq, mBuf);
            }
            break;
#endif /* LCKWU */
      }

      CMCHKPKLOG(SPkS16, spId, mBuf, EKWU011, pst);
      pst->event = (Event) KWU_EVT_DAT_REQ;
      ret1 = SPstTsk(pst,mBuf);
      if(pst->selector == ODU_SELECTOR_LC)
      {
         if (SPutStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)datReq,
                  sizeof(RlcDatReqInfo),SS_SHARABLE_MEMORY) != ROK)
         {
            SPutMsg(mBuf);
            return RFAILED;
         }
      }
   }
   return (ret1);
#endif
} /* cmPkRlcDatReq */


uint8_t cmPkKwuDatIndInfo(KwuDatIndInfo *param,Buffer *mBuf)
{

   CMCHKPK(oduUnpackUInt8, param->isOutOfSeq, mBuf);
#ifdef CCPU_OPT
   CMCHKPK(cmPkLteRnti, param->tCrnti, mBuf);
#endif
   CMCHKPK(cmPkLteRlcId, &param->rlcId, mBuf);
   return ROK;
}


uint8_t cmPkKwuDatInd(Pst * pst,KwuDatIndInfo* datInd,Buffer * mBuf)
{
#ifdef LCKWU
    S16 ret1 = ROK;
#endif /* LCKWU */
#ifdef TENB_SPLIT_ARCH
#ifdef SS_LOCKLESS_MEMORY
    SsMsgInfo   *mInfo; 
#endif
#endif
    KwuDatIndInfo *datIndInfo = NULLP;


#ifdef TENB_SPLIT_ARCH
    /* SSI_CHANGES: */
#ifdef SS_LOCKLESS_MEMORY
    mInfo = (SsMsgInfo *)mBuf->b_rptr;
    mInfo->region = pst->region;
#endif /* SS_LOCKLESS_MEMORY */
#endif

    switch(pst->selector)
    {
       case ODU_SELECTOR_LWLC:
          {
             /* When the Selector is LWLC, we need to allocate memory, copy
              * the contents and pass the pointer of the allocated memory. The
              * subsequent free would be done during the Unpack function of the
              * primitive. */
            if((ret1 = SGetStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&datIndInfo,
                        sizeof(KwuDatIndInfo),SS_SHARABLE_MEMORY)) != ROK)
            {
#if (ERRCLASS & ERRCLS_ADD_RES)
               if(ret1 != ROK)
               {
                  SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                        (ErrVal)EKWU028, (ErrVal)0, "SGetMsg() failed");
               }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
               return (ret1);
            }

            memcpy(datIndInfo, datInd,sizeof(KwuDatIndInfo));
            CMCHKPK(oduPackPointer,(PTR)datIndInfo, mBuf);
          }
          break;
       case ODU_SELECTOR_LC:
          {
#if (ERRCLASS & ERRCLS_ADD_RES)
             ret1 = cmPkKwuDatIndInfo( (datInd), mBuf);
             if(ret1 != ROK)
             {
                SPutMsg(mBuf);
                SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                      __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                      (ErrVal)EKWU012, (ErrVal)ret1, "Packing failure");
                return ( ret1 );
             }
#else
             cmPkKwuDatIndInfo( (datInd), mBuf);
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
          }
          break;
       default:
          SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ERRKWU, (ErrVal)ret1, "pst->selector is invalid\n");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
          return RFAILED;
    }

    pst->event = (Event) KWU_EVT_DAT_IND;

    return (SPstTsk(pst,mBuf));
} /* cmPkKwuDatInd */


S16 cmPkKwuDatCfmInfo(KwuDatCfmInfo *param,Buffer *mBuf)
{
   register uint32_t iter;

   for(iter = 0; iter < param->numSduIds; iter++)
   {
      CMCHKPK(oduUnpackUInt32, param->sduIds[iter], mBuf);
   }
   CMCHKPK(oduUnpackUInt32, param->numSduIds, mBuf);
   CMCHKPK(cmPkLteRlcId, &param->rlcId, mBuf);
   return ROK;
} /* cmPkKwuDatCfmInfo */


S16 cmPkKwuDatCfm(Pst * pst,SuId suId,KwuDatCfmInfo* datCfm)
{
#if (ERRCLASS & ERRCLS_ADD_RES)
   S16 ret1= ROK;
#endif 
   Buffer *mBuf;


   mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      return RFAILED;
   }

   switch(pst->selector)
   {
      case ODU_SELECTOR_LWLC:
         {
            CMCHKPK(oduPackPointer,(PTR)datCfm, mBuf);
         }
         break;
      case ODU_SELECTOR_LC:
         {
#if (ERRCLASS & ERRCLS_ADD_RES)
            ret1 = cmPkKwuDatCfmInfo( (datCfm), mBuf);
            if(ret1 != ROK)
            {
               SPutMsg(mBuf);
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)EKWU014, (ErrVal)ret1, "Packing failure");
               return ( ret1 );
            }
#else
            cmPkKwuDatCfmInfo( (datCfm), mBuf);
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
            if (SPutStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)datCfm,
                     sizeof(KwuDatCfmInfo),SS_SHARABLE_MEMORY) != ROK)
            {
               SPutMsg(mBuf);
               return RFAILED;
            }
         }
         break;
      default:
          SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ERRKWU, (ErrVal)ret1, "pst->selector is invalid\n");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
          return RFAILED;
   }

    CMCHKPKLOG(SPkS16, suId, mBuf, EKWU015, pst);
    pst->event = (Event) KWU_EVT_DAT_CFM;

    return (SPstTsk(pst,mBuf));
} /* cmPkKwuDatCfm */


S16 cmPkKwuDiscSduReq(Pst * pst,SpId spId,KwuDiscSduInfo* discSdu)
{
#ifdef LCKWU
   S16 ret1;
#endif /* LCKWU */
   Buffer *mBuf;
   KwuDiscSduInfo* discSduInfo = NULLP;

   mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      return RFAILED;
   }

   switch(pst->selector)
   {
      case ODU_SELECTOR_LWLC:
         {
            /* When the Selector is LWLC, we need to allocate memory, copy
             * the contents and pass the pointer of the allocated memory. The
             * subsequent free would be done during the Unpack function of the
             * primitive. */
           if((ret1 = SGetStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&discSduInfo,
                       sizeof(KwuDiscSduInfo),SS_SHARABLE_MEMORY)) != ROK)
           {
#if (ERRCLASS & ERRCLS_ADD_RES)
              if(ret1 != ROK)
              {
                 SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                       __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                        (ErrVal)EKWU028, (ErrVal)0, "SGetMsg() failed");
              }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
              return (ret1);
           }
            
           memcpy(discSduInfo, discSdu,sizeof(KwuDiscSduInfo));
           CMCHKPK(oduPackPointer,(PTR)discSduInfo, mBuf);
         }
         break;
      case ODU_SELECTOR_LC:
         {
#if (ERRCLASS & ERRCLS_ADD_RES)
            ret1 = cmPkKwuDiscSduInfo( (discSdu), mBuf);
            if(ret1 != ROK)
            {
               SPutMsg(mBuf);
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)EKWU016, (ErrVal)ret1, "Packing failure");
               return ( ret1 );
            }
#else
            cmPkKwuDiscSduInfo( (discSdu), mBuf);
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
             break;
          }
    }

    CMCHKPKLOG(SPkS16, spId, mBuf, EKWU017, pst);
    pst->event = (Event) KWU_EVT_DISC_SDU_REQ;

    return (SPstTsk(pst,mBuf));
} /* cmPkKwuDiscSduReq */


S16 cmPkKwuStaIndInfo(KwuStaIndInfo *param,Buffer *mBuf)
{
   S16 i;


   for (i = (param->numSdu - 1); i >= 0; i--)
   {
      CMCHKPK(oduUnpackUInt32, param->sduId[(uint16_t)i], mBuf);
   }
   CMCHKPK(oduUnpackUInt32, param->numSdu, mBuf);
   CMCHKPK(cmPkLteRlcId, &param->rlcId, mBuf);
   return ROK;
}

S16 cmPkKwuFlowCntrlIndInfo(KwuFlowCntrlIndInfo *param,Buffer *mBuf)
{

   CMCHKPK(oduUnpackUInt32, param->pktAdmitCnt, mBuf);   
   CMCHKPK(cmPkLteRlcId, &param->rlcId, mBuf);

   return ROK;
} /* cmPkKwuFlowCntrlIndInfo */

S16 cmUnpkKwuFlowCntrlIndInfo(KwuFlowCntrlIndInfo *param,Buffer *mBuf)
{
   
   CMCHKUNPK(cmUnpkLteRlcId, &param->rlcId, mBuf);
   CMCHKUNPK(oduPackUInt32, &param->pktAdmitCnt, mBuf);
  
   return ROK;
} /* cmUnpkKwuFlowCntrlIndInfo */


S16 cmPkKwuStaInd(Pst * pst,SuId suId,KwuStaIndInfo* staInd)
{
#ifdef LCKWU
#if (ERRCLASS & ERRCLS_ADD_RES)
   S16 ret1 = ROK;
#endif
#endif /* LCKWU */
   Buffer *mBuf;


   mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      return RFAILED;
   }

    switch(pst->selector)
    {
       case ODU_SELECTOR_LWLC:
          {
             CMCHKPK(oduPackPointer,(PTR) staInd, mBuf);
          }
          break;
       case ODU_SELECTOR_LC:
          {
#if (ERRCLASS & ERRCLS_ADD_RES)
             ret1 = cmPkKwuStaIndInfo( (staInd), mBuf);
             if(ret1 != ROK)
             {
                SPutMsg(mBuf);
                SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                      __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                      (ErrVal)EKWU018, (ErrVal)ret1, "Packing failure");
                return ( ret1 );
             }
#else
             cmPkKwuStaIndInfo( (staInd), mBuf);
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
             if (SPutStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)staInd,
                      sizeof(KwuStaIndInfo),SS_SHARABLE_MEMORY) != ROK)
             {
                SPutMsg(mBuf);
                return RFAILED;
             }
          }
          break;
       default:
          SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ERRKWU, (ErrVal)ret1, "pst->selector is invalid\n");
#endif
          return RFAILED;
    }

    CMCHKPKLOG(SPkS16, suId, mBuf, EKWU019, pst);
    pst->event = (Event) KWU_EVT_STA_IND;

    return (SPstTsk(pst,mBuf));
} /* cmPkKwuStaInd */


S16 cmPkKwuReEstCmpInd(Pst * pst,SuId suId,CmLteRlcId rlcId)
{
#ifdef LCKWU
#if (ERRCLASS & ERRCLS_ADD_RES)
   S16 ret1;
#endif
#endif /* LCKWU */
   Buffer *mBuf;


   mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      return RFAILED;
   }

    switch(pst->selector)
    {
#ifdef LCKWU
       case ODU_SELECTOR_LC:
          {
#if (ERRCLASS & ERRCLS_ADD_RES)
             ret1 = cmPkLteRlcId( &rlcId, mBuf);
             if(ret1 != ROK)
             {
                SPutMsg(mBuf);
                SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                      __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                      (ErrVal)EKWU018, (ErrVal)ret1, "Packing failure");
                return ( ret1 );
             }
#else
             cmPkLteRlcId( &rlcId, mBuf);
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
             break;
          }
#endif /* LCKWU */
    }

    CMCHKPKLOG(SPkS16, suId, mBuf, EKWU019, pst);
    pst->event = (Event) KWU_EVT_REEST_CMP_IND;

    return (SPstTsk(pst,mBuf));
} /* cmPkKwuReEstCmpInd */

/* kwu_c_001.main_3 added support for L2 Measurement */
S16 cmPkKwuDiscSduCfm(Pst *pst,SpId spId,KwuDiscSduInfo *discCfmSdu)
{
#if (ERRCLASS & ERRCLS_ADD_RES)
   S16 ret1 = ROK;
#endif
   Buffer *mBuf;


   mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      return RFAILED;
   }

   switch(pst->selector)
   {
      case ODU_SELECTOR_LWLC:
         {
            CMCHKPK(oduPackPointer,(PTR)discCfmSdu, mBuf);
         }
         break;
      case ODU_SELECTOR_LC:
         {
#if (ERRCLASS & ERRCLS_ADD_RES)
            ret1 = cmPkKwuDiscSduInfo((discCfmSdu), mBuf);
            if(ret1 != ROK)
            {
               SPutMsg(mBuf);
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)ERRKWU, (ErrVal)ret1, "Packing failure");
               return ( ret1 );
            }
#else
            cmPkKwuDiscSduInfo((discCfmSdu), mBuf);
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
            if (SPutStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)discCfmSdu,
                     sizeof(KwuDiscSduInfo),SS_SHARABLE_MEMORY) != ROK)
            {
               SPutMsg(mBuf);
               return RFAILED;
            }
         }
         break;
      default:
         SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ERRKWU, (ErrVal)ret1, "pst->selector is invalid\n");
#endif
          return RFAILED;
   }

   CMCHKPKLOG(SPkS16, spId, mBuf, ERRKWU, pst);
   pst->event = (Event) KWU_EVT_DISC_SDU_CFM;

   return (SPstTsk(pst,mBuf));
} /* cmPkKwuDiscSduCfm */

S16 cmPkKwuFlowCntrlInd(Pst *pst,SuId suId,KwuFlowCntrlIndInfo *flowCntrlIndInfo)
{
   Buffer *mBuf;


   mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
      SPutStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, 
               pst->pool, 
               (Data *)flowCntrlIndInfo,
               sizeof(KwuFlowCntrlIndInfo),0);
      return RFAILED;
   }
#ifdef LCKWU
   switch(pst->selector)
   {
      case ODU_SELECTOR_LC:
      {
         cmPkKwuFlowCntrlIndInfo((flowCntrlIndInfo), mBuf);

         if (SPutStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, 
                      pst->pool, 
                      (Data *)flowCntrlIndInfo,
                      sizeof(KwuFlowCntrlIndInfo),0) != ROK)
         {
            SPutMsg(mBuf);
            return RFAILED;
         }
      }
      break;
      
      case ODU_SELECTOR_LWLC:
      {
         CMCHKPK(oduPackPointer,(PTR) flowCntrlIndInfo, mBuf);
      }
      break;
   }
#endif

   CMCHKPKLOG(SPkS16, suId, mBuf, ERRKWU, pst);
   pst->event = (Event) KWU_EVT_FLOW_CNTRL_IND;

   return (SPstTsk(pst,mBuf));
} /* cmPkKwuFlowCntrlInd */

#ifdef LTE_L2_MEAS
S16 cmPkKwuDatAckInfo(KwuDatAckInfo *param,Buffer *mBuf)
{

   CMCHKPK(cmPkLteRlcId, &param->rlcId, mBuf);
   CMCHKPK(oduUnpackUInt32, param->sduId, mBuf);
   return ROK;
}

S16 cmPkKwuDatAckInd(Pst * pst,SuId suId,KwuDatAckInfo* datInd)
{
    S16 ret1;

   Buffer *mBuf;


   mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      return RFAILED;
   }

   ret1 = cmPkKwuDatAckInfo( (datInd), mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
   if(ret1 != ROK)
   {
      SPutMsg(mBuf);
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERRKWU, (ErrVal)ret1, "Packing failure");
      return ( ret1 );
   }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
   if (SPutStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)datInd,
            sizeof(KwuDatAckInfo),SS_SHARABLE_MEMORY) != ROK)
   {
      SPutMsg(mBuf);
      return RFAILED;
   }

   CMCHKPKLOG(SPkS16, suId, mBuf, ERRKWU, pst);
   pst->event = (Event) KWU_EVT_DAT_ACK_IND;

   return (SPstTsk(pst,mBuf));
} /* cmPkKwuDatAckInd */
#endif /* LTE_L2_MEAS */


S16 cmUnpkKwuBndReq(KwuBndReq func,Pst *pst,Buffer *mBuf)
{
   SuId suId = 0;
   SpId spId = 0;

   CMCHKUNPKLOG(SUnpkS16, &suId, mBuf, EKWU020, pst);
   CMCHKUNPKLOG(SUnpkS16, &spId, mBuf, EKWU021, pst);
   SPutMsg(mBuf);

   return ((*func)(pst, suId, spId));
}


S16 cmUnpkKwuBndCfm(KwuBndCfm func,Pst *pst,Buffer *mBuf)
{
   SuId suId = 0;
   uint8_t status = 0;


   CMCHKUNPKLOG(SUnpkS16, &suId, mBuf, EKWU022, pst);
   CMCHKUNPKLOG(oduPackUInt8, &status, mBuf, EKWU023, pst);
   SPutMsg(mBuf);

   return ((*func)(pst, suId, status));
}


S16 cmUnpkKwuUbndReq(KwuUbndReq func,Pst *pst,Buffer *mBuf)
{
   SpId spId = 0;
   Reason reason = 0;
   

   CMCHKUNPKLOG(SUnpkS16, &spId, mBuf, EKWU024, pst);
   CMCHKUNPKLOG(SUnpkS16, &reason, mBuf, EKWU025, pst);
   SPutMsg(mBuf);

   return ((*func)(pst, spId, reason));
} /* cmUnpkKwuUbndReq */


S16 cmUnpkRlcDatReqInfo(RlcDatReqInfo *param,Buffer *mBuf)
{


   CMCHKUNPK(cmUnpkLteRlcId, &param->rlcId, mBuf);
   CMCHKUNPK(oduPackUInt32, &param->sduId, mBuf);

#ifdef CCPU_OPT
   CMCHKUNPK(cmUnpkLteLcType, &param->lcType, mBuf);
   switch(param->lcType) {
      case CM_LTE_LCH_BCCH:
      case  CM_LTE_LCH_PCCH:
#ifdef EMTC_ENABLE
         CMCHKUNPK(oduPackUInt8,&param->pnb , mBuf);
         CMCHKUNPK(oduPackUInt8,&param->emtcDiReason , mBuf);
#endif         
         CMCHKUNPK(cmUnpkLteTimingInfo, &param->tm.tmg, mBuf);

         break;
      case CM_LTE_LCH_CCCH:
         CMCHKUNPK(cmUnpkLteRnti, &param->tm.rnti, mBuf);
         break;
      case CM_LTE_LCH_DTCH:
      case CM_LTE_LCH_DCCH:
         break;
      default :
         return RFAILED;
   }
#endif
   return ROK;
}


S16 cmUnpkKwuDatReq(KwuDatReq func,Pst *pst,Buffer *mBuf)
{
#if(ERRCLASS & ERRCLS_DEBUG)
   S16 ret1 = ROK; 
#endif
   S16 retVal;
   RlcDatReqInfo *datReq = NULLP;
   RlcDatReqInfo datReqTmp;


   switch(pst->selector)
   {
      case ODU_SELECTOR_LWLC:
         {
            CMCHKUNPK(oduUnpackPointer,(PTR *) &datReq, mBuf);
         }
         break;
      case ODU_SELECTOR_LC:
         {
            /* Allocate the memory statically  as there is no free 
             * in RLC */
            datReq = &datReqTmp;
            memset(datReq, 0, sizeof(RlcDatReqInfo));
#if(ERRCLASS & ERRCLS_DEBUG)
            ret1 = cmUnpkRlcDatReqInfo( (datReq), mBuf);
            if(ret1 != ROK)
            {
               SPutMsg(mBuf);
               SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
                     (ErrVal)EKWU027, (ErrVal)ret1, "Unpacking failure");
               return ( ret1 );
            }
#else
            cmUnpkRlcDatReqInfo( (datReq), mBuf);
#endif /* ERRCLASS & ERRCLS_DEBUG */
         }
         break;
      default:
         SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ERRKWU, (ErrVal)ret1, "pst->selector is invalid\n");
#endif
         return RFAILED;
   }

   retVal = (*func)(pst, datReq, mBuf);
   /* If LWLC is configured, we need to
    * free the memory here. */
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      retVal = SPutStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)datReq,
            sizeof(RlcDatReqInfo),SS_SHARABLE_MEMORY);
   }
   return (retVal);
} /* cmUnpkKwuDatReq */


uint8_t cmUnpkKwuDatIndInfo(KwuDatIndInfo *param,Buffer *mBuf)
{

   CMCHKUNPK(cmUnpkLteRlcId, &param->rlcId, mBuf);

#ifdef CCPU_OPT
   CMCHKUNPK(cmUnpkLteRnti, &param->tCrnti, mBuf);
#endif
   CMCHKUNPK(oduPackUInt8, &param->isOutOfSeq, mBuf);
   return ROK;
}


uint8_t cmUnpkKwuDatInd(KwuDatInd func,Pst *pst,Buffer *mBuf)
{
    S16 ret1 = ROK, retVal;
    KwuDatIndInfo *datInd = NULLP;
    KwuDatIndInfo datIndTmp;
    

    switch(pst->selector)
    {
      case ODU_SELECTOR_LWLC:
         {
            CMCHKUNPK(oduUnpackPointer,(PTR *) &datInd, mBuf);
         }
         break;
      case ODU_SELECTOR_LC:
         {
            /*SGetStaticBuffer used as RRC has an equivalent free but PDCP
             * doesn't free any memory */
            if(pst->dstEnt != ENTPJ)
            {
               if((ret1 = SGetStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&datInd,
                           sizeof(KwuDatIndInfo),SS_SHARABLE_MEMORY)) != ROK)
               {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EKWU028, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
                  return (ret1);
               }
            }
            else
            {
               datInd = &datIndTmp;
            }

            ret1 = cmUnpkKwuDatIndInfo( (datInd), mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
          if(ret1 != ROK)
          {
             SPutMsg(mBuf);
             SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
                   __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
                  (ErrVal)EKWU029, (ErrVal)ret1, "Unpacking failure");
             return ( ret1 );
          }
#endif /* ERRCLASS & ERRCLS_DEBUG */
         }
         break;
      default:
         SPutMsg(mBuf);
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ERRKWU, (ErrVal)ret1, "pst->selector is invalid\n");
         return RFAILED;
    }
    retVal = (*func)(pst, datInd, mBuf);
    /* If LWLC is configured and the destination entity is PDCP, we need to
     * free the memory here. */
    if((pst->selector == ODU_SELECTOR_LWLC) && (pst->dstEnt == ENTPJ))
    {
       retVal = SPutStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)datInd,
                sizeof(KwuDatIndInfo),SS_SHARABLE_MEMORY);
    }
    return (retVal);
} /* cmUnpkKwuDatInd */


S16 cmUnpkKwuDatCfmInfo(KwuDatCfmInfo *param,Buffer *mBuf)
{
   register S32 iter;

   CMCHKUNPK(cmUnpkLteRlcId, &param->rlcId, mBuf);
   CMCHKUNPK(oduPackUInt32, &param->numSduIds, mBuf);

#ifdef L2_L3_SPLIT /*Work Around */
   if (param->numSduIds >= KWU_MAX_DAT_CFM)
      param->numSduIds = KWU_MAX_DAT_CFM;
#endif
   for(iter = param->numSduIds -1; iter >= 0 ; iter--)
   {
      CMCHKUNPK(oduPackUInt32, &param->sduIds[iter], mBuf);
   }
   return ROK;
}

S16 cmUnpkKwuDatCfm(KwuDatCfm func,Pst *pst,Buffer *mBuf)
{
   S16 ret1 = ROK;
   SuId          suId = 0;
   KwuDatCfmInfo *datCfm = NULLP;


   CMCHKUNPK(SUnpkS16, &(suId), mBuf);

   switch(pst->selector)
   {
      case ODU_SELECTOR_LWLC:
         {
            CMCHKUNPK(oduUnpackPointer,(PTR *) &datCfm, mBuf);
         }
         break;
      case ODU_SELECTOR_LC:
         {
            if((ret1 = SGetStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&datCfm,\
                        sizeof(KwuDatCfmInfo),SS_SHARABLE_MEMORY)) != ROK)
            {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EKWU030, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
               return (ret1);
            }

            memset(datCfm, 0, sizeof(KwuDatCfmInfo));
            ret1 = cmUnpkKwuDatCfmInfo( (datCfm), mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
          if(ret1 != ROK)
          {
             SPutMsg(mBuf);
             SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
                   __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
                  (ErrVal)EKWU031, (ErrVal)ret1, "Unpacking failure");
             return ( ret1 );
          }
#endif /* ERRCLASS & ERRCLS_DEBUG */
         }
         break;
      default:
#if (ERRCLASS & ERRCLS_ADD_RES)
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ERRKWU, (ErrVal)ret1, "pst->selector is invalid\n");
#endif
          SPutMsg(mBuf);
          return RFAILED;
   }

   SPutMsg(mBuf);

   return ((*func)(pst, suId, datCfm));
} /* cmUnpkKwuDatCfm */


S16 cmUnpkKwuDiscSduReq(KwuDiscSduReq func,Pst *pst,Buffer *mBuf)
{
   S16 ret1 = ROK;
   SpId          spId = 0;
   KwuDiscSduInfo *discSdu = NULLP;


   CMCHKUNPK(SUnpkS16, &(spId), mBuf);
   switch(pst->selector)
   {
      case ODU_SELECTOR_LWLC:
         {
            CMCHKUNPK(oduUnpackPointer,(PTR *) &discSdu, mBuf);
         }
         break;
      case ODU_SELECTOR_LC:
         {
            if((ret1 = SGetStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&discSdu,\
                        sizeof(KwuDiscSduInfo),SS_SHARABLE_MEMORY)) != ROK)
            {
#if (ERRCLASS & ERRCLS_ADD_RES)
               /*MBUF_FIXX*/
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)EKWU032, (ErrVal)0, "SGetMsg() failed");

#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
               return (ret1);
            }

            memset(discSdu, 0, sizeof(KwuDiscSduInfo));
            ret1 = cmUnpkKwuDiscSduInfo( (discSdu), mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
            if(ret1 != ROK)
            {
               SPutMsg(mBuf);
               SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
                     (ErrVal)EKWU033, (ErrVal)ret1, "Unpacking failure");
               return ( ret1 );
            }
#endif /* ERRCLASS & ERRCLS_DEBUG */
            break;
         }
      default:
#if (ERRCLASS & ERRCLS_ADD_RES)
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ERRKWU, (ErrVal)ret1, "pst->selector is invalid\n");
#endif
          SPutMsg(mBuf);
          return RFAILED;
   }
   SPutMsg(mBuf);

   return ((*func)(pst, spId, discSdu));
} /* cmUnpkKwuDiscSduReq */


S16 cmUnpkKwuStaIndInfo(KwuStaIndInfo *param,Buffer *mBuf)
{
   uint32_t i;

   CMCHKUNPK(cmUnpkLteRlcId, &param->rlcId, mBuf);
   CMCHKUNPK(oduPackUInt32, &param->numSdu, mBuf);
   for (i = 0; i < param->numSdu; i++)
   {
      CMCHKUNPK(oduPackUInt32, &param->sduId[i], mBuf);
   }

   return ROK;
} /* cmUnpkKwuStaIndInfo */


S16 cmUnpkKwuStaInd(KwuStaInd func,Pst *pst,Buffer *mBuf)
{
   S16 ret1 = ROK;
   SuId          suId = 0;
   KwuStaIndInfo *staInd = NULLP;


   CMCHKUNPK(SUnpkS16, &(suId), mBuf);
   
   switch(pst->selector)
   {
      case ODU_SELECTOR_LWLC:
         {
            CMCHKUNPK(oduUnpackPointer,(PTR *)&staInd, mBuf);
         }
         break;
      case ODU_SELECTOR_LC:
         {
            if((ret1 = SGetStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&staInd,\
                        sizeof(KwuStaIndInfo),SS_SHARABLE_MEMORY)) != ROK)
            {
#if (ERRCLASS & ERRCLS_ADD_RES)
               if(ret1 != ROK)
               {
                  SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                        (ErrVal)EKWU034, (ErrVal)0, "SGetMsg() failed");
               }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
               return (ret1);
            }

            memset(staInd, 0, sizeof(KwuStaIndInfo));

            ret1 = cmUnpkKwuStaIndInfo( (staInd), mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
            if(ret1 != ROK)
            {
               SPutMsg(mBuf);
               SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
                     (ErrVal)EKWU035, (ErrVal)ret1, "Unpacking failure");
               return ( ret1 );
            }
#endif /* ERRCLASS & ERRCLS_DEBUG */
         }
         break;
      default:
          SPutMsg(mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ERRKWU, (ErrVal)ret1, "pst->selector is invalid\n");
#endif
          return RFAILED;
   }
   SPutMsg(mBuf);

   return ((*func)(pst, suId, staInd));
} /* cmUnpkKwuStaInd */


S16 cmUnpkKwuReEstCmpInd(KwuReEstCmpInd func,Pst *pst,Buffer *mBuf)
{
#if(ERRCLASS & ERRCLS_DEBUG)
   S16 ret1;
#endif
   SuId          suId = 0;
   CmLteRlcId    rlcId;

   memset(&rlcId, 0, sizeof(CmLteRlcId));
   CMCHKUNPK(SUnpkS16, &(suId), mBuf);
   switch(pst->selector)
   {
#ifdef LCKWU
      case ODU_SELECTOR_LC:
         {
#if(ERRCLASS & ERRCLS_DEBUG)
            ret1 = cmUnpkLteRlcId( &rlcId, mBuf);
            if(ret1 != ROK)
            {
               SPutMsg(mBuf);
               SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
                     (ErrVal)EKWU035, (ErrVal)ret1, "Unpacking failure");
               return ( ret1 );
            }
#else
            cmUnpkLteRlcId( &rlcId, mBuf);
#endif /* ERRCLASS & ERRCLS_DEBUG */
            break;
         }
#endif /* LCKWU */
   }
   SPutMsg(mBuf);

   return ((*func)(pst, suId, rlcId));
} /* cmUnpkKwuReEstCmpInd */

/* kwu_c_001.main_3 added support for L2 Measurement */
S16 cmUnpkKwuDiscSduCfm(KwuDiscSduCfm  func,Pst *pst,Buffer *mBuf)
{
   S16 ret1 = ROK;
   SpId  spId = 0;
   KwuDiscSduInfo *discSdu = NULLP;


   CMCHKUNPK(SUnpkS16, &(spId), mBuf);

   switch(pst->selector)
   {
      case ODU_SELECTOR_LWLC:
         {
            CMCHKUNPK(oduUnpackPointer,(PTR *) &discSdu, mBuf);
         }
         break;
      case ODU_SELECTOR_LC:
         {
            if((ret1 = SGetStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&discSdu,\
                        sizeof(KwuDiscSduInfo),SS_SHARABLE_MEMORY)) != ROK)
            {
#if (ERRCLASS & ERRCLS_ADD_RES)
      if(ret1 != ROK)
      {
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ERRKWU, (ErrVal)0, "SGetMsg() failed");
      }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
               return (ret1);
            }
            memset(discSdu, 0, sizeof(KwuDiscSduInfo));

   ret1 = cmUnpkKwuDiscSduInfo( (discSdu), mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
            if(ret1 != ROK)
            {
               SPutMsg(mBuf);
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)ERRKWU, (ErrVal)0, "SGetMsg() failed");
            }
#endif /* ERRCLASS & ERRCLS_DEBUG */
         }
         break;
      default:
          SPutMsg(mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ERRKWU, (ErrVal)ret1, "pst->selector is invalid\n");
#endif
          return RFAILED;
   }
   SPutMsg(mBuf);

   return ((*func)(pst, spId, discSdu));
} /* cmUnpkKwuDiscSduCfm */
S16 cmUnpkKwuFlowCntrlInd(KwuFlowCntrlInd func,Pst *pst,Buffer *mBuf)
{
   SuId   suId;
   KwuFlowCntrlIndInfo *flowCntrlInfo = NULLP;
   
   CMCHKUNPK(SUnpkS16, &(suId), mBuf);
   switch(pst->selector)
   {
#ifdef LCKWU
      case ODU_SELECTOR_LC:
      {
         if(SGetStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, 
                     pst->pool, 
                     (Data **)&flowCntrlInfo,
                     sizeof(KwuFlowCntrlIndInfo),0) != ROK)
         {
            SPutMsg(mBuf);
            return RFAILED;
         }

         cmUnpkKwuFlowCntrlIndInfo(flowCntrlInfo, mBuf);
      }
      break;

      case ODU_SELECTOR_LWLC:
      {
         CMCHKUNPK(oduUnpackPointer,(PTR *) &flowCntrlInfo, mBuf);
      }
      break;
#endif
   }

   SPutMsg(mBuf);

   return ((*func)(pst, suId, flowCntrlInfo));
} /* cmUnpkKwuFlowCntrlInd */
#ifdef LTE_L2_MEAS

S16 cmUnpkKwuDatAckInfo(KwuDatAckInfo *param,Buffer *mBuf)
{

   CMCHKUNPK(oduPackUInt32, &param->sduId, mBuf);
   CMCHKUNPK(cmUnpkLteRlcId, &param->rlcId, mBuf);

   return ROK;
}


S16 cmUnpkKwuDatAckInd(KwuDatAckInd func,Pst *pst,Buffer *mBuf)
{
    S16 ret1;
    SuId          suId = 0;
    KwuDatAckInfo *datInd = NULLP;
    

    if((ret1 = SGetStaticBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&datInd,\
                sizeof(KwuDatAckInfo),SS_SHARABLE_MEMORY)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                (ErrVal)ERRKWU, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       return (ret1);
    }

    memset(datInd, 0, sizeof(KwuDatAckInfo));

    CMCHKUNPK(SUnpkS16, &(suId), mBuf);
    ret1 = cmUnpkKwuDatAckInfo( (datInd), mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
             (ErrVal)ERRKWU, (ErrVal)ret1, "Unpacking failure");
       return ( ret1 );
    }
#endif /* ERRCLASS & ERRCLS_DEBUG */

    return ((*func)(pst, suId, datInd));
} /* cmUnpkKwuDatAckInd */
#endif /* LTE_L2_MEAS */
#endif /* LCKWU */

/**********************************************************************
         End of file
 **********************************************************************/
