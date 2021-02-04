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
  
     Desc:     C source code for Entry point fucntions
  
     File:     rg_mux.c 
  
**********************************************************************/

/** @file rg_mux.c
@brief MAC Multiplexing API.
*/


/* header include files -- defines (.h) */
#include "common_def.h"
#include "rg_env.h"        /* MAC Environment Defines */
#include "tfu.h"           /* TFU Interface defines */
#include "crg.h"           /* CRG Interface defines */
#include "rg_sch_inf.h"           /* RGR Interface defines */
#include "rgu.h"           /* RGU Interface defines */
#include "lrg.h"           /* LRG Interface defines */

#include "rg_err.h"        /* MAC error defines */
#include "rg.h"            /* MAC defines */

/* header/extern include files (.x) */

#include "rgu.x"           /* RGU Interface includes */
#include "tfu.x"           /* CRG Interface includes */
#include "crg.x"           /* CRG Interface includes */
#include "rg_sch_inf.x"    /* SCH Interface includes */
#include "rg_prg.x"        /* PRG Interface includes */
#include "rgu.x"           /* RGU Interface includes */
#include "lrg.x"           /* LRG Interface includes */

#include "du_app_mac_inf.h"
#include "rg.x"            /* MAC includes */
#include "ss_queue.h"
#include "ss_queue.x"
#include "ss_task.x"
#include "ss_msg.x"            /* MAC includes */
/* local defines */
#ifndef T2K_MEM_LEAK_DBG
 S16 ssGetDBufOfSize(Region region,Size size,Buffer **dBuf);
#else
char* file = __FILE__;
uint32_t line = __LINE__;
#endif

/* local typedefs */

/* global variables */
uint32_t rgDlrate_rgu;

/* local externs */

static Void rgMUXGet20bitRarGrnt ARGS((uint8_t ulBw,
                                        RgInfRarUlGrnt *msg3Grnt,
                                        uint8_t *grnt));
uint16_t rgMUXCalcRiv ARGS((uint8_t bw,
                                uint8_t rbStart,
                                uint8_t numRb));
 
#ifndef MS_MBUF_CORRUPTION
#define MS_BUF_ADD_ALLOC_CALLER()
#endif
/* forward references */

#define RG_PACK_SHDR_FIXD_SZ(_subHdr, _lcId, _mBuf, _ret) {\
   _subHdr.shLen = RG_FIXDSZ_CE_SHDR_LEN;\
   _subHdr.shData[0] = (0x3F & _lcId);\
   MS_BUF_ADD_ALLOC_CALLER(); \
   _ret = SAddPstMsgMult(&_subHdr.shData[0], _subHdr.shLen, _mBuf);\
}

#define RG_PACK_CE(_ce, _len, _ceBuf, _ret) {\
   MS_BUF_ADD_ALLOC_CALLER(); \
   _ret = SAddPstMsgMult((uint8_t *)(&(_ce)), _len, _ceBuf);\
}

#define RG_MUX_CALC_LEN(_len,_lenBytes,_elmTotLen) {\
   uint8_t _hdrLen;\
   _lenBytes    = (_len <= 255) ? 1 : 2;\
   _hdrLen      = _lenBytes + RG_SDU_SHDR_LEN;\
   _elmTotLen   = _hdrLen + _len;\
}

#define RG_PACK_VAR_SZ_CE_SDU_SHDR(_subHdr, _lcId, _len,_mBuf, _ret) {\
   _ret = ROK;\
   if(_len <= 255)\
   {\
      _subHdr.shData[0] = (0x3F & _lcId);\
      _subHdr.shLen = 2;\
      _subHdr.shData[1] = (0xFF & _len);\
      _subHdr.shData[2] = 0;\
   }\
   else\
   {\
   _subHdr.shData[0] = (0x7F & ((0x40) | _lcId));\
      _subHdr.shLen = 3;\
      _subHdr.shData[1] = (0xFF & (_len >> 8));\
      _subHdr.shData[2] = (0xFF & _len);\
   }\
   MS_BUF_ADD_ALLOC_CALLER(); \
   _ret = SAddPstMsgMult(&_subHdr.shData[0], _subHdr.shLen, _mBuf);\
}

#define RG_PACK_PAD_SHDR(_mBuf, _ret) {\
      _ret = SAddPreMsg(0x3F, _mBuf);\
}

#define RG_PACK_RAR_SHDR(_byte, _mBuf, _ret) {\
   _ret = SAddPstMsg(_byte, _mBuf);\
}


/**
 * @brief Function to add ces along with subhdrs.
 * This function packs first CE sub-hdr and then CE in ceBuf pointer 
 *
 * @details
 *
 *     Function : rgMUXAddCes
 *     
 *  @param[in]  Inst        inst
 *  @param[in] RgBldPduInfo   *pdu
 *  @param[in] Buffer        *ceBuf 
 *  @param[in] RgErrInfo     *err 
 *  @return    S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgMUXAddCes
(
Inst           inst,
RgBldPduInfo   *pdu,
Buffer         *ceBuf,
RgErrInfo      *err
)
{
   S16            ret;
   RgMUXSubHdr    subHdr;

   if (NULLP != pdu->contResId)
   {
      if(pdu->schdTbSz >= RG_CRES_ELM_LEN)
      {
         RG_PACK_SHDR_FIXD_SZ(subHdr, RG_CRES_LCID_IDX, ceBuf, ret);

         if(ret != ROK)
         {
            err->errCause = RGERR_MUX_BLD_CEHDR_FAIL;
            DU_LOG("\nERROR  -->  MAC : Muxing of Contention Resolution CE sub-header is failed");
            return RFAILED;
         }

         RG_PACK_CE(pdu->contResId->resId[0], RG_CRES_LEN, ceBuf, ret);

         if(ret != ROK)
         {
            err->errCause = RGERR_MUX_BLD_CE_FAIL;
            DU_LOG("\nERROR  -->  MAC : Muxing of Contention Resolution CE is failed");
            return RFAILED;
         }
         pdu->schdTbSz -= RG_CRES_ELM_LEN;
      }
   }
   if (TRUE == pdu->ta.pres)
   {
      if(pdu->schdTbSz >= RG_TA_ELM_LEN)
      {
         uint8_t taVal; /* Moving from outer scope to available scope */
         RG_PACK_SHDR_FIXD_SZ(subHdr, RG_TA_LCID_IDX, ceBuf, ret);

         if(ret != ROK)
         {
            err->errCause = RGERR_MUX_BLD_CEHDR_FAIL;
            DU_LOG("\nERROR  -->  MAC : Muxing of TA CE sub-hdr is failed");
            return RFAILED;
         }

         taVal = pdu->ta.val;
         RG_PACK_CE(taVal, RG_TA_LEN, ceBuf, ret);

         if(ret != ROK)
         {
            err->errCause = RGERR_MUX_BLD_CE_FAIL;
            DU_LOG("\nERROR  -->  MAC : Muxing of TA CE is failed");
            return RFAILED;
         }
         pdu->schdTbSz -= RG_TA_ELM_LEN;
         DU_LOG("\nDEBUG  -->  MAC : TA muxed by MAC: %u", pdu->ta.val);
      }
   }
#ifdef LTE_ADV
   if(TRUE == pdu->sCellActCe.pres)
   {
      if(pdu->schdTbSz >= RG_SCELL_CE_ELM_LEN)
      {
         /* Adding the subheader for ACT CE */
         RG_PACK_SHDR_FIXD_SZ(subHdr, RG_SCELL_LCID_IDX, ceBuf, ret);

         if(ret != ROK)
         {
            err->errCause = RGERR_MUX_BLD_CEHDR_FAIL;
            DU_LOG("\nERROR  -->  MAC : Muxing of SCELL Activation CE sub-hdr is failed")
            return RFAILED;
         }

         /* Adding the ACT CE */
         RG_PACK_CE(pdu->sCellActCe.val, RG_SCELL_ACT_CE_LEN, ceBuf, ret);

         if(ret != ROK)
         {
            err->errCause = RGERR_MUX_BLD_CE_FAIL;
            DU_LOG("\nERROR  -->  MAC : Muxing of SCELL Activation CE is failed")
            return RFAILED;
         }
         pdu->schdTbSz -= RG_SCELL_CE_ELM_LEN;

      }
   }
#endif
  
  /*LcId is not yet decided in 5G-NR spec for MAC CEs Hence, not writing code
   * new MAC CEs. */

   return ROK;
} /* rgMUXAddCes */

/**
 * @brief Function to insert SDU along with sub headers.
 *
 * @details
 *
 *     Function : rgMUXInsSdu
 *     
 *  @param[in]       Inst        inst
 *  @param[in]       MsgLen      *schdTbSz
 *  @param[in]       uint8_t          lcId
 *  @param[in]       Buffer      *sdu
 *  @param[out]      Buffer      *sduBuf 
 *  @param[out]      RgErrInfo   *err 
 *  @return    S16
 *      -# ROK 
 *      -# RFAILED
 **/
static S16 rgMUXInsSdu
(
Inst           inst,
MsgLen         *schdTbSz,
uint8_t        lcId,
Buffer         *sdu,
Buffer         *sduBuf,
RgErrInfo      *err
)
{
   S16            ret;
   MsgLen         msgLen = 0;
   uint8_t        lenBytes;
   MsgLen         elmTotLen;

   SFndLenMsg(sdu, &msgLen);

   RG_MUX_CALC_LEN(msgLen,lenBytes,elmTotLen);
   
   if (lcId == 3)
   {
     rgDlrate_rgu += msgLen; 
   }
   if (*schdTbSz >= elmTotLen)
   {
      RgMUXSubHdr    subHdr;
      RG_PACK_VAR_SZ_CE_SDU_SHDR(subHdr, lcId, msgLen,sduBuf, ret);
      if(ret != ROK)
      {
         err->errCause = RGERR_MUX_BLD_SDUHDR_FAIL;
         DU_LOG("\nERROR  -->  MAC : RGERR_MUX_BLD_SDUHDR_FAIL for LCID:%d",lcId);
         return RFAILED;
      }

#ifndef L2_OPTMZ /* no need to pack as passing not muxing all LCs PDUs to 1*/
      RG_PACK_SDU(sduBuf, sdu, ret);
#else
  //UDAY
      ret = ROK;
      UNUSED(sduBuf);
#endif

      if(ret != ROK)
      {
         err->errCause = RGERR_MUX_BLD_SDU_FAIL;
         DU_LOG("\nERROR  -->  MAC : RGERR_MUX_BLD_SDU_FAIL for LCID:%d",lcId);
         return RFAILED;
      }

      *schdTbSz -= elmTotLen;
   }
   else
   {
      /* This Sub-PDU can not be accodmodated at all */
      DU_LOG("\nERROR  -->  MAC : Failed lcId %u, elmTotLen %d lenBytes %d LCID:%d",
               lcId, ((S16)elmTotLen), lenBytes,lcId);
      DU_LOG("\nERROR  -->  MAC : msglen %d schdTbSz %d LCID:%d",
               ((S16)msgLen), ((S16)*schdTbSz),lcId);
      return RFAILED;
   }
   return ROK;
}

/**
 * @brief Function to insert SDU along with sub headers.
 *
 * @details
 *
 *     Function : rgMUXAddPadd
 *     
 *  @param[in]  Inst        inst
 *  @param[in]       RgBldPduInfo   *pdu
 *  @param[out]      Buffer        *mBuf 
 *  @param[out]      Buffer        *sduBuf 
 *  @return    S16
 *      -# ROK 
 *      -# RFAILED
 **/
#ifdef L2_OPTMZ
uint32_t padSize = 0;
#endif
S16 rgMUXAddPadd
(
Inst           inst,
MsgLen         *schdTbSz,
Buffer         *sduBuf,
Bool           isRar,
RgErrInfo      *err
)
{
   S16     ret = ROK;
   Buffer         *padBuf = NULLP;
   RgMUXSubHdr    subHdr;

#ifdef L2_OPTMZ
   padSize = 0;
#endif
   if(*schdTbSz)
   {
#ifndef L2_OPTMZ
      if(FALSE == isRar)
#else
      if((FALSE == isRar) && (NULL != sHdrBuf))
#endif

      {
         RG_PACK_SHDR_FIXD_SZ(subHdr, RG_PAD_LCID_IDX, sduBuf, ret);  

         if(ret != ROK)
         {
            err->errCause = RGERR_MUX_BLD_PADHDR_FAIL;
            DU_LOG("\nERROR  -->  MAC : RGERR_MUX_BLD_PADHDR_FAIL");
            return RFAILED;
         }

         *schdTbSz -= 1;
      }

      if (*schdTbSz)
      {
         if (*schdTbSz <= RG_MAX_PAD_ARR_SZ)
         {
#ifndef L2_OPTMZ
            RG_PACK_PAD(padBuf,*schdTbSz,sduBuf);
#else
            if(sduBuf)
            {
               RG_PACK_PAD(padBuf,*schdTbSz,sduBuf);
            }
            else
               padSize += *schdTbSz;
#endif
            if(ret != ROK)
            {
               err->errCause = RGERR_MUX_BLD_PAD_FAIL;
               DU_LOG("\nERROR  -->  MAC : RGERR_MUX_BLD_PAD_FAIL");
               return RFAILED;
            }
            *schdTbSz = 0;
         }
         else
         {
            while (*schdTbSz)
            {
               if (*schdTbSz > RG_MAX_PAD_ARR_SZ)
               {
#ifndef L2_OPTMZ
                  RG_PACK_PAD(padBuf,RG_MAX_PAD_ARR_SZ,sduBuf);
#else
                  if(sduBuf)
                  {
                     RG_PACK_PAD(padBuf,RG_MAX_PAD_ARR_SZ,sduBuf);
                  }
                  else
                     padSize += RG_MAX_PAD_ARR_SZ;
#endif

                  if(ret != ROK)
                  {
                     err->errCause = RGERR_MUX_BLD_PAD_FAIL;
                     DU_LOG("\nERROR  -->  MAC : RGERR_MUX_BLD_PAD_FAIL");
                     return RFAILED;
                  }

                  *schdTbSz -= RG_MAX_PAD_ARR_SZ;
               }
               else
               {
#ifndef L2_OPTMZ
                  RG_PACK_PAD(padBuf,*schdTbSz,sduBuf);
#else
                  if(sduBuf)
                  {
                     RG_PACK_PAD(padBuf,*schdTbSz,sduBuf);
                  }
                  else
                     padSize += *schdTbSz;
#endif

                  if(ret != ROK)
                  {
                     err->errCause = RGERR_MUX_BLD_PAD_FAIL;
                     DU_LOG("\nERROR  -->  MAC : RGERR_MUX_BLD_PAD_FAIL");
                     return RFAILED;

                  }
                  *schdTbSz = 0;
               }
            }
         }
      }
   }

   return ROK;
} /* rgMUXAddPadd */

#ifndef L2_OPTMZ
/**
 * @brief Function to add SDU along with sub headers.
 *
 * @details
 *
 *     Function : rgMUXAddSdus
 *     
 *  @param[in]  Inst        inst
 *  @param[in]       RgBldPduInfo   *pdu
 *  @param[out]      Buffer        *mBuf 
 *  @param[out]      Buffer        *sduBuf 
 *  @return    S16
 *      -# ROK 
 *      -# RFAILED
 **/
static S16 rgMUXAddSdus
(
Inst           inst,
RgBldPduInfo   *pdu,
Buffer         *sduBuf,
RgErrInfo      *err
)
{
   RgRguDDatReqPerUe *dDatReq;
   RgRguCmnDatReq    *cDatReq;

   switch(pdu->reqType)
   {
      case EVTRGUCDATREQ:
         cDatReq = (RgRguCmnDatReq *)(pdu->datReq);
         /* Add sdu(s) to the Message Buffer */
         if (NULLP != cDatReq)
         {
            if(rgMUXInsSdu(inst,&pdu->schdTbSz, 
                     RG_CCCH_LCID, cDatReq->pdu, sduBuf, err) != ROK)
            {
               return RFAILED;
            }
            RG_FREE_MSG(cDatReq->pdu);
         }
         break;

      case EVTRGUDDATREQ:
         dDatReq = (RgRguDDatReqPerUe *)(pdu->datReq);
         /* Add sdu(s) to the Message Buffer */
         if (NULLP != dDatReq)
         {
            if(pdu->tbIndex == 1)
            {
               uint16_t idx1, idx2;
               /* Adding this temporary variable for optimization */
               RguDatReqTb *datReqTb = &dDatReq->datReqTb[0];

               for (idx1=0; (idx1 < datReqTb->nmbLch); idx1++)
               {
                  for(idx2=0;
                     (idx2 < datReqTb->lchData[idx1].pdu.numPdu);
                     idx2++)
                  {
                     if(pdu->schdTbSz)
                     {
                        if(rgMUXInsSdu(inst,&pdu->schdTbSz,
                           datReqTb->lchData[idx1].lcId, 
                           datReqTb->lchData[idx1].pdu.mBuf[idx2],
                           sduBuf, err) != ROK)
                        {
                           DU_LOG("\nERROR  -->  MAC : FAILED for LCID:%d",datReqTb->lchData[idx1].lcId);
                           return RFAILED;
                        }
                     }
                     RG_FREE_MSG(datReqTb->lchData[idx1].pdu.mBuf[idx2]);
                  }
               }
            }
            else if(pdu->tbIndex == 2)
            {
               uint16_t idx1, idx2;
               RguDatReqTb *datReqTb = &dDatReq->datReqTb[1];
               for (idx1=0; (idx1 < datReqTb->nmbLch); idx1++)
               {
                  for(idx2=0;
                     (idx2 < datReqTb->lchData[idx1].pdu.numPdu);
                     idx2++)
                  {
                     if(pdu->schdTbSz)
                     {
                        if(rgMUXInsSdu(inst,&pdu->schdTbSz,
                           datReqTb->lchData[idx1].lcId, 
                           datReqTb->lchData[idx1].pdu.mBuf[idx2],
                           sduBuf, err) != ROK)
                        {
                           DU_LOG("\nERROR  -->  MAC : FAILED TB Size %d LCID:%d",
                                    ((S16)pdu->schdTbSz),datReqTb->lchData[idx1].lcId);
                           return RFAILED;
                        }
                     }
                     RG_FREE_MSG(datReqTb->lchData[idx1].pdu.mBuf[idx2]);
                  }
               }
            }
         }
         break;

      default:
         break;
   } /* End of switch(reqType) */
   if(rgMUXAddPadd(inst,&pdu->schdTbSz, sduBuf, FALSE, err) != ROK)
   {
      DU_LOG("\nERROR  -->  MAC : FAILED for TB Size:%d",(S16)pdu->schdTbSz);
      return RFAILED;
   }
   return ROK;
}

/**
 * @brief Function to create MAC PDU from RLC SDUs and control elements, if any. 
 *
 * @details
 *
 *     Function : rgMUXBldPdu
 *     
 *     -# This function shall be invoked by Downlink Harq Module as soon as a
 *        Data request is received from RLC for a UE along with its stored 
 *        control elements to create a MAC PDU.
 *     -# It shall create subheaders for the control elements (timing advance
 *        and contention resolution ID) and pack sub-header before each CE,
 *        if given, and then shall run through all the logical channels and
 *        create subheader for each of the SDUs given on that logical channel
 *        and pack corresponding sub-header before the each SDU 
 *     -# It shall invoke rgMUXPadPdu if the total length of the created 
 *        buffer is less than the scheduled TB size. 
 *     
 *           
 *  @param[in]  Inst           *inst
 *  @param[in]  RgBldPduInfo   *bldPdu
 *  @param[in]  Buffer         **txPdu
 *  @param[out] RgErrInfo      *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgMUXBldPdu
(
Inst           inst,
RgBldPduInfo   *pdu,
Buffer         **txPdu,
RgErrInfo      *err
)
{
   Buffer         *mBuf = NULLP;

   if (rgGetMsg(inst, &mBuf) != ROK)
   {
      /* Buffer couldnt get allocated. Return a failure */
      err->errCause = RGERR_MUX_MEM_ALLOC_FAIL;
      err->errType = RGERR_MUX_BLD_PDU;
      DU_LOG("\nERROR  -->  MAC : Memory allocation failed during MUXing of MAC TB: MacInst %d", inst);
      return RFAILED;
   }

   if(rgMUXAddCes(inst, pdu, mBuf, err) != ROK)
   {
      RG_FREE_MSG(mBuf);
      err->errType = RGERR_MUX_BLD_PDU;
      DU_LOG("\nERROR  -->  MAC : Failed to Multiplex MAC CEs: MacInst %d", inst);
      return RFAILED;
   }

   if(rgMUXAddSdus(inst, pdu, mBuf, err) != ROK)
   {
      RG_FREE_MSG(mBuf);
      err->errType = RGERR_MUX_BLD_PDU;
      DU_LOG("\nERROR  -->  MAC : FAILED to Multiplex MAC SDU: MacInst %d", inst);
      return RFAILED;
   }

   *txPdu = mBuf;

   return ROK;

}  /* rgMUXBldPdu */

#else /* else of ifndef L2_OPTMZ */

/**
 * @brief Function to add SDU along with sub headers.
 *
 * @details
 *
 *     Function : rgMUXAddSdus
 *     
 *  @param[in]       RgBldPduInfo   *pdu
 *  @param[out]      Buffer        *mBuf 
 *  @param[out]      Buffer        *sduBuf 
 *  @return    S16
 *      -# ROK 
 *      -# RFAILED
 **/
static S16 rgMUXAddSdus
(
Inst                inst,
RgBldPduInfo        *pdu,
Buffer              *sHdrBuf,
RgTfuDatReqTbInfo   *tb,
RgErrInfo           *err
)
{
   RgRguDDatReqPerUe  *dDatReq;
   RgRguCmnDatReq     *cDatReq;
   uint32_t lchIdx, pduIdx;

   switch(pdu->reqType)
   {
      case EVTRGUCDATREQ:
         cDatReq = (RgRguCmnDatReq *)(pdu->datReq);
         /* Add sdu(s) to the Message Buffer */
         if (NULLP != cDatReq)
         {
            if(rgMUXInsSdu(inst, &pdu->schdTbSz, 
                     RG_CCCH_LCID, cDatReq->pdu, 
                     sHdrBuf, NULLP, err) != ROK)
            {
               return RFAILED;
            }
            /* L2 Optimization for mUe/Tti: RLC pdu mbuf pointer will be passed
             * to CL it is stored in DlHqProc->TbInfo and it will be used in
             * case of harq retransmission. Store CCCH data at 0th index of
             * lch array*/
            tb->lchInfo[tb->numLch].mBuf[(tb->lchInfo[tb->numLch].numPdu)]\
               = cDatReq->pdu;
            tb->lchInfo[tb->numLch].numPdu++;
            tb->numLch++;
            DU_LOG("\nINFO   -->  MAC : MSG4 is muxed  numLch=%ld numPdu=%ld tbaddr =%p",\
	    tb->numLch,tb->lchInfo[tb->numLch-1].numPdu, (uint32_t)tb);
         }
         break;

      case EVTRGUDDATREQ:
         dDatReq = (RgRguDDatReqPerUe *)(pdu->datReq);
         /* Add sdu(s) to the Message Buffer */
         if (NULLP != dDatReq)
         {
            if(pdu->tbIndex == 1)
            {
               uint16_t idx1, idx2;
               /* Adding this temporary variable for optimization */
               RguDatReqTb *datReqTb = &dDatReq->datReqTb[0];
             
               tb->numLch = lchIdx = 0;

               for (idx1=0; (idx1 < datReqTb->nmbLch); idx1++)
               {
                  tb->lchInfo[lchIdx].numPdu = pduIdx = 0;

                  for(idx2=0;
                     (idx2 < datReqTb->lchData[idx1].pdu.numPdu);
                     idx2++)
                  {
                     if(pdu->schdTbSz)
                     {
                        if(rgMUXInsSdu(inst, &pdu->schdTbSz,
                           datReqTb->lchData[idx1].lcId, 
                           datReqTb->lchData[idx1].pdu.mBuf[idx2],
                           sHdrBuf, NULLP, err) != ROK)
                        {
                           DU_LOG("\nERROR   -->  MAC : Failed to insert SDU in rgMUXAddSdus\n");
                           return RFAILED;
                        }

                        /* L2 Optimization for mUe/Tti:Increment numPdu by 1
                         * Store pdu buffer in tb to send it to CL/PHY. Increment
                         * numPdu by 1*/
                        tb->lchInfo[lchIdx].mBuf[pduIdx] = datReqTb->lchData[idx1].pdu.mBuf[idx2];

#ifdef L2_OPTMZ
                        if(datReqTb->lchData[idx1].freeBuff == FALSE)
                        {/* Not incrementing refCnt for UM Mode. */
                           tb->lchInfo[lchIdx].mBuf[pduIdx]->refCnt++;
                        }
#endif
//UDAY 
                        if(NULL != datReqTb->lchData[idx1].pdu.mBuf[idx2]->b_cont)
                        {
                             Buffer *tmp;
                             tmp = datReqTb->lchData[idx1].pdu.mBuf[idx2]->b_cont;
                             if(NULL == tmp->b_rptr)
                             {
                                DU_LOG("\nERROR   -->  MAC : rgMUXAddSdus failed in EVTRGUDDATREQ");
                             }
                        }
                        else
                        {
                            DU_LOG("\nERROR   -->  MAC : rgMUXAddSdus failed in EVTRGUDDATREQ");
                        }
                        pduIdx++;
                        //tb->lchInfo[tb->numLch].numPdu++;
                        //tb->numLch++;

                    }
                  }
                  
                  if(pduIdx)
                  {
                    tb->lchInfo[lchIdx].numPdu = pduIdx;
                    /* If Bearer is UM then MBUF to be free by MAC layer */
                    tb->lchInfo[lchIdx].freeBuff = datReqTb->lchData[idx1].freeBuff;
                    lchIdx++;
                  }
               }
               tb->numLch = lchIdx;
            }
            else if(pdu->tbIndex == 2)
            {
               uint16_t idx1, idx2;
               RguDatReqTb *datReqTb = &dDatReq->datReqTb[1];
               tb->numLch = lchIdx = 0;
         //      prc_trace_format_string(0x40,3,": AddSdus: numOfLch=%d numOfPdu=%d, schdSz=%d", datReqTb->nmbLch, datReqTb->lchData[0].pdu.numPdu, pdu->schdTbSz);
              for (idx1=0; (idx1 < datReqTb->nmbLch); idx1++)
               {
                 tb->lchInfo[lchIdx].numPdu = pduIdx = 0;
                 for(idx2=0;
                     (idx2 < datReqTb->lchData[idx1].pdu.numPdu);
                     idx2++)
                  {
                     if(pdu->schdTbSz)
                     {
                        if(rgMUXInsSdu(inst, &pdu->schdTbSz,
                           datReqTb->lchData[idx1].lcId, 
                           datReqTb->lchData[idx1].pdu.mBuf[idx2],
                           sHdrBuf, NULLP, err) != ROK)
                        {
                           DU_LOG("\nERROR  -->  MAC : FAILED TB Size %d\n",
                                    ((S16)pdu->schdTbSz));
                           return RFAILED;
                        }
                         /* L2 Optimization for mUe/Tti:Increment numPdu by 1
                         * Store pdu buffer in tb to send it to CL/PHY. Increment
                         * numPdu by 1*/
                        tb->lchInfo[lchIdx].mBuf[pduIdx] = datReqTb->lchData[idx1].pdu.mBuf[idx2];
#ifdef L2_OPTMZ
                        if(datReqTb->lchData[idx1].freeBuff == FALSE)
                        {/* Not incrementing refCnt for UM Mode. */
                           tb->lchInfo[lchIdx].mBuf[pduIdx]->refCnt++;
                        }
#endif
                        if(NULL != datReqTb->lchData[idx1].pdu.mBuf[idx2]->b_cont)
                        {
                             Buffer *tmp;
                             tmp = datReqTb->lchData[idx1].pdu.mBuf[idx2]->b_cont;
                             if(NULL == tmp->b_rptr)
                             {
                                DU_LOG("\nERROR   -->  MAC : rgMUXAddSdus failed in EVTRGUDDATREQ");
                             }
                        }
                        else
                        {
                            DU_LOG("\nERROR   -->  MAC : rgMUXAddSdus failed in EVTRGUDDATREQ");
                        }
                        pduIdx++;
                       // tb->lchInfo[tb->numLch].numPdu++;
                       // tb->numLch++;

                     }
                  }

                  if(pduIdx)
                  {
                    tb->lchInfo[lchIdx].numPdu = pduIdx;
                    /* If Bearer is UM then MBUF to be free by MAC layer */
                    tb->lchInfo[lchIdx].freeBuff = datReqTb->lchData[idx1].freeBuff;
                    lchIdx++;
                  }
               }

               tb->numLch = lchIdx;
           }
         }
         break;

      case EVENT_SLOT_IND_TO_MAC:
         break;
      default:
         break;
   } /* End of switch(reqType) */

   
   if(rgMUXAddPadd(inst, &pdu->schdTbSz, sduBuf, NULLP, FALSE, err) != ROK)
   {
      //RGDBGERRNEW((rgPBuf, "FAILED"));
      return RFAILED;
   }
   tb->padSize = padSize;

   return ROK;
}

/**
 * @brief Function to create MAC PDU from RLC SDUs and control elements, if any. 
 *
 * @details
 *
 *     Function : rgMUXBldPdu
 *     -# This function shall be invoked by Downlink Harq Module as soon as a
 *        Data request is received from RLC for a UE along with its stored 
 *        control elements to create a MAC PDU.
 *     -# It shall create subheaders for the control elements (timing advance
 *        and contention resolution ID), if given, and then shall run through
 *        all the logical channels and create subheader for each of the SDUs
 *        given on that logical channel.
 *     -# L2 Optimization for mUe/Tti: Avoiding muxing to reduce overhead of 
 *           additional Mbuf allocation memory related operation.
       -# MAC header, MAC CEs, MAC PDUs and MAC padding are stored in pre-
            allocated mBufs. These pointers will not be freed by CL    
 *     -# It shall invoke rgMUXPadPdu if the total length of the created 
 *        buffer is less than the scheduled TB size.
 *     -# At successfull operation of this function tb->macHdr, will have
 *        complete MAC Header. tb->macCes will have MAC CEs if any. tb->
 *        lchInfo[idx].mBuf[idx] will have MAC SDU per LCH per TB per UE 
 *     
 *           
 *  @param[in]  RgBldPduInfo   *bldPdu
 *  @param[out] RgTbInfo       *tb
 *  @param[out] RgErrInfo      *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgMUXBldPdu
(
Inst               inst,
RgBldPduInfo       *pdu,
RgTfuDatReqTbInfo  *tb,
RgErrInfo          *err
)
{
   Buffer         *mBuf1; /* MAC hearder */
   Buffer         *mBuf2; /* MAC CEs */
   //uint32_t            lchIdx, pduIdx;

  /* Reseting macHdr and macCes pointers */
  if(tb->macHdr)
   SResetMBuf(tb->macHdr);
   if(tb->macCes)
   SResetMBuf(tb->macCes);
   
   mBuf1 = tb->macHdr; /* MAC hearder */
   mBuf2 = tb->macCes; /* MAC CEs */
   tb->tbSize  = pdu->schdTbSz;

   if(rgMUXAddCes(inst, pdu, mBuf1, mBuf2, err) != ROK)
   {
      /* Reset rPtr and wPtr to the base of data buffer(db_base)*/
      DU_LOG("\nERROR  -->  MAC : rgMUXBldPdu: rgMUXAddCes is Failed \n");
      RG_FREE_TB(tb);
      err->errType = RGERR_MUX_BLD_PDU;
      //RGDBGERRNEW((rgPBuf, "FAILED"));
      return RFAILED;
   }
   if(rgMUXAddSdus(inst, pdu, mBuf1, tb, err) != ROK)
   {
      /*TODO:MP Reset rPtr and wPtr to the base of data buffer(db_base)
       * Reset numLch and numPdu to zero and set MAC SDU buf to NULLP */
      DU_LOG("\nERROR  -->  MAC : rgMUXBldPdu: rgMUXAddSdus is Failed \n");
      RG_FREE_TB(tb);

      err->errType = RGERR_MUX_BLD_PDU;
      //RGDBGERRNEW((rgPBuf, "FAILED"));
      return RFAILED;
   }
// UDAY 
//      SPrntMsg(tb->macHdr, 0, 0);
//   prc_trace_format_string(0x40,3,": padSize=%ld", tb->padSize);

   tb->tbPres = TRUE;
   return ROK;

}  /* rgMUXBldPdu */

#endif /* end of L2_OPTMZ */

/**
 * @brief Function to create RAR PDU. 
 *
 * @details
 *
 *     Function : rgMUXBldRarPdu
 *                This function is used to build RAR PDUs and is being 
 *                invoked by the scheduler.
 *     
 *  @param[out]RgCellCb       *cellCb
 *  @param[in] RgRaRspAlloc   *bldPdu
 *  @param[in] Buffer         **txPdu 
 *  @param[out] RgErrInfo     *err
 *  @return    S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgMUXBldRarPdu
(
RgCellCb        *cell,
RgInfRaRntiInfo *alloc,
Buffer          **txPdu,
RgErrInfo       *err
)
{
   Buffer      *datBuf = NULLP;
   S16         ret; 
   uint8_t     data[RG_RAR_ELEM_LEN];
   uint8_t     hdrByte;
   MsgLen      schdTbSz;
   uint8_t     idx;
   Inst        inst = cell->macInst - RG_INST_START;

   schdTbSz = alloc->schdTbSz;
   /* RAR PDU Requirements */
   /*
   1. SubHeader - R/T/RAPID. //5GNR, changed E to R
   2. TA ( if applicable)
   3. Ul Grant:
         a. Hopping Flag - 1 Bit.
         b. Fixed Size RB Assignment. - 10 Bits.
         c. Truncated Modulation and coding scheme - 4 Bits.
         d. TPC command for scheduled PUSCH. - 3 Bits.
         e. UL Delay - 1 Bit.
         f. CQI Request - 1 Bit.
   4. Temporary Crnti.
   */

   /* Initialize the error type */
   err->errType = RGERR_MUX_BLD_RAR_PDU;
  
   if ((ret = rgGetMsg(inst,&datBuf)) != ROK)
   {
      /* Buffer couldnt get allocated. Return a failure */
      err->errCause = RGERR_MUX_MEM_ALLOC_FAIL;
      DU_LOG("\nERROR  -->  MAC : FAILED to getMsg");
      return RFAILED;
   }

   if (TRUE == alloc->backOffInd.pres)
   {
      /*Set T Bit , NO E bit Required */
      hdrByte = 0x00;
      /* Add the bi */
      hdrByte |= (0x0F & (alloc->backOffInd.val));

      /* Add the header */
      RG_PACK_RAR_SHDR(hdrByte, datBuf, ret);
      if(ret != ROK)
      {
         err->errCause = RGERR_MUX_BLD_BI_FAIL;
         DU_LOG("\nERROR  -->  MAC : RGERR_MUX_BLD_BI_FAIL");
         RG_FREE_MSG(datBuf);
         return RFAILED;
      }
      schdTbSz--;
   }

   for (idx=0; idx < (alloc->numCrnti) && 
         (schdTbSz >= RG_RAR_ELEM_LEN+RG_RAR_SHDR_LEN); idx++)
   {
      /* Add the tBit */
      hdrByte = 0x40; 
      /* Add the rapId */
      hdrByte |= (0x3F & (alloc->crntiInfo[idx].rapId));

         /* Add the header */
         RG_PACK_RAR_SHDR(hdrByte, datBuf, ret);
         if(ret != ROK)
         {
            err->errCause = RGERR_MUX_BLD_RAPIDHDR_FAIL;
            DU_LOG("\nERROR  -->  MAC : RGERR_MUX_BLD_RAPIDHDR_FAIL");
            RG_FREE_MSG(datBuf);
            return RFAILED;
         }

      /* Prepare the data */
      data[0]  =  0x7F & ((alloc->crntiInfo[idx].ta.val) >> 4);
      data[1] = 0;
      data[2] = 0;
      data[3] = 0;
      {    
         rgMUXGet20bitRarGrnt(cell->bwCfg.ulTotalBw, &(alloc->crntiInfo[idx].grnt), &data[1]);
      }
      data[1] |=  ((uint8_t)((alloc->crntiInfo[idx].ta.val) << 4));
      data[4]  =  (alloc->crntiInfo[idx].tmpCrnti) >> 8;
      data[5]  =  (uint8_t) (alloc->crntiInfo[idx].tmpCrnti);

      DU_LOG("\nDEBUG  -->  MAC : Rar,Rapid=%d, Temp CRNTI:%d", 
                alloc->crntiInfo[idx].rapId,
                alloc->crntiInfo[idx].tmpCrnti);
     MS_BUF_ADD_ALLOC_CALLER();
      if(SAddPstMsgMult(&data[0], RG_RAR_ELEM_LEN, datBuf) != ROK)
      {
         err->errCause = RGERR_MUX_BLD_RAPID_FAIL;
         DU_LOG("\nERROR  -->  MAC : RGERR_MUX_BLD_RAPID_FAIL");
         RG_FREE_MSG(datBuf);
         return RFAILED;
      }
      schdTbSz -= RG_RAR_ELEM_LEN+RG_RAR_SHDR_LEN;
   }

   if(rgMUXAddPadd(inst,&schdTbSz, datBuf, TRUE, err) != ROK)
   {
      RG_FREE_MSG(datBuf);
      DU_LOG("\nERROR  -->  MAC : FAILED to mux add padding");
      return RFAILED;
   }

   *txPdu = datBuf;

   return ROK;
} /* rgMUXBldRarPdu */

/***********************************************************
 *
 *     Func : rgMUXGet20bitRarGrnt
 *
 *     Desc : This function fills up the 20-bit grant
 *            for RA response.
 *
 *     Ret  : None.
 *
 *     Notes: None.
 *
 *     File : rg_mux.c
 *
 **********************************************************/
static Void rgMUXGet20bitRarGrnt
(
uint8_t             ulBw,
RgInfRarUlGrnt *msg3Grnt,
uint8_t             *grnt
)
{
   uint16_t       riv = rgMUXCalcRiv(ulBw, msg3Grnt->rbStart, msg3Grnt->numRb);

   grnt[2]  = msg3Grnt->cqiBit;   /* cqi bit is 0, output from sched */
   grnt[2] |= (msg3Grnt->delayBit << 1);
   grnt[2] |= (msg3Grnt->tpc << 2);
   grnt[2] |= (msg3Grnt->iMcsCrnt << 5);

   grnt[1]  = (msg3Grnt->iMcsCrnt >> 3);
   /* Forcing right shift to insert 0 as the LSB: 
    * since this is assumed in the computation */
   grnt[1] |= (uint8_t)((riv << 1) & 0xFE);

   grnt[0]  = (uint8_t)((riv >> 7) & 0x07);
   grnt[0] |= ((msg3Grnt->hop & 0x01) << 3);

   return;
} /* rgMUXGet20bitRarGrnt */

/***********************************************************
 *
 *     Func : rgMUXCalcRiv
 *
 *     Desc : This function calculates RIV.
 *
 *     Ret  : None.
 *
 *     Notes: None.
 *
 *     File : rg_mux.c
 *
 **********************************************************/
uint16_t rgMUXCalcRiv
(
uint8_t           bw,
uint8_t           rbStart,
uint8_t           numRb
)
{
   uint8_t           numRbMinus1 = numRb - 1;
   uint16_t          riv;

   if (numRbMinus1 <= bw/2)
   {
      riv = bw * numRbMinus1 + rbStart;
   }
   else
   {
      riv = bw * (bw - numRbMinus1) + (bw - rbStart - 1);
   }
   return (riv);
} /* rgMUXCalcRiv */



/**********************************************************************
 
         End of file
**********************************************************************/
